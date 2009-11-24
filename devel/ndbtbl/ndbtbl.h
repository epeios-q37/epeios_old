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
		ndbctt::content__ &_C( bso::bool__ CompleteInitializationIfNeeded = true ) const	// L'absence de 'const' est normale.
		{
			_Test( mReadOnly );

//			_CompleteInitializationIfNeeded( CompleteInitializationIfNeeded );

			return *S_.Content;
		}
		ndbctt::content__ &_C( bso::bool__ CompleteInitializationIfNeeded = true )
		{
			_Test( mReadWrite );

//			_CompleteInitializationIfNeeded( CompleteInitializationIfNeeded );

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
				return false;	// Pour éviter un warning.
				break;
			}
		}
	public:
		struct s
		{
			str::string_ ::s Label;
			_indexes_::s Indexes;
			ndbctt::content__ *Content;	// On n'utilise pas 'content_', pour pouvoir appliquer
										// une opération non 'const' dans une méthode 'const'.
			mode__ Mode;
		} &S_;
		str::string_ Label;	// Sert d'identifiant discriminat de la table. Utile pour servir de base aux noms de fichiers associées à la table.
		_indexes_ Indexes;
		table_( s &S )
		: S_( S ),
		  Label( S.Label ),
		  Indexes( S.Indexes )
		{}
		void reset( bso::bool__ P = true )
		{
			Label.reset();
			Indexes.reset( P );
			S_.Content = NULL;
			S_.Mode = m_Undefined;
		}
		E_VDTOR( table_ )	// Pour qu'un 'delete' sur cette classe appelle le destructeur de la classe héritante.
		void plug( mmm::E_MULTIMEMORY_ &MM )
		{
			Label.plug( MM );
			Indexes.plug( MM );
		}
		table_ &operator =( const table_ &T )
		{
			Label = T.Label;
			Indexes = T.Indexes;
			S_.Content = T.S_.Content;
			S_.Mode = T.S_.Mode;

			return *this;
		}
		void Init(
			const str::string_ &Label,
			ndbctt::content__ &Content,
			mode__ Mode )
		{
			reset();

			Indexes.Init();

			this->Label.Init( Label );
			S_.Content = &Content;
			S_.Mode = Mode;
		}
		E_NAVt( _C()., rrow__ );
		ndbctt::content__ &Content( bso::bool__ CompleteInitializationIfNeeded )
		{
			return _C( CompleteInitializationIfNeeded );
		}
		ndbctt::content__ &Content( bso::bool__ CompleteInitializationIfNeeded ) const	// L'absence de 'const' est normal.
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
		void ReindexAll( observer_functions__ &Observer = *(observer_functions__ *)NULL )
 		{
			_ReindexAll( Observer );
		}
		bso::bool__ AreAllIndexesSynchronized( void ) const;
	};

	E_AUTO( table )


}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
