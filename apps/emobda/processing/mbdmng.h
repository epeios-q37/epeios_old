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
			dbstbl::rrow__ Row,
			record_ &Record ) const;
		record_id__ _GetRecordId( dbstbl::rrow__ Row ) const;
		const datum_ _GetDatum(
			dbstbl::rrow__ Row,
			datum_ &Datum ) const;
		bso::bool__ _ExportStructure( void );
		bso::bool__ _ImportStructure( void );
		bso::bool__ _Create( void );
		bso::bool__ _Retrieve( void );
		bso::bool__ _SubInit( type__ Type );
	public:
		struct s {
			mbdeng::engine_::s Engine;
			mbdstr::structure_::s Structure;
			str::string_::s Location;
		};
		mbdeng::engine_ Engine;
		mbdstr::structure_ Structure;
		str::string_ Location;
		manager_( s &S )
		: Engine( S.Engine ),
		  Structure( S.Structure ),
		  Location( S.Location )
		{}
		void reset( bso::bool__ P = true )
		{
			Engine.reset( P );
			Structure.reset( P );
			Location.reset( P );
		}
		void plug( mmm::E_MULTIMEMORY_ &MM )
		{
			Engine.plug( MM );
			Structure.plug( MM );
			Location.plug( MM );
		}
		manager_ &operator =( const manager_ &M )
		{
			Engine = M.Engine;
			Structure = M.Structure;
			Location = M.Location;

			return *this;
		}
		bso::bool__ Init(
			const str::string_ &Location,
			dbstbl::mode__ Mode,
			bso::bool__ EraseIndexes,
			type__ Type )
		{
			bso::bool__ Success = false;

			reset();

			Engine.Init( Location, Mode, EraseIndexes, false );
			Structure.Init();
			this->Location.Init( Location );

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
			return Structure.AddTable( TableDescription );
		}
		field_row__ AddField(
			table_row__ TableRow,
			const field_description_ &FieldDescription )
		{
			field_row__ FieldRow = Structure.AddField( TableRow, FieldDescription );

			_ExportStructure();

			return FieldRow;
		}
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
			dbstbl::rrow__ Row = Engine.Last( Engine.GetTableRecordFieldIndexRow() );

			if ( Row == NONE )
				return MBDBSC_UNDEFINED_RECORD_ID;
			else
				return _GetRecordId( Row );
		}
		record_id__ AddRecord(
			const data_ &Data,
			const field_rows_ &FieldRows );
		void GetRecord(
			record_id__ RecordId,
			const field_rows_ &FieldRows,
			data_ &Data ) const;
		void DeleteRecord( record_id__ RecordId );
		bso::bool__ TableExists( table_row__ TableRow ) const
		{
			return Structure.Tables.Exists( TableRow );
		}

	};

	epeios::row__ TestUnicity( const field_rows_ &FieldRows );
	inline bso::bool__ AreUnique( const field_rows_ &FieldRows )
	{
		return ( TestUnicity( FieldRows ) == NONE );
	}
}


#endif