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
//D QUeuE.

#include "err.h"
#include "flw.h"
#include "tym.h"
#include "aem.h"

namespace que {

	using tym::memory_;

	namespace {
		using tym::memory_;

		// The link between queue nodes.
		struct link__
		{
		public:
			// Previous node.
			tym::row__ Previous;
			// Next node.
			tym::row__ Next;
			link__( void )
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
				tym::row__ Node1,
				tym::row__ Node2 )
			{
				if ( Previous == Node1 )
					Previous = Node2;
				else if ( Next == Node1 )
					Next = Node2;
			}
		};


		//c A set lof links.
		class links_
		: public E_MEMORY_( link__ ),
		  public aem::amount_extent_manager_
		{
		public:
			struct s
			: E_MEMORY_( link__ )::s,
			aem::amount_extent_manager_::s
			{};
			links_( s &S )
			: E_MEMORY_( link__ )( S ),
			  amount_extent_manager_( S )
			{}
			void reset( bso__bool P = true )
			{
				amount_extent_manager_::reset( P );
				E_MEMORY_( link__ )::reset( P );
			}
			links_ &operator =( const links_ &QL )
			{
				amount_extent_manager_::operator =( QL );
				E_MEMORY_( link__ )::Allocate( QL.Amount() );
				E_MEMORY_( link__ )::Write( QL, QL.Amount() );

				return *this;
			}
			//f Previous of 'Item' is set to 'Value'. Next remains unchanged.
			void SetPrevious(
				tym::row__ Item,
				tym::row__ Value )
			{
				link__ L = E_MEMORY_( link__ )::Read( Item );

				L.Previous = Value;

				E_MEMORY_( link__ )::Write( L, Item );
			}
			//f Next of 'Item' is set to 'Value'. Previous remains unchanged.
			void SetNext(
				tym::row__ Item,
				tym::row__ Value )
			{
				link__ L = E_MEMORY_( link__ )::Read( Item );

				L.Next = Value;

				E_MEMORY_( link__ )::Write( L, Item );
			}
			//f Initialization.
			void Init( void )
			{
				amount_extent_manager_::Init();
				E_MEMORY_( link__ )::Init();
			}
			//f Allocate for 'Size'.
			void Allocate(
				tym::size__ Size,
				aem::mode Mode = aem::mFast )
			{
				if ( amount_extent_manager_::AmountToAllocate( Size, Mode ) )
					E_MEMORY_( link__ )::Allocate( Size );
			}
		};
	}

	//c A queue. Use 'QUEUE_' rather than directly this.
	class queue_
	{
	private:
		void Initialize_(
			tym::row__ Begin,
			tym::row__ End );
		void HandleNeighboursForSwap_(
			link__ Link,
			tym::row__ Node )
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
		void TestIfItem_( tym::row__ Item )
		{
			link__ L = Links( Item );

			if ( ( L.Previous != NONE ) || ( L.Next != NONE ) )
				ERRu();
		}
	#endif
	public:
		//r Links between nodes.
		links_ Links;
		struct s
		{
			links_::s Links;
		};
		queue_( s &S )
		: Links( S.Links )
		{}
		void reset( bso__bool P = true )
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
			aem::mode Mode = aem::mFast )
		{
			tym::size__ Before = Links.Amount();
			Links.Allocate( Size, Mode );

			if ( Before < Size )
				Initialize_( Before, Size - 1 );
		}
		//f Return node next to 'Node'.
		tym::row__ Next( tym::row__ Node ) const
		{
			return Links( Node ).Next;
		}
		//f Return node previous to 'Node'.
		tym::row__ Previous( tym::row__ Node ) const
		{
			return Links( Node ).Previous;
		}
		//f Return true if 'Node' has next node.
		bso__bool HasNext( tym::row__ Node ) const
		{
			return Links( Node ).HasNext();
		}
		//f Return true if 'Node' has previous node.
		bso__bool HasPrevious( tym::row__ Node ) const
		{
			return Links( Node ).HasPrevious();
		}
		//f Insert 'Item' after 'Node'.
		void InsertItemAfterNode(
			tym::row__ Item,
			tym::row__ Node )
		{
	#ifdef QUE_DBG
			TestIfItem_( Item );
	#endif
			link__ LNode = Links( Node );
			link__ LItem = Links( Item );

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
			tym::row__ Item,
			tym::row__ Node )
		{
	#ifdef QUE_DBG
			TestIfItem_( Item );
	#endif
			link__ LNode = Links( Node );
			link__ LItem = Links( Item );

			if ( LNode.HasPrevious() )
				Links.SetNext( LNode.Previous, Item );

			LItem.Previous = LNode.Previous;
			LItem.Next = Node;
			LNode.Previous = Item;

			Links.Write( LNode, Node );
			Links.Write( LItem, Item );
		}
		//f Remove node 'Node'.
		void Remove( tym::row__ Node )
		{
			link__ LNode = Links( Node );

			if ( LNode.HasNext() )
				Links.SetPrevious( LNode.Next, LNode.Previous );

			if ( LNode.HasPrevious() )
				Links.SetNext( LNode.Previous, LNode.Next );

			LNode.Next = LNode.Previous = NONE;

			Links.Write( LNode, Node );
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
			tym::row__ Node1,
			tym::row__ Node2 )
		{
			link__ LNode1 = Links.Read( Node1 );
			link__ LNode2 = Links.Read( Node2 );

			HandleNeighboursForSwap_( LNode1, Node2 );
			HandleNeighboursForSwap_( LNode2, Node1 );

			LNode1.Replace( Node2, Node1 );
			LNode2.Replace( Node1, Node2 );

			Links.Write( LNode1, Node2 );
			Links.Write( LNode2, Node1 );
		}
	};

	AUTO( queue )

	//d A queue.
	#define E_QUEUE_	queue_
	#define E_QUEUE	queue



	//c A queue manager. The managed queue must be provided at each call ('Queue' parameter).
	class queue_manager__
	{
	private:
		tym::row__ Head_;
		tym::row__ Tail_;
		tym::size__ Amount_;
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
		void Init( E_QUEUE_ & )
		{
			Head_ = Tail_ = NONE;
			Amount_ = 0;
		}
		//f Create the queue with item 'Item'.
		void Create(
			tym::row__ Item,
			E_QUEUE_ & )
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
			tym::row__ Item,
			tym::row__ Node,
			E_QUEUE_ &Queue )
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
			tym::row__ Item,
			tym::row__ Node ,
			E_QUEUE_ &Queue )
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
			tym::row__ Node,
			E_QUEUE_ &Queue  )
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
		tym::size__ Amount( const E_QUEUE_ & ) const
		{
			return Amount_;
		}
		//f return 'true' if empty, false otherwise.
		bso__bool IsEmpty( void ) const
		{
			return !Amount_;
		}
		//f Return the head.
		tym::row__ Head( const E_QUEUE_ & ) const
		{
			return Head_;
		}
		//f Return first node.
		tym::row__ First( const E_QUEUE_ & ) const
		{
			return Head_;
		}
		//f Return the tail.
		tym::row__ Tail( const E_QUEUE_ & ) const
		{
			return Tail_;
		}
		//f Return last node.
		tym::row__ Last( const E_QUEUE_ & ) const
		{
			return Tail_;
		}
		//f Return node next to 'Node'.
		tym::row__ Next(
			tym::row__ Node,
			const E_QUEUE_ &Queue )
		{
	#ifdef QUE_DBG
			Test_();

			if ( Node == NONE )
				ERRu();
	#endif
			return Queue.Next( Node );
		}
		//f Return node previous to 'Node'.
		tym::row__ Previous(
			tym::row__ Node,
			const E_QUEUE_ &Queue ) const
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
			tym::row__ Node1,
			tym::row__ Node2,
			E_QUEUE_ &Queue  )
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
	class managed_queue_
	{
	public:
		E_QUEUE_ Queue;
		struct s {
			E_QUEUE_::s Queue;
			queue_manager__ QueueManager;
		} &S_;
		void reset( bso__bool P = true )
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
			aem::mode Mode = aem::mFast )
		{
			Queue.Allocate( Size, Mode );
		}
		//f Create the queue with item 'Item'.
		void Create( tym::row__ Item )
		{
			S_.QueueManager.Create( Item, Queue );
		}
		//f Insert 'Item' after 'Node'.
		void InsertItemAfterNode(
			tym::row__ Item,
			tym::row__ Node )
		{
			S_.QueueManager.InsertItemAfterNode( Item, Node, Queue );
		}
		//f Insert 'Item' Before 'Node'.
		void InsertItemBeforeNode(
			tym::row__ Item,
			tym::row__ Node )
		{
			S_.QueueManager.InsertItemBeforeNode( Item, Node, Queue );
		}
		//f Remove 'Node".
		void Remove( tym::row__ Node )
		{
			S_.QueueManager.Remove( Node, Queue );
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
		//f Return 'true' if empty, false otherwise.
		bso__bool IsEmpty( void ) const
		{
			return S_.QueueManager.IsEmpty();
		}
		//f Return the head.
		tym::row__ Head( void ) const
		{
			return S_.QueueManager.Head( Queue );
		}
		//f Return first node.
		tym::row__ First( void ) const
		{
			return S_.QueueManager.First( Queue );
		}
		//f Return the tail.
		tym::row__ Tail( void ) const
		{
			return S_.QueueManager.Tail( Queue );
		}
		//f Return last node.
		tym::row__ Last( void ) const
		{
			return S_.QueueManager.Last( Queue );;
		}
		//f Return node next to 'Node'.
		tym::row__ Next( tym::row__ Node ) const
		{
			return S_.QueueManager.Next( Node, Queue );
		}
		//f Return node previous to 'Node'.
		tym::row__ Previous( tym::row__ Node ) const
		{
			return S_.QueueManager.Previous( Node, Queue );
		}
		//f Swap 'Node1' and 'Node2'.
		void Swap(
			tym::row__ Node1,
			tym::row__ Node2 )
		{
			S_.QueueManager.Swap( Node1, Node2, Queue );
		}
	};

	AUTO( managed_queue )

	//d A managed queue.
	#define E_MQUEUE_		managed_queue_
	#define E_MQUEUE		managed_queue
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
