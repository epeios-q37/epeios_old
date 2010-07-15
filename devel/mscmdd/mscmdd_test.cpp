/*
	Test source for the 'mscmdd' library by Claude SIMON (http://zeusw.org/intl/contact.html).
	Copyright (C) 2004 Claude SIMON (http://zeusw.org/intl/contact.html).

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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mscmdd.h"
#include "mscmdm.h"
#include "mscmdf.h"
#include "mscvkp.h"

#include "err.h"
#include "cio.h"
#include "flx.h"
#include "str.h"

#include "mmreg.h"
#include "mmsystem.h"

using cio::cin;
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

void Recycle( 
	MIDIHDR *Header,
	HMIDIIN Device )
{
	MMRESULT Result;

	Header->dwFlags = 0;

	Result = midiInPrepareHeader( Device, Header, sizeof( *Header ) );
	Result = midiInAddBuffer( Device, Header, sizeof( *Header ) );
}

void CALLBACK MidiInProc(
  HMIDIIN hMidiIn,  
  UINT wMsg,        
  DWORD dwInstance, 
  DWORD dwParam1,   
  DWORD dwParam2    
)
{
ERRProlog
	MIDIHDR *Header = NULL;
	flw::oflow__ &Flow = *(flw::oflow__ *)dwInstance;
ERRBegin
	switch( wMsg ) {
	case MIM_DATA:
		break;
	case MIM_OPEN:
		break;
	case MIM_LONGDATA:
		Header = (MIDIHDR *)dwParam1;
		Flow.Write( Header->lpData, Header->dwBytesRecorded );
		Recycle( Header, hMidiIn );
		break;
	case MIM_CLOSE:
		break;
	}
ERRErr
ERREnd
ERREpilog
}

void Dump( const str::string_ &Buffer )
{
ERRProlog
	flx::bunch_iflow__<str::string_, bso::char__> Flow;
	mscmdf::header_chunk__ HeaderChunk;
	mscmdf::track_chunk_size__ TrackChunkSize;
	mscmdm::event_header__ EventHeader;
	mscvkp::data_set DataSet;
	mscvkp::data_sets DataSets;
ERRBegin
	Flow.Init( Buffer );

	DataSets.Init();

		mscmdm::GetEventHeader( Flow, EventHeader, true );

		while ( ( ( EventHeader.Event != mscmdm::eSystem ) || ( EventHeader.SystemEvent != mscmdm::seExclusive ) ) ) {
			mscmdm::PrintEvent( EventHeader, Flow, cout, mscmdm::oDevice );
			mscmdm::GetEventHeader( Flow, EventHeader, true );

			cout << txf::nl;
		}

		while ( ( Flow.AmountRed() < Buffer.Amount() ) && ( EventHeader.Event == mscmdm::eSystem ) && ( EventHeader.SystemEvent == mscmdm::seExclusive ) ) {
			DataSet.Init();
			mscvkp::Fill( Flow, DataSet, mscmdm::oDevice );
			DataSets.Append( DataSet );
			if ( Flow.AmountRed() < Buffer.Amount() )
				mscmdm::GetEventHeader( Flow, EventHeader, true );
		}
/*
		if ( ( EventHeader.Event != mscmdm::eMeta ) && ( EventHeader.MetaEvent = mscmdm::meEndOfTrack ) )
			ERRf();
*/
	cio::cout << DataSets;
ERRErr
ERREnd
ERREpilog
}


void In( void )
{
ERRProlog
	MIDIOUTCAPS OutCaps;
	MIDIINCAPS InCaps;
	HMIDIIN Device;
	MMRESULT Result;
	char Buffer[100];
	MIDIHDR Header;
	str::string String;
	flx::bunch_oflow___<str::string_, bso::char__> Flow;
ERRBegin
	cout << (unsigned long)midiInGetNumDevs() << txf::nl;
	cout << (unsigned long)midiOutGetNumDevs() << txf::nl;

	midiOutGetDevCaps( 0, &OutCaps, sizeof( OutCaps ) );
	midiOutGetDevCaps( 1, &OutCaps, sizeof( OutCaps ) );
	midiOutGetDevCaps( 2, &OutCaps, sizeof( OutCaps ) );

//	midiInGetDevCaps( 0, &InCaps, sizeof( InCaps ) );

	Header.dwBufferLength = sizeof( Buffer );
	Header.lpData = Buffer;

	String.Init();
	Flow.Init( String );

	if ( ( Result = midiInOpen( &Device, 0, (DWORD)MidiInProc, (DWORD)&Flow, CALLBACK_FUNCTION ) ) != MMSYSERR_NOERROR )
		ERRx();

	Recycle( &Header, Device );

	if ( ( Result = midiInStart( Device ) ) != MMSYSERR_NOERROR )
		ERRx();

	getchar();

	Result = midiInUnprepareHeader( Device, &Header, sizeof( Header ) );

	Result = midiInReset( Device );


	midiInClose( Device );

	Flow.reset();

	Dump( String );

ERRErr
ERREnd
ERREpilog
}

#define DATA "\xF0\x41\x10\x00\x1A\x12\x10\x00\x00\0Coucou\xF7"

