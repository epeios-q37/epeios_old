/*
  Test source for the 'tst' library by Claude L. Simon (simon@epeios.org).
  Copyright (C) 2000,2001 Claude L. SIMON (simon@epeios.org).

  This file is part of the Epeios (http://www.epeios.org/) project.
  

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
#include <iostream.h>

#include "tst.h"

#include "err.h"
#include "stf.h"

/* Namespace problem under VC++ */

namespace na {
	template <class t> class ca
	{
	private:
		t T;
	public:
		void f( void )
		{}
	};
}

namespace nb {

#if 1
	using namespace na;
#elif 0
	using na::ca;
#endif

	template <class t> class cb
	: public na::ca< t >
	{
	public:
		void f( void )
		{
			ca< t >::f();
		}
	};
}

na::ca< int > CA;

/* End namespace problem under VC++ */



void Generic( int argc, char *argv[] )
{
ERRProlog
	nb::cb< int > NB;
ERRBegin
	NB.f();
ERRErr
ERREnd
ERREpilog
}

int main( int argc, char *argv[] )
{
	int ExitCode = EXIT_SUCCESS;
ERRFProlog
ERRFBegin
	fout << "Test of library " << TSTTutor.Name << ' ' << __DATE__" "__TIME__"\n";

	switch( argc ) {
	case 1:
		Generic( argc, argv );
		break;
	case 2:
		if ( !strcmp( argv[1], "/i" ) )
		{
			TTR.Advertise();
			break;
		}
	default:
		fout << txf::sync;
		ferr << "\nBad arguments.\n";
		fout << "Usage: " << TSTTutor.Name << " [/i]\n\n";
		ERRt();
	}

ERRFErr
	ExitCode = EXIT_FAILURE;
ERRFEnd
	fout << "\nEnd of program " << TSTTutor.Name << ".\n";
ERRFEpilog
	return ExitCode;
}
