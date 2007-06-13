/*
	Test source for the 'mmm2' library by Claude SIMON (csimon@epeios.org).
	Copyright (C) 2004 Claude SIMON (csimon@epeios.org).

	This file is part of the Epeios (http://epeios.org/) project.

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
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

#include "mmm2.h"

#include "err.h"
#include "cio.h"
#include "mmi.h"
#include "str.h"

using cio::cin;
using cio::cout;
using cio::cerr;

using namespace mmm;

#define D	Multimemory.DisplayStructure( cio::cout );cio::cout << "-------------------" << txf::nl;

void Generic( int argc, char *argv[] )
{
ERRProlog
	multimemory Multimemory;
	str::string S1;
	str::string S2;
ERRBegin
	Multimemory.Init();

	S1.plug( Multimemory );
	S1.Init();
	S1.SetStepValue( 0 );

	S2.plug( Multimemory );
	S2.Init();
	S2.SetStepValue( 0 );

	D;

	S1.Append( "TOTO" );
	D;

	S2.Append( "sfertyrjhgjhgjhgjtgf" );
	D;

	S1.Append( "turlututu" );
	D;
ERRErr
ERREnd
ERREpilog
}

int main( int argc, char *argv[] )
{
ERRFProlog
ERRFBegin
	cout << "Test of library " << MMM2Tutor.Name << ' ' << __DATE__" "__TIME__"\n";

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
		cout << "Usage: " << MMM2Tutor.Name << " [/i]\n\n";
		ERRi();
	}

ERRFErr
ERRFEnd
ERRFEpilog
	return ERRExitValue;
}
