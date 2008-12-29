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

#ifndef MDBMNG__INC
#define MDBMNG__INC

#ifdef XXX_DBG
#	define MBDMNG__DBG
#endif

#include "mbdeng.h"
#include "mbdstr.h"

namespace mbdmng {

	using namespace mbdeng;
	using namespace mbdstr;
	using namespace mbdbsc;

	class manager_
	{
	private:
		const record_ &GetRecord_(
			dbstbl::rrow__ Row,
			record_ &Record ) const;
		record_id__ GetRecordId_( dbstbl::rrow__ Row ) const;
		const datum_ GetDatum_(
			dbstbl::rrow__ Row,
			datum_ &Datum ) const;
	public:
		struct s {
			mbdeng::table_::s Table;
			mbdstr::structure_::s Structure;
		};
		mbdeng::table_ Table;
		mbdstr::structure_ Structure;
		manager_( s &S )
		: Table( S.Table ),
		  Structure( S.Structure )
		{}
		void reset( bso::bool__ P = true )
		{
			Table.reset( P );
			Structure.reset( P );
		}
		void plug( mmm::E_MULTIMEMORY_ &MM )
		{
			Table.plug( MM );
			Structure.plug( MM );
		}
		manager_ &operator =( const manager_ &M )
		{
			Table = M.Table;
			Structure = M.Structure;

			return *this;
		}
		void Init(
			const str::string_ &Location,
			dbstbl::mode__ Mode,
			bso::bool__ Erase,
			bso::bool__ Partial )
		{
			reset();

			Table.Init( Location, Mode, Erase, Partial );
			Structure.Init();
		}
		field_row__ AddField(
			const field_description_ &FieldDescription,
			field_row__ FieldRow )
		{
			return Structure.AddField( FieldDescription, FieldRow );
		}
		epeios::row__ TestExistence( const field_rows_ &FieldRows ) const;	// Si != 'NONE', alors la valeur retournée fournit la position d'un 'FieldRow' inexistant.
		bso::bool__ Exist( const field_rows_ &FieldRows ) const
		{
			return ( TestExistence( FieldRows ) == NONE );
		}
		record_id__ GetLastRecordId( void ) const
		{
			dbstbl::rrow__ Row = Table.Last( Table.GetRecordIdFieldRowIndexRow() );

			if ( Row == NONE )
				return MBDBSC_UNDEFINED_RECORD_ID;
			else
				return GetRecordId_( Row );
		}
		record_id__ AddRecord(
			const data_ &Data,
			const field_rows_ &FieldRows );
		void GetRecord(
			record_id__ RecordId,
			const field_rows_ &FieldRows,
			data_ &Data ) const;
		void DeleteRecord( record_id__ RecordId );
	};

	epeios::row__ TestUnicity( const field_rows_ &FieldRows );
	inline bso::bool__ AreUnique( const field_rows_ &FieldRows )
	{
		return ( TestUnicity( FieldRows ) == NONE );
	}
}


#endif