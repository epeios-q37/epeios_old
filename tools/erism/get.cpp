/*
	'get' module by Claude SIMON (http://zeusw.org/epeios/contact.html).
	Part of the 'erism' tool.
	Copyright (C) 2011 by Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of the Epeios project (http://zeusw.org/epeios/).

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 3
	of the License, or (at your option) any later version.
 
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, go to http://www.fsf.org/
	or write to the:
  
         	         Free Software Foundation, Inc.,
           59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.*/
// $Id$

#include "get.h"

#include "common.h"
#include "identity.h"

#include "mscmdd.h"

using namespace get;
using namespace common;

static void Get_(
	flw::ioflow__ &Flow,
	const mscrmi::blocs_ &Blocs,
	const mscrmi::identity__ &Identity,
	mscrmi::adata_set_ &DataSet )
{
	mscrmi::transmission_status__ Status = mscrmi::ts_Undefined;

	if ( ( Status = mscrmi::Retrieve( Flow, Blocs, Identity, DataSet ) ) != mscrmi::tsOK )
		Report( eTransmissionError, &Status );
}

static void Get_(
	int DIn,
	int DOut,
	const mscrmi::blocs_ &Blocs,
	const mscrmi::identity__ &Identity,
	mscrmi::adata_set_ &DataSet )
{
ERRProlog
	mscmdd::midi_ioflow___ Flow;
ERRBegin
	Get_( common::OpenDevices( DIn, DOut, Flow ), Blocs, Identity, DataSet );
ERRErr
ERREnd
ERREpilog
}

static void ReadSettings_(
	mscrmi::device_id__ DeviceID,
	int DIn,
	int DOut,
	const mscrmi::midi_implementation_ &Implementation,
	txf::text_oflow__ &TFlow )
{
ERRProlog
	mscrmi::blocs Blocs;
	mscrmi::identity__ Identity;
	mscrmi::adata_set DataSet;
	mscrmi::settings Settings;
	xml::writer Writer;
ERRBegin
	Display( mDeviceModel, &Implementation.ModelLabel );
	cout << txf::nl << txf::commit;

	Blocs.Init();

	mscrmi::GetBlocs( Implementation, Blocs );

	common::GetIdentity( DeviceID, Implementation, Identity );

	DataSet.Init();
	Get_( DIn, DOut, Blocs, Identity, DataSet );

	Settings.Init();
	mscrmi::Fill( DataSet, Implementation, Settings );

	Writer.Init( TFlow, xml::oIndent, xml::encoding__() );

	mscrmi::Print( Settings, Implementation, Writer );
ERRErr
ERREnd
ERREpilog
}

static void ReadSettings_(
	mscrmi::device_id__ DeviceID,
	int DIn,
	int DOut,
	txf::text_oflow__ &TFlow )
{
ERRProlog
	mscrmi::midi_implementation Implementation;
ERRBegin
	Implementation.Init();

	ReadSettings_( DeviceID, DIn, DOut, common::GetImplementation( DeviceID, DIn, DOut, Implementation ), TFlow );
ERRErr
ERREnd
ERREpilog
}

void get::ReadSettings(
	const char *RawDeviceID,
	const char *RawDIn,
	const char *RawDOut,
	const char *RawOutput )
{
ERRProlog
	int DIn, DOut;
	mscrmi::device_id__ DeviceID = MSCRMI_UNDEFINED_DEVICE_ID;
	epeios::row__ Error = NONE;
	flf::file_oflow___ OFlow;
	txf::text_oflow__ TFlow;
	bso::bool__ BackedUp = false;
ERRBegin
	common::Normalize( RawDeviceID, RawDIn, RawDOut, DeviceID, DIn, DOut );

	if ( RawOutput != NULL ) {
		CreateBackupFile( RawOutput );
		BackedUp = true;

		if ( OFlow.Init( RawOutput ) != fil::sSuccess )
			Report( eUnableToOpenFile, RawOutput );

		TFlow.Init( OFlow );

		ReadSettings_( DeviceID, DIn, DOut, TFlow );
	} else
		ReadSettings_( DeviceID, DIn, DOut, cout );
ERRErr
	if ( BackedUp )
		RecoverBackupFile( RawOutput );
ERREnd
ERREpilog
}
