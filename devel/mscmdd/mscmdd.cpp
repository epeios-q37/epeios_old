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

#ifdef MSCMDD__ALSA

#endif

#ifdef MSCMDD__WINDOWS
static void Fill_(
	const char *Buffer,
	bso::size__ Amount,
	_data___ &Data )
{
	bso::bool__ Wait = false;
	bso::size__ AwareAmount = 0;

	if ( Data.Purge )	// Pas de protection; c'est un bool�en uniquement acc�d� en lecture.
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
			mtx::Unlock( Data.Empty );	// Si 'Data.Buffer' �tait vide, on signale au consommateur que ce n'est plus le cas.

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
				Buffer[2] =(fdr::datum__)( ( dwParam1 & 0xff0000 ) >> 16 );
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

fdr::size__ mscmdd::midi_in___::Read(
	fdr::size__ Maximum,
	fdr::datum__ *Buffer )
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

		mtx::Lock( _Data.Empty );	// On attend si n�cessaire que des donn�es soient didponibles.
		mtx::Unlock( _Data.Empty );

		mtx::Lock( _Data.Access );
	}

	// Bien que '_Data.Empty' ne soit pas verrouilles � ce point, il peut ne pas y avoir de donn�es disponibles, si toutes les donn�es ont �t� lues.

	if ( Maximum > _Data.Available )
		Maximum = _Data.Available;

	if ( _IsFull( _Data ) )
		mtx::Unlock( _Data.Full );	// Si '_Data.Buffer' �tait plein, on signale au producteur que ce n'est plus le cas.

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

	// '_Data' n'est pas initialis�, mais ce n'est pas grave, car ne sera ps utilis� tant qu'un 'Start' n'aura pas �t� lanc�.
	if ( midiInOpen( &_Handle, Device, (DWORD)MidiInProc_, (DWORD)&_Data, CALLBACK_FUNCTION ) != MMSYSERR_NOERROR ) {
		if ( ErrHandling != err::hUserDefined )
			ERRf();
		else
			return false;
	}

	_Data.Access = mtx::Create( mtx::mFree );
	_Data.Full = mtx::Create( mtx::mFree );
	_Data.Empty = mtx::Create( mtx::mFree );
	_Data.Purge = false;
	_Data.Buffer = _Cache;

	// '_Data' seulement initialis� maintenant pour signaler la r�ussite de l'op�ration d'ouverture.

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
	name_ &Name )
{
	while ( *WString ) {
		Name.Append( (char)*WString );

		WString++;
	}

}


// Pour g++ sous 'cygwin'.
static inline void Convert_(
	const CHAR *RawString,
	name_ &Name )
{
	Name.Append( RawString );
}
#endif

#ifdef MSCMDD__ALSA
typedef ctn::E_XMCONTAINER_( str::string_ ) strings_;
E_AUTO( strings )

static bso::ulong__ GetMIDIDevices_(
	strings_ &Names,
	nmes_ &Names,
	snd_rawmidi_stream_t Direction )
{
	bso::ulong__ Count = 0;
ERRProlog
	str::string Name;
	bso::integer_buffer__ Buffer;
ERRBegin
	int	cardNum;

   // Start with first card
   cardNum = -1;

	for (;;) {
		snd_ctl_t *cardHandle;

		// Get next sound card's card number. When "cardNum" == -1, then ALSA
		// fetches the first card
		if ( snd_card_next (&cardNum ) < 0)
			ERRs();

		// No more cards? ALSA sets "cardNum" to -1 if so
		if ( cardNum < 0 ) break;

		// Open this card's control interface. We specify only the card number -- not
		// any device nor sub-device too
		{
				char   str[64];

				sprintf(str, "hw:%i", cardNum);

				if ( snd_ctl_open(&cardHandle, str, 0) < 0)
					ERRs();
		  }

		{
			int      devNum;

			// Start with the first MIDI device on this card
			devNum = -1;
		
			for (;;)
			{
				snd_rawmidi_info_t  *rawMidiInfo;
				register int        subDevCount, i;

				// Get the number of the next MIDI device on this card
				if ( snd_ctl_rawmidi_next_device(cardHandle, &devNum) )
					ERRs();

				// No more MIDI devices on this card? ALSA sets "devNum" to -1 if so.
				// NOTE: It's possible that this sound card may have no MIDI devices on it
				// at all, for example if it's only a digital audio card
				if (devNum < 0) break;

				// To get some info about the subdevices of this MIDI device (on the card), we need a
				// snd_rawmidi_info_t, so let's allocate one on the stack
				snd_rawmidi_info_alloca(&rawMidiInfo);
				memset(rawMidiInfo, 0, snd_rawmidi_info_sizeof());

				// Tell ALSA which device (number) we want info about
				snd_rawmidi_info_set_device(rawMidiInfo, devNum);

				// Get info on the MIDI outs of this device
				snd_rawmidi_info_set_stream(rawMidiInfo, Direction );

				i = -1;
				subDevCount = 1;

				// More subdevices?
				while (++i < subDevCount) {
					// Tell ALSA to fill in our snd_rawmidi_info_t with info on this subdevice
					snd_rawmidi_info_set_subdevice(rawMidiInfo, i);

					if ( snd_ctl_rawmidi_info(cardHandle, rawMidiInfo) < 0 )
						ERRs();

					// Print out how many subdevices (once only)
					if (!i)
						subDevCount = snd_rawmidi_info_get_subdevices_count(rawMidiInfo);

					// NOTE: If there's only one subdevice, then the subdevice number is immaterial,
					// and can be omitted when you specify the hardware name

					Name.Init( "hw:" );
					Name.Append( bso::Convert( (bso::ulong__)cardNum, Buffer ) );
					Name.Append( ',' );
					Name.Append( bso::Convert( (bso::ulong__)devNum, Buffer ) );
					Name.Append( ',' );
					Name.Append( bso::Convert( (bso::ulong__)i, Buffer ) );

					Names.Append( Name );

					Names.Append( str::string( snd_rawmidi_info_get_subdevice_name( rawMidiInfo ) ) );

					Count++;

				}
			}
			// Close the card's control interface after we're done with it
			snd_ctl_close(cardHandle);
		}
	}

	snd_config_update_free_global();
ERRErr
ERREnd
ERREpilog
	return Count;
}

