/*
	Test source for the 'dbsdct' library by Claude SIMON (http://zeusw.org/intl/contact.html).
	Copyright (C) 2004 Claude SIMON (http://zeusw.org/intl/contact.html).

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

#include "dbsdct.h"

#include "err.h"
#include "cio.h"

using cio::cin;
using cio::cout;
using cio::cerr;

void Generic( int argc, char *argv[] )
{
ERRProlog
	dbsdct::file_content Content;
	dbsdct::row__ Row1 = NONE, Row2 = NONE;
	dbsdct::data Data;
ERRBegin
	if ( !Content.Init( str::string( "test" ) ) ) {
		cout << "Inexistent content : creating !" << txf::nl << txf::sync;

		Data.Init( "Hello !");
		Row1 = Content.Store( Data );

		Data.Init( "Hi !");
		Row2 = Content.Store( Data );

		Data.Init();
		Content.Retrieve( Row1, Data );
		cout << Data << txf::nl;
		
		Data.Init();
		Content.Retrieve( Row2, Data );
		cout << Data << txf::nl;

		Data.Init( "Hello the World !" );
		Content.Store( Data, Row1 );

		Data.Init( "Bonjour tout le monde !" );
		Content.Store( Data, Row2 );
	} else {
		Row1 = Content.First();
		Row2 = Content.Next( Row1 );
/*
		Row1 = 0;
		Row2 = 1;
*/
	}

	Data.Init();
	Content.Retrieve( Row1, Data );
	cout << Data << txf::nl;

	if ( Content.Exists( Row2 ) ) {
		Data.Init();
		Content.Retrieve( Row2, Data );
		cout << Data << txf::nl;
		Content.Erase( Row2 );
	} else {
		Data.Init( "Asta la vista, baby !" );
		Content.Store( Data );
	}


	cout << txf::sync;
ERRErr
ERREnd
ERREpilog
}

int main( int argc, char *argv[] )
{
ERRFProlog
ERRFBegin
	cout << "Test of library " << DBSDCTTutor.Name << ' ' << __DATE__" "__TIME__"\n";

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
		cout << "Usage: " << DBSDCTTutor.Name << " [/i]\n\n";
		ERRi();
	}

ERRFErr
ERRFEnd
ERRFEpilog
	return ERRExitValue;
}
