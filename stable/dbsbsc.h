/*
	Header for the 'dbsbsc' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Copyright (C) 2004 Claude SIMON (http://zeusw.org/intl/contact.html).

	This file is part of the Epeios (http://zeusw.org/epeios/) project.

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

#ifndef DBSBSC__INC
#define DBSBSC__INC

#define DBSBSC_NAME		"DBSBSC"

#define	DBSBSC_VERSION	"$Revision$"

#define DBSBSC_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"

#include "ttr.h"

extern class ttr_tutor &DBSBSCTutor;

#if defined( XXX_DBG ) && !defined( DBSBSC_NODBG )
#define DBSBSC_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision$
//C Claude SIMON (http://zeusw.org/intl/contact.html)
//R $Date$

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D DataBaSe BaSiCs 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "tol.h"
#include "mdr.h"
#include "mmm.h"
#include "epeios.h"
#include "lstctn.h"
#include "str.h"
#include "que.h"

namespace dbsbsc {

	E_ROW( rrow__ );	// Record row.
	E_ROW( crow__ );	// Cache row.

	typedef bso::ubyte__ skip_level__;
#define DBSBSC_NO_SKIP	0

	typedef bso::char__ atom__;
	typedef str::string_ datum_;
	typedef str::string datum;

	typedef lstctn::E_LXMCONTAINERt_( datum_, crow__ ) _container_;
	typedef que::E_MQUEUEt_( crow__ ) _queue_;
	typedef bch::E_BUNCHt_( crow__, rrow__ ) _list_;
	typedef bch::E_BUNCHt_( rrow__, crow__ ) _links_;

	#define DBSCTT_CACHE_DEFAULT_AMOUNT_MAX	( 2 << 15 )

	class _cache_
	{
	private:
		void _PutToHead( crow__ Row )
		{
			if ( Queue.Exists( Row ) && Queue.IsMember( Row ) )
				Queue.Delete( Row );

			if ( Queue.IsEmpty() )
				Queue.Create( Row );
			else
				Queue.BecomePrevious( Row, Queue.Head() );
		}
		bso::bool__ _IsMember( rrow__ Row ) const
		{
			if ( List.Exists( Row ) )
				return List( Row ) != NONE;
			else
				return false;
		}
	public:
		_container_ Container;
		_queue_ Queue;
		_list_ List;
		_links_ Links;
		struct s {
			_container_::s Container;
			_queue_::s Queue;
			_list_::s List;
			_links_::s Links;
			bso::ulong__ AmountMax;
		} &S_;
		_cache_( s &S )
		: S_( S ),
		  Container( S.Container ),
		  Queue( S.Queue ),
		  List( S.List ),
		  Links( S.Links )
		{}
		void reset( bso::bool__ P = true )
		{
			Container.reset( P );
			Queue.reset( P );
			List.reset( P );
			Links.reset( P );

			S_.AmountMax = 0;
		}
		void plug( mmm::E_MULTIMEMORY_ &MM )
		{
			Container.plug( MM );
			Queue.plug( MM );
			List.plug( MM );
			Links.plug( MM );
		}
		_cache_ &operator =( const _cache_ &C )
		{
			Container = C.Container;
			Queue = C.Queue;
			List = C.List;
			Links = C.Links;

			S_.AmountMax = C.S_.AmountMax;

			return *this;
		}
		void Init(
			epeios::size__ Size,
			bso::ulong__ AmountMax = DBSCTT_CACHE_DEFAULT_AMOUNT_MAX )
		{
			reset();

			Container.Init();
			Queue.Init();
			List.Init();
			Links.Init();

			Queue.Allocate( AmountMax );
			Container.Allocate( AmountMax );

			Links.Allocate( AmountMax );
			Links.Set( NONE );

			List.Allocate( Size );
			List.Set( NONE );

			S_.AmountMax = AmountMax;
		}
		bso::bool__ Retrieve(
			rrow__ Row,
			datum_ &Datum )
		{
			bso::bool__ IsMember = false;

			if ( IsMember = _IsMember( Row ) ) {
				crow__ CacheRow = List( Row );
				Container.Recall( CacheRow, Datum );
				_PutToHead( CacheRow );
			}

			return IsMember;
		}
		void Store(
			const datum_ &Datum,
			rrow__ Row )
		{
#ifdef DBSBSC_DBG
			if ( _IsMember( Row ) )
				ERRu();
#endif
			crow__ CacheRow = NONE;

			if ( Queue.Amount() >= S_.AmountMax ) {
				CacheRow = Queue.Tail();
				List.Store( NONE, Links( CacheRow ) );
			} else
				CacheRow = Container.New();

			Container.Store( Datum, CacheRow );
			Links.Store( Row, CacheRow );
			_PutToHead( CacheRow );
			List.Store( CacheRow, Row );
		}
		void Remove( rrow__ Row )
		{
			if ( _IsMember( Row ) ) {
				crow__ CacheRow = List( Row );
				Container.Delete( CacheRow );
				List.Store( NONE, Links( CacheRow ) );
				Links.Store( NONE, CacheRow );
				Queue.Delete( CacheRow );
				List.Store( NONE, Row );
			}
		}
	};

	E_AUTO( _cache )

	class file_features_	// Permet d'impl�menter certains comportements necessit�z par le stockage des donn�es dans un fichier.
	{
	protected:
		virtual void DBSBSCCompleteInitialization( bso::bool__ IgnoreAdditionalFiles ) = 0;// Permet d'�viter certaines op�rations co�teuses en temps � l'initialisation.
		virtual void DBSBSCErasePhysically( void ) = 0;
	public:
		struct s
		{
			bso::bool__ InitializationCompleted;
		} &S_;
		file_features_( s &S )
		: S_( S )
		{}
		void reset( bso::bool__ = true )
		{
			S_.InitializationCompleted = false;
		}
		void plug( mmm::E_MULTIMEMORY_ & )
		{
			// A des fins de standardisation.
		}
		void plug( mdr::E_MEMORY_DRIVER__ &)
		{
			// A des fins de standardisation.
		}
		file_features_ &operator =( const file_features_ &FF )
		{
			S_.InitializationCompleted = FF.S_.InitializationCompleted;

			return *this;
		}
		void Init( bso::bool__ Partial )
		{
			if ( !Partial )
				DBSBSCCompleteInitialization( false );

			S_.InitializationCompleted = !Partial;
		}
		void CompleteInitialization( void )
		{
			if ( S_.InitializationCompleted )
				ERRu();

			DBSBSCCompleteInitialization( false );

			S_.InitializationCompleted = true;
		}
		void ErasePhysically( void )
		{
			if ( !S_.InitializationCompleted )
				DBSBSCCompleteInitialization( true );	// Pour que les 'file memory driver's sous-jacents se connecte au fichier, sinon l'effacement ne se fait pas.

			DBSBSCErasePhysically();
		}
		E_RODISCLOSE_( bso::bool__, InitializationCompleted );
	};

	void DropFile(
		const str::string_ &RootFileName,
		const char *Extension );

}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
