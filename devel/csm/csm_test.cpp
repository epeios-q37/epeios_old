/*
  Test source for the 'csm' library by Claude SIMON (http://zeusw.org/intl/contact.html).
  Copyright (C) 2000 Claude SIMON (http://zeusw.org/intl/contact.html).

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
  along with this program; if not, go to http://www.fsf.org or write to the
  
                        Free Software Foundation, Inc.,
           59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

//	$Id$

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream.h>

#include "csm.h"

#include "err.h"
#include "stf.h"

class manager___
: public csm::manager___
{
protected:
	//v Client process function.
	virtual csm::behavior CSMCP(
		flw::ioflow___ &Client,
		flw::ioflow___ &Server,
		void *UP )
	{
		return csm::bStop;
	}
	//v Client initialization function.
	virtual void *CSMCI(
		flw::ioflow___ &Client,
		flw::ioflow___ &Server )
	{
		return NULL;
	}
	//v Client ending functions.
	virtual void CSMCE( void *UP )
	{}
	//v Server process function.
	virtual csm::behavior CSMSP(
		flw::ioflow___ &Client,
		void *UP )
	{
		return csm::bStop;
	}
	//v Server initialization function.
	virtual void *CSMSI( void )
	{
		return NULL;
	}
	//v Server ending function.
	virtual void CSMSE( void *UP )
	{}
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
ERRProlog
	manager___ Manager;
ERRBegin
	Manager.Init();
	Manager.Process( 1234 );

ERRErr
ERREnd
ERREpilog
}



int main( int argc, char *argv[] )
{
	int ExitCode = EXIT_SUCCESS;
ERRFProlog
ERRFBegin
	fout << "Test of library " << CSMTutor.Name << ' ' << __DATE__" "__TIME__"\n";

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
		fout << "Usage: " << CSMTutor.Name << " [/i]\n\n";
		ERRt();
	}

ERRFErr
	ExitCode = EXIT_FAILURE;
ERRFEnd
	fout << "\nEnd of program " << CSMTutor.Name << ".\n";
ERRFEpilog
	return ExitCode;
}
