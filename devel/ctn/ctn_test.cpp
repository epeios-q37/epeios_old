/*
  Test source for the 'ctn' library by Claude SIMON (http://zeusw.org/intl/contact.html).
  Copyright (C) 2000 Claude SIMON (http://zeusw.org/intl/contact.html).

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

#define CPE__USE_GCC_WORKAROUND

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>

#include "ctn.h"

#include "err.h"
#include "cio.h"
#include "str.h"
#include "flm.h"

using namespace ctn;

#define cout cio::COut
#define cerr cio::CErr

void Generic( int argc, const char *argv[] )
{
ERRProlog
ERRBegin
ERRErr
ERREnd
ERREpilog
}


/* Begin bug tracking code */

typedef str::string_	datum_;
typedef str::string		datum;



typedef ctn::E_MCONTAINER_( datum_ ) data_;
E_AUTO( data )

typedef ctn::E_CONTAINER_( data_ ) data_cluster_;
E_AUTO( data_cluster )

void Print( const data_ &Data )
{
	mdr::row__ Row = NONE;
	ctn::E_CMITEM( datum_ ) Datum;

	Datum.Init( Data );

	Row = Data.First();

	cout << "****" << txf::tab << txf::commit;

	while( Row != NONE ) {
		cout << '>' << txf::commit;
		cout << Datum( Row ) << '<' << txf::tab << txf::commit;
		Row = Data.Next( Row );
	}

	cout << txf::nl << txf::commit;
}

void Fill( data_cluster_ &DataCluster )
{
ERRProlog

	mdr::row__ Row = NONE;
	data Data;
ERRBegin

	Data.Init();

	Data.Append( datum( "tut" ) );
	Data.Append( datum( "tata" ) );
	Data.Append( datum( "to" ) );

	DataCluster.Allocate( 3 );
	Row = DataCluster.First();

	while( Row != NONE ) {
		DataCluster( Row ).Append( Data( Row ) );
		DataCluster( Row ).Flush();
		Row = DataCluster.Next( Row );
	}

	DataCluster.Flush();
ERRErr
ERREnd
ERREpilog
}

void PrintV( data_cluster_ &DataCluster )
{
	mdr::row__ Row = NONE;

	DataCluster.Flush();

	Row = DataCluster.First();

	cout << ">>> V" << txf::nl << txf::commit;

	while( Row != NONE ) {
		Print( DataCluster( Row ) );
		Row = DataCluster.Next( Row );
	}

	cout << "V >>>" << txf::nl << txf::commit;

	DataCluster.Flush();
}

void PrintC( const data_cluster_ &DataCluster )
{
	mdr::row__ Row = NONE;
	ctn::E_CITEM( data_ ) Data;

	Data.Init( DataCluster );

	Row = DataCluster.First();

	cout << ">>> C" << txf::nl << txf::commit;

	while( Row != NONE ) {
		Print( Data( Row ) );
		Row = DataCluster.Next( Row );
	}

	cout << "C >>>" << txf::nl << txf::commit;
}

void BugTracking( void )
{
ERRProlog
	data_cluster DataCluster;
	mdr::row__ Row = NONE;
ERRBegin
	DataCluster.Init();
	Fill( DataCluster );
	PrintC( DataCluster );
	PrintV( DataCluster );

	DataCluster.Init();
	DataCluster.Allocate( 3 );

	PrintC( DataCluster );
	PrintV( DataCluster );
ERRErr
ERREnd
ERREpilog
}


/* End bug tracking code */




