/*
  Test source for the 'mmm' library by Claude L. Simon (simon@epeios.org).
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

#include "mmm.h"

#include "err.h"
#include "stf.h"
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

	fout << tab << tab << tab << '(' << Compteur++ << ") " << ( D / 4 )<< ": " << ( ( ( C - 1 ) / 4 ) + 1 ) << " --> " << sync;
	D = M.Reallocate( D, C );
	fout << ( D / 4 )<< nl;

	M.PrintStructure();

	fout << nl;
}


#define LIMITE	3000
#define NOMBRE 5

void EssaiBase( int argc, char *argv[] )
{
ERRProlog
	flm::file_memory_driver M;
	E_MULTIMEMORY P;
	descriptor__ D[NOMBRE];
	bso::ushort__ C = 1;
ERRBegin
	M.Init( "coucou.tmp" );
	M.Manuel();
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
	// instructions � ex�cuter si erreur
ERREnd
	// instructions � ex�cuter, erreur ou non
ERREpilog
}

void EssaiComplet( int argc, char *argv[] )
{
ERRProlog
	flm::file_memory_driver Memoire;
	E_MULTIMEMORY P;
	multimemory_driver U1, U2, U3;
	bch::E_BUNCH( bso::ulong__ ) M1, M2, M3;
	int i = 50;
	bso::ulong__ j;
	cvm::conventional_memory_driver M;
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
		M1.Write( &j, 1, i );
		M1.Read( i, 1, &j );
		fout << j << '\t';
	}

	i = 0;

	M1.Read( 0, 50, M2 );

	M3.Write( M2, 50, 0 );

	while( i < 50  )
	{
		M1.Read( i, 1, &j );
		fout << j << '\t';

		M2.Read( i, 1, &j );
		fout << j << '\t';

		M3.Read( i, 1, &j );
		fout << j << '\t';

		i++;
	}

	P.PrintStructure();


ERRErr
	// instructions � ex�cuter si erreur
ERREnd
	// instructions � ex�cuter, erreur ou non
ERREpilog
}

#define TAILLE_MAX	100

void Essai( void )
{
ERRProlog
	flm::file_memory_driver Memoire;
	E_MULTIMEMORY Multimemoire;
	descriptor__ Descripteurs[100];
ERRBegin
	Memoire.Init();
	Memoire.Manuel();
//	Multimemoire.cfg( Memoire );
	Multimemoire.Init();

	for( int i = 0; i < sizeof( Descripteurs ) / sizeof( Descripteurs[0] ); i++ )
	{
		Descripteurs[i] = Multimemoire.Allocate( rand() % TAILLE_MAX + 4 );
		fout << Descripteurs[i] << ": " << Multimemoire.Size( Descripteurs[i] )<< tab << sync;
	}

	fout << nl;

	for(;;)
	{
		i = rand() % 100;

		Descripteurs[i] = Multimemoire.Reallocate( Descripteurs[i], Multimemoire.Size( Descripteurs[i] ) + rand() % TAILLE_MAX + 4 );
		fout << Descripteurs[i] << ": " << Multimemoire.Size( Descripteurs[i] )<< tab << sync;
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
	fout << "Test of library " << MMMTutor.Name << ' ' << __DATE__" "__TIME__"\n";

	switch( argc ) {
	case 1:
		Generic( argc, argv );
		EssaiBase( argc, argv );
		EssaiComplet( argc, argv );
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
		fout << "Usage: " << MMMTutor.Name << " [/i]\n\n";
		ERRt();
	}

ERRFErr
	ExitCode = EXIT_FAILURE;
ERRFEnd
	fout << "\nEnd of program " << MMMTutor.Name << ".\n";
ERRFEpilog
	return ExitCode;
}
