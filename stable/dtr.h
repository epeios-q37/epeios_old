/*
	Header for the 'dtr' library by Claude SIMON (csimon@epeios.org)
	Copyright (C) 2000-2004 Claude SIMON (csimon@epeios.org).

	This file is part of the Epeios (http://epeios.org/) project.

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 3
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
	enum kinship__ {
		kChild,
		kSibling,
		kParent,
		k_amount,
		k_Undefined,
	};
	
	template <typename r> class dynamic_tree_;	// Predeclaration.


	//c Browse structure.
	template <typename r> struct browser__
	{
	private:
		kinship__ _Kinship;
		r _Position;
		r _Root;
	public:
		browser__( void )
		{
			_Kinship = k_Undefined;
			_Position = NONE;
			_Root = NONE;
		}
		//f Initialization wih 'Root' as root.
		void Init( r Root )
		{
			_Kinship = k_Undefined;
			_Position = _Root = Root;
		}
		E_RODISCLOSE__( kinship__, Kinship )
		E_RODISCLOSE__( r, Position )
		E_RODISCLOSE__( r, Root )
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
			epeios::size__ Size,
			aem::mode__ Mode = aem::mDefault )
		{
			Tree.Allocate( Size, Mode );
			Queue.Allocate( Size, Mode );
		}
		//f 'First' becomes first of 'Node'.
		void BecomeFirstSibling( 
			r First,
			r Node )
		{
			if ( Tree.Left( Node ) == NONE )
				Tree.BecomeOverridingRight( First, Node );
			else
				Queue.BecomePrevious( First, Tree.Left( Node ) );

			Tree.BecomeOverridingLeft( First, Node );
		}
		//f 'Last' becomes last of 'Node'.
		void BecomeLast( 
			r Last,
			r Node )
		{
			if ( Tree.Right( Node ) == NONE )
				Tree.BecomeOverridingLeft( Last, Node );
			else
				Queue.BecomeNext( Last, Tree.Right( Node ) );

			Tree.BecomeOverridingRight( Last, Node );
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
		r FirstChild( r Node ) const
		{
			return Tree.Left( Node );
		}
		//f Return last node of 'Node'.
		r LastChild( r Node ) const
		{
			return Tree.Right( Node );
		}
		bso::bool__ HasChild( r Node ) const
		{
			return FirstChild( Node ) != NONE;
		}
		//f Return node previous to 'Node'.
		r PreviousSibling( r Node ) const
		{
			return Queue.Previous( Node );
		}
		bso::bool__ HasPreviousSibling( r Node ) const
		{
			return PreviousSibling( Node ) != NONE;
		}
		//f Return node next to 'Node'.
		r NextSibling( r Node ) const
		{
			return Queue.Next( Node );
		}
		bso::bool__ HasNextSibling( r Node ) const
		{
			return NextSibling( Node ) != NONE;
		}
		//f Return parent of 'Node'.
		r Parent( r Node ) const
		{
			return Tree.Parent( Node );
		}
		r Cut( r Node )
		{
			r P = Tree.Parent( Node );
			r L = Queue.Previous( Node ), R = Queue.Next( Node );

			Tree.Cut( Node );

			if ( P != NONE ) {
				if ( L == NONE ) {
					if ( R != NONE )
						Tree.BecomeLeft( R, P );
				}

				if ( R == NONE ) {
					if ( L != NONE )
						Tree.BecomeRight( L, P );
				}
			}

			Queue.Delete( Node );

			return Node;
		}
		bso::bool__ HasParent( r Node ) const
		{
			return Parent( Node ) != NONE;
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
			kinship__ &Kinship = Browser._Kinship;
			r &Position = Browser._Position, &Root = Browser._Root;

			switch ( Kinship ) {
			case k_Undefined:
				if ( Root == NONE )
					ERRu();

				if ( HasChild( Position ) ) {
					Position = FirstChild( Position );
					Kinship = kChild;
				}
				break;
			case kChild:
			case kSibling:
				if ( HasChild( Position ) ) {
					Position = FirstChild( Position );
					Kinship = kChild;
				} else if ( HasNextSibling( Position ) ) {
					Position = NextSibling( Position );
					Kinship = kSibling;
#ifdef DTR_DBG
				} else if ( !HasParent( Position ) ) {
					ERRc();
#endif
				} else {
					Position = Parent( Position );
					Kinship = kParent;
				}
				break;
			case kParent:
				if ( HasNextSibling( Position ) ) {
					Position = NextSibling( Position );
					Kinship = kSibling;
#ifdef DTR_DBG
				} else if ( !HasParent( Position ) ) {
					ERRc();
#endif
				} else {
					Position = Parent( Position );
					Kinship = kParent;
				}
				break;
			default:
				ERRu();
				break;
			}

			if ( Position == Root ) {
				Position = NONE;
				Kinship = k_Undefined;
			}


			return Position;
		}
		bso::ulong__ GetLevel( r Row ) const
		{
			bso::ulong__ Level = 0;

			while ( HasParent( Row ) ) {
				Level++;
				Row = Parent( Row );
			}

			return Level;
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
