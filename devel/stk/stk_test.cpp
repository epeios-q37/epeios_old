/*
  Test source for the 'stk' library by Claude L. Simon (simon@epeios.org).
  Copyright (C) 2000,2001 Claude L. SIMON (simon@epeios.org).

  This file is part of the Epeios (http://www.epeios.org/) project.
  

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
#include <iostream.h>

#include "stk.h"

#include "err.h"
#include "cio.h"
#include "str.h"

void Generic( int argc, char *argv[] )
{
ERRProlog
ERRBegin
ERRErr
ERREnd
ERREpilog
}

stk::E_BSTACK( bso::ubyte__ ) BStack;
stk::E_XMCSTACK( str::string_ ) CStack;

int main( int argc, char *argv[] )
{
	int ExitCode = EXIT_SUCCESS;
ERRFProlog
ERRFBegin
	cio::cout << "Test of library " << STKTutor.Name << ' ' << __DATE__" "__TIME__"\n";

	switch( argc ) {
	case 1:
		Generic( argc, argv );
		break;
	case 2:
		if ( !strcmp( argv[1], "/i" ) )
		{
			TTR.Advertise( cio::cout );
			break;
		}
	default:
		cio::cout << txf::sync;
		cio::cerr << "\nBad arguments.\n";
		cio::cout << "Usage: " << STKTutor.Name << " [/i]\n\n";
		ERRi();
	}

ERRFErr
	ExitCode = EXIT_FAILURE;
ERRFEnd
cio::cout << "\nEnd of program " << STKTutor.Name << ".\n";
ERRFEpilog
	return ExitCode;
}
