/*
	'ndbidx' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'ndbidx' header file ('ndbidx.h').
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

#define NDBIDX__COMPILATION

#include "ndbidx.h"

class ndbidxtutor
: public ttr_tutor
{
public:
	ndbidxtutor( void )
	: ttr_tutor( NDBIDX_NAME )
	{
#ifdef NDBIDX_DBG
		Version = NDBIDX_VERSION "\b\bD $";
#else
		Version = NDBIDX_VERSION;
#endif
		Owner = NDBIDX_OWNER;
		Date = "$Date$";
	}
	virtual ~ndbidxtutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace ndbidx;

#define TREE_FILE_NAME_EXTENSION	".edt"
#define QUEUE_FILE_NAME_EXTENSION	".edq"

#define MEMORY_REINDEXATION_LIMIT	10000000
/* Limite du nombre d'neregistrement au-delà de laquelle on utilise 
directement l'index sur le disque et non pas une copie temporaire en mémoire
pour éviter la mise en oeuvre de la mémoire virtuelle. */

#define RECORD_PANEL_SIZE		50000	// Nombre d'enregistrements par tranche.

#define RECORD_TEST_PANEL_SIZE		1000	// Nombre d'enregistrements pour la tranche de test.


bso::sign__ ndbidx::index_::_Seek(
	const datum_ &Datum,
	skip_level__ SkipLevel,
	behavior__ EqualBehavior,
	rrow__ &Row,
	bso::ubyte__ &Round,
	ndbctt::cache_ &Cache ) const
{
	bso::sign__ Result = 0;
ERRProlog
	datum DatumToCompare;
	idxbtq::E_ISEEKERt__( rrow__ ) Seeker;
ERRBegin
	_CompleteInitialization();

	Round = 0;

	if ( S_.Root == NONE ) {
		Row = NONE;

		ERRReturn;
	}

	Seeker.Init( BaseIndex, S_.Root );

	Row = Seeker.GetCurrent();

	while ( Row != NONE ) {
		DatumToCompare.Init();

		_Retrieve( Row, DatumToCompare, Cache );

		switch ( Result = _SortPointer->Compare( Datum, DatumToCompare, SkipLevel ) ) {
		case 0:
			switch ( EqualBehavior ) {
			case bStop:
				Row = NONE;	// Pour sortir de la boucle.
				break;
			case bGreater:
				Row = Seeker.SearchGreater();
				break;
			case bLesser:
				Row = Seeker.SearchLesser();
				break;
			case bStopIfOneChildMissing:
				if ( !Seeker.HasLesser() || !Seeker.HasGreater() )
					Row = NONE;	// Popur sortir de la boucle.
				else if ( *Row & 1 )	// Petit générateur aléatoire (sans doute inutile ?).
					Row = Seeker.SearchLesser();
				else
					Row = Seeker.SearchGreater();
				break;
			default:
				ERRu();
			}
			break;
		case 1:
			Row = Seeker.SearchGreater();
			break;
		case -1:
			Row = Seeker.SearchLesser();
			break;
		default:
			ERRc();
			break;
		}

		Round++;
	}

	Row = Seeker.GetCurrent();
ERRErr
ERREnd
ERREpilog
	return Result;
}

rrow__ ndbidx::index_::_SearchStrictGreater(
	rrow__ Row,
	skip_level__ SkipLevel ) const
{
	_CompleteInitialization();	

	rrow__ Buffer = BaseIndex.GetTreeGreater( Row );
	rrow__ Candidate = NONE;

	while ( ( Buffer != NONE ) && ( Compare( Buffer, Row, SkipLevel ) == 0 ) )
		Buffer = BaseIndex.GetTreeGreater( Buffer );

	if ( Buffer != NONE ) {
		Candidate = Buffer;

		Buffer = BaseIndex.GetTreeLesser( Buffer );

		while ( ( Buffer != NONE ) && ( Compare( Buffer, Row, SkipLevel ) != 0 ) ) {
			Candidate = Buffer;

			Buffer = BaseIndex.GetTreeLesser( Buffer );
		}

		if ( Buffer != NONE ) {
			Buffer = _SearchStrictGreater( Buffer, SkipLevel );

			if ( Buffer != NONE )
				Candidate = Buffer;
		}
	} else {
		Buffer = BaseIndex.GetTreeParent( Row );

		if ( Buffer != NONE ) {

			if ( BaseIndex.IsTreeGreater( Row ) ) {
				while ( ( Buffer != NONE ) && BaseIndex.IsTreeGreater( Buffer ) )
					Buffer = BaseIndex.GetTreeParent( Buffer );
			} else
				Buffer = Row;

			if ( Buffer != NONE ) {
				Buffer = BaseIndex.GetTreeParent( Buffer );

				if ( Buffer != NONE ) {
					if ( Compare( Row, Buffer, SkipLevel ) != 0 )
						Candidate = Buffer;
					else
						Candidate = _SearchStrictGreater( Buffer, SkipLevel );
				}
			}
		}
	}

	return Candidate;
}


