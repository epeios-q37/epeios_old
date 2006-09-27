/*
	'dbstbl' library by Claude SIMON (csimon@epeios.org)
	Requires the 'dbstbl' header file ('dbstbl.h').
	Copyright (C) $COPYRIGHT_DATES$Claude SIMON (csimon@epeios.org).
$_RAW_$
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

#define DBSTBL__COMPILATION

#include "dbstbl.h"

class dbstbltutor
: public ttr_tutor
{
public:
	dbstbltutor( void )
	: ttr_tutor( DBSTBL_NAME )
	{
#ifdef DBSTBL_DBG
		Version = DBSTBL_VERSION "\b\bD $";
#else
		Version = DBSTBL_VERSION;
#endif
		Owner = DBSTBL_OWNER;
		Date = "$Date$";
	}
	virtual ~dbstbltutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#define MEMORY_REINDEXATION_LIMIT	10000000
/* Limite du nombre d'neregistrement au-delà de laquelle on utilise 
directement l'index sur le disque et non pas une copie temporaire en mémoire
pour éviter la mise en oeuvre de la mémoire virtuelle. */

#define RECORD_PANEL_SIZE		50000	// Nombre d'enregistrements par tranche.

#define RECORD_TEST_PANEL_SIZE		1000	// Nombre d'enregistrements pour la tranche de test.


using namespace dbstbl;

void dbstbl::table_::_InsertInIndexes( rrow__ Row )
{
	irow__ IRow = Indexes.First();

	while ( IRow != NONE ) {
		_I( IRow ).Index( Row, false );

		IRow = Indexes.Next( IRow );
	}
}

void dbstbl::table_::_DeleteFromIndexes( rrow__ Row )
{
	irow__ IRow = Indexes.First();

	while ( IRow != NONE ) {
		_I( IRow ).Delete( Row );

		IRow = Indexes.Next( IRow );
	}
}

static inline void Reindex_(
	rrows_ &Rows,
	index_ &Index,
	observer_functions__ &Observer,
	dbsctt::_cache_  &Cache,
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
			Observer.Notify( HandledRecordAmount, Index.Content().Amount(), BalancingCount );

			Chrono.Launch();
		}
	}

}

void dbstbl::table_::_Reindex(
	irow__ IRow,
	observer_functions__ &Observer )
{
ERRProlog
	index_ &Index = _I( IRow );
	const content_ &Content = C_();
	mdr::size__ HandledRecordAmount = 0;
	tol::chrono__ Chrono;
	dbsidx::index IndexInMemory;
	dbsidx::index_ *UsedIndex = NULL;
	dbsctt::_cache  Cache;
	tol::E_DPOINTER___( extremities__ ) Extremities;
	bso::ulong__ BalancingCount = 0;
	bch::E_BUNCH( rrow__ ) Rows;
	rrow__ Row = NONE;
	bso::ulong__ PanelRecordCounter;
	bso::ulong__ PanelRecordSize;
	bso::bool__ Randomly = false;
ERRBegin
	Index.Reset();

	if ( Content.Extent() < MEMORY_REINDEXATION_LIMIT ) {
		IndexInMemory.Init( Index.Content(), Index.SortFunction() );

		IndexInMemory.Allocate( Content.Extent() );

		UsedIndex = &IndexInMemory;
	} else
		UsedIndex = &Index;

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

	if ( UsedIndex != &Index )
		Index = *UsedIndex;
ERRErr
ERREnd
ERREpilog
}

void dbstbl::table_::_ReindexAll( observer_functions__ &Observer )
{
	_Test();

	irow__ Row = Indexes.First();

	if ( &Observer != NULL ) {
		Observer._TotalIndexAmount = Indexes.Amount();
		Observer._HandledIndexAmount = 0;
	}

	while ( Row != NONE ) {
		_Reindex( Row, Observer );

		if ( &Observer )
			Observer._HandledIndexAmount++;

		Row = Indexes.Next( Row );
	}
}

