/*
	Header for the 'wllio' library by Claude SIMON (csimon@epeios.org)
	Copyright (C) $COPYRIGHT_DATES$Claude SIMON (csimon@epeios.org).
$_RAW_$
	This file is part of the Epeios (http://epeios.org/) project.

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

#ifndef WLLIO__INC
#define WLLIO__INC

#define WLLIO_NAME		"WLLIO"

#define	WLLIO_VERSION	"$Revision$"

#define WLLIO_OWNER		"Claude SIMON (csimon@epeios.org)"

#include "ttr.h"

extern class ttr_tutor &WLLIOTutor;

#if defined( XXX_DBG ) && !defined( WLLIO_NODBG )
#define WLLIO_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision$
//C Claude SIMON (csimon@epeios.org)
//R $Date$

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D Windows Low-Level Input/Output 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "iodef.h"
#include "bso.h"
#include <io.h>
#include <fcntl.h>

#undef EOF

namespace wllio {

	using namespace iodef;

	typedef int amount__;

	class lowlevel_io__
	{
	private:
		int &FD_;
	public:
		lowlevel_io__( int &FD )
		: FD_( FD )
		{}
		unsigned int Read(
			amount__ Amount,
			void *Buffer )
		{
			if ( ( Amount = _read( FD_, Buffer, Amount ) ) == -1 )
				ERRd();

			return Amount;
		}
		int Write(
			const void *Buffer,
			amount__ Amount )
		{
			if ( ( Amount = _write( FD_, Buffer, Amount ) ) == -1 )
				ERRd();

			return Amount;
		}
		void Seek( long Offset )
		{
			if ( _lseek( FD_, Offset, SEEK_SET ) != Offset )
				ERRd();
		}
		void Flush( void )
		{
			if ( _commit( FD_ ) != 0 )
				ERRd();
		}
		bso::bool__ EOF( void )
		{
			switch( _eof( FD_ ) ) {
			case 1:
				return true;
				break;
			case 0:
				return false;
				break;
			default:
				ERRd();
				return 1;
				break;
			}
		}
	};

	class file_lowlevel_io___
	: public lowlevel_io__
	{
	private:
		int FD_;
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P ) {
				if ( FD_ != -1 )
					if ( _close( FD_ ) != 0 )
						ERRx();
			}

			FD_ = -1;
		}
		file_lowlevel_io___( void )
		: lowlevel_io__( FD_ )
		{
			reset( false );
		}
		~file_lowlevel_io___( void )
		{
			reset();
		}
		status__ Init(
			const char *FileName,
			mode__ Mode )
		{
			int Flags = _O_BINARY;

			reset();

			switch ( Mode ) {
			case mRemove:
				Flags |= _O_TRUNC | _O_CREAT |_O_RDWR;
				break;
			case mAppend:
				Flags |= _O_APPEND | _O_RDWR;
				break;
			case mReadWrite:
				Flags |= _O_RDWR;
				break;
			case mReadOnly:
				Flags |= _O_RDONLY;
				break;
			default:
				ERRu();
				break;
			}

			if ( ( FD_ = _open( FileName, Flags ) ) ==  -1 )
				return sFailure;
			else
				return sSuccess;
		}
	};
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
