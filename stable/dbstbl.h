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
#include "dbsctt.h"
#include "dbsidx.h"
#include "cpe.h"

#ifdef DBSTBL_THREAD_SAFE
#	define DBSTBL__THREAD_SAFE
#elif !defined( DBSTBL_NO_THREAD_SAFE
#	ifdef CPE__T_MT
#		define DBSTBL__THREAD_SAFE
#	endif
#endif

#ifdef DBSTBL__THREAD_SAFE
#	include "lck.h"
#endif

namespace dbstbl {

	using namespace dbsctt;
	using namespace dbsidx;

	E_ROW( irow__ );

	enum mode__ {
		// Pas d'indexation, ou diff�r�e.
		mBulk,
		// Lecture seule.
		mReadOnly,
		// Lecture/�criture.
		mReadWrite,
		// Administrateur : changement de la structure  (cr�ation/suppression d'index, p. ex.).
		mAdmin,
		m_amount,
		m_Undefined
	};

	typedef bch::E_BUNCHt_( index_ *, irow__ ) _indexes_;

	class observer_functions__
	{
	private:
		// Dur�e entre deuw appels en ms.
		time_t _Delay;
		bso::ulong__ _CurrentIndex, _IndexAmount;
	protected:
		virtual void DBSTBLNotify(
			bso::ulong__ CurrentIndex,
			bso::ulong__ IndexAmount,
			bso::ulong__ CurrentRecord,
			bso::ulong__ RecordAmount ) = 0;
	public:
		observer_functions__( void )
		{
			_Delay = 1000;	// D�lai par d�faut : 1 s.
			_CurrentIndex = _IndexAmount = 0;
		}
		void SetDelay( time_t Delay )
		{
			_Delay = Delay;
		}
		void Notify(
			bso::ulong__ CurrentRecord,
			bso::ulong__ RecordAmount )
		{
			DBSTBLNotify( _CurrentIndex, _IndexAmount, CurrentRecord, RecordAmount );
		}
		friend class table_;
	};

	typedef bch::E_BUNCH_( rrow__ ) rrows_;
	E_AUTO( rrows );

	typedef ctn::E_XMCONTAINER_( datum_ ) data_;
	E_AUTO( data );

