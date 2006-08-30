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

using namespace dbstbl;

class chrono__
{
private:
	time_t _Delay;
	time_t _Limit;
public:
	void Init( time_t Delay )
	{
		_Limit = 0;
		_Delay = Delay;
	}
	void Launch( void )
	{
		_Limit = tol::Clock() + _Delay;
	}
	bso::bool__ IsElapsed( void ) const
	{
		return tol::Clock() > _Limit;
	}
};


void dbstbl::table_::_InsertInIndexes( rrow__ Row )
{
	irow__ IRow = Indexes.First();

	while ( IRow != NONE ) {
		_I( IRow ).Index( Row );

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

void dbstbl::table_::_Reindex(
	irow__ IRow,
	observer_functions__ &Observer )
{
	index_ &Index = _I( IRow );
	const content_ &Content = _C();
	mdr::size__ RecordCount = 0;
	chrono__ Chrono;

	Index.Reset();

	Index.Allocate( Content.Amount() );

	rrow__ Row = Content.First();

	if ( &Observer != NULL ) {
		Observer.Notify( 0, Content.Amount() );
		Chrono.Init( Observer._Delay );
		Chrono.Launch();
	}

	while ( Row != NONE ) {
		Index.Index( Row );

		RecordCount++;

		if ( ( &Observer != NULL ) && Chrono.IsElapsed() ) {
			Observer.Notify( RecordCount, Content.Amount() );

			Chrono.Launch();
		}

		Row = Content.Next( Row );
	}

	if ( &Observer != NULL )
		Observer.Notify( RecordCount, Content.Amount() );
}

void dbstbl::table_::_ReindexAll( observer_functions__ &Observer )
{
	_Test();

	bso::ulong__ IndexCount = 0;
	irow__ Row = Indexes.First();

	if ( &Observer != NULL )
		Observer._IndexAmount = Indexes.Amount();

	while ( Row != NONE ) {
		IndexCount++;

		if ( &Observer )
			Observer._CurrentIndex = IndexCount;

		_Reindex( Row, Observer );

		Row = Indexes.Next( Row );
	}
}

void dbstbl::table_::Retrieve(
	rrows_ Rows,
	data_ &Data ) const
{
ERRProlog
	datum Datum;
	epeios::row__ Row = NONE;
ERRBegin
	_Test();

	const content_ &Content = _C();

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


#ifdef DBSTBL__THREAD_SAFE

#define RO	const table_ &T = _RO();
#define RW	table_ &T = _RW();

#define RRO	_RRO();
#define RRW	_RRW();

irow__ dbstbl::thread_safe_table_::AddIndex(
	index_ &Index,
	bso::bool__ Reindex,
	observer_functions__ &Observer )
{
	irow__ Row = NONE;
ERRProlog
ERRBegin
	RW

	Row = T.AddIndex( Index, Reindex, Observer );
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

	T.Insert( Datum );
ERRErr
ERREnd
	RRW
ERREpilog
	return Row;
}

void dbstbl::thread_safe_table_::Insert(
	const data_ &Data,
	rrows_ &RecordRows,
	time_t Delay )
{
ERRProlog
	ctn::E_CMITEM( datum_ ) Datum;
	epeios::row__ Row = NONE;
	chrono__ Chrono;
ERRBegin
	RW

	epeios::row__ Row = Data.First();

	Datum.Init( Data );

	Chrono.Init( Delay );
	Chrono.Launch();

	while ( Row != NONE ) {
		if ( Chrono.IsElapsed() ) {
			_RRW();

			tht::Defer();

			_RW();

			Chrono.Launch();
		}


		RecordRows.Append( T.Insert( Datum( Row ) ) );

		Row = Data.Next( Row );
	}
ERRErr
ERREnd
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
	const rrows_ &RecordRows,
	time_t Delay )
{
ERRProlog
	ctn::E_CMITEM( datum_ ) Datum;
	epeios::row__ Row = NONE;
	chrono__ Chrono;
ERRBegin
	RW

	if ( Data.Amount() != RecordRows.Amount() )
		ERRu();

	Row = Data.First();

	Datum.Init( Data );

	Chrono.Init( Delay );
	Chrono.Launch();

	while ( Row != NONE ) {
		if ( Chrono.IsElapsed() ) {
			_RRW();

			tht::Defer();

			_RW();

			Chrono.Launch();
		}

		T.Update( Datum( Row ), RecordRows( Row ) );

		Row = Data.Next( Row );
	}
ERRErr
ERREnd
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
	rrows_ Rows,
	data_ &Data,
	time_t Delay )
{
ERRProlog
	datum Datum;
	epeios::row__ Row = NONE;
	chrono__ Chrono;
ERRBegin
	RO

	Row = Rows.First();

	Chrono.Init( Delay );
	Chrono.Launch();

	while ( Row != NONE ) {
		if ( Chrono.IsElapsed() ) {
			_RRO();

			tht::Defer();

			_RO();

			Chrono.Launch();
		}

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

void dbstbl::thread_safe_table_::Delete(
	const rrows_ &RecordRows,
	time_t Delay )
{
ERRProlog
	epeios::row__ Row = NONE;
	chrono__ Chrono;
ERRBegin
	RW

	Row = RecordRows.First();

	Chrono.Init( Delay );
	Chrono.Launch();

	while ( Row != NONE ) {
		if ( Chrono.IsElapsed() ) {
			_RRW();

			tht::Defer();

			_RW();

			Chrono.Launch();
		}

		T.Delete( RecordRows( Row ) );

		Row = RecordRows.Next( Row );
	}
ERRErr
ERREnd
ERREpilog
}

rrow__ dbstbl::thread_safe_table_::Seek(
	const datum_ &Datum,
	irow__ IRow,
	bso::sign__ &Sign )
{
	rrow__ Row = NONE;
ERRProlog
ERRBegin
	RO

	Row = T.Seek( Datum, IRow, Sign );
ERRErr
ERREnd
	RRO
ERREpilog
	return Row;
}

rrow__ dbstbl::thread_safe_table_::Seek(
	const datum_ &Datum,
	irow__ IRow )
{
	rrow__ Row = NONE;
ERRProlog
ERRBegin
	RO

	Row = T.Seek( Datum, IRow );
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

mode__ dbstbl::thread_safe_table_::SwitchMode(
	mode__ Mode,
	observer_functions__ &Observer )
{
ERRProlog
ERRBegin
	RW

	Mode = T.SwitchMode( Mode, Observer );
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
	rows_ &Rows,
	time_t Delay )
{
ERRProlog
	epeios::row__ Row = NONE;
	chrono__ Chrono;
ERRBegin
	RO

	Row = RecordRows.First();

	Chrono.Init( Delay );
	Chrono.Launch();


	while ( Row != NONE ) {

		if ( Chrono.IsElapsed() ) {
			_RRO();

			tht::Defer();

			_RO();

			Chrono.Launch();
		}

		if ( !T.RecordExists( RecordRows( Row ) ) )
			Rows.Append( Row );

		Row = RecordRows.Next( Row );
	}

ERRErr
ERREnd
	RRO
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
