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
	
	class read_write_lock___
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
		read_write_lock___( void )
		{
			reset( false );
		}
		~read_write_lock___( void )
		{
			reset( true );
		}
		void Init( void )
		{
			reset();

			ReadCounterProtection = mtx::Create();
			WritingRequest = mtx::Create();
			WritingPermission = mtx::Create();
			OverflowProtection = mtx::Create();
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
	};

	template <typename object> class access_control__
	{
	private:
		object *Object_;
		lck::read_write_lock___ *Lock_;
		bso::bool__ Locked_;
		bso::bool__ Writing_;
	public:
		void reset( bso::bool__ P = true )
		{
			Object_ = NULL;
			Lock_ = NULL;
			Locked_ = false;
			Writing_ = false;
		}
		access_control__( void )
		{
			reset( false );
		}
		~access_control__( void )
		{
			reset( true );
		}
		void Init(
			object &Object,
			lck::read_write_lock___ &Lock )
		{
			Object_ = &Object;
			Lock_ = &Lock;
		}
		const object &GetForReading( void )
		{
#ifdef LCK__DBG
			if ( Locked_ )
				ERRu();
#endif
			Lock_->WaitUntilReadingAllowed();

			Locked_ = true;
			Writing_ = false;

			return *Object_;
		}
		void ReleaseReading( void )
		{
#ifdef LCK__DBG
			if ( !Locked_ )
				ERRu();

			if ( Writing_ )
				ERRu();
#endif
			Lock_->ReadingAchieved();

			Locked_ = false;
		}
		object &GetForWriting( void )
		{
#ifdef LCK__DBG
			if ( Locked_ )
				ERRu();
#endif
			Lock_->WaitUntilWritingAllowed();

			Locked_ = true;
			Writing_ = true;

			return *Object_;
		}
		void ReleaseWriting( void )
		{
#ifdef LCK__DBG
			if ( !Locked_ )
				ERRu();

			if ( !Writing_ )
				ERRu();
#endif
			Lock_->WritingAchieved();

			Locked_ = false;
		}
		bso::bool__ IsLocked( void ) const
		{
			return Locked_;
		}
		bso::bool__ IsWriting( void ) const
		{
			return Writing_;
		}
		bso::bool__ ReleaseLock( void )	// Return true if it was locked.
		{
			if ( IsLocked() ) {
				if ( IsWriting() ) 
					ReleaseWriting();
				else
					ReleaseReading();
				return true;
			} else
				return false;
		}
	};


}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