	typedef bch::E_BUNCH_( epeios::row__ ) rows_;
	E_AUTO( rows );

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
		void _Test( void )
		{
			switch ( _S.Mode ) {
			case mBulk:
			case mReadWrite:
			case mAdmin:
				break;
			case mReadOnly:
				ERRu();
				break;
			default:
				ERRu();
				break;
			}
		}
		void _Test( void ) const
		{
			switch ( _S.Mode ) {
			case mBulk:
			case mReadWrite:
			case mReadOnly:
			case mAdmin:
				break;
			default:
				ERRu();
				break;
			}
		}
		void _Reindex(
			irow__ Row,
			observer_functions__ &Observer );
		void _InsertInIndexes( rrow__ Row );
		void _DeleteFromIndexes( rrow__ Row );
		void _ReindexAll( observer_functions__ &Observer );
		void _ResetAllIndexes( void );
		bso::bool__ _IsBulk( void ) const
		{
			switch ( _S.Mode ) {
			case mBulk:
				return true;
			case mReadWrite:
			case mReadOnly:
				return false;
				break;
			default:
				ERRu();
				return false;	// Pour �viter un warning.
				break;
			}
		}
	public:
		_indexes_ Indexes;
		struct s
		{
			_indexes_::s Indexes;
			content_ *Content;
			mode__ Mode;
		} &_S;
		table_( s &S )
		: _S( S ),
		Indexes( S.Indexes )
		{}
		void reset( bso::bool__ P = true )
		{
			Indexes.reset( P );
			_S.Content = NULL;
			_S.Mode = m_Undefined;
		}
		void plug( mmm::E_MULTIMEMORY_ &MM )
		{
			Indexes.plug( MM );
		}
		table_ &operator =( const table_ &T )
		{
			Indexes = T.Indexes;
			_S.Content = T._S.Content;
			_S.Mode = T._S.Mode;

			return *this;
		}
		void Init(
			content_ &Content,
			mode__ Mode = mAdmin )
		{
			reset();

			Indexes.Init();

			_S.Content = &Content;
			_S.Mode = Mode;
		}
		irow__ AddIndex(
			index_ &Index,
			bso::bool__ Reindex = false,
			observer_functions__ &Observer = *(observer_functions__ *)NULL )
		{
			if ( _S.Mode != mAdmin )
				ERRu();

			_Test();

			irow__ Row = Indexes.Append( &Index );

			if ( Reindex ) {
				if ( &Observer != NULL ) {
					Observer._CurrentIndex = 1;
					Observer._IndexAmount = 1;
				}
				_Reindex( Row, Observer );
			} else if ( Index.Amount() != _C().Amount() )
				ERRu();

			return Row;
		}
		rrow__ Insert( const datum_ &Datum )
		{
			_Test();

			rrow__ Row = _C().Store( Datum );

			if ( !_IsBulk() )
				_InsertInIndexes( Row );

			return Row;
		}
		void Insert(
			const data_ &Data,
			rrows_ &RecordRows );
		void Update(
			const datum_ &Datum,
			rrow__ RecordRow )
		{
			_Test();

			if ( !_IsBulk() )
				_DeleteFromIndexes( RecordRow );

			_C().Store( Datum, RecordRow );

			if ( !_IsBulk() )
				_InsertInIndexes( RecordRow );
		}
		void Update(
			const data_ &Data,
			const rrows_ &RecordRows );
		void Retrieve(
			rrow__ Row,
			datum_ &Datum ) const
		{
			_Test();

			_C().Retrieve( Row, Datum );
		}
		void Retrieve(
			rrows_ Row,
			data_ &Data ) const;
		void Delete( rrow__ RecordRow )
		{
			_Test();

			_C().Erase( RecordRow );

			if ( !_IsBulk() )
				_DeleteFromIndexes( RecordRow );
		}
		void Delete( const rrows_ &RecordRows );
		rrow__ Seek(
			const datum_ &Datum,
			irow__ IRow,
			bso::sign__ &Sign ) const
		{
			_Test();

			if ( _IsBulk() )
				ERRu();

			return _I( IRow ).Seek( Datum, Sign );
		}
		rrow__ Seek(
			const datum_ &Datum,
			irow__ IRow ) const
		{
			bso::sign__ Sign;

			_Test();

			return Seek( Datum, IRow, Sign );
		}
		bso::bool__ Begins(
			rrow__ RecordRow,
			const datum_ &Pattern,
			irow__ IndexRow ) const
		{
			_Test();

			if ( _IsBulk() )
				ERRu();

			return _I( IndexRow).Begins( RecordRow, Pattern );
		}
		bso::sign__ Compare(
			rrow__ RecordRow,
			const datum_&Pattern,
			irow__ IndexRow ) const
		{
			_Test();

			return _I( IndexRow ).Compare( RecordRow, Pattern );
		}
		rrow__ First( irow__ IRow ) const
		{
			_Test();

			if ( IRow == NONE )
				return _C().First();
			else if ( _IsBulk() )
				ERRu();

			return _I( IRow ).First();
		}
		rrow__ Last( irow__ IRow ) const
		{
			_Test();

			if ( IRow == NONE )
				return _C().Last();
			else if ( _IsBulk() )
				ERRu();

			return _I( IRow ).Last();
		}
		rrow__ Next( 
			irow__ IRow,
			rrow__ Row ) const
		{
			_Test();

			if ( IRow == NONE )
				return _C().Next( Row );
			else if ( _IsBulk() )
				ERRu();

			return _I( IRow ).Next( Row );
		}
		rrow__ Previous( 
			irow__ IRow,
			rrow__ Row ) const
		{
			_Test();

			if ( IRow == NONE )
				return _C().Previous( Row );
			else if ( _IsBulk() )
				ERRu();

			return _I( IRow ).Previous( Row );
		}
		mdr::size__ Amount( void ) const
		{
			_Test();

			return _C().Amount();
		}
		mode__ SwitchMode(
			mode__ Mode,
			observer_functions__ &Observer = *(observer_functions__ *)NULL )
		{
			mode__ OldMode = _S.Mode;

			switch ( Mode ) {
			case mBulk:
				_ResetAllIndexes();
				break;
			case mAdmin:
			case mReadWrite:
			case mReadOnly:
				switch( OldMode ) {
				case mBulk:
					_ReindexAll( Observer );
					break;
				case mReadOnly:
				case mReadWrite:
					break;
				default:
					ERRc();
					break;
				}
			default:
				ERRu();
				break;
			}

			_S.Mode = Mode;

			return OldMode;
		}
		mode__ Mode( void ) const
		{
			_Test();

			return _S.Mode;
		}
		bso::bool__ RecordExists( rrow__ RecordRow ) const
		{
			return _C().Exists( RecordRow );
		}
		// 'Rows' contient les position dans 'RecordRows' des enregistrement inexistants.
		void TestRecordsExistence(
			const rrows_ &RecordRows,
			rows_ &Rows ) const;
	};

	E_AUTO( table )

