/*
	Header for the 'fil' library by Claude SIMON (csimon@epeios.org)
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

#ifndef FIL__INC
#define FIL__INC

#define FIL_NAME		"FIL"

#define	FIL_VERSION	"$Revision$"

#define FIL_OWNER		"Claude SIMON (csimon@epeios.org)"

#include "ttr.h"

extern class ttr_tutor &FILTutor;

#if defined( XXX_DBG ) && !defined( FIL_NODBG )
#define FIL_DBG
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
//D FILe 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

//D FILe. File management.

#include "cpe.h"

#ifdef FIL_USE_STANDARD_IO
#	define FIL__STANDARD_IO
#elif defined( FIL_USE_LOWLEVEL_IO )
#	define FIL__LOWLEVEL_IO
#else
#	if defined( CPE__MS ) || defined( CPE__UNIX )
#		define FIL__LOWLEVEL_IO
#	else
#		define FIL__STANDARD_IO
#	endif
#endif

#ifdef FIL__LOWLEVEL_IO
#	ifdef CPE__MS
#		define FIL__MS_LOWLEVEL_IO
#	elif ( defined( CPE__UNIX )
#		define FIL__UNIX_LOWLEVEL_IO
#	else
#		error "No low-level I/O available for this plateform"
#	endif
#endif

#include "err.h"

#ifdef FLM__UNIX_LOWLEVEL_IO
#	include <unistd.h>
#	include <fcntl.h>
#elif defined( FLM__MS_LOWLEVEL_IO )
#	include <io.h>
#	include <fcntl.h>
#	include <sys/stat.h>
#elif defined( FLM__STANDARD_IO )
#	include <stdio.h>
#endif

namespace fil
{
	//e Open mode.
	enum mode {
		//i Unknow.
		mUnknow,
		//i Ecrase le contenu du fichier.
		mRemove,
		//i Ajoute à la fin du fichier.
		mAppend,
		//i Amount of mode.
		m_amount
	};
 	//e Status.
	enum status {
		//i Unknow,
		sUnknow,
		//i Success.
		sSuccess,
		//i Failure.
		sFailure,
		//i Amount of status.
		s_amount
	};

	typedef flf::file_iflow__	_iflow__;

	//c A file as standard input flow.
	class file_iflow___
	: public _iflow__
	{
	private:
#ifdef FIL__STANDARD_IO
		FILE *File_;
#elif defined( FIL__MS_LOWLEVEL_IO ) ||defined( FIL__UNIX_LOWLEVEL_IO )
		int FD_;
#endif
	public:
		void reset( bool P = true )
		{
			_iflow__::reset( P );

			if ( P ) {
#ifdef FIL__STANDARD_IO
				if ( File_ != NULL )
					fclose( File_ );
#elif defined( FIL__MS_LOWLEVEL_IO )
				if ( FD_ != -1 )
					_close( FD_ );
#elif defined( FIL__UNIX_LOWLEVEL_IO )
				if ( FD_ != -1 )
					close( FD_ );
#endif
			}

#ifdef FIL__STANDARD_IO
			File_ = NULL;
#elif defined( FIL__MS_LOWLEVEL_IO ) ||defined( FIL__UNIX_LOWLEVEL_IO )
			FD_ = -1;
#endif
		}
		file_iflow___( void )
		: _iflow__( File_ )
		{
			reset( false );
		}
		virtual ~file_iflow___( void )
		{
			reset( true );
		}
		/*f Initialization with the file named 'FileName'. Correct the '/' or '\' if 'Handle' = 'hCorrect'.
		Return 'sFailure' if initialization failes, and 'ErrHandle' set to 'err::hUsual', 'sSuccess' otherwise. */
		status Init(
			const char *FileName,
			err::handle ErrHandle = err::hUsual );
	};

	typedef flf::file_oflow__	_oflow__;

	//c A file as standard output flow.
	class file_oflow___
	: public _oflow__
	{
	private:
	private:
#ifdef FIL__STANDARD_IO
		FILE *File_;
#elif defined( FIL__MS_LOWLEVEL_IO ) ||defined( FIL__UNIX_LOWLEVEL_IO )
		int FD_;
#endif
	public:
		void reset( bool P = true )
		{
			_iflow__::reset( P );

			if ( P ) {
#ifdef FIL__STANDARD_IO
				if ( File_ != NULL )
					fclose( File_ );
#elif defined( FIL__MS_LOWLEVEL_IO )
				if ( FD_ != -1 )
					_close( FD_ );
#elif defined( FIL__UNIX_LOWLEVEL_IO )
				if ( FD_ != -1 )
					close( FD_ );
#endif
			}

#ifdef FIL__STANDARD_IO
			File_ = NULL;
#elif defined( FIL__MS_LOWLEVEL_IO ) ||defined( FIL__UNIX_LOWLEVEL_IO )
			FD_ = -1;
#endif
		}
		file_oflow___( void )
		: _oflow__( File_ )
		{
			reset( false );
		}
		virtual ~file_oflow___( void )
		{
			reset( true );
		}
		/*f Initialization with the file named 'FileName'. If this file already exists, 'Mode' is was happen with it.
		If 'Handle' = 'hCorrect', corrects the '\' or '/' in file name.
		Return 'sFailure' if initialization failes, and 'ErrHandle' set to 'err::hUsual', 'sSuccess' otherwise. */
		status Init(
			const char *FileName,
			fil::mode Mode = fil::mRemove,
			err::handle ErrHandle = err::hUsual );
		/*f Initialization with the file named 'FileName'. If this file already exists, 'Mode' is was happen with it.
		If 'Handle' = 'hCorrect', corrects the '\' or '/' in file name.
		Return 'sFailure' if initialization failes, and 'ErrHandle' set to 'err::hUsual', 'sSuccess' otherwise. */
		status Init(
			const char *Nom,
			err::handle ErrHandle,
			mode Mode = fil::mRemove )
		{
			return Init( Nom, Mode, ErrHandle );
		}
	};
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
