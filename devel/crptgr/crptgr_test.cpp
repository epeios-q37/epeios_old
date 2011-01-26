/*
	Test source for the 'crptgr' library by Claude SIMON (csimon at zeusw dot org).
	Copyright (C) 20112004 Claude SIMON.

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

#include "crptgr.h"

#include "err.h"
#include "cio.h"
#include "str.h"
#include "flx.h"

using cio::cin;
using cio::cout;
using cio::cerr;

#define TEXT "Ceci est un texte quelconque pour tester le decryptage et l'encryptage."
//#define TEXT "a"
#define KEY	"Hello world !"

void Encrypt(
	const str::string_ &Text,
	str::string_ &Encrypted,
	const char *Key )
{
ERRProlog
	flx::E_STRING_OFLOW___ SFlow;
	crptgr::encrypt_oflow___ Encrypter;
	txf::text_oflow__ TFlow;
	epeios::row__ Row = NONE;
ERRBegin
	SFlow.Init( Encrypted );
	Encrypter.Init( SFlow, Key );
	TFlow.Init( Encrypter );

	Row = Text.First();

	while ( Row != NONE ) {
		TFlow.Put( Text( Row ) );
		Row = Text.Next( Row );
	}

ERRErr
ERREnd
ERREpilog
}

void Decrypt(
	const str::string_ &Encrypted,
	str::string_ &Text,
	const char *Key,
	epeios::size__ Amount )
{
ERRProlog
	flx::E_STRING_IFLOW__ SFlow;
	crptgr::decrypt_iflow___ Decrypter;
	bso::char__ C = 0;
ERRBegin
	SFlow.Init( Encrypted );
	Decrypter.Init( SFlow, Key );
	Decrypter.EOFD( "\x1a" );

	while ( ( C = Decrypter.Get() ) != '\x1a' )
		Text.Append( C );
ERRErr
ERREnd
ERREpilog
}


void Generic( int argc, char *argv[] )
{
ERRProlog
	str::string Text, Encrypted;
ERRBegin
	Encrypted.Init();
	Encrypt( str::string( TEXT ), Encrypted, KEY );

	Text.Init();
	Decrypt( Encrypted, Text, KEY, strlen( TEXT ) );

	cout << '"' << Text << '"' << txf::commit;
ERRErr
ERREnd
ERREpilog
}

int main( int argc, char *argv[] )
{
ERRFProlog
ERRFBegin
	cout << "Test of library " << CRPTGRTutor.Name << ' ' << __DATE__" "__TIME__"\n";

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
		cout << txf::commit;
		cerr << "\nBad arguments.\n";
		cout << "Usage: " << CRPTGRTutor.Name << " [/i]\n\n";
		ERRi();
	}

ERRFErr
ERRFEnd
ERRFEpilog
	return ERRExitValue;
}
