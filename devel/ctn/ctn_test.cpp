/*
  Test source for the 'ctn' library by Claude L. Simon (simon@epeios.org).
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
#include <iostream.h>

#include "ctn.h"

#include "err.h"
#include "stf.h"
#include "str.h"
#include "flm.h"

using namespace ctn;

void Generic( int argc, const char *argv[] )
{
ERRProlog
ERRBegin
ERRErr
ERREnd
ERREpilog
}

void EssaiBasic( void )
{
ERRProlog
	E_XCONTAINER( str::string_ ) C;
//	MITEM( str::string_ ) E;
ERRBegin
	C.Init();
#if 0
	C.Allocate( 2 );

	E.Init( C );

	C( 0 ).Init();
	C( 0 ) = "coucou";

	C( 1 ).Init();
	C( 1 ) = "hello";
#else
	C.Add( str::string( "coucou" ) );

	C.Add( str::string( "hello" ) );
#endif

	fout << C( 0 ) << txf::tab;
	fout << C( 1 ) << txf::nl;
	
ERRErr
ERREnd
ERREpilog
}



#define LM	'Z'
#define Lm	'z'
#define LC	'9'

void Classer( E_MCONTAINER_( str::string_ ) &Liste )
{
ERRProlog
	bso::bool__ Swap = true;
	str::string S1;
	E_MITEM( str::string_ ) S2;
	E_MITEM( str::string_ ) EListe;
ERRBegin
	S1.Init();
	S2.Init( Liste );
	EListe.Init( Liste );

	while ( Swap )
	{
		Swap = false;

		if ( Liste.Amount() )
			S2.Sync( 0 );
//			S2 = Liste[0];

		for ( epeios::row_t__ i = 1; i < Liste.Amount(); i++ )
	{
			S1 = S2();
			S2.Sync( i );
//			S2 = Liste[i];

			if ( S1 < S2() )
			{
//				Liste.Dynamique.Multimemoire()->AfficherStructure();

				Swap = true;

				EListe(i-1) = S2();
				S2() = S1;

//				Liste.Dynamique.Multimemoire()->AfficherStructure();
			}
		}
	}
ERRErr
ERREnd
ERREpilog
}
void Remplir( str::string_ &S )
{
	int i = 10UL * rand() / RAND_MAX + 1;

	while ( i-- )
		S.Add( (char)( 'a' + ( 'z' - 'a' ) * rand() / RAND_MAX ) );
}



void Essai( int argc, const char *argv[] )
{
ERRProlog
	flm::file_memory_driver F;
	mmm::multimemory M;
	E_XMCONTAINER( str::string_ ) CS, CD;
/*	fch_flot_sortie_fichier S;
	fch_flot_entree_fichier E;
*/	E_MITEM( str::string_ ) ECS, ECD;
	str::string Str;
	epeios::row__ P;
ERRBegin
	F.Init();
	F.Manuel();
	M.plug( F );
	M.Init();
	CS.plug( M );
	CS.Init();
	CS.Allocate( 12 );
	ECS.Init( CS );
	ECD.Init( CD );

	Str.Init();

	Str = "a";

	ECS(5).Init();
	Str = "a";
	ECS(5)= Str;
	ECS(4).Init();
	Str = "bc";
	ECS(4)= Str;
	ECS(3).Init();
	Str = "def";
	ECS(3)= Str;
	ECS(2).Init();
	Str = "ghij";
	ECS(2)= Str;
	ECS(1).Init();
	Str = "qlmno";
	ECS(1)= Str;
	ECS(0).Init();
	Str = "parstu";
	ECS(0)= Str;
	ECS(6).Init();
	Str = "c";
	ECS(6)= Str;
	Str = "co";
	ECS(7).Init();
	ECS(7)= Str;
	Str = "cou";
	ECS(8).Init();
	ECS(8)= Str;
	Str = "couc";
	ECS(9).Init();
	ECS(9)= Str;
	Str = "couco";
	ECS(10).Init();
	ECS(10)= Str;
	Str = "coucou";
	ECS(11).Init();
	ECS(11)= Str;

	P = CS.First();
	
	while( P != NONE ) {
		fout << ECS(P) << txf::tab;
		P = CS.Next( P );
	}

	fout << txf::nl;

	Classer( CS );
#if 0
	S.Init( "Essai.txt" );
	S << CS;
	S.prg();

	E.Init( "Essai.txt" );
	CD.Init();
	E >> CD;
	E.prg();
#else
	CD.Init();

	CD = CS;
	ECD.Init( CD );
#endif

	P = CD.First();
	
	while( P != NONE ) {
		fout << ECD(P) << txf::tab;
		P = CD.Next( P );
	}

	ECD.Sync();

	fout << txf::nl;

	CD.Delete( 3 );

	P = CD.First();
	
	while( P != NONE ) {
		fout << ECD(P) << txf::tab;
		P = CD.Next( P );
	}

	fout << txf::nl;
ERRErr
ERREnd
ERREpilog

}



