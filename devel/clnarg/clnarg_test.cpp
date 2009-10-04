/*
  Test source for the 'clnarg' library by Claude L. Simon (epeios@epeios.org).
  Copyright (C) 2000 Claude L. SIMON (epeios@epeios.org).

  This file is part of the Epeios (http://www.epeios.org/) project.
  

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 3
  of the License, or (at your option) any later version.
 
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, go to http://www.fsf.org or write to the
  
                        Free Software Foundation, Inc.,
           59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

//	$Id$

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream.h>

#include "clnarg.h"

#include "err.h"
#include "stf.h"

void Generic( int argc, const char *argv[] )
{
ERRProlog
ERRBegin
ERRErr
ERREnd
ERREpilog
}

enum command {
	cA,
	cB,
	cC,
	cVersion,
	c_amount
};

enum option {
	oA,
	oB,
	oC,
	o_amount
};

void Print( const clnarg::arguments_ &Arguments )
{
	tym::row__ P = Arguments.First();
	ctn::E_CMITEM( clnarg::argument_ ) Item;

	while( P != NONE ) {
		stf::cout << txf::tab << Arguments( P, Item );
		P = Arguments.Next( P );
	}
}


void Print( 
	clnarg::analyzer___ &A,
	clnarg::id__ Id )
{
ERRProlog
	clnarg::arguments Arguments;
ERRBegin
	Arguments.Init();

	if ( Id != CLNARG_NONE )
		A.GetArguments( Id, Arguments );
	else
		A.GetArguments( Arguments );

	Print( Arguments );
ERRErr
ERREnd
ERREpilog
}

void Print(
	const clnarg::option_list &List,
	clnarg::analyzer___ &A )
{
	tym::row__ P = List.First();

	while( P != NONE ) {
		stf::cout << (unsigned long)List( P ) << ": ";
		Print( A, List( P ) );
		stf::cout << txf::nl;
		P = List.Next( P ); 
	}

	Print( A, CLNARG_NONE );
}

void Essai( int argc, const char *argv[] )
{
ERRProlog
	clnarg::description Description;
	clnarg::analyzer___ Analyzer;
	clnarg::option_list List;
	const char *UnknowOption;
	int C;
ERRBegin
	Description.Init();

	Description.AddCommand( 'B', "BLong", cB );
	Description.AddCommand( 'A', "ALong", cA );
	Description.AddCommand( 'v', "version", cVersion );
	Description.AddCommand( 'C', "CLong", cC );

	Description.AddOption( 'b', "bLong", oB );
	Description.AddOption( 'c', "cLong", oC );
	Description.AddOption( 'a', "aLong", oA );

	Analyzer.Init( argv, argc, Description );

	stf::cout <<(unsigned long)( C = Analyzer.GetCommand() ) << txf::nl;

	if ( C == cVersion )
		TTR.Advertise();

	List.Init();

	if ( ( UnknowOption = Analyzer.GetOptions( List ) ) != NULL ) {
		stf::cerr << '\'' << UnknowOption << "': Unknow command/option." << txf::nl;
		ERRu();
	}

	Print( List, Analyzer );

ERRErr
ERREnd
ERREpilog
}


int main( int argc, const char *argv[] )
{
	int ExitCode = EXIT_SUCCESS;
ERRFProlog
ERRFBegin
	stf::cout << "Test of library " << CLNARGTutor.Name << ' ' << __DATE__" "__TIME__"\n";

	Essai( argc, argv );
/*
	switch( argc ) {
	case 1:
		Generic( argc, argv );
		Essai( argc, argv );
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
		fout << "Usage: " << CLNARGTutor.Name << " [/i]\n\n";
		ERRt();
	}
*/

ERRFErr
	ExitCode = EXIT_FAILURE;
ERRFEnd
	stf::cout << "\nEnd of program " << CLNARGTutor.Name << ".\n";
ERRFEpilog
	return ExitCode;
}
