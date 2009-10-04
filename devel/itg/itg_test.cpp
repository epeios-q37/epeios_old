/*
  Test source for the 'itg' library by Claude SIMON (http://zeusw.org/intl/contact.html).
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
#include <math.h>

#include "itg.h"

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

void EssaiSimple( void )
{
	itg::integer E1, E2, E3;

	E1.Init();
	E2.Init();
	E3.Init();

	E1 = 234567;
	E2 = 345678;
	E3 = 4567890;

	fout << E1 << txf::tab << E2 << txf::tab << E3 << txf::nl << txf::sync;

	E3 /= E1;
	E2 *= E2;
//	E2 = E3;

	fout << E2 << txf::tab << E3 << txf::nl;
}

int unsigned long Rand( void )
{
	return ::rand() + 1UL;
}

#define LIMITE_RATIO	1.0E-15

int Hasard( void )
{
ERRProlog
	itg::integer E1, E2, E, A, M;
	long double D, R, Diff, Ratio;
	unsigned long Iter = 0, Echec = 0;
	char C;
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
			* Rand();

		fout << "Op. 1:\t" << E1.GetLongFloat() << '\t' << E1 << '\n';

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
			* Rand()
			- Rand()
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


		fout << "Op. 2:\t" << E2.GetLongFloat() << '\t' << E2 << '\n';
	
// Addition

		D = E1.GetLongFloat() + E2.GetLongFloat();
		Iter++;

		R = ( A = E = E1 + E2 ).GetLongFloat();

		fout << "** + **\tDifférence résultat:\t" << ( Diff = R - D ) << '\t';

		if ( R != 0 )
			fout << "Ratio:\t" << ( Ratio = fabs( Diff / R ) );

		fout << "\n\tRésultat:\t" << D << '\t' << E << "\n";

		if ( Ratio > LIMITE_RATIO )
		{
			Echec++;
			fout << "\a\a*****************************************\n";
		}

// Soustraction

		D = E1.GetLongFloat() - E2.GetLongFloat();
		Iter++;

		R = ( E = E1 - E2 ).GetLongFloat();

		fout  << "** - **\tDifférence résultat:\t" << ( Diff = R - D ) << '\t';

		if ( R != 0 )
			fout << "Ratio:\t" << ( Ratio = fabs( Diff / R ) );

		fout << "\n\tRésultat:\t" << D << '\t' << E << "\n";

		if ( Ratio > LIMITE_RATIO )
		{
			fout << "\a\a*****************************************\n";
			Echec++;
		}

// Multiplication

		D = E1.GetLongFloat() * E2.GetLongFloat();
		Iter++;

		R = ( M = E = E1 * E2 ).GetLongFloat();

		fout << "** * **\tDifférence résultat:\t" << ( Diff = ( R - D )) << '\t';

		if ( R != 0 )
			fout << "Ratio:\t" << ( Ratio = fabs( Diff / R ) );

		fout << "\n\tRésultat:\t" << D << '\t' << E << "\n";

		if ( Ratio > LIMITE_RATIO )
		{
			Echec++;
			fout << "\a\a*****************************************\n";
		}


// Division

		if ( E2.GetLongFloat() != 0.0 )
		{
			D = E1.GetLongFloat() / E2.GetLongFloat();
			Iter++;

			R = ( E = E1 / E2 ).GetLongFloat();

			fout << "** / **\tDifférence résultat:\t" << ( Diff = R - D )<< '\t';

			if ( R != 0 )
				fout << "Ratio:\t" << ( Ratio = fabs( Diff / R ) );

			fout << "\n\tRésultat:\t" << D << '\t' << E << "\n";

			if ( Ratio > LIMITE_RATIO )
			{
				Echec++;
				fout << "\a\a*****************************************\n";
			}
		}

		fout << "$$$\n";

		Iter++;
		fout << A << '\t' << ( A - E1 ) << '\t' << E2 << '\n';

		if ( ( A - E1 ) != E2 )
		{
			Echec++;
			fout << "\a\a$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n";
		}

		fout << "$$$\n";

		Iter++;

		fout << M << '\t' << ( M / E1 ) << '\t' << E2 << '\t' << ( M % E1 ) << '\n';

		if ( ( M / E1 ) != E2 )
		{
			Echec++;
			fout << "\a\a$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n";
		}

		fout << "$$$$$\n";

		fout << "\nCompte-rendu:\t" << "E: " << Echec << '\t' << "I: "
			<< Iter << '\t' << "R: " << (bso__lfloat)( (double)Echec / double(Iter) ) << "\n\n";

		fin >> C;
	}
ERRErr
ERREnd
ERREpilog

	return ~0;
}


int main( int argc, char *argv[] )
{
	int ExitCode = EXIT_SUCCESS;
ERRFProlog
ERRFBegin
	fout << "Test of library " << ITGTutor.Name << ' ' << __DATE__" "__TIME__"\n";

	switch( argc ) {
	case 1:
		Generic( argc, argv );
		EssaiSimple();
		Hasard();
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
		fout << "Usage: " << ITGTutor.Name << " [/i]\n\n";
		ERRt();
	}

ERRFErr
	ExitCode = EXIT_FAILURE;
ERRFEnd
	fout << "\nEnd of program " << ITGTutor.Name << ".\n";
ERRFEpilog
	return ExitCode;
}
