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

#ifndef MDBBSC__INC
#define MDBBSC__INC

#include "dbsbsc.h"
#include "mmm.h"
#include "mdr.h"

#define MBDBSC_CONTENT_ROOT_FILE_NAME				"Data"
#define MBDBSC_RECORD_FIELD_INDEX_ROOT_FILE_NAME	"RecordFieldIndex"
#define MBDBSC_FIELD_DATUM_INDEX_ROOT_FILE_NAME		"FieldDatum"

namespace mbdbsc {

	const str::string_ &BuildLocatedContentRootFileName(
		const str::string_ &Location,
		str::string_ &LocalizedRootFileName );

	const str::string_ &BuildLocatedRecordFieldIndexRootFileName(
		const str::string_ &Location,
		str::string_ &LocalizedRootFileName );

	const str::string_ &BuildLocatedFieldDatumIndexRootFileName(
		const str::string_ &Location,
		str::string_ &LocalizedRootFileName );

	E_ROW( record_row__ );
	E_ROW( field_row__ );

	typedef str::string_ datum_;
	typedef str::string datum;

	typedef dbsbsc::datum_	raw_datum_;
	typedef dbsbsc::datum	raw_datum;

	class record_static_part__
	{
	public:
		record_row__ RecordRow;
		field_row__ FieldRow;
	};

	inline bso::sign__ RecordRowCompare(
		const record_static_part__ &O1,
		const record_static_part__ &O2 )
	{
		return bso::Compare( *O1.RecordRow, *O2.RecordRow );
	}

	inline bso::sign__ FieldRowCompare(
		const record_static_part__ &O1,
		const record_static_part__ &O2 )
	{
		return bso::Compare( *O1.FieldRow, *O2.FieldRow );
	}

	inline bso::sign__ RecordRowFieldRowCompare(
		const record_static_part__ &O1,
		const record_static_part__ &O2 )
	{
		bso::sign__ Result = RecordRowCompare( O1, O2 );

		if ( Result == 0 )
			Result = FieldRowCompare( O1, O2 );

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

	inline bso::sign__ FieldRowCompare(
		const raw_datum_ &D1,
		const raw_datum_ &D2 )
	{
		return FieldRowCompare( ExtractRecordStaticPart( D1 ),  ExtractRecordStaticPart( D2 ) );
	}

	inline bso::sign__ RecordRowFieldRowCompare(
		const raw_datum_ &D1,
		const raw_datum_ &D2 )
	{
		return RecordRowFieldRowCompare( ExtractRecordStaticPart( D1 ),  ExtractRecordStaticPart( D2 ) );
	}

	bso::sign__ Compare(
		const raw_datum_ &D1,
		epeios::row__ O1,
		const raw_datum_ &D2,
		epeios::row__ &O2 );

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

			S_.RecordRow = NONE;
			S_.FieldRow = NONE;
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

			S_.RecordRow = R.S_.RecordRow;
			S_.FieldRow = R.S_.FieldRow;

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

			S_.FieldRow = RecordStaticPart.FieldRow;
			S_.RecordRow = RecordStaticPart.RecordRow;
		}
		const record_static_part__ GetStaticPart( void ) const
		{
			return S_;
		}
	};

	inline bso::sign__ FieldRowDatumCompare(
		const raw_datum_ &D1,
		const raw_datum_ &D2 )
	{
		bso::sign__ Result = FieldRowCompare( D1, D2 );

		if ( Result == 0 )
			Result = Compare( D1, D1.First( sizeof( record_static_part__ ) ), D2, D2.First( sizeof( record_static_part__ ) ) );

		return Result;
	}


	void Extract(
		const raw_datum_ &RawDatum,
		record_ &Record );

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