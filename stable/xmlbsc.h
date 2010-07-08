/*
  Header for the 'xmlbsc' library by Claude L. Simon (csimon@webmails.com)
  Copyright (C) 2000,2001 Claude L. SIMON (csimon@webmails.com) 

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

#ifndef XMLBSC__INC
#define XMLBSC__INC

#define XMLBSC_NAME		"XMLBSC"

#define	XMLBSC_VERSION	"$Revision$"	

#define XMLBSC_OWNER		"the Epeios project (http://epeios.org/)"

#include "ttr.h"

extern class ttr_tutor &XMLBSCTutor;

#if defined( XXX_DBG ) && !defined( XMLBSC_NODBG )
#define XMLBSC_DBG 
#endif

/* Begin of automatic documentation generation part. */

//V $Revision$
//C Claude L. SIMON (csimon@webmails.com)
//R $Date$

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

/* Addendum to the automatic documentation generation part. */
//D eXtended Markup Langage BaSiCs
/* End addendum to automatic documentation generation part. */

#error "Obsolete. Use library 'HOSBSC' instead."

#include "err.h"
#include "flw.h"
#include "dtr.h"
#include "lst.h"
#include "ctn.h"
#include "flm.h"

namespace xmlbsc {

	using dtr::dynamic_tree_;
	using lst::list_;
	using ctn::mono_extended_container_;

	/*c Contains the file memory driver tu use together with the 'hdbbsc' class.
	Designed for using with the merged memory ('MMG' library). */
	class file_memory_drivers {
	public:
		struct tree_drivers {
			flm::file_memory_driver Tree, Queue;
		} DTree;
		flm::file_memory_driver List;
		struct container_drivers {
			flm::file_memory_driver Statics;
			struct descriptors_drivers {
				flm::file_memory_driver Descriptors;
				flm::file_memory_driver Multimemory;
			} Dynamics;
		} Container;
		//f Initialize with generic name 'Name', directory 'Directory' and  suffix 'Suffix'.
		void Init(
			const char *Name,	
			const char *Directory = NULL,
			const char *Suffix = "hdb" );
		//f Make persistent.
		void MakePersistent( void )
		{
			DTree.Tree.Persistant();
			DTree.Queue.Persistant();
			List.Persistant();
			Container.Statics.Persistant();
			Container.Dynamics.Descriptors.Persistant();
			Container.Dynamics.Multimemory.Persistant();
		}		
		//f Manual mode.
		void Manual( void )
		{
			DTree.Tree.Manuel();
			DTree.Queue.Manuel();
			List.Manuel();
			Container.Statics.Manuel();
			Container.Dynamics.Descriptors.Manuel();
			Container.Dynamics.Multimemory.Manuel();
		}
	};

