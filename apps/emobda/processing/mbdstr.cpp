/*
	'mdbstr.cpp' by Claude SIMON (http://zeusw.org/).

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

#include "mbdstr.h"

using namespace mbdstr;

table_row__ mbdstr::structure_::SearchTable( const str::string_ &Name ) const
{
	ctn::E_CITEMt( table_, table_row__ ) Table;
	table_row__ Row = Tables.First();

	Table.Init( Tables );

	while ( ( Row != NONE ) && ( Table( Row ).Name != Name ) )
		Row = Tables.Next( Row );

	return Row;
}

field_row__ mbdstr::structure_::SearchField(
	table_row__ TableRow,
	const str::string_ &Name ) const
{
	ctn::E_CITEMt( table_, table_row__ ) Table;
	ctn::E_CITEMt( field_, field_row__ ) Field;
	epeios::row__ Row = NONE;

	Table.Init( Tables );	
	Field.Init( Fields );

	const field_rows_ &FieldRows = Table( TableRow ).Fields;

	Row = FieldRows.First();

	while ( ( Row != NONE ) && ( Field( FieldRows( Row ) ).Name != Name ) )
		Row = FieldRows.Next( Row );

	if ( Row == NONE )
		return NONE;
	else
		return FieldRows( Row );
}

field_row__ mbdstr::structure_::AddField(
	table_row__ TableRow,
	const field_description_ &Description )
{
	field_row__ FieldRow = NONE;
ERRProlog
	field Field;
	field_id__ Id = MBDBSC_UNDEFINED_FIELD_ID;
ERRBegin
	if ( Description.Name.Amount() == 0 )
		ERRu();

	Id = Tables( TableRow ).GetNewFieldId( Description.GetId() );

	Field.Init( Description.Name, Description.Comment, Id, TableRow );

	FieldRow = Fields.Append( Field );

	Tables( TableRow ).Append( FieldRow );

	Tables.Flush();
ERRErr
ERREnd
ERREpilog
	return FieldRow;
}

void mbdstr::structure_::DeleteField( field_row__ FieldRow )
{
	table_row__ TableRow = GetFieldTableRow( FieldRow );

	if ( TableRow == NONE )
		ERRc();

	Tables( TableRow ).RemoveField( FieldRow );

	Tables.Flush();

	Fields.Remove( FieldRow );

	Fields.Flush();
}

void mbdstr::structure_::DeleteFields( const field_rows_ &FieldRows )
{
	epeios::row__ Row = FieldRows.Last();

	while ( Row != NONE ) {
		DeleteField( FieldRows( Row ) );

		Row = FieldRows.Last();
	}
}

void mbdstr::structure_::DeleteTable( table_row__ TableRow )
{
ERRProlog
	field_rows FieldRows;
ERRBegin
	FieldRows.Init();

	GetTableFieldRows( TableRow, FieldRows );

	DeleteFields( FieldRows );

	Tables.Delete( TableRow );
ERRErr
ERREnd
ERREpilog
}

void mbdstr::structure_::ModifyField(
	field_row__ FieldRow,
	const field_description_ &Description )
{
	if ( Description.GetId() != MBDBSC_UNDEFINED_FIELD_ID )
		ERRu();

	Fields( FieldRow ).Set( Description.Name, Description.Comment );

	Fields.Flush();
}

void mbdstr::structure_::_AddFields(
	table_row__ TableRow,									
	const field_descriptions_ &Descriptions )
{
	epeios::row__ Row = Descriptions.First();
	ctn::E_CITEM( field_description_ ) Description;

	Description.Init( Descriptions );

	while ( Row != NONE ) {
		AddField( TableRow, Description( Row ) );

		Row = Descriptions.Next( Row );
	}
}

void mbdstr::structure_::ModifyTable(
	table_row__ Row,
	const table_description_ &Description )
{
	if ( Description.GetId() != MBDBSC_UNDEFINED_TABLE_ID )
		ERRu();

	if ( Description.Fields.Amount() != 0 )
		ERRu();

	Tables( Row ).Set( Description.Name, Description.Comment );

	Tables.Flush();
}

table_row__ mbdstr::structure_::AddTable( const table_description_ &Description )
{
	table_row__ TableRow = NONE;
ERRProlog
	table Table;
	table_id__ TableId = MBDBSC_UNDEFINED_TABLE_ID;
ERRBegin
	TableId = Description.GetId();

	if ( TableId == MBDBSC_UNDEFINED_TABLE_ID )
		TableId = TableIdsStore.New();
	else
		TableIdsStore.New( TableId );

	Table.Init( Description.Name, Description.Comment, TableId );

	TableRow = Tables.Add( Table );

	_AddFields( TableRow, Description.Fields );
ERRErr
ERREnd
ERREpilog
	return TableRow;
}

static void Set_(
	structure_ &Structure,
	const mbdtbl::table_descriptions_ &Descriptions )
{
	epeios::row__ Row = Descriptions.First();
	ctn::E_CITEM( mbdtbl::table_description_ ) Description;

	Description.Init( Descriptions );

	while ( Row != NONE ) {
		Structure.AddTable( Description( Row ) );

		Row = Descriptions.Next( Row );
	}
}

void mbdstr::structure_::Init( const structure_description_ &Description )
{
	Init( Description.Name, Description.Comment );

	Set_( *this, Description.Tables );
}

bso::bool__ mbdstr::structure_::AreFieldsOwnedByTable(
	const field_rows_ &FieldRows,
	table_row__ TableRow ) const
{
	epeios::row__ Row = FieldRows.First();

	while ( ( Row != NONE ) && IsFieldOwnedByTable( FieldRows( Row ), TableRow ) )
		Row = FieldRows.Next( Row );

	return Row == NONE;
}

bso::bool__ mbdstr::structure_::FieldsExist( const field_rows_ &FieldRows ) const
{
	epeios::row__ Row = FieldRows.First();

	while ( ( Row != NONE ) && FieldExists( FieldRows( Row ) ) )
		Row = FieldRows.Next( Row );

	return Row == NONE;
}


