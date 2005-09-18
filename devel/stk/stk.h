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
#include "ctn.h"

namespace stk {

	//c Stack of static objects of type 't'. Use 'STACK_( t )' rather then directly this class.
	template <typename structure, typename item, typename row> class stack_
	: public structure
	{
	public:
		struct s
		: public structure::s
		{};
		stack_( s &S )
		: structure( S )
		{}
		void reset( bool P = true )
		{
			structure::reset( P );
		}
		void plug( mdr::E_MEMORY_DRIVER_ &MDriver )
		{
			structure::plug( MDriver );
		}
		void plug( mmm::multimemory_ &M )
		{
			structure::plug( M );
		}
		stack_ &operator =( const stack_ &S )
		{
			structure::operator =( S );

			return *this;
		}
		//f Initialization.
		void Init( void )
		{
			structure::Init();
			structure::SetNoDecreasingState( true );
		}
		//f Place 'Object' at the top of the stack. Return the position where this object is put.
		row Push( const item &Object )
		{
			return structure::Append( Object );
		}
		//f Remove the object at row 'R'. If 'Adjust' at 'true', than adjust the size of the stack.
		void Remove(
			row Row,
			aem::mode Mode = aem::mDefault )
		{
			structure::Remove( Row );
		}
		//f Return and remove the object at the bottom of the stack. If 'Adjust' at 'true', than adjust the size of the stack.
		void Pop( item &Item )
		{
			structure::Recall( structure::Last(), Item );

			Remove( structure::Last() );
		}
		//f Return 'true' if 'Object' exists in the stack, false otherwise.
		bso::bool__ Exists( const item &Object ) const
		{
			return structure::Search( Object, 0, structure::Amount() ) != NONE;
		}
		//f Return true if an entry exists for row 'Row'.
		bso::bool__ Exists( row Row ) const
		{
			return structure::Exists( Row );
		}
		//f Return the value stord on top of the stack.
		void Top( item &Item ) const
		{
#ifdef STK_DBG
			if ( structure::Amount() == 0 )
				ERRl();
#endif
			structure::Recall( structure::Last(), Item );
		}
	};

	E_AUTO3( stack )

	template <typename item, typename row> class bstack_
	: public stack_< bch::bunch_< item, row >, item, row >
	{
	public:
		struct s
		: public stack_< bch::bunch_< item, row >, item, row >::s
		{};
		bstack_( s &S )
		: stack_< bch::bunch_< item, row >, item, row >( S )
		{}
		void Pop( item &Item )
		{
			stack_< bch::bunch_< item, row >, item, row >::	Pop( Item );
		}
		item Pop( void )
		{
			item Item;

			Pop( Item );

			return Item;
		}
		void Top( item &Item ) const
		{
			stack_< bch::bunch_< item, row >, item, row >::Top( Item );
		}
		item Top( void ) const
		{
			item Item;

			Top( Item );

			return Item;
		}
	};

	E_AUTO2( bstack )

	//m A stack of static object of type 't'.
	#define E_BSTACKt_( item, row )	bstack_< item, row >
	#define E_BSTACKt( item, row )	bstack< item, row >

	E_ROW( row__ );

	#define E_BSTACK_( item )	E_BSTACKt_( item, stk::row__ )
	#define E_BSTACK( item )	E_BSTACKt( item, stk::row__ )

	#define E_XMCSTACKt_( item, row )	stack_< ctn::E_XMCONTAINERt_( item, row ), item, row >
	#define E_XMCSTACKt( item, row )	stack< ctn::E_XMCONTAINERt_( item, row ), item, row >

	#define E_XMCSTACK_( item )		E_XMCSTACKt_( item, stk::row__ )
	#define E_XMCSTACK( item )		E_XMCSTACKt( item, stk::row__ )

}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
