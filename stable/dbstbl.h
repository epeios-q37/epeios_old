/*
	Header for the 'dbstbl' library by Claude SIMON (csimon@epeios.org)
	Copyright (C) 2004 Claude SIMON (csimon@epeios.org).

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

#ifndef DBSTBL__INC
#define DBSTBL__INC

#define DBSTBL_NAME		"DBSTBL"

#define	DBSTBL_VERSION	"$Revision$"

#define DBSTBL_OWNER		"Claude SIMON (csimon@epeios.org)"

#include "ttr.h"

extern class ttr_tutor &DBSTBLTutor;

#if defined( XXX_DBG ) && !defined( DBSTBL_NODBG )
#define DBSTBL_DBG
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
//D DataBaSe TaBLe 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "dbsdct.h"
#include "dbsidx.h"
#include "cpe.h"

#ifdef DBSTBL_THREAD_SAFE
#	define DBSTBL__THREAD_SAFE
#elif !defined( DBSTBL_NO_THREAD_SAFE
#	ifdef CPE__MT
#		define DBSTBL__THREAD_SAFE
#	endif
#endif

#ifdef DBSTBL__THREAD_SAFE
#	include "lck.h"
#endif

namespace dbstbl {

	E_ROW( irow__ );

	using dbsdct::content_;
	using dbsidx::index_;
	using dbsdct::data_;
	using dbsdct::row__;

	typedef bch::E_BUNCHt_( index_ *, irow__ ) _indexes_;

	class table_
	{
	private:
		void _ITest( irow__ Row ) const
		{
#ifdef DBSTBL_DBG
			if ( !Indexes.Exists( Row ) )
				ERRc();
#endif
		}
		const content_ &_C( void ) const
		{
			return *_S.Content;
		}
		content_ &_C( void )
		{
			return *_S.Content;
		}
		const index_ &_I( irow__ Row ) const
		{
			_ITest( Row );

			return *Indexes( Row );
		}
		index_ &_I( irow__ Row )
		{
			_ITest( Row );

			return *Indexes( Row );
		}
		void _Reindex( irow__ Row )
		{
			_ITest( Row );

			ERRl();
		}
		void _InsertInIndexes( row__ Row );
		void _DeleteFromIndexes( row__ Row );
	public:
		_indexes_ Indexes;
		struct s
		{
			_indexes_::s Indexes;
			content_ *Content;
		} &_S;
		table_( s &S )
		: _S( S ),
		Indexes( S.Indexes )
		{}
		void reset( bso::bool__ P = true )
		{
			Indexes.reset( P );
		}
		void plug( mmm::E_MULTIMEMORY_ &MM )
		{
			Indexes.plug( MM );
		}
		table_ &operator =( const table_ &T )
		{
			Indexes = T.Indexes;
			_S.Content = T._S.Content;

			return *this;
		}
		void Init( content_ &Content )
		{
			reset();

			Indexes.Init();

			_S.Content = &Content;
		}
		irow__ AddIndex(
			index_ &Index,
			bso::bool__ Reindex = false )
		{
			irow__ Row = Indexes.Append( &Index );

			if ( Reindex )
				_Reindex( Row );
			else if ( Index.Amount() != _C().Amount() )
				ERRu();

			return Row;
		}
		row__ Store( const data_ &Data )
		{
			row__ Row = _C().Store( Data );

			_InsertInIndexes( Row );

			return Row;
		}
		void Store(
			const data_ &Data,
			row__ Row )
		{
			_DeleteFromIndexes( Row );

			_C().Store( Data, Row );

			_InsertInIndexes( Row );
		}
		void Delete( row__ Row )
		{
			_C().Erase( Row );

			_DeleteFromIndexes( Row );
		}
		row__ Search(
			const data_ &Data,
			irow__ IRow,
			bso::sign__ &Sign ) const
		{
			return _I( IRow ).Search( Data, Sign );
		}
		row__ Search(
			const data_ &Data,
			irow__ IRow ) const
		{
			bso::sign__ Sign;

			return Search( Data, IRow, Sign );
		}
		row__ First( irow__ IRow ) const
		{
			if ( IRow == NONE )
				return _C().First();
			else
				return _I( IRow ).First();
		}
		row__ Last( irow__ IRow ) const
		{
			if ( IRow == NONE )
				return _C().Last();
			else
				return _I( IRow ).Last();
		}
		row__ Next( 
			irow__ IRow,
			row__ Row ) const
		{
			if ( IRow == NONE )
				return _C().Next( Row );
			else
				return _I( IRow ).Next( Row );
		}
		row__ Previous( 
			irow__ IRow,
			row__ Row ) const
		{
			if ( IRow == NONE )
				return _C().Previous( Row );
			else
				return _I( IRow ).Previous( Row );
		}
		mdr::size__ Amount( void ) const
		{
			return _C().Amount();
		}
	};

	E_AUTO( table )

#ifdef DBSTBL__THREAD_SAFE
	class thread_safe_table_
	{
	private:
		lck::control___<table_> _C( void )
		{
			return _S.Control;
		}
		const table_ &_RO( void )
		{
			return _C().GetReadOnly();
		}
		table_ &_RW( void )
		{
			return _C().GetReadWrite();
		}
		void _RRO( void )
		{
			_C().ReleaseReadOnly();
		}
		void _RRW( void )
		{
			_C().ReleaseReadWrite();
		}
	public:
		table_ Table;
		struct s
		{
			table_::s Table;
			lck::control___<table_> Control;
		} &_S;
		thread_safe_table_( s &S )
		: _S( S ),
		  Table( S.Table )
		{}
		void reset( bso::bool__ P = true )
		{
			Table.reset( P );
			_S.Control.reset( P );
		}
		void plug( mmm::E_MULTIMEMORY_ &MM )
		{
			Table.plug( MM );
		}
		thread_safe_table_ &operator =( const thread_safe_table_ &TST )
		{
			ERRl();

			return *this;	// Pour éviter un warning.
		}
		void Init( content_ &Content )
		{
			reset();

			Table.Init( Content );

			_S.Control.Init( Table );
		}
		irow__ AddIndex(
			index_ &Index,
			bso::bool__ Reindex = false );
		row__ Store( const data_ &Data );
		void Store(
			const data_ &Data,
			row__ Row );
		void Delete( row__ Row );
		row__ Search(
			const data_ &Data,
			irow__ IRow,
			bso::sign__ &Sign );
		row__ Search(
			const data_ &Data,
			irow__ IRow );
		row__ First( irow__ IRow );
		row__ Last( irow__ IRow );
		row__ Next( 
			irow__ IRow,
			row__ Row );
		row__ Previous( 
			irow__ IRow,
			row__ Row );
		mdr::size__ Amount( void );
	};

	E_AUTO( thread_safe_table )

#	define	E_DBTABLE_	thread_safe_table_
#	define	E_DBTABLE	thread_safe_table
#else
#	define	E_DBTABLE_	table_
#	define	E_DBTABLE	table
#endif

}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
