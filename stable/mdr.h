/* begin of 'entete.txt' template file V1.1 */
/* Best viewed with a tab size of 4 */
/*
	This file is part of the Epeios project. For all information
	concerning the Epeios project, this file and its conditions of use,
	consult the site: 

			http://www.epeios.org/
*/
/* end of 'entete.txt' template file */
/* begin of 'entete.h' template file V1.3 */
/*
Version:
	1.0.6 16/03/2000 19:37:41

Instigators:
	Claude SIMON

Authors:
	Claude SIMON

Contributors:
	/

Coordinator:
	Claude SIMON

Description:
	Memory DRiver
*/

/* Begin of automatic documentation generation part. */

//V 1.0.6
//C Claude SIMON
//D Memory DRiver
//R 16/03/2000 19:37:41

/* End of automatic documentation generation part. */

/* end of 'entete.h' template file */
/* begin of 'xxx.h' template file V2.6 */

#ifndef MDR__INC
#define MDR__INC

#define MDR_NAME		"MDR"

#define	MDR_VERSION		"1.0.6"	
#define MDR_OWNER		"The Epeios Project (www.epeios.org)"	

//  	Substitution is not applied to the rest when emancipated

#ifdef MDR__LINE
#line 14
#endif

#include "ttr.h"

extern class ttr_tutor &MDRTutor;

#if defined( XXX_DBG ) && !defined( MDR_NODBG )
#define MDR_DBG 
#endif

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
/* end of 'xxx.h' template file */