/*
  Header for the 'mdr' library by Claude SIMON (csimon@epeios.org)
  Copyright (C) 2002 Claude SIMON (csimon@epeios.org) 

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

	//t The portable version of a 'size__'.
	typedef bso::p_ulong__	p_size__;

	//t The size of a buffer.
	typedef bso::bsize__	bsize__;

	//t The type of the data in a memory.
	typedef bso::raw__	data__;

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

	//c Basic memory driver. Use 'MEMORY_DRIVER_' instead directly this class.
	class basic_memory_driver_
	{
	protected:
		//v Place 'Amount' bytes in 'Buffer' from the memory at position 'Position'.
		virtual void MDRRead(
			row__ Position,
			bsize__ Amount,
			data__ *Buffer ) = 0;
		//v Write 'Amount' bytes from 'Buffer' to memory at position 'Position'.
		virtual void MDRWrite(
			const data__ *Buffer,
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
		virtual void MDRSynchronize( void )
		{
			// for read-only memory.
		}
	public:
		struct s {};
		basic_memory_driver_( s &S )
		{
			reset( true );
		}
		virtual ~basic_memory_driver_( void )	// to be sure that the destructor of dervaed classes are call.
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
		//f Place 'Amount' bytes in 'Buffer' from the memory at position 'Position'.
		void Read(
			row__ Position,
			bsize__ Amount,
			data__ *Buffer )
		{
			MDRRead( Position, Amount, Buffer );
		}
		//f Write 'Amount' bytes from 'Buffer' to memory at position 'Position'.
		void Write(
			const data__ *Buffer,
			bsize__ Amount,
			row__ Position )
		{
			MDRWrite( Buffer, Amount, Position );
		}
		//f Allocate 'Size' bytes in memory.
		void Allocate( size__ Size )
		{
			MDRAllocate( Size );
		}
		//f Flush caches.
		void Synchronize( void )
		{
			MDRSynchronize();
		}
	};

	class basic_memory_driver
	: public basic_memory_driver_
	{
	public:
		basic_memory_driver_::s static_;
		basic_memory_driver( void )
		: basic_memory_driver_( static_ )
		{
			basic_memory_driver_::reset( false );
		}
		virtual ~basic_memory_driver( void )
		{
			basic_memory_driver_::reset( true );
		}
	};

	#define E_MEMORY_DRIVER_	basic_memory_driver_
	#define E_MEMORY_DRIVER		basic_memory_driver
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
