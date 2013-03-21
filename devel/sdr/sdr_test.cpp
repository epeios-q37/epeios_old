/*
	Test source for the 'sdr' library by Claude SIMON (csimon at zeusw dot org).
	Copyright (C) 2013-2004 Claude SIMON.

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

#include "sdr.h"

#include "err.h"
#include "cio.h"

using cio::CIn;
using cio::COut;
using cio::CErr;

#define DELTA	1000

void Generic( int argc, char *argv[] )
{
ERRProlog
	sdr::size__ Size = 0;
	sdr::xsize__ XSize;
	tol::timer__ Timer;
	bso::nuint__ Length = 0;
ERRBegin
	Timer.Init( 100 );
	Timer.Launch();

	do {
		if ( Size >= ( SDR_SIZE_MAX - DELTA ) ) 
			Size = SDR_SIZE_MAX;
		else
			Size += DELTA;


		XSize = sdr::Convert( Size );

		if( Timer.IsElapsed() ) {
			cio::COut << Size << txf::pad << (bso::nuint__)XSize.BufferSize() << txf::rfl << txf::commit;
			Timer.Launch();
		}

		if ( sdr::Convert( XSize.DSizeBuffer() ) != Size )
			ERRc();

		if ( Length != XSize.BufferSize() ) {
			cio::COut << Size << txf::pad << ( Length = XSize.BufferSize() ) << txf::nl << txf::commit;
			Timer.Launch();
		}


	} while ( Size < SDR_SIZE_MAX );

ERRErr
ERREnd
	cio::COut << txf::commit;
ERREpilog
}

int main( int argc, char *argv[] )
{
ERRFProlog
ERRFBegin
	COut << "Test of library " << SDRTutor.Name << ' ' << __DATE__" "__TIME__"\n";

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
		COut << "Usage: " << SDRTutor.Name << " [/i]\n\n";
		ERRExit( EXIT_FAILURE );
	}

ERRFErr
ERRFEnd
ERRFEpilog
	return ERRExitValue;
}
