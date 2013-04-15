/*
  Test source for the 'bch' library by Claude SIMON (http://zeusw.org/intl/contact.html).
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
#include <iostream.h>

#include "bch.h"

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


#include "flm.h"

#define NOMBRE 50

using namespace txf;
using namespace bch;

typedef bso::ubyte__ my__;
typedef bch::E_BUNCH__( my__, 20 ) mys__;
typedef bch::E_BUNCH_( mys__ ) mys_;
E_AUTO( mys )


void Dump( my__ M )
{
	stf::cout << (unsigned long)M << ' ';
}

template <typename t> void Dump( const t &Mys )
{
	epeios::row__ R = Mys.First();

	while( R != E_NIL ) {
		Dump( Mys( R ) );
		R = Mys.Next( R );
	}

	stf::cout << txf::nl;
}


void Test( void )
{
ERRProlog
	mys__ Mys__;
	mys Mys;
ERRBegin
	Mys__ .Init();
	Mys.Init();

	for( int i = 0; i < 10 ; i++ ) {
		Mys__.Init();
		for( int j = i; j < 10 ; j++ )
			Mys__.Append( j );

		Mys.Append( Mys__ );
	}

	stf::cout << txf::nl;

	Dump( Mys );

	stf::cout << txf::nl << txf::sync;


ERRErr
ERREnd
ERREpilog
}
	

void Essai( void )
{
ERRProlog
	flm::file_memory_driver F;
	mmm::multimemory M;
	E_P_BUNCH( bso::ulong__ ) E1, E2;
//	fch_flot_entree_fichier Entree;
//	fch_flot_sortie_fichier Sortie;
	E_P_BUNCH__( int, 3 ) G, H;
	bso::ulong__ i;
	row_t__ P;
ERRBegin
	F.Init( "couocu.tmp" );
	M.plug( F );
	M.Init();

	E1.plug( M );
	E1.Init();

	E2.plug( M );
	E2.Init();

	E1.Allocate( NOMBRE );
	E2.Allocate( NOMBRE );

	for ( i = 0; i < NOMBRE; i++ )
	{
		E1.Store( i, i );
		E2.Store( (bso::ulong__)( NOMBRE - i ), i );
	}

	for ( i = 0; i < E1.Amount(); i++ )
		stf::cout << E1(i) << tab << E2(i) << tab;

	stf::cout << nl;

	stf::cout << ">>>>>>>>> Comparaison: "<< (long)Compare( E1, E2 ) << nl;

	E1.Insert( E2, NOMBRE / 2 );

	for ( P = 0; P < E1.Amount(); P++ )
		stf::cout << E1( P ) << tab;

	stf::cout << nl;

	stf::cout << ">>>>>>>>> Comparaison: "<< (long)Compare( E1, E2 ) << nl;

	for ( P = 0; P < E2.Amount(); P++ )
		stf::cout << E2( P ) << tab;

	stf::cout << nl;

	E1 = E2;

	stf::cout << ">>>>>>>>> Comparaison: "<< (long)Compare( E1, E2 ) << nl;

	E1.write( stf::coutF );

ERRErr
	// instructions à exécuter si erreur
ERREnd
	// instructions à exécuter, erreur ou non
ERREpilog
}


int main( int argc, char *argv[] )
{
	int ExitCode = EXIT_SUCCESS;
ERRFProlog
ERRFBegin
	stf::cout << "Test of library " << BCHTutor.Name << ' ' << __DATE__" "__TIME__"\n";

	switch( argc ) {
	case 1:
		Generic( argc, argv );
		Essai();
		Test();
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
		stf::cout << "Usage: " << BCHTutor.Name << " [/i]\n\n";
		ERRt();
	}

ERRFErr
	ExitCode = EXIT_FAILURE;
ERRFEnd
	stf::cout << "\nEnd of program " << BCHTutor.Name << ".\n";
ERRFEpilog
	return ExitCode;
}
