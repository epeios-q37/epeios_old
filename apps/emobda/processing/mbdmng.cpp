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

void mbdmng::manager_::DeleteField( field_row__ FieldRow )
{
ERRProlog
	record Record;
	raw_datum RawDatum;
	table_id__ TableId = MBDBSC_UNDEFINED_TABLE_ID;
	field_id__ FieldId = MBDBSC_UNDEFINED_FIELD_ID;
	record_row__ RecordRow = NONE;
ERRBegin
	FieldId = Structure.GetFieldFieldId( FieldRow );
	TableId = Structure.GetFieldTableId( FieldRow );

	Record.Init( TableId, FieldId, MBDBSC_UNDEFINED_RECORD_ID, datum( "" ) );

	RawDatum.Init();
	Convert( Record, RawDatum );

	RecordRow = Engine.TableFieldDatumIndex.StrictSeek( RawDatum, dbsidx::bStop, 1 );	// We ignore the datum.

	while ( RecordRow != NONE ) {
		Engine.Delete( RecordRow );

		RecordRow = Engine.TableFieldDatumIndex.StrictSeek( RawDatum, dbsidx::bStop, 1 );	// We ignore the datum.
	}

	Structure.DeleteField( FieldRow );

	_ExportStructure();
ERRErr
ERREnd
ERREpilog
}


const record_ &mbdmng::manager_::_GetRecord(
	dbstbl::rrow__ Row,
	record_ &Record ) const
{
ERRProlog
	raw_datum RawDatum;
ERRBegin
	RawDatum.Init();

	Engine.Retrieve( Row, RawDatum );

	Extract( RawDatum, Record );
ERRErr
ERREnd
ERREpilog
	return Record;
}

record_id__ mbdmng::manager_::_GetRecordId( dbstbl::rrow__ Row ) const
{
	record_id__ RecordId = MBDBSC_UNDEFINED_RECORD_ID;
ERRProlog
	record Record;
ERRBegin
	Record.Init();

	RecordId = _GetRecord( Row, Record ).GetStaticPart().RecordId;
ERRErr
ERREnd
ERREpilog
	return RecordId;
}

const datum_ &mbdmng::manager_::_GetDatum(
	dbstbl::rrow__ Row,
	datum_ &Datum ) const
{
ERRProlog
	record Record;
ERRBegin
	Record.Init();

	Datum.Append( _GetRecord( Row, Record ).Datum );
ERRErr
ERREnd
ERREpilog
	return Datum;
}

bso::bool__ mbdmng::manager_::RecordExists(
	record_id__ RecordId,
	table_row__ TableRow ) const
{
	bso::bool__ Exists = false;
ERRProlog
	record Record;
	raw_datum RawDatum;
	table_id__ TableId = MBDBSC_UNDEFINED_TABLE_ID;
ERRBegin
	TableId = Structure.GetTableTableId( TableRow );

	Record.Init( TableId, MBDBSC_UNDEFINED_FIELD_ID, RecordId, datum( "" ) );

	RawDatum.Init();
	Convert( Record, RawDatum );

	Exists = Engine.TableRecordFieldIndex.StrictSeek( RawDatum, dbsidx::bStop, 1 ) != NONE;	// We ignore the field.
ERRErr
ERREnd
ERREpilog
	return Exists;
}


bso::bool__ mbdmng::manager_::_AddOrModifyRecord(
	record_id__ RecordId,
	const data_ &Data,
	table_row__ TableRow,
	const field_rows_ &FieldRows )
{
	bso::bool__ Modification = false;
ERRProlog
	ctn::E_CMITEM( datum_ ) Datum;
	epeios::row__ Row = NONE;
	record Record;
	raw_datum RawDatum;
	field_row__ FieldRow = NONE;
	table_id__ TableId = MBDBSC_UNDEFINED_TABLE_ID;
	record_row__ RecordRow = NONE;
ERRBegin
	ctn::E_CMITEM( datum_ ) Datum;
	Row = Data.First();

#ifdef MBDMNG__DBG
	if ( Data.Amount() != FieldRows.Amount() )
		ERRu();

	if ( !Exist( FieldRows ) )
		ERRu();

	if ( !AreUnique( FieldRows ) )
		ERRu();
#endif
	TableId = Structure.GetTableTableId( TableRow );

	Datum.Init( Data );

	while ( Row != NONE ) {
		FieldRow = FieldRows( Row );
		Record.Init( TableId, Structure.GetFieldFieldId( FieldRow ), RecordId, Datum( Row ) );

		RawDatum.Init();
		Convert( Record, RawDatum );

		RecordRow = Engine.TableRecordFieldIndex.StrictSeek( RawDatum, dbsidx::bStop, DBSIDX_NO_SKIP );

		if ( RecordRow != NONE ) {
			Modification = true;
			Engine.Delete( RecordRow );
		}

		Engine.Insert( RawDatum );

		Row = Data.Next( Row );
	}
ERRErr
ERREnd
ERREpilog
	return Modification;
}

