/*
	Test source for the 'mmm2' library by Claude SIMON (csimon@epeios.org).
	Copyright (C) 2004 Claude SIMON (csimon@epeios.org).

	This file is part of the Epeios (http://epeios.org/) project.

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

#include "mmm2.h"

#include "err.h"
#include "cio.h"
#include "mmi.h"
#include "str.h"

using cio::cin;
using cio::cout;
using cio::cerr;

using namespace mmm;

#define D\
	Multimemory.DisplayStructure( cio::cout );\
	cio::cout << "-------------------" << txf::nl;

#define I( s )\
	s.plug( Multimemory );\
	s.Init();\
	s.SetStepValue( 0 );\


void Generic( int argc, char *argv[] )
{
ERRProlog
	multimemory Multimemory;
	str::string S1, S2, S3, S4, S5;
ERRBegin
	Multimemory.Init();
	Multimemory.Preallocate( 1000 );

	I( S1 );
	I( S2 );
	I( S3 );
	I( S4 );
	I( S5 );

	D;

	S1.Append( "TOTO" );
	D;

	S2.Append( "sfertyr" );
	D;

	S1.Append( "t" );
	D;

	S2.reset();
	D;

	S2.Append( "tt" );
	D;

	S2.Append( "ttryertyrtyertyerty" );
	D;

	S3.Append( "dfghdfghdfghdfghdffg" );
	D;

	S4.Append( "dfsdfgsdfgsdfgsdfg" );
	D;

	S5.Append( "dfsdfgsdfgsdfgsdfg" );
	D;

	S3.reset();
	D;

	S5.reset();
	D;

	S2.reset();
	D;

	cout << "**********" << txf::nl;

	S2.Append( "oiyiuuiyui" );
	D;

	S2.Append( "oiyiklmjlllllllllluuiyui" );
	D;
/*
	S2.reset();
	D;
*/
ERRErr
ERREnd
ERREpilog
}

int main( int argc, char *argv[] )
{
ERRFProlog
ERRFBegin
	cout << "Test of library " << MMM2Tutor.Name << ' ' << __DATE__" "__TIME__"\n";

	switch( argc ) {
	case 1:
		Generic( argc, argv );
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
		cout << "Usage: " << MMM2Tutor.Name << " [/i]\n\n";
		ERRi();
	}

ERRFErr
ERRFEnd
ERRFEpilog
	return ERRExitValue;
}
