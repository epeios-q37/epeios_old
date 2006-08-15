/*
	Header for the 'idxbtq' library by Claude SIMON (csimon@epeios.org)
	Copyright (C) 2000-2004 Claude SIMON (csimon@epeios.org).

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

#define IDXBTQ_OWNER		"Claude SIMON (csimon@epeios.org)"

#include "ttr.h"

extern class ttr_tutor &IDXBTQTutor;

#if defined( XXX_DBG ) && !defined( IDXBTQ_NODBG )
#define IDXBTQ_DBG
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
//D InDeX Best of Tree and Queue 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

/* Addendum to the automatic documentation generation part. */
//D InDeX Best of Tree and Queue
/* End addendum to automatic documentation generation part. */


#include "err.h"
#include "cpe.h"
#include "idxque.h"
#include "idxbtr.h"

namespace idxbtq {
	using idxbtr::tree_index_;
	using idxque::queue_index_;

	//c Index using a tree-based index and a queue-based index. Fast browsing and sorting.
	template <typename r> class tree_queue_index_
	: public E_IBTREEt_( r ),
	  public E_IQUEUEt_( r )
	{
	public:
		struct s
		: public E_IBTREEt_( r )::s,
		  public E_IQUEUEt_( r )::s
		{};
		tree_queue_index_( s &S )
		: E_IBTREEt_( r )( S ),
		  E_IQUEUEt_( r )( S )
		{}
		void reset( bool P = true )
		{
			E_IBTREEt_( r )::reset( P );
			E_IQUEUEt_( r )::reset( P );
		}
		void plug( mmm::multimemory_ &MM )
		{
			E_IBTREEt_( r )::plug( MM );
			E_IQUEUEt_( r )::plug( MM );
		}
		tree_queue_index_ &operator =( const tree_queue_index_ &I )
		{
			E_IBTREEt_( r )::operator =( I );
			E_IQUEUEt_( r )::operator =( I );

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
			E_IBTREEt_( r )::Init();
			E_IQUEUEt_( r )::Init();
		}
		//f Allocate enough room to contain 'Size' items.
		void Allocate( epeios::size__ Size )
		{
			E_IBTREEt_( r )::Allocate( Size );
			E_IQUEUEt_( r )::Allocate( Size );
		}
		/*f 'New' becomes the next item of 'Item'. 'NextAvailable( Item )' must
		return true to use this function. */
		void BecomeNext(
			r New,
			r Item )
		{
			E_IBTREEt_( r )::BecomeNext( New, Item );
			E_IQUEUEt_( r )::BecomeNext( New, Item );
		}
		/*f 'New' becomes the previous item of 'Item'. 'PreviousAvailable( Item )' must
		return true to use this function. */
		void BecomePrevious(
			r New,
			r Item )
		{
			E_IBTREEt_( r )::BecomePrevious( New, Item );
			E_IQUEUEt_( r )::BecomePrevious( New, Item );
		}
		//f Put 'Row' as greater then 'Current'. 'Current' must be the result as a search with 'seeker_'.
		void MarkAsGreater(
			r Row,
			r Current )
		{
			E_IBTREEt_( r )::MarkAsGreater( Row, Current );
			E_IQUEUEt_( r )::MarkAsGreater( Row, Current );
		}
		//f Put 'Row' as greater then 'Current'. 'Current' must be the result as a search with 'seeker_'.
		void MarkAsLesser(
			r Row,
			r Current )
		{
			E_IBTREEt_( r )::MarkAsLesser( Row, Current );
			E_IQUEUEt_( r )::MarkAsLesser( Row, Current );
		}
		r Delete(
			r Item,
			r Root )
		{
			E_IQUEUEt_( r )::Delete( Item );
			return E_IBTREEt_( r )::Delete( Item, Root );
		}
		//f Balances the tree of the index.
		r Balance(
			r Root,
			mdr::E_MEMORY_DRIVER__ &MD = MDR_INTERNAL_MEMORY_DRIVER )
		{
			return E_IBTREEt_( r )::Fill( *this, First( Root ), MD );
		}
		r First( r Root ) const
		{
			return E_IBTREEt_( r )::First( Root );
		}
		r Last( r Root ) const
		{
			return E_IBTREEt_( r )::Last( Root );
		}
		r Next( r Current ) const
		{
			return E_IQUEUEt_( r )::Next( Current );
		}
		r Previous( r Current ) const
		{
			return E_IQUEUEt_( r )::Previous( Current );
		}
		epeios::size__ Amount( void ) const
		{
			return E_IBTREEt_( r )::Amount();
		}
	};

	E_AUTO1( tree_queue_index )

	template <typename r> E_TTYPEDEF__( idxbtr::E_TSEEKERt__( r ), index_seeker__ );
}

//d An index.
#define E_INDEXt_( r )	tree_queue_index_<r>
#define E_INDEXt( r )	tree_queue_index<r>

#define E_INDEX_	E_INDEXt_( epeios::row__ )
#define E_INDEX		E_INDEXt( epeios::row__ )

#define E_ISEEKERt__( r )	index_seeker__<r>
#define E_ISEEKER__			index_seeker__<epeios::row__>

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
