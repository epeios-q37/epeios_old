/*
	Test source for the 'mscmdx' library by Claude SIMON (csimon at zeusw dot org).
	Copyright (C) 2004 Claude SIMON.

	This file is part of the Epeios (http://zeusw.org/epeios/) project.

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
           59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

//	$Id$

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mscmdx.h"

#include "err.h"
#include "cio.h"
#include "flf.h"
#include "xtf.h"

using cio::cin;
using cio::cout;
using cio::cerr;

void Generic( int argc, char *argv[] )
{
ERRProlog
ERRBegin
ERRErr
ERREnd
ERREpilog
}

void Read( void )
{
ERRProlog
	flf::file_iflow___ IFlow;
	xml::writer Writer;
ERRBegin
	Writer.Init( cio::cout );

	IFlow.Init( "H:\\cvs\\pmsq\\MIDI\\OMS\\OMS010.mid" );

	mscmdx::MIDIToXMID( IFlow, mscmdm::oFile, Writer );
ERRErr
ERREnd
ERREpilog
}

void Write( void )
{
ERRProlog
	flf::file_iflow___ IFlow;
	xtf::extended_text_iflow__ XFlow;
	flf::file_oflow___ OFlow;
ERRBegin
	IFlow.Init( "Test.xmid" );
	XFlow.Init( IFlow );

	OFlow.Init( "test.mid" );

	mscmdx::XMIDToMIDI( XFlow, OFlow );
ERRErr
ERREnd
ERREpilog
}

int main( int argc, char *argv[] )
{
ERRFProlog
ERRFBegin
	cout << "Test of library " << MSCMDXTutor.Name << ' ' << __DATE__" "__TIME__"\n";

	switch( argc ) {
	case 1:
		Generic( argc, argv );
//		Read();
		Write();
		break;
	case 2:
		if ( !strcmp( argv[1], "/i" ) )
		{
			TTR.Advertise( cout );
			break;
		}
	default:
		cout << txf::sync;
		cerr << "\nBad arguments.\n";
		cout << "Usage: " << MSCMDXTutor.Name << " [/i]\n\n";
		ERRi();
	}

ERRFErr
ERRFEnd
ERRFEpilog
	return ERRExitValue;
}
