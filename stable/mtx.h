/*
	Header for the 'mtx' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Copyright (C) 2000, 2003-2004 Claude SIMON (http://zeusw.org/intl/contact.html).

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

#ifndef MTX__INC
#define MTX__INC

#define MTX_NAME		"MTX"

#define	MTX_VERSION	"$Revision$"

#define MTX_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"

#include "ttr.h"

extern class ttr_tutor &MTXTutor;

#if defined( XXX_DBG ) && !defined( MTX_NODBG )
#define MTX_DBG
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
//D MuTeX 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "bso.h"
#include "cpe.h"
#include "tht.h"
#include "tol.h"

#ifdef MTX_DEFAULT_DELAY
#	define MTX__DEFAULT_DELAY	MTX_DEFAULT_DELAY
#else
#	define MTX__DEFAULT_DELAY	1
#endif

#if !defined( MTX_NO_ATOMIC_OPERATIONS )
#	define MTX__USE_ATOMIC_OPERATIONS
#endif

#ifdef MTX__USE_ATOMIC_OPERATIONS
#	if defined( CPE__T_MS )
#		if defined (CPE__T_CYGWIN )
#			define MTX__USE_PTHREAD_MUTEX
#		else
#			define MTX__USE_MS_ATOMIC_OPERATIONS
#		endif
#	elif defined (CPE__T_LINUX )
#		if ( MTX__USE_ATOMIC_LIB )
#			define MTX__USE_LINUX_ATOMIC_OPERATIONS
#		else
#			define MTX__USE_PTHREAD_MUTEX
#		endif
#	elif defined (CPE__T_MAC )
#		define MTX__USE_MAC_ATOMIC_OPERATIONS
#	else
#		error "No atomic operations available for this compiling enviroment"
#	endif
#else
#	define MTX__NO_ATOMIC_OPERATIONS
#endif

#ifndef MTX__USE_ATOMIC_OPERATIONS
#	ifndef MTX_SUPPRESS_NO_ATOMIC_OPERATIONS_WARNING
#		ifdef CPE__C_VC
#			pragma message( "BE CAREFUL : Mutexes do not use atomic operations !" )
#		elif defined( CPE__C_GCC )
#			warning "BE CAREFUL : Mutexes do not use atomic operations !"
#		endif
#	endif
#endif

#ifdef MTX__USE_LINUX_ATOMIC_OPERATIONS
#	include "asm/atomic.h"
#endif

#ifdef MTX__USE_MAC_ATOMIC_OPERATIONS
#	include <libkern/OSAtomic.h>
#endif

#ifdef MTX__USE_PTHREAD_MUTEX
#	include <pthread.h>
#endif

#ifndef CPE__T_MT
#	error "This library only useful in multitasking context, in which you are not."
#endif

#ifndef MTX_NO_CONTROL
#	ifdef MTX_DBG
#		define MTX__CONTROL
#	endif
#endif

//d A invalid mutex handler.
#define MTX__INVALID_HANDLER	NULL	// Utilisation interne.
#define MTX_INVALID_HANDLER		MTX__INVALID_HANDLER	// Pour les utilisateurs.


#define MTX__COUNTER_OVERFLOW_VALUE	BSO_SSHORT_MIN

namespace mtx {

	enum mode__ {
		mFree,	// Le mutex n'a pas de propriétaire. N'importe quel 'thread' peut le déverrouiller.
		mOwned,	// Le thread propriétaire peut le verrouiller autant de fois qu'il le veut sans être bloqué.
				// En mode 'debug', une erreur est génèrée si le mutex est déverrouiller pas un thread autre que le 
				// propriétaire.
		m_amount,
		m_Undefined
	};

#ifdef	MTX__USE_MS_ATOMIC_OPERATIONS
	typedef LONG counter__;
#elif defined( MTX__USE_LINUX_ATOMIC_OPERATIONS )
	typedef atomic_t	counter__;
#elif defined( MTX__USE_MAC_ATOMIC_OPERATIONS )
	typedef int32_t	counter__;
#elif defined ( MTX__USE_PTHREAD_MUTEX )
	struct counter__ {
		volatile bso::sshort__ Value;
		pthread_mutexattr_t MutexAttr;
		pthread_mutex_t Mutex;
	};
#elif defined( MTX__NO_ATOMIC_OPERATIONS )
	typedef volatile bso::sshort__ counter__;
#else
#	error "No mutex handling scheme !"
#endif

#ifdef MTX__CONTROL
#	define MTX__RELEASED_MUTEX_COUNTER_VALUE	2
#endif

#define MTX__UNLOCKED_MUTEX_COUNTER_VALUE	1
#define MTX__UNLOCKED_MUTEX_COUNTER_SIGN	MTX__UNLOCKED_MUTEX_COUNTER_VALUE

	inline void _Set(
		counter__ &Counter,
		int Value,
		bso::bool__ Destroy )	// Retourne le signe de 'Counter'.
	{
#ifdef	MTX__USE_MS_ATOMIC_OPERATIONS
		Counter = Value;
#elif defined( MTX__USE_LINUX_ATOMIC_OPERATIONS )
		atomic_set( &Counter, Value );
#elif defined( MTX__USE_MAC_ATOMIC_OPERATIONS )
		Counter = Value;
#elif defined( MTX__USE_PTHREAD_MUTEX )
		Counter.Value = Value;
		if ( Destroy ) {
			if ( pthread_mutex_destroy( &Counter.Mutex ) )
				ERRs();
			if ( pthread_mutexattr_destroy( &Counter.MutexAttr ) )
				ERRs();
		} else {
			if ( pthread_mutexattr_init( &Counter.MutexAttr ) )
				ERRs();
			if ( pthread_mutexattr_setpshared( &Counter.MutexAttr, PTHREAD_PROCESS_PRIVATE ) )
				ERRs();
			if ( pthread_mutex_init( &Counter.Mutex, &Counter.MutexAttr ) )
				ERRs();
		}
#elif defined( MTX__NO_ATOMIC_OPERATIONS )
		Counter = Value;
#else
#	error "No mutex handling scheme !"
#endif
	}

	inline bso::sign__ _GetSign( counter__ &Counter )	// Retourne le signe de 'Counter'.
	{
#ifdef	MTX__USE_MS_ATOMIC_OPERATIONS
		return Counter;
#elif defined( MTX__USE_LINUX_ATOMIC_OPERATIONS )
		return atomic_read( &Counter );
#elif defined( MTX__USE_MAC_ATOMIC_OPERATIONS )
		return Counter;
#elif defined( MTX__USE_PTHREAD_MUTEX )
		return Counter.Value;
#elif defined( MTX__NO_ATOMIC_OPERATIONS )
		return Counter;
#else
#	error "No mutex handling scheme !"
#endif
	}

	inline int _GetValue( counter__ &Counter )	// Retourne la valeur de 'Counter'.
	{
#ifdef	MTX__USE_MS_ATOMIC_OPERATIONS
		return Counter;
#elif defined( MTX__USE_LINUX_ATOMIC_OPERATIONS )
		return atomic_read( &Counter );
#elif defined( MTX__USE_MAC_ATOMIC_OPERATIONS )
		return Counter;
#elif defined( MTX__USE_PTHREAD_MUTEX )
		return Counter.Value;
#elif defined( MTX__NO_ATOMIC_OPERATIONS )
		return Counter;
#else
#	error "No mutex handling scheme !"
#endif
	}

	inline void _Inc( counter__ &Counter )	// Incrémente 'Counter'.
	{
#ifdef	MTX__USE_MS_ATOMIC_OPERATIONS
		InterlockedIncrement( &Counter );
#elif defined( MTX__USE_LINUX_ATOMIC_OPERATIONS )
		atomic_inc( &Counter );
#elif defined( MTX__USE_MAC_ATOMIC_OPERATIONS )
		OSAtomicIncrement32( &Counter );
#elif defined( MTX__USE_PTHREAD_MUTEX )
		if ( pthread_mutex_lock( &Counter.Mutex ) )
			ERRs();;
		++Counter.Value;
		if ( pthread_mutex_unlock( &Counter.Mutex ) )
			ERRs();
#elif defined( MTX__NO_ATOMIC_OPERATIONS )
		++Counter;
#else
#	error "No mutex handling scheme !"
#endif
	}

	inline bso::bool__ _DecAndTest( counter__ &Counter )	// Décrémente 'Counter'.et retourne 'true' si à zéro.
	{
#ifdef	MTX__USE_MS_ATOMIC_OPERATIONS
		return InterlockedDecrement( &Counter ) == 0;
#elif defined( MTX__USE_LINUX_ATOMIC_OPERATIONS )
		return atomic_dec_and_test( &Counter );
#elif defined( MTX__USE_MAC_ATOMIC_OPERATIONS )
		return OSAtomicDecrement32( &Counter ) == 1;	// '== 1' car 'OSAtomicDecrement' retourne la valeur AVANT décrémentation.
#elif defined( MTX__USE_PTHREAD_MUTEX )
		bso::bool__ Buffer;
		if ( pthread_mutex_lock( &Counter.Mutex ) )
			ERRs();
		Buffer = --Counter.Value == 0;
		if ( pthread_mutex_unlock( &Counter.Mutex ) )
			ERRs();
		return Buffer;
#elif defined( MTX__NO_ATOMIC_OPERATIONS )
		return --Counter == 0;
#else
#	error "No mutex handling scheme !"
#endif
	}

	//t A mutex handler.
	typedef struct mutex__ {
		counter__ Counter;
		mode__ Mode;
		tht::thread_id__ Owner;
#ifdef MTX__CONTROL
		void Release( bso::bool__ Destroy )	// 'Destruction' à vrai si appelé par le destructeur.
		{
			_Set( Counter, MTX__RELEASED_MUTEX_COUNTER_VALUE, Destroy );
		}
		bso::bool__ IsReleased( void )
		{
			return _GetValue( Counter ) == MTX__RELEASED_MUTEX_COUNTER_VALUE;
		}
#endif
		bso::bool__ IsLocked( void )
		{
#ifdef MTX__CONTROL
			if ( IsReleased() )
				ERRu();
#endif
			return _GetSign( Counter ) != MTX__UNLOCKED_MUTEX_COUNTER_SIGN;
		}
		bso::bool__ TryToLock( void )
		{
#ifdef MTX__CONTROL
				if ( IsReleased() )
					ERRu();
#endif
			if ( IsLocked() )
				switch ( Mode ) {
				case mFree:
					return false;
					break;
				case mOwned:
					return tht::GetTID() == Owner;
					break;
				default:
					ERRu();
					break;
				}

			if ( _GetValue( Counter ) == MTX__COUNTER_OVERFLOW_VALUE )
				ERRl();

			if ( _DecAndTest( Counter ) ) {
				Owner = tht::GetTID();
				return true;
			} else {
				_Inc( Counter );
				return false;
			}
		}
		void Unlock( void )
		{
#ifdef XXX_DBG
			if ( !IsLocked() )
				ERRu();

			switch ( Mode ) {
			case mFree:
				break;
			case mOwned:
				if ( Owner != tht::GetTID() )
					ERRu();
				break;
			default:
				ERRu();
				break;
			}
#endif
			_Inc( Counter );

			Owner = THT_UNDEFINED_THREAD_ID;
		}
		bso::bool__ IsOwner( void ) const
		{
			return Owner == tht::GetTID();
		}
		mutex__( mode__ Mode )
		{
			_Set( Counter, MTX__UNLOCKED_MUTEX_COUNTER_VALUE, false );

			this->Mode = Mode;
			Owner = THT_UNDEFINED_THREAD_ID;

#ifdef MTX_DBG
			switch( this->Mode ) {
			case mFree:
			case mOwned:
				break;
			default:
				ERRu();
				break;
			}
#endif

		}
		~mutex__( void )
		{
#ifdef MTX__CONTROL
			Release( true );
#endif
		}
	} *mutex_handler__;


	//f Return a new mutex handler.
	inline mutex_handler__ Create( mode__ Mode )
	{
		mutex_handler__ Handler;
		
		if ( ( Handler = new mutex__( Mode ) ) == NULL )
			ERRa();

		return Handler;
	}

	inline bso::bool__ IsLocked( mutex_handler__ Handler )
	{
#ifdef MTX_DBG
		if ( Handler == NULL )
			ERRu();
#endif
		return Handler->IsLocked();
	}


	//f Try to lock 'Handler' without blocking. Return 'true' if locks succeed, false otherwise.
	inline bso::bool__ TryToLock( mutex_handler__ Handler )
	{
#ifdef MTX_DBG
		if ( Handler == NULL )
			ERRu();
#endif
		return Handler->TryToLock();
	}

	// Wait until mutex unlocked.
	inline void WaitUntilUnlocked_( mutex_handler__ Handler )
	{
		while( !TryToLock( Handler ) )
		{
			tol::InitializeRandomGenerator();
			tht::Defer( ( (unsigned int)tht::GetTID() + rand() ) % 5 + 1 ); // 'rand() donne-t'il la même suite dans des coeurs différents ?
		}
	}

	//f Lock 'Handler'. Blocks until lock succeed.
	inline void Lock( mutex_handler__ Handler )
	{
#ifdef MTX_DBG
		if ( Handler == NULL )
			ERRu();
#endif
		if ( !TryToLock( Handler ) )
			WaitUntilUnlocked_( Handler );
	}

	//f Unlock 'Handler'.
	inline void Unlock( mutex_handler__ Handler )
	{
#ifdef MTX_DBG
		if ( Handler == NULL )
			ERRu();
#endif
		Handler->Unlock();
	}

	inline bso::bool__ IsOwner( mutex_handler__ Handler )
	{
		return Handler->IsOwner();
	}

	//f Delete the mutex of handler 'Handler'.
	inline void Delete(
		mutex_handler__ Handler,
		bso::bool__ EvenIfLocked = false )
	{
#ifdef MTX_DBG
		if ( Handler == NULL )
			ERRu();

		if ( Handler->IsLocked() && !EvenIfLocked )
			ERRu();
#endif
		delete Handler;
	}

	//c A mutex.
	class mutex___
	{
	private:
		// Mutex handler.
		mutex_handler__ Handler_;
	public:
		void reset( bool P = true )
		{
			Handler_ = MTX__INVALID_HANDLER;
		}
		mutex___( void )
		{
			reset( false );
		}
		~mutex___( void )
		{
			reset( true );
		}
		//f Initialization.with mutex handler 'Handler'.
		void Init( mutex_handler__ Handler )
		{
			reset();
			
			Handler_ = Handler;
		}
		//f Lock the semaphore.
		void Lock( void )
		{
#ifdef MTX_DBG
			if ( Handler_ == MTX__INVALID_HANDLER )
				ERRu();
#endif
			mtx::Lock( Handler_ );
		}
		//f Unlock the semaphore.
		void Unlock( void )
		{
#ifdef MTX_DBG
			if ( Handler_ == MTX__INVALID_HANDLER )
				ERRu();
#endif
			mtx::Unlock( Handler_ );
		}
		//f Try to lock without blocking. Return true if success, false otherwise.
		bso::bool__ TryToLock( void )
		{
#ifdef MTX_DBG
			if ( Handler_ == MTX__INVALID_HANDLER )
				ERRu();
#endif
			return mtx::TryToLock( Handler_ );
		}
		bso::bool__ IsLocked( void )
		{
			return mtx::IsLocked( Handler_ );
		}
		bso::bool__ IsOwner( void ) const
		{
			return mtx::IsOwner( Handler_ );
		}
	};
}




/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
