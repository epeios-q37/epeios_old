/*
	'ndbidx' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'ndbidx' header file ('ndbidx.h').
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

	Seeker.Init( _Index(), S_.Root );

	Row = Seeker.GetCurrent();

	while ( Row != NONE ) {
		DatumToCompare.Init();

		if ( !_Retrieve( Row, DatumToCompare, Cache ) )
			ERRc();

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

	rrow__ Buffer = _Index().GetTreeGreater( Row );
	rrow__ Candidate = NONE;

	while ( ( Buffer != NONE ) && ( Compare( Buffer, Row, SkipLevel ) == 0 ) )
		Buffer = _Index().GetTreeGreater( Buffer );

	if ( Buffer != NONE ) {
		Candidate = Buffer;

		Buffer = _Index().GetTreeLesser( Buffer );

		while ( ( Buffer != NONE ) && ( Compare( Buffer, Row, SkipLevel ) != 0 ) ) {
			Candidate = Buffer;

			Buffer = _Index().GetTreeLesser( Buffer );
		}

		if ( Buffer != NONE ) {
			Buffer = _SearchStrictGreater( Buffer, SkipLevel );

			if ( Buffer != NONE )
				Candidate = Buffer;
		}
	} else {
		Buffer = _Index().GetTreeParent( Row );

		if ( Buffer != NONE ) {

			if ( _Index().IsTreeGreater( Row ) ) {
				while ( ( Buffer != NONE ) && _Index().IsTreeGreater( Buffer ) )
					Buffer = _Index().GetTreeParent( Buffer );
			} else
				Buffer = Row;

			if ( Buffer != NONE ) {
				Buffer = _Index().GetTreeParent( Buffer );

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

rrow__ ndbidx::index_::_SearchStrictLesser(
	rrow__ Row,
	skip_level__ SkipLevel ) const
{
	_CompleteInitialization();	

	rrow__ Buffer = _Index().GetTreeLesser( Row );
	rrow__ Candidate = NONE;

	while ( ( Buffer != NONE ) && ( Compare( Buffer, Row, SkipLevel ) == 0 ) )
		Buffer = _Index().GetTreeLesser( Buffer );

	if ( Buffer != NONE ) {
		Candidate = Buffer;

		Buffer = _Index().GetTreeGreater( Buffer );

		while ( ( Buffer != NONE ) && ( Compare( Buffer, Row, SkipLevel ) != 0 ) ) {
			Candidate = Buffer;

			Buffer = _Index().GetTreeGreater( Buffer );
		}

		if ( Buffer != NONE ) {
			Buffer = _SearchStrictLesser( Buffer, SkipLevel );

			if ( Buffer != NONE )
				Candidate = Buffer;
		}
	} else {
		Buffer = _Index().GetTreeParent( Row );

		if ( Buffer != NONE ) {

			if ( _Index().IsTreeLesser( Row ) ) {
				while ( ( Buffer != NONE ) && _Index().IsTreeLesser( Buffer ) )
					Buffer = _Index().GetTreeParent( Buffer );
			} else
				Buffer = Row;

			if ( Buffer != NONE ) {
				Buffer = _Index().GetTreeParent( Buffer );

				if ( Buffer != NONE ) {
					if ( Compare( Row, Buffer, SkipLevel ) != 0 )
						Candidate = Buffer;
					else
						Candidate = _SearchStrictLesser( Buffer, SkipLevel );
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
	if ( _Bufferized )
		ERRu();

	_CompleteInitialization();

	if ( _Content().Amount() > DIndex.Amount() )
		DIndex.Allocate( _Content().Amount(), aem::m_Default );

	if ( S_.Root == NONE ) {
		S_.Root = Row;

		DIndex.BecomeRoot( Row );

		ERRReturn;
	}

	Datum.Init();

	if ( !_Retrieve( Row, Datum, *(ndbctt::cache_ *)NULL ) )
		ERRReturn;	// L'enregistrement n'existe pas, on ne va donc pas l'inclure dans l'index.
					// Ce cas ne devrait pas arrivé, sauf lorsqu'il y a eu des problèmes de corruption corrigés à la main.

	if ( Extremities != NULL ) {
		if ( Extremities->Smallest == NONE )
			Extremities->Smallest = DIndex.First( S_.Root );

		TargetRow = Extremities->Smallest;

		DatumToCompare.Init();

		if ( !_Retrieve( TargetRow, DatumToCompare, Cache ) )
			ERRc();

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
			Extremities->Greatest = DIndex.Last( S_.Root );

		TargetRow = Extremities->Greatest;

		DatumToCompare.Init();

		if ( !_Retrieve( TargetRow, DatumToCompare, Cache ) )
			ERRc();

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
		S_.Root = DIndex.BecomeLesser( Row, TargetRow, S_.Root );
		break;
	case 0:	// Pas de problème avec la gestion des 'extremities', 
		if ( !DIndex.TreeHasLesser( TargetRow ) )
			S_.Root = DIndex.BecomeLesser( Row, TargetRow, S_.Root );
		else if ( !DIndex.TreeHasGreater( TargetRow ) )
			S_.Root = DIndex.BecomeGreater( Row, TargetRow, S_.Root );
		else
			ERRc();
		break;
	case 1:
		if ( ( Extremities != NULL ) && ( Extremities->Greatest == TargetRow ) )
			Extremities->Greatest = Row;
		S_.Root = DIndex.BecomeGreater( Row, TargetRow, S_.Root );
		break;
	default:
		ERRc();
		break;
	}

//	Display( BaseIndex, S_.Root, cout );

#ifdef NDBIDX_DBG
	if ( ( Round > 32 ) || ( ( 2UL << ( Round >> 1 ) ) > _Content().Amount() ) )
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

	_Content().Retrieve( Row, Datum );

	Row = Next( Row );

	while ( Row != NONE ) {
		if ( Compare( Row, Datum, NDBIDX_NO_SKIP ) == 1 )
			ERRReturn;

		Datum.Init();

		_Content().Retrieve( Row, Datum );

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

	if ( !_Retrieve( RecordRow, Datum, *(ndbctt::cache_ *)NULL ) )
		ERRc();

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

	if ( !_Retrieve( RecordRow2, Pattern, *(ndbctt::cache_ *)NULL ) )
		ERRc();

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
	bso::size__ &HandledRecordAmount,
	bso::ulong__ &BalancingCount,
	tol::E_DPOINTER___( extremities__ ) &Extremities,
	bso::bool__ Randomly )
{
	mdr::row__ Row = NONE;
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
			Observer.Notify( HandledRecordAmount, Index.Content().Amount(), BalancingCount );

			Chrono.Launch();
		}
	}
}

void ndbidx::index_::Reindex( observer_functions__ &Observer )
{
ERRProlog
	const ndbctt::content__ &Content = _Content();
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
		IndexInMemory.Init( *_ContentPointer, SortFunction() );

		IndexInMemory.Allocate( Content.Extent(), aem::m_Default );

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
	const str::string_ &BaseFileName,
	bso::bool__ Erase,
	fil::mode__ Mode,
	flm::id__ ID )
{
ERRProlog
	str::string TreeFileName;
	STR_BUFFER___ TreeFileNameBuffer;
	str::string QueueFileName;
	STR_BUFFER___ QueueFileNameBuffer;
ERRBegin
	reset();

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
