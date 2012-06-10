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
	addendum__ Addendum = 0;

	cio::COut << tab << tab << tab << '(' << Compteur++ << ") " << (bso::ulong__)( D / 4 )<< ": " << (bso::ulong__)( ( ( C - 1 ) / 4 ) + 1 ) << " --> " << commit;
	D = M.Reallocate( D, C, Addendum );
	cio::COut << (bso::ulong__)( D / 4 )<< nl;

	M.DisplayStructure( cio::COut );

	cio::COut << nl;
}


#define LIMITE	3000
#define NOMBRE 5

void EssaiBase( int argc, char *argv[] )
{
ERRProlog
	flm::file_memory_driver___ M;
	E_MULTIMEMORY P;
	descriptor__ D[NOMBRE];
	bso::ushort__ C = 1;
	flm::id__ ID = FLM_UNDEFINED_ID;
ERRBegin
	ID = flm::GetId();
	M.Init( ID, "coucou.tmp" );
	M.Manual();
	P.plug( M );
	P.Init();

	for ( int i = 0; i < NOMBRE; i++ )
		D[i] = MMM_UNDEFINED_DESCRIPTOR;

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
	if ( ID != FLM_UNDEFINED_ID )
		flm::ReleaseId( ID );
	// instructions à exécuter, erreur ou non
ERREpilog
}

void EssaiComplet( int argc, char *argv[] )
{
ERRProlog
	flm::file_memory_driver___ Memoire;
	E_MULTIMEMORY P;
	standalone_multimemory_driver__ U1, U2, U3;
	bch::E_BUNCH( bso::ulong__ ) M1, M2, M3;
	int i = 50;
	bso::ulong__ j;
	cvm::standalone_conventional_memory_driver___ M;
//	int j;
	flm::id__ ID = FLM_UNDEFINED_ID;
ERRBegin
	ID = flm::GetId();
	Memoire.Init( ID, false );
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
		cio::COut << j << '\t';
	}

	i = 0;

	M2.Store( M1, 50 );

	M3.Store( M2, 50, 0 );

	while( i < 50  )
	{
		M1.Recall( i, 1, &j );
		cio::COut << j << '\t';

		M2.Recall( i, 1, &j );
		cio::COut << j << '\t';

		M3.Recall( i, 1, &j );
		cio::COut << j << '\t';

		i++;
	}

	P.DisplayStructure( cio::COut );


ERRErr
	// instructions à exécuter si erreur
	if ( ID != FLM_UNDEFINED_ID )
		flm::ReleaseId( ID );
ERREnd
	// instructions à exécuter, erreur ou non
ERREpilog
}

#define TAILLE_MAX	100

void Essai( void )
{
ERRProlog
	flm::file_memory_driver___ Memoire;
	E_MULTIMEMORY Multimemoire;
	descriptor__ Descripteurs[100];
	flm::id__ ID = FLM_UNDEFINED_ID;
	mmm::addendum__ Addendum = 0;
ERRBegin
	ID = flm::GetId();
	Memoire.Init( ID, false );
	Memoire.Manual();
	Multimemoire.plug( Memoire );
	Multimemoire.Init();

	for( int i = 0; i < sizeof( Descripteurs ) / sizeof( Descripteurs[0] ); i++ )
	{
		Descripteurs[i] = Multimemoire.Allocate( rand() % TAILLE_MAX + 4, Addendum );
		cio::COut << (bso::ulong__)Descripteurs[i] << ": " << (bso::ulong__)Multimemoire.Size( Descripteurs[i] )<< tab << commit;
	}

	cio::COut << nl;

	while ( rand() % 100000 )
	{
		int i = rand() % 100;

		Descripteurs[i] = Multimemoire.Reallocate( Descripteurs[i], Multimemoire.Size( Descripteurs[i] ) + rand() % TAILLE_MAX + 4, Addendum );
		cio::COut << (bso::ulong__)Descripteurs[i] << ": " << (bso::ulong__)Multimemoire.Size( Descripteurs[i] )<< tab << commit;
	}


ERRErr
	if ( ID != FLM_UNDEFINED_ID )
		flm::ReleaseId( ID );
ERREnd
ERREpilog
}


int main( int argc, char *argv[] )
{
	int ExitCode = EXIT_SUCCESS;
ERRFProlog
ERRFBegin
	cio::COut << "Test of library " << MMMTutor.Name << ' ' << __DATE__" "__TIME__"\n";

	switch( argc ) {
	case 1:
		Generic( argc, argv );
		EssaiBase( argc, argv );
		Essai();
		EssaiComplet( argc, argv );
		break;
	case 2:
		if ( !strcmp( argv[1], "/i" ) )
		{
			TTR.Advertise( cio::COut );
			break;
		}
	default:
		cio::COut << commit;
		cio::CErr << "\nBad arguments.\n";
		cio::COut << "Usage: " << MMMTutor.Name << " [/i]\n\n";
		ERRi();
	}

ERRFErr
	ExitCode = EXIT_FAILURE;
ERRFEnd
	cio::COut << "\nEnd of program " << MMMTutor.Name << ".\n";
ERRFEpilog
	return ExitCode;
}
