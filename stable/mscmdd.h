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

#ifndef CPE__T_MS
#	error "Only implemented for windows".
#endif

#ifndef MSCMDD_ALLOWED
#	error "'MSCMDD' works only under Windows. If you want to use it, define 'MSCMDD_ALLOWED'."
#endif

#ifdef CPE__T_MS
#	include "windows.h"
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
			fwf::size__ Wanted,
			fwf::size__ Minimum )
		{
			MIDIHDR     midiHdr;
			int Err;

			midiHdr.lpData = (LPSTR)Buffer;

			midiHdr.dwBufferLength = Wanted;

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

			return Wanted;
		}
		virtual void FWFSynchronize( void )
		{}
	public:
		void reset( bso::bool__ P = true )
		{
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
			err::handle ErrHandle )
		{
			reset();

			if ( midiOutOpen( &_Handle, Device, 0, 0, CALLBACK_NULL) != MMSYSERR_NOERROR ) {
				if ( ErrHandle != err::hSkip )
					ERRf();
				else
					return false;
			}

			return true;
		}
	};

	class unsafe_midi_oflow___
	: public flw::oflow__
	{
	private:
		midi_oflow_functions___ _Functions;
		flw::datum__ _Cache[128];	// VK77 n'accepte que 128 octets à la fois.
	public:
		void reset( bool P = true )
		{
			flw::oflow__::reset( P );
			_Functions.reset( P );
		}
		unsafe_midi_oflow___( flw::size__ AmountMax = FLW_SIZE_MAX )
		: oflow__( _Functions, _Cache, sizeof( _Cache ), AmountMax )
		{
			reset( false );
		}
		virtual ~unsafe_midi_oflow___( void )
		{
			reset();
		}
		//f Initialization with socket 'Socket' and 'TimeOut' as timeout.
		bso::bool__ Init(
			int DeviceId,
			err::handle ErrHandle = err::hUsual )
		{
			reset();

			return _Functions.Init( DeviceId, ErrHandle );
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
