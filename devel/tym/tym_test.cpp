/*
  Test source for the 'tym' library by Claude L. Simon (simon@epeios.org).
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

#include "tym.h"

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


typedef bso__ushort	mytype__;

#define TAILLE_MEMOIRE	100

void Essai( void )
{
ERRProlog
/*	mfc_pilote_memoire_fichier R1;
	mfc_pilote_memoire_fichier R2;
*/	tym::E_MEMORY( mytype__ ) M1;
	tym::E_MEMORY( mytype__ ) M2;
	bso__ulong I;
	mytype__ J;
	ifstream IN;
	ofstream OUT;
ERRBegin
/*	R1.Init();
	R2.Init();
*/
	M1.Init();
	M1.Allocate( TAILLE_MEMOIRE );

	M2.Init();
	M2.Allocate( TAILLE_MEMOIRE );

	for ( I = 0; I < TAILLE_MEMOIRE; I++ )
	{
		J = (mytype__)I + 1;
		M1.Write( 60000 + J, I );
	}


	for ( I = 0; I < TAILLE_MEMOIRE; I++ )
	{
		M1.Read( I, J );
		fout << J << '\t' << txf::sync;
	}

	fout << txf::nl;

	M1.Read( 0, TAILLE_MEMOIRE, M2 );

	for ( I = 0; I < TAILLE_MEMOIRE; I++ )
	{
		fout << M2.Read( I ) << '\t' << txf::sync;
	}

	fout << '\n';

ERRErr
ERREnd
ERREpilog
}




int main( int argc, char *argv[] )
{
	int ExitCode = EXIT_SUCCESS;
ERRFProlog
ERRFBegin
	fout << "Test of library " << TYMTutor.Name << ' ' << __DATE__" "__TIME__"\n";

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
		fout << "Usage: " << TYMTutor.Name << " [/i]\n\n";
		ERRt();
	}

ERRFErr
	ExitCode = EXIT_FAILURE;
ERRFEnd
	fout << "\nEnd of program " << TYMTutor.Name << ".\n";
ERRFEpilog
	return ExitCode;
}
