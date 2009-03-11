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

#define MBDSTR_STRUCTURE_VERSION	"0.1.0"	// Doit être modifié dés que la structure interne (le format des fichiers) est modifié.

namespace mbdstr {
	using namespace mbdfld;
	using namespace mbdtbl;

	typedef ids::E_IDS_STORE_( table_id__ ) table_ids_store_;

	class structure_core_
	{
	public:
		struct s
		{
			str::string_::s Name, Comment;
		};
		str::string_ Name, Comment;
		structure_core_( s &S )
		: Name( S.Name ),
		  Comment( S.Comment )
		{}
		void reset( bso::bool__ P = true )
		{
			Name.reset( P );
			Comment.reset( P );
		}
		void plug( mmm::E_MULTIMEMORY_ &MM )
		{
			Name.plug( MM );
			Comment.plug( MM );
		}
		structure_core_ &operator =( const structure_core_ &SD )
		{
			Name = SD.Name;
			Comment= SD.Comment;

			return *this;
		}
		void Init( void )
		{
			reset();

			Name.Init();
			Comment.Init();
		}
		void Init(
			const str::string_ &Name,
			const str::string_ &Comment )
		{
			Init();

			this->Name = Name;
			this->Comment = Comment;
		}
	};

	class structure_
	: public structure_core_
	{
	private:
		void _AddFields(
			table_row__ TableRow,
			const field_descriptions_ &Descriptions );
	public:
		struct s
		: public structure_core_::s
		{
			tables_::s Tables;
			fields_::s Fields;
			table_ids_store_::s TableIdsStore;
		};
		tables_ Tables;
		fields_ Fields;
		table_ids_store_ TableIdsStore;
		structure_( s &S )
		: structure_core_( S ),
		  Tables( S.Tables ),
		  Fields( S.Fields ),
		  TableIdsStore( S.TableIdsStore )
		{}
		void reset( bso::bool__ P = true )
		{
			structure_core_::reset( P );
			Tables.reset( P );
			Fields.reset( P );
			TableIdsStore.reset( P );
		}
		void plug( mmm::E_MULTIMEMORY_ &MM )
		{
			structure_core_::plug( MM );
			Tables.plug( MM );
			Fields.plug( MM );
			TableIdsStore.plug( MM );
		}
		structure_ &operator =( const structure_ &S )
		{
			structure_core_::operator =( S );
			Tables = S.Tables;
			Fields = S.Fields;
			TableIdsStore = S.TableIdsStore;

			return *this;
		}
		void Init( void )
		{
			reset();

			structure_core_::Init();
			Tables.Init();
			Fields.Init();
			TableIdsStore.Init();
		}
		void Init(
			const str::string_ &Name,
			const str::string_ &Comment )
		{
			Init();

			structure_core_::Init( Name, Comment );
		}
		void Init( const class structure_description_ &Description );
		void ModifyField(
			field_row__ FieldRow,
			const field_description_ &Description );
		field_row__ AddField(
			table_row__ TableRow,
			const field_description_ &Description );
		table_row__ SearchTable( const str::string_ &Name ) const;
		field_row__ SearchField(
			table_row__ TableRow,
			const str::string_ &Name ) const;
		bso::bool__ TableExists( table_row__ TableRow ) const
		{
			return Tables.Exists( TableRow );
		}
		bso::bool__ FieldExists( field_row__ FieldRow ) const
		{
			return Fields.Exists( FieldRow );
		}
		bso::bool__ FieldsExist( const field_rows_ &FieldRows ) const;
		bso::bool__ IsFieldOwnedByTable(
			field_row__ FieldRow,
			table_row__ TableRow ) const
		{
			ctn::E_CITEMt( table_, table_row__ ) Table;

			Table.Init( Tables );

			return Table( TableRow ).OwnsField( FieldRow );
		}
		bso::bool__ AreFieldsOwnedByTable(
			const field_rows_ &FieldRows,
			table_row__ TableRow ) const;
		field_id__ GetFieldFieldId( field_row__ FieldRow ) const
		{
			ctn::E_CITEMt( field_, field_row__ ) Field;
			Field.Init( Fields );

			return Field( FieldRow ).Id();
		}
		table_id__ GetTableTableId( table_row__ TableRow ) const
		{
			ctn::E_CITEMt( table_, table_row__ ) Table;

			Table.Init( Tables );

			return Table( TableRow ).Id();
		}
		table_row__ AddTable( const table_description_ &Description );
		void ModifyTable(
			table_row__ Row,
			const table_description_ &Description );
	};

	E_AUTO( structure );


	class structure_description_
	: public structure_core_
	{
	public:
		struct s
		: public structure_core_::s
		{
			mbdtbl::table_descriptions_::s Tables;
		};
		mbdtbl::table_descriptions_ Tables;
		structure_description_( s &S )
		: structure_core_( S ),
		  Tables( S.Tables )
		{}
		void reset( bso::bool__ P = true )
		{
			structure_core_::reset( P );
			Tables.reset( P );
		}
		void plug( mmm::E_MULTIMEMORY_ &MM )
		{
			structure_core_::plug( MM );
			Tables.plug( MM );
		}
		structure_description_ &operator =( const structure_description_ &SD )
		{
			structure_core_::operator =( SD );
			Tables = SD.Tables;

			return *this;
		}
		void Init( void )
		{
			reset();

			structure_core_::Init();
			Tables.Init();
		}
		void Init(
			const str::string_ &Name,
			const str::string_ &Comment )
		{
			Init();

			structure_core_::Init( Name, Comment );
		}

	};

	E_AUTO( structure_description );

	void Set(
		structure_ &Structure,
		const structure_description_ &Description );
}

#endif