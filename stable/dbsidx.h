/*
	Header for the 'dbsidx' library by Claude SIMON (csimon@epeios.org)
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

#ifndef DBSIDX__INC
#define DBSIDX__INC

#define DBSIDX_NAME		"DBSIDX"

#define	DBSIDX_VERSION	"$Revision$"

#define DBSIDX_OWNER		"Claude SIMON (csimon@epeios.org)"

#include "ttr.h"

extern class ttr_tutor &DBSIDXTutor;

#if defined( XXX_DBG ) && !defined( DBSIDX_NODBG )
#define DBSIDX_DBG
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
//D DataBaSe Index 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "idxbtq.h"
#include "dbsctt.h"
#include "dbsbsc.h"

namespace dbsidx {
	using dbsbsc::datum_;
	using dbsbsc::datum;

	using dbsbsc::rrow__;

	class sort_function__
	{
	protected:
		virtual bso::sign__ DBSIDXCompare(
			const datum_ &Datum1,
			const datum_ &Datum2 ) = 0;
	public:
		bso::sign__ Compare(
			const datum_ &Data1,
			const datum_ &Data2 )
		{
			return DBSIDXCompare( Data1, Data2 );
		}
	};

	typedef idxbtq::E_INDEXt_( rrow__ ) _index_;
	E_AUTO( _index )

	struct extremities__
	{
		rrow__ Smallest, Greatest;
		bso::ulong__ Used;
		extremities__( void )
		{
			Smallest = Greatest = NONE;
			Used = 0;
		}
	};

	// Pour idiquer le comportement d'un "seek' lorsqu'il tombe sur un élément qu est égal à celui recherché.
	enum behavior__ {
		// On stoppe la recherche.
		bStop,
		// On cherche le plus grand
		bGreater,
		// On cherche le plus petit
		bLesser,
		// On stoppe s'il manque l'un des fils. (sert lors d'une indexation).
		bStopIfOneChildMissing,
		b_amount,
		b_Undefined
	};

	typedef dbsbsc::file_features_	_file_features_;

	class index_
	: public _file_features_
	{
	protected:
		virtual void DBSBSCCompleteInitialization( void )
		{
			// Rien à faire.
		}
		virtual void DBSBSCDrop( void )
		{
			reset();
		}
	private:
		bso::sign__ _Seek(
			const datum_ &Data,
			behavior__ StopIfEqual,
			rrow__ &Row,
			bso::ubyte__ &Round,
			dbsctt::_cache_ &Cache ) const;
		const dbsctt::content__ &_Content( void ) const
		{
			return *S_.Content;
		}
		void _Retrieve(
			rrow__ Row,
			datum_ &Datum,
			dbsctt::_cache_ &Cache ) const
		{
			_Content().Retrieve( Row, Datum, Cache );
		}
		void _Touch( bso::bool__ CompareWithContent )
		{
			S_.ModificationTimeStamp = tol::Clock( false );

			if ( CompareWithContent && ( S_.ModificationTimeStamp == Content().ModificationTimeStamp() ) )
				S_.ModificationTimeStamp = tol::Clock( true );
		}
	public:
		_index_ BaseIndex;
		struct s
		: public _file_features_::s
		{
			_index_::s BaseIndex;
			rrow__ Root;
			sort_function__ *Sort;
			const dbsctt::content__ *Content;
			time_t ModificationTimeStamp;
		} &S_;
		index_( s &S )
		: S_( S ),
		  _file_features_( S ),
		  BaseIndex( S.BaseIndex )
		{}
		void reset( bso::bool__ P = true )
		{
			BaseIndex.reset( P );
			S_.Root = NONE;

			S_.Sort = NULL;
			S_.Content = NULL;

			S_.ModificationTimeStamp = 0;

			_file_features_::reset( P );
		}
		void plug( mmm::E_MULTIMEMORY_ &MM )
		{
			BaseIndex.plug( MM );
		}
		index_ &operator =( const index_ &I )
		{
			BaseIndex = I.BaseIndex;
			S_.Root = I.S_.Root;

			S_.Sort = I.S_.Sort;
			S_.Content = I.S_.Content;

			S_.ModificationTimeStamp = I.S_.ModificationTimeStamp;

			return *this;
		}
		void Init(
			const dbsctt::content__ &Content = *(dbsctt::content__ *)NULL,
			sort_function__ &Sort = *(sort_function__ *)NULL,
			bso::bool__ Partial = false )
		{
			BaseIndex.Init();
			S_.Root = NONE;

			S_.Content = &Content;
			S_.Sort = &Sort;

			S_.ModificationTimeStamp = 0;

			_file_features_::Init( Partial );
		}
		// Vide l'index.
		void Reset( void )
		{
			S_.Root = NONE;
			BaseIndex.Init();

			S_.ModificationTimeStamp = 0;
		}
		void Allocate( mdr::size__ Size )
		{
			BaseIndex.Allocate( Size );
		}
		bso::ubyte__ Index(
			rrow__ Row,
			extremities__ *Extremities,	// Si à 'true', compare d'abord avec les extrémités. Utilisé pour la réindexation.
			dbsctt::_cache_ &Cache  = *(dbsctt:: _cache_ *)NULL );	// Retourne le nombre de noeuds parcourus pour cette indexation.
		void Delete( rrow__ Row )
		{
#ifdef DBSIDX_DBG
			if ( S_.Root == NONE )
				ERRu();
#endif
			S_.Root = BaseIndex.Delete( Row, S_.Root );

			_Touch( false );
		}
		rrow__ Seek(
			const datum_ &Datum,
			behavior__ EqualBehavior,
			bso::sign__ &Sign ) const;
		rrow__ Seek(
			behavior__ EqualBehavior,
			const datum_ &Datum ) const
		{
			bso::sign__ Sign;

			return Seek( Datum, EqualBehavior, Sign );
		}
		bso::sign__ Compare(
			rrow__ RecordId,
			const datum_ &Pattern ) const;
		rrow__ SearchRoot( void )
		{
			ERRl();

			return S_.Root;
		}
		rrow__ First( void ) const
		{
			if ( S_.Root != NONE )
				return BaseIndex.First( S_.Root );
			else
				return NONE;
		}
		rrow__ Last( void ) const
		{
			if ( S_.Root != NONE )
				return BaseIndex.Last( S_.Root );
			else
				return NONE;
		}
		rrow__ Next( rrow__ Row ) const
		{
			return BaseIndex.Next( Row );
		}
		rrow__ Previous( rrow__ Row ) const
		{
			return BaseIndex.Previous( Row );
		}
		mdr::size__ Amount( void ) const
		{
			return BaseIndex.Amount();
		}
		bso::bool__ IsSynchronized( void ) const
		{
			return S_.ModificationTimeStamp > _Content().ModificationTimeStamp();
		}
		sort_function__ &SortFunction( void ) const
		{
			return *S_.Sort;
		}
		const dbsctt::content__ &Content( void ) const
		{
			return *S_.Content;
		}
		void Balance( void )
		{
			if ( S_.Root != NONE )
				S_.Root = BaseIndex.Balance( S_.Root );

			_Touch( false );
		}
		rrow__ CompareTreeAndQueue( void ) const
		{
			if ( S_.Root != NONE )
				return BaseIndex.Compare( S_.Root );
			else
				return NONE;
		}
		rrow__ Test( void ) const;
		void Drop( void )
		{
			_file_features_::Drop();
		}
		E_RODISCLOSE_( time_t, ModificationTimeStamp );
	};

	E_AUTO( index )

	// 'index' stockés dans des fichiers.

	class file_index_
	: public index_
	{
	protected:
		virtual void DBSBSCCompleteInitialization( void )
		{
			_ConnectToFiles();
		}
		virtual void DBSBSCDrop( void )
		{
			index_::DBSBSCDrop();
			_Drop();
		}
	private:
		void _SaveRoot( void ) const;
		bso::bool__ _ConnectToFiles( void );
		void _Drop( void );
	public:
		str::string_ RootFileName;
		struct s
		: public index_::s
		{
			struct memory_driver__ {
				flm::E_FILE_MEMORY_DRIVER___ Tree;
				flm::E_FILE_MEMORY_DRIVER___ Queue;
			} MemoryDriver;
			str::string_::s RootFileName;
			bso::bool__ Erase;	// Seulement utile lors d'uen initialisation retardée.
			mdr::mode__ Mode;
		} &S_;
		file_index_( s &S )
		: S_( S ), 
		  index_( S ),
		  RootFileName( S.RootFileName )
		{}
		void reset( bso::bool__ P = true )
		{
			if ( P ) {
				if ( ( RootFileName.Amount() != 0 ) && ModificationTimeStamp() != 0 )
					_SaveRoot();
			}

			S_.MemoryDriver.Tree.reset( P );
			S_.MemoryDriver.Queue.reset( P );
			RootFileName.reset( P );
			index_::reset( P );
			S_.Erase = false;
			S_.Mode = mdr::m_Undefined;
		}
		void plug( mmm::E_MULTIMEMORY_ & )
		{
			ERRu();	// Cette méthode n'a pas de sens dans ce contexte.
		}
		file_index_ &operator =( const file_index_ &FI )
		{
			index_::operator =( FI );

			return *this;
		}
		void Init(
			const str::string_ &RootFileName,
			const dbsctt::content__ &Content,
			sort_function__ &Sort,
			mdr::mode__ Mode,
			bso::bool__ Erase,
			bso::bool__ Partial )
		{
			reset();

			this->RootFileName.Init( RootFileName );

			index_::Init( Content, Sort, Partial );

			S_.Mode = Mode;
			S_.Erase = Erase;
		}
		void CloseFiles( void )
		{
			S_.MemoryDriver.Tree.Liberer();
			S_.MemoryDriver.Queue.Liberer();
		}
	};

	E_AUTO( file_index )




}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
