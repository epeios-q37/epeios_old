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
	1.0.4 16/03/2000 07:04:39

Instigators:
	Claude SIMON

Authors:
	Claude SIMON

Contributors:
	/

Coordinator:
	Claude SIMON

Description:
	ConVentional Memory
*/

/* Begin of automatic documentation generation part. */

//V 1.0.4
//C Claude SIMON
//D ConVentional Memory
//R 16/03/2000 07:04:39

/* End of automatic documentation generation part. */

/* end of 'entete.h' template file */
/* begin of 'xxx.h' template file V2.6 */

#ifndef CVM__INC
#define CVM__INC

#define CVM_NAME		"CVM"

#define	CVM_VERSION		"1.0.4"	
#define CVM_OWNER		"The Epeios Project (www.epeios.org)"	

//  	Substitution is not applied to the rest when emancipated

#ifdef CVM__LINE
#line 14
#endif

#include "ttr.h"

extern class ttr_tutor &CVMTutor;

#if defined( XXX_DBG ) && !defined( CVM_NODBG )
#define CVM_DBG 
#endif

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "mdr.h"

//t Position.
typedef size_t cvm__position;

//t Type of data.
typedef mdr__data cvm__data;

//t Size.
typedef size_t cvm__size;

//c Basic conventional memory.
class cvm_basic_conventional_memory
{
	char *Tampon_;
		// le contenu de la mémoire
#ifdef CVM_DBG
	void Test_( void )
	{
		if ( Tampon_ == NULL )
			ERRu();
	}
#endif
protected:
	// lit à partir de 'Position' et place dans 'Tampon' 'Nombre' octets
	void Read(
		cvm__position Position,
		cvm__size Amount,
		cvm__data *Buffer )
	{
#ifdef CVM_DBG
		if ( Amount )
			Test_();
#endif
		memcpy( Buffer, Tampon_ + Position, Amount );
	}
	// écrit 'Nombre' octets à partir de 'Position' dans 'Tampon'
	void Write(
		const cvm__data *Buffer,
		cvm__size Amount,
		cvm__position Position )
	{
#ifdef CVM_DBG
		if (Amount ) 
			Test_();
#endif
		memcpy( Tampon_ + Position, Buffer, Amount );
	}
	// alloue 'Nombre' octets
	void Allocate( cvm__size Size )
	{
		char *Tampon;

		Tampon = (char *)realloc( Tampon_, Size );

		if ( !Tampon && Size )
			ERRa();

		Tampon_ = Tampon;
	}
public:
	void reset( bool P = true )
	{
		if ( P )
		{
			if ( Tampon_ )
				free( Tampon_ );
		}

		Tampon_ = NULL;
	}
	void plug( class mmm_multimemory_ & ){}	// stadardization
	cvm_basic_conventional_memory( void )
	{
		reset( false );
	}
	virtual ~cvm_basic_conventional_memory( void )
	{
		reset( true );
	}
	//f Initialization.
	void Init( void ){}
};

//c Conventional memory driver.
class cvm_conventional_memory_driver
: public MEMORY_DRIVER,
  public cvm_basic_conventional_memory
{
protected:
	virtual void MDRRead(
		mdr__position Position,
		mdr__bsize Amount,
		mdr__data *Buffer )
	{
		cvm_basic_conventional_memory::Read( Position, Amount, Buffer );
	}
	// écrit 'Nombre' octets à la position 'Position'
	virtual void MDRWrite(
		const mdr__data *Buffer,
		mdr__bsize Nombre,
		mdr__position Position )
	{
		cvm_basic_conventional_memory::Write( Buffer, Nombre, Position );
	}
	// alloue 'Taille' octets
	virtual void MDRAllocate( mdr__size Size )
	{
		cvm_basic_conventional_memory::Allocate( Size );
	}
public:
	void reset( bool P = true )
	{
		cvm_basic_conventional_memory::reset( P );
		MEMORY_DRIVER::reset( P );
	}
	void plug( mmm_multimemory_ &M )
	{
		cvm_basic_conventional_memory::plug( M );
		MEMORY_DRIVER::plug( M );
	}
	cvm_conventional_memory_driver( void )
	{
		reset( false );
	}
	virtual ~cvm_conventional_memory_driver( void )
	{
		reset( true );
	}
	//f Initialization.
	void Init( void )
	{
		cvm_basic_conventional_memory::Init();
		MEMORY_DRIVER::Init();
	}
};




/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
/* end of 'xxx.h' template file */