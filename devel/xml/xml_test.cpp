/*
	Test source for the 'xml' library by Claude SIMON (http://zeusw.org/intl/contact.html).
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
#include "flx.h"

#include "xml.h"

#include "err.h"
#include "cio.h"
#include "flf.h"

#include "fnm.h"
#include "lcl.h"

using cio::cin;
using cio::cout;
using cio::cerr;

struct callback__
: public xml::callback__
{
	int ident;
	void Ident_( void )
	{
		int Counter = ident;

		while ( Counter-- )
			cout << txf::tab;
	}
	virtual bso::bool__ XMLProcessingInstruction( const xml::dump_ &Dump )
	{
		cout << "PI : " << Dump.RawData << txf::nl;

		return true;
	}
	virtual bso::bool__ XMLStartTag(
		const str::string_ &Name,
		const xml::dump_ &Dump )
	{
		Ident_();
		cout << "Tag : '" << Name << '\'' << txf::nl;

		ident++;

		return true;
	}
	virtual bso::bool__ XMLValue(
		const str::string_ &TagName,
		const str::string_ &Value,
		const xml::dump_ &Dump )
	{
		Ident_();

		cout << "Value : '" << Value << "' (" << TagName << ')' << txf::nl;

		return true;
	}
	virtual bso::bool__ XMLAttribute(
		const str::string_ &TagName,
		const str::string_ &Name,
		const str::string_ &Value,
		const xml::dump_ &Dump )
	{
		Ident_();

		cout << "Attribute : '" << Name << "', '" << Value << "' (" << TagName << ')' << txf::nl;

		return true;
	}
	virtual bso::bool__ XMLEndTag(
		const str::string_ &Name,
		const xml::dump_ &Dump )
	{
		ident--;

		Ident_();

		cout << "Tag closed : '" << Name << '\'' << txf::nl;

		return true;
	}
	callback__( void )
	{
		ident = 0;
	}
};

#define FILE	"test.xml"


void Generic( int argc, char *argv[] )
{
ERRProlog
//	str::string	Example;
	callback__ Callback;
//	flx::E_STRING_IFLOW__ Flow;
	flf::file_iflow___ Flow;
	xtf::extended_text_iflow__ XTFlow;
	str::string GuiltyFileName;
	const char *Directory;
	xml::extended_status__ Status = xml::xs_Undefined;
	FNM_BUFFER___ Buffer;
	lcl::locales Locales;
	str::string Translation;
ERRBegin
//	Example.Init( "<xcf:bloc>Value<OtherRoot>Before<Leaf Tree=\"Larch\">before<Element/>after</Leaf>After</OtherRoot><Root>Before<Leaf Tree=\"Larch\">before<Element/>after</Leaf>After</Root></xcf:bloc>" );
//	Flow.Init( Example );

	Flow.Init( FILE );

	Directory = fnm::GetLocation( FILE, Buffer );

	Flow.EOFD( XTF_EOXT );

	XTFlow.Init( Flow );

	GuiltyFileName.Init();

	if ( ( Status = xml::ExtendedParse( XTFlow, str::string( "xcf" ), Callback, str::string( Directory ), GuiltyFileName ) ) != xml::xsOK ) {
		cout << txf::sync;
		cerr << txf::nl << "Error at line " << XTFlow.Coord().Line << ", Column " << XTFlow.Coord().Column;

		if ( GuiltyFileName.Amount() != 0 )
			cerr << " in file '" << GuiltyFileName << '\'';

		Locales.Init();
		Translation.Init();
		cerr << " : " << xml::GetTranslation( Status, str::string(), Locales, Translation );

		cerr << txf::nl;
	}

	
	cout << txf::nl << txf::sync;

ERRErr
ERREnd
ERREpilog
}

int main( int argc, char *argv[] )
{
ERRFProlog
ERRFBegin
	cout << "Test of library " << XMLTutor.Name << ' ' << __DATE__" "__TIME__"\n";

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
		cout << "Usage: " << XMLTutor.Name << " [/i]\n\n";
		ERRi();
	}

ERRFErr
ERRFEnd
ERRFEpilog
	return ERRExitValue;
}
