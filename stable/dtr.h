/* begin of 'entete.txt' template file V1.1 */
/* Best viewed with a tab size of 4 */
/*
	This file is part of the Epeios project. For all information
	concerning the Epeios project, this file and its conditions of use,
	consult the site: 

			http://www.epeios.org/
*/
/* end of 'entete.txt' template file */
/* begin of 'entete.h' template file V1.3 */
/*
Version:
	1.0.4 09/07/2000 10:40:38

Instigators:
	Claude SIMON

Authors:
	Claude SIMON

Contributors:
	/

Coordinator:
	Claude SIMON

Description:
	Dynamic TRee
*/

/* Begin of automatic documentation generation part. */

//V 1.0.4
//C Claude SIMON
//D Dynamic TRee
//R 09/07/2000 10:40:38

/* End of automatic documentation generation part. */

/* end of 'entete.h' template file */
/* begin of 'xxx.h' template file V2.6 */

#ifndef DTR__INC
#define DTR__INC

#define DTR_NAME		"DTR"

#define	DTR_VERSION		"1.0.4"	
#define DTR_OWNER		"The Epeios Project (www.epeios.org)"	

//  	Substitution is not applied to the rest when emancipated

#ifdef DTR__LINE
#line 14
#endif

#include "ttr.h"

extern class ttr_tutor &DTRTutor;

#if defined( XXX_DBG ) && !defined( DTR_NODBG )
#define DTR_DBG 
#endif

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "que.h"
#include "btr.h"


struct dtr {
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
};


//c Browse structure.
struct dtr__browse
{
	dtr::browse Status;
	POSITION__ P;
	dtr__browse( void )
	{
		Status = dtr::bUnknow;
		P = NONE;
	}
	//f Initialization wih 'Root' as root'.
	void Init( POSITION__ Root )
	{
		Status = dtr::bRoot;
		P = Root;
	}
};


//c A dynamic tree.
class dtr_dynamic_tree_
{
public:
	//r A binary tree.
	btr_binary_tree_ Tree;
	//r A queue
	que_queue_ Queue;
	struct s 
	{
		btr_binary_tree_::s Tree;
		que_queue_::s Queue;
	};
	dtr_dynamic_tree_( s &S )
	: Tree( S.Tree ),
	  Queue( S.Queue )
	{}
	void reset( bso__bool P = true )
	{
		Tree.reset( P );
		Queue.reset( P );
	}
	void plug( mmm_multimemory_ &M )
	{
		Tree.plug( M );
		Queue.plug( M );
	}
	dtr_dynamic_tree_ &operator =( const dtr_dynamic_tree_ &T )
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
	void Allocate( SIZE__ Size )
	{
		Tree.Allocate( Size );
		Queue.Allocate( Size );
	}
	//f 'First' becomes first of 'Node'.
	void BecomeFirst( 
		POSITION__ First,
		POSITION__ Node )
	{
		if ( Tree.Left( Node ) == NONE )
			Tree.BecomeRight( First, Node );
		else
			Queue.InsertItemBeforeNode( First, Tree.Left( Node ) );
	
		Tree.BecomeLeft( First, Node );
	}
	//f 'Last' becomes last of 'Node'.
	void BecomeLast( 
		POSITION__ Last,
		POSITION__ Node )
	{
		if ( Tree.Right( Node ) == NONE )
			Tree.BecomeLeft( Last, Node );
		else
			Queue.InsertItemAfterNode( Last, Tree.Right( Node ) );

		Tree.BecomeRight( Last, Node );
	}
	//f 'Previous' becomes node previous to 'Node'.
	void BecomePrevious(
		POSITION__ Previous,
		POSITION__ Node )
	{
		POSITION__ Parent;

		Queue.InsertItemBeforeNode( Previous, Node );

		if ( ( Parent = Tree.Parent( Node ) ) != NONE ) {
			if ( Tree.Left( Parent ) == Node )
				Tree.BecomeLeft( Previous, Parent );
			Tree.ForceParent( Previous, Parent );
		}
	}
	//f 'Next' becomes node next to 'Node'.
	void BecomeNext(
		POSITION__ Next,
		POSITION__ Node )
	{
		POSITION__ Parent;

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
		POSITION__ Node1,
		POSITION__ Node2 )
	{
		POSITION__ Parent;

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
	POSITION__ First( POSITION__ Node ) const
	{
		return Tree.Left( Node );
	}
	//f Return last node of 'Node'.
	POSITION__ Last( POSITION__ Node ) const
	{
		return Tree.Right( Node );
	}
	//f Return node previous to 'Node'.
	POSITION__ Previous( POSITION__ Node ) const
	{
		return Queue.Previous( Node );
	}
	//f Return node next to 'Node'.
	POSITION__ Next( POSITION__ Node ) const
	{
		return Queue.Next( Node );
	}
	//f Return parent of 'Node'.
	POSITION__ Parent( POSITION__ Node ) const
	{
		return Tree.Parent( Node );
	}
	/*f Return the first, then next node, or 'NONE' if no more,
	using 'BrowseStruct'. In 'Status' is put the browse status
	with the previous node.*/
	POSITION__ Browse(
		dtr__browse &BrowseStruct,
		dtr::browse &BrowseStatus ) const
	{
		POSITION__ P = BrowseStruct.P;
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

AUTO( dtr_dynamic_tree )

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
/* end of 'xxx.h' template file */