#ifdef DBSTBL__THREAD_SAFE

#	ifdef DBSTBL_DEFAULT_DELAY
#		define DBSTBL__DEFAULT_DELAY	DBSTBL_DEFAULT_DELAY
#	else
#		define DBSTBL__DEFAULT_DELAY	100	// en ms.
#endif

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

			return *this;	// Pour �viter un warning.
		}
		void Init(
			content_ &Content,
			mode__ Mode = mAdmin )
		{
			reset();

			Table.Init( Content, Mode );

			_S.Control.Init( Table );
		}
		irow__ AddIndex(
			index_ &Index,
			bso::bool__ Reindex = false,
			observer_functions__ &Observer = *(observer_functions__ *)NULL );
		rrow__ Insert( const datum_ &Datum );
		void Insert(
			const data_ &Data,
			rrows_ &RecordRows,
			time_t Delay = DBSTBL__DEFAULT_DELAY );
		void Update(
			const datum_ &Datum,
			rrow__ RecordRow );
		void Update(
			const data_ &Data,
			const rrows_ &RecordRows,
			time_t Delay = DBSTBL__DEFAULT_DELAY );
		void Retrieve(
			rrow__ Row,
			datum_ &Datum );
		// Toute les 'Delay' ms, le 'thread' rend la main pour donner une chance aux autres 'thread's de s'ex�cuter.
		void Retrieve(
			rrows_ Rows,
			data_ &Data,
			time_t Delay = DBSTBL__DEFAULT_DELAY );
		void Delete( rrow__ RecordRow );
		void Delete(
			const rrows_ &RecordRows,
			time_t Delay = DBSTBL__DEFAULT_DELAY );
		rrow__ Seek(
			const datum_ &Datum,
			irow__ IRow,
			bso::sign__ &Sign );
		rrow__ Seek(
			const datum_ &Datum,
			irow__ IRow );
		bso::bool__ Begins(
			rrow__ RecordRow,
			const datum_ &Datum,
			irow__ IRow );
		bso::sign__ Compare(
			rrow__ RecordRow,
			const datum_&Pattern,
			irow__ IndexRow );
		rrow__ First( irow__ IRow );
		rrow__ Last( irow__ IRow );
		rrow__ Next( 
			irow__ IRow,
			rrow__ Row );
		rrow__ Previous( 
			irow__ IRow,
			rrow__ Row );
		mdr::size__ Amount( void );
		mode__ SwitchMode(
			mode__ Mode,
			observer_functions__ &Observer = *(observer_functions__ *)NULL );
		mode__ Mode( void );
		bso::bool__ RecordExists( rrow__ RecordRow );
		// 'Rows' contient les position dans 'RecordRows' des enregistrement inexistants.
		void TestRecordsExistence(
			const rrows_ &RecordRows,
			rows_ &Rows,
			time_t Delay = DBSTBL__DEFAULT_DELAY );
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
