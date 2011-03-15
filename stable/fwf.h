/*
	Header for the 'fwf' library by Claude SIMON (http://zeusw.org/intl/contact.html)
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

#ifndef FWF__INC
#define FWF__INC

#define FWF_NAME		"FWF"

#define	FWF_VERSION	"$Revision$"

#define FWF_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"

#include "ttr.h"

extern class ttr_tutor &FWFTutor;

#if defined( XXX_DBG ) && !defined( FWF_NODBG )
#define FWF_DBG
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
//D FloW Functions 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "bso.h"

#include <string.h>

#ifdef FWF_THREAD_SAFE
#	define FWF__TS
#elif !defined( FWF_THREAD_UNSAFE )
#	ifdef CPE__T_MT
#		define FWF__TS
#	endif
#endif

#ifdef FWF__TS
#	include "mtx.h"
#	define FWF_NO_MUTEX	MTX_INVALID_HANDLER
	typedef mtx::mutex_handler__ mutex__;
#else
	typedef void *mutex__;
#	define FWF_NO_MUTEX	NULL
#endif

#ifdef FWF_DEFAULT_CACHE_SIZE
#	define FWF__DEFAULT_CACHE_SIZE	FWF_DEFAULT_CACHE_SIZE
#else
#	define FWF__DEFAULT_CACHE_SIZE	1024
#endif



namespace fwf {
		//t Amount of data.
	using bso::size__;

	extern mutex__ _FakeMutex;	// Sert de leurre lorsqu'on n'a pas besoin que d'un objet 'thread-safe'.

	enum thread_safety__
	{
		tsEnabled,
		tsDisabled,
		ts_amount,
		ts_Undefined,
		ts_Default =
#ifdef FWF__TS
		tsEnabled
#else
		tsDisabled
#endif
	};

	//d The max value for a amount.
#	define FWF_SIZE_MAX			BSO_SIZE_MAX

	//t Type of a datum.
	typedef unsigned char		datum__;

#ifdef FWF__TS
	inline void Test_( mutex__ Mutex )
	{
		if ( Mutex == FWF_NO_MUTEX )
			ERRc();
	}
#endif

	inline mutex__ Create_( thread_safety__ ThreadSafety )
	{
		switch ( ThreadSafety ) {
		case tsEnabled:
#ifdef FWF__TS
			return mtx::Create( mtx::mOwned );
#else
			ERRu();
#endif
			break;
		case tsDisabled:
			return _FakeMutex;
			break;
		default:
			ERRu();
			break;
		}

		return FWF_NO_MUTEX;	// Pour �viter un 'warning'.
	}


	inline void Delete_( mutex__ Mutex )
	{
#ifdef FWF__TS
		if ( Mutex != FWF_NO_MUTEX )
			if ( Mutex != _FakeMutex )
				mtx::Delete( Mutex );
#endif
	}

	inline void Lock_( mutex__ Mutex )
	{
#ifdef FWF__TS
		if ( Mutex != FWF_NO_MUTEX )
			if ( Mutex != _FakeMutex )
				mtx::Lock( Mutex );
#endif
	}

	inline void Unlock_( mutex__ Mutex )
	{
#ifdef FWF__TS
		if ( Mutex != FWF_NO_MUTEX )
			if ( Mutex != _FakeMutex )
				mtx::Unlock( Mutex );
	#endif
	}

	inline bso::bool__ IsLocked_( mutex__ Mutex )
	{
#ifdef FWF__TS
		if ( Mutex != FWF_NO_MUTEX )
			if ( Mutex != _FakeMutex )
				return mtx::IsLocked( Mutex );
#endif
		return false;
	}

	inline bso::bool__ IsOwner_( mutex__ Mutex )
	{
#ifdef FWF__TS
		if ( Mutex != FWF_NO_MUTEX )
			if ( Mutex != _FakeMutex )
				return mtx::IsOwner( Mutex );
#endif
		return true;
	}

	class iflow_functions_base___
	{
	private:
		mutex__ _Mutex;	// Mutex pour prot�ger la ressource.
		datum__ *_Cache;
		size__ _Size;
		size__ _Available;
		size__ _Position;
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
		void _FillCache( void )
		{
#ifdef FWF_DBG
			if ( ( _Cache == NULL ) || ( _Size == 0 ) )
				ERRu();

			if ( _Available != 0 )
				ERRc();
#endif
			_Position = 1;
			_Available = FWFRead( _Size - 1, _Cache + 1 );	// On laisse un octet de libre au d�but pour un �ventuel 'Unget(...)'.
		}
	protected:
		// Retourne le nombre d'octets effectivement lus. Ne retourne '0' que si plus aucune donn�e n'est disponibe.
		virtual size__ FWFRead(
			size__ Maximum,
			datum__ *Buffer ) = 0;
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

			_Cache = NULL;
			_Size = _Available = _Position = 0;
			_Mutex = FWF_NO_MUTEX;
		}
		iflow_functions_base___( void )
		{
			reset( false );
		}
		virtual ~iflow_functions_base___( void )
		{
			reset();
		}
		void Init(
			datum__ *Cache,
			size__ Size,
			thread_safety__ ThreadSafety )
		{
			reset();

			_Cache = Cache;
			_Size = Size;

			_Available = _Position = 0;
			_Mutex = Create_( ThreadSafety );
		}
		void Dismiss( void )
		{
			if ( _Mutex != FWF_NO_MUTEX ) {
				FWFDismiss();

				_Unlock();
			}
		}
		size__ Read(
			size__ Maximum,
			datum__ *Buffer,
			bso::bool__ AdjustCache,
			bso::bool__ &CacheIsEmpty )
		{
#ifdef FWF_DBG
			if ( Maximum < 1 )
				ERRu();
#endif
			_Lock();

			if ( _Available == 0 )
				_FillCache();

			if ( Maximum > _Available )
				Maximum = _Available;

			memcpy( Buffer, _Cache + _Position, Maximum );

			if ( AdjustCache ) {
				_Available -= Maximum;
				_Position += Maximum;
			}

			CacheIsEmpty = _Available == 0;

			return Maximum;
		}
		void Unget( datum__ Datum )
		{
			_Lock();

			if ( _Available == 0 )
				_Position = 1;

			if ( _Position == 0 )
				ERRu();	// Appeler 'Unget(...)' deux fois de suite (ou seulement avec des 'View(...)' entre) n'est pas conseill�.

			_Cache[--_Position] = Datum;
			_Available++;
		}
	};

	template <int cache_size = FWF__DEFAULT_CACHE_SIZE> class iflow_functions___
	: public iflow_functions_base___
	{
	private:
		datum__ _Cache[cache_size];
	public:
		void Init( thread_safety__ ThreadSafety )
		{
			iflow_functions_base___::Init( _Cache, sizeof( _Cache ), ThreadSafety );
		}
	};

	class oflow_functions_base___
	{
	private:
		mutex__ _Mutex;	// Mutex pour prot�ger la ressource.
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
		// Retourne le nombre d'octets effectivement �crits. Ne retourne '0' que si plus aucune donn�e ne peut �tre �crite.
		virtual size__ FWFWrite(
			const datum__ *Buffer,
			size__ MAximum ) = 0;
		virtual void FWFCommit( void ) = 0;
	public:
		void reset( bso::bool__ P = true ) 
		{
			if ( P ) {
				if ( _Mutex != FWF_NO_MUTEX ) {
					if ( IsOwner_( _Mutex ) )
						Commit();
					Delete_( _Mutex );
				}
			}

			_Mutex = FWF_NO_MUTEX;
		}
		oflow_functions_base___( void )
		{
			reset( false );
		}
		virtual ~oflow_functions_base___( void )
		{
			reset();
		}
		void Init( thread_safety__ ThreadSafety )
		{
			reset();

			_Mutex = Create_( ThreadSafety );
		}
		void Commit( void )
		{
			FWFCommit();
			_Unlock();
		}
		size__ Write(
			const datum__ *Buffer,
			size__ Maximum )
		{
			_Lock();
			return FWFWrite( Buffer, Maximum );
		}
	};

	template <int = 0> class oflow_functions___
	: public oflow_functions_base___
	{};

	class ioflow_functions_base___
	: public iflow_functions_base___,
	  public oflow_functions_base___
	{
	public:
		void reset( bso::bool__ P = true )
		{
			iflow_functions_base___::reset( P );
			oflow_functions_base___::reset( P );
		}
		void Init(
			datum__ *InputCache,
			size__ InputCacheSize,
			thread_safety__ ThreadSafety )
		{
			iflow_functions_base___::Init( InputCache, InputCacheSize, ThreadSafety );
			oflow_functions_base___::Init( ThreadSafety );
		}
	};

	template <int input_cache_size = FWF__DEFAULT_CACHE_SIZE> class ioflow_functions___
	: public ioflow_functions_base___
	{
	private:
		datum__ _InputCache[input_cache_size];
	public:
		void Init( thread_safety__ ThreadSafety )
		{
			ioflow_functions_base___::Init( _InputCache, sizeof( _InputCache ), ThreadSafety );
		}
	};
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
