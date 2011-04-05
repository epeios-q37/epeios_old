/*
	'set' module by Claude SIMON (http://zeusw.org/epeios/contact.html).
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

#include "set.h"

#include "common.h"
#include "identity.h"

#include "mscmdd.h"

using namespace set;
using namespace common;

static void GetSettings_(
	const char *SettingsFileName,
	const mscrmi::midi_implementation_ &Implementation,
	mscrmi::adata_set_ &DataSet )
{
ERRProlog
	flf::file_iflow___ Flow;
	xpp::preprocessing_iflow___ PFlow;
	xtf::extended_text_iflow__ XFlow;
	xml::parser___ Parser;
ERRBegin
	if ( Flow.Init( SettingsFileName ) != fil::sSuccess )
		Report( eUnableToOpenFile, SettingsFileName );

	PFlow.Init( Flow, str::string(), NULL );

	XFlow.Init( PFlow );

	Parser.Init( XFlow, xml::eh_Default );

	mscrmi::Parse( Parser, Implementation, DataSet );
ERRErr
ERREnd
ERREpilog
}

static void WriteSettings_(
	mscrmi::device_id__ DeviceID,
	flw::ioflow__ &Flow,
	const char *SettingsFileName )
{
ERRProlog
	mscrmi::midi_implementation Implementation;
	mscrmi::adata_set DataSet;
	mscrmi::identity__ Identity;
ERRBegin
	Implementation.Init();
	common::GetImplementation( DeviceID, Flow, Implementation );

	DataSet.Init();
	GetSettings_( SettingsFileName, Implementation, DataSet );

	common::GetIdentity( DeviceID, Implementation, Identity );

	mscrmi::Send( DataSet, Identity, Flow );
ERRErr
ERREnd
ERREpilog
}

static void WriteSettings_(
	mscrmi::device_id__ DeviceID,
	int DIn,
	int DOut,
	const char *SettingsFileName )
{
ERRProlog
	mscmdd::midi_ioflow___ Flow;
ERRBegin
	WriteSettings_( DeviceID, common::OpenDevices( DIn, DOut, Flow ), SettingsFileName );
ERRErr
ERREnd
ERREpilog
}


void set::WriteSettings(
	const char *RawDeviceID,
	const char *RawDIn,
	const char *RawDOut,
	const char *RawInput )
{
	int DIn, DOut;
	mscrmi::device_id__ DeviceID = MSCRMI_UNDEFINED_DEVICE_ID;
	epeios::row__ Error = NONE;

	common::Normalize( RawDeviceID, RawDIn, RawDOut, DeviceID, DIn, DOut );

	if ( ( RawInput == NULL ) || ( *RawInput == 0 ) ) 
		Report( eMissingSettingsFileName );

	WriteSettings_( DeviceID, DIn, DOut, RawInput );
}

/**/

static inline void SplitRange_(
	const str::string_ &Range,
	str::base__ Base,
	bso::ulong__ &Min,
	bso::ulong__ &Max,
	common::error__ ErrorValue )
{
ERRProlog
	epeios::row__ Error = NONE;
	epeios::row__ Position = NONE;
ERRBegin
	Min = Range.ToUL( &Error, Base );

	if ( Error != NONE ) {

		if ( Range( Error ) != '-' )
			common::Report( ErrorValue );

		Position = Range.Next( Error );

		if ( Position == NONE )
			common::Report( ErrorValue );

		Error = NONE;

		Max = Range.ToUL( Position, &Error, Base );

		if ( Error != NONE )
			common::Report( ErrorValue );
	} else
		Max = Min;

	if ( Min > Max )
		common::Report( ErrorValue );
ERRErr
ERREnd
ERREpilog
}

static inline void SplitRange_(
	const char *Range,
	str::base__ Base,
	bso::ulong__ &Min,
	bso::ulong__ &Max,
	common::error__ ErrorValue )
{
	SplitRange_( str::string( Range ), Base, Min, Max, ErrorValue );
}

