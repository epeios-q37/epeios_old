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
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

namespace pllio {
	typedef int	amount__;

	typedef int	descriptor__;

#define	PLLIO_UNDEFINED_DESCRIPTOR	-1

	class io_core__
	{
	protected:
		descriptor__ D_;
		void _Test( void ) const
		{
#ifdef PLLIO_DBG
			if ( D_ == PLLIO_UNDEFINED_DESCRIPTOR )
				ERRu();
#endif
		}
	public:
		io_core__( descriptor__ D )
		{
			D_ = D;
		}
		void Seek( long Offset )
		{
			_Test();

			if ( lseek( D_, Offset, SEEK_SET ) != Offset )
				ERRd();
		}
		void operator()( descriptor__ D )
		{
			D_ = D;
		}
	};

	class lowlevel_input__
	: public virtual io_core__
	{
	public:
		lowlevel_input__( descriptor__ D )
		: io_core__( D )
		{}
		unsigned int Read(
			amount__ Amount,
			void *Buffer )
		{
			_Test();

			if ( Amount > SSIZE_MAX )
				Amount = SSIZE_MAX;

			if ( ( Amount = read( D_, Buffer, Amount ) ) == -1 )
				ERRd();

			return Amount;
		}
		bso::bool__ OnEOF( void )
		{
			struct stat Stat;
			off_t Position;

			_Test();

			if ( fstat( D_, &Stat ) != 0 )
				ERRd();

			if ( ( Position = lseek( D_, 0, SEEK_CUR ) ) == -1 )
				ERRd();

			return Position >= Stat.st_size;
		}
	};

	class lowlevel_output__
	: public virtual io_core__
	{
	public:
		lowlevel_output__( descriptor__ D )
		: io_core__( D )
		{}
		int Write(
			const void *Buffer,
			amount__ Amount )
		{
			_Test();

			if ( Amount > SSIZE_MAX )
				Amount = SSIZE_MAX;

			if ( ( Amount = write( D_, Buffer, Amount ) ) == -1 )
				ERRd();

			return Amount;
		}
		void Flush( void )
		{
			_Test();
/*
#ifdef CPE__CYGWIN
			fsync( D_ );
#else
			fdatasync( D_ );
#endif
			*/
		}
	};

	class lowlevel_io__
	: public lowlevel_output__,
	  public lowlevel_input__
	{
	public:
		lowlevel_io__( descriptor__ D = PLLIO_UNDEFINED_DESCRIPTOR )
		: lowlevel_output__( D ),
		  lowlevel_input__( D ),
		  io_core__( D )
		{}
	};
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
