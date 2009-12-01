/*
  Test source for the 'tym' library by Claude SIMON (http://zeusw.org/intl/contact.html).
  Copyright (C) 2000,2001 Claude SIMON (http://zeusw.org/intl/contact.html).

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

#include "tym.h"

#include "err.h"
#include "stf.h"

void Generic( int argc, char *argv[] )
{
ERRProlog
ERRBegin
ERRErr
ERREnd
ERREpilog
}


typedef bso::ushort__	mytype__;

#define TAILLE_MEMOIRE	100

void Essai( void )
{
ERRProlog
/*	mfc_pilote_memoire_fichier R1;
	mfc_pilote_memoire_fichier R2;
*/	tym::E_MEMORY( mytype__ ) M1;
	tym::E_MEMORY( mytype__ ) M2;
	tym::E_MEMORY__( mytype__, 3 ) M3;
	tym::E_MEMORY___( mytype__ ) M4;
	bso::ulong__ I;
	mytype__ J;
	ifstream IN;
	ofstream OUT;
ERRBegin
/*	R1.Init();
	R2.Init();
*/
	M1.Init();
	M1.Allocate( TAILLE_MEMOIRE );
	M1.Store( 0, (epeios::row__)0, TAILLE_MEMOIRE );

	M2.Init();
	M2.Allocate( TAILLE_MEMOIRE );

	M3.Init();

	for ( I = 0; I < TAILLE_MEMOIRE; I++ )
	{
		J = (mytype__)I + 1;
		M1.Store( 60000 + J, I );
	}


	for ( I = 0; I < TAILLE_MEMOIRE; I++ )
	{
		M1.Recall( I, J );
		stf::cout << J << '\t' << txf::sync;
	}

	stf::cout << txf::nl;

	M2.Store( M1, TAILLE_MEMOIRE );

	for ( I = 0; I < TAILLE_MEMOIRE; I++ )
	{
		stf::cout << M2.Get( I ) << '\t' << txf::sync;
	}

	stf::cout << '\n';

	M1.Swap( 1, 2 );

ERRErr
ERREnd
ERREpilog
}




int main( int argc, char *argv[] )
{
	int ExitCode = EXIT_SUCCESS;
ERRFProlog
ERRFBegin
	stf::cout << "Test of library " << TYMTutor.Name << ' ' << __DATE__" "__TIME__"\n";

	switch( argc ) {
	case 1:
		Generic( argc, argv );
		Essai();
		break;
	case 2:
		if ( !strcmp( argv[1], "/i" ) )
		{
			TTR.Advertise();
			break;
		}
	default:
		stf::cout << txf::sync;
		stf::cerr << "\nBad arguments.\n";
		stf::cout << "Usage: " << TYMTutor.Name << " [/i]\n\n";
		ERRt();
	}

ERRFErr
	ExitCode = EXIT_FAILURE;
ERRFEnd
	stf::cout << "\nEnd of program " << TYMTutor.Name << ".\n";
ERRFEpilog
	return ExitCode;
}
