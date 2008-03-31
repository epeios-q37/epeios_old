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
		void Allocate(
			epeios::size__ Size,
			aem::mode__ Mode )
		{
			E_IBTREEt_( r )::Allocate( Size, Mode );
			E_IQUEUEt_( r )::Allocate( Size, Mode );
		}
		void BecomeNext( void )	//Obsolete.
		{
			ERRu();	// 
		}
		void BecomePrevious( void )	//Obsolete.
		{
			ERRu();	// 
		}
		//f Put 'Row' as greater then 'Current'. 'Current' must be the result as a search with 'seeker_'.
		r BecomeGreater(
			r Row,
			r Current,
			r Root )
		{
			E_IQUEUEt_( r )::BecomeGreater( Row, Current );
			return E_IBTREEt_( r )::BecomeGreater( Row, Current, Root );
		}
		//f Put 'Row' as greater then 'Current'. 'Current' must be the result as a search with 'seeker_'.
		r BecomeLesser(
			r Row,
			r Current,
			r Root )
		{
			E_IQUEUEt_( r )::BecomeLesser( Row, Current );
			return E_IBTREEt_( r )::BecomeLesser( Row, Current, Root );
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
		r Compare( r Root ) const
		{
			return E_IBTREEt_( r )::Compare( *this, First( Root ) );
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
		E_IBTREEt_( r ) &Tree( void )
		{
			return *this;
		}
		const E_IBTREEt_( r ) &Tree( void ) const
		{
			return *this;
		}
		E_IQUEUEt_( r ) &Queue( void )
		{
			return *this;
		}
		const E_IQUEUEt_( r ) &Queue( void ) const
		{
			return *this;
		}
		r GetTreeParent( r Node ) const
		{
			return E_IBTREEt_( r )::GetParent( Node );
		}
		r GetTreeLesser( r Node ) const
		{
			return E_IBTREEt_( r )::GetLesser( Node );
		}
		bso::bool__ IsTreeLesser( r Node ) const
		{
			r Parent = GetTreeParent( Node );

			if ( Parent != NONE )
				return GetTreeLesser( Parent ) == Node;
			else
				return false;
		}
		r GetTreeGreater( r Node ) const
		{
			return E_IBTREEt_( r )::GetGreater( Node );
		}
		bso::bool__ IsTreeGreater( r Node ) const
		{
			r Parent = GetTreeParent( Node );

			if ( Parent != NONE )
				return GetTreeGreater( Parent ) == Node;
			else
				return false;
		}
		bso::bool__ TreeHasLesser( r Node ) const
		{
			return E_IBTREEt_( r )::HasLesser( Node );
		}
		bso::bool__ TreeHasGreater( r Node ) const
		{
			return E_IBTREEt_( r )::HasGreater( Node );
		}
		void HasGreater( void )	// Obsolete.
		{
			ERRu();	
		}
		void HasLesser( void )	// Obsolete.
		{
			ERRu();	
		}
	};

	E_AUTO1( tree_queue_index )

	class index_file_manager___ {
	private:
		idxbtr::tree_index_file_manager___ _TreeFileManager;
		idxque::queue_index_file_manager___ _QueueFileManager;
	public:
		void reset( bso::bool__ P = true )
		{
			_TreeFileManager.reset( P );
			_QueueFileManager.reset( P );
		}
		index_file_manager___( void ) 
		{
			reset( false );
		}
		~index_file_manager___( void ) 
		{
			reset();
		}
		void Init( 
			const char *TreeFileName,
			const char *QueueFileName,
			mdr::mode__ Mode,
			bso::bool__ Persistent,
			flm::id__ ID )
		{
			reset();

			_TreeFileManager.Init( TreeFileName, Mode, Persistent, ID );

			_QueueFileManager.Init( QueueFileName, Mode, Persistent, ID );
		}
		void ReleaseFiles( void )
		{
			_TreeFileManager.ReleaseFile();
			_QueueFileManager.ReleaseFile();
		}
		void Mode( mdr::mode__ Mode )
		{
			_TreeFileManager.Mode( Mode );
			_QueueFileManager.Mode( Mode );
		}
		bso::bool__ IsPersistent( void ) const
		{
			if ( ( _TreeFileManager.IsPersistent() ) != _QueueFileManager.IsPersistent() )
				ERRc();

			return _TreeFileManager.IsPersistent();
		}
		bso::bool__ Exists( void ) const
		{
			bso::bool__ Exists = _TreeFileManager.Exists();

			if ( Exists != _QueueFileManager.Exists() )
				ERRc();

			return Exists;
		}
		bso::bool__ CreateFiles( err::handle ErrHandle = err::hUsual )
		{
			bso::bool__ Success = _TreeFileManager.CreateFile( ErrHandle );

			if ( !Success )
				return false;

			Success = _QueueFileManager.CreateFile( ErrHandle );

			return Success;
		}
		void Drop( void )
		{
			_TreeFileManager.Drop();
			_QueueFileManager.Drop();
		}
		idxbtr::tree_index_file_manager___ &TreeFileManager( void )
		{
			return _TreeFileManager;
		}
		idxque::queue_index_file_manager___ &QueueFileManager( void )
		{
			return _QueueFileManager;
		}
		time_t TimeStamp( void ) const
		{
			time_t TreeTimeStamp, QueueTimeStamp;

			TreeTimeStamp = _TreeFileManager.TimeStamp();
			QueueTimeStamp = _QueueFileManager.TimeStamp();

			return ( TreeTimeStamp > QueueTimeStamp ? TreeTimeStamp : QueueTimeStamp );
		}
	};


	template <typename index> bso::bool__ Connect(
		index &Index,
		index_file_manager___ &FileManager )
	{
		bso::bool__ Exists = idxbtr::Connect( Index, FileManager.TreeFileManager() );

		if ( Exists != idxque::Connect( Index, FileManager.QueueFileManager() ) )
			ERRc();

		return Exists;
	}



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
