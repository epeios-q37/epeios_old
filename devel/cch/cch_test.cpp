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

TYPEDEF( epeios::row_t__, brow__ );
TYPEDEF( epeios::row_t__, crow__ );
typedef bso::ushort__	mytype__;

void Generic( int argc, char *argv[] )
{
ERRProlog
ERRBegin
ERRErr
ERREnd
ERREpilog
}

void Essai( int argc, char *argv[] )
{
ERRProlog
	ctn::E_XMCONTAINERt( bch::E_BUNCHt_( mytype__, brow__ ), crow__ ) Container;
	cch::E_RW_CACHESt___( mytype__, brow__, crow__ ) RWCaches;
	int rnd, i, j;
ERRBegin

	Container.Init();

	Container.Allocate( 10 );

	RWCaches.Init( Container, 80, 2 );

	for( i = 0; i < 10; i++ ) {
		for( j = 0; j < 100;  j++ )
			RWCaches.Add( j + i * 100, i );
	}

	Container.Sync();


	for( i = 0; i <= 100; i++ ) {
		rnd = rand() % 1000;

		fout << (unsigned long)rnd << ": " << (unsigned long)RWCaches.Get( rnd / 100, rnd % 100 ) << txf::tab << txf::sync;
	}
ERRErr
ERREnd
ERREpilog
}

void Test( int argc, char *argv[] )
{
ERRProlog
	bch::E_BUNCHt( mytype__, brow__ ) Bunch;
	cch::E_RW_CACHEt___( mytype__, brow__ ) RWCache;
	cch::E_RO_CACHEt___( mytype__, brow__ ) ROCache;
	int i;
	int rnd;
ERRBegin
	Bunch.Init();

	RWCache.Init( Bunch, 10 );

	for( i = 0; i <= 100; i++ )
		RWCache.Add( i );

	for( i = 0; i <= 100; i++ )
		fout << (unsigned long)i << ": " << (unsigned long)RWCache.Get( i ) << txf::tab;

	fout << txf::nl;

	ROCache.Init( Bunch, 10 );

	for( i = 100; i >= 0; i-- )
		fout << (unsigned long)i << ": " << (unsigned long)ROCache.Get( i ) << txf::tab;

	fout << txf::nl;


	for( i = 0; i <= 100; i++ ) {
		rnd = rand() % 101;

		fout << (unsigned long)rnd << ": " << (unsigned long)ROCache.Get( rnd ) << txf::tab;
	}

	RWCache.Amount();

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
		fout << txf::nl << "-------------------------------------------" << txf::nl;
		Essai( argc, argv );
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
