/*
	Header for the 'dbstbl' library by Claude SIMON (http://zeusw.org/intl/contact.html)
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

#ifndef DBSTBL__INC
#define DBSTBL__INC

#define DBSTBL_NAME		"DBSTBL"

#define	DBSTBL_VERSION	"$Revision$"

#define DBSTBL_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"

#include "ttr.h"

extern class ttr_tutor &DBSTBLTutor;

#if defined( XXX_DBG ) && !defined( DBSTBL_NODBG )
#define DBSTBL_DBG
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
#elif !defined( DBSTBL_NO_THREAD_SAFE )
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

	inline mdr::mode__ Convert( mode__ Mode )
	{
		switch ( Mode ) {
		case mBulk:
		case mReadWrite:
		case mAdmin:
			return mdr::mReadWrite;
			break;
		case mReadOnly:
			return mdr::mReadOnly;
			break;
		case m_Undefined:
			ERRu();
			break;
		default:
			ERRu();
			break;
		}

		return mdr::m_Undefined;	// Pour �viter un 'warning'.
	}

	typedef bch::E_BUNCH_( index_ * ) _indexes_;

	typedef bch::E_BUNCH_( rrow__ ) rrows_;
	E_AUTO( rrows );

	typedef ctn::E_XMCONTAINER_( datum_ ) data_;
	E_AUTO( data );

	typedef bch::E_BUNCH_( epeios::row__ ) rows_;
	E_AUTO( rows );

	class table_
	{
	private:
#if 0
		void _ConnectIndexToFileIfNot( irow__ Row ) const
		{
#ifdef DBSTBL_DBG
			if ( !Indexes.Exists( Row ) )
				ERRc();
#endif
			if ( !Indexes( Row )->InitializationCompleted() ) {
				if ( !S_.Content->InitializationCompleted() )
					S_.Content->CompleteInitialization();

				Indexes( Row )->CompleteInitialization();

			}
		}
#endif
		void _CompleteInitializationIfNeeded( bso::bool__ CompleteInitializationIfNeeded ) const
		{
			if ( !S_.Content->InitializationCompleted() && CompleteInitializationIfNeeded )
				S_.Content->CompleteInitialization();
		}
		dbsctt::content__ &_C( bso::bool__ CompleteInitializationIfNeeded = true ) const	// L'absence de 'const' est normale.
		{
			_Test( mReadOnly );

			_CompleteInitializationIfNeeded( CompleteInitializationIfNeeded );

			return *S_.Content;
		}
		dbsctt::content__ &_C( bso::bool__ CompleteInitializationIfNeeded = true )
		{
			_Test( mReadWrite );

			_CompleteInitializationIfNeeded( CompleteInitializationIfNeeded );

			return *S_.Content;
		}
#if 0
		const index_ &_I(
			irow__ Row,
			bso::bool__ DontConnectToFiles = false ) const
		{
			if ( !DontConnectToFiles )
				_ConnectIndexToFileIfNot( Row );

			return *Indexes( Row );
		}
		index_ &_I(
			irow__ Row,
			bso::bool__ DontConnectToFiles = false )
		{
			if ( !DontConnectToFiles )
				_ConnectIndexToFileIfNot( Row );

			return *Indexes( Row );
		}
#endif
		void _TestMode( mode__ Mode ) const
		{
			switch ( Mode ) {
			case mBulk:
				ERRc();
				break;
			case mAdmin:
				switch ( S_.Mode ) {
				case mBulk:
					ERRu();
					break;
				case mAdmin:
					break;
				case mReadWrite:
				case mReadOnly:
					ERRu();
					break;
				default:
					ERRc();
					break;
				}
				break;
			case mReadWrite:
				switch ( S_.Mode ) {
				case mBulk:
				case mAdmin:
				case mReadWrite:
					break;
				case mReadOnly:
					ERRu();
					break;
				default:
					ERRc();
					break;
				}
				break;
			case mReadOnly:
				break;
				switch ( S_.Mode ) {
				case mBulk:
					ERRu();
					break;
				case mAdmin:
				case mReadWrite:
				case mReadOnly:
					break;
				default:
					ERRc();
					break;
				}
				break;
			default:
				ERRc();
				break;
			}
		}
		void _Test( mode__ Mode )
		{
			_TestMode( Mode );

			switch ( S_.Mode ) {
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
		void _Test( mode__ Mode ) const
		{
			_TestMode( Mode );

			switch ( S_.Mode ) {
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
#if 0
		void _Reindex(
			irow__ Row,
			observer_functions__ &Observer );
#endif
		void _InsertInIndexes( rrow__ Row );
		void _DeleteFromIndexes( rrow__ Row );
		void _ReindexAll( observer_functions__ &Observer );
		void _ResetAllIndexes( void );
		bso::bool__ _IsBulk( void ) const
		{
			switch ( S_.Mode ) {
			case mBulk:
				return true;
			case mReadWrite:
			case mReadOnly:
			case mAdmin:
				return false;
				break;
			default:
				ERRu();
				return false;	// Pour �viter un warning.
				break;
			}
		}
	protected:
		virtual void DBSTBLErasePhysically( void )
		{}
	public:
		struct s
		{
			_indexes_::s Indexes;
			dbsctt::content__ *Content;	// On n'utilise pas 'content_', pour pouvoir appliquer
										// une op�ration non 'const' dans une m�thode 'const'.
			mode__ Mode;
		} &S_;
		_indexes_ Indexes;
		table_( s &S )
		: S_( S ),
		  Indexes( S.Indexes )
		{}
		void reset( bso::bool__ P = true )
		{
			Indexes.reset( P );
			S_.Content = NULL;
			S_.Mode = m_Undefined;
		}
		E_VDTOR( table_ )	// Pour qu'un 'delete' sur cette classe appelle le destructeur de la classe h�ritante.
		void plug( mmm::E_MULTIMEMORY_ &MM )
		{
			Indexes.plug( MM );
		}
		table_ &operator =( const table_ &T )
		{
			Indexes = T.Indexes;
			S_.Content = T.S_.Content;
			S_.Mode = T.S_.Mode;

			return *this;
		}
		void Init(
			dbsctt::content__ &Content,
			mode__ Mode )
		{
			reset();

			Indexes.Init();

			S_.Content = &Content;
			S_.Mode = Mode;
		}
		E_NAVt( _C()., rrow__ );
		dbsctt::content__ &Content( bso::bool__ CompleteInitializationIfNeeded )
		{
			return _C( CompleteInitializationIfNeeded );
		}
		dbsctt::content__ &Content( bso::bool__ CompleteInitializationIfNeeded ) const	// L'absence de 'const' est normal.
		{
			return _C( CompleteInitializationIfNeeded );
		}
		void AddIndex( index_ &Index )
		{
			_Test( mAdmin );

			Indexes.Append( &Index );
		}
		rrow__ Insert( const datum_ &Datum )
		{
			_Test( mReadWrite );

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
			_Test( mReadWrite );

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
			_Test( mReadOnly );

			_C().Retrieve( Row, Datum );
		}
		void Retrieve(
			const rrows_ &Rows,
			data_ &Data ) const;
		void Delete( rrow__ RecordRow )
		{
			_Test( mReadWrite );

			_C().Erase( RecordRow );

			if ( !_IsBulk() )
				_DeleteFromIndexes( RecordRow );
		}
		void Delete( const rrows_ &RecordRows );
#if 0
		rrow__ LooseSeek(
			const datum_ &Datum,
			irow__ IRow,
			behavior__ EqualBehavior,
			skip_level__ SkipLevel,
			bso::sign__ &Sign ) const
		{
			_Test( mReadOnly );

			if ( _IsBulk() )
				ERRu();

			return _I( IRow ).LooseSeek( Datum, EqualBehavior, SkipLevel, Sign );
		}
		rrow__ StrictSeek(
			const datum_ &Datum,
			irow__ IRow,
			behavior__ EqualBehavior,
			skip_level__ SkipLevel ) const
		{
			_Test( mReadOnly );

			if ( _IsBulk() )
				ERRu();

			return _I( IRow ).StrictSeek( Datum, EqualBehavior, SkipLevel );
		}
		bso::sign__ Compare(
			rrow__ RecordRow,
			const datum_&Pattern,
			irow__ IndexRow,
			skip_level__ SkipLevel ) const
		{
			_Test( mReadOnly );

			return _I( IndexRow ).Compare( RecordRow, Pattern, SkipLevel );
		}
		rrow__ First( irow__ IRow ) const
		{
			_Test( mReadOnly );

			if ( IRow == NONE )
				return C_().First();
			else if ( _IsBulk() )
				ERRu();

			return _I( IRow ).First();
		}
		rrow__ Last( irow__ IRow ) const
		{
			_Test( mReadOnly );

			if ( IRow == NONE )
				return C_().Last();
			else if ( _IsBulk() )
				ERRu();

			return _I( IRow ).Last();
		}
		rrow__ Next( 
			irow__ IRow,
			rrow__ Row ) const
		{
			_Test( mReadOnly );

			if ( IRow == NONE )
				return C_().Next( Row );
			else if ( _IsBulk() )
				ERRu();

			return _I( IRow ).Next( Row );
		}
		rrow__ GetStrictGreater(
			irow__ IRow,
			rrow__ Row,
			skip_level__ SkipLevel) const
		{
			_Test( mReadOnly );

			if ( _IsBulk() )
				ERRu();

			return _I( IRow ).StrictGreater( Row, SkipLevel );
		}
		rrow__ Previous( 
			irow__ IRow,
			rrow__ Row ) const
		{
			_Test( mReadOnly );

			if ( IRow == NONE )
				return C_().Previous( Row );
			else if ( _IsBulk() )
				ERRu();

			return _I( IRow ).Previous( Row );
		}
#endif
		mode__ SwitchMode( mode__ Mode )
		{
			mode__ OldMode = S_.Mode;

			switch ( Mode ) {
			case mBulk:
//				_ResetAllIndexes();
				break;
			case mAdmin:
			case mReadWrite:
			case mReadOnly:
				switch( OldMode ) {
				case mBulk:
					if ( !AreAllIndexesSynchronized() )
						ERRu();
					break;
				case mAdmin:
				case mReadOnly:
				case mReadWrite:
					break;
				default:
					ERRc();
					break;
				}
				break;
			default:
				ERRu();
				break;
			}

			S_.Mode = Mode;

			return OldMode;
		}
		mode__ Mode( void ) const
		{
			return S_.Mode;
		}
		bso::bool__ RecordExists( rrow__ RecordRow ) const
		{
			_Test( mReadOnly );

			return _C().Exists( RecordRow );
		}
		// 'Rows' contient les position dans 'RecordRows' des enregistrement inexistants.
		void TestRecordsExistence(
			const rrows_ &RecordRows,
			rows_ &Rows ) const;
#if 0
		void Reindex(
			irow__ IndexRow,
			observer_functions__ &Observer = *(observer_functions__ *)NULL )
		{
			_Reindex( IndexRow, Observer );
		}
#endif
		void ReindexAll( observer_functions__ &Observer = *(observer_functions__ *)NULL )
 		{
			_ReindexAll( Observer );
		}
#if 0
		bso::bool__ IsIndexSynchronized( irow__ IndexRow ) const
		{
			return _I( IndexRow ).IsSynchronized();
		}
#endif
		bso::bool__ AreAllIndexesSynchronized( void ) const;
		void ErasePhysically( void )
		{
			DBSTBLErasePhysically();
		}
	};

	E_AUTO( table )

#ifdef DBSTBL__THREAD_SAFE

#	ifdef DBSTBL_DEFAULT_DELAY
#		define DBSTBL__DEFAULT_DELAY	DBSTBL_DEFAULT_DELAY
#	else
#		define DBSTBL__DEFAULT_DELAY	100	// en ms.
#	endif

	class thread_safe_table_
	{
	private:
		lck::control___<table_> &_C( void )
		{
			return S_.Control;
		}
		table_ &_Lock( void )
		{
			return _C().GetExclusiveAccess();
		}
		void _Release( void )
		{
			_C().ReleaseExclusiveAccess();
		}
	protected:
		virtual void DBSTBLErasePhysically( void );
	public:
		table_ Table;
		struct s
		{
			table_::s Table;
			lck::control___<table_> Control;
		} &S_;
		thread_safe_table_( s &S )
		: S_( S ),
		  Table( S.Table )
		{}
		void reset( bso::bool__ P = true )
		{
			Table.reset( P );
			S_.Control.reset( P );
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
			dbsctt::content__ &Content,
			mode__ Mode )
		{
			reset();

			Table.Init( Content, Mode );

			S_.Control.Init( Table );
		}
		void AddIndex( index_ &Index );
		rrow__ Insert( const datum_ &Datum );
		void Insert(
			const data_ &Data,
			rrows_ &RecordRows );
		void Update(
			const datum_ &Datum,
			rrow__ RecordRow );
		void Update(
			const data_ &Data,
			const rrows_ &RecordRows );
		void Retrieve(
			rrow__ Row,
			datum_ &Datum );
		void Retrieve(
			const rrows_ &Rows,
			data_ &Data );
		void Delete( rrow__ RecordRow );
		void Delete(
			const rrows_ &RecordRows );
#if 0
		rrow__ LooseSeek(
			const datum_ &Datum,
			irow__ IRow,
			behavior__ EqualBehavior,
			skip_level__ SkipLevel,
			bso::sign__ &Sign );
		rrow__ StrictSeek(
			const datum_ &Datum,
			irow__ IRow,
			behavior__ EqualBehavior,
			skip_level__ SkipLevel );
		bso::sign__ Compare(
			rrow__ RecordRow,
			const datum_&Pattern,
			irow__ IndexRow,
			skip_level__ SkipLevel );
		rrow__ First( irow__ IRow );
		rrow__ Last( irow__ IRow );
		rrow__ Next( 
			irow__ IRow,
			rrow__ Row );
		rrow__ GetStrictGreater( 
			irow__ IRow,
			rrow__ Row,
			skip_level__ SkipLevel );
		rrow__ Previous( 
			irow__ IRow,
			rrow__ Row );
#endif
		mdr::size__ Amount( void );
		mdr::size__ Extent( void );
		mode__ SwitchMode( mode__ Mode );
		mode__ Mode( void );
		bso::bool__ RecordExists( rrow__ RecordRow );
		// 'Rows' contient les position dans 'RecordRows' des enregistrement inexistants.
		void TestRecordsExistence(
			const rrows_ &RecordRows,
			rows_ &Rows );
#if 0
		void Reindex(
			irow__ IndexRow,
			observer_functions__ &Observer = *(observer_functions__ *)NULL );
#endif
		void ReindexAll( observer_functions__ &Observer = *(observer_functions__ *)NULL );
#if 0
		bso::bool__ IsIndexSynchronized( irow__ IndexRow );
#endif
		bso::bool__ AreAllIndexesSynchronized( void );
		void ErasePhysically( void )
		{
			DBSTBLErasePhysically();
		}
	};

	E_AUTO( thread_safe_table )

#	define	E_DBTABLE_	thread_safe_table_
#	define	E_DBTABLE	thread_safe_table
#else
#	define	E_DBTABLE_	table_
#	define	E_DBTABLE	table
#endif

	class file_table
	: public table
	{
	private:
		// Seulement l'un des deux est utilis�.
		dbsdct::file_dynamic_content _Dynamic;
		dbssct::file_static_content _Static;
	protected:
		virtual void DBSTBLErasePhysically( void )
		{
			table::ErasePhysically();
			Content.ErasePhysically();
		}
	public:
		content__ Content;
		str::string RootFileName;
		void InitStatic(
			epeios::size__ Size,
			const str::string_ &Path,
			const str::string_ &RootFileName,
			mode__ Mode,
			flm::id__ ID );
		void InitDynamic(
			const str::string_ &Path,
			const str::string_ &RootFileName,
			mode__ Mode,
			flm::id__ ID );
	};

	typedef file_table file_table_;


}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