void mbdmng::manager_::DeleteRecord(
	record_id__ RecordId,
	table_row__ TableRow )
{
ERRProlog
	record Record;
	raw_datum RawDatum;
	table_id__ TableId = MBDBSC_UNDEFINED_TABLE_ID;
	record_row__ RecordRow = NONE;
ERRBegin
	TableId = Structure.GetTableTableId( TableRow );

	Record.Init( TableId, MBDBSC_UNDEFINED_FIELD_ID, RecordId, datum( "" ) );

	RawDatum.Init();
	Convert( Record, RawDatum );

	RecordRow = Engine.TableRecordFieldIndex.StrictSeek( RawDatum, dbsidx::bStop, 1 );	// We ignore the field.

	while ( RecordRow != NONE ) {
		Engine.Delete( RecordRow );

		RecordRow = Engine.TableRecordFieldIndex.StrictSeek( RawDatum, dbsidx::bStop, 1 );	// We ignore the field.
	}
ERRErr
ERREnd
ERREpilog
}

void mbdmng::manager_::GetRecords(
	table_row__ TableRow,
	record_ids_ &RecordIds ) const
{
ERRProlog
	record_row__ RecordRow;
	record Record;
	raw_datum RawDatum;
	record_row__ NextToLastRecordRow = NONE;
	record_id__
			RecordId = MBDBSC_UNDEFINED_RECORD_ID,
			PreviousRecordId = MBDBSC_UNDEFINED_RECORD_ID;
ERRBegin
	Record.Init( Structure.GetTableTableId( TableRow ), MBDBSC_UNDEFINED_FIELD_ID, MBDBSC_UNDEFINED_RECORD_ID, datum() );

	RawDatum.Init();
	mbdbsc::Convert( Record, RawDatum );

	RecordRow = Engine.TableRecordFieldIndex.StrictSeek( RawDatum, dbsidx::bGreater, 2 );

	if ( RecordRow != NONE ) {
		NextToLastRecordRow = Engine.TableRecordFieldIndex.Next( RecordRow );	// Can be 'NONE' without ptoblem.

		RecordRow = Engine.TableRecordFieldIndex.StrictSeek( RawDatum, dbsidx::bLesser, 2 );

		while ( RecordRow != NextToLastRecordRow ) {
			RawDatum.Init();
			Engine.Retrieve( RecordRow, RawDatum );

			RecordId = mbdbsc::ExtractRecordStaticPart( RawDatum ).RecordId;

			if ( PreviousRecordId != RecordId ) {
				RecordIds.Append( mbdbsc::ExtractRecordStaticPart( RawDatum ).RecordId );
				PreviousRecordId = RecordId;
			}

			RecordRow = Engine.TableRecordFieldIndex.Next( RecordRow );
		}
	}
ERRErr
ERREnd
ERREpilog
}

void mbdmng::manager_::GetRecord(
	table_row__ TableRow,
	const field_rows_ &FieldRows,
	record_id__ RecordId,
	data_ &Data ) const
{
ERRProlog
	record Record;
	epeios::row__ Row = NONE;
	dbstbl::rrow__ RawRecordRow = NONE;
	raw_datum RawDatum;
	datum Datum;
	field_row__ FieldRow = NONE;
	table_id__ TableId= MBDBSC_UNDEFINED_TABLE_ID;
ERRBegin
#ifdef MBDMNG__DBG
	if ( !Exist( FieldRows ) )
		ERRu();

	if ( !AreUnique( FieldRows ) )
		ERRu();
#endif
	TableId = Structure.GetTableTableId( TableRow );
	Record.Init();
	
	Record.GetStaticPart().RecordId = RecordId;

	Row = FieldRows.First();

	while ( Row != NONE ) {
		FieldRow = FieldRows( Row );

		Record.GetStaticPart().TableId = TableId;
		Record.GetStaticPart().FieldId = Structure.GetFieldFieldId( FieldRow );

		RawDatum.Init();

		Convert( Record, RawDatum );

		RawRecordRow = Engine.StrictSeek( RawDatum, Engine.GetTableRecordFieldIndexRow(), dbsidx::bStop, DBSIDX_NO_SKIP );

		Datum.Init();

		if ( RawRecordRow != NONE )
			_GetDatum( RawRecordRow, Datum );

		Data.Append( Datum );

		Row = FieldRows.Next( Row );
	}
ERRErr
ERREnd
ERREpilog
}

