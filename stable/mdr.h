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

#include "err.h"
#include "flw.h"
#include "bso.h"

//t The position in a memory.
typedef bso__ulong	mdr__position;

//t The size of a memory.
typedef bso__ulong	mdr__size;

//t The size of a buffer.
typedef bso__bsize	mdr__bsize;

//t The type of the data in a memory.
typedef bso__raw	mdr__data;

struct mdr
{
	// How the memory is used.
	enum mode
	{
		// Read only
		mReadOnly,
		// Read/write
		mReadWrite
	};
};


//c Basic memory driver. Use 'MEMORY_DRIVER_' instead directly this class.
class mdr_basic_memory_driver_
{
protected:
	//v Place 'Amount' bytes in 'Buffer' from the memory at position 'Position'.
	virtual void MDRRead(
		mdr__position Position,
		mdr__bsize Amount,
		mdr__data *Buffer ) = 0;
	//v Write 'Amount' bytes from 'Buffer' to memory at position 'Position'.
	virtual void MDRWrite(
		const mdr__data *Buffer,
		mdr__bsize Amount,
		mdr__position Position )
	{
		ERRu();
		// For read-only memory.
	}
	//v Allocate 'Capacity' bytes in memory.
	virtual void MDRAllocate( mdr__size Size )
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
	mdr_basic_memory_driver_( s &S )
	{
		reset( true );
	}
	virtual ~mdr_basic_memory_driver_( void )	// to be sure that the destructor of dervaed classes are call.
	{
		reset( false );
	}
	void reset( bool = true )
	{}
	void plug( class mmm_multimemory_ & )
	{
		// For standardization.
	}
	//f Initialization.
	void Init( void )
	{}
	//f Place 'Amount' bytes in 'Buffer' from the memory at position 'Position'.
	void Read(
		mdr__position Position,
		mdr__bsize Amount,
		mdr__data *Buffer )
	{
		MDRRead( Position, Amount, Buffer );
	}
	//f Write 'Amount' bytes from 'Buffer' to memory at position 'Position'.
	void Write(
		const mdr__data *Buffer,
		mdr__bsize Amount,
		mdr__position Position )
	{
		MDRWrite( Buffer, Amount, Position );
	}
	//f Allocate 'Size' bytes in memory.
	void Allocate( mdr__size Size )
	{
		MDRAllocate( Size );
	}
	//f Flush caches.
	void Synchronize( void )
	{
		MDRSynchronize();
	}
};

class mdr_basic_memory_driver
: public mdr_basic_memory_driver_
{
public:
	mdr_basic_memory_driver_::s static_;
	mdr_basic_memory_driver( void )
	: mdr_basic_memory_driver_( static_ )
	{
		mdr_basic_memory_driver_::reset( false );
	}
	virtual ~mdr_basic_memory_driver( void )
	{
		mdr_basic_memory_driver_::reset( true );
	}
};


//d Because it is often used in the Epeios project, this is a shorter name for 'mdr_basic_memory_driver_'.
#define MEMORY_DRIVER_	mdr_basic_memory_driver_
#define MEMORY_DRIVER	mdr_basic_memory_driver

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
/* end of 'xxx.h' template file */