void dbstbl::table_::Retrieve(
	const rrows_ &Rows,
	data_ &Data ) const
{
ERRProlog
	datum Datum;
	epeios::row__ Row = NONE;
ERRBegin
	_Test();

	const content_ &Content = C_();

	Row = Rows.First();

	while ( Row != NONE ) {
		Datum.Init();

		Content.Retrieve( Rows( Row ), Datum );

		Data.Append( Datum );

		Row = Rows.Next( Row );
	}
ERRErr
ERREnd
ERREpilog
}

void dbstbl::table_::Insert(
	const data_ &Data,
	rrows_ &RecordRows )
{
	_Test();

	ctn::E_CMITEM( datum_ ) Datum;
	epeios::row__ Row = Data.First();

	Datum.Init( Data );

	while ( Row != NONE ) {
		RecordRows.Append( Insert( Datum( Row ) ) );

		Row = Data.Next( Row );
	}
}

void dbstbl::table_::Update(
	const data_ &Data,
	const rrows_ &RecordRows )
{
	if ( Data.Amount() != RecordRows.Amount() )
		ERRu();

	_Test();

	ctn::E_CMITEM( datum_ ) Datum;
	epeios::row__ Row = Data.First();

	Datum.Init( Data );

	while ( Row != NONE ) {
		Update( Datum( Row ), RecordRows( Row ) );

		Row = Data.Next( Row );
	}
}

void dbstbl::table_::Delete( const rrows_ &RecordRows )
{
	_Test();

	epeios::row__ Row = RecordRows.First();

	while ( Row != NONE ) {
		Delete( RecordRows( Row ) );

		Row = RecordRows.Next( Row );
	}
}


void dbstbl::table_::_ResetAllIndexes( void )
{
	irow__ Row = Indexes.First();

	while ( Row != NONE ) {
		_I( Row ).Reset();

		Row = Indexes.Next( Row );
	}
}

void dbstbl::table_::TestRecordsExistence(
	const rrows_ &RecordRows,
	rows_ &Rows ) const
{
	epeios::row__ Row = RecordRows.First();

	while ( Row != NONE ) {
		if ( !RecordExists( RecordRows( Row ) ) )
			Rows.Append( Row );

		Row = RecordRows.Next( Row );
	}

}


bso::bool__ dbstbl::table_::AreAllIndexesSynchronized( void ) const
{
	irow__ Row = Indexes.First();

	while ( Row != NONE ) {
		if ( !IsIndexSynchronized( Row ) )
			return false;

		Row = Indexes.Next( Row );
	}

	return true;
}


#ifdef DBSTBL__THREAD_SAFE

#define RO	const table_ &T = _RO();
#define RW	table_ &T = _RW();

#define RRO	_RRO();
#define RRW	_RRW();

irow__ dbstbl::thread_safe_table_::AddIndex( index_ &Index )
{
	irow__ Row = NONE;
ERRProlog
ERRBegin
	RW

	Row = T.AddIndex( Index );
ERRErr
ERREnd
	RRW
ERREpilog
	return Row;
}

rrow__ dbstbl::thread_safe_table_::Insert( const datum_ &Datum )
{
	rrow__ Row = NONE;
ERRProlog
ERRBegin
	RW

	Row = T.Insert( Datum );
ERRErr
ERREnd
	RRW
ERREpilog
	return Row;
}

void dbstbl::thread_safe_table_::Insert(
	const data_ &Data,
	rrows_ &RecordRows )
{
ERRProlog
	ctn::E_CMITEM( datum_ ) Datum;
	epeios::row__ Row = NONE;
ERRBegin
	RW

	epeios::row__ Row = Data.First();

	Datum.Init( Data );

	while ( Row != NONE ) {
		RecordRows.Append( T.Insert( Datum( Row ) ) );

		Row = Data.Next( Row );
	}
ERRErr
ERREnd
	RRW
ERREpilog
}

void dbstbl::thread_safe_table_::Update(
	const datum_ &Datum,
	rrow__ Row )
{
ERRProlog
ERRBegin
	RW

	T.Update( Datum, Row );
ERRErr
ERREnd
	RRW
ERREpilog
}

