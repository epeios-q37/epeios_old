/*
  Test source for the 'mtx' library by Claude L. Simon (simon@epeios.org).
  Copyright (C) 2000 Claude L. SIMON (simon@epeios.org).

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

#include "mtx.h"

#include "err.h"
#include "stf.h"
#include "mtk.h"
#include "tol.h"

void Generic( int argc, char *argv[] )
{
ERRProlog
ERRBegin
ERRErr
ERREnd
ERREpilog
}

#if 1

void f( void *UP )
{
	mtx::mutex_handler__ &Mutex = *(mtx::mutex_handler__ *)UP;

	stf::fout << "- Waiting ..." << txf::nl;

	tol::Wait( 5 );

	stf::fout << "- Unlocking ..." << txf::nl;

	tol::Wait( 1 );

	mtx::Unlock( Mutex );
}

void Test( void )
{
ERRProlog
	mtx::mutex_handler__ Mutex = MTX_INVALID_HANDLER;
ERRBegin
	Mutex = mtx::Create();

	stf::fout << "+ Locking ..." << txf::nl;

	mtx::Lock( Mutex );

	stf::fout << "+ Locked ..." << txf::nl;

	mtk::Launch( f, &Mutex );

	tol::Wait( 1 );

	stf::fout << "+ Locking ..." << txf::nl;

	mtx::Lock( Mutex );

	stf::fout << "+ Locked ..." << txf::nl;

	mtx::Unlock( Mutex );
ERRErr
ERREnd
	if ( Mutex != MTX_INVALID_HANDLER )
		mtx::Delete( Mutex );

	mtx::Lock( Mutex );
ERREpilog
}

#else

void Test( void )
{
ERRProlog
	mtx::mutex_handler__ Mutex = MTX_INVALID_HANDLER;
ERRBegin
	Mutex = mtx::Create();

	stf::fout << "Locking ..." << txf::nl;

	mtx::Lock( Mutex );

	stf::fout << "Locked ..." << txf::nl;

	if ( !mtx::TryToLock( Mutex ) )
		stf::fout << "Locking failed ..." << txf::nl;
	else
		stf::fout << "Locking succeed ..." << txf::nl;

	stf::fout << "Unlocking ..." << txf::nl;

	mtx::Unlock( Mutex );

	stf::fout << "Unlocked ..." << txf::nl;

	stf::fout << "Locking ..." << txf::nl;

	mtx::Lock( Mutex );

	stf::fout << "Locked ..." << txf::nl;

	stf::fout << "Unlocking ..." << txf::nl;

	mtx::Unlock( Mutex );

	stf::fout << "Unlocked ..." << txf::nl;
ERRErr
ERREnd
	if ( Mutex != MTX_INVALID_HANDLER )
		mtx::Delete( Mutex );
ERREpilog
}

#endif
int main( int argc, char *argv[] )
{
	int ExitCode = EXIT_SUCCESS;
ERRFProlog
ERRFBegin
	stf::fout << "Test of library " << MTXTutor.Name << ' ' << __DATE__" "__TIME__"\n";

	switch( argc ) {
	case 1:
		Generic( argc, argv );
		Test();
		break;
	case 2:
		if ( !strcmp( argv[1], "/i" ) )
		{
			TTR.Advertise();
			break;
		}
	default:
		stf::fout << txf::sync;
		stf::ferr << "\nBad arguments.\n";
		stf::fout << "Usage: " << MTXTutor.Name << " [/i]\n\n";
		ERRt();
	}

ERRFErr
	ExitCode = EXIT_FAILURE;
ERRFEnd
	stf::fout << "\nEnd of program " << MTXTutor.Name << ".\n";
ERRFEpilog
	return ExitCode;
}
