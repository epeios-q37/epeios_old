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

namespace cvm {

	namespace {
		using namespace mdr;
	}

	//c Basic conventional memory.
	class basic_conventional_memory
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
			row__ Position,
			size__ Amount,
			data__ *Buffer )
		{
	#ifdef CVM_DBG
			if ( Amount )
				Test_();
	#endif
			memcpy( Buffer, Tampon_ + Position, Amount );
		}
		// écrit 'Nombre' octets à partir de 'Position' dans 'Tampon'
		void Write(
			const data__ *Buffer,
			size__ Amount,
			row__ Position )
		{
	#ifdef CVM_DBG
			if (Amount ) 
				Test_();
	#endif
			memcpy( Tampon_ + Position, Buffer, Amount );
		}
		// alloue 'Nombre' octets
		void Allocate( size__ Size )
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
		void plug( class mmm::multimemory_ & ){}	// standardization
		basic_conventional_memory( void )
		{
			reset( false );
		}
		virtual ~basic_conventional_memory( void )
		{
			reset( true );
		}
		//f Initialization.
		void Init( void ){}
	};

	//c Conventional memory driver.
	class conventional_memory_driver
	: public mdr::E_MEMORY_DRIVER,
	  public basic_conventional_memory
	{
	protected:
		virtual void MDRRead(
			row__ Position,
			bsize__ Amount,
			data__ *Buffer )
		{
			basic_conventional_memory::Read( Position, Amount, Buffer );
		}
		// écrit 'Nombre' octets à la position 'Position'
		virtual void MDRWrite(
			const data__ *Buffer,
			bsize__ Nombre,
			row__ Position )
		{
			basic_conventional_memory::Write( Buffer, Nombre, Position );
		}
		// alloue 'Taille' octets
		virtual void MDRAllocate( size__ Size )
		{
			basic_conventional_memory::Allocate( Size );
		}
	public:
		void reset( bool P = true )
		{
			basic_conventional_memory::reset( P );
			E_MEMORY_DRIVER::reset( P );
		}
		void plug( mmm::multimemory_ &M )
		{
			basic_conventional_memory::plug( M );
			E_MEMORY_DRIVER::plug( M );
		}
		conventional_memory_driver( void )
		{
			reset( false );
		}
		virtual ~conventional_memory_driver( void )
		{
			reset( true );
		}
		//f Initialization.
		void Init( void )
		{
			basic_conventional_memory::Init();
			E_MEMORY_DRIVER::Init();
		}
	};
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
