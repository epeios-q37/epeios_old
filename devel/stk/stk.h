/*
	Header for the 'stk' library by Claude SIMON (csimon@epeios.org)
	Copyright (C) $COPYRIGHT_DATES$Claude SIMON (csimon@epeios.org).
$_RAW_$
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
#include "bch.h"

namespace stk {

	using bch::bunch_;

	//c Stack of static objects of type 't'. Use 'STACK_( t )' rather then directly this class.
	template <class t, typename r> class stack_
	: public bch::E_BUNCHt_( t, r )
	{
	public:
		struct s
		: public bch::E_BUNCHt_( t, r )::s
		{};
		stack_( s &S )
		: bch::E_BUNCHt_( t, r )( S )
		{}
		void reset( bool P = true )
		{
			E_BUNCHt_( t, r )::reset( P );
		}
		void plug( mdr::E_MEMORY_DRIVER_ &MDriver )
		{
			E_BUNCHt_( t, r )::plug( MDriver );
		}
		void plug( mmm::multimemory_ &M )
		{
			E_BUNCHt_( t, r )::plug( M );
		}
		stack_ &operator =( const stack_ &S )
		{
			E_BUNCHt_( t, r )::operator =( S );

			return *this;
		}
		//f Initialization.
		void Init( void )
		{
			E_BUNCHt_( t, r )::Init();
			E_BUNCHt_( t, r )::SetNoDecreasingState( true );
		}
		//f Place 'Object' at the top of the stack. Return the position where this object is put.
		r Push( t Object )
		{
			return E_BUNCHt_( t, r )::Append( Object );
		}
		//f Remove the object at row 'R'. If 'Adjust' at 'true', than adjust the size of the stack.
		void Remove(
			r Row,
			aem::mode Mode = aem::mDefault )
		{
			E_BUNCHt_( t, r )::Delete( Row );
		}
		//f Return and remove the object at the bottom of the stack. If 'Adjust' at 'true', than adjust the size of the stack.
		t Pop( aem::mode Mode = aem::mDefault )
		{
			t Objet = E_BUNCHt_( t, r )::Get( E_BUNCHt_( t, r )::Last() );

			Remove( E_BUNCHt_( t, r )::Last() );
		
			return Objet;
		}
		//f Return 'true' if 'Object' exists in the stack, false otherwise.
		bso::bool__ Exists( t Object ) const
		{
			return E_BUNCHt_( t, r )::Search( Object, 0, Amount() ) != NONE;
		}
		//f Return true if an entry exists for row 'Row'.
		bso::bool__ Exists( r Row ) const
		{
			return E_BUNCHt_( t, r )::Exists( Row );
		}
		//f Return the value stord on top of the stack.
		t Top( void ) const
		{
#ifdef STK_DBG
			if ( Amount() == 0 )
				ERRl();
#endif
			return E_BUNCHt_( t, r )::Get( Last() );
		}
	};

	E_AUTO2( stack )

	//m A stack of static object of type 't'.
	#define E_STACKt_( t, r )	stack_< t, r >
	#define E_STACKt( t, r )	stack< t, r >

	E_ROW( row__ );

	#define E_STACK( t )	E_STACKt( t, stk::row__ )
	#define E_STACK_( t )	E_STACKt_( t, stk::row__ )

}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
