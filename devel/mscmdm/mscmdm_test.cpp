/*
  Test source for the 'mscmdm' library by Claude SIMON (http://zeusw.org/intl/contact.html).
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

#include "mscmdf.h"
#include "mscmdm.h"

#include "err.h"
#include "cio.h"
#include "flf.h"

void Generic( int argc, char *argv[] )
{
ERRProlog
ERRBegin
ERRErr
ERREnd
ERREpilog
}

class callback__
: public mscmdm::callback__
{
private:
	flw::oflow__ &_OFlow;
protected:
	virtual bso::bool__ MSCMDMHandleEvent(
		const mscmdm::event_header__ &EventHeader,
		const mscmdm::data_ &Data,
		mscmdm::origin__ Origin )
	{
		bso::bool__ Stop = false;
	ERRProlog
		mscmdm::data RawData;
	ERRBegin
		mscmdm::PrintEvent( EventHeader, Data, cio::cout );

		cio::cout << txf::nl;

		RawData.Init();

		if ( EventHeader.EventType == mscmdm::etMeta ) {
			RawData.Append( EventHeader.MetaEvent.Id );
			mscmdm::Encode( EventHeader.MetaEvent.Size, RawData );
			RawData.Append( Data );
		} else
			RawData = Data;

		mscmdm::PutEventHeader( EventHeader.DeltaTimeTicks, EventHeader.Id, RawData, EventHeader.MIDIEvent.Tied, _OFlow );

		Stop = ( ( EventHeader.EventType == mscmdm::etMeta ) && ( EventHeader.MetaEvent.Event == mscmdm::mtaEndOfTrack ) );
	ERRErr
	ERREnd
	ERREpilog
		return !Stop;
	}
public:
	callback__( flw::oflow__ &OFlow )
	: _OFlow( OFlow )
	{}
};

class callback_delayed
: public mscmdm::callback__
{
private:
	flw::oflow__ &_OFlow;
	mscmdm::events Events;
protected:
	virtual bso::bool__ MSCMDMHandleEvent(
		const mscmdm::event_header__ &EventHeader,
		const mscmdm::data_ &Data,
		mscmdm::origin__ Origin )
	{
		bso::bool__ Stop = false;
	ERRProlog
		mscmdm::event Event;
	ERRBegin
		mscmdm::PrintEvent( EventHeader, Data, cio::cout );

		cio::cout << txf::nl;

		Event.Init( EventHeader, Data );

		Events.Append( Event );

		Stop = ( ( EventHeader.EventType == mscmdm::etMeta ) && ( EventHeader.MetaEvent.Event == mscmdm::mtaEndOfTrack ) );

		if ( Stop ) {
			PutTrack( Events, _OFlow );
			Events.Init();
		}
	ERRErr
	ERREnd
	ERREpilog
		return !Stop;
	}
public:
	callback_delayed( flw::oflow__ &OFlow )
	: _OFlow( OFlow )
	{
		Events.Init();
	}
};

void Test( int argc, char *argv[] )
{
ERRProlog
	flf::file_iflow___ IFlow;
	mscmdf::header_chunk__ HeaderChunk;
	mscmdf::track_chunk_size__ TrackChunkSize;
	flf::file_oflow___ OFlow;
//	callback__ Callback( OFlow );
	callback_delayed Callback( OFlow );
ERRBegin
	IFlow.Init( "H:\\cvs\\pmsq\\MIDI\\OMS\\OMS010.mid" );
//	IFlow.Init( "H:\\cvs\\pmsq\\VK\\Factory.mid" );

	OFlow.Init( "Test.mid" );

	mscmdf::GetHeaderChunk( IFlow, HeaderChunk );
	mscmdf::PutHeaderChunk( HeaderChunk, OFlow );

	while( HeaderChunk.TrackChunkAmount-- ) {
		TrackChunkSize = mscmdf::GetTrackChunkSize( IFlow );
//		mscmdf::PutTrackChunkHeader( TrackChunkSize, OFlow );

		mscmdm::Parse( Callback, IFlow, mscmdm::oFile );

		cio::cout << txf::nl;
	}

ERRErr
ERREnd
ERREpilog
}



int main( int argc, char *argv[] )
{
	int ExitCode = EXIT_SUCCESS;
ERRFProlog
ERRFBegin
cio::cout << "Test of library " << MSCMDMTutor.Name << ' ' << __DATE__" "__TIME__"\n";

	switch( argc ) {
	case 1:
		Test( argc, argv );
		break;
	case 2:
		if ( !strcmp( argv[1], "/i" ) )
		{
			TTR.Advertise( cio::cout );
			break;
		}
	default:
		cio::cout << txf::sync;
		cio::cerr << "\nBad arguments.\n";
		cio::cout << "Usage: " << MSCMDMTutor.Name << " [/i]\n\n";
		ERRi();
	}

ERRFErr
	ExitCode = EXIT_FAILURE;
ERRFEnd
	cio::cout << "\nEnd of program " << MSCMDMTutor.Name << ".\n";
ERRFEpilog
	return ExitCode;
}
