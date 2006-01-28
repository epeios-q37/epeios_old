/*
	Header for the 'mtx' library by Claude SIMON (csimon@epeios.org)
	Copyright (C) 2000, 2003-2004 Claude SIMON (csimon@epeios.org).

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

#include "bso.h"
#include "cpe.h"
#include "tht.h"
#include "tol.h"

#ifdef MTX_DEFAULT_DELAY
#	define MTX__DEFAULT_DELAY	MTX_DEFAULT_DELAY
#else
#	define MTX__DEFAULT_DELAY	1
#endif

#ifndef CPE__MT
#	error "This library only useful in multitasking context, in which you are not."
#endif

#ifndef MTX_NO_CONTROL
#	ifdef MTX_DBG
#		define MTX__CONTROL
#	endif
#endif

//d A invalid mutex handler.
#define MTX_INVALID_HANDLER	NULL


#define MTX_MAX_COUNTER_VALUE	BSO_UBYTE_MAX

namespace mtx {

	enum mode__ {
		mFree,	// Le mutex n'a pas de propriétaire. N'importe quel 'thread' peut le déverouiiler.
		mOwned,	// Le thread propriétaire peut le vérouiiler autant de fois qu'il le veut sans être bloqué.
				// En mode 'debug', une erreur est génèrée si le mutex est déverouiiler pas una utre threéd que le 
				// propriétaire.
		m_amount,
		m_Undefined
	};

	namespace {
		typedef bso::ubyte__ counter__;
	}

#ifdef MTX__CONTROL
#	define MTX_RELEASED_MUTEX_VALUE	BSO_UBYTE_MAX
#endif

	//t A mutex handler.
	typedef struct mutex__ {
		counter__ Counter;
		mode__ Mode;
		tht::thread_id__ Owner;
#ifdef MTX__CONTROL
		void Release( void )
		{
			Counter = MTX_RELEASED_MUTEX_VALUE;
		}
		bso::bool__ IsReleased( void ) const
		{
			return Counter == MTX_RELEASED_MUTEX_VALUE;
		}
#endif
		bso::bool__ IsLocked( void ) const
		{
#ifdef MTX__CONTROL
			if ( IsReleased() )
				ERRu();
#endif
			return Counter != 0;
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

			if ( ++Counter == MTX_MAX_COUNTER_VALUE )
				ERRl();

			if ( Counter > 1 )
			{
				Counter--;
				return false;
			}
			else {
				Owner = tht::GetTID();
				return true;
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
			Counter--;
			Owner = THT_UNDEFINED_THREAD_ID;
		}
		mutex__( mode__ Mode )
		{
			Counter = 0;
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
			Release();
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

	// Wait until mutex unlocked, with 'Delay' millisecond between delays.
	inline void WaitUntilUnlocked_(
		mutex_handler__ Handler,
		unsigned long Delay = MTX__DEFAULT_DELAY )
	{
		while( !TryToLock( Handler ) )
			tht::Defer( Delay );
	}

	//f Lock 'Handler'. Blocks until lock succeed with 'Delay' millisecond between tries.
	inline void Lock(
		mutex_handler__ Handler,
		unsigned long Delay = MTX__DEFAULT_DELAY )
	{
#ifdef MTX_DBG
		if ( Handler == NULL )
			ERRu();
#endif
		if ( !TryToLock( Handler ) )
			WaitUntilUnlocked_( Handler, Delay );
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
		//f Lock the semaphore.Waits 'Delay' millisecond between tries. BLOCKING FUNCTION.
		void Lock( unsigned long Delay = MTX__DEFAULT_DELAY )
		{
#ifdef MTX_DBG
			if ( Handler_ == MTX_INVALID_HANDLER )
				ERRu();
#endif
			mtx::Lock( Handler_, Delay );
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
		bso::bool__ IsLocked( void )
		{
			return mtx::IsLocked( Handler_ );
		}
	};
}




/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
