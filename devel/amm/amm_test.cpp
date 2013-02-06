/*
	Test source for the 'amm' library by Claude SIMON (csimon at zeusw dot org).
	Copyright (C) 20132004 Claude SIMON.

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

#include "amm.h"

#include "err.h"
#include "cio.h"

using cio::CIn;
using cio::COut;
using cio::CErr;

#define DELTA	1000

void Generic( int argc, char *argv[] )
{
ERRProlog
	mdr::size__ Size = 0;
	mdr::xsize__ XSize;
	tol::timer__ Timer;
	bso::ubyte__ Length = 0;
ERRBegin
	Timer.Init( 100 );
	Timer.Launch();

	do {
		if ( Size >= ( MDR_SIZE_MAX - DELTA ) ) 
			Size = MDR_SIZE_MAX;
		else
			Size += DELTA;


		XSize = mdr::Convert( Size );

		if( Timer.IsElapsed() ) {
			cio::COut << Size << txf::pad << (bso::ulong__)XSize.Length << txf::rfl << txf::commit;
			Timer.Launch();
		}

		if ( mdr::Convert( XSize.Size() ) != Size )
			ERRc();

		if ( Length != XSize.Length ) {
			cio::COut << Size << txf::pad << (bso::ulong__)( Length = XSize.Length ) << txf::nl << txf::commit;
			Timer.Launch();
		}


	} while ( Size < MDR_SIZE_MAX );

ERRErr
ERREnd
	cio::COut << txf::commit;
ERREpilog
}

int main( int argc, char *argv[] )
{
ERRFProlog
ERRFBegin
	COut << "Test of library " << AMMTutor.Name << ' ' << __DATE__" "__TIME__"\n";

	switch( argc ) {
	case 1:
		Generic( argc, argv );
		break;
	case 2:
		if ( !strcmp( argv[1], "/i" ) )
		{
			TTR.Advertise( COut );
			break;
		}
	default:
		COut << txf::commit;
		CErr << "\nBad arguments.\n";
		COut << "Usage: " << AMMTutor.Name << " [/i]\n\n";
		ERRExit( EXIT_FAILURE );
	}

ERRFErr
ERRFEnd
ERRFEpilog
	return ERRExitValue;
}
