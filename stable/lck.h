/*
	Header for the 'lck' library by Claude SIMON (csimon@epeios.org)
	Copyright (C) 2003 Claude SIMON (csimon@epeios.org).

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
			mtx::Lock( WritingRequest ); // Locked if there is a write request.
			mtx::Unlock( WritingRequest );

			mtx::Lock( ReadCounterProtection );
	
			while ( ReadCounter == LCK_MAX_CONCURRENT_READ ) {
				mtx::Unlock( ReadCounterProtection );
				mtx::Lock( OverflowProtection );
				mtx::Lock( WritingRequest ); // Locked if there is a write request.
				mtx::Unlock( WritingRequest );
				mtx::Lock( ReadCounterProtection );
			}

			ReadCounter++;

			if ( ReadCounter == 1 )
				mtx::Lock( WritingPermission );
			
			if ( ReadCounter == LCK_MAX_CONCURRENT_READ )
				mtx::Lock( OverflowProtection );

			mtx::Unlock( ReadCounterProtection );
		}
		void ReadingAchieved( void )
		{
			mtx::Lock( ReadCounterProtection );

			if ( ReadCounter == 1 )
				mtx::Unlock( WritingPermission );
			
			if ( ReadCounter == LCK_MAX_CONCURRENT_READ )
				mtx::Unlock( OverflowProtection );

			ReadCounter--;

			mtx::Unlock( ReadCounterProtection );
		}
		void WaitUntilWritingAllowed( void )
		{
			mtx::Lock( WritingRequest );	// To lock all coming read (and write) request;

			mtx::Lock( WritingPermission );
		}
		void WritingAchieved( void )
		{
			mtx::Unlock( WritingPermission );

			mtx::Unlock( WritingRequest );
		}
	};

}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