void EssaiCopie( int argc, const char *argv[] )
{
ERRProlog
	flm::file_memory_driver F;
	mmm::multimemory Mm;
	E_XCONTAINER( E_XCONTAINER_( E_XMCONTAINER_( str::string_ ) ) ) CM;
	E_XCONTAINER( E_XMCONTAINER_( str::string_ ) ) Cm;
	E_XMCONTAINER( str::string_ ) CC;
	str::string S;
/*	fch_flot_sortie_fichier FO;
	fch_flot_entree_fichier FI;
*/	E_ITEM( E_XCONTAINER_( E_XMCONTAINER_( str::string_ ) ) ) ECM;
//	ITEM( MCONTAINER_( str::string_ ) ) ECm;
	E_MITEM( str::string_ ) ECC;
	char M, m, C;
ERRBegin
	F.Init("a.tmp");
//	F.Manuel();
	Mm.plug( F );
	Mm.Init();
	CM.plug( Mm );
	CM.Init();
	Cm.Init();
	CC.Init();

	CM.Allocate( LM - 'A' + 1 );
	Cm.Allocate( Lm - 'a' + 1 );
	CC.Allocate( LC - '0' + 1 );

	ECC.Init( CC );
//	ECm.Init( Cm );
	ECM.Init( CM );

	for ( M = 'A'; M <= LM; M++ )
	{
		for ( m = 'a'; m <= Lm; m++ )
		{
			for ( C = '0'; C <= LC; C++ )
			{
				S.Init();
//				S.SetStepValue( 0 );

				S.Add( M );
				S.Add( m );
				S.Add( C );

				ECC( C - '0' ).Init();
				ECC() = S;
//				fout << S << " ";
				fout << ECC(C - '0') << " ";

			}

			ECC.Sync();

			fout << '\t';
/*
			ECm( m - 'a' ).Init();
			ECm() = CC;
*/
			Cm( m - 'a' ).Init();
			Cm() = CC;

		}

//		ECm.Sync();
		Cm.Sync();

		fout << txf::nl;
		ECM(M - 'A').Init();
		ECM() = Cm;
	}

	fout << "--------------" << txf::nl;

	ECM.ChangeMode( mdr::mReadOnly );
//	ECm.ChangeMode( mdr::mReadOnly );
	ECC.ChangeMode( mdr::mReadOnly );

	for ( M = 'A'; M <= LM; M++ )
	{
		Cm = ECM(M - 'A');

		for ( m ='a'; m <= Lm; m++ )
		{
//			CC = ECm(m - 'a');
			CC = Cm(m - 'a');

			for ( C = '0'; C <= LC; C++ )
			{
				S = ECC(C - '0');

				fout << S << ' ' << txf::sync;
			}

			fout << '\t';

		}

		fout << txf::nl;
	}
/*
	CMS.Mode( plm::mModification );
	F.Mode( plm::mModification );
*/
	// Pour que l'objet puisse se détruire.

ERRErr
	// instructions à exécuter si erreur
ERREnd
	// instructions à exécuter, erreur ou non
ERREpilog
}

void SP2(
	E_XMCONTAINER_( str::string_ ) &O,
	char M,
	char m )
{
ERRProlog
	E_MITEM( str::string_ ) E;
ERRBegin
	E.Init( O );

	for ( char C = '0'; C <= LC; C++ )
	{
		E(C - '0').Init();
//		E(C - '0').SetStepValue( 0 );


		E(C - '0').Add( M );
		E(C - '0').Add( m );
		E(C - '0').Add( C );

//		E(C - '0').Adjust();

		fout << E(C - '0') << " " << txf::sync;
	}
ERRErr
ERREnd
ERREpilog
}


void SP1(
	E_XCONTAINER_( E_XMCONTAINER_( str::string_ ) ) &O,
	char M)
{
ERRProlog
	E_ITEM( E_XMCONTAINER_( str::string_ ) ) E;
ERRBegin
	E.Init( O );

	for ( char m = 'a'; m <= Lm; m++ )
	{
		E(m - 'a').Init();
		E(m - 'a').Allocate( LC - '0' + 1 );

		SP2( E( m - 'a' ), M, m );

		fout << '\t';
	}
ERRErr
ERREnd
ERREpilog
}

void SSP2(
	const E_MCONTAINER_( str::string_ ) &O,
	char M,
	char m )
{
ERRProlog
	E_CMITEM( str::string_ ) E;
ERRBegin
	E.Init( O );

	for ( char C = '0'; C <= LC; C++ )
	{
		fout << E(C - '0') << ' ';
	}
ERRErr
ERREnd
ERREpilog
}


void SSP1(
	const E_XCONTAINER_( E_XMCONTAINER_( str::string_ ) ) &O,
	char M)
{
ERRProlog
	E_CITEM( E_XMCONTAINER_( str::string_ ) ) E;
ERRBegin
	E.Init( O );

	for ( char m = 'a'; m <= Lm; m++ )
	{
		SSP2( E(m - 'a'), M, m );

		fout << '\t';
	}
ERRErr
ERREnd
ERREpilog
}