void EssaiBasic( void )
{
ERRProlog
	mmm::multimemory MM;
	E_MCONTAINER( str::string_ ) C;
//	MITEM( str::string_ ) E;
ERRBegin
	MM.Init();
//	MM.Preallocate( 10000 );
	C.plug( MM );
	C.Init();
//	MM.DisplayStructure( cout );
#if 0
	C.Allocate( 2 );

	E.Init( C );

	C( 0 ).Init();
	C( 0 ) = "coucou";

	C( 1 ).Init();
	C( 1 ) = "hello";
#else
	C.Append( str::string( "coucou" ) );
//	MM.DisplayStructure( cout );

	C.Insert( str::string( "hello" ), 0 );
//	MM.DisplayStructure( cout );

	C(0).Insert( "toto", 3 );
	C.Flush();
//	MM.DisplayStructure( cout );
#endif

	cout << C( 0 ) << txf::tab << txf::commit;
	cout << C( 1 ) << txf::nl << txf::commit;

//	MM.DisplayStructure( cout );
	
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
	str::string S;
	E_MITEM( str::string_ ) I1, I2;
ERRBegin
	S.Init();
	I1.Init( Liste );
	I2.Init( Liste );

	while ( Swap )
	{
		Swap = false;

		if ( Liste.Amount() )
			I1.Set( 0 );
//			S2 = Liste[0];

		for ( mdr::row_t__ i = 1; i < Liste.Amount(); i++ )
		{
			S = I1();
			I1.Set( i );
//			S2 = Liste[i];

			if ( S < I1() )
			{
//				Liste.Dynamique.Multimemoire()->AfficherStructure();

				Swap = true;

				I2(i-1) = I1();
				I1() = S;
				
				I2.Flush();

//				Liste.Dynamique.Multimemoire()->AfficherStructure();
			}
		}
	}

	I1.Flush();
	I2.Flush();

ERRErr
ERREnd
ERREpilog
}
void Remplir( str::string_ &S )
{
	int i = 10UL * rand() / RAND_MAX + 1;

	while ( i-- )
		S.Append( (char)( 'a' + ( 'z' - 'a' ) * rand() / RAND_MAX ) );
}



void PetitEssai( int argc, const char *argv[] )
{
ERRProlog
	mmm::multimemory M;
	E_MCONTAINER( str::string_ ) CS;
/*	fch_flot_sortie_fichier S;
	fch_flot_entree_fichier E;
*/	E_MITEM( str::string_ ) ECS;
	str::string Str;
	mdr::row__ P;
ERRBegin
	M.Init();
	CS.plug( M );
	CS.Init();
	CS.Allocate( 2 );
	ECS.Init( CS );

	Str.Init();

	ECS(0).Init();
	Str = "ab";
	ECS(0)= Str;

	ECS(1).Init();
	Str = "c";
	ECS(1)= Str;
	ECS.Flush();

	Str = "de";
	CS.Insert( Str, 1 );



	P = CS.First();
	
	while( P != NONE ) {
		cout << ECS(P) << txf::tab;
		P = CS.Next( P );
	}

	cout << txf::nl;

	ECS.Flush();

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
#endif

	P = CS.First();
	
	while( P != NONE ) {
		cout << ECS(P) << txf::tab;
		P = CS.Next( P );
	}

	ECS.Flush();

	cout << txf::nl;
ERRErr
ERREnd
ERREpilog

}

void Essai( int argc, const char *argv[] )
{
ERRProlog
	flm::id__ ID = FLM_UNDEFINED_ID;
	flm::E_FILE_MEMORY_DRIVER___ F;
	mmm::multimemory M;
	E_MCONTAINER( str::string_ ) CS, CD;
/*	fch_flot_sortie_fichier S;
	fch_flot_entree_fichier E;
*/	E_MITEM( str::string_ ) ECS, ECD;
	str::string Str;
	mdr::row__ P;
ERRBegin
	ID = flm::GetId();
	F.Init( ID );
	F.Automatic();
//	M.plug( F );
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
		cout << ECS(P) << txf::tab;
		P = CS.Next( P );
	}

	cout << txf::nl;

	ECS.Flush();

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
		cout << ECD(P) << txf::tab;
		P = CD.Next( P );
	}

	ECD.Flush();

	cout << txf::nl;

	CD.Remove( 3 );

	P = CD.First();
	
	while( P != NONE ) {
		cout << ECD(P) << txf::tab;
		P = CD.Next( P );
	}

	cout << txf::nl;
ERRErr
ERREnd
	if ( ID != FLM_UNDEFINED_ID )
		flm::ReleaseId( ID );
