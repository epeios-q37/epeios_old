/*
	'mdbtbl.cpp' by Claude SIMON (http://zeusw.org/).

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

#include "mbdtbl.h"

using namespace mbdtbl;

using namespace mbdfld;

static void AppendFields_(
	table_ &Table,
	fields_ &Fields,
	const field_descriptions_ &FieldDescriptions )
{
ERRProlog
	epeios::row__ Row = NONE;
	field Field;
	ctn::E_CITEM( field_description_ ) FieldDescription;
ERRBegin
	FieldDescription.Init( FieldDescriptions );

	Row = FieldDescriptions.First();

	while ( Row != NONE ) {
		Field.Init();

		Set( Field, FieldDescription( Row ) );

		Table.Append( Fields.Append( Field ) );

		Row = FieldDescriptions.Next( Row );
	}
ERRErr
ERREnd
ERREpilog
}

void mbdtbl::Set(
	table_ &Table,
	fields_ &Fields,
	const table_description_ &TableDescription )
{
ERRProlog
	field Field;
ERRBegin
	Table.Init( TableDescription.Name, TableDescription.Comment, TableDescription.Id() );

	AppendFields_( Table, Fields, TableDescription.Fields );
ERRErr
ERREnd
ERREpilog
}

void mdbtbl::Set(
	tables_ &Tables,
	const table_descriptions_ &TableDescriptions );
