/*
	Header for the 'fdr' library by Claude SIMON (csimon at zeusw dot org)
	Copyright (C) , 2004 Claude SIMON.

	This file is part of the Epeios (http://zeusw.org/epeios/) project.

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

#ifndef FDR__INC
#define FDR__INC

#define FDR_NAME		"FDR"

#define	FDR_VERSION	"$Revision$"

#define FDR_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &FDRTutor;

#if defined( XXX_DBG ) && !defined( FDR_NODBG )
#define FDR_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision$
//C Claude SIMON (csimon at zeusw dot org)
//R $Date$

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D Flow DRiver 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "bso.h"
#include "strng.h"

#ifdef FDR_THREAD_SAFE
#	define FDR__TS
#elif !defined( FDR_THREAD_UNSAFE )
#	ifdef CPE__MT
#		define FDR__TS
#	endif
#endif

#ifdef FDR__TS
#	include "mtx.h"
#	define FDR_NO_MUTEX	MTX_INVALID_HANDLER
	typedef mtx::mutex_handler__ mutex__;
#else
	typedef void *mutex__;
#	define FDR_NO_MUTEX	NULL
#endif

#ifdef FDR_DEFAULT_CACHE_SIZE
#	define FDR__DEFAULT_CACHE_SIZE	FDR_DEFAULT_CACHE_SIZE
#else
#	define FDR__DEFAULT_CACHE_SIZE	1024
#endif

# define FDR_SIZE_MAX	BSO_SIZE_MAX


namespace fdr {
		//t Amount of data.
	using bso::size__;

	enum thread_safety__
	{
		tsEnabled,
		tsDisabled,
		ts_amount,
		ts_Undefined,
		ts_Default =
#ifdef FDR__TS
		tsEnabled
#else
		tsDisabled
#endif
	};

	//d The max value for a amount.
#	define FDR_SIZE_MAX			BSO_SIZE_MAX

	//t Type of a datum.
	typedef unsigned char		datum__;

#ifdef FDR__TS
	inline void Test_( mutex__ Mutex )
	{
		if ( Mutex == FDR_NO_MUTEX )
			ERRc();
	}
#endif

	inline mutex__ Create_( thread_safety__ ThreadSafety )
	{
		switch ( ThreadSafety ) {
		case tsEnabled:
#ifdef FDR__TS
			return mtx::Create( mtx::mProtecting );
#else
			ERRc();
#endif
			break;
		case tsDisabled:
			return FDR_NO_MUTEX;
			break;
		default:
			ERRc();
			break;
		}

		return FDR_NO_MUTEX;	// Pour éviter un 'warning'.
	}


	inline void Delete_( mutex__ Mutex )
	{
#ifdef FDR__TS
		if ( Mutex != FDR_NO_MUTEX )
			mtx::Delete( Mutex );
#endif
	}

	inline void Lock_( mutex__ Mutex )
	{
#ifdef FDR__TS
		if ( Mutex != FDR_NO_MUTEX )
			mtx::Lock( Mutex );
#endif
	}

	inline void Unlock_( mutex__ Mutex )
	{
#ifdef FDR__TS
		if ( Mutex != FDR_NO_MUTEX )
			mtx::Unlock( Mutex );
	#endif
	}

	inline bso::bool__ IsLocked_( mutex__ Mutex )
	{
#ifdef FDR__TS
		if ( Mutex != FDR_NO_MUTEX )
			return mtx::IsLocked( Mutex );
#endif
		return false;
	}

	inline bso::bool__ IsOwner_( mutex__ Mutex )
	{
#ifdef FDR__TS
		if ( Mutex != FDR_NO_MUTEX )
			return mtx::IsOwner( Mutex );
#endif
		return true;
	}

	class iflow_driver_base___
	{
	private:
		mutex__ _Mutex;	// Mutex pour protèger la ressource.
		datum__ *_Cache;
		size__ _Size;	// Si == '0', signale fin de 'flow' atteint.
		size__ _Available;
		size__ _Position;
		void _Lock( void )
		{
			Lock_( _Mutex );
		}
		void _Unlock( void )
		{
			if ( IsLocked_( _Mutex ) ) {
#ifdef FDR_DBG
				if ( !IsOwner_( _Mutex ) )
					ERRc();
#endif
				Unlock_( _Mutex );
			}
		}
		void _FillCache( void )
		{
#ifdef FDR_DBG
			if ( _Cache == NULL )
				ERRc();

			if ( _Available != 0 )
				ERRc();
#endif
			if ( _Size != 0 ) {
				_Position = 1;
				_Available = FDRRead( _Size - 1, _Cache + 1 );	// On laisse un octet de libre au début pour un éventuel 'Unget(...)'.

				if ( _Available == 0 )
					_Size = 0;	// Pour signaler que la fin de 'flow' est atteinte.
			}
		}
	protected:
		// Retourne le nombre d'octets effectivement lus. Ne retourne '0' que si plus aucune donnée n'est disponibe.
		virtual size__ FDRRead(
			size__ Maximum,
			datum__ *Buffer ) = 0;
		virtual void FDRDismiss( void ) = 0;
	public:
		void reset( bso::bool__ P = true ) 
		{
			if ( P ) {
				Dismiss();
				Delete_( _Mutex );
			}

			_Cache = NULL;
			_Size = _Available = _Position = 0;
			_Mutex = FDR_NO_MUTEX;
		}
		iflow_driver_base___( void )
		{
			reset( false );
		}
		virtual ~iflow_driver_base___( void )
		{
			reset();
		}
		void Init(
			datum__ *Cache,
			size__ Size,
			thread_safety__ ThreadSafety )
		{
			reset();
#ifdef FDR_DBG
			if ( Cache == NULL )
				ERRc();
#endif

			_Cache = Cache;
			_Size = Size;

			_Available = _Position = 0;
			_Mutex = Create_( ThreadSafety );
		}
		void Dismiss( void )
		{
			if ( _Cache != NULL ) {
				FDRDismiss();
				_Unlock();
			}
		}
		size__ Read(
			size__ Maximum,
			datum__ *Buffer,
			bso::bool__ AdjustCache,
			bso::bool__ &CacheIsEmpty )
		{
#ifdef FDR_DBG
			if ( Maximum < 1 )
				ERRc();
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
				ERRc();	// Appeler 'Unget(...)' deux fois de suite (ou seulement avec des 'View(...)' entre) n'est pas conseillé.

			_Cache[--_Position] = Datum;
			_Available++;
		}
		bso::bool__ IsLocked( void )
		{
			return IsLocked_( _Mutex );
		}
		bso::bool__ IFlowIsLocked( void )	// Simplifie l'utilisation de 'ioflow_driver_...'
		{
			return IsLocked();
		}
		size__ Available( void ) const	// Retourne la quantité de donnéées disponible dans le cache.
		{
			return _Available;
		}
	};

	template <int cache_size = FDR__DEFAULT_CACHE_SIZE> class iflow_driver___
	: public iflow_driver_base___
	{
	private:
		datum__ _Cache[cache_size+1];	// '+1' pour gèrer le 'Unget()'.
	public:
		void reset( bso::bool__ P = true )
		{
			iflow_driver_base___::reset( P );
		}
		iflow_driver___( void )
		{
			reset( false );
		}
		~iflow_driver___( void )
		{
			reset();
		}
		void Init( thread_safety__ ThreadSafety )
		{
			iflow_driver_base___::Init( _Cache, sizeof( _Cache ), ThreadSafety );
		}
	};

	class oflow_driver_base___
	{
	private:
		mutex__ _Mutex;	// Mutex pour protèger la ressource.
		bso::bool__ _Initialized;	// Pour éviter des 'pure virtual function call'.
		void _Lock( void )
		{
			Lock_( _Mutex );
		}
		void _Unlock( void )
		{
			if ( IsLocked_( _Mutex ) ) {
#ifdef FDR_DBG
				if ( !IsOwner_( _Mutex ) )
					ERRc();
#endif
				Unlock_( _Mutex );
			}
		}
	protected:
		// Retourne le nombre d'octets effectivement écrits. Ne retourne '0' que si plus aucune donnée ne peut être écrite.
		virtual size__ FDRWrite(
			const datum__ *Buffer,
			size__ MAximum ) = 0;
		virtual void FDRCommit( void ) = 0;
	public:
		void reset( bso::bool__ P = true ) 
		{
			if ( P ) {
				Commit();
				Delete_( _Mutex );
			}

			_Mutex = FDR_NO_MUTEX;
			_Initialized = false;
		}
		oflow_driver_base___( void )
		{
			reset( false );
		}
		virtual ~oflow_driver_base___( void )
		{
			reset();
		}
		void Init( thread_safety__ ThreadSafety )
		{
			reset();

			_Mutex = Create_( ThreadSafety );
			_Initialized = true;
		}
		void Commit( void )
		{
			if ( _Initialized ) {
				FDRCommit();
				_Unlock();
			}
		}
		size__ Write(
			const datum__ *Buffer,
			size__ Maximum )
		{
			_Lock();
			return FDRWrite( Buffer, Maximum );
		}
		bso::bool__ IsLocked( void )
		{
			return IsLocked_( _Mutex );
		}
		bso::bool__ OFlowIsLocked( void )	// Simplifie l'utilisation de 'ioflow_driver_...'
		{
			return IsLocked();
		}
	};

	// Uniquement pour avoir une symétrie par rapport à 'iflow_driver___'.
	template <int Dummy = 0> class oflow_driver___
	: public oflow_driver_base___
	{
	public:
		void reset( bso::bool__ P = true )
		{
			if ( Dummy != 0 )	
				ERRc();	// 'Dummy' n'étant pas utilisé, rien ne sert de modifier sa valeur.

			oflow_driver_base___::reset( P );
		}
		oflow_driver___( void )
		{
			reset( false );
		}
		~oflow_driver___( void )
		{
			reset();
		}
	};

	class ioflow_driver_base___
	: public iflow_driver_base___,
	  public oflow_driver_base___
	{
	public:
		void reset( bso::bool__ P = true )
		{
			iflow_driver_base___::reset( P );
			oflow_driver_base___::reset( P );
		}
		void Init(
			datum__ *InputCache,
			size__ InputCacheSize,
			thread_safety__ ThreadSafety )
		{
			iflow_driver_base___::Init( InputCache, InputCacheSize, ThreadSafety );
			oflow_driver_base___::Init( ThreadSafety );
		}
	};

	template <int input_cache_size = FDR__DEFAULT_CACHE_SIZE> class ioflow_driver___
	: public ioflow_driver_base___
	{
	private:
		datum__ _InputCache[input_cache_size];
	public:
		void Init( thread_safety__ ThreadSafety )
		{
			ioflow_driver_base___::Init( _InputCache, sizeof( _InputCache ), ThreadSafety );
		}
	};
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
