/*
	'mdbbsc.h' by Claude SIMON (http://zeusw.org/).

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

// eMoBDa BaSiCs

#ifndef MBDBSC__INC
#define MBDBSC__INC

#include "dbsbsc.h"
#include "mmm.h"
#include "mdr.h"

#define MBDBSC_STRUCTURE_FILE_EXTENSION				"embd"
#define MBDBSC_CONTENT_ROOT_FILE_NAME				"Data"
#define MBDBSC_TABLE_RECORD_FIELD_INDEX_ROOT_FILE_NAME	"TableRecordFieldIndex"
#define MBDBSC_TABLE_FIELD_DATUM_INDEX_ROOT_FILE_NAME		"TableFieldDatumIndex"

namespace mbdbsc {

	const str::string_ &BuildLocatedContentRootFileName(
		const str::string_ &Location,
		str::string_ &LocalizedRootFileName );

	const str::string_ &BuildLocatedTableRecordFieldIndexRootFileName(
		const str::string_ &Location,
		str::string_ &LocalizedRootFileName );

	const str::string_ &BuildLocatedTableFieldDatumIndexRootFileName(
		const str::string_ &Location,
		str::string_ &LocalizedRootFileName );


	typedef bso::ushort__ record_id_t__;
	E_TYPEDEF( record_id_t__, record_id__ );
#define MBDBSC_UNDEFINED_RECORD_ID	((record_id__)BSO_USHORT_MAX)
	typedef bch::E_BUNCH_( record_id__ ) record_ids_;
	E_AUTO( record_ids );

	typedef dbsbsc::rrow__ record_row__;

	E_ROW( field_row__ );

	typedef bch::E_BUNCH_( field_row__ ) field_rows_;
	E_AUTO( field_rows );

	E_ROW( table_row__ );

	typedef bch::E_BUNCH_( table_row__ ) table_rows_;
	E_AUTO( table_rows );

	typedef str::string_ datum_;
	typedef str::string datum;

	typedef ctn::E_XMCONTAINER_( datum_ ) data_;
	E_AUTO( data );

	typedef ctn::E_XCONTAINER_( data_ ) data_cluster_;
	E_AUTO( data_cluster );

	typedef dbsbsc::datum_	raw_datum_;
	typedef dbsbsc::datum	raw_datum;

	typedef bso::ubyte__ id_t__;

	typedef id_t__ table_id_t__;
	E_TYPEDEF( table_id_t__, table_id__ );
#define MBDBSC_UNDEFINED_TABLE_ID	((table_id__)BSO_UBYTE_MAX)

	typedef id_t__ field_id_t__;
	E_TYPEDEF( field_id_t__, field_id__ );
#define MBDBSC_UNDEFINED_FIELD_ID	((field_id__)BSO_UBYTE_MAX)

#pragma pack(push,1)
	class record_static_part__
	{
	public:
		table_id__ TableId;
		field_id__ FieldId;
		record_id__ RecordId;
	};
#pragma pack(pop)
	inline bso::sign__ TableCompare(
		const record_static_part__ &O1,
		const record_static_part__ &O2 )
	{
		return bso::Compare( *O1.TableId, *O2.TableId );
	}

	inline bso::sign__ FieldCompare(
		const record_static_part__ &O1,
		const record_static_part__ &O2 )
	{
		return bso::Compare( *O1.FieldId, *O2.FieldId );
	}

	inline bso::sign__ RecordCompare(
		const record_static_part__ &O1,
		const record_static_part__ &O2 )
	{
		return bso::Compare( *O1.RecordId, *O2.RecordId );
	}

	inline bso::sign__ TableRecordFieldCompare(
		const record_static_part__ &O1,
		const record_static_part__ &O2,
		dbsbsc::skip_level__ SkipLevel )
	{
		bso::sign__ Result = TableCompare( O1, O2 );

		if ( Result == 0 ) {
			if ( SkipLevel < 2 ) {
				Result = RecordCompare( O1, O2 );

				if ( Result == 0 )
					if ( SkipLevel < 1 )
						Result = FieldCompare( O1, O2 );
			}
		}

		return Result;
	}

	inline void ExtractRecordStaticPart(
		const raw_datum_ &RawDatum,
		record_static_part__ &RecordStaticPart )
	{
		if ( RawDatum.Amount() < sizeof( RecordStaticPart ) )
			ERRu();

		RawDatum.Get( RawDatum.First(), sizeof( record_static_part__ ), (dbsbsc::atom__ *)&RecordStaticPart );
	}

	inline record_static_part__ ExtractRecordStaticPart( const raw_datum_ &Datum )
	{
		record_static_part__ RecordStaticPart;

		ExtractRecordStaticPart( Datum, RecordStaticPart );

		return RecordStaticPart;
	}

	inline bso::sign__ TableCompare(
		const raw_datum_ &D1,
		const raw_datum_ &D2 )
	{
		return TableCompare( ExtractRecordStaticPart( D1 ),  ExtractRecordStaticPart( D2 ) ); 
	}

	inline bso::sign__ FieldCompare(
		const raw_datum_ &D1,
		const raw_datum_ &D2 )
	{
		return FieldCompare( ExtractRecordStaticPart( D1 ),  ExtractRecordStaticPart( D2 ) ); 
	}

	inline bso::sign__ TableRecordFieldCompare(
		const raw_datum_ &D1,
		const raw_datum_ &D2,
		dbsbsc::skip_level__ SkipLevel )
	{
		return TableRecordFieldCompare( ExtractRecordStaticPart( D1 ),  ExtractRecordStaticPart( D2 ), SkipLevel );
	}

	bso::sign__ Compare(
		const raw_datum_ &D1,
		epeios::row__ O1,
		const raw_datum_ &D2,
		epeios::row__ O2 );

	class record_ {
	public:
		struct s
		: public record_static_part__
		{
			datum_::s Datum;
		} &S_;
		datum_ Datum;
		record_( s &S )
		: S_( S ),
		  Datum( S.Datum )
		{}
		void reset( bso::bool__ P = true )
		{
			Datum.reset( P );

			S_.TableId = MBDBSC_UNDEFINED_TABLE_ID;
			S_.FieldId = MBDBSC_UNDEFINED_FIELD_ID;
			S_.RecordId = MBDBSC_UNDEFINED_RECORD_ID;
		}
		void plug( mdr::E_MEMORY_DRIVER__ &MD )
		{
			Datum.plug( MD );
		}
		void plug( mmm::E_MULTIMEMORY_ &MM )
		{
			Datum.plug( MM );
		}
		record_ &operator=( const record_ &R )
		{
			Datum = R.Datum;

			S_.TableId = R.S_.TableId;
			S_.FieldId = R.S_.FieldId;
			S_.RecordId = R.S_.RecordId;

			return *this;
		}
		void Init( void )
		{
			reset();

			Datum.Init();
		}
		void Init(
			const record_static_part__ &RecordStaticPart,
			const datum_ &Datum )
		{
			Init();

			this->Datum = Datum;

			S_.TableId = RecordStaticPart.TableId;
			S_.FieldId = RecordStaticPart.FieldId;
			S_.RecordId = RecordStaticPart.RecordId;
		}
		void Init(
			table_id__ TableId,
			field_id__ FieldId,
			record_id__ RecordId,
			const datum_ &Datum )
		{
			record_static_part__ RecordStaticPart = { TableId, FieldId, RecordId };

			Init( RecordStaticPart, Datum );
		}
		const record_static_part__ &GetStaticPart( void ) const
		{
			return S_;
		}
		record_static_part__ &GetStaticPart( void )
		{
			return S_;
		}
	};

	E_AUTO( record );

	inline bso::sign__ TableFieldDatumCompare(
		const raw_datum_ &D1,
		const raw_datum_ &D2,
		dbsbsc::skip_level__ SkipLevel )
	{
		bso::sign__ Result = TableCompare( D1, D2 );

		if ( Result == 0 ) {
			if ( SkipLevel < 2 ) {
				Result = FieldCompare( D1, D2 );

				if ( Result == 0 )
					if ( SkipLevel < 1 )
						Result = Compare( D1, D1.First( sizeof( record_static_part__ ) ), D2, D2.First( sizeof( record_static_part__ ) ) );
			}
		}

		return Result;
	}

	inline void Extract(
		const raw_datum_ &RawDatum,
		record_ &Record )
	{
		ExtractRecordStaticPart( RawDatum, Record.GetStaticPart() );

		Record.Datum.Append( RawDatum, RawDatum.First( sizeof( record_static_part__ ) ) );
	}

	inline void Convert(
		const record_ &Record,
		raw_datum_ &RawDatum )
	{
		const record_static_part__ &RecordStaticPart = Record.GetStaticPart();

		RawDatum.Append( (const dbsbsc::atom__ *)&RecordStaticPart, sizeof( RecordStaticPart ) );

		RawDatum.Append( Record.Datum );
	}
}

#endif