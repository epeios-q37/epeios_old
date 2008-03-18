/*
	Header for the 'wllio' library by Claude SIMON (csimon@epeios.org)
	Copyright (C) 2004 Claude SIMON (csimon@epeios.org).

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

#include "cpe.h"
#include "err.h"
#include "bso.h"
#ifdef CPE__CYGWIN
#	include <mingw/io.h>
#else
#	include <io.h>
#endif
#include <fcntl.h>

namespace wllio {

	typedef int amount__;

	typedef int descriptor__;

#define WLLIO_UNDEFINED_DESCRIPTOR	-1

	class io_core__
	{
	protected:
		descriptor__ _D;
		void _Test( void ) const
		{
#ifdef WLLIO_DBG
			if ( _D == WLLIO_UNDEFINED_DESCRIPTOR )
				ERRu();
#endif
		}
	public:
		void reset( bso::bool__ = true )
		{
			_D = WLLIO_UNDEFINED_DESCRIPTOR;
		}
		io_core__( void )
		{
			reset( false );
		}
		virtual ~io_core__( void )
		{
			reset();
		}
		void Seek( long Offset )
		{
			_Test();

			if ( _lseek( _D, Offset, SEEK_SET ) != Offset )
				ERRd();
		}
		long Size( void )
		{
			long Size;

			_Test();

			if ( ( Size = _lseek( _D, 0, SEEK_END ) ) == -1 )
				ERRd();

			return Size;
		}
		void Init( descriptor__ D )
		{
			_D = D;
		}
		bso::bool__ IsSTDIN( void ) const
		{
			return _fileno( stdin ) == _D;
		}
	};

	class lowlevel_input__
	: public virtual io_core__
	{
	public:
		void reset( bso::bool__ P = true )
		{
			io_core__::reset( P );
		}
		lowlevel_input__( void )
		{
			reset( false );
		}
		~lowlevel_input__( void )
		{
			reset();
		}
		void Init( descriptor__ D )
		{
			io_core__::Init( D );
		}
		unsigned int Read(
			amount__ Amount,
			void *Buffer )
		{
			_Test();

			if ( ( Amount = _read( _D, Buffer, Amount ) ) == -1 )
				ERRd();

			return Amount;
		}
		bso::bool__ OnEOF( void )
		{
			_Test();

			if ( IsSTDIN() ) {
				return feof( stdin ) != 0;
			} else 
				switch( _eof( _D ) ) {
				case 1:
					return true;
					break;
				case 0:
					return false;
					break;
				default:
	#ifdef WLLIO_DBG
					int Error = errno;	// Pour le d�bogage.
	#endif
					if ( _D == _fileno( stdin ) )	// Si 'stdin'.
						return false;
					else
						ERRd();
					return true;	// To avoid a warning.
					break;
				}
		}
	};

	class lowlevel_output__
	: public virtual io_core__
	{
	public:
		void reset( bso::bool__ P = true )
		{
			io_core__::reset( P );
		}
		lowlevel_output__( void )
		{
			reset( false );
		}
		~lowlevel_output__( void )
		{
			reset();
		}
		void Init( descriptor__ D )
		{
			io_core__::Init( D );
		}
		int Write(
			const void *Buffer,
			amount__ Amount )
		{
			if ( ( Amount = _write( _D, Buffer, Amount ) ) == -1 )
				ERRd();

			return Amount;
		}
		void Flush( void )
		{
			if ( _D == _fileno( stdin ) )
				ERRu();
			else if ( _D == WLLIO_UNDEFINED_DESCRIPTOR )
				ERRu();
			else if ( ( _D != _fileno( stdout ) )
				      && ( _D != _fileno( stderr ) ) )
				if ( _commit( _D ) != 0 )
					ERRd();
		}
	};

	class lowlevel_io__
	: public lowlevel_output__,
	  public lowlevel_input__
	{
	public:
		void reset( bso::bool__ P = true )
		{
			lowlevel_output__::reset( P );
			lowlevel_input__::reset( P );
		}
		lowlevel_io__( void )
		{
			reset( false );
		}
		~lowlevel_io__( void )
		{
			reset();
		}
		void Init( descriptor__ D )
		{
			lowlevel_output__::Init( D );
			lowlevel_input__::Init( D );
		}
	};
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