ERREpilog

}

#define A( m )	( m + m )
#define A_( m ) ( m - m )

void EssaiBogue( int argc, const char *argv[] )
{
ERRProlog
	str::string S;
	E_MCONTAINER( str::string_ ) CS;
	E_CONTAINER( E_MCONTAINER_( str::string_ ) ) CC;
//	E_XCONTAINER( E_XCONTAINER_( E_XMCONTAINER_( str::string_ ) ) ) CM;
ERRBegin
	CC.Init();
//	CC.Dynamics.Multimemoire.DisplayStructure( cio::cout );

	CC.Dynamics.Multimemoire.Preallocate( 100000 );
//	CC.Dynamics.Multimemoire.DisplayStructure( cio::cout );

	CS.Init();
//	CS.Dynamics.Multimemoire.Preallocate( 100000 );

	CC.Allocate( 1 );
	CC.Dynamics.Multimemoire.DisplayStructure( cout );
//
	CS.Allocate( 1 );
//	CS.Dynamics.Multimemoire.DisplayStructure( cio::cout );

	S.Init();

	S.Append( "Aa0" );

	CS( 0 ).Init();
	CS() = S;

	cout << CS( 0 ) << " ";
	CS.Flush();

	CS.Dynamics.Multimemoire.DisplayStructure( cout );

	cout << '\t';

	CC( 0 ).Init();
	CC.Dynamics.Multimemoire.DisplayStructure( cout );
	CC() = CS;

	CC.Flush();

	CS.Dynamics.Multimemoire.DisplayStructure( cout );
	CC.Dynamics.Multimemoire.DisplayStructure( cout );

	cout << txf::nl;

	CS = CC( 0 );

	CS.Dynamics.Multimemoire.DisplayStructure( cout );

	S = CS( 0 );

	cout << S << ' ' << txf::commit;

	cout << '\t';

	cout << txf::nl;
	
ERRErr
ERREnd
ERREpilog
}

void EssaiCopie( int argc, const char *argv[] )
{
ERRProlog
	flm::id__ ID = FLM_UNDEFINED_ID;
	int a = A( A_( 3 ) );
	flm::E_FILE_MEMORY_DRIVER___ F;
	mmm::multimemory Mm;
	E_MCONTAINER( str::string_ ) CC;
	E_CONTAINER( E_MCONTAINER_( str::string_ ) ) Cm;
	E_CONTAINER( E_CONTAINER_( E_MCONTAINER_( str::string_ ) ) ) CM;
	str::string S;
/*	fch_flot_sortie_fichier FO;
	fch_flot_entree_fichier FI;
*/	E_ITEM( E_CONTAINER_( E_MCONTAINER_( str::string_ ) ) ) ECM;
//	ITEM( MCONTAINER_( str::string_ ) ) ECm;
	E_MITEM( str::string_ ) ECC;
	char M, m, C;
ERRBegin
	ID = flm::GetId();
	F.Init( ID, "a.tmp" );
//	F.Manuel();
	Mm.plug( F );
	Mm.Init();
	Mm.Preallocate( 10000000 );
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

				S.Append( M );
				S.Append( m );
				S.Append( C );

				ECC( C - '0' ).Init();
				ECC() = S;
//				fout << S << " ";
				cout << ECC(C - '0') << " ";

//				CC.Dynamics.Multimemoire.DisplayStructure( cio::cout );

			}

			ECC.Flush();

			cout << '\t';
/*
			ECm( m - 'a' ).Init();
			ECm() = CC;
*/
			Cm( m - 'a' ).Init();
			Cm() = CC;

//			Cm.Dynamics.Multimemoire.DisplayStructure( cio::cout );
		}

//		ECm.Flush();
		Cm.Flush();

//		Mm.DisplayStructure( cio::cout );

		cout << txf::nl;
		ECM(M - 'A').Init();
		ECM() = Cm;

//		Mm.DisplayStructure( cio::cout );
	}

	cout << "--------------" << txf::nl;

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

				cout << S << ' ' << txf::commit;
			}

			cout << '\t';

		}

		cout << txf::nl;
	}
