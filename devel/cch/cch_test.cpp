/*
  Test source for the 'cch' library by Claude L. Simon (csimon@epeios.org).
  Copyright (C) 2000,2001 Claude L. SIMON (csimon@epeios.org).

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
#include <iostream.h>

#include "cch.h"

#include "err.h"
#include "stf.h"

TYPEDEF( epeios::row_t__, test__ );

void Generic( int argc, char *argv[] )
{
ERRProlog
ERRBegin
ERRErr
ERREnd
ERREpilog
}

void Test( int argc, char *argv[] )
{
ERRProlog
	bch::E_BUNCHt( bso::ubyte__, test__ ) Bunch;
	cch::E_RW_CACHEt___( bso::ubyte__, test__ ) Cache;
	int i;
	int rnd;
ERRBegin
	Bunch.Init();
	Bunch.Allocate( 101 );

	Cache.Init( Bunch, 10 );

	for( i = 0; i <= 100; i++ )
		Cache.Put( i, i );

	for( i = 0; i <= 100; i++ )
		fout << (unsigned long)i << ": " << (unsigned long)Cache.Get( i ) << txf::tab;

	fout << txf::nl;

	for( i = 100; i >= 0; i-- )
		fout << (unsigned long)i << ": " << (unsigned long)Cache.Get( i ) << txf::tab;

	fout << txf::nl;


	for( i = 0; i <= 100; i++ ) {
		rnd = rand() % 101;

		fout << (unsigned long)rnd << ": " << (unsigned long)Cache.Get( rnd ) << txf::tab;
	}

ERRErr
ERREnd
ERREpilog
}


int main( int argc, char *argv[] )
{
	int ExitCode = EXIT_SUCCESS;
ERRFProlog
ERRFBegin
	fout << "Test of library " << CCHTutor.Name << ' ' << __DATE__" "__TIME__"\n";

	switch( argc ) {
	case 1:
		Generic( argc, argv );
		Test( argc, argv );
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
		fout << "Usage: " << CCHTutor.Name << " [/i]\n\n";
		ERRt();
	}

ERRFErr
	ExitCode = EXIT_FAILURE;
ERRFEnd
	fout << "\nEnd of program " << CCHTutor.Name << ".\n";
ERRFEpilog
	return ExitCode;
}