	//c The basic manager of item type 'i' and referred by 'referrence__'
	template <typename i, typename r> class basic_
	: public E_LISTt_( r ),
	  public E_DTREEt_( r ),
	  public E_XMCONTAINERt_( i, r )
	{
	private:
		r Create_( const i &Item )
		{
			r P = E_LISTt_( r )::CreateEntry();

			E_XMCONTAINERt_( i, r )::Write( Item, P );

			return P;
		}
		void CreateRoot_( r Position )
		{
	#ifdef HDBBSC_DBG
			if ( S_.Root != NONE )
				ERRu();
	#endif
			S_.Root = Position;
		}
		r CreateRoot_( const i &Item )
		{
			r P = Create_( Item );

			CreateRoot_( P );

			return P;
		}
	protected:
		void LSTAllocate( epeios::size__ Size )
		{
			E_DTREEt_( r )::Allocate( Size, aem::mFit );

			E_XMCONTAINERt_( i, r )::Allocate( Size, aem::mFit );
		}
		void BecomeFirst( 
			r First,
			r Node )
		{
			E_DTREEt_( r )::BecomeFirst( First, Node );
		}
		void BecomeLast( 
			r Last,
			r Node )
		{
			E_DTREEt_( r )::BecomeLast( Last, Node );
		}
		void BecomePrevious(
			r Previous,
			r Node )
		{
			E_DTREEt_( r )::BecomePrevious( Previous, Node );
		}
		void BecomeNext(
			r Next,
			r Node )
		{
			E_DTREEt_( r )::BecomeNext( Next, Node );
		}
	public:
		struct s
		: public E_LISTt_( r )::s,
		  public E_DTREEt_( r )::s,
		  public E_XMCONTAINERt_( i, r )::s
		{
			r Root;
		} &S_;
		basic_( s &S )
		: S_( S ),
		  E_LISTt_( r )( S ),
		  E_DTREEt_( r )( S ),
		  E_XMCONTAINERt_( i, r )( S )
		{}
		void reset( bool P = true )
		{
			E_LISTt_( r )::reset( P );
			E_DTREEt_( r )::reset( P );
			E_XMCONTAINERt_( i, r )::reset( P );

			S_.Root = NONE;
		}
		void plug( mmm::multimemory_ &M )
		{
			E_LISTt_( r )::plug( M );
			E_DTREEt_( r )::plug( M );
			E_XMCONTAINERt_( i, r )::plug( M );
		}
		basic_ &operator =( const basic_ &BM )
		{
			E_LISTt_( r )::operator =( BM );
			E_DTREEt_( r )::operator =( BM );
			E_XMCONTAINERt_( i, r )::operator =( BM );
			S_.Root = BM.S_.Root;

			return *this;
		}
		//f Plug the file meomry drivers 'Drivers'. Call it BEFORE 'Init()'.
		void Plug( file_memory_drivers &Drivers )
		{
			E_DTREEt_( r )::Tree.plug( Drivers.DTree.Tree );
			E_DTREEt_( r )::Queue.plug( Drivers.DTree.Queue );
			E_LISTt_( r )::plug( Drivers.List );
			E_XMCONTAINERt_( i, r )::Statics.plug( Drivers.Container.Statics );
			E_XMCONTAINERt_( i, r )::Dynamics.Descripteurs.plug( Drivers.Container.Dynamics.Descriptors );
			E_XMCONTAINERt_( i, r )::Dynamics.Multimemoire.plug( Drivers.Container.Dynamics.Multimemory );
		}
		/*f Initialization with root 'Root'. Return reference of the root
		node. */
		r Init( const i &Root )
		{
			E_LISTt_( r )::Init();
			E_DTREEt_( r )::Init();
			E_XMCONTAINERt_( i, r )::Init();

			S_.Root = NONE;

			return CreateRoot_( Root );
		}
		//f Return the position where new item 'Item' is put.
		r Create( const i &Item )
		{
			return Create_( Item );
		}
		//f Return the psotion of a new empty item which would be initialized later.
		r Create( void )
		{
			return E_LISTt_( r )::CreateEntry();
		}
		//f Return the root of the tree, 'NONE' if none.
		r GetRoot( void ) const
		{
			return S_.Root;
		}
		/*f The new item 'Item' becomes first child of node 'Node'.
		Return position where 'Item' is put. */
		r BecomeFirst(
			const i &Item,
			r Node )
		{
			r P = Create( Item );

			E_DTREEt_( r )::BecomeFirst( P, Node );

			return P;
		}
		/*f The new name 'Name' becomes last child of node 'Node'.
		Return position where 'Name' is put. */
		r BecomeLast(
			const i &Item,
			r Node )
		{
			r P = Create( Item );

			E_DTREEt_( r )::BecomeLast( P, Node );

			return P;
		}
		/*f The new name 'Name' becomes node next to node 'Node'.
		Return position where 'Name' is put. */
		r BecomeNext(
			const i &Item,
			r Node )
		{
			r P = Create( Item );
	#ifdef HDBBSC__DEBUG
			if ( Node == S_.Root )
				ERRu();
	#endif

			E_DTREEt_( r )::BecomeNext( P, Node );

			return P;
		}
		/*f The new name 'Name' becomes node previous to node 'Node'.
		Return position where 'Name' is put. */
		r BecomePrevious(
			const i &Item,
			r Node )
		{
			r P = Create( Item );

	#ifdef HDBBSC__DEBUG
			if ( Node == S_.Root )
				ERRu();
	#endif

			E_DTREEt_( r )::BecomePrevious( P, Node );

			return P;
		}
		//f Returns true if 'Node' exists, or false if no.
		bso::bool__ Exists( r Node ) const
		{
			return E_DTREEt_( r )::Parent( Node ) != NONE;
		}
		r First( r Node ) const
		{
			return E_DTREEt_( r )::First( Node );
		}
		r Next( r Node ) const
		{
			return E_DTREEt_( r )::Next( Node );
		}
		r Previous( r Node ) const
		{
			return E_DTREEt_( r )::Previous( Node );
		}
		r Last( r Node ) const
		{
			return E_DTREEt_( r )::Last( Node );
		}
		epeios::size__ Amount( void ) const
		{
			return E_DTREEt_( r )::Amount();
		}
	};

	AUTO1( basic )

}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
