/*
	Test source for the 'xpp' library by Claude SIMON (csimon at zeusw dot org).
	Copyright (C) 2004 Claude SIMON.

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

#include "xpp.h"

#include "err.h"
#include "cio.h"
#include "flf.h"
#include "fnm.h"
#include "lcl.h"

using cio::cin;
using cio::cout;
using cio::cerr;

#define TEST_CASE	2

#if TEST_CASE == 1
#define LOCATION	"H:\\cvs\\epeios\\tools\\expp\\"	
#define FILENAME	"linux.xcf"
#endif

#if TEST_CASE == 2
#define LOCATION	""	
#define FILENAME	"basic.xml"
#endif

#if TEST_CASE == 3
#define LOCATION	""	
#define FILENAME	"test.xml"
#endif


#define FILE		LOCATION FILENAME


void Generic( int argc, char *argv[] )
{
ERRProlog
	flf::file_iflow___ Flow;
	xpp::preprocessing_extended_text_iflow___ XTFlow;
	FNM_BUFFER___ Buffer;
	xml::browser___ Browser;
ERRBegin
//	Example.Init( "<xcf:bloc>Value<OtherRoot>Before<Leaf Tree=\"Larch\">before<Element/>after</Leaf>After</OtherRoot><Root>Before<Leaf Tree=\"Larch\">before<Element/>after</Leaf>After</Root></xcf:bloc>" );
//	Flow.Init( Example );

	Flow.Init( FILE );

//	Directory = fnm::GetLocation( FILE, Buffer );

	Flow.EOFD( XTF_EOXT );

	XTFlow.Init( Flow, str::string( LOCATION ) );

	Browser.Init( XTFlow );

	switch ( Browser.Browse( xml::tfValue ) ) {
	case xml::tValue:
		cout << '>' << Browser.Value() << '<' << txf::sync;
		break;
	case xml::tProcessed:
		cout << Browser.DumpData() << txf::sync;
		break;
	case xml::tError:
		cerr << "Error '" << xpp::GetLabel( XTFlow.Preprocessor().Status() ) << "' at line " << XTFlow.Coord().Line << " column " << XTFlow.Coord().Column;
		if ( XTFlow.Preprocessor().LocalizedFileName().Amount() != 0 )
			cerr << " in file '" << XTFlow.Preprocessor().LocalizedFileName() << '\'';
		cerr << " !" << txf::nl << txf::sync;
		break;
	default:
		ERRc();
		break;
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
	cout << "Test of library " << XPPTutor.Name << ' ' << __DATE__" "__TIME__"\n";

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
		cout << "Usage: " << XPPTutor.Name << " [/i]\n\n";
		ERRi();
	}

ERRFErr
ERRFEnd
ERRFEpilog
	return ERRExitValue;
}