bso::ubyte__ ndbidx::index_::Index(
	rrow__ Row,
	extremities__ *Extremities,
	ndbctt::cache_ &Cache )
{
	bso::ubyte__ Round = 0;
ERRProlog
	datum DatumToCompare;
	datum Datum;
	rrow__ TargetRow = NONE;
	bso::sign__ Result;
//	tol::buffer__ Buffer;
//	cio::aware_cout___ cout;
ERRBegin
	_CompleteInitialization();

	if ( _Content( true ).Amount() > BaseIndex.Amount() )
		BaseIndex.Allocate( _Content( true ).Amount(), aem::mDefault );

	if ( S_.Root == NONE ) {
		S_.Root = Row;

		BaseIndex.BecomeRoot( Row );

		ERRReturn;
	}

	Datum.Init();

	_Retrieve( Row, Datum, *(ndbctt::cache_ *)NULL );

	if ( Extremities != NULL ) {
		if ( Extremities->Smallest == NONE )
			Extremities->Smallest = BaseIndex.First( S_.Root );

		TargetRow = Extremities->Smallest;

		DatumToCompare.Init();

		_Retrieve( TargetRow, DatumToCompare, Cache );

		switch ( Result = _SortPointer->Compare( Datum, DatumToCompare, NDBIDX_NO_SKIP ) ) {
		case 0:
			Result = -1;	// Pour forcer son positionnement en tant que premier.
		case -1:
			break;
		case 1:
			TargetRow = NONE;
			break;
		default:
			ERRc();
			break;
		}
	}

	if ( ( TargetRow == NONE ) && ( Extremities != NULL ) ) {
		if ( Extremities->Greatest == NONE )
			Extremities->Greatest = BaseIndex.Last( S_.Root );

		TargetRow = Extremities->Greatest;

		DatumToCompare.Init();

		_Retrieve( TargetRow, DatumToCompare, Cache );

		switch ( Result = _SortPointer->Compare( Datum, DatumToCompare, NDBIDX_NO_SKIP ) ) {
		case 0:
			Result = 1;	// Pour forcer son positionnement en tant que dernier.
		case 1:
			break;
		case -1:
			TargetRow = NONE;
			break;
		default:
			ERRc();
			break;
		}
	} 

	if ( TargetRow == NONE )
		Result = _Seek( Datum, NDBIDX_NO_SKIP, bStopIfOneChildMissing, TargetRow, Round, Cache );
	else
		Extremities->Used++;

//	cout << tol::DateAndTime( Buffer ) << txf::tab << &BaseIndex << txf::nl <<  txf::sync;
//	Display( BaseIndex, S_.Root, cout );

	switch ( Result ) {
	case -1:
		if ( ( Extremities != NULL ) && ( Extremities->Smallest == TargetRow ) )
			Extremities->Smallest = Row;
		S_.Root = BaseIndex.BecomeLesser( Row, TargetRow, S_.Root );
		break;
	case 0:	// Pas de problème avec la gestion des 'extremities', 
		if ( !BaseIndex.TreeHasLesser( TargetRow ) )
			S_.Root = BaseIndex.BecomeLesser( Row, TargetRow, S_.Root );
		else if ( !BaseIndex.TreeHasGreater( TargetRow ) )
			S_.Root = BaseIndex.BecomeGreater( Row, TargetRow, S_.Root );
		else
			ERRc();
		break;
	case 1:
		if ( ( Extremities != NULL ) && ( Extremities->Greatest == TargetRow ) )
			Extremities->Greatest = Row;
		S_.Root = BaseIndex.BecomeGreater( Row, TargetRow, S_.Root );
		break;
	default:
		ERRc();
		break;
	}

//	Display( BaseIndex, S_.Root, cout );

#ifdef NDBIDX_DBG
	if ( ( Round > 32 ) || ( ( 2UL << ( Round >> 1 ) ) > _Content( true ).Amount() ) )
		S_.Root = S_.Root;	// Juste pour pouvoir placer un point d'arrêt.
#endif

#ifdef NDBIDX_DBG
	if ( TargetRow == NONE )
		ERRc();
#endif

ERRErr
ERREnd
	_Touch( false );
ERREpilog
	return Round;
}

