/*
  Test source for the 'brkanl' library by Claude L. Simon (epeios@epeios.org).
  Copyright (C) 2000 Claude L. SIMON (epeios@epeios.org).

  This file is part of the Epeios (http://www.epeios.org/) project.
  

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.
 
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, go to http://www.fsf.org or write to the
  
                        Free Software Foundation, Inc.,
           59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

//	$Id$

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream.h>

#include "brkanl.h"

using namespace brkanl;

#include "err.h"
#include "stf.h"
#include "clt.h"
#include "salcsm.h"
#include "broker.h"

class manager
: public salcsm::manager___
{
protected:
	//v Client process function.
	virtual salcsm::behavior SALCSMCP(
		flw::ioflow___ &Client,
		flw::ioflow___ &Server,
		void *UP )
	{
		::broker::broker &Broker = *(::broker::broker *)UP;
		
		Broker.Handle( Client ); 
	
		return salcsm::bContinue;
	}
	//v Client initialization function.
	virtual void *SALCSMCI(
		flw::ioflow___ &Client,
		flw::ioflow___ &Server )
	{
		::broker::broker *Broker;
		
		if ( ( Broker = new ::broker::broker ) == NULL )
			ERRa();
			
		Broker->Init();
		
		return Broker;
	}
	//v Client ending functions.
	virtual void SALCSMCE( void *UP )
	{
		if ( UP != NULL )
			delete (::broker::broker *)UP;
	}
};


void Generic( int argc, char *argv[] )
{
ERRProlog
ERRBegin
ERRErr
ERREnd
ERREpilog
}

void Print(	const parameters_ &Parameters )
{
	POSITION__ P = Parameters.First();

	while( P != NONE ) {
		fout << txf::tab << brkcst::CastsNames[Parameters( P )];
		P = Parameters.Next( P );
	}

	fout << txf::nl;
}

void Print(	const commands_ &Commands )
{
	CITEM( command_ ) Command;
	POSITION__ P = Commands.First();

	Command.Init( Commands );

	while( P != NONE ) {
		fout << Command( P ).Identification.Value << " (" << Command( P ).Identification.ID() << "):" << txf::nl;
		Print( Command( P ).Parameters );
		P = Commands.Next( P );
	}

	fout << txf::nl;
}

void Print( types_ &Types )
{
	CITEM( type_ ) Type;
	POSITION__ P = Types.First();

	Type.Init( Types );

	while( P != NONE ) {
		fout << Type( P ).Identification.Value << " (" << Type( P ).Identification.ID() << "):" << txf::nl;
		Print( Type( P ).Commands );
		P = Types.Next( P );
	}
}



void Essai( void )
{
ERRProlog
	frtend::frontend___ Frontend;
	manager Manager;
	types Types;
	sck::socket_ioflow___ Flow;
ERRBegin
	Types.Init();
	Flow.Init( clt::Connect( "localhost:1234" ) );
	Frontend.Init( Flow );
//	Manager.Init();
//	Frontend.Init( Manager.Process() );
	
	Analyze( Frontend, Types );

	Print( Types);
ERRErr
ERREnd
ERREpilog
}

int main( int argc, char *argv[] )
{
	int ExitCode = EXIT_SUCCESS;
ERRFProlog
ERRFBegin
	fout << "Test of library " << BRKANLTutor.Name << ' ' << __DATE__" "__TIME__"\n";

	switch( argc ) {
	case 1:
		Generic( argc, argv );
		Essai();
		break;
	case 2:
		if ( !strcmp( argv[1], "/i" ) )
		{
			TTR.Advertise();
			break;
		}
	default:
		fout << txf::sync;
		ferr << "\nBad arguments.\n";
		fout << "Usage: " << BRKANLTutor.Name << " [/i]\n\n";
		ERRt();
	}

ERRFErr
	ExitCode = EXIT_FAILURE;
ERRFEnd
	fout << "\nEnd of program " << BRKANLTutor.Name << ".\n";
ERRFEpilog
	return ExitCode;
}
