/*
	Header for the 'fwf' library by Claude SIMON (csimon@epeios.org)
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

#ifndef FWF__INC
#define FWF__INC

#define FWF_NAME		"FWF"

#define	FWF_VERSION	"$Revision$"

#define FWF_OWNER		"Claude SIMON (csimon@epeios.org)"

#include "ttr.h"

extern class ttr_tutor &FWFTutor;

#if defined( XXX_DBG ) && !defined( FWF_NODBG )
#define FWF_DBG
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
//D FloW Functions 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "bso.h"

#ifdef CPE__T_MT
#	include "mtx.h"
#	define FWF_NO_MUTEX	MTX_INVALID_HANDLER
	typedef mtx::mutex_handler__ mutex__;
#else
	typedef void *mutex__;
#	define FWF_NO_MUTEX	NULL
#endif


namespace fwf {
		//t Amount of data.
	typedef bso::msize__		size__;

	//d The max value for a amount.
#	define FWF_SIZE_MAX			BSO_MSIZE_MAX

	//t Size (of a cache, for example).
	typedef bso::bsize__		bsize__;

	//d The max value for a size.
#	define FWF_BSIZE_MAX		BSO_BSIZE_MAX

	//t Type of a datum.
	typedef unsigned char		datum__;

#ifdef CPE__T_MT
	inline void Test_( mutex__ Mutex )
	{
		if ( Mutex == FWF_NO_MUTEX )
			ERRc();
	}
#endif

	inline mutex__ Create_( void )
	{
#ifdef CPE__T_MT
		return mtx::Create( mtx::mOwned );
#else
		static int A;
		return &A;	// Peur importe la valeur, pour peu qu'elle ne soit pas nulle.
#endif
	}


	inline void Delete_( mutex__ Mutex )
	{
#ifdef CPE__T_MT
		Test_( Mutex );
		mtx::Delete( Mutex );
#endif
	}

	inline void Lock_( mutex__ Mutex )
	{
#ifdef CPE__T_MT
		Test_( Mutex );
		mtx::Lock( Mutex );
#endif
	}

	inline void Unlock_( mutex__ Mutex )
	{
#ifdef CPE__T_MT
		Test_( Mutex );
		mtx::Unlock( Mutex );
#endif
	}

	inline bso::bool__ IsLocked_( mutex__ Mutex )
	{
#ifdef CPE__T_MT
		Test_( Mutex );
		return mtx::IsLocked( Mutex );
#endif
		return false;
	}

	inline bso::bool__ IsOwner_( mutex__ Mutex )
	{
#ifdef CPE__T_MT
		Test_( Mutex );
		return mtx::IsOwner( Mutex );
#endif
		return false;
	}

		//c Base input flow.
	class iflow_functions___
	{
	private:
		mutex__ _Mutex;	// Mutex pour protèger la ressource.
		void _Lock( void )
		{
			Lock_( _Mutex );
		}
		void _Unlock( void )
		{
			if ( IsLocked_( _Mutex ) ) {
#ifdef FWF_DBG
				if ( !IsOwner_( _Mutex ) )
					ERRu();
#endif
				Unlock_( _Mutex );
			}
		}
	protected:
		/* Place un minimum de 'Miimum' octets et jusqu'à 'Wanted' octets dans 'Buffer'. Retourne le nombre d'octets
		effectivement place dans 'Buffer'. Cette value peut être infèrieure à 'Minimum' uniquement si toutes les données 
		ont été lues du périphérique sous-jacent. */
		virtual bsize__ FWFRead(
			bsize__ Minimum,
			datum__ *Buffer,
			bsize__ Wanted ) = 0;
		virtual void FWFDismiss( void ) = 0;
	public:
		void reset( bso::bool__ P = true ) 
		{
			if ( P ) {
				if ( _Mutex != FWF_NO_MUTEX ) {
						if ( IsOwner_( _Mutex ) )
							Dismiss();
					Delete_( _Mutex );
				}
			}

			_Mutex = FWF_NO_MUTEX;
		}
		iflow_functions___( void )
		{
			reset( false );
		}
		virtual ~iflow_functions___( void )
		{
			reset();
		}
		void Init( void )
		{
			reset();

			_Mutex = Create_();
		}
		void Dismiss( void )
		{
			if ( _Mutex != FWF_NO_MUTEX ) {
				FWFDismiss();

				_Unlock();
			}
		}
		bsize__ Read(
			bsize__ Minimum,
			datum__ *Buffer,
			bsize__ Wanted )
		{
#ifdef FWF_DBG
			if ( Minimum < 1 )
				ERRu();
#endif
			_Lock();

			return FWFRead( Minimum, Buffer, Wanted );
		}
	};

	//c Basic output flow.
	class oflow_functions___
	{
	private:
		mutex__ _Mutex;	// Mutex pour protèger la ressource.
		void _Lock( void )
		{
			Lock_( _Mutex );
		}
		void _Unlock( void )
		{
			if ( IsLocked_( _Mutex ) ) {
#ifdef FWF_DBG
				if ( !IsOwner_( _Mutex ) )
					ERRu();
#endif
				Unlock_( _Mutex );
			}
		}
	protected:
		virtual bsize__ FWFWrite(
			const datum__ *Buffer,
			bsize__ Wanted,
			bsize__ Minimum ) = 0;
		virtual void FWFSynchronize( void ) = 0;
	public:
		void reset( bso::bool__ P = true ) 
		{
			if ( P ) {
				if ( _Mutex != FWF_NO_MUTEX ) {
					if ( IsOwner_( _Mutex ) )
						Synchronize();
					Delete_( _Mutex );
				}
			}

			_Mutex = FWF_NO_MUTEX;
		}
		oflow_functions___( void )
		{
			reset( false );
		}
		virtual ~oflow_functions___( void )
		{
			reset();
		}
		void Init( void )
		{
			reset();

			_Mutex = Create_();
		}
		void Synchronize( void )
		{
			if ( _Mutex != FWF_NO_MUTEX ) {
				FWFSynchronize();
				_Unlock();
			}
		}
		bsize__ Write(
			const datum__ *Buffer,
			bsize__ Wanted,
			bsize__ Minimum )
		{
			_Lock();
			return FWFWrite( Buffer, Wanted, Minimum );
		}
	};

	class ioflow_functions___
	: public iflow_functions___,
	  public oflow_functions___
	{
	public:
		void reset( bso::bool__ P = true )
		{
			iflow_functions___::reset( P );
			oflow_functions___::reset( P );
		}
		void Init( void )
		{
			iflow_functions___::Init();
			oflow_functions___::Init();;
		}
	};
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
