/*
	Test source for the 'bfl' library by Claude SIMON (csimon@epeios.org).
	Copyright (C) 2004 Claude SIMON (csimon@epeios.org).

	This file is part of the Epeios (http://epeios.org/) project.

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

#include "bfl.h"

#include "err.h"
#include "cio.h"
#include "flx.h"
#include "str.h"

using cio::cin;
using cio::cout;
using cio::cerr;

#define AMOUNT	800

void Write( str::string_ &Buffer )
{
ERRProlog
	flx::E_STRING_OFLOW___ Flow;
	bfl::bit_oflow__ BFlow( Flow );
	bso::bool__ Value;
	int Counter = AMOUNT;
ERRBegin
	Flow.Init( Buffer );

	while ( Counter-- )	{
		Value = rand() % 2 ? true : false;

		cout << (bso::ulong__)( Value ? 1 : 0 );

		BFlow.Put( Value );
	}

	cout << txf::nl;
	
ERRErr
ERREnd
ERREpilog
}

void Read( const str::string_ &Buffer )
{
ERRProlog
	flx::E_STRING_IFLOW__ Flow;
	bfl::bit_iflow__ BFlow( Flow );
	int Counter = AMOUNT;
ERRBegin
	Flow.Init( Buffer );

	while ( Counter-- )
		cout << (bso::ulong__)BFlow.Get() << txf::sync;

	cout << txf::nl <<txf::sync;
	
ERRErr
ERREnd
ERREpilog
}

void Generic( int argc, char *argv[] )
{
ERRProlog
	str::string Buffer;
ERRBegin
	Buffer.Init();

	Write( Buffer );
	cout << Buffer.Amount() << txf::nl << txf::sync;
	Read( Buffer );
ERRErr
ERREnd
ERREpilog
}

int main( int argc, char *argv[] )
{
ERRFProlog
ERRFBegin
	cout << "Test of library " << BFLTutor.Name << ' ' << __DATE__" "__TIME__"\n";

	switch( argc ) {
	case 1:
		Generic( argc, argv );
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
		cout << "Usage: " << BFLTutor.Name << " [/i]\n\n";
		ERRi();
	}

ERRFErr
ERRFEnd
ERRFEpilog
	return ERRExitValue;
}
