/*
	Header for the 'stk' library by Claude SIMON (csimon@epeios.org)
	Copyright (C) 2000-2003 Claude SIMON (csimon@epeios.org).

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

#ifndef STK__INC
#define STK__INC

#define STK_NAME		"STK"

#define	STK_VERSION	"$Revision$"

#define STK_OWNER		"Claude SIMON (csimon@epeios.org)"

#include "ttr.h"

extern class ttr_tutor &STKTutor;

#if defined( XXX_DBG ) && !defined( STK_NODBG )
#define STK_DBG
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
//D STacK 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "tym.h"
#include "aem.h"

namespace stk {

	using tym::memory_;
	using aem::amount_extent_manager_;

	//c Stack of static objects of type 't'. Use 'STACK_( t )' rather then directly this class.
	template <class t, typename r> class stack_
	: public tym::E_MEMORYt_( t, r ),
	  public aem::amount_extent_manager_<r>
	{
	public:
		struct s
		: public tym::E_MEMORYt_( t, r )::s,
		  public aem::amount_extent_manager_<r>::s
		{};
		stack_( s &S )
		: tym::E_MEMORYt_( t, r )( S ),
		  aem::amount_extent_manager_<r>( S )
		{}
		void reset( bool P = true )
		{
			E_MEMORYt_( t, r )::reset( P );
			amount_extent_manager_<r>::reset( P );
		}
		void plug( mdr::E_MEMORY_DRIVER_ &MDriver )
		{
			E_MEMORYt_( t, r )::plug( MDriver );
		}
		void plug( mmm::multimemory_ &M )
		{
			E_MEMORYt_( t, r )::plug( M );
		}
		stack_ &operator =( const stack_ &S )
		{
			amount_extent_manager_<r>::operator =( S );
			E_MEMORYt_( t, r )::Allocate( S.Amount() );
			E_MEMORYt_( t, r )::Store( S, S.Amount() );

			return *this;
		}
		//f Initialization.
		void Init( void )
		{
			E_MEMORYt_( t, r )::Init();
			amount_extent_manager_<r>::Init();
			amount_extent_manager_<r>::SetNoDecreasingState( true );
		}
		//f Place 'Object' at the top of the stack. Return the position where this object is put.
		r Push( t Object )
		{
			tym::size__ Size = Amount() + 1;

			if ( amount_extent_manager_<r>::AmountToAllocate( Size, aem::mDefault ) )
				E_MEMORYt_( t, r )::Allocate( Size );

			E_MEMORYt_( t, r )::Store( Object, Amount() - 1 );

			return Amount() - 1;

		}
		//f Return and remove the object at the bottom of the stack. If 'Adjust' at 'true', than adjust the size of the stack.
		t Pop( aem::mode Mode = aem::mDefault )
		{
			tym::size__ Size = Amount() - 1;

			t Objet = E_MEMORYt_( t, r )::Get( Size );

			if ( amount_extent_manager_<r>::AmountToAllocate( Size, Mode ) )
				E_MEMORYt_( t, r )::Allocate( Size );

			return Objet;
		}
		//f Return 'true' if 'Object' exists in the stack, false otherwise.
		bso::bool__ Exists( t Object ) const
		{
			if ( Amount() )
				return E_MEMORYt_( t, r )::Locate( Object, 0, Amount() ) != NONE;
			else
				return false;
		}
		//f Return the value stord on top of the stack.
		t Top( void ) const
		{
#ifdef STK_DBG
			if ( Amount() == 0 )
				ERRl();
#endif
			return E_MEMORYt_( t, r )::Get( Last() );
		}
		E_NAVt( amount_extent_manager_<r>::, r ) 
	};

	E_AUTO2( stack )

	//m A stack of static object of type 't'.
	#define E_STACKt_( t, r )	stack_< t, r >
	#define E_STACKt( t, r )	stack< t, r >

	#define E_STACK( t )	E_STACKt( t, epeios::row__ )
	#define E_STACK_( t )	E_STACKt_( t, epeios::row__ )

}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
