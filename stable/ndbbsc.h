/*
	Header for the 'ndbbsc' library by Claude SIMON (csimon at zeusw dot org)
	Copyright (C) 2004 Claude SIMON.

	This file is part of the Epeios (http://zeusw.org/epeios/) project.

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

#ifndef NDBBSC__INC
#define NDBBSC__INC

#define NDBBSC_NAME		"NDBBSC"

#define	NDBBSC_VERSION	"$Revision$"

#define NDBBSC_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &NDBBSCTutor;

#if defined( XXX_DBG ) && !defined( NDBBSC_NODBG )
#define NDBBSC_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision$
//C Claude SIMON (csimon at zeusw dot org)
//R $Date$

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D New DataBase BaSiCs (will be moved on its own project in the future) 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "epeios.h"
#include "bso.h"
#include "str.h"
#include "lstctn.h"
#include "que.h"
#include "bch.h"

namespace ndbbsc {
	E_ROW( rrow__ );	// 'Record row'.
	E_ROW( crow__ );	// 'Cache row'.

	typedef bso::char__ atom__;
	typedef str::string_ datum_;
	typedef str::string datum;

	typedef lstctn::E_LXMCONTAINERt_( datum_, crow__ ) _container_;
	typedef que::E_MQUEUEt_( crow__ ) _queue_;
	typedef bch::E_BUNCHt_( crow__, rrow__ ) _list_;
	typedef bch::E_BUNCHt_( rrow__, crow__ ) _links_;

	#define NDBBSC_CACHE_DEFAULT_AMOUNT_MAX	( 2 << 15 )

	class cache_
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
		cache_( s &S )
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
		cache_ &operator =( const cache_ &C )
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
			bso::ulong__ AmountMax = NDBBSC_CACHE_DEFAULT_AMOUNT_MAX )
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
#ifdef NDBBSC_DBG
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

	E_AUTO( cache )

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
