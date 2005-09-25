/*
	Header for the 'fil' library by Claude SIMON (csimon@epeios.org)
	Copyright (C) 2000-2001, 2004 Claude SIMON (csimon@epeios.org).

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

#include "iof.h"
#include "flw.h"
#include "txf.h"
#include "cio.h"
#include "tol.h"

//d The default backup file extension.
#define FIL_DEFAULT_BACKUP_FILE_EXTENSION	".bak"

#ifdef FIL_FLOW_BUFFER_SIZE
#	define FIL__FLOW_BUFFER_SIZE FIL_FLOW_BUFFER_SIZE
#else
#	define FIL__FLOW_BUFFER_SIZE	100
#endif

namespace fil
{
	enum mode__ {
		//i Unknow.
		mUnknow,
		//i Ecrase le contenu du fichier.
		mRemove,
		//i Ajoute à la fin du fichier.
		mAppend,
		//i Ouvre le fichier ne lecture/écriture.
		mReadWrite,
		//i Ouvre le fichier en lecture seule.
		mReadOnly,
		//i Amount of mode.
		m_amount
	};
 	//e Status.
	enum status__ {
		//i Failure.
		sFailure = 0,
		//i Success.
		sSuccess,
		//i Unknow,
		s_Unknow,
		//i Amount of status.
		s_amount
	};

	iof::descriptor__ _Open(
		const char *Nom,
		mode__ Mode );

	void _Close( iof::descriptor__ D );

	typedef flw::iflow__	_iflow__;
	typedef iof::io__		_io__;

	class file___
	: public _io__
	{
	private:
		iof::descriptor__ D_;
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P ) {
				if ( D_ != IOF_UNDEFINED_DESCRIPTOR )
					_Close( D_ );
			}

			D_ = IOF_UNDEFINED_DESCRIPTOR;
		}
		file___( void )
		{
			reset( false );
		}
		~file___( void )
		{
			reset();
		}
		status__ Init(
			const char *FileName,
			mode__ Mode,
			err::handle ErrHandle = err::hUsual )
		{
			reset();

			D_ = _Open( FileName, Mode );

			if ( D_ == IOF_UNDEFINED_DESCRIPTOR ) {
				switch ( ErrHandle ) {
				case err::hSkip:
					return sFailure;
					break;
				case err::hUsual:
					ERRd();
					break;
				default:
					ERRu();
					break;
				}
			}

			_io__::Init( D_ );

			return sSuccess;
		}
	};

	typedef iof::io_iflow__		_io_iflow__;

	//c A file as standard input flow.
	class file_iflow___
	: public _io_iflow__
	{
	private:
		iof::descriptor__ D_;
	public:
		void reset( bool P = true )
		{
			if ( P ) {
				if ( D_ != IOF_UNDEFINED_DESCRIPTOR )
					_Close( D_ );
			}

			_io_iflow__::reset( P );

			D_ = IOF_UNDEFINED_DESCRIPTOR;
		}
		file_iflow___( void )
		{
			reset( false );
		}
		virtual ~file_iflow___( void )
		{
			reset( true );
		}
		status__ Init(
			const char *FileName,
			mode__ Mode = mReadOnly,
			err::handle ErrHandle = err::hUsual )
		{
			reset();

			D_ = _Open( FileName, Mode );

			if ( D_ == IOF_UNDEFINED_DESCRIPTOR ) {
				switch ( ErrHandle ) {
				case err::hSkip:
					return sFailure;
					break;
				case err::hUsual:
					ERRd();
					break;
				default:
					ERRu();
					break;
				}
			}

			_io_iflow__::Init( D_, FLW_NO_MUTEX );

			return sSuccess;
		}
		status__ Init(
			const char *FileName,
			err::handle ErrHandle,
			mode__ Mode = mReadOnly )
		{
			return Init( FileName, Mode, ErrHandle );
		}
	};

	typedef iof::io_oflow__		_io_oflow__;

	//c A file as standard input flow.
	class file_oflow___
	: public _io_oflow__
	{
	private:
		iof::descriptor__ D_;
	public:
		void reset( bool P = true )
		{
			if ( P ) {
				if ( D_ != IOF_UNDEFINED_DESCRIPTOR ) {
					_io_oflow__::Synchronize();
					_Close( D_ );
				}
			}

			_io_oflow__::reset( P );

			D_ = IOF_UNDEFINED_DESCRIPTOR;
		}
		file_oflow___( void )
		{
			reset( false );
		}
		virtual ~file_oflow___( void )
		{
			reset( true );
		}
		status__ Init(
			const char *FileName,
			mode__ Mode = mRemove,
			err::handle ErrHandle = err::hUsual )
		{
			reset();

			D_ = _Open( FileName, Mode );

			if ( D_ == IOF_UNDEFINED_DESCRIPTOR ) {
				switch ( ErrHandle ) {
				case err::hSkip:
					return sFailure;
					break;
				case err::hUsual:
					ERRd();
					break;
				default:
					ERRu();
					break;
				}
			}

			io_oflow__::Init( D_, FLW_NO_MUTEX );

			return sSuccess;
		}
		status__ Init(
			const char *FileName,
			err::handle ErrHandle,
			mode__ Mode = mRemove )
		{
			return Init( FileName, Mode, ErrHandle );
		}
	};
	//e Error code which can occurs during backup file operation.
	enum rbf
	{
		//i No error.
		rbfOK,
		//i error by renaming.
		rbfRenaming,
		//i Error by duplication. Occurs only with 'TOLCreateBackupFile()'.
		rbfDuplication,
		//i Error by suppression. 
		rbfSuppression,
		//i Erreur by allocation. Occurs only with 'TOLRecoverBackupFile()'.
		rbfAllocation
	};
	//e How handle the backuped file.
	enum hbf
	{
		//i Rename it.
		hbfRename,
		//i Duplicate it.
		hbfDuplicate
	};


	/*f Make a backup file from the file 'File', if exist, in adding 'Extension'.
	If 'Handle' == 'tol::hbfDuplicate', the backup file is create by duplicating the original one.
	If 'Handle' == 'tol::hbfRename', the bachup file is create by renaming the original one. */
	rbf CreateBackupFile(
		const char *Name,
		hbf Handle,
#if defined( CPE__CONSOLE ) && !defined( CPE__MT )
		txf::text_oflow__ &Flow = cio::cerr,
#else
		txf::text_oflow__ &Flow,
#endif
		const char *Extension = FIL_DEFAULT_BACKUP_FILE_EXTENSION,
		err::handle = err::hUsual  );

	//f Recover the backup file 'Name' with 'Extension' as extension.
	rbf RecoverBackupFile(
		const char *Name,
#if defined( CPE__CONSOLE ) && !defined( CPE__MT )
		txf::text_oflow__ &Flow = cio::cerr,
#else
		txf::text_oflow__ &Flow,
#endif
		const char *Extension = FIL_DEFAULT_BACKUP_FILE_EXTENSION,
		err::handle = err::hUsual  );
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
