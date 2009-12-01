/*
	'ndbtbl' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'ndbtbl' header file ('ndbtbl.h').
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

#define NDBTBL__COMPILATION

#include "ndbtbl.h"

class ndbtbltutor
: public ttr_tutor
{
public:
	ndbtbltutor( void )
	: ttr_tutor( NDBTBL_NAME )
	{
#ifdef NDBTBL_DBG
		Version = NDBTBL_VERSION "\b\bD $";
#else
		Version = NDBTBL_VERSION;
#endif
		Owner = NDBTBL_OWNER;
		Date = "$Date$";
	}
	virtual ~ndbtbltutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#include "fnm.h"

using namespace ndbtbl;

void ndbtbl::table_::_InsertInIndexes( rrow__ Row )
{
	epeios::row__ IRow = Indexes.First();

	while ( IRow != NONE ) {
		Indexes( IRow )->Index( Row, false );

		IRow = Indexes.Next( IRow );
	}
}

void ndbtbl::table_::_DeleteFromIndexes( rrow__ Row )
{
	epeios::row__ IRow = Indexes.First();

	while ( IRow != NONE ) {
/*		if ( !Indexes( IRow )->InitializationCompleted() )
			Indexes( IRow )->CompleteInitialization();
*/
		Indexes( IRow )->Delete( Row );

		IRow = Indexes.Next( IRow );
	}
}

void ndbtbl::table_::_ReindexAll( observer_functions__ &Observer )
{
	_Test( mReadWrite );

	epeios::row__ Row = Indexes.First();

	if ( &Observer != NULL ) {
		Observer.Set( Indexes.Amount() );
	}

	while ( Row != NONE ) {
		Indexes( Row )->Reindex( Observer );

		if ( &Observer )
			Observer.IncrementHandledIndexAmount();

		Row = Indexes.Next( Row );
	}
}

void ndbtbl::table_::Retrieve(
	const rrows_ &Rows,
	data_ &Data ) const
{
ERRProlog
	datum Datum;
	epeios::row__ Row = NONE;
ERRBegin
	_Test( mReadOnly );

	Row = Rows.First();

	while ( Row != NONE ) {
		Datum.Init();

		Content().Retrieve( Rows( Row ), Datum );

		Data.Append( Datum );

		Row = Rows.Next( Row );
	}
ERRErr
ERREnd
ERREpilog
}

void ndbtbl::table_::Insert(
	const data_ &Data,
	rrows_ &RecordRows )
{
	_Test( mReadWrite );

	ctn::E_CMITEM( datum_ ) Datum;
	epeios::row__ Row = Data.First();

	Datum.Init( Data );

	while ( Row != NONE ) {
		RecordRows.Append( Insert( Datum( Row ) ) );

		Row = Data.Next( Row );
	}
}

void ndbtbl::table_::Update(
	const data_ &Data,
	const rrows_ &RecordRows )
{
	if ( Data.Amount() != RecordRows.Amount() )
		ERRu();

	_Test( mReadWrite );

	ctn::E_CMITEM( datum_ ) Datum;
	epeios::row__ Row = Data.First();

	Datum.Init( Data );

	while ( Row != NONE ) {
		Update( Datum( Row ), RecordRows( Row ) );

		Row = Data.Next( Row );
	}
}

void ndbtbl::table_::Delete( const rrows_ &RecordRows )
{
	_Test( mReadWrite );

	epeios::row__ Row = RecordRows.First();

	while ( Row != NONE ) {
		Delete( RecordRows( Row ) );

		Row = RecordRows.Next( Row );
	}
}


void ndbtbl::table_::_ResetAllIndexes( void )
{
	epeios::row__ Row = Indexes.First();

	while ( Row != NONE ) {
		Indexes( Row )->Reset();

		Row = Indexes.Next( Row );
	}
}

void ndbtbl::table_::TestRecordsExistence(
	const rrows_ &RecordRows,
	rows_ &Rows ) const
{
	_Test( mReadOnly );

	epeios::row__ Row = RecordRows.First();

	while ( Row != NONE ) {
		if ( !RecordExists( RecordRows( Row ) ) )
			Rows.Append( Row );

		Row = RecordRows.Next( Row );
	}

}

bso::bool__ ndbtbl::table_::AreAllIndexesSynchronized( void ) const
{
	_Test( mReadOnly );

	epeios::row__ Row = Indexes.First();

	while ( Row != NONE ) {
		if ( !Indexes( Row )->IsSynchronized() )
			return false;

		Row = Indexes.Next( Row );
	}

	return true;
}
/*
static const str::string_ &GetFileName_(
	const str::string_ &Path,
	const str::string_ &RootFileName,
	str::string_ &FileName )
{
#ifdef DBIEDB_DBG
	if ( RootFileName.Amount() == 0 )
		ERRc();
#endif

	FileName.Init( Path );

	if ( FileName.Amount() != 0 )
		FileName.Append( FNM_DIRECTORY_SEPARATOR_STRING );

	FileName.Append( RootFileName );

	return FileName;
}
*/

void ndbtbl::table_spreaded_file_manager___::_InitStatic(
	table_ &Table,
	const str::string_ &BaseFileName,
	mode__ Mode,
	flm::id__ ID )
{
ERRProlog
	str::string FileName;
ERRBegin
	FileName.Init();

//	GetFileName_( Path, BaseFileName, FileName );

	if ( Mode == ndbtbl::mReadOnly )
		_Static.Init( Table.Content.Static(), BaseFileName, mdr::mReadOnly, ID );
	else
		_Static.Init( Table.Content.Static(), BaseFileName, mdr::mReadWrite, ID );
ERRErr
ERREnd
ERREpilog
}

void ndbtbl::table_spreaded_file_manager___::_InitDynamic(
	table_ &Table,
	const str::string_ &BaseFileName,
	mode__ Mode,
	flm::id__ ID )
{
ERRProlog
	str::string FileName;
ERRBegin
	FileName.Init();

//	GetFileName_( Path, RootFileName, FileName );

	if ( Mode == ndbtbl::mReadOnly )
		_Dynamic.Init( Table.Content.Dynamic(), BaseFileName, mdr::mReadOnly, ID );
	else
		_Dynamic.Init( Table.Content.Dynamic(), BaseFileName, mdr::mReadWrite, ID );
ERRErr
ERREnd
ERREpilog
}




/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class ndbtblpersonnalization
: public ndbtbltutor
{
public:
	ndbtblpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~ndbtblpersonnalization( void )
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

static ndbtblpersonnalization Tutor;

ttr_tutor &NDBTBLTutor = Tutor;