void EssaiDirect( int argc, const char *argv[] )
{
ERRProlog
	flm::file_memory_driver F;
	mmm::multimemory Mm;
//	CONTAINER( CONTAINER_( ctn_conteneur_polymemoire_< UTL_2EN1( str::string_, str::string_::s ) > ) ) GC;
	E_XCONTAINER( E_XCONTAINER_( E_XMCONTAINER_( str::string_ ) ) ) GC;
	E_ITEM( E_XCONTAINER_( E_XMCONTAINER_( str::string_ ) ) ) EGC;
	char M;
ERRBegin
	F.Init("b.tmp");
	F.Manuel();
	Mm.plug( F );
	Mm.Init();
//	GC.plug( Mm );
	GC.Init();
	GC.Allocate( LM - 'A' + 1 );

	EGC.Init( GC );

	for ( M = 'A'; M <= LM; M++ )
	{
		EGC(M - 'A').Init();
//		GC[M - 'A'].Multimemoire.Optimisation( mmm::Temps );
		EGC(M - 'A').Allocate( Lm - 'a' + 1 );

		SP1( EGC(M - 'A' ), M );

		fout << txf::nl;

	}

	fout << "--------------" << txf::nl;

	for ( M = 'A'; M <= LM; M++ )
	{
		SSP1( EGC(M - 'A'), M );

		fout << txf::nl;

	}



ERRErr
	// instructions à exécuter si erreur
ERREnd
	// instructions à exécuter, erreur ou non
ERREpilog
}

void EssaiSimpleMono( void )
{
ERRProlog
	flm::file_memory_driver F;
	mmm::multimemory M;
	E_XMCONTAINER( str::string_ ) C;
	E_MITEM( str::string_ ) E;
ERRBegin
	F.Init( "coucou.tmp" );
	M.plug( F );
	M.Init();
	C.plug( M );
	C.Init();
	C.Allocate( 2 );
	E.Init( C );

	E(0).Init();
	E(0) = str::string( "a" );
	E(1).Init();
	E(1)= str::string( "b" );
	fout << E(0) << txf::tab;
	fout << E(1) << txf::nl;
ERRErr
ERREnd
ERREpilog
}

void EssaiSimpleMulti( void )
{
ERRProlog
	flm::file_memory_driver F;
	mmm::multimemory M;
	E_CONTAINER( str::string_ ) C;
	E_ITEM( str::string_ ) E;
ERRBegin
	F.Init( "coucou.tmp" );
	M.plug( F );
	M.Init();
	C.plug( M );
	C.Init();
	C.Allocate( 2 );
	E.Init( C );

	E(0).Init();
	E(0) = str::string( "a" );
	E(1).Init();
	E(1)= str::string( "b" );
	fout << E(0) << txf::tab;
	fout << E(1) << txf::nl;
ERRErr
ERREnd
ERREpilog
}

void EssaiConteneurDansConteneur( void )
{
ERRProlog
	flm::file_memory_driver F;
	mmm::multimemory M;
	E_XMCONTAINER( str::string_ ) CS;
	E_XCONTAINER( E_XMCONTAINER_( str::string_ ) ) CC;
//	ITEM( str::string_ ) ECS;
//	ITEM( CONTENEUR_( str::string_ ) ) ECC;
ERRBegin
	F.Init( "temp.tmp" );
	M.plug( F );
	M.Init();

//	CS.plug( M );
	CS.Init();

//	CC.plug( M );
	CC.Init();

//	ECS.Init( CS );
//	ECC.Init( CC );

	CS.Allocate( 1 );
	CC.Allocate( 2 );

	CS(0).Init();
	CS() = "coucou";
	CS.Sync();

	CC(0).Init();
	CC() = CS;
	CC.Sync();

	fout << CS( 0 ) << txf::tab;

	CS(0).Init();
	CS() = "hello";
	CS.Sync();

	CC(1).Init();
	CC() = CS;
	CC.Sync();

	fout << CS( 0 ) << txf::tab;

	CS = CC( 0 );
	CC.Sync();
	fout << CS( 0 ) << txf::tab;

	CS = CC( 1 );
	CC.Sync();
	fout << CS( 0 ) << txf::nl;

	CS.Sync();
ERRErr
ERREnd
ERREpilog
}




int main( int argc, const char *argv[] )
{
	int ExitCode = EXIT_SUCCESS;
ERRFProlog
ERRFBegin
	fout << "Test of library " << CTNTutor.Name << ' ' << __DATE__" "__TIME__"\n";

	switch( argc ) {
	case 1:
		Generic( argc, argv );
		EssaiBasic();
		EssaiConteneurDansConteneur();
		EssaiSimpleMono();
		EssaiSimpleMulti();
		Essai( argc, argv );
#if 1
		fout << "********************************************************" << txf::nl;
		EssaiDirect( argc, argv );
		fout << "********************************************************" << txf::nl;
		EssaiCopie( argc, argv );
#endif
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
		fout << "Usage: " << CTNTutor.Name << " [/i]\n\n";
		ERRt();
	}

ERRFErr
	ExitCode = EXIT_FAILURE;
ERRFEnd
	fout << "\nEnd of program " << CTNTutor.Name << ".\n";
ERRFEpilog
	return ExitCode;
}
