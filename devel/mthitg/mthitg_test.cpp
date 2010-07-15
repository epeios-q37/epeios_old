/*
  Test source for the 'mthitg' library by Claude SIMON (http://zeusw.org/intl/contact.html).
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
/*
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
*/
#include <math.h>

#include "mthitg.h"

#include "err.h"
#include "cio.h"

void Generic( int argc, char *argv[] )
{
ERRProlog
ERRBegin
ERRErr
ERREnd
ERREpilog
}

void EssaiSimple( void )
{
	mthitg::integer E1, E2, E3;

	E1.Init();
	E2.Init();
	E3.Init();

	E1 = 567;
	E2 = 345678;
	E3 = 4567890;

	cio::cout << E1 << txf::tab << E2 << txf::tab << E3 << txf::nl << txf::sync;

	E3 /= E1;
	E2 *= E2;
//	E2 = E3;

	cio::cout << E2 << txf::tab << E3 << txf::nl;
}

mthitg::integer Rand( void )
{
	return mthitg::integer( ::rand() + 1UL );
}

#define LIMITE_RATIO	1.0E-15

int Hasard( void )
{
ERRProlog
	mthitg::integer E1, E2, E, A, M;
	long double D, R, Diff, Ratio;
	double Dummy;
	unsigned long Iter = 0, Echec = 0;
//	char C;
ERRBegin
	E1.Init();
	E2.Init();
	E.Init();
	A.Init();
	M.Init();

	for(;;)
	{
		E1 = Rand()
			* Rand()
			* Rand()
			* Rand()
			* Rand()
			* Rand()
			* Rand()
			* Rand()
			* Rand()
			* Rand()
			* Rand()
			* Rand()
			* Rand()
			* Rand()
			* Rand()
			* Rand()
			* Rand()
			* Rand()
			* Rand()
			* Rand()
			* Rand()
			* Rand()
			* Rand()
			* Rand()
			* Rand()
			* Rand()
			* Rand()
			* Rand()
			* Rand()
			* Rand()
			* Rand()
			* Rand();

		cio::cout << "Op. 1:\t" << E1.GetLongFloat() << '\t' << E1 << txf::nl;

		E2 = Rand()
			* Rand()
			* Rand()
			* Rand()
			* Rand()
			* Rand()
			* Rand()
			* Rand()
			* Rand()
			* Rand()
			* Rand()
			* Rand()
			* Rand()
			* Rand()
			* Rand()
			* Rand()
			* Rand();

		cio::cout << "Op. 2:\t" << E2.GetLongFloat() << '\t' << E2 << txf::nl;
	
// Addition

		D = E1.GetLongFloat() + E2.GetLongFloat();
		Iter++;

		R = ( A = E = E1 + E2 ).GetLongFloat();

		cio::cout << "** + **\tDifférence résultat:\t" << ( Diff = R - D ) << '\t';

		if ( R != 0 )
			cio::cout << "Ratio:\t" << ( Ratio = fabs( Diff / R ) );

		cio::cout << "\n\tRésultat:\t" << D << '\t' << E << txf::nl;

		if ( Ratio > LIMITE_RATIO )
		{
			Echec++;
//			cio::cout << "\a\a*****************************************" << txf::nl;
		}

// Soustraction

		D = E1.GetLongFloat() - E2.GetLongFloat();
		Iter++;

		R = ( E = E1 - E2 ).GetLongFloat();

		cio::cout  << "** - **\tDifférence résultat:\t" << ( Diff = R - D ) << '\t';

		if ( R != 0 )
			cio::cout << "Ratio:\t" << ( Ratio = fabs( Diff / R ) );

		cio::cout << "\n\tRésultat:\t" << D << '\t' << E << txf::nl;

		if ( Ratio > LIMITE_RATIO )
		{
//			cio::cout << "\a\a*****************************************" << txf::nl;
			Echec++;
		}

// Multiplication
		D = E1.GetLongFloat() * E2.GetLongFloat();
		Iter++;

		R = ( M = E = E1 * E2 ).GetLongFloat();

		cio::cout << "** * **\tDifférence résultat:\t" << ( Diff = ( R - D )) << '\t';

		if ( R != 0 )
			cio::cout << "Ratio:\t" << ( Ratio = fabs( Diff / R ) );

		cio::cout << "\n\tRésultat:\t" << D << '\t' << E << txf::nl;

		if ( Ratio > LIMITE_RATIO )
		{
			Echec++;
//			cio::cout << "\a\a*****************************************" << txf::nl;
		}

// Division

		if ( E2.GetLongFloat() != 0.0 )
		{
			modf( E1.GetLongFloat() / E2.GetLongFloat(), &Dummy );

			D = Dummy;

			Iter++;

			R = ( E = E1 / E2 ).GetLongFloat();

			cio::cout << "** / **\tDifférence résultat:\t" << ( Diff = R - D )<< '\t';

			if ( R != 0 )
				cio::cout << "Ratio:\t" << ( Ratio = fabs( Diff / R ) );

			cio::cout << "\n\tRésultat:\t" << D << '\t' << E << txf::nl;

			if ( Ratio > LIMITE_RATIO )
			{
				Echec++;
//				cio::cout << "\a\a*****************************************" << txf::nl;
			}
		}

		cio::cout << "$$$" << txf::nl;

		Iter++;
		cio::cout << A << '\t' << ( A - E1 ) << '\t' << E2 << txf::nl;

		if ( ( A - E1 ) != E2 )
		{
			Echec++;
//			cio::cout << "\a\a$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$" << txf::nl;
		}

		cio::cout << "$$$" << txf::nl;

		Iter++;

		cio::cout << M << '\t' << txf::sync;
		cio::cout << ( M / E1 ) << '\t' << txf::sync;
		cio::cout << E2 << '\t' << txf::sync;
		cio::cout << ( M % E1 ) << txf::nl;

		if ( ( M / E1 ) != E2 )
		{
			Echec++;
//			cio::cout << "\a\a$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$" << txf::nl;
		}

		cio::cout << "$$$$$" << txf::nl;

		cio::cout << "\nCompte-rendu:\t" << "E: " << Echec << '\t' << "I: "
			<< Iter << '\t' << "R: " << (bso::lfloat__)( (double)Echec / double(Iter) ) << "\n" << txf::nl;

//		fin >> C;
	}
ERRErr
ERREnd
ERREpilog

	return ~0;
}

void BogueDivision( void )
{
	mthitg::integer I = 3600;
	bso::lfloat__ F = 3600;

	cio::cout << F << txf::tab << I << txf::nl;

	I *= mthitg::integer( 30 );
	F *= 30;

	cio::cout << F << txf::tab << I << txf::nl;

	I /= mthitg::integer( 1 );
	F /= 1;

	cio::cout << F << txf::tab << I << txf::nl;
}


int main( int argc, char *argv[] )
{
	int ExitCode = EXIT_SUCCESS;
ERRFProlog
ERRFBegin
	cio::cout << "Test of library " << MTHITGTutor.Name << ' ' << __DATE__" "__TIME__"\n";

	switch( argc ) {
	case 1:
		Generic( argc, argv );
		BogueDivision();
		EssaiSimple();
		Hasard();
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
		cio::cout << "Usage: " << MTHITGTutor.Name << " [/i]\n\n";
		ERRi();
	}

ERRFErr
	ExitCode = EXIT_FAILURE;
ERRFEnd
	cio::cout << "\nEnd of program " << MTHITGTutor.Name << ".\n";
ERRFEpilog
	return ExitCode;
}
