/*
  Test source for the 'tol' library by Claude SIMON (http://zeusw.org/intl/contact.html).
  Copyright (C) 2000 Claude SIMON (http://zeusw.org/intl/contact.html).

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

#include "tol.h"

#include "err.h"
#include "cio.h"


using cio::COut;
using cio::CErr;

void Generic( int argc, char *argv[] )
{
ERRProlog
ERRBegin
    SYSTEMTIME st, lt;
    
    GetSystemTime(&st);
    GetLocalTime(&lt);
    
    printf("The system time is: %02d:%02d\n", st.wHour, st.wMinute);
    printf(" The local time is: %02d:%02d\n", lt.wHour, lt.wMinute);

	cio::COut << tol::DateAndTime() << txf::nl << txf::commit;
ERRErr
ERREnd
ERREpilog
}

int main( int argc, char *argv[] )
{
	int ExitCode = EXIT_SUCCESS;
ERRFProlog
ERRFBegin
	cio::COut << "Test of library " << TOLTutor.Name << ' ' << __DATE__" "__TIME__"\n";

	switch( argc ) {
	case 1:
		Generic( argc, argv );
		break;
	case 2:
		if ( !strcmp( argv[1], "/i" ) )
		{
			TTR.Advertise( cio::COut);
			break;
		}
	default:
		cio::COut << txf::commit;
		cio::CErr << "\nBad arguments.\n";
		cio::COut << "Usage: " << TOLTutor.Name << " [/i]\n\n";
		ERRExit( EXIT_FAILURE );
	}

ERRFErr
	ExitCode = EXIT_FAILURE;
ERRFEnd
	cio::COut << "\nEnd of program " << TOLTutor.Name << ".\n";
ERRFEpilog
	return ExitCode;
}
