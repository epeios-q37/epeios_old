/*
	'mscmdd' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Requires the 'mscmdd' header file ('mscmdd.h').
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

static void Fill_(
	const char *Buffer,
	bso::size__ Amount,
	_data___ &Data )
{
	bso::bool__ Wait = false;
	bso::size__ AwareAmount = 0;

	if ( Data.Purge )	// Pas de protection; c'est un booléen uniquement accèdé en lecture.
		return;

	while ( Amount != 0 ) {
		mtx::Lock( Data.Full );	// On attend tant que 'Data.Buffer' est plein.
		mtx::Unlock( Data.Full );

		mtx::Lock( Data.Access ) ;

		AwareAmount = _Emptyness( Data );

		if ( AwareAmount > Amount )
			AwareAmount = Amount;

		memcpy( Data.Buffer + Data.Available + Data.Position, Buffer, AwareAmount );

		if ( _IsEmpty( Data ) )
			mtx::Unlock( Data.Empty );	// Si 'Data.Buffer' était vide, on signale au consommateur que ce n'est plus le cas.

		Amount -= AwareAmount;
		Buffer += AwareAmount;

		Data.Available += AwareAmount;

		if( _IsFull( Data ) )
			mtx::Lock( Data.Full );

		mtx::Unlock( Data.Access );
	}
}

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
	bso::bool__ Wait = false;
ERRBegin
	switch( wMsg ) {
	case MIM_DATA:
		char Buffer[3];
		Buffer[0] = dwParam1 & 0xff;
		switch ( mscmdm::DetermineEvent( dwParam1 & 0xff, ( dwParam1 & 0xff00 ) >> 8, Event ) ) {
		case mscmdm::etMIDI:
			switch ( mscmdm::GetMIDIEventDataSize( (mscmdm::midi_event__)Event ) ) {
			case 0:
				Fill_( Buffer, 1, Data );
				break;
			case 1:
				Buffer[1] = ( ( dwParam1 & 0xff00 ) >> 8 );
				Fill_( Buffer, 2, Data );
				break;
			case 2:
				Buffer[1] = ( ( dwParam1 & 0xff00 ) >> 8 );
				Buffer[2] =(fwf::datum__)( ( dwParam1 & 0xff0000 ) >> 16 );
				Fill_( Buffer, 3, Data );
				break;
			default:
				ERRc();
				break;
			}
			break;
		case mscmdm::etSystem:
			Fill_( Buffer, 1, Data );
			break;
		default:
			ERRc();
			break;
		}
		break;
	case MIM_OPEN:
		break;
	case MIM_LONGDATA:
		Header = (MIDIHDR *)dwParam1;

		Fill_( Header->lpData, Header->dwBytesRecorded, Data );

		Header->dwUser = 0;

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

fwf::size__ mscmdd::midi_in___::Read(
	fwf::size__ Maximum,
	fwf::datum__ *Buffer )
{
	bso::ubyte__ Amount = sizeof( _Header ) / sizeof( *_Header );

	while ( Amount-- ) {
		if( _Header[Amount].dwUser == 0 ) {
			_Header[Amount].lpData = _HeaderBuffer[Amount];
			_Header[Amount].dwUser = _Header[Amount].dwBufferLength = sizeof( _HeaderBuffer ) / sizeof( *_HeaderBuffer );

			if ( midiInUnprepareHeader( _Handle, &_Header[Amount], sizeof( _Header[Amount] ) ) != MMSYSERR_NOERROR )
				ERRs();

			_Header[Amount].dwFlags = 0;

			if ( midiInPrepareHeader( _Handle, &_Header[Amount], sizeof( _Header[Amount] ) ) != MMSYSERR_NOERROR )
				ERRs();

			if ( midiInAddBuffer( _Handle, &_Header[Amount], sizeof( _Header[Amount] ) ) != MMSYSERR_NOERROR )
				ERRs();
		}
	}

	mtx::Lock( _Data.Access );

	if ( _IsEmpty( _Data ) ) {
		if ( !_Started ) {
				if ( midiInStart( _Handle ) != MMSYSERR_NOERROR )
					ERRf();
			_Started = true;
		}

		mtx::Unlock( _Data.Access );

		mtx::Lock( _Data.Empty );	// On attend si nécessaire que des données soient didponibles.
		mtx::Unlock( _Data.Empty );

		mtx::Lock( _Data.Access );
	}

	// Bien que '_Data.Empty' ne soit pas verrouilles à ce point, il peut ne pas y avoir de données disponibles, si toutes les données ont été lues.

	if ( Maximum > _Data.Available )
		Maximum = _Data.Available;

	if ( _IsFull( _Data ) )
		mtx::Unlock( _Data.Full );	// Si '_Data.Buffer' était plein, on signale au producteur que ce n'est plus le cas.

	memcpy( Buffer, _Data.Buffer + _Data.Position, Maximum );

	_Data.Available -= Maximum;
	_Data.Position += Maximum;

	if ( _Data.Available == 0 ) {
		_Data.Position = 0;
		mtx::Lock( _Data.Empty );
	}

	mtx::Unlock( _Data.Access );

	return Maximum;
}

bso::bool__ mscmdd::midi_in___::Init(
	int Device,
	err::handling__ ErrHandling )
{
	bso::ubyte__ Amount = sizeof( _Header ) / sizeof( *_Header );
	reset();

	_Data.Access = mtx::Create( mtx::mFree );
	_Data.Full = mtx::Create( mtx::mFree );
	_Data.Empty = mtx::Create( mtx::mFree );

	_Data.Purge = false;

	_Data.Buffer = _Cache;

	if ( midiInOpen( &_Handle, Device, (DWORD)MidiInProc_, (DWORD)&_Data, CALLBACK_FUNCTION ) != MMSYSERR_NOERROR ) {
		if ( ErrHandling != err::hUserDefined )
			ERRf();
		else
			return false;
	}

	while( Amount-- ) {
		_Header[Amount].lpData = _HeaderBuffer[Amount];
		_Header[Amount].dwUser = _Header[Amount].dwBufferLength = sizeof( _HeaderBuffer ) / sizeof( *_HeaderBuffer );
		_Header[Amount].dwFlags = 0;

		if ( midiInPrepareHeader( _Handle, &_Header[Amount], sizeof( _Header[Amount] ) ) != MMSYSERR_NOERROR )
			ERRs();

		if ( midiInAddBuffer( _Handle, &_Header[Amount], sizeof( _Header[Amount] ) ) != MMSYSERR_NOERROR )
			ERRs();
	}

	_Data.Size = sizeof( _Cache ) / sizeof( *_Cache );

	mtx::Lock( _Data.Empty );

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
