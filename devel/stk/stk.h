/*
  Header for the 'stk' library by Claude L. Simon (simon@epeios.org)
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

#ifndef STK__INC
#define STK__INC

#define STK_NAME		"STK"

#define	STK_VERSION	"$Revision$"	

#define STK_OWNER		"the Epeios project (http://www.epeios.org/)"

#include "ttr.h"

extern class ttr_tutor &STKTutor;

#if defined( XXX_DBG ) && !defined( STK_NODBG )
#define STK_DBG 
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
#include "tym.h"
#include "aem.h"

namespace stk {

	using tym::memory_;

	//c Stack of static objects of type 't'. Use 'STACK_( t )' rather then directly this class.
	template <class t> class stack_
	: public tym::E_MEMORY_( t ),
	  public aem::amount_extent_manager_
	{
	public:
		struct s
		: public tym::E_MEMORY_( t )::s,
		  public aem::amount_extent_manager_::s
		{};
		stack_( s &S )
		: tym::E_MEMORY_( t )( S ),
		  aem::amount_extent_manager_( S )
		{}
		void reset( bool P = true )
		{
			E_MEMORY_( t )::reset( P );
			amount_extent_manager_::reset( P );
		}
		void plug( mdr::E_MEMORY_DRIVER_ &MDriver )
		{
			E_MEMORY_( t )::plug( MDriver );
		}
		void plug( mmm::multimemory_ &M )
		{
			E_MEMORY_( t )::plug( M );
		}
		stack_ &operator =( const stack_ &S )
		{
			amount_extent_manager_::operator =( S );
			E_MEMORY_( t )::Allocate( S.Amount() );
			E_MEMORY_( t )::Write( S, S.Amount() );

			return *this;
		}
		//f Initialization.
		void Init( void )
		{
			E_MEMORY_( t )::Init();
			amount_extent_manager_::Init();
			amount_extent_manager_::SetNoDecreasingState( true );
		}
		//f Place 'Object' at the top of the stack. Return the position where this object is put.
		tym::row__ Push( t Object )
		{
			tym::size__ Size = Amount() + 1;

			if ( amount_extent_manager_::AmountToAllocate( Size, aem::mDefault ) )
				E_MEMORY_( t )::Allocate( Size );

			E_MEMORY_( t )::Write( Object, Amount() - 1 );

			return Amount() - 1;

		}
		//f Return and remove the object at the bottom of the stack. If 'Adjust' at 'true', than adjust the size of the stack.
		t Pop( aem::mode Mode = aem::mDefault )
		{
			tym::size__ Size = Amount() - 1;

			t Objet = E_MEMORY_( t )::Read( Size );

			if ( amount_extent_manager_::AmountToAllocate( Size, Mode ) )
				E_MEMORY_( t )::Allocate( Size );

			return Objet;
		}
	};

	AUTO1( stack )

	//m A stack of static object of type 't'.
	#define E_STACK_( t )	stack_< t >

	#define E_STACK( t )	stack< t >
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
