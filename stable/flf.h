/*
	Header for the 'flf' library by Claude SIMON (csimon@epeios.org)
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

#ifndef FLF__INC
#define FLF__INC

#define FLF_NAME		"FLF"

#define	FLF_VERSION	"$Revision$"

#define FLF_OWNER		"Claude SIMON (csimon@epeios.org)"

#include "ttr.h"

extern class ttr_tutor &FLFTutor;

#if defined( XXX_DBG ) && !defined( FLF_NODBG )
#define FLF_DBG
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
//D FLow Functions 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "cpe.h"
#include "err.h"
#include "bso.h"
#include "fil.h"
#include "iof.h"

namespace flf {
	class file_iflow_functions___
	: public iof::io_iflow_functions___
	{
	private:
		iop::descriptor__ _D;
	public:
		void reset( bso::bool__ P = true )
		{
			io_iflow_functions___::reset( P );

			if ( P ) {
				if ( _D != IOP_UNDEFINED_DESCRIPTOR )
					fil::Close( _D );
			}

			_D = IOP_UNDEFINED_DESCRIPTOR;
		}
		file_iflow_functions___( void )
		{
			reset( false );
		}
		~file_iflow_functions___( void )
		{
			reset();
		}
		fil::status__ Init(
			const char *FileName,
			fil::mode__ Mode = fil::mReadOnly,
			err::handle ErrHandling = err::hUsual )
		{
			reset();

			fil::status__ Status = fil::s_Undefined;
			_D = fil::Open( FileName, Mode );

			if ( _D == IOP_UNDEFINED_DESCRIPTOR ) {
				Status = fil::sFailure;
				switch ( ErrHandling ) {
				case err::hSkip:
					break;
				case err::hUsual:
					ERRf();
					break;
				default:
					ERRu();
					break;
				}
			} else {
				Status = fil::sSuccess;
				io_iflow_functions___::Init( _D );
			}

			return Status;
		}
		fil::status__ Init(
			const char *FileName,
			err::handle ErrHandle,
			fil::mode__ Mode = fil::mReadOnly )
		{
			return Init( FileName, Mode, ErrHandle );
		}
	};

	typedef iof::io_iflow___		_io_iflow___;

	//c A file as standard input flow.
	class file_iflow___
	: public _io_iflow___
	{
	private:
		iop::descriptor__ D_;
	public:
		void reset( bool P = true )
		{
			if ( P ) {
				if ( D_ != IOP_UNDEFINED_DESCRIPTOR )
					fil::Close( D_ );
			}

			_io_iflow___::reset( P );

			D_ = IOP_UNDEFINED_DESCRIPTOR;
		}
		file_iflow___( flw::size__ AmountMax = FLW_SIZE_MAX )
		: _io_iflow___( AmountMax )
		{
			reset( false );
		}
		virtual ~file_iflow___( void )
		{
			reset( true );
		}
		fil::status__ Init(
			const char *FileName,
			fil::mode__ Mode = fil::mReadOnly,
			err::handle ErrHandle = err::hUsual )
		{
			reset();

			D_ = fil::Open( FileName, Mode );

			if ( D_ == IOP_UNDEFINED_DESCRIPTOR ) {
				switch ( ErrHandle ) {
				case err::hSkip:
					return fil::sFailure;
					break;
				case err::hUsual:
					ERRd();
					break;
				default:
					ERRu();
					break;
				}
			}

			_io_iflow___::Init( D_ );

			return fil::sSuccess;
		}
		fil::status__ Init(
			const char *FileName,
			err::handle ErrHandle,
			fil::mode__ Mode = fil::mReadOnly )
		{
			return Init( FileName, Mode, ErrHandle );
		}
	};

	class file_oflow_functions___
	: public iof::io_oflow_functions___
	{
	private:
		iop::descriptor__ _D;
	public:
		void reset( bso::bool__ P = true )
		{
			io_oflow_functions___::reset( P );

			if ( P ) {
				if ( _D != IOP_UNDEFINED_DESCRIPTOR )
					fil::Close( _D );
			}

			_D = IOP_UNDEFINED_DESCRIPTOR;
		}
		file_oflow_functions___( void )
		{
			reset( false );
		}
		~file_oflow_functions___( void )
		{
			reset();
		}
		fil::status__ Init(
			const char *FileName,
			fil::mode__ Mode = fil::mRemove,
			err::handle ErrHandling = err::hUsual )
		{
			reset();

			fil::status__ Status = fil::s_Undefined;
			_D = fil::Open( FileName, Mode );

			if ( _D == IOP_UNDEFINED_DESCRIPTOR ) {
				Status = fil::sFailure;
				switch ( ErrHandling ) {
				case err::hSkip:
					break;
				case err::hUsual:
					ERRf();
					break;
				default:
					ERRu();
					break;
				}
			} else {
				Status = fil::sSuccess;
				io_oflow_functions___::Init( _D );
			}

			return Status;
		}
		fil::status__ Init(
			const char *FileName,
			err::handle ErrHandle,
			fil::mode__ Mode = fil::mRemove )
		{
			return Init( FileName, Mode, ErrHandle );
		}
	};

	typedef iof::io_oflow___		_io_oflow___;

	//c A file as standard input flow.
	class file_oflow___
	: public _io_oflow___
	{
	private:
		iop::descriptor__ D_;
	public:
		void reset( bool P = true )
		{
			_io_oflow___::reset( P );

			if ( P ) {
				if ( D_ != IOP_UNDEFINED_DESCRIPTOR ) {
					fil::Close( D_ );
				}
			}

			D_ = IOP_UNDEFINED_DESCRIPTOR;
		}
		file_oflow___( flw::size__ AmountMax = FLW_SIZE_MAX )
		: _io_oflow___( AmountMax )
		{
			reset( false );
		}
		virtual ~file_oflow___( void )
		{
			reset( true );
		}
		fil::status__ Init(
			const char *FileName,
			fil::mode__ Mode = fil::mRemove,
			err::handle ErrHandle = err::hUsual )
		{
			reset();

			D_ = fil::Open( FileName, Mode );

			if ( D_ == IOP_UNDEFINED_DESCRIPTOR ) {
				switch ( ErrHandle ) {
				case err::hSkip:
					return fil::sFailure;
					break;
				case err::hUsual:
					ERRd();
					break;
				default:
					ERRu();
					break;
				}
			}

			io_oflow___::Init( D_ );

			return fil::sSuccess;
		}
		fil::status__ Init(
			const char *FileName,
			err::handle ErrHandle,
			fil::mode__ Mode = fil::mRemove )
		{
			return Init( FileName, Mode, ErrHandle );
		}
	};
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
