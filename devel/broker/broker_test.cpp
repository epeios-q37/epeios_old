/*
  Test source for the 'broker' library by Claude L. Simon (epeios@epeios.org).
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

#include "broker.h"

using namespace broker;

#include "err.h"
#include "stf.h"
#include "csm.h"

class manager
: public csm::manager___
{
protected:
	//v Client process function.
	virtual csm::behavior CSMCP(
		flw::ioflow___ &Client,
		flw::ioflow___ &Server,
		void *UP )
	{
		::broker::broker &Broker = *(::broker::broker *)UP;
		
		Broker.Handle( Client ); 
	
		return csm::bContinue;
	}
	//v Client initialization function.
	virtual void *CSMCI(
		flw::ioflow___ &Client,
		flw::ioflow___ &Server )
	{
		::broker::broker *Broker;
		
		if ( ( Broker = new ::broker::broker ) == NULL )
			ERRa();
			
		Broker->Init();
	}
	//v Client ending functions.
	virtual void CSMCE( void *UP )
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

void Essai( void )
{
	manager Manager;
	
	Manager.Init();
	
	Manager.Process( 1234 );
}

int main( int argc, char *argv[] )
{
	int ExitCode = EXIT_SUCCESS;
ERRFProlog
ERRFBegin
	fout << "Test of library " << BROKERTutor.Name << ' ' << __DATE__" "__TIME__"\n";

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
		fout << "Usage: " << BROKERTutor.Name << " [/i]\n\n";
		ERRt();
	}

ERRFErr
	ExitCode = EXIT_FAILURE;
ERRFEnd
	fout << "\nEnd of program " << BROKERTutor.Name << ".\n";
ERRFEpilog
	return ExitCode;
}
