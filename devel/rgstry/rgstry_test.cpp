/*
	Test source for the 'rgstry' library by Claude SIMON (http://zeusw.org/intl/contact.html).
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

#include "rgstry.h"

#include "err.h"
#include "cio.h"
#include "flf.h"
#include "xtf.h"

using cio::cin;
using cio::cout;
using cio::cerr;

void Consult(
	const rgstry::registry_ &Registry,
	rgstry::row__ Root )
{
	epeios::row__ Cursor = NONE;
	rgstry::buffer Buffer;
#if 0
	cout << "Limit : " << Registry.GetPathValue( str::string( "RecordAmountLimitation" ), Root, Buffer ) << txf::nl;

	cout << "Outfit : " << Registry.GetPathValue( str::string( "Outfit" ), Root, Buffer ) << txf::nl;
#else
	cout << "Host : " << Registry.GetValue( str::string( "Backend/Host" ), Root, Buffer ) << txf::nl << txf::sync;

	cout << "Service : " << Registry.GetValue( str::string( "Backend/Service" ), Root, Buffer ) << txf::nl << txf::sync;
#endif

	Registry.Dump( Root, true, true, cout );

	cout << txf::nl << txf::sync;
}


void Generic( int argc, char *argv[] )
{
ERRProlog
	flf::file_iflow___ FFlow;
	rgstry::registry Registry;
	rgstry::row__ Root;
	xtf::coord__ Coord;
	str::string FileName;
	rgstry::error_details ErrorDetails;
	rgstry::buffer Buffer;
ERRBegin
	FFlow.Init( "essai.xml" );

	Registry.Init();

	FileName.Init();

	Coord.Init();

	ErrorDetails.Init();
	Root = rgstry::Parse( FFlow, str::string( "" ), Registry, NONE, ErrorDetails );

	if ( Root == NONE ) {
		cerr << "Erreur";

		if ( FileName.Amount() )
			cerr << " fichier '" << FileName << '\'';
	 
		cerr << " ligne " << ErrorDetails.GetCoord().Line << " colonne " << ErrorDetails.GetCoord().Column << txf::nl;

		ERRu();
	}

	Consult( Registry, Root );

	Registry.SetValue( str::string( "Test/@Id" ), str::string( "coucou" ), Root );

	Consult( Registry, Root );

	Registry.SetValue( str::string( "Test[@Id=\"coucou\"]/@Other" ), str::string( "Hello !" ), Root );

	Consult( Registry, Root );

	Registry.SetValue( str::string( "Profiles/Profile[name=\"Default\"]/Tables/Table[id=\"0\"]/Outfits/Outfit[display=\"ListView\"]" ), str::string( "Essai" ), Root );

	Consult( Registry, Root );

	cout << Registry.GetValue( str::string( "Test[@Id=\"coucou\"]/@Other" ), Root, Buffer ) << txf::nl << txf::sync;
	cout << Registry.GetValue( str::string( "Test[Other=\"Hello !\"]" ), Root, Buffer ) << txf::nl << txf::sync;
ERRErr
ERREnd
ERREpilog
}

int main( int argc, char *argv[] )
{
ERRFProlog
ERRFBegin
	cout << "Test of library " << RGSTRYTutor.Name << ' ' << __DATE__" "__TIME__"\n" << txf::sync;

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
		cout << "Usage: " << RGSTRYTutor.Name << " [/i]\n\n";
		ERRi();
	}

ERRFErr
ERRFEnd
ERRFEpilog
	return ERRExitValue;
}
