/*
  Header for the 'que' library by Claude L. Simon (simon@epeios.org)
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

#ifndef QUE__INC
#define QUE__INC

#define QUE_NAME		"QUE"

#define	QUE_VERSION	"$Revision$"	

#define QUE_OWNER		"the Epeios project (http://www.epeios.org/)"

#include "ttr.h"

extern class ttr_tutor &QUETutor;

#if defined( XXX_DBG ) && !defined( QUE_NODBG )
#define QUE_DBG 
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
#include "set.h"


// The link between queue nodes.
struct que__link
{
public:
	// Previous node.
	POSITION__ Previous;
	// Next node.
	POSITION__ Next;
	que__link( void )
	{
		Previous = Next = NONE;
	}
	// Return 'true' if node has next node, false otherwise.
	bso__bool HasNext( void )
	{
		return Next != NONE;
	}
	// Return 'true' if node has previous node, false otherwise.
	bso__bool HasPrevious( void )
	{
		return Previous != NONE;
	}
	// To help swapping. Replace all reference to 'Node1' by 'Node2'.
	void Replace(
		POSITION__ Node1,
		POSITION__ Node2 )
	{
		if ( Previous == Node1 )
			Previous = Node2;
		else if ( Next == Node1 )
			Next = Node2;
	}
};


//c A set lof links.
class que_links_
: public SET_( que__link )
{
public:
	struct s
	: SET_( que__link )::s
	{};
	que_links_( s &S )
	: SET_( que__link )( S )
	{}
	que_links_ &operator =( const que_links_ &QL )
	{
		SET_( que__link )::operator =( QL );

		return *this;
	}
	//f Previous of 'Item' is set to 'Value'. Next remains unchanged.
	void SetPrevious(
		POSITION__ Item,
		POSITION__ Value )
	{
		que__link L = SET_( que__link )::Read( Item );

		L.Previous = Value;

		SET_( que__link )::Write( L, Item );
	}
	//f Next of 'Item' is set to 'Value'. Previous remains unchanged.
	void SetNext(
		POSITION__ Item,
		POSITION__ Value )
	{
		que__link L = SET_( que__link )::Read( Item );

		L.Next = Value;

		SET_( que__link )::Write( L, Item );
	}
};

//c A queue. Use 'QUEUE_' rather than directly this.
class que_queue_
{
private:
	void Initialize_(
		POSITION__ Begin,
		POSITION__ End );
	void HandleNeighboursForSwap_(
		que__link Link,
		POSITION__ Node )
	{
		que__link L;

		if ( Link.HasPrevious() ) {
			L = Links( Link.Previous );
			L.Next = Node;
			Links.Write( L, Link.Previous );
		}

		if ( Link.HasNext() ) {
			L = Links( Link.Next );
			L.Previous = Node;
			Links.Write( L, Link.Next );
		}
	}
#ifdef QUE_DBG
	void TestIfItem_( POSITION__ Item )
	{
		que__link L = Links( Item );

		if ( ( L.Previous != NONE ) || ( L.Next != NONE ) )
			ERRu();
	}
#endif
public:
	//r Links between nodes.
	que_links_ Links;
	struct s
	{
		que_links_::s Links;
	};
	que_queue_( s &S )
	: Links( S.Links )
	{}
	void reset( bso__bool P = true )
	{
		Links.reset( P );
	}
	void plug( mmm_multimemory_ &M )
	{
		Links.plug( M );
	}
	void plug( MEMORY_DRIVER_ &MD )
	{
		Links.plug( MD );
	}
	que_queue_ &operator =( const que_queue_ &Q )
	{
		Links = Q.Links;

		return *this;
	}
	//f Initialization.
	void Init( void )
	{
		Links.Init();
	}
	//f Allocate enough room to contains 'Size' nodes.
	void Allocate( SIZE__ Size )
	{
		SIZE__ Before = Links.Extent();
		Links.Allocate( Size );

		if ( Before < Size )
			Initialize_( Before, Size - 1 );
	}
	//f Return node next to 'Node'.
	POSITION__ Next( POSITION__ Node ) const
	{
		return Links( Node ).Next;
	}
	//f Return node previous to 'Node'.
	POSITION__ Previous( POSITION__ Node ) const
	{
		return Links( Node ).Previous;
	}
	//f Return true if 'Node' has next node.
	bso__bool HasNext( POSITION__ Node ) const
	{
		return Links( Node ).HasNext();
	}
	//f Return true if 'Node' has previous node.
	bso__bool HasPrevious( POSITION__ Node ) const
	{
		return Links( Node ).HasPrevious();
	}
	//f Insert 'Item' after 'Node'.
	void InsertItemAfterNode(
		POSITION__ Item,
		POSITION__ Node )
	{
#ifdef QUE_DBG
		TestIfItem_( Item );
#endif
		que__link LNode = Links( Node );
		que__link LItem = Links( Item );

		if ( LNode.HasNext() )
			Links.SetPrevious( LNode.Next, Item );

		LItem.Next = LNode.Next;
		LItem.Previous = Node;
		LNode.Next = Item;

		Links.Write( LNode, Node );
		Links.Write( LItem, Item );
	}
	//f Insert 'Item' before 'Node'.
	void InsertItemBeforeNode(
		POSITION__ Item,
		POSITION__ Node )
	{
#ifdef QUE_DBG
		TestIfItem_( Item );
#endif
		que__link LNode = Links( Node );
		que__link LItem = Links( Item );

		if ( LNode.HasPrevious() )
			Links.SetNext( LNode.Previous, Item );

		LItem.Previous = LNode.Previous;
		LItem.Next = Node;
		LNode.Previous = Item;

		Links.Write( LNode, Node );
		Links.Write( LItem, Item );
	}
	//f Remove node 'Node'.
	void Remove( POSITION__ Node )
	{
		que__link LNode = Links( Node );

		if ( LNode.HasNext() )
			Links.SetPrevious( LNode.Next, LNode.Previous );

		if ( LNode.HasPrevious() )
			Links.SetNext( LNode.Previous, LNode.Next );

		LNode.Next = LNode.Previous = NONE;

		Links.Write( LNode, Node );
	}
	//f Return the extent of the queue.
	SIZE__ Extent( void ) const
	{
		return Links.Extent();
	}
	//f Return the amount of node in the queue.
	SIZE__ Amount( void ) const
	{
		return Links.Amount();
	}
	//f Swap 'Node1' with 'Node2'.
	void Swap(
		POSITION__ Node1,
		POSITION__ Node2 )
	{
		que__link LNode1 = Links.Read( Node1 );
		que__link LNode2 = Links.Read( Node2 );

		HandleNeighboursForSwap_( LNode1, Node2 );
		HandleNeighboursForSwap_( LNode2, Node1 );

		LNode1.Replace( Node2, Node1 );
		LNode2.Replace( Node1, Node2 );

		Links.Write( LNode1, Node2 );
		Links.Write( LNode2, Node1 );
	}
};

AUTO( que_queue )

//d A queue.
#define QUEUE_	que_queue_
#define QUEUE	que_queue



//c A queue manager. The managed queue must be provided at each call ('Queue' parameter).
class que__queue_manager
{
private:
	POSITION__ Head_;
	POSITION__ Tail_;
	SIZE__ Amount_;
#ifdef QUE_DBG
	void Test_( void ) const
	{
		if ( ( Tail_ == NONE ) ||( Head_ == NONE ) )
			ERRu();
	}
#endif
public:
	void reset( bso__bool = true )
	{
		Head_ = Tail_ = NONE;
		Amount_ = 0;
	}
	que__queue_manager( void )
	{
		reset( false );
	}
	que__queue_manager &operator =( const que__queue_manager &Q )
	{
		Head_ = Q.Head_;
		Tail_ = Q.Tail_;
		Amount_ = Q.Amount_;

		return *this;
	}
	//f Initialization with queue 'Queue'.
	void Init( QUEUE_ & )
	{
		Head_ = Tail_ = NONE;
		Amount_ = 0;
	}
	//f Create the queue with item 'Item'.
	void Create(
		POSITION__ Item,
		QUEUE_ & )
	{
#ifdef QUE_DBG
		if ( ( Head_ != NONE ) || ( Tail_ != NONE ) )
			ERRu();
#endif
		Head_ = Tail_ = Item;

		Amount_ = 1;
	}
	//f Insert 'Item' after 'Node'.
	void InsertItemAfterNode(
		POSITION__ Item,
		POSITION__ Node,
		QUEUE_ &Queue )
	{
#ifdef QUE_DBG
		Test_();
#endif
		Queue.InsertItemAfterNode( Item, Node );

		if ( Node == Tail_ )
			Tail_ = Item;

		Amount_++;
	}
	//f Insert 'Item' Before 'Node'.
	void InsertItemBeforeNode(
		POSITION__ Item,
		POSITION__ Node ,
		QUEUE_ &Queue )
	{
#ifdef QUE_DBG
		Test_();
#endif
		Queue.InsertItemBeforeNode( Item, Node );

		if ( Node == Head_ )
			Head_ = Item;

		Amount_++;
	}
	//f Remove 'Node".
	void Remove(
		POSITION__ Node,
		QUEUE_ &Queue  )
	{
#ifdef QUE_DBG
		Test_();
#endif
		if ( Tail_ == Node )
			Tail_ = Queue.Previous( Node );

		if ( Head_ == Node )
			Head_ = Queue.Next( Node );

		Queue.Remove( Node );

		Amount_--;
	}
	//f Return amount of node in the queue.
	SIZE__ Amount( const QUEUE_ & ) const
	{
		return Amount_;
	}
	//f Return the head.
	POSITION__ Head( const QUEUE_ & ) const
	{
		return Head_;
	}
	//f Return first node.
	POSITION__ First( const QUEUE_ & ) const
	{
		return Head_;
	}
	//f Return the tail.
	POSITION__ Tail( const QUEUE_ & ) const
	{
		return Tail_;
	}
	//f Return last node.
	POSITION__ Last( const QUEUE_ & ) const
	{
		return Tail_;
	}
	//f Return node next to 'Node'.
	POSITION__ Next(
		POSITION__ Node,
		const QUEUE_ &Queue )
	{
#ifdef QUE_DBG
		Test_();

		if ( Node == NONE )
			ERRu();
#endif
		return Queue.Next( Node );
	}
	//f Return node previous to 'Node'.
	POSITION__ Previous(
		POSITION__ Node,
		const QUEUE_ &Queue ) const
	{
#ifdef QUE_DBG
		Test_();

		if ( Node == NONE )
			ERRu();
#endif
		return Queue.Previous( Node );
	}
	//f Swap 'Node1' and 'Node2'.
	void Swap(
		POSITION__ Node1,
		POSITION__ Node2,
		QUEUE_ &Queue  )
	{
#ifdef QUE_DBG
		Test_();
#endif
		if ( Tail_ == Node1 )
			Tail_ = Node2;
		else if ( Tail_ == Node2 )
			Tail_ = Node1;

		if ( Head_ == Node1 )
			Head_ = Node2;
		else if ( Head_ == Node2 )
			Head_ = Node1;

		Queue.Swap( Node1, Node2 );
	}
};

		
//c A managed queue (with head and tail). Use 'MQUEUE_' rather then directly this class.
class que_managed_queue_
{
public:
	QUEUE_ Queue;
	struct s {
		QUEUE_::s Queue;
		que__queue_manager QueueManager;
	} &S_;
	void reset( bso__bool P = true )
	{
		Queue.reset( P );
		S_.QueueManager.reset( P );
	}
	que_managed_queue_( s &S )
	: S_( S ),
	  Queue( S.Queue )
	{}
	void plug( mmm_multimemory_ &M )
	{
		Queue.plug( M );
	}
	void plug( MEMORY_DRIVER_ &MD )
	{
		Queue.plug( MD );
	}
	que_managed_queue_ &operator =( const que_managed_queue_ &Q )
	{
		Queue = Q.Queue;
		S_.QueueManager = Q.S_.QueueManager;

		return *this;
	}
	//f Initialization with queue 'Queue'.
	void Init( void )
	{
		Queue.Init();
		S_.QueueManager.Init( Queue );
	}
	//f Allocate enough room to contains 'Size' nodes.
	void Allocate( SIZE__ Size )
	{
		Queue.Allocate( Size );
	}
	//f Create the queue with item 'Item'.
	void Create( POSITION__ Item )
	{
		S_.QueueManager.Create( Item, Queue );
	}
	//f Insert 'Item' after 'Node'.
	void InsertItemAfterNode(
		POSITION__ Item,
		POSITION__ Node )
	{
		S_.QueueManager.InsertItemAfterNode( Item, Node, Queue );
	}
	//f Insert 'Item' Before 'Node'.
	void InsertItemBeforeNode(
		POSITION__ Item,
		POSITION__ Node )
	{
		S_.QueueManager.InsertItemBeforeNode( Item, Node, Queue );
	}
	//f Remove 'Node".
	void Remove( POSITION__ Node )
	{
		S_.QueueManager.Remove( Node, Queue );
	}
	//f Return amount of node in the queue.
	SIZE__ Amount( void ) const
	{
		return S_.QueueManager.Amount( Queue );
	}
	//f Return the head.
	POSITION__ Head( void ) const
	{
		return S_.QueueManager.Head( Queue );
	}
	//f Return first node.
	POSITION__ First( void ) const
	{
		return S_.QueueManager.First( Queue );
	}
	//f Return the tail.
	POSITION__ Tail( void ) const
	{
		return S_.QueueManager.Tail( Queue );
	}
	//f Return last node.
	POSITION__ Last( void ) const
	{
		return S_.QueueManager.Last( Queue );;
	}
	//f Return node next to 'Node'.
	POSITION__ Next( POSITION__ Node ) const
	{
		return S_.QueueManager.Next( Node, Queue );
	}
	//f Return node previous to 'Node'.
	POSITION__ Previous( POSITION__ Node ) const
	{
		return S_.QueueManager.Previous( Node, Queue );
	}
	//f Swap 'Node1' and 'Node2'.
	void Swap(
		POSITION__ Node1,
		POSITION__ Node2 )
	{
		S_.QueueManager.Swap( Node1, Node2, Queue );
	}
};

AUTO( que_managed_queue )

//d A managed queue.
#define MQUEUE_		que_managed_queue_
#define MQUEUE		que_managed_queue

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