rrow__ ndbidx::index_::LooseSeek( 
	const datum_ &Datum,
	behavior__ EqualBehavior,
	skip_level__ SkipLevel,
	bso::sign__ &Sign ) const
{
	rrow__ Row = NONE;
	bso::ubyte__ Round;

	_CompleteInitialization();

	if ( S_.Root == NONE )
		return NONE;

	Sign = _Seek( Datum, SkipLevel, EqualBehavior, Row, Round, *(ndbctt::cache_ *)NULL );

#ifdef NDBIDX_DBG
	if ( Row == NONE )
		ERRc();
#endif

	return Row;
}

rrow__ ndbidx::index_::Test( void ) const
{
	rrow__ Row = NONE;
ERRProlog
	datum Datum;
ERRBegin
	_CompleteInitialization();

	if ( S_.Root == NONE )
		ERRReturn;

	Row = First();

	Datum.Init();

	_Content( true ).Retrieve( Row, Datum );

	Row = Next( Row );

	while ( Row != NONE ) {
		if ( Compare( Row, Datum, NDBIDX_NO_SKIP ) == 1 )
			ERRReturn;

		Datum.Init();

		_Content( true ).Retrieve( Row, Datum );

		Row = Next( Row );
	}
ERRErr
ERREnd
ERREpilog
	return Row;
}


bso::sign__ ndbidx::index_::Compare(
	rrow__ RecordRow,
	const datum_ &Pattern,
	skip_level__ SkipLevel ) const
{
	bso::sign__ Result = 0;
ERRProlog
	datum Datum;
ERRBegin
	Datum.Init();

	_CompleteInitialization();

	_Retrieve( RecordRow, Datum, *(ndbctt::cache_ *)NULL );

	Result = _SortPointer->Compare( Datum, Pattern, SkipLevel  );
ERRErr
ERREnd
ERREpilog
	return Result;
}

bso::sign__ ndbidx::index_::Compare(
	rrow__ RecordRow1,
	rrow__ RecordRow2,
	skip_level__ SkipLevel ) const
{
	bso::sign__ Result = 0;
ERRProlog
	datum Pattern;
ERRBegin
	Pattern.Init();

	_CompleteInitialization();

	_Retrieve( RecordRow2, Pattern, *(ndbctt::cache_ *)NULL );

	Result = Compare( RecordRow1, Pattern, SkipLevel );
ERRErr
ERREnd
ERREpilog
	return Result;
}

static inline void Reindex_(
	rrows_ &Rows,
	index_ &Index,
	observer_functions__ &Observer,
	ndbctt::cache_  &Cache,
	tol::chrono__ &Chrono,
	bso::ulong__ &HandledRecordAmount,
	bso::ulong__ &BalancingCount,
	tol::E_DPOINTER___( extremities__ ) &Extremities,
	bso::bool__ Randomly )
{
	epeios::row__ Row = NONE;
	bso::ubyte__ Round = 0;

	while ( Rows.Amount() ) {
		if ( Randomly )
			Row = Rows.Amount() - ( rand() % Rows.Amount() ) - 1;
		else
			Row = Rows.First();

		Round = Index.Index( Rows( Row ), Extremities, Cache );

		Rows.Remove( Row );

		if ( ( 1UL << ( Round >> 3 ) ) > HandledRecordAmount ) {
			Index.Balance();
			BalancingCount++;
			if ( ( Extremities == NULL ) && ( BalancingCount > 1 ) )
				Extremities = new extremities__;
		}

		HandledRecordAmount++;

		if ( ( &Observer != NULL ) && Chrono.IsElapsed() ) {
			Observer.Notify( HandledRecordAmount, Index.Content( true ).Amount(), BalancingCount );

			Chrono.Launch();
		}
	}
}

