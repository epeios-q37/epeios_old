/*
  Header for the 'dtr' library by Claude L. Simon (simon@epeios.org)
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

#ifndef DTR__INC
#define DTR__INC

#define DTR_NAME		"DTR"

#define	DTR_VERSION	"$Revision$"	

#define DTR_OWNER		"the Epeios project (http://www.epeios.org/)"

#include "ttr.h"

extern class ttr_tutor &DTRTutor;

#if defined( XXX_DBG ) && !defined( DTR_NODBG )
#define DTR_DBG 
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
#include "que.h"
#include "btr.h"


namespace dtr {
	//e Browse status;
	enum browse {
		//i Unknow.
		bUnknow,
		//i Is root.
		bRoot,
		//i Is child
		bChild,
		//i Is brother.
		bBrother,
		//i Is parent (already handled).
		bParent
	};

	//c Browse structure.
	struct browse__
	{
		browse Status;
		tym::row__ P;
		browse__( void )
		{
			Status = bUnknow;
			P = NONE;
		}
		//f Initialization wih 'Root' as root'.
		void Init( tym::row__ Root )
		{
			Status = bRoot;
			P = Root;
		}
	};


	//c A dynamic tree.
	class dynamic_tree_
	{
	public:
		//r A binary tree.
		btr::binary_tree_ Tree;
		//r A queue
		que::queue_ Queue;
		struct s 
		{
			btr::binary_tree_::s Tree;
			que::queue_::s Queue;
		};
		dynamic_tree_( s &S )
		: Tree( S.Tree ),
		  Queue( S.Queue )
		{}
		void reset( bso__bool P = true )
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
			bso__bool Adjust = false )
		{
			Tree.Allocate( Size, Adjust );
			Queue.Allocate( Size, Adjust );
		}
		//f 'First' becomes first of 'Node'.
		void BecomeFirst( 
			tym::row__ First,
			tym::row__ Node )
		{
			if ( Tree.Left( Node ) == NONE )
				Tree.BecomeRight( First, Node );
			else
				Queue.InsertItemBeforeNode( First, Tree.Left( Node ) );

			Tree.BecomeLeft( First, Node );
		}
		//f 'Last' becomes last of 'Node'.
		void BecomeLast( 
			tym::row__ Last,
			tym::row__ Node )
		{
			if ( Tree.Right( Node ) == NONE )
				Tree.BecomeLeft( Last, Node );
			else
				Queue.InsertItemAfterNode( Last, Tree.Right( Node ) );

			Tree.BecomeRight( Last, Node );
		}
		//f 'Previous' becomes node previous to 'Node'.
		void BecomePrevious(
			tym::row__ Previous,
			tym::row__ Node )
		{
			tym::row__ Parent;

			Queue.InsertItemBeforeNode( Previous, Node );

			if ( ( Parent = Tree.Parent( Node ) ) != NONE ) {
				if ( Tree.Left( Parent ) == Node )
					Tree.BecomeLeft( Previous, Parent );
				Tree.ForceParent( Previous, Parent );
			}
		}
		//f 'Next' becomes node next to 'Node'.
		void BecomeNext(
			tym::row__ Next,
			tym::row__ Node )
		{
			tym::row__ Parent;

			Queue.InsertItemAfterNode( Next, Node );

			if ( ( Parent = Tree.Parent( Node ) ) != NONE ) {
				if ( Tree.Right( Parent ) == Node )
					Tree.BecomeRight( Next, Parent );
				Tree.ForceParent( Next, Parent );
			}
		}
	#if 0
		// Swap 'Node1' and 'Node2'. Both nodes must be from the same queue.
		void Swap(
			tym::row__ Node1,
			tym::row__ Node2 )
		{
			tym::row__ Parent;

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
		tym::row__ First( tym::row__ Node ) const
		{
			return Tree.Left( Node );
		}
		//f Return last node of 'Node'.
		tym::row__ Last( tym::row__ Node ) const
		{
			return Tree.Right( Node );
		}
		//f Return node previous to 'Node'.
		tym::row__ Previous( tym::row__ Node ) const
		{
			return Queue.Previous( Node );
		}
		//f Return node next to 'Node'.
		tym::row__ Next( tym::row__ Node ) const
		{
			return Queue.Next( Node );
		}
		//f Return parent of 'Node'.
		tym::row__ Parent( tym::row__ Node ) const
		{
			return Tree.Parent( Node );
		}
		/*f Return the first, then next node, or 'NONE' if no more,
		using 'BrowseStruct'. In 'Status' is put the browse status
		with the previous node.*/
		tym::row__ Browse(
			browse__ &BrowseStruct,
			browse &BrowseStatus ) const
		{
			tym::row__ P = BrowseStruct.P;
			BrowseStatus = BrowseStruct.Status;

			if ( P != NONE ) {
				if ( ( BrowseStruct.Status != dtr::bParent ) )
					if ( First( P ) != NONE ) {
						BrowseStruct.Status = dtr::bChild;
						BrowseStruct.P = First( P );
					}

				if ( P == BrowseStruct.P )
					if ( Next( P ) != NONE ) {
						BrowseStruct.Status = dtr::bBrother;
						BrowseStruct.P = Next( P );
					} else  if ( ( BrowseStruct.P = Parent( P ) ) != NONE )
						BrowseStruct.Status = dtr::bParent;
					else
						BrowseStruct.Status = dtr::bUnknow;
			}

			return P;
		}
	};

	AUTO( dynamic_tree )
	
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
