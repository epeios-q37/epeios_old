/*
	Header for the 'mdr' library by Claude SIMON (csimon@epeios.org)
	Copyright (C) $COPYRIGHT_DATES$Claude SIMON (csimon@epeios.org).
$_RAW_$
	This file is part of the Epeios (http://epeios.org/) project.

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

#define MDR_INTERNAL_MEMORY_DRIVER *(mdr::E_MEMORY_DRIVER__ *)NULL

namespace mdr {

	//t The position in a memory.
	typedef bso::size__	row_t__;
	typedef bso::p_size__	p_row_t__;


	#define MDR_ROW_T_MAX		( BSO_SIZE_MAX - 1 )	// 'BSO_SIZE_MAX' is 'NONE'.


	//t The size of a memory.
	using bso::size__;
	#define MDR_SIZE_MAX	BSO_SIZE_MAX

	//t The portable version of a 'size__'.
	using bso::p_size__;
	#define MDR_P_SIZE_MAX	BSO_P_SIZE_MAX

	//t The type of the datum in a memory.
	typedef bso::raw__	datum__;

	//e Access mode to the memory.
	enum mode__
	{
		//i Read only.
		mReadOnly,
		//i Read/write
		mReadWrite,
		//i amount of mode.
		m_amount,
		//i Undefined.
		m_Undefined,
	};

	//c Abstract memory driver. Use 'E_MEMORY_DRIVER__' instead directly this class.
	class memory_driver__
	{
	private:
		mdr::size__ &_Extent;
#ifdef MDR_DBG
		void _Test(
			row_t__ Position,
			size__ Amount ) const
		{
			if ( Position >= _Extent )
				if ( Amount > 0 )
					ERRu();

			if ( ( Position + Amount ) > _Extent )
				ERRu();
		}
#endif
	protected:
		//v Recall 'Amount' at position 'Position' and put them in 'Buffer'.
		virtual void MDRRecall(
			row_t__ Position,
			size__ Amount,
			datum__ *Buffer ) = 0;
		//v Write 'Amount' bytes from 'Buffer' to memory at position 'Position'.
		virtual void MDRStore(
			const datum__ *Buffer,
			size__ Amount,
			row_t__ Position )
		{
			ERRu();
			// For read-only memory.
		}
		//v Alloue 'Size' octet.
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
		datum__ *_Pointer;	// Only for the conventional memory driver.
	public:
		memory_driver__( mdr::size__ &Extent )
		: _Extent( Extent )
		{
			reset( false );
		}
		virtual ~memory_driver__( void )	// to be sure that the destructor of dervaed classes are call.
		{
			reset( true );
		}
		void reset( bool = true )
		{
			_Extent = 0;
			_Pointer = NULL;
		}
		//f Initialization.
		void Init( void )
		{
			reset();
		}
		//f Recall 'Amount' at position 'Position' and put them into 'Buffer'. Return 'Buffer'.
		void Recall(
			row_t__ Position,
			size__ Amount,
			datum__ *Buffer )
		{
#ifdef MDR_DBG
			_Test( Position, Amount );
#endif
			if ( _Pointer != NULL )
				memcpy( Buffer, _Pointer + Position, Amount );
			else
				MDRRecall( Position, Amount, Buffer );
		}
		//f Store 'Amount' bytes from 'Buffer' at position 'Position'.
		void Store(
			const datum__ *Buffer,
			size__ Amount,
			row_t__ Position )
		{
#ifdef MDR_DBG
			_Test( Position, Amount );
#endif
			if ( _Pointer != NULL )
				memcpy( _Pointer + Position, Buffer, Amount );
			else
				MDRStore( Buffer, Amount, Position );
		}
		//f Allocate 'Size' bytes in memory.
		void Allocate( size__ Size )
		{
			MDRAllocate( Size );
			_Extent = Size;
		}
		//f Flush buffers.
		void Flush( void )
		{
			if ( _Pointer == NULL )
				MDRFlush();
		}
	};

/*
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
*/

	#define E_MEMORY_DRIVER__	memory_driver__

	template <typename md> class standalone_memory_driver__
	: public md
	{
	private:
		mdr::size__ _Extent;
	public:
		standalone_memory_driver__( void )
		: md( _Extent )
		{}
	};

	#define E_STANDALONE_MEMORY_DRIVER__( md ) standalone_memory_driver__<md>
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
