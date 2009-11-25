/*
	'mbdmng.h' by Claude SIMON (http://zeusw.org/).

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

// eMoBDa MaNaGer

#ifndef MBDMNG__INC
#define MBDMNG__INC

#ifdef XXX_DBG
#	define MBDMNG__DBG
#endif

#include "mbdeng.h"
#include "mbdstr.h"
#include "mbddsc.h"

namespace mbdmng {

	using namespace mbdeng;
	using namespace mbdstr;
	using namespace mbdbsc;

	enum type__ {
		tCreate,	// Création d'une nouvelle base.
		tRetrieve,	// Récupération.
		t_amont,
		t_Undefined
	};

	class manager_
	{
	private:
		const record_ &_GetRecord(
			ndbtbl::rrow__ Row,
			record_ &Record ) const;
		record_id__ _GetRecordId( ndbtbl::rrow__ Row ) const;
		const datum_ &_GetDatum(
			ndbtbl::rrow__ Row,
			datum_ &Datum ) const;
		bso::bool__ _ExportStructure( void );
		bso::bool__ _ImportStructure( void );
		bso::bool__ _Create( void );
		bso::bool__ _Retrieve( void );
		bso::bool__ _SubInit( type__ Type );
		bso::bool__ _AddOrModifyRecord(
			record_id__ RecordId,
			const data_ &Data,
			table_row__ TableRow,
			const field_rows_ &FieldRows );
	public:
		struct s {
			mbdeng::engine_::s Engine;
			mbdstr::structure_::s Structure;
			str::string_::s BaseDirectoryName;
		};
		mbdeng::engine_ Engine;
		mbdstr::structure_ Structure;
		str::string_ BaseDirectoryName;
		manager_( s &S )
		: Engine( S.Engine ),
		  Structure( S.Structure ),
		  BaseDirectoryName( S.BaseDirectoryName )
		{}
		void reset( bso::bool__ P = true )
		{
			Engine.reset( P );
			Structure.reset( P );
			BaseDirectoryName.reset( P );
		}
		void plug( mmm::E_MULTIMEMORY_ &MM )
		{
			Engine.plug( MM );
			Structure.plug( MM );
			BaseDirectoryName.plug( MM );
		}
		manager_ &operator =( const manager_ &M )
		{
			Engine = M.Engine;
			Structure = M.Structure;
			BaseDirectoryName = M.BaseDirectoryName;

			return *this;
		}
		bso::bool__ Init(
			const str::string_ &Location,
			const str::string_ &BaseName,
			ndbtbl::mode__ Mode,
			bso::bool__ EraseIndexes,
			type__ Type )
		{
			bso::bool__ Success = false;

			reset();

			Engine.Init( Location, BaseName, Mode, EraseIndexes, false );
			Structure.Init();

			BaseDirectoryName.Init();
			mbdbsc::BuildBaseDirectoryName( Location, BaseName, BaseDirectoryName );

			return _SubInit( Type );
		}
		void SetNameAndComment(
			const str::string_ &Name,
			const str::string_ &Comment )
		{
			Structure.Name = Name;
			Structure.Comment = Comment;

			_ExportStructure();
		}
		table_row__ SearchTable( const str::string_ &Name ) const
		{
			return Structure.SearchTable( Name );
		}
		table_row__ AddTable( const mbdtbl::table_description_ &TableDescription )
		{
			table_row__ TableRow = Structure.AddTable( TableDescription );

			_ExportStructure();

			return TableRow;
		}
		void ModifyTable(
			table_row__ TableRow,
			const mbdtbl::table_description_ &TableDescription )
		{
			Structure.ModifyTable( TableRow, TableDescription );

			_ExportStructure();
		}
		void DeleteTable( table_row__ TableRow );
		field_row__ AddField(
			table_row__ TableRow,
			const field_description_ &FieldDescription )
		{
			field_row__ FieldRow = Structure.AddField( TableRow, FieldDescription );

			_ExportStructure();

			return FieldRow;
		}
		void ModifyField(
			field_row__ FieldRow,
			const field_description_ &FieldDescription )
		{
			Structure.ModifyField( FieldRow, FieldDescription );

			_ExportStructure();
		}
		void DeleteField( field_row__ FieldRow );
		field_row__ SearchField(
			table_row__ TableRow,
			const str::string_ &Name ) const
		{
			return Structure.SearchField( TableRow, Name );
		}
		epeios::row__ TestExistence( const field_rows_ &FieldRows ) const;	// Si != 'NONE', alors la valeur retournée fournit la position d'un 'FieldRow' inexistant.
		bso::bool__ Exist( const field_rows_ &FieldRows ) const
		{
			return ( TestExistence( FieldRows ) == NONE );
		}
		record_id__ GetLastRecordId( void ) const
		{
			ndbtbl::rrow__ Row = Engine.TableRecordFieldIndex.Last();

			if ( Row == NONE )
				return MBDBSC_UNDEFINED_RECORD_ID;
			else
				return _GetRecordId( Row );
		}
		record_id__ AddRecord(
			const data_ &Data,
			table_row__ TableRow,
			const field_rows_ &FieldRows )
		{
			record_id__ RecordId = *GetLastRecordId() + 1;

			if ( _AddOrModifyRecord( RecordId, Data, TableRow, FieldRows ) )
				ERRc();

			return RecordId;
		}
		bso::bool__ ModifyRecord(
			record_id__ RecordId,
			const data_ &Data,
			table_row__ TableRow,
			const field_rows_ &FieldRows )
		{
			return _AddOrModifyRecord( RecordId, Data, TableRow, FieldRows );
		}
		bso::bool__ RecordExists(
			record_id__ RecordId,
			table_row__ TableRow ) const;
		void DeleteRecord(
			record_id__ RecordId,
			table_row__ TableRow );
		void GetRecords(
			table_row__ TableRow,
			record_ids_ &RecordIds ) const;
		void GetRecords(
			table_row__ TableRow,
			field_row__ SortFieldRow,
			record_ids_ &RecordIds ) const;
		void GetRecordData(
			table_row__ TableRow,
			const field_rows_ &FieldRows,
			record_id__ RecordId,
			data_ &Data ) const;
		void GetRecordsData(
			table_row__ TableRow,
			const field_rows_ &FieldRows,
			const record_ids_ &RecordIds,
			data_cluster_ &DataCluster ) const;
		void DeleteRecord( record_id__ RecordId );
		bso::bool__ TableExists( table_row__ TableRow ) const
		{
			return Structure.TableExists( TableRow );
		}
		bso::bool__ FieldExists( field_row__ FieldRow ) const
		{
			return Structure.FieldExists( FieldRow );
		}
		bso::bool__ FieldsExist( const field_rows_ &FieldRows ) const
		{
			return Structure.FieldsExist( FieldRows );
		}
		bso::bool__ IsFieldOwnedByTable(
			field_row__ FieldRow,
			table_row__ TableRow ) const
		{
			return Structure.IsFieldOwnedByTable( FieldRow, TableRow );
		}
		bso::bool__ AreFieldsOwnedByTable(
			const field_rows_ &FieldRows,
			table_row__ TableRow ) const
		{
			return Structure.AreFieldsOwnedByTable( FieldRows, TableRow );
		}
	};

	epeios::row__ TestUnicity( const field_rows_ &FieldRows );
	inline bso::bool__ AreUnique( const field_rows_ &FieldRows )
	{
		return ( TestUnicity( FieldRows ) == NONE );
	}

	typedef ctn::E_XMCONTAINER_( str::string_ ) strings_;
	E_AUTO( strings );

	const strings_ &GetDatabases(
		const char *Dir,
		strings_ &Databases );
}


#endif