void StreamOut( void )
{
ERRProlog
	MIDIOUTCAPS OutCaps;
	HMIDISTRM Device;
	MMRESULT Result;
	char Buffer[100];
	MIDIHDR Header;
	str::string String;
	flx::bunch_oflow___<str::string_, bso::char__> Flow;
	UINT_PTR DeviceId = 1;
	mscvkp::data_set DataSet;
ERRBegin
//	cout << (unsigned long)midiInGetNumDevs() << txf::nl;
	cout << (unsigned long)midiOutGetNumDevs() << txf::nl;

	midiOutGetDevCaps( 0, &OutCaps, sizeof( OutCaps ) );
	midiOutGetDevCaps( 1, &OutCaps, sizeof( OutCaps ) );
	midiOutGetDevCaps( 2, &OutCaps, sizeof( OutCaps ) );

//	midiInGetDevCaps( 0, &InCaps, sizeof( InCaps ) );

	Header.lpData = Buffer;

	Header.dwFlags = 0;

	DataSet.Init();

	DataSet.SetAddress( 0x1000000000 );

	DataSet.Append( "coucou" );

	String.Init();

	mscvkp::CreateMessage( DataSet, String );

	String.Recall( String.First(), String.Amount(), Header.lpData );

	Header.dwBufferLength = Header.dwBytesRecorded = String.Amount();

	if ( ( Result = midiStreamOpen( &Device, &DeviceId, 1, NULL, NULL, CALLBACK_NULL ) ) != MMSYSERR_NOERROR )
		ERRx();

	if ( ( Result = midiOutPrepareHeader( (HMIDIOUT)Device, &Header, sizeof(MIDIHDR) ) ) != MMSYSERR_NOERROR )
		ERRx();

	if ( ( Result = midiStreamOut( Device, &Header, sizeof(MIDIHDR) ) ) != MMSYSERR_NOERROR )
		ERRx();

	if ( ( Result = midiStreamRestart( Device ) ) != MMSYSERR_NOERROR )
		ERRx();

	if ( ( Result = midiOutUnprepareHeader( (HMIDIOUT)Device, &Header, sizeof(MIDIHDR) ) ) != MMSYSERR_NOERROR )
		ERRx();


ERRErr
ERREnd
	if ( ( Result = midiStreamClose( Device ) ) != MMSYSERR_NOERROR )
		ERRx();
ERREpilog
}

void Out( void )
{
ERRProlog
	HMIDIOUT    handle;
	MIDIHDR     midiHdr;
	UINT        err;
	char		sysEx[100];
	mscvkp::data_set DataSet;
	str::string String;
	MIDIOUTCAPS OutCaps;
ERRBegin
	midiOutGetDevCaps( 0, &OutCaps, sizeof( OutCaps ) );
	midiOutGetDevCaps( 1, &OutCaps, sizeof( OutCaps ) );
	midiOutGetDevCaps( 2, &OutCaps, sizeof( OutCaps ) );

	/* Open default MIDI Out device */
	if (!midiOutOpen(&handle, 1, 0, 0, CALLBACK_NULL))
	{
		/* Store pointer in MIDIHDR */
		midiHdr.lpData = sysEx;

		DataSet.Init();

		DataSet.SetAddress( 0x10000000 );

		DataSet.Append( "hello the World!" );

		String.Init();

		mscvkp::CreateMessage( DataSet, String );

		String.Recall( String.First(), String.Amount(), midiHdr.lpData );

		midiHdr.dwBufferLength = midiHdr.dwBytesRecorded = String.Amount();

		/* Flags must be set to 0 */
		midiHdr.dwFlags = 0;

		/* Prepare the buffer and MIDIHDR */
		err = midiOutPrepareHeader(handle,  &midiHdr, sizeof(MIDIHDR));
		if (!err)
		{
			/* Output the SysEx message */
			err = midiOutLongMsg(handle, &midiHdr, sizeof(MIDIHDR));
			if (err)
			{
				char   errMsg[120];

				midiOutGetErrorText(err, (LPWSTR)errMsg, 120);
				printf("Error: %s\r\n", errMsg);
			}

			/* Unprepare the buffer and MIDIHDR */
			while (MIDIERR_STILLPLAYING == midiOutUnprepareHeader(handle, &midiHdr, sizeof(MIDIHDR)))
			{
				/* Should put a delay in here rather than a busy-wait */		    
			}
		}

		/* Close the MIDI device */
		midiOutClose(handle);
	}
ERRErr
ERREnd
ERREpilog
}

void OutWithFlow( void )
{
ERRProlog
	mscmdd::unsafe_midi_oflow___ Flow;
	str::string String;
	MIDIOUTCAPS OutCaps;
	mscvkp::data_set DataSet;
ERRBegin
	midiOutGetDevCaps( 0, &OutCaps, sizeof( OutCaps ) );
	midiOutGetDevCaps( 1, &OutCaps, sizeof( OutCaps ) );
	midiOutGetDevCaps( 2, &OutCaps, sizeof( OutCaps ) );

	DataSet.Init();

	DataSet.SetAddress( 0x10000000 );

	DataSet.Append( "Salut !" );

	String.Init();

	mscvkp::CreateMessage( DataSet, String );

	Flow.Init( 1 );

	epeios::row__ Row = String.First();

	while ( Row != NONE ) {
		Flow.Put( String( Row ) );

		Row = String.Next( Row );
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
	cout << "Test of library " << MSCMDDTutor.Name << ' ' << __DATE__" "__TIME__"\n";

	switch( argc ) {
	case 1:
		Generic( argc, argv );
		In();
//		Out();
//		OutWithFlow();
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
		cout << "Usage: " << MSCMDDTutor.Name << " [/i]\n\n";
		ERRi();
	}

ERRFErr
	ExitCode = EXIT_FAILURE;
ERRFEnd
	cout << "\nEnd of program " << MSCMDDTutor.Name << ".\n";
ERRFEpilog
	return ExitCode;
}
