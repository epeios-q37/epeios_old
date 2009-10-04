/*
	Test source for the 'lstctn' library by Claude SIMON (csimon@epeios.org).
	Copyright (C) 2004 Claude SIMON (csimon@epeios.org).

	This file is part of the Epeios (http://epeios.org/) project.

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

#include "lstctn.h"

#include "err.h"
#include "cio.h"

#include "str.h"

using cio::cin;
using cio::cout;
using cio::cerr;

void Generic( int argc, const char *argv[] )
{
ERRProlog
	lstctn::E_LCONTAINER( str::string_ ) Container;
	lstctn::list_container_file_manager___ FileManager;
ERRBegin
ERRErr
ERREnd
ERREpilog
}

using namespace lstctn;

#define LM	'D'
#define Lm	'a'
#define LC	'1'

using namespace ctn;

void Essai( int argc, const char *argv[] )
{
ERRProlog
//	int a = A( A_( 3 ) );
	E_XMCONTAINER( str::string_ ) CC;
	E_XCONTAINER( E_XMCONTAINER_( str::string_ ) ) Cm;
	E_LXCONTAINER( E_XCONTAINER_( E_XMCONTAINER_( str::string_ ) ) ) CM;
	list_container_file_manager___ FileManager;
	str::string S;
	ctn::E_ITEM( E_XCONTAINER_( E_XMCONTAINER_( str::string_ ) ) ) ECM;
	ctn::E_MITEM( str::string_ ) ECC;
	char M, m, C;
ERRBegin
	FileManager.Init( "Test.cst", "Test.cdn", "Test.cmm", "Test.lst", mdr::mReadWrite, true );

	Cm.Init();
	CC.Init();

	Cm.Allocate( Lm - 'a' + 1 );
	CC.Allocate( LC - '0' + 1 );

	ECC.Init( CC );
//	ECm.Init( Cm );
	ECM.Init( CM );

	CM.Init();

	if ( !Connect( CM, FileManager ) ) {

		cout << "***** CREATION *****" << txf::nl << txf::sync;

		CM.Allocate( LM - 'A' + 1 );

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
/*
		CM.Delete( CM.First( 1 ) );
		CM.Delete( CM.Last( 1 ) );
		CM.Delete( CM.Last( 1 ) );
*/
	} else {
		cout << "***** RECUPERATION *****" << txf::nl << txf::sync;
	}

	CM.Delete( CM.Last( 1 ) );

	cout << "--------------" << txf::nl;

	ECM.ChangeMode( mdr::mReadOnly );
//	ECm.ChangeMode( mdr::mReadOnly );
	ECC.ChangeMode( mdr::mReadOnly );

	epeios::row__ RM = CM.First();

	while ( RM != NONE ) {
		Cm = ECM( RM );

		for ( m ='a'; m <= Lm; m++ )
		{
//			CC = ECm(m - 'a');
			CC = Cm(m - 'a');

			for ( C = '0'; C <= LC; C++ )
			{
				S.Init();
				S = ECC(C - '0');

				cout << S << ' ' << txf::sync;
			}

			cout << '\t';

		}

		cout << txf::nl;

		RM = CM.Next( RM );
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



int main( int argc, const char *argv[] )
{
ERRFProlog
ERRFBegin
	cout << "Test of library " << LSTCTNTutor.Name << ' ' << __DATE__" "__TIME__"\n";

	switch( argc ) {
	case 1:
		Generic( argc, argv );
		Essai( argc, argv );
		break;
	case 2:
		if ( !strcmp( argv[1], "/i" ) )
		{
			TTR.Advertise( cout );
			break;
		}
	default:
		cout << txf::sync;
		cerr << "\nBad arguments.\n";
		cout << "Usage: " << LSTCTNTutor.Name << " [/i]\n\n";
		ERRi();
	}

ERRFErr
ERRFEnd
ERRFEpilog
	return ERRExitValue;
}