void dbstbl::thread_safe_table_::Update(
	const data_ &Data,
	const rrows_ &RecordRows )
{
ERRProlog
	ctn::E_CMITEM( datum_ ) Datum;
	epeios::row__ Row = NONE;
ERRBegin
	RW

	if ( Data.Amount() != RecordRows.Amount() )
		ERRu();

	Row = Data.First();

	Datum.Init( Data );

	while ( Row != NONE ) {
		T.Update( Datum( Row ), RecordRows( Row ) );

		Row = Data.Next( Row );
	}
ERRErr
ERREnd
	RRW
ERREpilog
}

void dbstbl::thread_safe_table_::Retrieve(
	rrow__ Row,
	datum_ &Datum )
{
ERRProlog
ERRBegin
	RO

	T.Retrieve( Row, Datum );
ERRErr
ERREnd
	RRO
ERREpilog
}

void dbstbl::thread_safe_table_::Retrieve(
	const rrows_ &Rows,
	data_ &Data )
{
ERRProlog
	datum Datum;
	epeios::row__ Row = NONE;
ERRBegin
	RO

	Row = Rows.First();

	while ( Row != NONE ) {
		Datum.Init();

		T.Retrieve( Rows( Row ), Datum );

		Data.Append( Datum );

		Row = Rows.Next( Row );
	}
ERRErr
ERREnd
	RRO
ERREpilog
}

void dbstbl::thread_safe_table_::Delete( rrow__ Row )
{
ERRProlog
ERRBegin
	RW

	T.Delete( Row );
ERRErr
ERREnd
	RRW
ERREpilog
}

void dbstbl::thread_safe_table_::Delete( const rrows_ &RecordRows )
{
ERRProlog
	epeios::row__ Row = NONE;
ERRBegin
	RW

	Row = RecordRows.First();

	while ( Row != NONE ) {
		T.Delete( RecordRows( Row ) );

		Row = RecordRows.Next( Row );
	}
ERRErr
ERREnd
	RRW
ERREpilog
}

rrow__ dbstbl::thread_safe_table_::Seek(
	const datum_ &Datum,
	irow__ IRow,
	behavior__ EqualBehavior,
	bso::sign__ &Sign )
{
	rrow__ Row = NONE;
ERRProlog
ERRBegin
	RO

	Row = T.Seek( Datum, IRow, EqualBehavior, Sign );
ERRErr
ERREnd
	RRO
ERREpilog
	return Row;
}

rrow__ dbstbl::thread_safe_table_::Seek(
	const datum_ &Datum,
	irow__ IRow,
	behavior__ EqualBehavior )
{
	rrow__ Row = NONE;
ERRProlog
ERRBegin
	RO

	Row = T.Seek( Datum, IRow, EqualBehavior );
ERRErr
ERREnd
	RRO
ERREpilog
	return Row;
}

bso::bool__ dbstbl::thread_safe_table_::Begins(
	rrow__ RecordRow,
	const datum_ &Pattern,
	irow__ IRow )
{
	bso::bool__ Result = false;
ERRProlog
ERRBegin
	RO

	Result = T.Begins( RecordRow, Pattern, IRow );
ERRErr
ERREnd
	RRO
ERREpilog
	return Result;
}

rrow__ dbstbl::thread_safe_table_::First( irow__ IRow )
{
	rrow__ Row = NONE;
ERRProlog
ERRBegin
	RO

	Row = T.First( IRow );
ERRErr
ERREnd
	RRO
ERREpilog
	return Row;
}

rrow__ dbstbl::thread_safe_table_::Last( irow__ IRow )
{
	rrow__ Row = NONE;
ERRProlog
ERRBegin
	RO

	Row = T.Last( IRow );
ERRErr
ERREnd
	RRO
ERREpilog
	return Row;
}

rrow__ dbstbl::thread_safe_table_::Next( 
	irow__ IRow,
	rrow__ Row )
{
ERRProlog
ERRBegin
	RO

	Row = T.Next( IRow, Row );
ERRErr
ERREnd
	RRO
ERREpilog
	return Row;
}

