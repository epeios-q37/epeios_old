/*
	'mscmdd' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Requires the 'mscmdd' header file ('mscmdd.h').
	Copyright (C) $COPYRIGHT_DATES$Claude SIMON (http://zeusw.org/intl/contact.html).
$_RAW_$
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

#define MSCMDD__COMPILATION

#include "mscmdd.h"

class mscmddtutor
: public ttr_tutor
{
public:
	mscmddtutor( void )
	: ttr_tutor( MSCMDD_NAME )
	{
#ifdef MSCMDD_DBG
		Version = MSCMDD_VERSION "\b\bD $";
#else
		Version = MSCMDD_VERSION;
#endif
		Owner = MSCMDD_OWNER;
		Date = "$Date$";
	}
	virtual ~mscmddtutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace mscmdd;

#include "mscmdm.h"

static void CALLBACK MidiInProc_(
  HMIDIIN hMidiIn,  
  UINT wMsg,        
  DWORD dwInstance, 
  DWORD dwParam1,   
  DWORD dwParam2 )
{
ERRProlog
	_data___ &Data = *(_data___ *)dwInstance;
	bso::ubyte__ Event = 0;
	MIDIHDR *Header = NULL;
ERRBegin
	if ( !Data.Purge ) {
		mtx::Lock( Data.WriteLock );	// On attend que les précédentes données soient toutes lues.
		mtx::Unlock( Data.WriteLock );
	}

#ifdef MSCMDD_DBG
	if ( Data.Available != 0 )
		ERRc();
#endif

	switch( wMsg ) {
	case MIM_DATA:
		if ( !Data.Purge ) {
			Data.Buffer[0] = dwParam1 & 0xff;
			switch ( mscmdm::DetermineEvent( dwParam1 & 0xff, ( dwParam1 & 0xff00 ) >> 8, Event ) ) {
			case mscmdm::etMIDI:
				switch ( mscmdm::GetMIDIEventDataSize( (mscmdm::midi_event__)Event ) ) {
				case 0:
					break;
				case 1:
					Data.Buffer[1] = ( ( dwParam1 & 0xff00 ) >> 8 );
					Data.Available = 2;
					break;
				case 2:
					Data.Buffer[1] = ( ( dwParam1 & 0xff00 ) >> 8 );
					Data.Buffer[2] =(fwf::datum__)( ( dwParam1 & 0xff0000 ) >> 16 );
					Data.Available = 3;
					break;
				default:
					ERRc();
					break;
				}
				break;
			default:
				ERRc();
				break;
			}
			mtx::Lock( Data.WriteLock );
			mtx::Unlock( Data.ReadLock );	// On signale au consommateur que de nouvelles données sont disponibles.
		}
		break;
	case MIM_OPEN:
		break;
	case MIM_LONGDATA:
		if ( !Data.Purge ) {
			Header = (MIDIHDR *)dwParam1;
			memcpy( Data.Buffer, Header->lpData, Header->dwBytesRecorded );
			Data.Available = Header->dwBytesRecorded;
			Data.Recycle = true;

			mtx::Lock( Data.WriteLock );
			mtx::Unlock( Data.ReadLock );	// On signale au consommateur que de nouvelles données sont disponibles.
		}
		break;
	case MIM_CLOSE:
		break;
	default:
		ERRs();
		break;
	}


ERRErr
ERREnd
ERREpilog
}

fwf::size__ mscmdd::midi_iflow_functions___::FWFRead(
	fwf::size__ Maximum,
	fwf::datum__ *Buffer )
{
	if ( !_Data.Available ) {
		if ( !_Started ) {
			midiInStart( _Handle );
			_Started = true;
		}

		mtx::Lock( _Data.ReadLock );	// On attend que des données soient disponibles.
		mtx::Unlock( _Data.ReadLock );

		if ( _Data.Recycle ) {

//			tht::Suspend( 200 );

			while ( ( _Header.dwFlags & MHDR_DONE ) != MHDR_DONE)
				_Started = _Started;

			if ( midiInUnprepareHeader( _Handle, &_Header, sizeof( _Header ) ) != MMSYSERR_NOERROR )
				ERRs();

			_Header.lpData = _HeaderBuffer;
			_Header.dwBufferLength = sizeof( _HeaderBuffer );
			_Header.dwFlags = 0;

			if ( midiInPrepareHeader( _Handle, &_Header, sizeof( _Header ) ) != MMSYSERR_NOERROR )
				ERRs();

			if ( midiInAddBuffer( _Handle, &_Header, sizeof( _Header ) ) != MMSYSERR_NOERROR )
				ERRs();

			_Data.Recycle = false;
		}
	}

	if ( Maximum > _Data.Available )
		Maximum = _Data.Available;

	memcpy( Buffer, _Data.Buffer + _Data.Position, Maximum );

	_Data.Available -= Maximum;
	_Data.Position += Maximum;

	if ( _Data.Available == 0 ) {
		_Data.Position = 0;
		mtx::Lock( _Data.ReadLock );
		mtx::Unlock( _Data.WriteLock );
	}

	return Maximum;
}

bso::bool__ mscmdd::midi_iflow_functions___::Init(
	int Device,
	err::handling__ ErrHandling,
	fwf::thread_safety__ ThreadSafety )
{
	reset();

	fwf::iflow_functions___::Init( ThreadSafety );

	_Data.ReadLock = mtx::Create( mtx::mFree );
	_Data.WriteLock = mtx::Create( mtx::mFree );
	_Data.Purge = false;

	_Data.Buffer = _Cache;

	if ( midiInOpen( &_Handle, Device, (DWORD)MidiInProc_, (DWORD)&_Data, CALLBACK_FUNCTION ) != MMSYSERR_NOERROR ) {
		if ( ErrHandling != err::hUserDefined )
			ERRf();
		else
			return false;
	}

	_Header.lpData = _HeaderBuffer;
	_Header.dwBufferLength = sizeof( _HeaderBuffer );
	_Header.dwFlags = 0;

	if ( midiInPrepareHeader( _Handle, &_Header, sizeof( _Header ) ) != MMSYSERR_NOERROR )
		ERRs();

	if ( midiInAddBuffer( _Handle, &_Header, sizeof( _Header ) ) != MMSYSERR_NOERROR )
		ERRs();

	mtx::Lock( _Data.ReadLock );

	_Data.Recycle = false;;

	return true;
}


static void Convert_(
	const WCHAR *WString,
	description_ &String )
{
	while ( *WString ) {
		String.Append( (char)*WString );

		WString++;
	}

}

bso::ulong__ mscmdd::GetMidiInDeviceDescriptions( descriptions_ &Descriptions )
{
	bso::ulong__ Count;
ERRProlog
	MIDIINCAPS InCaps;
	bso::ulong__ Counter = 0;
	description Description;
ERRBegin
	Count =  midiInGetNumDevs();

	while ( Counter < Count ) {
		midiInGetDevCaps( Counter++, &InCaps, sizeof( InCaps ) );

		Description.Init();

		Convert_( InCaps.szPname, Description );

		Descriptions.Append( Description );
	}
ERRErr
ERREnd
ERREpilog
	return Count;
}

bso::ulong__ mscmdd::GetMidiOutDeviceDescriptions( descriptions_ &Descriptions )
{
	bso::ulong__ Count;
ERRProlog
	MIDIOUTCAPS OutCaps;
	bso::ulong__ Counter = 0;
	description Description;
ERRBegin
	Count =  midiOutGetNumDevs();

	while ( Counter < Count ) {
		midiOutGetDevCaps( Counter++, &OutCaps, sizeof( OutCaps ) );

		Description.Init();

		Convert_( OutCaps.szPname, Description );

		Descriptions.Append( Description );
	}
ERRErr
ERREnd
ERREpilog
	return Count;
}


using namespace mscmdd;

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class mscmddpersonnalization
: public mscmddtutor
{
public:
	mscmddpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~mscmddpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the ending of the application  */
	}
};


/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

// 'static' by GNU C++.

static mscmddpersonnalization Tutor;

ttr_tutor &MSCMDDTutor = Tutor;
