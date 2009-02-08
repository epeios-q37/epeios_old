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
#include "fnm.h"
#include "flf.h"
#include "fil.h"
#include "dir.h"
#include "mbddsc.h"

using namespace mbdmng;

const record_ &mbdmng::manager_::GetRecord_(
	dbstbl::rrow__ Row,
	record_ &Record ) const
{
ERRProlog
	raw_datum RawDatum;
ERRBegin
	RawDatum.Init();

	Engine.Retrieve( Row, RawDatum );
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
	field_row__ FieldRow = NONE;
ERRBegin
	ctn::E_CMITEM( datum_ ) Datum;
	Row = Data.First();
	RecordId = *GetLastRecordId() + 1;

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
		FieldRow = FieldRows( Row );
		Record.Init( Structure.GetFieldTableId( FieldRow ), Structure.GetFieldFieldId( FieldRow ), RecordId, Datum( Row ) );

		RawDatum.Init();
		Convert( Record, RawDatum );

		Engine.Insert( RawDatum );

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
	field_row__ FieldRow = NONE;
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
		FieldRow = FieldRows( Row );

		Record.GetStaticPart().TableId = Structure.GetFieldTableId( FieldRow );
		Record.GetStaticPart().FieldId = Structure.GetFieldFieldId( FieldRow );

		RawDatum.Init();

		Convert( Record, RawDatum );

		RawRecordRow = Engine.Seek( RawDatum, Engine.GetTableRecordFieldIndexRow(), dbstbl::bStop, DBSIDX_NO_SKIP );

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

	RawRecordRow = Engine.Seek( RawDatum, Engine.TableRecordFieldIndexRow(), dbstbl::bStop, 1 );

	while ( RawRecordRow != NONE ) {
		Engine.Delete( RawRecordRow );

		RawRecordRow = Engine.Seek( RawDatum, Engine.TableRecordFieldIndexRow(), dbstbl::bStop, 1 );
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

void mbdmng::manager_::ExportStructure_( void )
{
ERRProlog
	flf::file_oflow___ Flow;
	txf::text_oflow__ TFlow( Flow );
	xml::writer Writer;
	tol::E_FPOINTER___( bso::char__ ) LocationBuffer;
	FNM_BUFFER___ Buffer;
	const char *Filename = NULL;
ERRBegin
	Filename = fnm::BuildFileName( LocationBuffer = Location.Convert(), "", ".embd", Buffer );

	Flow.Init( Filename );

	Writer.Init( TFlow );

	mbddsc::Export( Structure, Writer, true );
ERRErr
ERREnd
ERREpilog
}

static void Import_(
	const char *Filename,
	mbddsc::description_ &Description )
{
ERRProlog
	flf::file_iflow___ Flow;
	xtf::extended_text_iflow__ XFlow;
ERRBegin
	Flow.Init( Filename );

	XFlow.Init( Flow );

	mbddsc::Import( XFlow, Description );
ERRErr
ERREnd
ERREpilog
}

bso::bool__ mbdmng::manager_::ImportStructure_( void )
{
	bso::bool__ Exists = false;
ERRProlog
	tol::E_FPOINTER___( bso::char__ ) LocationBuffer;
	FNM_BUFFER___ Buffer;
	const char *Filename = NULL;
	mbddsc::description Description;
ERRBegin
	Filename = fnm::BuildFileName( LocationBuffer = Location.Convert(), "", ".embd", Buffer );

	if ( Exists = fil::FileExists( Filename ) ) {
		Description.Init();

		Import_( Filename, Description );
	}
		
ERRErr
ERREnd
ERREpilog
	return Exists;
}

