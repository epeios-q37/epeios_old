/*
	Header for the 'mtx' library by Claude SIMON (csimon@epeios.org)
	Copyright (C) 2000, 2003  Claude SIMON (csimon@epeios.org).

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

#ifndef MTX__INC
#define MTX__INC

#define MTX_NAME		"MTX"

#define	MTX_VERSION	"$Revision$"

#define MTX_OWNER		"Claude SIMON (csimon@epeios.org)"

#include "ttr.h"

extern class ttr_tutor &MTXTutor;

#if defined( XXX_DBG ) && !defined( MTX_NODBG )
#define MTX_DBG
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
//D MuTeX 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

/* Addendum to the automatic documentation generation part. */
//D MuTeX.
/* End addendum to automatic documentation generation part. */

#include "bso.h"
#include "cpe.h"

#ifndef CPE__MT
	error "This library only useful in multitasking context, in which you are not."
#endif

#if defined( CPE__UNIX ) && !defined( MTX_USE_COUNTER ) && !defined( CPE__CYGWIN )
#	define MTX__USE_PTHREAD_MUTEX
#	include <pthread.h>
#	include <errno.h>
#else
#	define MTX__USE_COUNTER
#endif

#if !defined( MTX__USE_PTHREAD_MUTEX ) && !defined( MTX__USE_COUNTER )
#	error "Don't know what mutex type to use."
#endif

#ifdef MTX_CONTROL
#	ifdef MTX_DBG
#		ifdef MTX__USE_PTHREAD_MUTEX
#			error "'MTX_CONTROL' can not be using pthread mutexes."
#		else
#			define MTX__CONTROL
#		endif
#	else
#		error "'MTX_CONTROL' can be used only in debug modus!".
#	endif
#endif

#include "stf.h"

//d A invalid mutex handler.
#define MTX_INVALID_HANDLER	NULL


#define MTX_MAX_COUNTER_VALUE	BSO_UBYTE_MAX

namespace mtx {

#ifdef MTX__USE_COUNTER
	namespace {
		typedef bso::ubyte__ counter__;
	}
#endif

#ifdef MTX__CONTROL
#	define MTX_RELEASED_MUTEX	BSO_UBYTE_MAX
#endif

#ifdef MTX__USE_PTHREAD_MUTEX
	typedef pthread_mutex_t *mutex_handler__;
#elif defined( MTX__USE_COUNTER )
	//t A mutex handler.
	typedef counter__ *mutex_handler__;
#endif

	//f Return a new mutex handler.
	inline mutex_handler__ Create( void )
	{
		mutex_handler__ Handler;
		
		if ( ( Handler = (mutex_handler__)malloc( sizeof( *Handler ) ) ) == NULL )
			ERRa();

#ifdef MTX__USE_PTHREAD_MUTEX
		if ( pthread_mutex_init( Handler, NULL ) != 0 )
			ERRs();
#elif defined( MTX__USE_COUNTER )
		*Handler = 0;
#endif

		return Handler;
	}

	//f Try to lock 'Handler' without blocking. Return 'true' if locks succeed, false otherwise.
	inline bso::bool__ TryToLock( mutex_handler__ Handler )
	{
#ifdef MTX__USE_PTHREAD_MUTEX
		switch ( pthread_mutex_trylock( Handler ) ) {
		case 0:
			return true;
			break;
		case EBUSY:
			return false;
			break;
		default:
			ERRs();
			break;
		}
#elif defined( MTX__USE_COUNTER )
#	ifdef MTX__CONTROL
		if ( *Handler == MTX_RELEASED_MUTEX )
			ERRu();
#	endif
		if ( *Handler != 0 )
			return false;

		if ( *Handler == MTX_MAX_COUNTER_VALUE )
			ERRl();

		(*Handler)++;

		if ( *Handler > 1 )
		{
			(*Handler)--;
			return false;
		}
		else
			return true;
#endif
	}

#ifdef MTX__USE_COUNTER
	// Wait until mutex unlocked.
	inline void WaitUntilUnlocked_( mutex_handler__ Handler )
	{
		while( !TryToLock( Handler ) )
			tol::Defer();
	}
#endif

	//f Lock 'Handler'. Blocks until lock succeed.
	inline void Lock( mutex_handler__ Handler )
	{
#ifdef MTX__USE_PTHREAD_MUTEX
		if ( pthread_mutex_lock( Handler ) != 0 )
			ERRs();
#elif defined( MTX__USE_COUNTER )
#	ifdef MTX__CONTROL
		if ( *Handler == MTX_RELEASED_MUTEX )
			ERRu();
#	endif
		if ( !TryToLock( Handler ) )
			WaitUntilUnlocked_( Handler );
#endif
	}

	//f Unlock 'Handler'.
	inline void Unlock( mutex_handler__ Handler )
	{
#ifdef MTX__USE_PTHREAD_MUTEX
		if ( pthread_mutex_unlock( Handler ) != 0 )
			ERRs();
#elif defined( MTX__USE_COUNTER )
#	ifdef MTX__CONTROL
		if ( *Handler == MTX_RELEASED_MUTEX )
			ERRu();
		if ( *Handler == 0 )
			ERRu();
#	endif
		(*Handler)--;
#endif
	}

	//f Delete the mutex of handler 'Handler'.
	inline void Delete( mutex_handler__ Handler )
	{
#ifdef MTX__USE_PTHREAD_MUTEX
		if ( pthread_mutex_destroy( Handler ) != 0 ) {
				ERRs();
		}
#endif
#ifdef MTX__CONTROL
		if ( *Handler != 0 )
			ERRu();
		*Handler = MTX_RELEASED_MUTEX;
#else
		free( Handler );
#endif

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
			Handler_ = MTX_INVALID_HANDLER;
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
		//f Lock the semaphore. BLOCKING FUNCTION.
		void Lock( void )
		{
#ifdef MTX_DBG
			if ( Handler_ == MTX_INVALID_HANDLER )
				ERRu();
#endif
			mtx::Lock( Handler_ );
		}
		//f Unlock the semaphore.
		void Unlock( void )
		{
#ifdef MTX_DBG
			if ( Handler_ == MTX_INVALID_HANDLER )
				ERRu();
#endif
			mtx::Unlock( Handler_ );
		}
		//f Try to lock without blocking. Return true if success, false otherwise.
		bso::bool__ TryToLock( void )
		{
#ifdef MTX_DBG
			if ( Handler_ == MTX_INVALID_HANDLER )
				ERRu();
#endif
			return mtx::TryToLock( Handler_ );
		}
	};
}




/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
