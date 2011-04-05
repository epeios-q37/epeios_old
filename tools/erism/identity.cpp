/*
	'identity' module by Claude SIMON (http://zeusw.org/epeios/contact.html).
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


#include "identity.h"
#include "common.h"

#include "mscmdd.h"
#include "mscrmi.h"

using namespace identity;

static void Display_( const mscrmi::midi_implementation_ &Implementation )
{
	common::cout << Implementation.ModelLabel;
}

static void Display_(
	const mscrmi::midi_implementations_ &Implementations,
	epeios::row__ Row )
{
	ctn::E_CITEM( mscrmi::midi_implementation_ ) Implementation;

	Implementation.Init( Implementations );

	Display_( Implementation( Row ) );
}

void Identify_(
	mscrmi::device_id__ DeviceID,
	int DIn,
	int DOut )
{
ERRProlog
	mscrmi::midi_implementations Implementations;
	epeios::row__ Row = NONE;
	mscrmi::device_family__ DeviceFamily;
	mscrmi::software_revision__ SoftwareRevision;
	str::string Buffer;
ERRBegin
	Implementations.Init();

	common::GetImplementations( Implementations );

	Display_( Implementations, common::Identify( DeviceID, DIn, DOut, Implementations, DeviceFamily, SoftwareRevision ) );

	Buffer.Init();
	common::cout << " (" << mscrmi::ToString( DeviceFamily, Buffer ) << ';';
	
	Buffer.Init();
	common::cout << mscrmi::ToString( SoftwareRevision, Buffer ) << ')';

	common::cout << txf::nl;
ERRErr
ERREnd
ERREpilog
}

void identity::Identify(
	const char *RawDeviceID,
	const char *RawDIn,
	const char *RawDOut )
{
	int DIn, DOut;
	mscrmi::device_id__ DeviceID = MSCRMI_UNDEFINED_DEVICE_ID;
	epeios::row__ Error = NONE;

	common::Normalize( RawDeviceID, RawDIn, RawDOut, DeviceID, DIn, DOut );

	Identify_( DeviceID, DIn, DOut );
}

