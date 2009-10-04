/*
  Test source for the 'mmm' library by Claude SIMON (http://zeusw.org/intl/contact.html).
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

#include "mmm.h"

#include "err.h"
#include "cio.h"
#include "tym.h"
#include "flm.h"
#include "bch.h"

using namespace txf;
using namespace mmm;

void Generic( int argc, char *argv[] )
{
ERRProlog
ERRBegin
ERRErr
ERREnd
ERREpilog
}

void Test(
	E_MULTIMEMORY_ &M,
	descriptor__ &D,
	size__ C )
{
	static unsigned long Compteur = 1;

	cio::cout << tab << tab << tab << '(' << Compteur++ << ") " << ( D / 4 )<< ": " << ( ( ( C - 1 ) / 4 ) + 1 ) << " --> " << sync;
	D = M.Reallocate( D, C );
	cio::cout << ( D / 4 )<< nl;

	M.PrintStructure( cio::cout );

	cio::cout << nl;
}


#define LIMITE	3000
#define NOMBRE 5

void EssaiBase( int argc, char *argv[] )
{
ERRProlog
	flm::standalone_file_memory_driver___ M;
	E_MULTIMEMORY P;
	descriptor__ D[NOMBRE];
	bso::ushort__ C = 1;
ERRBegin
	M.Init( "coucou.tmp" );
	M.Manual();
//	P.plug( M );
	P.Init();

	for ( int i = 0; i < NOMBRE; i++ )
		D[i] = 0;

	while( C < LIMITE )
	{
		Test( P, D[rand()%NOMBRE], C );
		C += 10;
	}

/*	I1 = P.IndiceBloc( D1 );
	I2 = P.IndiceBloc( D2 );

	cout << D1 << '\t' << I1 << '\t'<< P.Descripteur( I1 ) << endl;
	cout << D2 << '\t' << I2 << '\t'<< P.Descripteur( I2 ) << endl;
*/
ERRErr
	// instructions à exécuter si erreur
ERREnd
	// instructions à exécuter, erreur ou non
ERREpilog
}

void EssaiComplet( int argc, char *argv[] )
{
ERRProlog
	flm::standalone_file_memory_driver___ Memoire;
	E_MULTIMEMORY P;
	standalone_multimemory_driver__ U1, U2, U3;
	bch::E_BUNCH( bso::ulong__ ) M1, M2, M3;
	int i = 50;
	bso::ulong__ j;
	cvm::standalone_conventional_memory_driver__ M;
//	int j;
ERRBegin
	Memoire.Init();
	P.plug( Memoire );
	P.Init();

	U1.Init( P );
	U2.Init( P );
	U3.Init( P );

	M1.plug( U1 );
	M2.plug( U2 );
	M3.plug( U3 );
	M1.Init();
	M2.Init();
	M3.Init();

	M1.Allocate( 50 );
	M2.Allocate( 50 );
	M3.Allocate( 50 );

	while( i-- )
	{
		j = i + 300;
		M1.Store( &j, 1, i );
		M1.Recall( i, 1, &j );
		cio::cout << j << '\t';
	}

	i = 0;

	M2.Store( M1, 50 );

	M3.Store( M2, 50, 0 );

	while( i < 50  )
	{
		M1.Recall( i, 1, &j );
		cio::cout << j << '\t';

		M2.Recall( i, 1, &j );
		cio::cout << j << '\t';

		M3.Recall( i, 1, &j );
		cio::cout << j << '\t';

		i++;
	}

	P.PrintStructure( cio::cout );


ERRErr
	// instructions à exécuter si erreur
ERREnd
	// instructions à exécuter, erreur ou non
ERREpilog
}

#define TAILLE_MAX	100

void Essai( void )
{
ERRProlog
	flm::standalone_file_memory_driver___ Memoire;
	E_MULTIMEMORY Multimemoire;
	descriptor__ Descripteurs[100];
ERRBegin
	Memoire.Init();
	Memoire.Manual();
//	Multimemoire.cfg( Memoire );
	Multimemoire.Init();

	for( int i = 0; i < sizeof( Descripteurs ) / sizeof( Descripteurs[0] ); i++ )
	{
		Descripteurs[i] = Multimemoire.Allocate( rand() % TAILLE_MAX + 4 );
		cio::cout << Descripteurs[i] << ": " << Multimemoire.Size( Descripteurs[i] )<< tab << sync;
	}

	cio::cout << nl;

	for(;;)
	{
		int i = rand() % 100;

		Descripteurs[i] = Multimemoire.Reallocate( Descripteurs[i], Multimemoire.Size( Descripteurs[i] ) + rand() % TAILLE_MAX + 4 );
		cio::cout << Descripteurs[i] << ": " << Multimemoire.Size( Descripteurs[i] )<< tab << sync;
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
	cio::cout << "Test of library " << MMMTutor.Name << ' ' << __DATE__" "__TIME__"\n";

	switch( argc ) {
	case 1:
		Generic( argc, argv );
		EssaiBase( argc, argv );
		EssaiComplet( argc, argv );
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
		cio::cout << "Usage: " << MMMTutor.Name << " [/i]\n\n";
		ERRi();
	}

ERRFErr
	ExitCode = EXIT_FAILURE;
ERRFEnd
	cio::cout << "\nEnd of program " << MMMTutor.Name << ".\n";
ERRFEpilog
	return ExitCode;
}
