/*
  Test source for the 'htp' library by Claude SIMON (csimon@epeios.org).
  Copyright (C) 2002 Claude SIMON (csimon@epeios.org).

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
#include <iostream.h>

#include "htp.h"

#include "err.h"
#include "stf.h"
#include "clt.h"

void Generic( int argc, char *argv[] )
{
ERRProlog
ERRBegin
ERRErr
ERREnd
ERREpilog
}

void Test( void )
{
ERRProlog
	sck::socket__ Socket = SCK_INVALID_SOCKET;
	sck::socket_ioflow___ RFlow;
	txf::text_oflow___ TFlow;
	str::string Request;
ERRBegin
	Socket = clt::Connect( "freshmeat.net", "80" );

	RFlow.Init( Socket );

	TFlow.Init( RFlow );

	Request.Init( "<?xml version=\"1.0\"?><methodCall><methodName>fetch_available_release_foci</methodName><params/></methodCall>" );

	htp::Post( str::string( "/xmlrpc/" ), str::string( "freshmeat.net" ), str::string( "epeios" ), str::string( "text/xml" ), Request, TFlow );

	TFlow << txf::sync;

	while ( 1 )
		stf::cout << (char)RFlow.Get();

ERRErr
ERREnd
ERREpilog
}

int main( int argc, char *argv[] )
{
	int ExitCode = EXIT_SUCCESS;
ERRFProlog
ERRFBegin
	stf::cout << "Test of library " << HTPTutor.Name << ' ' << __DATE__" "__TIME__"\n";

	switch( argc ) {
	case 1:
//		Generic( argc, argv );
		Test();
		break;
	case 2:
		if ( !strcmp( argv[1], "/i" ) )
		{
			TTR.Advertise();
			break;
		}
	default:
		stf::cout << txf::sync;
		stf::cerr << "\nBad arguments.\n";
		stf::cout << "Usage: " << HTPTutor.Name << " [/i]\n\n";
		ERRi();
	}

ERRFErr
	ExitCode = EXIT_FAILURE;
ERRFEnd
	stf::cout << "\nEnd of program " << HTPTutor.Name << ".\n";
ERRFEpilog
	return ExitCode;
}
