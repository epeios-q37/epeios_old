/*
	Header for the 'btf' library by Claude SIMON (csimon@epeios.org)
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

#ifndef BTF__INC
#define BTF__INC

#define BTF_NAME		"BTF"

#define	BTF_VERSION	"$Revision$"

#define BTF_OWNER		"Claude SIMON (csimon@epeios.org)"

#include "ttr.h"

extern class ttr_tutor &BTFTutor;

#if defined( XXX_DBG ) && !defined( BTF_NODBG )
#define BTF_DBG
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
//D Binary Tree Filler 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "btr.h"
#include "stk.h"

namespace btf {
	using stk::stack_;

	E_ROW( row__ );

	//c Binary tree_filler.
	template <typename r> class binary_tree_filler_
	: public E_STACKt_( r, row__ )
	{
	private:
		btr::E_BTREEt_( r ) *Tree_;
	public:
		struct s
		: E_STACKt_( r, row__ )::s
		{};
		binary_tree_filler_( s &S )
		: E_STACKt_( r, row__ )( S )
		{}
		void reset( bso::bool__ P = true )
		{
			Tree_ = NULL;
			E_STACKt_( r, row__ )::reset( P );
		}
		void plug( mdr::E_MEMORY_DRIVER_ &MD )
		{
			E_STACKt_( r, row__ )::plug( MD );
		}
		void plug( mmm::E_MULTIMEMORY_ &MM )
		{
			E_STACKt_( r, row__ )::plug( MM );
		}
		binary_tree_filler_ &operator =( const binary_tree_filler_ &BTF )
		{
			E_STACKt_( r, row__ )::operator =( BTF );

			return *this;
		}
		//f Initialization with binary tree 'Tree' of root (if not empty) 'Root'.
		void Init(
			btr::E_BTREEt_( r ) &Tree,
			r Root )
		{
			Tree_ = &Tree;
			E_STACKt_( r, row__ )::Init();

			if ( Root != NONE )
				E_STACKt_( r, row__ )::Push( Root );
		}
		//f Join the 2 nodes at bottom of stack with 'Node', which is pushed in stack.
		void Join( r Node )
		{
#ifdef BTF_DBG
			if ( E_STACKt_( r, row__ )::Amount() < 2 )
				ERRu();
#endif
			Tree_->BecomeRight( E_STACKt_( r, row__ )::Pop(), Node );
			Tree_->BecomeLeft( E_STACKt_( r, row__ )::Pop(), Node );

			E_STACKt_( r, row__ )::Push( Node );
		}
		//f Return true if the tree is complete.
		bso::bool__ IsComplete( void ) const
		{
			return E_STACKt_( r, row__ )::Amount() == 1;
		}
		//f Return the root of the tree. Significant only if 'IsComplete()' return true.
		r GetRoot( void ) const
		{
#ifdef BTF_DBG
			if ( !IsComplete() )
				ERRu();
#endif
			return E_STACKt_( r, row__ )::Get( E_STACKt_( r, row__ )::First() );
		}
		//f Return true if a call to 'Join' is possible, false otherwise.
		bso::bool__ IsJoinable( void )
		{
			return E_STACKt_( r, row__ )::Amount() >= 2;
		}
		//f Only for the 'NAVt()' macro from 'TOL' library. Don't use.
		bso::bool__ Exists( row__ ) const
		{
			ERRu();

			return false;	// Only to avoid a warning.
		}
	};

	E_AUTO1( binary_tree_filler )

	#define E_BTREE_FILLERt_( t )	binary_tree_filler_< t >
	#define E_BTREE_FILLERt( t )	binary_tree_filler< t >

	#define E_BTREE_FILLER_	E_BTREE_FILLERt_( epeios::row__ )
	#define E_BTREE_FILLER	E_BTREE_FILLERt( epeios::row__ )

}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
