/*
  Header for the 'cvm' library by Claude L. Simon (simon@epeios.org)
  Copyright (C) 2000,2001 Claude L. SIMON (simon@epeios.org) 

  This file is part of the Epeios (http://www.epeios.org/) project.
  

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

#ifndef CVM__INC
#define CVM__INC

#define CVM_NAME		"CVM"

#define	CVM_VERSION	"$Revision$"	

#define CVM_OWNER		"the Epeios project (http://www.epeios.org/)"

#include "ttr.h"

extern class ttr_tutor &CVMTutor;

#if defined( XXX_DBG ) && !defined( CVM_NODBG )
#define CVM_DBG 
#endif

/* Begin of automatic documentation generation part. */

//V $Revision$
//C Claude L. SIMON (simon@epeios.org)
//R $Date$

/* End of automatic documentation generation part. */

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