void mbdmng::manager_::GetRecords(
	table_row__ TableRow,
	const field_rows_ &FieldRows,
	const record_ids_ &RecordIds,
	data_cluster_ &DataCluster ) const
{
ERRProlog
	epeios::row__ Row = NONE;
	data Data;
ERRBegin
	Row = RecordIds.First();

	while ( Row != NONE ) {
		Data.Init();

		GetRecord( TableRow, FieldRows, RecordIds( Row ), Data );

		DataCluster.Append( Data );

		Row = RecordIds.Next( Row );
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

	RawRecordRow = Engine.StrictSeek( RawDatum, Engine.TableRecordFieldIndexRow(), dbsidx::bStop, 1 );

	while ( RawRecordRow != NONE ) {
		Engine.Delete( RawRecordRow );

		RawRecordRow = Engine.StrictSeek( RawDatum, Engine.TableRecordFieldIndexRow(), dbsidx::bStop, 1 );
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

inline static const char *GetStructureFileName_(
	str::string_ &Location,
	FNM_BUFFER___ &Buffer )
{
ERRProlog
	STR_BUFFER___ LocationBuffer;
ERRBegin
	fnm::BuildFileName( Location.Convert( LocationBuffer ), "", ".embd", Buffer );
ERRErr
ERREnd
ERREpilog
	return Buffer;
}

bso::bool__ mbdmng::manager_::_ExportStructure( void )
{
	bso::bool__ Success = false;
ERRProlog
	flf::file_oflow___ Flow;
	txf::text_oflow__ TFlow( Flow );
	xml::writer Writer;
	FNM_BUFFER___ Buffer;
	const char *Filename = NULL;
ERRBegin
	if ( Flow.Init( GetStructureFileName_( Location, Buffer ), err::hSkip ) == fil::sSuccess ) {
		Success = true;

		Writer.Init( TFlow );

		mbddsc::Export( Structure, Writer, true );
	}
ERRErr
ERREnd
ERREpilog
	return Success;
}

static bso::bool__ Import_(
	const char *Filename,
	mbddsc::description_ &Description )
{
	bso::bool__ Success = false;
ERRProlog
	flf::file_iflow___ Flow;
	xtf::extended_text_iflow__ XFlow;
ERRBegin
	if ( Flow.Init( Filename, err::hSkip ) == fil::sSuccess ) {
		XFlow.Init( Flow );

		Success = mbddsc::Import( XFlow, Description );
	}
ERRErr
ERREnd
ERREpilog
	return Success;
}

bso::bool__ mbdmng::manager_::_ImportStructure( void )
{
	bso::bool__ Success = false;
ERRProlog
	FNM_BUFFER___ Buffer;
	const char *Filename = NULL;
	mbddsc::description Description;
ERRBegin
	Filename = GetStructureFileName_( Location, Buffer );

	if ( Success = fil::FileExists( Filename ) ) {
		Description.Init();

		Success = Import_( Filename, Description );
	}

	Structure.Init( Description );
ERRErr
ERREnd
ERREpilog
	return Success;
}

bso::bool__ mbdmng::manager_::_Create( void )
{
	bso::bool__ Success = false;
ERRProlog
	STR_BUFFER___ Buffer;
ERRBegin
	if ( dir::CreateDir( Location.Convert( Buffer ) ) == dir::sOK )
		Success = _ExportStructure();
ERRErr
ERREnd
ERREpilog
	return Success;
}

bso::bool__ mbdmng::manager_::_Retrieve( void )
{
	bso::bool__ Success = false;
ERRProlog
	tol::E_FPOINTER___( bso::char__ ) Buffer;
ERRBegin
	Success = _ImportStructure();
ERRErr
ERREnd
ERREpilog
	return Success;
}

bso::bool__ mbdmng::manager_::_SubInit( type__ Type )
{
	bso::bool__ Success = false;

	switch ( Type ) {
	case tCreate:
		Success = _Create();
		break;
	case tRetrieve:
		Success = _Retrieve();
		break;
	default:
		ERRu();
		break;
	}

	return Success;
}


