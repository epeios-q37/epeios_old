/*
	Test source for the 'ltf' library by Claude SIMON (csimon@epeios.org).
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

#include "ltf.h"

#include "err.h"
#include "cio.h"
#include "tol.h"

using cio::cin;
using cio::cout;
using cio::cerr;

void Generic( int argc, char *argv[] )
{
ERRProlog
	ltf::line_text_flow__<> LC( cout );
ERRBegin
	while ( 1 )
		if ( rand() % 60 ) {
			LC << (char)( rand() % 26 + 'A' );

			if ( ( rand() % 5 ) == 0 )
				LC << txf::sync;

			tol::Suspend( 100 );
		} else if ( rand() % 3 ) {
			LC.Clear();
		} else
			LC.CR();


ERRErr
ERREnd
ERREpilog
}

int main( int argc, char *argv[] )
{
	int ExitCode = EXIT_SUCCESS;
ERRFProlog
ERRFBegin
	cout << "Test of library " << LTFTutor.Name << ' ' << __DATE__" "__TIME__"\n";

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
		cout << "Usage: " << LTFTutor.Name << " [/i]\n\n";
		ERRi();
	}

ERRFErr
	ExitCode = EXIT_FAILURE;
ERRFEnd
	cout << "\nEnd of program " << LTFTutor.Name << ".\n";
ERRFEpilog
	return ExitCode;
}
