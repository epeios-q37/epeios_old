/*
  Test source for the 'mscvkx' library by Claude SIMON (http://zeusw.org/intl/contact.html).
  Copyright (C) 2002 Claude SIMON (http://zeusw.org/intl/contact.html).

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

#include "mscvkx.h"

#include "err.h"
#include "cio.h"
#include "flf.h"
#include "mscmdf.h"

using cio::cout;
using cio::cerr;

void Generic( int argc, char *argv[] )
{
ERRProlog
ERRBegin
ERRErr
ERREnd
ERREpilog
}

void Test( int argc, char *argv[] )
{
ERRProlog
	flf::file_iflow___ IFlow;
	mscmdf::header_chunk__ HeaderChunk;
	mscmdf::track_chunk_size__ TrackChunkSize;
	mscmdm::event_header__ EventHeader;
	mscvkp::data_set DataSet;
	mscvkp::data_sets DataSets;
	flf::file_oflow___ OFlow;
	txf::text_oflow__ TOFlow( OFlow );
ERRBegin
	IFlow.Init( "test.mid" );

	DataSets.Init();

	mscmdf::GetHeaderChunk( IFlow, HeaderChunk );
	TrackChunkSize = mscmdf::GetTrackChunkDescription( IFlow );

	while( HeaderChunk.TrackChunkAmount-- ) {
		mscmdm::GetEventHeader( IFlow, EventHeader, false );

		while ( ( ( EventHeader.Event != mscmdm::eSystem ) || ( EventHeader.SystemEvent != mscmdm::seExclusive ) ) ) {
			mscmdm::PrintEvent( EventHeader, IFlow, cout, mscmdm::oFile );
			mscmdm::GetEventHeader( IFlow, EventHeader, false );

			cout << txf::nl;
		}

		while ( ( ( EventHeader.Event == mscmdm::eSystem ) && ( EventHeader.SystemEvent == mscmdm::seExclusive ) ) ) {
			DataSet.Init();
			mscvkp::Fill( IFlow, DataSet, mscmdm::oFile );
			DataSets.Append( DataSet );
			mscmdm::GetEventHeader( IFlow, EventHeader, false );
		}

		if ( ( EventHeader.Event != mscmdm::eMeta ) && ( EventHeader.MetaEvent = mscmdm::meEndOfTrack ) )
			ERRf();
	}

	OFlow.Init( "dump.xml" );

	mscvkx::Write( DataSets, TOFlow );
ERRErr
ERREnd
ERREpilog
}

void TestReading( void )
{
ERRProlog
	flf::file_iflow___ IFlow;
	xtf::extended_text_iflow__ XFlow;
	mscvkp::data_sets DataSets;
	flf::file_oflow___ OFlow;
	txf::text_oflow__ TOFlow( OFlow );
ERRBegin
	IFlow.Init( "dump.xml" );

	XFlow.Init( IFlow );

	DataSets.Init();

	if ( !mscvkx::Parse( XFlow, DataSets ) )
		ERRf();


	OFlow.Init( "dump2.xml" );

	mscvkx::Write( DataSets, TOFlow );
ERRErr
ERREnd
ERREpilog
}


int main( int argc, char *argv[] )
{
	int ExitCode = EXIT_SUCCESS;
ERRFProlog
ERRFBegin
	cout << "Test of library " << MSCVKXTutor.Name << ' ' << __DATE__" "__TIME__"\n";

	switch( argc ) {
	case 1:
		Generic( argc, argv );
		Test( argc, argv );//
//		TestReading();
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
		cout << "Usage: " << MSCVKXTutor.Name << " [/i]\n\n";
		ERRi();
	}

ERRFErr
	ExitCode = EXIT_FAILURE;
ERRFEnd
	cout << "\nEnd of program " << MSCVKXTutor.Name << ".\n";
ERRFEpilog
	return ExitCode;
}