/*
	CMS.Mode( plm::mModification );
	F.Mode( plm::mModification );
*/
	// Pour que l'objet puisse se d�truire.

ERRErr
	// instructions � ex�cuter si erreur
ERREnd
	// instructions � ex�cuter, erreur ou non
ERREpilog
}


void EssaiPersistence( int argc, const char *argv[] )
{
ERRProlog
	flm::id__ ID = FLM_UNDEFINED_ID;
	int a = A( A_( 3 ) );
	E_MCONTAINER( str::string_ ) CC;
	E_CONTAINER( E_MCONTAINER_( str::string_ ) ) Cm;
	E_CONTAINER( E_CONTAINER_( E_MCONTAINER_( str::string_ ) ) ) CM;
	ctn::container_file_manager___<E_CONTAINER_( E_CONTAINER_( E_MCONTAINER_( str::string_ ) ) )> FileManager;
	str::string S;
/*	fch_flot_sortie_fichier FO;
	fch_flot_entree_fichier FI;
*/	E_ITEM( E_CONTAINER_( E_MCONTAINER_( str::string_ ) ) ) ECM;
//	ITEM( MCONTAINER_( str::string_ ) ) ECm;
	E_MITEM( str::string_ ) ECC;
	char M, m, C;
ERRBegin
	ID = flm::GetId();
	FileManager.Init( "Test.cst", "Test.cdn", "Test.cmm", "Test.cmf", fil::mReadWrite, true, ID );

	Cm.Init();
	CC.Init();

	Cm.Allocate( Lm - 'a' + 1 );
	CC.Allocate( LC - '0' + 1 );

	ECC.Init( CC );
//	ECm.Init( Cm );
	ECM.Init( CM );

	CM.Init();

	if ( !ctn::Plug( CM, FileManager ) ) {

		cout << "***** CREATION *****" << txf::nl << txf::commit;


		CM.Allocate( LM - 'A' + 1 );

		for ( M = 'A'; M <= LM; M++ )
		{
			for ( m = 'a'; m <= Lm; m++ )
			{
				for ( C = '0'; C <= LC; C++ )
				{
					S.Init();
					S.SetStepValue( 0 );

					S.Append( M );
					S.Append( m );
					S.Append( C );

					ECC( C - '0' ).Init();
					ECC() = S;
	//				fout << S << " ";
					cout << ECC(C - '0') << " ";

				}

				ECC.Flush();

				cout << '\t';
	/*
				ECm( m - 'a' ).Init();
				ECm() = CC;
	*/
				Cm( m - 'a' ).Init();
				Cm() = CC;

			}

	//		ECm.Flush();
			Cm.Flush();

			cout << txf::nl;
			ECM(M - 'A').Init();
			ECM() = Cm;
		}
	} else {
		cout << "***** RECUPERATION *****" << txf::nl << txf::commit;
	}

	CM.Dynamics.Multimemoire.DisplayStructure( cout );


	cout << "--------------" << txf::nl;

	for ( M = 'A'; M <= LM; M++ )
	{
		Cm = ECM(M - 'A');

		for ( m ='a'; m <= Lm; m++ )
		{
//			CC = ECm(m - 'a');
			CC = Cm(m - 'a');

			for ( C = '0'; C <= LC; C++ )
			{
				S.Init();
				S = ECC(C - '0');

				cout << S << ' ' << txf::commit;
			}

			cout << '\t';

		}

		cout << txf::nl;
	}
/*
	CMS.Mode( plm::mModification );
	F.Mode( plm::mModification );
*/
	// Pour que l'objet puisse se d�truire.

ERRErr
	// instructions � ex�cuter si erreur
ERREnd
	if ( ID != FLM_UNDEFINED_ID )
		flm::ReleaseId( ID );
	// instructions � ex�cuter, erreur ou non
ERREpilog
}


void SP2(
	E_MCONTAINER_( str::string_ ) &O,
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


		E(C - '0').Append( M );
		E(C - '0').Append( m );
		E(C - '0').Append( C );

//		E(C - '0').Adjust();

		cout << E(C - '0') << " " << txf::commit;
	}
