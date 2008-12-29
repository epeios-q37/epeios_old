/*
	'mbdmng.cpp' by Claude SIMON (http://zeusw.org/).

	 This file is part of 'emobda' software.

    'emobda' is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'emobda' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with 'emobda'.  If not, see <http://www.gnu.org/licenses/>.
*/

// $Id$

#include "mbdmng.h"

using namespace mbdmng;

const record_ &mbdmng::manager_::GetRecord_(
	dbstbl::rrow__ Row,
	record_ &Record ) const
{
ERRProlog
	raw_datum RawDatum;
ERRBegin
	RawDatum.Init();

	Table.Retrieve( Row, RawDatum );
ERRErr
ERREnd
ERREpilog
	return Record;
}

record_id__ mbdmng::manager_::GetRecordId_( dbstbl::rrow__ Row ) const
{
	record_id__ RecordId = MBDBSC_UNDEFINED_RECORD_ID;
ERRProlog
	record Record;
ERRBegin
	Record.Init();

	RecordId = GetRecord_( Row, Record ).GetStaticPart().RecordId;
ERRErr
ERREnd
ERREpilog
	return RecordId;
}

const datum_ mbdmng::manager_::GetDatum_(
	dbstbl::rrow__ Row,
	datum_ &Datum ) const
{
ERRProlog
	record Record;
ERRBegin
	Record.Init();

	Datum.Append( GetRecord_( Row, Record ).Datum );
ERRErr
ERREnd
ERREpilog
	return Datum;
}


record_id__ mbdmng::manager_::AddRecord(
	const data_ &Data,
	const field_rows_ &FieldRows )
{
	record_id__ RecordId = MBDBSC_UNDEFINED_RECORD_ID;
ERRProlog
	ctn::E_CMITEM( datum_ ) Datum;
	epeios::row__ Row = NONE;
	record Record;
	raw_datum RawDatum;
ERRBegin
	ctn::E_CMITEM( datum_ ) Datum;
	Row = Data.First();
	RecordId = GetLastRecordId() + 1;

#ifdef MBDMNG__DBG
	if ( Data.Amount() != FieldRows.Amount() )
		ERRu();

	if ( !Exist( FieldRows ) )
		ERRu();

	if ( !AreUnique( FieldRows ) )
		ERRu();
#endif
	Datum.Init( Data );

	while ( Row != NONE ) {
		Record.Init( RecordId, FieldRows( Row ), Datum( Row ) );

		RawDatum.Init();
		Convert( Record, RawDatum );

		Table.Insert( RawDatum );

		Row = Data.Next( Row );
	}
ERRErr
ERREnd
ERREpilog
	return RecordId;
}

void mbdmng::manager_::GetRecord(
	record_id__ RecordId,
	const field_rows_ &FieldRows,
	data_ &Data ) const
{
ERRProlog
	record Record;
	epeios::row__ Row = NONE;
	dbstbl::rrow__ RawRecordRow = NONE;
	raw_datum RawDatum;
	datum Datum;
ERRBegin
#ifdef MBDMNG__DBG
	if ( !Exist( FieldRows ) )
		ERRu();

	if ( !AreUnique( FieldRows ) )
		ERRu();
#endif
	Record.Init();
	
	Record.GetStaticPart().RecordId = RecordId;

	while ( Row != NONE ) {
		Record.GetStaticPart().FieldRow = FieldRows( Row );

		RawDatum.Init();

		Convert( Record, RawDatum );

		RawRecordRow = Table.Seek( RawDatum, Table.GetRecordIdFieldRowIndexRow(), dbstbl::bStop, DBSIDX_NO_SKIP );

		Datum.Init();

		if ( RawRecordRow != NONE )
			GetDatum_( RawRecordRow, Datum );

		Data.Append( Datum );

		Row = FieldRows.Next( Row );
	}
ERRErr
ERREnd
ERREpilog
}

void mbdmng::manager_::DeleteRecord( record_id__ RecordId )
{
ERRProlog
	record Record;
	raw_datum RawDatum;
	dbstbl::rrow__ RawRecordRow = NONE;
ERRBegin
	Record.Init();
	
	Record.GetStaticPart().RecordId = RecordId;

	RawDatum.Init();
	Convert( Record, RawDatum );

	RawRecordRow = Table.Seek( RawDatum, Table.GetRecordIdFieldRowIndexRow(), dbstbl::bStop, 1 );

	while ( RawRecordRow != NONE ) {
		Table.Delete( RawRecordRow );

		RawRecordRow = Table.Seek( RawDatum, Table.GetRecordIdFieldRowIndexRow(), dbstbl::bStop, 1 );
	}
ERRErr
ERREnd
ERREpilog
}

epeios::row__ mbdmng::manager_::TestExistence( const mbdbsc::field_rows_ &FieldRows ) const
{
	epeios::row__ Row = FieldRows.First();

	while ( ( Row != NONE ) && ( Structure.Fields.Exists( FieldRows( Row ) ) ) )
		Row = FieldRows.Next( Row );

	return Row;
}

epeios::row__ mbdmng::TestUnicity( const field_rows_ &FieldRows )
{
	epeios::row__ Row = FieldRows.Last();

	while ( ( Row != NONE ) && ( FieldRows.Search( FieldRows( Row ) ) == Row ) )
		Row = FieldRows.Previous( Row );

	return Row;
}



