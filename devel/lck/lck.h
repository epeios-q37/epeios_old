/*
	Header for the 'lck' library by Claude SIMON (csimon@epeios.org)
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

#ifndef LCK__INC
#define LCK__INC

#define LCK_NAME		"LCK"

#define	LCK_VERSION	"$Revision$"

#define LCK_OWNER		"Claude SIMON (csimon@epeios.org)"

#include "ttr.h"

extern class ttr_tutor &LCKTutor;

#if defined( XXX_DBG ) && !defined( LCK_NODBG )
#define LCK_DBG
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
//D LoCKs 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "mtx.h"


namespace lck {

	//t Type of an access counter.
	typedef bso::ubyte__ counter__;	// If changed, change below line too.

	//d Maximum amount of concurrent read.
	#define LCK_MAX_CONCURRENT_READ	BSO_UBYTE_MAX
	
	class lock___
	{
	public:
		mtx::mutex_handler__ ReadCounterProtection;
		mtx::mutex_handler__ WritingRequest;
		mtx::mutex_handler__ WritingPermission;
		mtx::mutex_handler__ OverflowProtection;
		counter__ ReadCounter;
		void reset( bso::bool__ P = true )
		{
			if ( P ) {
				if ( ReadCounterProtection != MTX_INVALID_HANDLER )
					mtx::Delete( ReadCounterProtection );

				if ( WritingRequest != MTX_INVALID_HANDLER )
					mtx::Delete( WritingRequest );

				if ( WritingPermission != MTX_INVALID_HANDLER )
					mtx::Delete( WritingPermission );

				if ( OverflowProtection != MTX_INVALID_HANDLER )
					mtx::Delete( OverflowProtection );
			}

			ReadCounterProtection = MTX_INVALID_HANDLER;
			WritingRequest= MTX_INVALID_HANDLER;
			WritingPermission = MTX_INVALID_HANDLER;
			OverflowProtection = MTX_INVALID_HANDLER;

			ReadCounter = 0;
		}
		lock___( void )
		{
			reset( false );
		}
		~lock___( void )
		{
			reset( true );
		}
		void Init( void )
		{
			reset();

			ReadCounterProtection = mtx::Create( mtx::mFree );
			WritingRequest = mtx::Create( mtx::mFree );
			WritingPermission = mtx::Create( mtx::mFree );
			OverflowProtection = mtx::Create( mtx::mFree );
		}
		void WaitUntilReadingAllowed( void )
		{
			mtx::Lock( WritingRequest );	// Wait until there is no
			mtx::Unlock( WritingRequest );	// more write request pending.

			mtx::Lock( ReadCounterProtection );	// Wait until read counter available,
												// and avoid other to access them.
	
			while ( ReadCounter == LCK_MAX_CONCURRENT_READ ) { // While there is too more reading pending.
				mtx::Unlock( ReadCounterProtection );	// Release the read counter.
				mtx::Lock( OverflowProtection );	// Wait until there is lesser reading pending.
				mtx::Lock( WritingRequest );	// Wait until there is no
				mtx::Unlock( WritingRequest );	// more write request pending.
				mtx::Lock( ReadCounterProtection );	// Wait until read counter available,
													// and avoid other to access them.
			}

			// There is not too much reading and no writing.

			ReadCounter++;	// To tell that there is one more reading pending.

			if ( ReadCounter == 1 )	// If there was no reading before.
				mtx::Lock( WritingPermission );	// Wait that there is no more writing
												// and forbid all comming writing request.
			else if ( ReadCounter == LCK_MAX_CONCURRENT_READ )	// If there is to much reading.
				mtx::Lock( OverflowProtection );	// Forbid more reading.

			mtx::Unlock( ReadCounterProtection );	// Read counter released.
		}
		void ReadingAchieved( void )
		{
			mtx::Lock( ReadCounterProtection );	// Wait until read counter available,
												// and avoid other to access them.

			if ( ReadCounter == 1 )	// If this was the last reading,
				mtx::Unlock( WritingPermission );	// Allow comming writing.
			else if ( ReadCounter == LCK_MAX_CONCURRENT_READ )	// If there was too much reading.
				mtx::Unlock( OverflowProtection );	// Allox yet more reading.

			ReadCounter--;	// One less reading.

			mtx::Unlock( ReadCounterProtection );	// Read counter released.
		}
		void WaitUntilWritingAllowed( void )
		{
			mtx::Lock( WritingRequest );	// To lock all coming read (and write) request;

			mtx::Lock( WritingPermission );	// To ensure that writing is allowed.
		}
		void WritingAchieved( void )
		{
			mtx::Unlock( WritingPermission );	// Reading request can yet forbid all incoming writing.

			mtx::Unlock( WritingRequest );	// Another writing (an reading) can yet occurs.
		}
		bso::bool__ IsLockedForWriting( void ) const
		{
			return mtx::IsLocked( WritingPermission );
		}
		bso::bool__ IsLockedForReading( void ) const
		{
			bso::bool__ IsLocked;

			mtx::Lock( ReadCounterProtection );	// Wait until read counter available,
												// and avoid other to access them.

			IsLocked = ReadCounter != 0;

			mtx::Unlock( ReadCounterProtection );

			return IsLocked;
		}
		bso::bool__ IsLocked( void ) const
		{
			return IsLockedForWriting() || IsLockedForReading();
		}
	};

	template <typename object> class control___
	{
	private:
		object *Object_;
		lck::lock___ Lock_;
		bso::bool__ Locked_;
		bso::bool__ ReadWrite_;
		mtx::mutex_handler__ Mutex_;
		void _Lock( void )
		{
			mtx::Lock( Mutex_ );
		}
		void _Unlock( void )
		{
			mtx::Unlock( Mutex_ );
		}
		void _ReleaseReadOnly( void )
		{
#ifdef LCK__DBG
			if ( !Locked_ )
				ERRu();

			if ( ReadWrite_ )
				ERRu();
#endif
			Lock_.ReadingAchieved();

			Locked_ = false;
		}
		void _ReleaseReadWrite( void )
		{
#ifdef LCK__DBG
			if ( !Locked_ )
				ERRu();

			if ( !ReadWrite_ )
				ERRu();
#endif
			Lock_.WritingAchieved();

			Locked_ = false;
		}
	public:
		void reset( bso::bool__ P = true )
		{

			if ( P ) {
				if ( Mutex_ != MTX_INVALID_HANDLER )
					mtx::Delete( Mutex_ );
			}

			Object_ = NULL;
			Lock_.reset( P );
			Locked_ = false;
			ReadWrite_ = false;
			Mutex_ = MTX_INVALID_HANDLER;
		}
		control___( void )
		{
			reset( false );
		}
		~control___( void )
		{
			reset( true );
		}
		void Init( object &Object )
		{
			reset();

			Object_ = &Object;
			Lock_.Init();
			Mutex_ = mtx::Create( mtx::mFree );
		}
		const object &GetReadOnly( void )
		{
#ifdef LCK__DBG
			_Lock();
			if ( Locked_ )
				ERRu();
			_Unlock();
#endif
			Lock_.WaitUntilReadingAllowed();

			_Lock();
			Locked_ = true;
			ReadWrite_ = false;
			_Unlock();

			return *Object_;
		}
		const object &GetSharedAccess( void )
		{
			return GetReadOnly();
		}
		void ReleaseReadOnly( void )
		{
			_Lock();
			_ReleaseReadOnly();
			_Unlock();
		}
		void ReleaseSharedAccess( void )
		{
			ReleaseReadOnly();
		}
		object &GetReadWrite( void )
		{
#ifdef LCK__DBG
			_Lock();
			if ( Locked_ )
				ERRu();
			_Unlock();
#endif
			Lock_.WaitUntilWritingAllowed();

			_Lock();
			Locked_ = true;
			ReadWrite_ = true;
			_Unlock();

			return *Object_;
		}
		object &GetExclusiveAccess( void )
		{
			return GetReadWrite();
		}
		void ReleaseReadWrite( void )
		{
			_Lock();
			_ReleaseReadWrite();
			_Unlock();
		}
		void ReleaseExclusiveAccess( void )
		{
			ReleaseReadWrite();
		}
		bso::bool__ ReleaseLock( void )	// Return true if it was locked.
		{
			bso::bool__ WasLocked = false;

			_Lock();

			if ( Locked_ ) {
				if ( ReadWrite_ ) 
					ReleaseReadWrite();
				else
					ReleaseReadOnly();
				WasLocked =true;
			}

			_Unlock();

			return WasLocked;
		}
		object &GetWithoutLocking( void )
		{
			return *Object_;
		}
		const object &GetWithoutLocking( void ) const
		{
			return *Object_;
		}
		bso::bool__ IsLocked( void )
		{
			return Lock_.IsLocked();
		}
	};

	template <typename object> class read_only_access___
	{
	private:
		control___<object> *Control_;
		bso::bool__ _Locked;
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P )
				if ( Control_ != NULL ) {
					if ( _Locked )
						Control_->ReleaseReadOnly();
				}

			Control_ = NULL;
			_Locked = false;
		}
		read_only_access___( void )
		{
			reset( false );
		}
		~read_only_access___( void )
		{
			reset();
		}
		void Init( control___<object> &Control )
		{
			reset();

			Control_ = &Control;
		}
		const object &operator ()( void )
		{
			if ( !_Locked ) {
				Control_->GetReadOnly();
				_Locked = true;
			}

			return Control_->GetWithoutLocking();
		}
		bso::bool__ IsLocked( void ) const
		{
			return _Locked;
		}
		bso::bool__ Release( bso::bool__ ErrorIfNotLocked = true )
		{
			if ( _Locked ) {
				Control_->ReleaseReadOnly();
				_Locked = false;
				return true;
			} else if ( ErrorIfNotLocked )
				ERRu();

			return false;
		}
	};

	template <typename object> class read_write_access___
	{
	private:
		control___<object> *Control_;
		bso::bool__ _Locked;
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P )
				if ( Control_ != NULL )
					if ( _Locked )
						Control_->ReleaseReadWrite();

			Control_ = NULL;
			_Locked = false;
		}
		read_write_access___( void )
		{
			reset( false );
		}
		~read_write_access___( void )
		{
			reset();
		}
		void Init( control___<object> &Control )
		{
			reset();

			Control_ = &Control;
		}
		object &operator ()( void )
		{
			if ( !_Locked ) {
				Control_->GetReadWrite();
				_Locked = true;
			}

			return Control_->GetWithoutLocking();
		}
		bso::bool__ IsLocked( void ) const
		{
			return _Locked;
		}
		bso::bool__ Release( bso::bool__ ErrorIfNotLocked = true )
		{
			if ( _Locked ) {
				Control_->ReleaseReadWrite();
				_Locked = false;
				return true;
			} else if ( ErrorIfNotLocked )
				ERRu();

			return false;
		}
	};


	template <typename objet> E_TTYPEDEF___( read_write_access___<objet>,	exclusive_access___ );
	template <typename objet> E_TTYPEDEF___( read_only_access___<objet>,	shared_access___ );
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