ERRErr
ERREnd
ERREpilog
}


void SP1(
	E_CONTAINER_( E_MCONTAINER_( str::string_ ) ) &O,
	char M)
{
ERRProlog
	E_ITEM( E_MCONTAINER_( str::string_ ) ) E;
ERRBegin
	E.Init( O );

	for ( char m = 'a'; m <= Lm; m++ )
	{
		E(m - 'a').Init();
		E(m - 'a').Allocate( LC - '0' + 1 );

		SP2( E( m - 'a' ), M, m );

		cout << '\t';
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
		cout << E(C - '0') << ' ' << txf::commit;
	}
ERRErr
ERREnd
ERREpilog
}


void SSP1(
	const E_CONTAINER_( E_MCONTAINER_( str::string_ ) ) &O,
	char M)

{
ERRProlog
	E_CITEM( E_MCONTAINER_( str::string_ ) ) E;
ERRBegin
	E.Init( O );

	for ( char m = 'a'; m <= Lm; m++ )
	{
		SSP2( E(m - 'a'), M, m );

		cout << txf::tab << txf::commit;
	}
ERRErr
ERREnd
ERREpilog
}



void EssaiDirect( int argc, const char *argv[] )
{
ERRProlog
	flm::id__ ID = FLM_UNDEFINED_ID;
	flm::E_FILE_MEMORY_DRIVER___ F;
	mmm::multimemory Mm;
//	CONTAINER( CONTAINER_( ctn_conteneur_polymemoire_< UTL_2EN1( str::string_, str::string_::s ) > ) ) GC;
	E_CONTAINER( E_CONTAINER_( E_MCONTAINER_( str::string_ ) ) ) GC;
	E_ITEM( E_CONTAINER_( E_MCONTAINER_( str::string_ ) ) ) EGC;
	char M;
ERRBegin
	ID = flm::GetId();
	F.Init( ID, "b.tmp");
	F.Manual();
	Mm.plug( F );
	Mm.Init();
	Mm.Preallocate( 10000000 );
	GC.plug( Mm );
	GC.Init();
	GC.Allocate( LM - 'A' + 1 );

	EGC.Init( GC );

	for ( M = 'A'; M <= LM; M++ )
	{
		EGC(M - 'A').Init();
//		GC[M - 'A'].Multimemoire.Optimisation( mmm::Temps );
		EGC(M - 'A').Allocate( Lm - 'a' + 1 );

		SP1( EGC(M - 'A' ), M );

		cout << txf::nl;
	}

//	Mm.DisplayStructure( cio::cout );

	cout << "--------------" << txf::nl;

	for ( M = 'A'; M <= LM; M++ )
	{
		SSP1( EGC(M - 'A'), M );

		cout << txf::nl;

	}

//	Mm.DisplayStructure( cio::cout );

ERRErr
	// instructions � ex�cuter si erreur
ERREnd
	if ( ID != FLM_UNDEFINED_ID )
		flm::ReleaseId( ID );
	// instructions � ex�cuter, erreur ou non
ERREpilog
}

void EssaiSimpleMono( void )
{
ERRProlog
	flm::id__ ID = FLM_UNDEFINED_ID;
	flm::E_FILE_MEMORY_DRIVER___ F;
	mmm::multimemory M;
	E_MCONTAINER( str::string_ ) C;
	E_MITEM( str::string_ ) E;
ERRBegin
	ID = flm::GetId();
	F.Init( ID, "coucou.tmp" );
//	M.plug( F );
	M.Init();
	C.plug( M );
	C.Init();
	C.Allocate( 2 );
	E.Init( C );

	E(0).Init();
	E(0) = str::string( "ca" );
	E(1).Init();
	E(1)= str::string( "bte" );
	cout << E(0) << txf::tab;
	cout << E(1) << txf::nl;
ERRErr
ERREnd
	if ( ID != FLM_UNDEFINED_ID )
		flm::ReleaseId( ID );
ERREpilog
}

