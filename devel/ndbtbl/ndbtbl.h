/*
	Header for the 'ndbtbl' library by Claude SIMON (csimon at zeusw dot org)
	Copyright (C) $COPYRIGHT_DATES$Claude SIMON.
$_RAW_$
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

#ifndef NDBTBL__INC
#define NDBTBL__INC

#define NDBTBL_NAME		"NDBTBL"

#define	NDBTBL_VERSION	"$Revision$"

#define NDBTBL_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &NDBTBLTutor;

#if defined( XXX_DBG ) && !defined( NDBTBL_NODBG )
#define NDBTBL_DBG
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
//D New DataBase TaBLe (will be moved on its own project in the future) 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "ndbbsc.h"
#include "ndbctt.h"
#include "ndbidx.h"

namespace ndbtbl {
	using namespace ndbctt;
	using namespace ndbidx;

	enum mode__ {
		// Pas d'indexation, ou diffèrée.
		mBulk,
		// Lecture seule.
		mReadOnly,
		// Lecture/écriture.
		mReadWrite,
		// Administrateur : changement de la structure  (création/suppression d'index, p. ex.).
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

		return mdr::m_Undefined;	// Pour éviter un 'warning'.
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
				return false;	// Pour éviter un warning.
				break;
			}
		}
	public:
		struct s
		{
			content_::s Content;
			_indexes_::s Indexes;
										// une opération non 'const' dans une méthode 'const'.
			mode__ Mode;
		} &S_;
		content_ Content;
		_indexes_ Indexes;
		table_( s &S )
		: S_( S ),
		  Content( S.Content ),
		  Indexes( S.Indexes )
		{}
		void reset( bso::bool__ P = true )
		{
			Content.reset( P );
			Indexes.reset( P );
			S_.Mode = m_Undefined;
		}
		E_VDTOR( table_ )	// Pour qu'un 'delete' sur cette classe appelle le destructeur de la classe héritante.
		void plug( mmm::E_MULTIMEMORY_ &MM )
		{
			Content.plug( MM );
			Indexes.plug( MM );
		}
		table_ &operator =( const table_ &T )
		{
			Content = T.Content;
			Indexes = T.Indexes;
			S_.Content = T.S_.Content;
			S_.Mode = T.S_.Mode;

			return *this;
		}
		void Init( mode__ Mode )
		{
			reset();

			Content.Init();
			Indexes.Init();

			S_.Mode = Mode;
		}
		void InitStatic(
			epeios::size__ Size,
			mode__ Mode )
		{
			Init( Mode );

			Content.InitStatic( Size );
		}
		void InitDynamic( mode__ Mode )
		{
			Init( Mode );

			Content.InitDynamic();
		}
		E_NAVt( Content()., rrow__ );
		void AddIndex( index_ &Index )
		{
			_Test( mAdmin );

			Indexes.Append( &Index );
		}
		rrow__ Insert( const datum_ &Datum )
		{
			_Test( mReadWrite );

			rrow__ Row = Content().Store( Datum );

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

			Content().Store( Datum, RecordRow );

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

			Content().Retrieve( Row, Datum );
		}
		void Retrieve(
			const rrows_ &Rows,
			data_ &Data ) const;
		void Delete( rrow__ RecordRow )
		{
			_Test( mReadWrite );

			Content().Erase( RecordRow );

			if ( !_IsBulk() )
				_DeleteFromIndexes( RecordRow );
		}
		void Delete( const rrows_ &RecordRows );
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

			return Content().Exists( RecordRow );
		}
		// 'Rows' contient les position dans 'RecordRows' des enregistrement inexistants.
		void TestRecordsExistence(
			const rrows_ &RecordRows,
			rows_ &Rows ) const;
		void ReindexAll( observer_functions__ &Observer = *(observer_functions__ *)NULL )
 		{
			_ReindexAll( Observer );
		}
		bso::bool__ AreAllIndexesSynchronized( void ) const;
		type__ Type( void ) const
		{
			return Content().Type();
		}
	};

	E_AUTO( table )

	class table_spreaded_file_manager___
	{
	private:
		// Seulement l'un des deux est utilisé.
		ndbdct::dynamic_content_spreaded_file_manager___ _Dynamic;
		ndbsct::static_content_spreaded_file_manager___ _Static;
		void _InitStatic(
			table_ &Table,
			const str::string_ &Path,
			const str::string_ &RootFileName,
			mode__ Mode,
			flm::id__ ID );
		void _InitDynamic(
			table_ &Table,
			const str::string_ &Path,
			const str::string_ &RootFileName,
			mode__ Mode,
			flm::id__ ID );
	public:
		void Init(
			table_ &Table,
			const str::string_ &Path,
			const str::string_ &RootFileName,
			mode__ Mode,
			flm::id__ ID )
		{
			switch ( Table.Type() ) {
			case tStatic:
				_InitStatic( Table, Path, RootFileName, Mode, ID );
				break;
			case tDynamic:
				_InitDynamic( Table, Path, RootFileName, Mode, ID );
				break;
			default:
				ERRc();
				break;
			}
		}
	};



}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
