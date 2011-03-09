/*
	Header for the 'mscmdd' library by Claude SIMON (http://zeusw.org/intl/contact.html)
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

#ifndef MSCMDD__INC
#define MSCMDD__INC

#define MSCMDD_NAME		"MSCMDD"

#define	MSCMDD_VERSION	"$Revision$"

#define MSCMDD_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"

#include "ttr.h"

extern class ttr_tutor &MSCMDDTutor;

#if defined( XXX_DBG ) && !defined( MSCMDD_NODBG )
#define MSCMDD_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision$
//C Claude SIMON (http://zeusw.org/intl/contact.html)
//R $Date$

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D MuSiC MiDi Device 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "cpe.h"
#include "str.h"
#include "ctn.h"
#include "mtx.h"

#ifndef CPE__T_MS
#	error "Only implemented for windows".
#endif

#ifndef MSCMDD_ALLOWED
#	error "'MSCMDD' works only under Windows. If you want to use it, define 'MSCMDD_ALLOWED'."
#endif

#ifdef CPE__T_MS
#	include "windows.h"
#endif

#ifdef MSCMDD_ICACHE_SIZE
#	define MSCMDD__ICACHE_SIZE	MSCMDD_ICACHE_SIZE
#else
#	define MSCMDD__ICACHE_SIZE FLW__ICACHE_SIZE
#endif

#ifdef MSCMDD_OCACHE_SIZE
#	define MSCMDD__OCACHE_SIZE	MSCMDD_OCACHE_SIZE
#else
#	define MSCMDD__OCACHE_SIZE FLW__OCACHE_SIZE
#endif


namespace mscmdd {
#ifdef CPE__T_MS
	typedef UINT_PTR	handler__;
	typedef char name__[MAXPNAMELEN];
#endif

	class midi_oflow_functions___
	: public fwf::oflow_functions___
	{
	private:
		HMIDIOUT    _Handle;
	protected:
		virtual fwf::size__ FWFWrite(
			const fwf::datum__ *Buffer,
			fwf::size__ Maximum )
		{
			MIDIHDR     midiHdr;
			int Err;

			midiHdr.lpData = (LPSTR)Buffer;

			midiHdr.dwBufferLength = Maximum;

			midiHdr.dwFlags = 0;

#ifdef MSCMDD_DBG
			if ( _Handle == NULL )
				ERRu();
#endif
			/* Prepare the buffer and MIDIHDR */
			if ( midiOutPrepareHeader( _Handle,  &midiHdr, sizeof(MIDIHDR)) != MMSYSERR_NOERROR )
				ERRf();

			if ( midiOutLongMsg( _Handle, &midiHdr, sizeof(MIDIHDR ) ) != MMSYSERR_NOERROR )
				ERRf();

			while ( ( Err = midiOutUnprepareHeader( _Handle, &midiHdr, sizeof(MIDIHDR ) ) ) == MIDIERR_STILLPLAYING )
			{}

			if ( Err != MMSYSERR_NOERROR )
				ERRf();

			return Maximum;
		}
		virtual void FWFCommit( void )
		{}
	public:
		void reset( bso::bool__ P = true )
		{
			fwf::oflow_functions___::reset( P );

			if ( P )
				if ( _Handle != NULL )
					if ( midiOutClose( _Handle ) != MMSYSERR_NOERROR )
						ERRf();

			_Handle = NULL;
		}
		midi_oflow_functions___( void )
		{
			reset( false );
		}
		~midi_oflow_functions___( void )
		{
			reset();
		}
		bso::bool__ Init(
			int Device,
			err::handling__ ErrHandling = err::h_Default,
			fwf::thread_safety__ ThreadSafety = fwf::ts_Default )
		{
			reset();

			fwf::oflow_functions___::Init( ThreadSafety );

			if ( midiOutOpen( &_Handle, Device, 0, 0, CALLBACK_NULL) != MMSYSERR_NOERROR ) {
				if ( ErrHandling != err::hUserDefined )
					ERRf();
				else
					return false;
			}

			return true;
		}
	};

	// VK77 n'accepte que 128 octets à la fois.

	template <int CacheSize = MSCMDD__OCACHE_SIZE> class midi_oflow___
	: public flw::standalone_oflow__<CacheSize>
	{
	private:
		midi_oflow_functions___ _Functions;
	public:
		void reset( bool P = true )
		{
			flw::standalone_oflow__<CacheSize>::reset( P );
			_Functions.reset( P );
		}
		midi_oflow___( void )
		{
			reset( false );
		}
		virtual ~midi_oflow___( void )
		{
			reset();
		}
		//f Initialization with socket 'Socket' and 'TimeOut' as timeout.
		bso::bool__ Init(
			int DeviceId,
			flw::size__ AmountMax = FLW_SIZE_MAX,
			err::handling__ ErrHandle = err::h_Default )
		{
			flw::standalone_oflow__<CacheSize>::Init( _Functions, AmountMax );

			return _Functions.Init( DeviceId, ErrHandle );
		}
		bso::bool__ Init(
			int DeviceId,
			err::handling__ ErrHandle )
		{
			return Init( DeviceId, FLW_SIZE_MAX, ErrHandle );
		}
	};

	struct _data___
	{
		mtx::mutex_handler__ Access;	// Pour protèger l'accés aus données de cet structure.
		mtx::mutex_handler__ Full;		// Pour faire attendre le producteur si 'Buffer' est plein.
		mtx::mutex_handler__ Empty;		// Pout faire attendre le consommateur si 'Buffer' est vide.
		fwf::datum__ *Buffer;
		fwf::size__ Size, Available, Position;
		bso::bool__ Purge;	// Lorsque à 'true', purge l'ensemble des données MIDI.
	};

	inline bso::bool__ _IsFull( const _data___ &Data )
	{
#ifdef MSCMDD_DBG
		if ( !mtx::IsLocked( Data.Access ) )
			ERRc();
#endif
		return ( ( Data.Available + Data.Position ) == Data.Size );
	}

	inline bso::bool__ _IsEmpty( const _data___ &Data )
	{
#ifdef MSCMDD_DBG
		if ( !mtx::IsLocked( Data.Access ) )
			ERRc();
#endif
		return ( Data.Available == 0 );
	}

	inline bso::size__ _Emptyness( const _data___ &Data )
	{
#ifdef MSCMDD_DBG
		if ( !mtx::IsLocked( Data.Access ) )
			ERRc();
#endif
		return ( Data.Size - ( Data.Available + Data.Position ) );
	}


	class midi_iflow_functions___
	: public fwf::iflow_functions___
	{
	private:
		bso::bool__ _Started;
		HMIDIIN _Handle;
		MIDIHDR _Header[3];
		fwf::datum__ _Cache[2000];
		char _HeaderBuffer[512][3];
		_data___ _Data;
		virtual fwf::size__ FWFRead(
			fwf::size__ Maximum,
			fwf::datum__ *Buffer );
		virtual void FWFDismiss( void )
		{
			if ( _Started ) {
				midiInStop( _Handle );
				_Started = false;
			}
		}
		void _Purge( void )
		{
			_Data.Purge = true;

			if ( midiInReset( _Handle ) != MMSYSERR_NOERROR )
				ERRf();
		}
	public:
		void reset( bso::bool__ P = true )
		{
			fwf::iflow_functions___::reset( P );

			if ( P ) {
				if ( _Data.Buffer != NULL ) {
					if ( _Started )
						if ( midiInStop( _Handle ) != MMSYSERR_NOERROR )
						ERRf();

					_Purge();

					mtx::Delete( _Data.Access );
					mtx::Delete( _Data.Full );
					mtx::Delete( _Data.Empty, true );	// Comme toute les données ont été lues, ce mutex est verrouillé, on autorise donc sa destruction même si verrouillé.


					midiInClose( _Handle );
				}
			}

			_Started = false;
			_Data.Available = _Data.Position = _Data.Size = 0;
			_Data.Access = _Data.Full = _Data.Empty = MTX_INVALID_HANDLER;
			_Data.Buffer = NULL;
		}
		midi_iflow_functions___( void )
		{
			reset( false );
		}
		~midi_iflow_functions___( void )
		{
			reset();
		}
		bso::bool__ Init(
			int Device,
			err::handling__ ErrHandling = err::h_Default,
			fwf::thread_safety__ ThreadSafety = fwf::ts_Default );
		void Start( void )
		{
			if ( !_Started ) {
				if ( midiInStart( _Handle ) != MMSYSERR_NOERROR )
					ERRf();
				_Started = true;
			}
		}
		void Stop( void )
		{
			if ( !_Started ) {
				if ( midiInStop( _Handle ) != MMSYSERR_NOERROR )
					ERRf();
				_Started = false;
			}

		}
	};

	template <int CacheSize = MSCMDD__OCACHE_SIZE> class midi_iflow___
	: public flw::standalone_iflow__<CacheSize>
	{
	private:
		midi_iflow_functions___ _Functions;
	public:
		void reset( bool P = true )
		{
			flw::standalone_iflow__<CacheSize>::reset( P );
			_Functions.reset( P );
		}
		midi_iflow___( void )
		{
			reset( false );
		}
		virtual ~midi_iflow___( void )
		{
			reset();
		}
		bso::bool__ Init(
			int DeviceId,
			flw::size__ AmountMax = FLW_SIZE_MAX,
			err::handling__ ErrHandle = err::h_Default )
		{
			flw::standalone_iflow__<CacheSize>::Init( _Functions, AmountMax );

			return _Functions.Init( DeviceId, ErrHandle );
		}
		bso::bool__ Init(
			int DeviceId,
			err::handling__ ErrHandle )
		{
			return Init( DeviceId, FLW_SIZE_MAX, ErrHandle );
		}
		void Start( void )
		{
			_Functions.Start();
		}
		void Stop( void )
		{
			_Functions.Stop();
		}
	};


	typedef str::string_	description_;
	typedef str::string		description;

	typedef ctn::E_XMCONTAINER_( description_ ) descriptions_;
	E_AUTO( descriptions )

	bso::ulong__ GetMidiInDeviceDescriptions( descriptions_ &Descriptions );
	bso::ulong__ GetMidiOutDeviceDescriptions( descriptions_ &Descriptions );

}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
