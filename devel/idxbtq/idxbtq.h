/*
  Header for the 'idxbtq' library by Claude L. Simon (csimon@epeios.org)
  Copyright (C) 2000,2001 Claude L. SIMON (csimon@epeios.org) 

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

#ifndef IDXBTQ__INC
#define IDXBTQ__INC

#define IDXBTQ_NAME		"IDXBTQ"

#define	IDXBTQ_VERSION	"$Revision$"	

#define IDXBTQ_OWNER		"the Epeios project (http://epeios.org/)"

#include "ttr.h"

extern class ttr_tutor &IDXBTQTutor;

#if defined( XXX_DBG ) && !defined( IDXBTQ_NODBG )
#define IDXBTQ_DBG 
#endif

/* Begin of automatic documentation generation part. */

//V $Revision$
//C Claude L. SIMON (csimon@epeios.org)
//R $Date$

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#include "err.h"
#include "idxque.h"
#include "idxbtr.h"

namespace idxbtq {
	using idxbtr::tree_index_;
	using idxque::queue_index_;

	//c Index using a tree-based index and a queue-based index. Fast browsing and sorting.
	class tree_queue_index_
	: public E_IBTREE_,
	  public E_IQUEUE_
	{
	public:
		struct s
		: public E_IBTREE_::s,
		  public E_IQUEUE_::s
		{};
		tree_queue_index_( s &S )
		: E_IBTREE_( S ),
		  E_IQUEUE_( S )
		{}
		void reset( bool P = true )
		{
			E_IBTREE_::reset( P );
			E_IQUEUE_::reset( P );
		}
		void plug( mmm::multimemory_ &MM )
		{
			E_IBTREE_::plug( MM );
			E_IQUEUE_::plug( MM );
		}
		tree_queue_index_ &operator =( const tree_queue_index_ &I )
		{
			E_IBTREE_::operator =( I );
			E_IQUEUE_::operator =( I );

			return *this;
		}
	/*	void ecrire( flo_sortie_ &Flot ) const
		{
			tree_index_::ecrire( Flot );
			queue_index_::ecrire( Flot );
		}
		void lire( flo_entree_ &Flot )
		{
			tree_index_::lire( Flot );
			queue_index_::lire( Flot );
		}
	*/	//f Initializtion.
		void Init( void )
		{
			E_IBTREE_::Init();
			E_IQUEUE_::Init();
		}
		NAV( E_IQUEUE_:: )
		//f 'Item' becomes the first item of the index, if empty.
		void Create( tym::row__ Item )
		{
			if ( !IsEmpty() )
				ERRu();

			E_IBTREE_::Create( Item );
			E_IQUEUE_::Create( Item );
		}
		//f Allocate enough room to contain 'Size' items.
		void Allocate( epeios::size__ Size )
		{
			E_IBTREE_::Allocate( Size );
			E_IQUEUE_::Allocate( Size );
		}
		/*f 'New' becomes the next item of 'Item'. 'NextAvailable( Item )' must
		return true to use this function. */
		void BecomeNext(
			epeios::row__ New,
			epeios::row__ Item )
		{
			E_IBTREE_::BecomeNext( New, Item );
			E_IQUEUE_::BecomeNext( New, Item );
		}
		/*f 'New' becomes the previous item of 'Item'. 'PreviousAvailable( Item )' must
		return true to use this function. */
		void BecomePrevious(
			epeios::row__ New,
			epeios::row__ Item )
		{
			E_IBTREE_::BecomePrevious( New, Item );
			E_IQUEUE_::BecomePrevious( New, Item );
		}
		//f Remove 'Item'.
		void Remove( tym::row__ Item )
		{
			E_IQUEUE_::Remove( Item );
			E_IBTREE_::Remove( Item );
		}
		//f Balances the tree of the index.
		void Balance( mdr::E_MEMORY_DRIVER_ &MD = *(mdr::E_MEMORY_DRIVER_ *)NULL )
		{
			if ( E_IQUEUE_::Amount() )
				E_IBTREE_::Fill( E_IQUEUE_::Queue, E_IQUEUE_::First(), MD );
		}
	};

	AUTO( tree_queue_index )
}

//d An index.
#define E_INDEX_	tree_queue_index_

#define E_INDEX		tree_queue_index


/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
