/*
	Header for the 'pllio' library by Claude SIMON (csimon@epeios.org)
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

#ifndef PLLIO__INC
#define PLLIO__INC

#define PLLIO_NAME		"PLLIO"

#define	PLLIO_VERSION	"$Revision$"

#define PLLIO_OWNER		"Claude SIMON (csimon@epeios.org)"

#include "ttr.h"

extern class ttr_tutor &PLLIOTutor;

#if defined( XXX_DBG ) && !defined( PLLIO_NODBG )
#define PLLIO_DBG
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
//D POSIX Low-Level Input/Output 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "iodef.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#undef EOF

namespace pllio {

	using namespace iodef;

	typedef int	amount__;

	typedef int	descriptor__;

	class lowlevel_io__
	{
	private:
		descriptor__ D_;
	public:
		lowlevel_io__( descriptor__ D = -1 )
		{
			D_ = D;
		}
		unsigned int Read(
			amount__ Amount,
			void *Buffer )
		{
			if ( ( Amount = read( D_, Buffer, Amount ) ) == -1 )
				ERRd();

			return Amount;
		}
		int Write(
			const void *Buffer,
			amount__ Amount )
		{
			if ( Amount = write( D_, Buffer, Amount ) == -1 )
				ERRd();

			return Amount;
		}
		void Seek( long Offset )
		{
			if ( lseek( D_, Offset, SEEK_SET ) != Offset )
				ERRd();
		}
		void Flush( void )
		{
#ifdef CPE__CYGWIN
			fsync( D_ );
#else
			fdatasync( D_ );
#endif
		}
		bso::bool__ EOF( void )
		{
			struct stat Stat;
			off_t Position;

			if ( fstat( D_, &Stat ) != 0 )
				ERRd();

			if ( ( Position = lseek( D_, 0, SEEK_CUR ) ) == -1 )
				ERRd();

			return Position >= Stat.st_size;
		}
		void operator()( int D )
		{
			D_ = D;
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
					if ( close( FD_ ) != 0 )
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
			int Flags = O_BINARY;

			reset();

			switch ( Mode ) {
			case mRemove:
				Flags |= O_TRUNC | O_CREAT | O_RDWR;
				break;
			case mAppend:
				Flags |= O_APPEND | O_RDWR;
				break;
			case mReadWrite:
				Flags |= O_RDWR;
				break;
			case mReadOnly:
				Flags |= O_RDONLY;
				break;
			default:
				ERRu();
				break;
			}

			if ( ( FD_ = ( FileName, Flags ) ) ==  -1 )
				return sFailure;
			else {
				lowlevel_io__::operator()( FD_ );
				return sSuccess;
			}
		}
	};
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
