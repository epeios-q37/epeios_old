/*
  Test source for the 'bch' library by Claude L. Simon (simon@epeios.org).
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

void Essai( void )
{
ERRProlog
	flm::file_memory_driver F;
	mmm::multimemory M;
	E_BUNCH( bso__ulong ) E1, E2;
//	fch_flot_entree_fichier Entree;
//	fch_flot_sortie_fichier Sortie;
	E_BUNCH__( int, 3 ) G, H;
	bso__ulong i;
	row__ P;
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
		E1.Write( i, i );
		E2.Write( (bso__ulong)( NOMBRE - i ), i );
	}

	for ( i = 0; i < E1.Amount(); i++ )
		fout << E1(i) << tab << E2(i) << tab;

	fout << nl;

	fout << ">>>>>>>>> Comparaison: "<< (long)Compare( E1, E2 ) << nl;

	E1.Insert( E2, NOMBRE / 2 );

	for ( P = 0; P < E1.Amount(); P++ )
		fout << E1( P ) << tab;

	fout << nl;

	fout << ">>>>>>>>> Comparaison: "<< (long)Compare( E1, E2 ) << nl;

	for ( P = 0; P < E2.Amount(); P++ )
		fout << E2( P ) << tab;

	fout << nl;

	E1 = E2;

	fout << ">>>>>>>>> Comparaison: "<< (long)Compare( E1, E2 ) << nl;

ERRErr
	// instructions � ex�cuter si erreur
ERREnd
	// instructions � ex�cuter, erreur ou non
ERREpilog
}


int main( int argc, char *argv[] )
{
	int ExitCode = EXIT_SUCCESS;
ERRFProlog
ERRFBegin
	fout << "Test of library " << BCHTutor.Name << ' ' << __DATE__" "__TIME__"\n";

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
		fout << txf::sync;
		ferr << "\nBad arguments.\n";
		fout << "Usage: " << BCHTutor.Name << " [/i]\n\n";
		ERRt();
	}

ERRFErr
	ExitCode = EXIT_FAILURE;
ERRFEnd
	fout << "\nEnd of program " << BCHTutor.Name << ".\n";
ERRFEpilog
	return ExitCode;
}
