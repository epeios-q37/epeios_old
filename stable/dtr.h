/*
	Header for the 'dtr' library by Claude SIMON (csimon@epeios.org)
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

#ifndef DTR__INC
#define DTR__INC

#define DTR_NAME		"DTR"

#define	DTR_VERSION	"$Revision$"

#define DTR_OWNER		"Claude SIMON (csimon@epeios.org)"

#include "ttr.h"

extern class ttr_tutor &DTRTutor;

#if defined( XXX_DBG ) && !defined( DTR_NODBG )
#define DTR_DBG
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
//D Dynamic TRee 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "que.h"
#include "btr.h"


namespace dtr {
	//e Browse status;
	enum browse {
		//i Undefined status.
		bUndefined,
		//i First element (always the root node).
		bFirst,
		/*i Last element, unless given root node have no child. So don't trust
		this item to stop browsing. */
		bLast,
		//i Is child
		bChild,
		//i Is brother.
		bBrother,
		//i Is parent (already handled).
		bParent
	};
	
	template <typename r> class dynamic_tree_;	// Predeclaration.


	//c Browse structure.
	template <typename r> struct browser__
	{
	private:
		browse Status_;
		r Position_;
		r Root_;
	public:
		browser__( void )
		{
			Status_ = bUndefined;
			Position_ = NONE;
			Root_ = NONE;
		}
		//f Initialization wih 'Root' as root.
		void Init( r Root )
		{
			Status_ = bFirst;
			Position_ = Root_ = Root;
		}
		//f Return current position.
		r Position( void )
		{
			return Position_;
		}
		//r Return root.
		r Root( void )
		{
			return Root_;
		}
		//f Return status.
		browse Status( void )
		{
			return Status_;
		}
		friend class dynamic_tree_<r>;
	};

	//c A dynamic tree.
	template <typename r> class dynamic_tree_
	{
	public:
		//r A binary tree.
		btr::E_BTREEt_( r ) Tree;
		//r A queue
		que::E_QUEUEt_( r ) Queue;
		struct s 
		{
			typename btr::E_BTREEt_( r )::s Tree;
			typename que::E_QUEUEt_( r )::s Queue;
		};
		dynamic_tree_( s &S )
		: Tree( S.Tree ),
		  Queue( S.Queue )
		{}
		void reset( bso::bool__ P = true )
		{
			Tree.reset( P );
			Queue.reset( P );
		}
		void plug( mmm::multimemory_ &M )
		{
			Tree.plug( M );
			Queue.plug( M );
		}
		dynamic_tree_ &operator =( const dynamic_tree_ &T )
		{
			Tree = T.Tree;
			Queue = T.Queue;

			return *this;
		}
		//f Initialization.
		void Init( void )
		{
			Tree.Init();
			Queue.Init();
		}
		//f Allocate place nedeed for 'Size' nodes.
		void Allocate(
			tym::size__ Size,
			aem::mode Mode = aem::mDefault )
		{
			Tree.Allocate( Size, Mode );
			Queue.Allocate( Size, Mode );
		}
		//f 'First' becomes first of 'Node'.
		void BecomeFirst( 
			r First,
			r Node )
		{
			if ( Tree.Left( Node ) == NONE )
				Tree.BecomeRight( First, Node );
			else
				Queue.BecomePrevious( First, Tree.Left( Node ) );

			Tree.BecomeLeft( First, Node );
		}
		//f 'Last' becomes last of 'Node'.
		void BecomeLast( 
			r Last,
			r Node )
		{
			if ( Tree.Right( Node ) == NONE )
				Tree.BecomeLeft( Last, Node );
			else
				Queue.BecomeNext( Last, Tree.Right( Node ) );

			Tree.BecomeRight( Last, Node );
		}
		//f 'Previous' becomes node previous to 'Node'.
		void BecomePrevious(
			r Previous,
			r Node )
		{
			r Parent;

			Queue.BecomePrevious( Previous, Node );

			if ( ( Parent = Tree.Parent( Node ) ) != NONE ) {
				if ( Tree.Left( Parent ) == Node )
					Tree.BecomeLeft( Previous, Parent );
				Tree.ForceParent( Previous, Parent );
			}
		}
		//f 'Next' becomes node next to 'Node'.
		void BecomeNext(
			r Next,
			r Node )
		{
			r Parent;

			Queue.BecomeNext( Next, Node );

			if ( ( Parent = Tree.Parent( Node ) ) != NONE ) {
				if ( Tree.Right( Parent ) == Node )
					Tree.BecomeRight( Next, Parent );
				Tree.ForceParent( Next, Parent );
			}
		}
	#if 0
		// Swap 'Node1' and 'Node2'. Both nodes must be from the same queue.
		void Swap(
			r Node1,
			r Node2 )
		{
			r Parent;

			Queue.Swap( Node1, Node2 );

			if ( ( Parent = Tree.Parent( Node1 ) ) != NONE ) {
				if ( Tree.Left( Parent ) == Node1 )
					Tree.BecomeLeft( Node2, Parent );
				else if ( Tree.Left( Parent ) == Node2 )
					Tree.BecomeLeft( Node1, Parent );

				if ( Tree.Right( Parent ) == Node1 )
					Tree.BecomeRight( Node2, Parent );
				else if ( Tree.Right( Parent ) == Node2 )
					Tree.BecomeRight( Node1, Parent );
			}
		}
	#endif
		//f Return first node of 'Node'.
		r First( r Node ) const
		{
			return Tree.Left( Node );
		}
		//f Return last node of 'Node'.
		r Last( r Node ) const
		{
			return Tree.Right( Node );
		}
		//f Return node previous to 'Node'.
		r Previous( r Node ) const
		{
			return Queue.Previous( Node );
		}
		//f Return node next to 'Node'.
		r Next( r Node ) const
		{
			return Queue.Next( Node );
		}
		//f Return parent of 'Node'.
		r Parent( r Node ) const
		{
			return Tree.Parent( Node );
		}
		//f Return amount of nodes.
		epeios::size__ Amount( void ) const
		{
			return Tree.Amount();
		}
		/*f Return the first, then next node, or 'NONE' if no more,
		using 'BrowseStruct'. */
		r Browse( browser__<r> &Browser ) const
		{
			r P = Browser.Position_;
			
			if ( Browser.Status_ == dtr::bUndefined )
				Browser.Position_ = NONE;
			else if ( ( Browser.Status_ == dtr::bLast )
			 	       && ( Browser.Root_ == NONE ) ) {
				Browser.Position_ = NONE;
				Browser.Status_ = dtr::bUndefined;
			} else if ( P != NONE ) {
				if ( ( Browser.Status_ != dtr::bParent ) )
					if ( First( P ) != NONE ) {
						Browser.Status_ = dtr::bChild;
						Browser.Position_ = First( P );
					}

				if ( P == Browser.Position_ )
					if ( P != Browser.Root_ ) {
						if ( Next( P ) != NONE ) {
							Browser.Status_ = dtr::bBrother;
							Browser.Position_ = Next( P );
						} else  if ( ( Browser.Position_ = Parent( P ) ) != Browser.Root_ )
							Browser.Status_ = dtr::bParent;
						else {
							Browser.Status_ = dtr::bLast;
							Browser.Root_ = NONE;
						}
					} else {
						Browser.Root_ = NONE;
						Browser.Position_ = NONE;
						Browser.Status_ = dtr::bUndefined;
					}
			}

			return Browser.Position_;
		}
	};

	E_AUTO1( dynamic_tree )

	#define E_DTREEt( r )	dynamic_tree< r >
	#define E_DTREEt_( r )	dynamic_tree_< r >

	#define E_DTREE		E_DTREEt( epeios::row__ )
	#define E_DTREE_	E_DTREEt_( epeios::row__ )
	
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
