/*
	Test source for the 'rgstry' library by Claude SIMON (csimon@epeios.org).
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

#include "rgstry.h"

#include "err.h"
#include "cio.h"
#include "fil.h"
#include "xtf.h"

using cio::cin;
using cio::cout;
using cio::cerr;

void Consult(
	const rgstry::registry_ &Registry,
	rgstry::nrow__ Root )
{
	epeios::row__ Cursor = NONE;
	rgstry::term_buffer Buffer;

	cout << "Host : " << Registry.GetPathValue( str::string( "Backend/Host" ), Root, Buffer ) << txf::nl;

	cout << "Service : " << Registry.GetPathValue( str::string( "Backend/Service" ), Root, Buffer ) << txf::nl;
}


void Generic( int argc, char *argv[] )
{
ERRProlog
	fil::file_iflow___ FFlow;
	xtf::extended_text_iflow__ XFlow;
	rgstry::registry Registry;
	rgstry::nrow__ Root;
	xtf::location__ Line, Column;
ERRBegin
	FFlow.Init( "essai.xml" );
	FFlow.EOFD( XTF_EOXT );

	XFlow.Init( FFlow );

	Registry.Init();

	Root = rgstry::Parse( XFlow, Registry, NONE, Line, Column );

	if ( Root == NONE ) {
	 cerr << "Erreur ligne " << Line << " colomne " << Column << txf::nl;
	 ERRu();
	}

	Consult( Registry, Root );
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