rrow__ dbstbl::thread_safe_table_::Previous( 
	irow__ IRow,
	rrow__ Row )
{
ERRProlog
ERRBegin
	RO

	Row = T.Previous( IRow, Row );
ERRErr
ERREnd
	RRO
ERREpilog
	return Row;
}

mdr::size__ dbstbl::thread_safe_table_::Amount( void )
{
	mdr::size__ Amount;
ERRProlog
ERRBegin
	RO

	Amount = T.Amount();
ERRErr
ERREnd
	RRO
ERREpilog
	return Amount;
}

mdr::size__ dbstbl::thread_safe_table_::Extent( void )
{
	mdr::size__ Extent;
ERRProlog
ERRBegin
	RO

	Extent = T.Extent();
ERRErr
ERREnd
	RRO
ERREpilog
	return Extent;
}

mode__ dbstbl::thread_safe_table_::SwitchMode( mode__ Mode )
{
ERRProlog
ERRBegin
	RW

	Mode = T.SwitchMode( Mode );
ERRErr
ERREnd
	RRW
ERREpilog
	return Mode;
}

mode__ dbstbl::thread_safe_table_::Mode( void )
{
	mode__ Mode = m_Undefined;
ERRProlog
ERRBegin
	RO

	Mode = T.Mode();
ERRErr
ERREnd
	RRO
ERREpilog
	return Mode;
}

bso::bool__ dbstbl::thread_safe_table_::RecordExists( rrow__ RecordRow )
{
	bso::bool__ Exists = false;
ERRProlog
ERRBegin
	RO

	Exists = T.RecordExists( RecordRow );
ERRErr
ERREnd
	RRO
ERREpilog
	return Exists;
}

void dbstbl::thread_safe_table_::TestRecordsExistence(
	const rrows_ &RecordRows,
	rows_ &Rows )
{
ERRProlog
	epeios::row__ Row = NONE;
ERRBegin
	RO

	Row = RecordRows.First();

	while ( Row != NONE ) {
		if ( !T.RecordExists( RecordRows( Row ) ) )
			Rows.Append( Row );

		Row = RecordRows.Next( Row );
	}

ERRErr
ERREnd
	RRO
ERREpilog
}

bso::sign__ dbstbl::thread_safe_table_::Compare(
	rrow__ RecordRow,
	const datum_&Pattern,
	irow__ IndexRow )
{
	bso::sign__ Result = 0;
ERRProlog
ERRBegin
	RO

	Result = T.Compare( RecordRow, Pattern, IndexRow );
ERRErr
ERREnd
	RRO
ERREpilog
	return Result;
}

bso::bool__ dbstbl::thread_safe_table_::IsIndexSynchronized( irow__ IndexRow )
{
	bso::bool__ Result = false;
ERRProlog
ERRBegin
	RO

	Result = T.IsIndexSynchronized( IndexRow );
ERRErr
ERREnd
	RRO
ERREpilog
	return Result;
}

bso::bool__ dbstbl::thread_safe_table_::AreAllIndexesSynchronized( void )
{
	bso::bool__ Result = false;
ERRProlog
ERRBegin
	RO

	Result = T.AreAllIndexesSynchronized();
ERRErr
ERREnd
	RRO
ERREpilog
	return Result;
}

void dbstbl::thread_safe_table_::Reindex(
	irow__ IndexRow,
	observer_functions__ &Observer )
{
ERRProlog
ERRBegin
	RW

	T.Reindex( IndexRow, Observer );
ERRErr
ERREnd
	RRW
ERREpilog
}

void dbstbl::thread_safe_table_::ReindexAll( observer_functions__ &Observer )
{
ERRProlog
ERRBegin
	RW

	T.ReindexAll( Observer );
ERRErr
ERREnd
	RRW
ERREpilog
}

#endif

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class dbstblpersonnalization
: public dbstbltutor
{
public:
	dbstblpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~dbstblpersonnalization( void )
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

static dbstblpersonnalization Tutor;

ttr_tutor &DBSTBLTutor = Tutor;