void ndbidx::index_::Reindex( observer_functions__ &Observer )
{
ERRProlog
	const ndbctt::content__ &Content = _Content( false );
	mdr::size__ HandledRecordAmount = 0;
	tol::chrono__ Chrono;
	ndbidx::index IndexInMemory;
	ndbidx::index_ *UsedIndex = NULL;
	ndbbsc::cache  Cache;
	tol::E_DPOINTER___( extremities__ ) Extremities;
	bso::ulong__ BalancingCount = 0;
	bch::E_BUNCH( rrow__ ) Rows;
	rrow__ Row = NONE;
	bso::ulong__ PanelRecordCounter;
	bso::ulong__ PanelRecordSize;
	bso::bool__ Randomly = false;
ERRBegin
	_CompleteInitialization();

	Reset();

	if ( Content.Amount() == 0 )
		ERRReturn;

	if ( Content.Extent() < MEMORY_REINDEXATION_LIMIT ) {
		IndexInMemory.Init( Content, SortFunction() );

		IndexInMemory.Allocate( Content.Extent(), aem::mDefault );

		UsedIndex = &IndexInMemory;
	} else
		UsedIndex = this;

	Cache.Init( Content.Extent() );

	Rows.Init();

	Row = Content.First();

	PanelRecordSize = RECORD_TEST_PANEL_SIZE;

	PanelRecordCounter = PanelRecordSize;

	if ( ( &Observer != NULL ) && ( Content.Amount() != 0 ) ) {
		Observer.Notify( 0, Content.Amount(), BalancingCount );
		Chrono.Init( Observer._Delay );
		Chrono.Launch();
	}

	while ( Row != NONE ) {
		Rows.Append( Row );

		if ( PanelRecordCounter-- == 0 ) {
			Reindex_( Rows, *UsedIndex, Observer, Cache, Chrono, HandledRecordAmount, BalancingCount, Extremities, Randomly );

			if ( Randomly == false )
				if ( ( Extremities == NULL ) || ( Extremities->Used < ( ( 2 * PanelRecordSize ) / 3 ) ) )
					Randomly = true;
				else
					Extremities->Used = 0;


			PanelRecordSize = RECORD_PANEL_SIZE;

			PanelRecordCounter = PanelRecordSize;
		}

		Row = Content.Next( Row );
	}

	Reindex_( Rows, *UsedIndex, Observer, Cache, Chrono, HandledRecordAmount, BalancingCount, Extremities, Randomly );

	if ( ( &Observer != NULL ) && ( Content.Amount() != 0 ) )
		Observer.Notify( HandledRecordAmount, Content.Amount(), BalancingCount );

	UsedIndex->Balance();

	if ( UsedIndex != this )
		this->operator =( *UsedIndex );
ERRErr
ERREnd
ERREpilog
}


void ndbidx::index_atomized_file_manager___::Init(
	index_ &Index,
	const str::string_ &BaseFileName,
	bso::bool__ Erase,
	mdr::mode__ Mode,
	flm::id__ ID )
{
ERRProlog
	str::string TreeFileName;
	STR_BUFFER___ TreeFileNameBuffer;
	str::string QueueFileName;
	STR_BUFFER___ QueueFileNameBuffer;
ERRBegin
	reset();

	_Index = &Index;

	TreeFileName.Init( BaseFileName );
	TreeFileName.Append( TREE_FILE_NAME_EXTENSION );

	QueueFileName.Init( BaseFileName );
	QueueFileName.Append( QUEUE_FILE_NAME_EXTENSION );

	_FileManager.Init( TreeFileName.Convert( TreeFileNameBuffer ), QueueFileName.Convert( QueueFileNameBuffer ), Mode, true, ID );

	_BaseFileName.Init( BaseFileName );

	_Mode = Mode;

	if ( Erase )
		_ErasePhysically();
ERRErr
ERREnd
ERREpilog
}

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class ndbidxpersonnalization
: public ndbidxtutor
{
public:
	ndbidxpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~ndbidxpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the ending of the application  */
	}
};


/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

// 'static' by GNU C++.

static ndbidxpersonnalization Tutor;

ttr_tutor &NDBIDXTutor = Tutor;
