/*
	Test source for the 'lcl' library by Claude SIMON (csimon at zeusw dot org).
	Copyright (C) 20092004 Claude SIMON.

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

#include "lcl.h"

#include "err.h"
#include "cio.h"
#include "flf.h"

using cio::cin;
using cio::cout;
using cio::cerr;

void Display( const lcl::strings_ &Strings )
{
ERRProlog
	ctn::E_CMITEM( str::string_ ) String;
	epeios::row__ Row = NONE;
ERRBegin
	String.Init( Strings );

	Row = Strings.First();

	while ( Row != NONE ) {
		cout << String( Row ) << txf::tab;

		Row = Strings.Next( Row );
	}

	cout << txf::nl;
ERRErr
ERREnd
ERREpilog
}


void Generic( int argc, char *argv[] )
{
ERRProlog
	flf::file_iflow___ FFlow;
	xtf::extended_text_iflow__ XFlow;
	lcl::locales Locales;
	lcl::strings Labels, Wordings;
	str::string Translation;
	lcl::strings Values;
ERRBegin
	FFlow.Init( "test.xml" );
	XFlow.Init( FFlow );

	if ( !Locales.Init( XFlow, "Locale[target=\"test\"]" ) )
		ERRc();

	Labels.Init();
	Wordings.Init();

	Locales.GetLanguages( Labels, Wordings );

	Display( Labels );
	Display( Wordings );

	Values.Init();

	Values.Append( str::string( "Valeur UNO" ) );
	Values.Append( str::string( "Value 2" ) );
	Values.Append( str::string( "III" ) );

	Translation.Init();

	if ( Locales.GetTranslation( "UnableToCreateDatabase", "fr", Translation  ) )
		cout << Translation << txf::nl;
	else
		cout << "Not found !" << txf::nl;

	lcl::ReplaceTags( Translation, Values );
	cout << Translation << txf::nl;

	Translation.Init();

	if ( Locales.GetTranslation( "UnableToFindFile", "en", Translation  ) )
		cout << Translation << txf::nl;
	else
		cout << "Not found !" << txf::nl;

	lcl::ReplaceTags( Translation, Values );
	cout << Translation << txf::nl;
ERRErr
ERREnd
ERREpilog
}

int main( int argc, char *argv[] )
{
ERRFProlog
ERRFBegin
	cout << "Test of library " << LCLTutor.Name << ' ' << __DATE__" "__TIME__"\n";

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
		cout << "Usage: " << LCLTutor.Name << " [/i]\n\n";
		ERRi();
	}

ERRFErr
ERRFEnd
ERRFEpilog
	return ERRExitValue;
}
