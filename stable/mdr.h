/*
	Header for the 'mdr' library by Claude SIMON (csimon@epeios.org)
	Copyright (C) 2000-2004 Claude SIMON (csimon@epeios.org).

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

#ifndef MDR__INC
#define MDR__INC

#define MDR_NAME		"MDR"

#define	MDR_VERSION	"$Revision$"

#define MDR_OWNER		"Claude SIMON (csimon@epeios.org)"

#include "ttr.h"

extern class ttr_tutor &MDRTutor;

#if defined( XXX_DBG ) && !defined( MDR_NODBG )
#define MDR_DBG
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
//D Memory DRiver 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

//D Memory DRiver.

#include "err.h"
#include "flw.h"
#include "bso.h"

namespace mmm {
	class multimemory_;
}

namespace mdr {

	//t The position in a memory.
	typedef bso::ulong__	row__;

	//t The size of a memory.
	typedef bso::ulong__	size__;
	#define MDR_SIZE_MAX	BSO_ULONG_MAX

	//t The portable version of a 'size__'.
	typedef bso::p_ulong__	p_size__;
	#define MDR_P_SIZE_MAX	BSO_P_ULONG_MAX

	//t The size of a buffer.
	typedef bso::bsize__	bsize__;
	#define MDR_BSIZE_MAX	BSO_BSIZE_MAX

	//t The type of the datum in a memory.
	typedef bso::raw__	datum__;

	//e Access mode to the memory.
	enum mode
	{
		//i Undefined.
		mUndefined,
		//i Read only.
		mReadOnly,
		//i Read/write
		mReadWrite,
		//i amount of mode.
		m_amount
	};

	//c Abstract memory driver. Use 'E_MEMORY_DRIVER_' instead directly this class.
	class memory_driver_
	{
	protected:
		//v Recall 'Amount' at position 'Position' and put them in 'Buffer'.
		virtual void MDRRecall(
			row__ Position,
			bsize__ Amount,
			datum__ *Buffer ) = 0;
		//v Write 'Amount' bytes from 'Buffer' to memory at position 'Position'.
		virtual void MDRStore(
			const datum__ *Buffer,
			bsize__ Amount,
			row__ Position )
		{
			ERRu();
			// For read-only memory.
		}
		//v Allocate 'Capacity' bytes in memory.
		virtual void MDRAllocate( size__ Size )
		{
			ERRu();
			// For read-only memory.
		}
		//v Flush caches.
		virtual void MDRFlush( void )
		{
			ERRu();
			// for read-only memory.
		}
	public:
		struct s {};
		memory_driver_( s &S )
		{
			reset( true );
		}
		virtual ~memory_driver_( void )	// to be sure that the destructor of dervaed classes are call.
		{
			reset( false );
		}
		void reset( bool = true )
		{}
		void plug( class mmm::multimemory_ & )
		{
			// For standardization.
		}
		//f Initialization.
		void Init( void )
		{}
		//f Recall 'Amount' at position 'Position' and put them into 'Buffer'. Return 'Buffer'.
		void Recall(
			row__ Position,
			bsize__ Amount,
			datum__ *Buffer )
		{
			MDRRecall( Position, Amount, Buffer );
		}
		//f Store 'Amount' bytes from 'Buffer' at position 'Position'.
		void Store(
			const datum__ *Buffer,
			bsize__ Amount,
			row__ Position )
		{
			MDRStore( Buffer, Amount, Position );
		}
		//f Allocate 'Size' bytes in memory.
		void Allocate( size__ Size )
		{
			MDRAllocate( Size );
		}
		//f Flush buffers.
		void Flush( void )
		{
			MDRFlush();
		}
	};

	class memory_driver
	: public memory_driver_
	{
	public:
		memory_driver_::s static_;
		memory_driver( void )
		: memory_driver_( static_ )
		{
			memory_driver_::reset( false );
		}
		virtual ~memory_driver( void )
		{
			memory_driver_::reset( true );
		}
	};

	#define E_MEMORY_DRIVER_	memory_driver_
	#define E_MEMORY_DRIVER		memory_driver
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