static inline void SplitAddressRange_(
	const char *Range,
	bso::ulong__ &Min,
	bso::ulong__ &Max )
{
	SplitRange_( Range, str::b16, Min, Max, common::eBadAddressRange ); 
}

static inline void SplitValueRange_(
	const char *Range,
	bso::ulong__ &Min,
	bso::ulong__ &Max )
{
	SplitRange_( Range, str::b10, Min, Max, common::eBadValueRange ); 
}

inline mscrmi::size__ Set_( 
	const mscrmi::parameter_ &Parameter,
	bso::ulong__ VMin,
	bso::ulong__ VMax,
	mscrmi::adata_ &Data )
{
	mscrmi::size__ Size = Parameter.Size();

	if ( !mscrmi::Append( VMin + rand() % ( VMax - VMin + 1 ), Size, Data ) )
		Report( eBadValueRange );

	return Size;
}

mscrmi::size__ Set_( 
	mscrmi::row__ Row,
	bso::ulong__ VMin,
	bso::ulong__ VMax,
	const mscrmi::parameters_ &Parameters,
	mscrmi::adata_ &Data )
{
	ctn::E_CMITEMt( mscrmi::parameter_, mscrmi::row__ ) Parameter;

	Parameter.Init( Parameters );

	return Set_( Parameter( Row ), VMin, VMax, Data );
}

static void SetData_( 
	bso::ulong__ AMin,
	bso::ulong__ AMax,
	bso::ulong__ VMin,
	bso::ulong__ VMax,
	const mscrmi::parameters_ &Parameters,
	mscrmi::adata_ &Data )
{
	mscrmi::row__ Row = NONE;
	mscrmi::address__ &Address = AMin;

	Data.Address() = AMin;

	while ( Address <= AMax ) {
		Row = mscrmi::Search( Address, Row, Parameters );

		if ( Row != NONE )
			Address = mscrmi::Sum( Address, Set_( Row, VMin, VMax, Parameters, Data ) );
		else
			Report( eUnableToFindParameterForAddress, Address );
	}
}

static void Randomize_(
	mscrmi::device_id__ DeviceID,
	flw::ioflow__ &Flow,
	bso::ulong__ AMin,
	bso::ulong__ AMax,
	bso::ulong__ VMin,
	bso::ulong__ VMax )
{
ERRProlog
	mscrmi::midi_implementation Implementation;
	mscrmi::adata Data;
	mscrmi::identity__ Identity;
ERRBegin
	Implementation.Init();
	common::GetImplementation( DeviceID, Flow, Implementation );

	common::GetIdentity( DeviceID, Implementation, Identity );

	Data.Init();
	SetData_( AMin, AMax, VMin, VMax, Implementation.Parameters, Data );

	mscrmi::Send( Data, Identity, Flow );
ERRErr
ERREnd
ERREpilog
}

static void Randomize_(
	mscrmi::device_id__ DeviceID,
	int DIn,
	int DOut,
	bso::ulong__ AMin,
	bso::ulong__ AMax,
	bso::ulong__ VMin,
	bso::ulong__ VMax )
{
ERRProlog
	mscmdd::midi_ioflow___ Flow;
ERRBegin
	common::OpenDevices( DIn, DOut, Flow );

	Randomize_( DeviceID, Flow, AMin, AMax, VMin, VMax );
ERRErr
ERREnd
ERREpilog
}

void set::Randomize(
	const char *RawDeviceID,
	const char *RawDIn,
	const char *RawDOut,
	const char *AddressRange,
	const char *ValueRange )
{
	int DIn, DOut;
	mscrmi::device_id__ DeviceID = MSCRMI_UNDEFINED_DEVICE_ID;
	epeios::row__ Error = NONE;
	bso::ulong__ AMin, AMax, VMin, VMax;

	common::Normalize( RawDeviceID, RawDIn, RawDOut, DeviceID, DIn, DOut );

	SplitAddressRange_( AddressRange, AMin, AMax );
	SplitValueRange_( ValueRange, VMin, VMax );

	Randomize_( DeviceID, DIn, DOut, AMin, AMax, VMin, VMax );
}
