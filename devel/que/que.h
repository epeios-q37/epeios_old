/*
	Header for the 'que' library by Claude SIMON (csimon@epeios.org)
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

#ifndef QUE__INC
#define QUE__INC

#define QUE_NAME		"QUE"

#define	QUE_VERSION	"$Revision$"

#define QUE_OWNER		"Claude SIMON (csimon@epeios.org)"

#include "ttr.h"

extern class ttr_tutor &QUETutor;

#if defined( XXX_DBG ) && !defined( QUE_NODBG )
#define QUE_DBG
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
//D QUEue 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/
//D QUeuE.

#include "err.h"
#include "flw.h"
#include "tym.h"
#include "bch.h"
#include "stk.h"

namespace que {

	using tym::memory_;
	using namespace epeios;

	//e dump direction.	
	enum direction
	{
		//i Unknow direction.
		dUnknow,
		//i Ascending.
		dAscending,
		//i Descending.
		dDescending,
		//i amount of directyon item.
		d_amount
	};
	
	// The link between queue nodes.
	struct link__
	{
	public:
		// Previous node.
		row_t__ Previous;
		// Next node.
		row_t__ Next;
		link__( void )
		{
			Previous = Next = NONE;
		}
		// Return 'true' if node has next node, false otherwise.
		bso::bool__ HasNext( void ) const
		{
			return Next != NONE;
		}
		// Return 'true' if node has previous node, false otherwise.
		bso::bool__ HasPrevious( void ) const
		{
			return Previous != NONE;
		}
		// To help swapping. Replace all reference to 'Node1' by 'Node2'.
		void Replace(
			row_t__ Node1,
			row_t__ Node2 )
		{
			if ( Previous == Node1 )
				Previous = Node2;
			else if ( Next == Node1 )
				Next = Node2;
		}
	};

	using namespace aem;

	typedef bch::E_BUNCHt_( que::link__, row__ ) _lbunch_;

	//c A set lof links.
	class links_
	: public _lbunch_
	{
	public:
		struct s
		: _lbunch_::s
		{};
		links_( s &S )
		: _lbunch_( S )
		{}
		void reset( bso::bool__ P = true )
		{
			_lbunch_::reset( P );
		}
		links_ &operator =( const links_ &QL )
		{
			_lbunch_::operator =( QL );
			return *this;
		}
		//f Previous of 'Item' is set to 'Value'. Next remains unchanged.
		void SetPrevious(
			row_t__ Item,
			row_t__ Value )
		{
			que::link__ L = Read( Item );

			L.Previous = Value;

			Write( L, Item );
		}
		//f Next of 'Item' is set to 'Value'. Previous remains unchanged.
		void SetNext(
			row_t__ Item,
			row_t__ Value )
		{
			que::link__ L = Read( Item );

			L.Next = Value;

			Write( L, Item );
		}
		//f Initialization.
		void Init( void )
		{
			_lbunch_::Init();
		}
		void Initialize(
			row_t__ Begin,
			row_t__ End );
	};
	
	AUTO( links )
	
	//c A queue. Use 'QUEUE_' rather than directly this.
	template <typename r> class queue_
	{
	private:
		void HandleNeighboursForSwap_(
			que::link__ Link,
			epeios::row_t__ Node )
		{
			link__ L;

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
		void TestIfItem_( epeios::row_t__ Item )
		{
			link__ L = Links( Item );

			if ( ( L.Previous != NONE ) || ( L.Next != NONE ) )
				ERRu();
		}
	#endif
	public:
		//r Links between nodes.
		que::links_ Links;
		struct s
		{
			que::links_::s Links;
		};
		queue_( s &S )
		: Links( S.Links )
		{}
		void reset( bso::bool__ P = true )
		{
			Links.reset( P );
		}
		void plug( mmm::multimemory_ &M )
		{
			Links.plug( M );
		}
		void plug( mdr::E_MEMORY_DRIVER_ &MD )
		{
			Links.plug( MD );
		}
		queue_ &operator =( const queue_ &Q )
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
		void Allocate(
			tym::size__ Size,
			aem::mode Mode = aem::mDefault )
		{
			tym::size__ Before = Links.Amount();
			Links.Allocate( Size, Mode );

			if ( Before < Size )
				Links.Initialize( Before, Size - 1 );
		}
		//f Return node next to 'Node'.
		r Next( r Node ) const
		{
			return Links( *Node ).Next;
		}
		//f Return node previous to 'Node'.
		r Previous( r Node ) const
		{
			return Links( *Node ).Previous;
		}
		//f Return true if 'Node' has next node.
		bso::bool__ HasNext( r Node ) const
		{
			return Links( *Node ).HasNext();
		}
		//f Return true if 'Node' has previous node.
		bso::bool__ HasPrevious( r Node ) const
		{
			return Links( *Node ).HasPrevious();
		}
		//f Insert 'Item' after 'Node'.
		void BecomeNext(
			r Item,
			r Node )
		{
	#ifdef QUE_DBG
			TestIfItem_( *Item );
	#endif
			link__ LNode = Links( *Node );
			link__ LItem = Links( *Item );

			if ( LNode.HasNext() )
				Links.SetPrevious( LNode.Next, *Item );

			LItem.Next = LNode.Next;
			LItem.Previous = *Node;
			LNode.Next = *Item;

			Links.Write( LNode, *Node );
			Links.Write( LItem, *Item );
		}
		//f Insert 'Item' before 'Node'.
		void BecomePrevious(
			r Item,
			r Node )
		{
	#ifdef QUE_DBG
			TestIfItem_( *Item );
	#endif
			link__ LNode = Links( *Node );
			link__ LItem = Links( *Item );

			if ( LNode.HasPrevious() )
				Links.SetNext( LNode.Previous, *Item );

			LItem.Previous = LNode.Previous;
			LItem.Next = *Node;
			LNode.Previous = *Item;

			Links.Write( LNode, *Node );
			Links.Write( LItem, *Item );
		}
		//f Delete node 'Node'.
		void Delete( r Node )
		{
			link__ LNode = Links( *Node );

			if ( LNode.HasNext() )
				Links.SetPrevious( LNode.Next, LNode.Previous );

			if ( LNode.HasPrevious() )
				Links.SetNext( LNode.Previous, LNode.Next );

			LNode.Next = LNode.Previous = NONE;

			Links.Write( LNode, *Node );
		}
		//f Return the extent of the queue.
		tym::size__ Extent( void ) const
		{
			return Links.Extent();
		}
		//f Return the amount of node in the queue.
		tym::size__ Amount( void ) const
		{
			return Links.Amount();
		}
		//f Swap 'Node1' with 'Node2'.
		void Swap(
			r Node1,
			r Node2 )
		{
			link__ LNode1 = Links.Read( *Node1 );
			link__ LNode2 = Links.Read( *Node2 );

			HandleNeighboursForSwap_( LNode1, *Node2 );
			HandleNeighboursForSwap_( LNode2, *Node1 );

			LNode1.Replace( *Node2, *Node1 );
			LNode2.Replace( *Node1, *Node2 );

			Links.Write( LNode1, *Node2 );
			Links.Write( LNode2, *Node1 );
		}
		//f Return true if 'Node' exists, false oterwise.
		bso::bool__ Exists( r Node ) const
		{
			return Links.Exists( *Node );
		}
		void Dump(
			stk::E_STACK_( r ) &Stack,
			r Begin,
			que::direction Direction ) const;
	};

	AUTO1( queue )

	//d A queue.
	#define E_QUEUEt_( r )		queue_< r >
	#define E_QUEUEt(r )		queue< r >

	#define E_QUEUE_	E_QUEUEt_( epeios::row__ )
	#define E_QUEUE		E_QUEUEt( epeios::row__ )

	void Dump_(
		const E_QUEUE_ &Queue,
		stk::E_STACK_( epeios::row__ ) &Stack,
		epeios::row__ Begin,
		direction Direction );

	template <typename r> inline void queue_<r>::Dump(
		stk::E_STACK_( r ) &Stack,
		r Begin,
		que::direction Direction ) const
	{
		Dump_( *(E_QUEUE_ *)this, Stack, Begin, Direction );
	}


	//c A queue manager. The managed queue must be provided at each call ('Queue' parameter).
	template <typename r> class queue_manager__
	{
	private:
		r Head_;
		r Tail_;
		tym::size__ Amount_;
	#ifdef QUE_DBG
		void Test_( void ) const
		{
			if ( ( Tail_ == NONE ) ||( Head_ == NONE ) )
				ERRu();
		}
	#endif
	public:
		void reset( bso::bool__ = true )
		{
			Head_ = Tail_ = NONE;
			Amount_ = 0;
		}
		queue_manager__( void )
		{
			reset( false );
		}
		queue_manager__ &operator =( const queue_manager__ &Q )
		{
			Head_ = Q.Head_;
			Tail_ = Q.Tail_;
			Amount_ = Q.Amount_;

			return *this;
		}
		//f Initialization with queue 'Queue'.
		void Init( que::E_QUEUEt_( r ) & )
		{
			Head_ = Tail_ = NONE;
			Amount_ = 0;
		}
		//f Create the queue with item 'Item'.
		void Create(
			r Item,
			que::E_QUEUEt_(r) & )
		{
	#ifdef QUE_DBG
			if ( ( Head_ != NONE ) || ( Tail_ != NONE ) )
				ERRu();
	#endif
			Head_ = Tail_ = Item;

			Amount_ = 1;
		}
		//f Insert 'Item' after 'Node'.
		void BecomeNext(
			r Item,
			r Node,
			que::E_QUEUEt_(r) &Queue )
		{
	#ifdef QUE_DBG
			Test_();
	#endif
			Queue.BecomeNext( Item, Node );

			if ( Node == Tail_ )
				Tail_ = Item;

			Amount_++;
		}
		//f Insert 'Item' Before 'Node'.
		void BecomePrevious(
			r Item,
			r Node ,
			que::E_QUEUEt_(r) &Queue )
		{
	#ifdef QUE_DBG
			Test_();
	#endif
			Queue.BecomePrevious( Item, Node );

			if ( Node == Head_ )
				Head_ = Item;

			Amount_++;
		}
		//f Delete 'Node'.
		void Delete(
			r Node,
			que::E_QUEUEt_(r) &Queue  )
		{
	#ifdef QUE_DBG
			Test_();
	#endif
			if ( Tail_ == Node )
				Tail_ = Queue.Previous( Node );

			if ( Head_ == Node )
				Head_ = Queue.Next( Node );

			Queue.Delete( Node );

			Amount_--;
		}
		//f Return amount of node in the queue.
		tym::size__ Amount( const que::E_QUEUEt_(r) & ) const
		{
			return Amount_;
		}
		//f Return 'true' if 'Node' exists, false otherwise.
		bso::bool__ Exists(
			r Node,
			const que::E_QUEUEt_(r) &Queue ) const
		{
			return Queue.Exists( Node );
		}
		//f return 'true' if empty, false otherwise.
		bso::bool__ IsEmpty( void ) const
		{
			return !Amount_;
		}
		//f Return the head.
		r Head( const que::E_QUEUEt_(r) & ) const
		{
			return Head_;
		}
		//f Return first node.
		r First( const que::E_QUEUEt_(r) & ) const
		{
			return Head_;
		}
		//f Return the tail.
		r Tail( const que::E_QUEUEt_(r) & ) const
		{
			return Tail_;
		}
		//f Return last node.
		r Last( const que::E_QUEUEt_(r) & ) const
		{
			return Tail_;
		}
		//f Return node next to 'Node'.
		r Next(
			r Node,
			const que::E_QUEUEt_(r) &Queue )
		{
	#ifdef QUE_DBG
			Test_();

			if ( Node == NONE )
				ERRu();
	#endif
			return Queue.Next( Node );
		}
		//f Return node previous to 'Node'.
		r Previous(
			r Node,
			const que::E_QUEUEt_(r) &Queue ) const
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
			r Node1,
			r Node2,
			que::E_QUEUEt_(r) &Queue  )
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
	template <typename r> class managed_queue_
	{
	public:
		E_QUEUEt_( r ) Queue;
		struct s {
			typename E_QUEUEt_( r )::s Queue;
			queue_manager__<r> QueueManager;
		} &S_;
		void reset( bso::bool__ P = true )
		{
			Queue.reset( P );
			S_.QueueManager.reset( P );
		}
		managed_queue_( s &S )
		: S_( S ),
		  Queue( S.Queue )
		{}
		void plug( mmm::multimemory_ &M )
		{
			Queue.plug( M );
		}
		void plug( mdr::E_MEMORY_DRIVER_ &MD )
		{
			Queue.plug( MD );
		}
		managed_queue_ &operator =( const managed_queue_ &Q )
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
		void Allocate(
			tym::size__ Size,
			aem::mode Mode = aem::mDefault )
		{
			Queue.Allocate( Size, Mode );
		}
		//f Create the queue with item 'Item'.
		void Create( r Item )
		{
			S_.QueueManager.Create( Item, Queue );
		}
		//f Insert 'Item' after 'Node'.
		void BecomeNext(
			r Item,
			r Node )
		{
			S_.QueueManager.BecomeNext( Item, Node, Queue );
		}
		//f Insert 'Item' Before 'Node'.
		void BecomePrevious(
			r Item,
			r Node )
		{
			S_.QueueManager.BecomePrevious( Item, Node, Queue );
		}
		//f Delete 'Node'.
		void Delete( r Node )
		{
			S_.QueueManager.Delete( Node, Queue );
		}
		//f Return amount of node in the queue.
		tym::size__ Amount( void ) const
		{
			return S_.QueueManager.Amount( Queue );
		}
		//f Return extent of node in the queue.
		tym::size__ Extent( void ) const
		{
			return Queue.Extent();
		}
		//f Return 'true' if 'Node' exists, false otherwise.
		bso::bool__ Exists( r Node ) const
		{
			return S_.QueueManager.Exists( Node, Queue );
		}
		//f Return 'true' if empty, false otherwise.
		bso::bool__ IsEmpty( void ) const
		{
			return S_.QueueManager.IsEmpty();
		}
		//f Return the head.
		r Head( void ) const
		{
			return S_.QueueManager.Head( Queue );
		}
		//f Return first node.
		r First( void ) const
		{
			return S_.QueueManager.First( Queue );
		}
		//f Return the tail.
		r Tail( void ) const
		{
			return S_.QueueManager.Tail( Queue );
		}
		//f Return last node.
		r Last( void ) const
		{
			return S_.QueueManager.Last( Queue );;
		}
		//f Return node next to 'Node'.
		r Next( r Node ) const
		{
			return S_.QueueManager.Next( Node, Queue );
		}
		//f Return node previous to 'Node'.
		r Previous( r Node ) const
		{
			return S_.QueueManager.Previous( Node, Queue );
		}
		//f Swap 'Node1' and 'Node2'.
		void Swap(
			r Node1,
			r Node2 )
		{
			S_.QueueManager.Swap( Node1, Node2, Queue );
		}
		void Dump(
			stk::E_STACK_( r ) &Stack,
			r Begin,
			que::direction Direction ) const
		{
			Queue.Dump( Stack, Begin, Direction );
		}
	};

	AUTO1( managed_queue )

	//d A managed queue.
	#define E_MQUEUEt_( r )		managed_queue_< r >
	#define E_MQUEUEt( r )		managed_queue<r>
	
	#define E_MQUEUE_	E_MQUEUEt_( epeios::row__ )
	#define E_MQUEUE	E_MQUEUEt( epeios::row__ )
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
