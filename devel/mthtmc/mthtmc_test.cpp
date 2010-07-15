/*
	Test source for the 'mthtmc' library by Claude SIMON (csimon at zeusw dot org).
	Copyright (C) 2004 Claude SIMON.

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

#include "mthtmc.h"

#include "err.h"
#include "cio.h"

using cio::cin;
using cio::cout;
using cio::cerr;

void Test( int argc, char *argv[] )
{
ERRProlog
	mthtmc::xfps XFPS;
	mthtmc::stamped_frame_count SFC;
	mthfrc::fraction Fraction;
ERRBegin
	XFPS.Init( 1, mthtmc::tNDF );

	SFC.Init( mthitg::integer( 4294967295 ), XFPS );

	Fraction.Init();

	SFC.GetFraction( Fraction );

	cio::cout << SFC.FrameCount << txf::tab << Fraction;

	cio::cout << txf::nl;

	XFPS.Init( 25, mthtmc::tNDF );

	mthtmc::Display( SFC, XFPS, str::string( "%F - %.1F" ), '.', '.', '.','.', cio::cout );

	cio::cout << txf::nl;
ERRErr
ERREnd
ERREpilog
}

void Generic( int argc, char *argv[] )
{
ERRProlog
ERRBegin
ERRErr
ERREnd
ERREpilog
}

int main( int argc, char *argv[] )
{
ERRFProlog
ERRFBegin
	cout << "Test of library " << MTHTMCTutor.Name << ' ' << __DATE__" "__TIME__"\n";

	switch( argc ) {
	case 1:
		Generic( argc, argv );
		Test( argc, argv );
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
		cout << "Usage: " << MTHTMCTutor.Name << " [/i]\n\n";
		ERRi();
	}

ERRFErr
ERRFEnd
ERRFEpilog
	return ERRExitValue;
}
