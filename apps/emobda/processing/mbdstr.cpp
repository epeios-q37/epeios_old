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
	ctn::E_CITEMt( field_, field_row__ ) Field;
	field_row__ Row = Fields.First();
	table_id__ TableId = GetTableTableId( TableRow );

	Field.Init( Fields );

	while ( ( Row != NONE )
		    && ( ( Field( Row ).GetTableId() != TableId )
			     || ( Field( Row ).Name != Name ) ) )
		Row = Fields.Next( Row );

	return Row;
}

field_row__ mbdstr::structure_::AddField(
	table_row__ TableRow,
	const field_description_ &Description )
{
	field_row__ FieldRow = NONE;
ERRProlog
	field Field;
	field_id__ FieldId = MBDBSC_UNDEFINED_FIELD_ID;
	table_id__ TableId = MBDBSC_UNDEFINED_TABLE_ID;
ERRBegin
	if ( Description.Name.Amount() == 0 )
		ERRu();

	TableId = Description.GetTableId();

	if ( TableId != MBDBSC_UNDEFINED_TABLE_ID )
		if ( TableId != Tables( TableRow ).GetId() )
			ERRu();

	FieldId = Tables( TableRow ).GetNewFieldId( Description.GetId() );

	Field.Init( Description.Name, Description.Comment, TableId, FieldId );

	FieldRow = Fields.Add( Field );

	Tables( TableRow ).Append( FieldRow );

	Tables.Flush();
ERRErr
ERREnd
ERREpilog
	return FieldRow;
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
ERRErr
ERREnd
ERREpilog
	return TableRow;
}



