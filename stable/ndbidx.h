/*
	Header for the 'ndbidx' library by Claude SIMON (csimon at zeusw dot org)
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

#ifndef NDBIDX__INC
#define NDBIDX__INC

#define NDBIDX_NAME		"NDBIDX"

#define	NDBIDX_VERSION	"$Revision$"

#define NDBIDX_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &NDBIDXTutor;

#if defined( XXX_DBG ) && !defined( NDBIDX_NODBG )
#define NDBIDX_DBG
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
//D New DataBase InDeXes (will be moved on its own project in the future) 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "idxbtq.h"
#include "ndbbsc.h"
#include "ndbctt.h"

namespace ndbidx {
	using ndbbsc::datum_;
	using ndbbsc::datum;

	using ndbbsc::rrow__;

	using ndbbsc::rrows_;
	using ndbbsc::rrows;

	typedef bso::ubyte__ skip_level__;

#define NDBIDX_NO_SKIP	0

	class sort_function__
	{
	protected:
		virtual bso::sign__ NDBIDXCompare(
			const datum_ &Datum1,
			const datum_ &Datum2,
			skip_level__ SkipLevel ) = 0;	// Si == 0, la comparaison se fait sur tous les champs
	public:
		bso::sign__ Compare(
			const datum_ &Datum1,
			const datum_ &Datum2,
			skip_level__ SkipLevel )
		{
			return NDBIDXCompare( Datum1, Datum2, SkipLevel );
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

	class observer_functions__
	{
	private:
		// Durée entre deuw appels en ms.
		time_t _Delay;
		bso::ulong__ _HandledIndexAmount, _TotalIndexAmount;
	protected:
		virtual void NDBTBLNotify(
			bso::ulong__ HandledIndexAmount,
			bso::ulong__ TotalIndexAmount,
			bso::ulong__ HandledRecordAmount,
			bso::ulong__ TotalRecordAmount,
			bso::ulong__ BalancingCount ) = 0;
	public:
		void reset( bso::bool__ = true )
		{
			_Delay = 1000;	// Délai par défaut : 1 s.
			_TotalIndexAmount = _HandledIndexAmount = 0;
		}
		observer_functions__( void )
		{
			reset( false );
		}
		void Init( time_t Delay = 1000 )	// Délai par défaut : 1 s.
		{
			reset();

			_Delay = Delay;
		}
		void Set( bso::ulong__ TotalIndexAmount )
		{
			_TotalIndexAmount = TotalIndexAmount;
			_HandledIndexAmount = 0;
		}
		void IncrementHandledIndexAmount( bso::ulong__ Amount = 1 )
		{
			_HandledIndexAmount += Amount;
		}
		void Notify(
			bso::ulong__ HandledRecordAmount,
			bso::ulong__ TotalRecordAmount,
			bso::ulong__ BalancingCount )
		{
			NDBTBLNotify( _HandledIndexAmount, _TotalIndexAmount, HandledRecordAmount, TotalRecordAmount, BalancingCount );
		}
		friend class index_;
	};

	class index_
	{
	private:
		bso::sign__ _Seek(
			const datum_ &Data,
			skip_level__ SkipLevel,
			behavior__ StopIfEqual,
			rrow__ &Row,
			bso::ubyte__ &Round,
			ndbctt::cache_ &Cache ) const;
		const ndbctt::content__ &_Content( bso::bool__ CompleteInitializationIfNeeded ) const
		{
/*
			if ( !S_.Content->InitializationCompleted() && CompleteInitializationIfNeeded )
				S_.Content->CompleteInitialization();
*/
			return *S_.Content;
		}
		void _Retrieve(
			rrow__ Row,
			datum_ &Datum,
			ndbctt::cache_ &Cache ) const
		{
			_Content( true ).Retrieve( Row, Datum, Cache );
		}
		void _Touch( bso::bool__ CompareWithContent )
		{
			S_.ModificationTimeStamp = tol::Clock( false );

			if ( CompareWithContent && ( S_.ModificationTimeStamp == Content( true ).ModificationTimeStamp() ) )
				S_.ModificationTimeStamp = tol::Clock( true );
		}
		rrow__ _SearchStrictGreater(
			rrow__ Row,
			skip_level__ SkipLevel ) const;
	public:
		_index_ BaseIndex;
		struct s
		{
			_index_::s BaseIndex;
			rrow__ Root;
			sort_function__ *Sort;
			const ndbctt::content__ *Content;
			time_t ModificationTimeStamp;
		} &S_;
		index_( s &S )
		: S_( S ),
		  BaseIndex( S.BaseIndex )
		{}
		void reset( bso::bool__ P = true )
		{
			BaseIndex.reset( P );
			S_.Root = NONE;

			S_.Sort = NULL;
			S_.Content = NULL;

			S_.ModificationTimeStamp = 0;
		}
		E_VDTOR( index_ )	// Pour qu'un 'delete' sur cette classe appelle le destructeur de la classe héritante.
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
			const ndbctt::content__ &Content = *(const ndbctt::content__ *)NULL,
			sort_function__ &Sort = *(sort_function__ *)NULL )
		{
			BaseIndex.Init();
			S_.Root = NONE;

			S_.Content = &Content;
			S_.Sort = &Sort;

			S_.ModificationTimeStamp = 0;
		}
		// Vide l'index.
		void Reset( void )
		{
			S_.Root = NONE;
			BaseIndex.Init();

			S_.ModificationTimeStamp = 0;
		}
		void Allocate(
			mdr::size__ Size,
			aem::mode__ Mode )
		{
			BaseIndex.Allocate( Size, Mode );
		}
		bso::ubyte__ Index(
			rrow__ Row,
			extremities__ *Extremities,	// Si à 'true', compare d'abord avec les extrémités. Utilisé pour la réindexation.
			ndbctt::cache_ &Cache  = *(ndbctt:: cache_ *)NULL );	// Retourne le nombre de noeuds parcourus pour cette indexation.
		void Delete( rrow__ Row )
		{
#ifdef NDBIDX_DBG
			if ( S_.Root == NONE )
				ERRu();
#endif
			S_.Root = BaseIndex.Delete( Row, S_.Root );

			_Touch( false );
		}
		rrow__ LooseSeek(
			const datum_ &Datum,
			behavior__ EqualBehavior,
			skip_level__ SkipLevel,
			bso::sign__ &Sign ) const;	// Retourne l'élément le plus proche, même si 
		rrow__ StrictSeek(
			const datum_ &Datum,
			behavior__ EqualBehavior,
			skip_level__ SkipLevel ) const
		{
			bso::sign__ Sign;
			rrow__ Row = LooseSeek( Datum, EqualBehavior, SkipLevel, Sign );

			switch ( Sign ) {
			case -1:
				if ( EqualBehavior == bStop )
					Row = NONE;
				else
					Row = Next( Row );
				break;
			case 0:
				break;
			case 1:
				if ( EqualBehavior == bStop )
					Row = NONE;
				else
					Row = Previous( Row );
				break;
			default:
				ERRc();
				break;
			}

			return Row;
		}
		bso::sign__ Compare(
			rrow__ RecordId,
			const datum_ &Pattern,
			skip_level__ SkipLevel ) const;
		bso::sign__ Compare(
			const datum_ &Pattern,
			rrow__ RecordId,
			skip_level__ SkipLevel ) const
		{
			return -Compare( RecordId, Pattern, SkipLevel );
		}
		bso::sign__ Compare(
			rrow__ RecordRow1,
			rrow__ RecordRow2,
			skip_level__ SkipLevel ) const;
		rrow__ SearchRoot( void )
		{	
			rrow__ Candidate = S_.Root = S_.Content->First();

			if ( Candidate != NONE ) 
				while ( ( Candidate = BaseIndex.GetTreeParent( Candidate ) ) != NONE )
					S_.Root = Candidate;

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
		rrow__ StrictGreater(
			rrow__ Row,
			skip_level__ SkipLevel ) const
		{
			rrow__ Candidate = Next( Row );

			if ( Candidate == NONE )
				return NONE;
			else if ( Compare( Row, Candidate, SkipLevel ) != 0 )
				return Candidate;
			else
				return _SearchStrictGreater( Row, SkipLevel );
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
			return S_.ModificationTimeStamp > _Content( false ).ModificationTimeStamp();
		}
		sort_function__ &SortFunction( void ) const
		{
			return *S_.Sort;
		}
		const ndbctt::content__ &Content( bso::bool__ CompleteInitializationIfNeeded ) const
		{
			return _Content( CompleteInitializationIfNeeded );
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
		void Reindex( observer_functions__ &Observer );
		E_RODISCLOSE_( time_t, ModificationTimeStamp );
	};

	E_AUTO( index )

	class index_spreaded_file_manager___
	{
	private:
		index_ *_Index;
		str::string _RootFileName;
		idxbtq::index_file_manager___ _FileManager;
		mdr::mode__ _Mode;
		bso::bool__ _ConnectToFiles()
		{
			if ( idxbtq::Connect( _Index->BaseIndex, _FileManager ) ) {
				_Index->SearchRoot();
				return true;
			} else {
				return false;
			}
		}
		void _ErasePhysically( void )
		{
			_FileManager.Drop();
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_FileManager.ReleaseFiles();

			_FileManager.reset( P );
			_Mode = mdr::m_Undefined;
			_RootFileName.reset( P );
			_Index = NULL;
		}
		index_spreaded_file_manager___( void )
		{
			reset( false );
		}
		~index_spreaded_file_manager___( void )
		{
			reset();
		}
		void Init(
			index_ &Index,
			const str::string_ &RootFileName,
			mdr::mode__ Mode,
			flm::id__ ID );
		void CloseFiles( void )
		{
			_FileManager.ReleaseFiles();
		}
	};


}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
