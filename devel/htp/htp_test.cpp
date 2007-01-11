/*
	Test source for the 'htp' library by Claude SIMON (csimon@epeios.org).
	Copyright (C) 2002, 2004 Claude SIMON (csimon@epeios.org).

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

#include "htp.h"

#include "err.h"
#include "cio.h"
#include "sck.h"
#include "csdbnc.h"

using cio::cin;
using cio::cout;
using cio::cerr;

void Generic( int argc, char *argv[] )
{
ERRProlog
	sck::socket__ Socket = SCK_INVALID_SOCKET;
	sck::unsafe_socket_ioflow___ RFlow;
	txf::text_oflow__ TFlow( RFlow );
	str::string Request;
	htp::fields Fields;
	htp::field Field;
ERRBegin
	Socket = csdbnc::Connect( "musicanet.org", "80" );

	RFlow.Init( Socket );

//	TFlow.Init( RFlow );

	Fields.Init();

	Field.Init( htp::fHost, "musicanet.org" );
	Fields.Append( Field );

	Field.Init( htp::fReferrer, "http://musicanet.org/fr/contacts.php" );
	Fields.Append( Field );

	Field.Init( htp::fAccept, "*/*" );
	Fields.Append( Field );

	Field.Init( htp::fContentType, "application/x-www-form-urlencoded" );
	Fields.Append( Field );

	Request.Init( "sujet=Test SPAM&nom=&prenom=&email= its\nContent-Transfer-Encoding: quoted-printable\nContent-Type: text/plain\nSubject: Spam test\nbcc: develop@logiplus.fr,melanie@musicanet.org,claude.simon@zeusw.org\nCeci est un courrier envoyé en exploitant une faille du script 'envoi.php' !\n\n&os=&navigateur=&message=Message de test de vulnérabilité au SPAM.A ignorer." );

	htp::Post( str::string( "/fr/envoi.php" ), Fields, Request, TFlow );

	TFlow << txf::sync;

	while ( 1 )
		cio::cout << (char)RFlow.Get();

ERRErr
ERREnd
ERREpilog
}

int main( int argc, char *argv[] )
{
ERRFProlog
ERRFBegin
	cout << "Test of library " << HTPTutor.Name << ' ' << __DATE__" "__TIME__"\n";

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
		cout << "Usage: " << HTPTutor.Name << " [/i]\n\n";
		ERRi();
	}

ERRFErr
ERRFEnd
ERRFEpilog
	return ERRExitValue;
}