void EssaiSimpleMulti( void )
{
ERRProlog
	flm::id__ ID = FLM_UNDEFINED_ID;
	flm::E_FILE_MEMORY_DRIVER___ F;
	mmm::multimemory M;
	E_CONTAINER( str::string_ ) C;
	E_ITEM( str::string_ ) E;
ERRBegin
	ID = flm::GetId();
	F.Init( ID, "coucou.tmp" );
	M.plug( F );
	M.Init();
	C.plug( M );
	C.Init();
	C.Allocate( 2 );
	E.Init( C );

	E(0).Init();
	E(0) = str::string( "ca" );
	E(1).Init();
	E(1)= str::string( "bte" );
	cout << E(0) << txf::tab;
	cout << E(1) << txf::nl;
ERRErr
ERREnd
	if ( ID != FLM_UNDEFINED_ID )
		flm::ReleaseId( ID );
ERREpilog
}

void EssaiConteneurDansConteneur( void )
{
ERRProlog
	flm::id__ ID = FLM_UNDEFINED_ID;
	flm::E_FILE_MEMORY_DRIVER___ F;
	mmm::multimemory M;
	E_CONTAINER( str::string_ ) CS;
	E_CONTAINER( E_CONTAINER_( str::string_ ) ) CC;
//	ITEM( str::string_ ) ECS;
//	ITEM( CONTENEUR_( str::string_ ) ) ECC;
ERRBegin
	ID = flm::GetId();
	F.Init( ID, "temp.tmp" );
	M.plug( F );
	M.Init();

//	CS.plug( M );
	CS.Init();

//	CC.plug( M );
	CC.Init();

//	ECS.Init( CS );
//	ECC.Init( CC );

	CS.Allocate( 1 );

	CS(0).Init();
	CS() = "coucou";
	CS.Flush();

	CC.Allocate( 2 );

	CC(0).Init();
	CC() = CS;
	CC.Flush();

	cout << CS( 0 ) << txf::tab << txf::commit;

	CS(0).Init();
	CS() = "hello";
	CS.Flush();

	CC(1).Init();
	CC() = CS;
	CC.Flush();

	cout << CS( 0 ) << txf::tab << txf::commit;

	CS = CC( 0 );
	CC.Flush();
	cout << CS( 0 ) << txf::tab;

	CS = CC( 1 );
	CC.Flush();
	cout << CS( 0 ) << txf::nl;

	CS.Flush();
ERRErr
ERREnd
	if ( ID != FLM_UNDEFINED_ID )
		flm::ReleaseId( ID );
ERREpilog
}




int main( int argc, const char *argv[] )
{
	int ExitCode = EXIT_SUCCESS;
ERRFProlog
ERRFBegin
	cout << "Test of library " << CTNTutor.Name << ' ' << __DATE__" "__TIME__"\n";

	switch( argc ) {
	case 1:
		Generic( argc, argv );
#if 1
		EssaiBasic();
//		EssaiConteneurDansConteneur();
//		EssaiSimpleMono();
//		EssaiSimpleMulti();
		PetitEssai( argc, argv );
//		Essai( argc, argv );
//		cout << "********************************************************" << txf::nl;
////		EssaiDirect( argc, argv );
//		cout << "********************************************************" << txf::nl;
//		EssaiBogue( argc, argv);
//		EssaiCopie( argc, argv );
		cout << "********************************************************" << txf::nl;
		EssaiPersistence( argc, argv );
#else
		BugTracking();
#endif
		break;
	case 2:
		if ( !strcmp( argv[1], "/i" ) )
		{
			TTR.Advertise( cout );
			break;
		}
	default:
		cout << txf::commit;
		cerr << "\nBad arguments.\n";
		cout << "Usage: " << CTNTutor.Name << " [/i]\n\n";
		ERRExit( EXIT_SUCCESS );
	}

ERRFErr
	ExitCode = EXIT_FAILURE;
ERRFEnd
	cout << "\nEnd of program " << CTNTutor.Name << ".\n";
ERRFEpilog
	return ExitCode;
}
