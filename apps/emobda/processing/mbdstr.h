/*
	'mdbstr.h' by Claude SIMON (http://zeusw.org/).

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

// eMoBDa STRucture

#ifndef MBDSTR__INC
#define MBDSTR__INC

#include "mbdfld.h"
#include "mbdtbl.h"

#define MBDSTR_STRUCTURE_VERSION	"0.1.0"	// Doit �tre modifi� d�s que la structure interne (le format des fichiers) est modifi�.

namespace mbdstr {
	using namespace mbdfld;
	using namespace mbdtbl;

	typedef ids::E_IDS_STORE_( table_id__ ) table_ids_store_;

	class structure_
	{
	private:
		void _AddFields(
			table_row__ TableRow,
			const field_descriptions_ &Descriptions );
	public:
		struct s
		{
			str::string_::s Name, Comment;
			tables_::s Tables;
			fields_::s Fields;
			table_ids_store_::s TableIdsStore;
		};
		str::string_ Name, Comment;
		tables_ Tables;
		fields_ Fields;
		table_ids_store_ TableIdsStore;
		structure_( s &S )
		: Name( S.Name ),
		  Comment( S.Comment ),
		  Tables( S.Tables ),
		  Fields( S.Fields ),
		  TableIdsStore( S.TableIdsStore )
		{}
		void reset( bso::bool__ P = true )
		{
			Name.reset( P );
			Comment.reset( P );
			Tables.reset( P );
			Fields.reset( P );
			TableIdsStore.reset( P );
		}
		void plug( mmm::E_MULTIMEMORY_ &MM )
		{
			Name.plug( MM );
			Comment.plug( MM );
			Tables.plug( MM );
			Fields.plug( MM );
			TableIdsStore.plug( MM );
		}
		structure_ &operator =( const structure_ &D )
		{
			Name = D.Name;
			Comment= D.Comment;
			Tables = D.Tables;
			Fields = D.Fields;
			TableIdsStore = D.TableIdsStore;

			return *this;
		}
		void Init( void )
		{
			reset();

			Name.Init();
			Comment.Init();
			Tables.Init();
			Fields.Init();
			TableIdsStore.Init();
		}
		void Init(
			const str::string_ &Name,
			const str::string_ &Comment )
		{
			Init();

			this->Name = Name;
			this->Comment = Comment;
		}
		field_row__ AddField(
			table_row__ TableRow,
			const field_description_ &Description );
		table_row__ SearchTable( const str::string_ &Name ) const;
		field_row__ SearchField(
			table_row__ TableRow,
			const str::string_ &Name ) const;
		void GetFieldTableAndRecordId(
			field_row__ FieldRow,
			table_id__ &TableId,
			field_id__ &FieldId ) const
		{
			ctn::E_CITEMt( field_, field_row__ ) Field;
			Field.Init( Fields );

			TableId = Field( FieldRow ).TableId();
			FieldId = Field( FieldRow ).FieldId();
		}
		table_id__ GetFieldTableId( field_row__ FieldRow ) const
		{
			table_id__ TableId = MBDBSC_UNDEFINED_TABLE_ID;
			field_id__ FieldId = MBDBSC_UNDEFINED_FIELD_ID;

			GetFieldTableAndRecordId( FieldRow, TableId, FieldId );

			return TableId;
		}
		field_id__ GetFieldFieldId( field_row__ FieldRow ) const
		{
			table_id__ TableId = MBDBSC_UNDEFINED_TABLE_ID;
			field_id__ FieldId = MBDBSC_UNDEFINED_FIELD_ID;

			GetFieldTableAndRecordId( FieldRow, TableId, FieldId );

			return FieldId;
		}
		table_id__ GetTableTableId( table_row__ TableRow ) const
		{
			ctn::E_CITEMt( table_, table_row__ ) Table;

			Table.Init( Tables );

			return Table( TableRow ).TableId();
		}
		table_row__ AddTable( const table_description_ &Description );
	};

	E_AUTO( structure );
}

#endif