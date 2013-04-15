/*
	Test source for the 'ags' library by Claude SIMON (csimon at zeusw dot org).
	Copyright (C) 20132004 Claude SIMON.

	This file is part of the Epeios (http://zeusw.org/epeios/) project.

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

#include "ags.h"

#include "err.h"
#include "cio.h"
#include "ctn.h"
#include "str.h"

using cio::CIn;
using cio::COut;
using cio::CErr;

#define DS	Storage.DisplayStructure( COut );COut << txf::commit

void Generic( int argc, char *argv[] )
{
ERRProlog
	ags::E_ASTORAGE Storage;
	ags::descriptor__ D1, D2, D3, D4, D5;
ERRBegin
	Storage.Init();
	DS;

	D1 = Storage.Allocate( 2 );
	DS;
	D2 = Storage.Allocate( 2000 );
	DS;
	D3 = Storage.Allocate( 3000 );
	DS;
	D4 = Storage.Allocate( 4000 );
	DS;
	D5 = Storage.Allocate( 5000 );
	DS;
	D3 = Storage.Reallocate( D3, 4000 );
	DS;
	D2 = Storage.Reallocate( D2, 2500 );
	DS;
	Storage.Free( D2 );
	DS;
	Storage.Free( D4 );
	DS;
ERRErr
ERREnd
ERREpilog
}

typedef ctn::E_MCONTAINER_( str::string_ ) strings_;
E_AUTO( strings );

sdr::row__ Locate(
	const str::string_ &Name,
	const strings_ &Strings )
{
	ctn::E_CMITEM( str::string_ ) String;
	sdr::row__ Row = Strings.First();

	String.Init( Strings );

	while ( ( Row!= E_NIL ) && ( String( Row ) != Name ) )
		Row = Strings.Next( Row );

	if ( Row == E_NIL )
		ERRc();

	return Row;
}


void Essai( void )
{
ERRProlog
	ags::E_ASTORAGE Storage;
	strings Strings1, Strings2;
ERRBegin
	Storage.Init();
	Strings1.plug( Storage );
	Strings2.plug( Storage );

	Strings1.Init();
	Strings2.Init();

	Strings1.Append( str::string( "toto" ) );
	DS;
	Strings2.Append( str::string( "toto" ) );
	DS;

	Strings1.Append( str::string( "tata" ) );
	DS;
	Strings2.Append( str::string( "tata" ) );
	DS;

	Strings1.Append( str::string( "titi" ) );
	DS;
	Strings2.Append( str::string( "titi" ) );
	DS;

	Strings1.Remove( Locate( str::string( "toto" ), Strings1 ) );
	DS;
ERRErr
ERREnd
ERREpilog
}


int main( int argc, char *argv[] )
{
ERRFProlog
ERRFBegin
	COut << "Test of library " << AGSTutor.Name << ' ' << __DATE__" "__TIME__"\n";

	switch( argc ) {
	case 1:
//		Generic( argc, argv );
		Essai();
		break;
	case 2:
		if ( !strcmp( argv[1], "/i" ) )
		{
			TTR.Advertise( COut );
			break;
		}
	default:
		COut << txf::commit;
		CErr << "\nBad arguments.\n";
		COut << "Usage: " << AGSTutor.Name << " [/i]\n\n";
		ERRExit( EXIT_SUCCESS );
	}

ERRFErr
ERRFEnd
ERRFEpilog
	return ERRExitValue;
}