bso::bool__ mscmdd::GetMIDIInDeviceName(
	int Device,
	str::string_ &Name )
{
	bso::bool__ Success = false;
ERRProlog
	strings Names;
	names Names;
ERRBegin
	Names.Init();
	NAmes.Init();
	
	 if ( GetMIDIDevices_( Names, NAmes, SND_RAWMIDI_STREAM_INPUT ) <= Device )
		 ERRReturn;
	 
	 Success = true;
	 
	 Name = Names( Device );
ERRErr
ERREnd
ERREpilog
	 return Success;
}

bso::bool__ mscmdd::GetMIDIOutDeviceName(
	int Device,
	str::string_ &Name )
{
	bso::bool__ Success = false;
ERRProlog
	strings Names;
	names NAmes;
ERRBegin
	Names.Init();
	NAmes.Init();
	
	 if ( GetMIDIDevices_( Names, Names, SND_RAWMIDI_STREAM_OUTPUT ) <= Device )
		 ERRReturn;
	 
	 Success = true;
	 
	 Name = Names( Device );
ERRErr
ERREnd
ERREpilog
	 return Success;
}

#endif


bso::ulong__ mscmdd::GetMidiInDevicesNames( names_ &Names )
{
	bso::ulong__ Count = 0;
#ifdef MSCMDD__WINDOWS
ERRProlog
	MIDIINCAPS InCaps;
	bso::ulong__ Counter = 0;
	name Name;
ERRBegin
	Count =  midiInGetNumDevs();

	while ( Counter < Count ) {
		midiInGetDevCaps( Counter++, &InCaps, sizeof( InCaps ) );

		Name.Init();

		Convert_( InCaps.szPname, Name );

		Names.Append( Name );
	}
ERRErr
ERREnd
ERREpilog
#elif defined( MSCMDD__ALSA )
ERRProlog
	strings Names;
ERRBegin
	Names.Init();
	
	Count = GetMIDIDevices_( Names, Names, SND_RAWMIDI_STREAM_INPUT );
ERRErr
ERREnd
ERREpilog
#endif
	return Count;
}

bso::ulong__ mscmdd::GetMidiOutDevicesNames( names_ &Names )
{
	bso::ulong__ Count = 0;
#ifdef MSCMDD__WINDOWS
ERRProlog
	MIDIOUTCAPS OutCaps;
	bso::ulong__ Counter = 0;
	name Name;
ERRBegin
	Count =  midiOutGetNumDevs();

	while ( Counter < Count ) {
		midiOutGetDevCaps( Counter++, &OutCaps, sizeof( OutCaps ) );

		Name.Init();

		Convert_( OutCaps.szPname, Name );

		Names.Append( Name );
	}
ERRErr
ERREnd
ERREpilog
#elif defined( MSCMDD__ALSA )
ERRProlog
	strings Names;
ERRBegin
	Names.Init();
	
	Count = GetMIDIDevices_( Names, NAmes, SND_RAWMIDI_STREAM_OUTPUT );
ERRErr
ERREnd
ERREpilog
#endif
	return Count;
}

#define CASE( label ) LCL_CASE( label, s )

const char *mscmdd::Label( status__ Status )
{
	switch ( Status ) {
	CASE( UnableToOpenMIDIInDevice );
	CASE( UnableToOpenMIDIOutDevice );
	default:
		ERRu();
		break;
	}

	return NULL;	// Pour �viter un 'warning'.
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