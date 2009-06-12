/*
	'mbdbkd.cpp' by Claude SIMON (http://zeusw.org/).

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

#include "mbdbkd.h"

bso::bool__ mbdbkd::backend___::GetTableInfos(
	table__ Table,
	str::string_ &Name,
	str::string_ &Comment,
	table_id__ &Id )
{
	bso::bool__ Success = false;
ERRProlog
	tables Tables;
	bkdacc::strings Names, Comments;
	table_ids Ids;
ERRBegin
	Tables.Init();
	Tables.Append( Table );

	Names.Init();
	Comments.Init();
	Ids.Init();

	if ( Success = GetTablesInfos( Tables, Names, Comments, Ids ) ) {
		Name = Names( Names.First() );
		Comment = Comments( Comments.First() );
		Id = Ids( Ids.First() );
	}
ERRErr
ERREnd
ERREpilog
	return Success;
}

bso::bool__ mbdbkd::backend___::GetFieldInfos(
	field__ Field,
	str::string_ &Name,
	str::string_ &Comment,
	field_id__ &Id,
	table__ &Table )
{
	bso::bool__ Success = false;
ERRProlog
	fields Fields;
	bkdacc::strings Names, Comments;
	field_ids Ids;
	tables Tables;
ERRBegin
	Fields.Init();
	Fields.Append( Field );

	Names.Init();
	Comments.Init();
	Ids.Init();
	Tables.Init();

	if ( Success = GetFieldsInfos( Fields, Names, Comments, Ids, Tables ) ) {
		Name = Names( Names.First() );
		Comment = Comments( Comments.First() );
		Id = Ids( Ids.First() );
		Table = Tables( Tables.First() );
	}
ERRErr
ERREnd
ERREpilog
	return Success;
}
