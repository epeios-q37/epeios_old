/*
	Test source for the 'dbsidx' library by Claude SIMON (csimon@epeios.org).
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

#include "dbsidx.h"
#include "dbsdct.h"

#include "err.h"
#include "cio.h"

using cio::cin;
using cio::cout;
using cio::cerr;

#define TEXT	"azertyuiopmlkjhgfdsqwxcvbnAQWXSZEDCVFRTGBNHYUJKIOLMP1029384756"

const char *Generate( void )
{
	static  char Buffer[100];

	Buffer[99] = 0;

	int L = 99;

	while ( L-- )
		Buffer[L] = TEXT[rand() % sizeof( TEXT )];

	return Buffer;
}

void Delete(
	  dbsdct::content_ &Content,
	  dbsidx::index_ &Index )
{
	dbsidx::row__ Row = NONE;
	const char *Buffer;

	Buffer = Generate();

	if ( ( Row = Index.Search( str::string( Buffer ) ) ) != NONE ) {
		Index.Delete( Row );
		Content.Erase( Row );
	}
}

void Fill(
	  dbsdct::content_ &Content,
	  dbsidx::index_ &Index )
{
	int L = 10000;
	const char *Buffer;

	tol::InitializeRandomGenerator();

	while ( L-- ) {

		if ( ( rand() % 10 ) == 0 )
			Delete( Content, Index );
		else {
			Buffer = Generate();

			cout << Buffer << txf::tab << txf::sync;

			Index.Index( Content.Store( str::string( Buffer ) ) );
		}
	}
}

void Display(
	  const dbsdct::content_ &Content,
	  const dbsidx::index_ &Index )
{
ERRProlog
	dbsdct::row__ Row = NONE;
	dbsdct::data Data;
ERRBegin
	cout << txf::nl << "---------------------" << txf::nl;

	Row = Index.First();

	while ( Row != NONE ) {
		Data.Init();

		Content.Retrieve( Row, Data );

		cout << Data << txf::tab << txf::sync;

		Row = Index.Next( Row );
	}

	cout << txf::nl << "---------------------" << txf::nl;
ERRErr
ERREnd
ERREpilog
}

void Display(
	const dbsdct::content_ &Content,
	dbsidx::row__ Row )
{
ERRProlog
	dbsidx::data Data;
ERRBegin
	if ( Row != NONE ) {
		Data.Init();
		Content.Retrieve( Row, Data );
		cout << Data;
	} else
		cout << "<>";

ERRErr
ERREnd
ERREpilog
}




void Search(
	  const dbsdct::content_ &Content,
	  const dbsidx::index_ &Index )
{
	dbsidx::row__ Row = NONE;
	const char *Buffer;

	Buffer = Generate();

	cout << txf::nl << Buffer << " : ";

	if ( ( Row = Index.Search( str::string( Buffer ) ) ) != NONE ) {
		Display( Content, Index.Previous( Row ) );
		cout << txf::tab;
		Display( Content, Row );
		cout << txf::tab;
		Display( Content, Index.Next( Row ) );
		cout << txf::nl << txf::sync;
	}
}

class sort__
: public dbsidx::sort_function__
{
protected:
	virtual bso::sign__ DBSIDXCompare(
		const dbsdct::data_ &Data1,
		const dbsdct::data_ &Data2 )
	{
			bso::sign__ Sign;
		ERRProlog
			dbsdct::data D1, D2;
		ERRBegin
			D1.Init( Data1 );
			D2.Init( Data2 );

			str::ToUpper( D1 );
			str::ToUpper( D2 );

			Sign = str::Compare( D1, D2 );
		ERRErr
		ERREnd
		ERREpilog
			return Sign;
	}
};


void Generic( int argc, char *argv[] )
{
ERRProlog
	dbsidx::file_index Index;
	dbsdct::file_content Content;
	sort__ Sort;
ERRBegin
	Content.Init( str::string( "test\\d" ) );
	Index.Init( str::string( "test\\i" ), Content, Sort );
	Fill( Content, Index );
	Display( Content, Index );
	Search( Content, Index );
ERRErr
ERREnd
ERREpilog
}

int main( int argc, char *argv[] )
{
ERRFProlog
ERRFBegin
	cout << "Test of library " << DBSIDXTutor.Name << ' ' << __DATE__" "__TIME__"\n";

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
		cout << "Usage: " << DBSIDXTutor.Name << " [/i]\n\n";
		ERRi();
	}

ERRFErr
ERRFEnd
ERRFEpilog
	return ERRExitValue;
}
