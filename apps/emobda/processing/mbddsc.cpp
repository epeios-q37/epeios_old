/*
	'mdbdsc.cpp' by Claude SIMON (http://zeusw.org/).

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

#include "mbddsc.h"

using namespace mbddsc;

using xml::writer_;

static inline void DescriptionHeader_( writer_ &Writer )
{
	Writer.PushTag( MBDDSC_DESCRIPTION_TAG_NAME );
	Writer.PutAttribute( MBDDSC_NAMESPACE_ATTRIBUTE_NAME, MBDDSC_NAMESPACE_URI );
	Writer.PutAttribute( MBDDSC_VERSION_ATTRIBUTE_NAME, GetLabel( vDefault ) );
}

static inline void DescriptionFooter_( writer_ &Writer )
{
	Writer.PopTag();
}

static inline const char *GetDefaultLabel( item__ Item )
{
	return GetLabel( Item, vDefault );
}

#define DL( item )	GetDefaultLabel( i##item )

static void Export_(
	const field_ &Field,
	field_row__ FieldRow,
	writer_ &Writer )
{
	bso::integer_buffer__ Buffer;

	Writer.PushTag( DL( FieldTagName ) );
	Writer.PutAttribute( DL( FieldIdAttributeName ), bso::Convert( *FieldRow, Buffer ) );
	Writer.PushTag( DL( FieldNameTagName ) );
	Writer.PutValue( Field.Name );
	Writer.PopTag();
	Writer.PopTag();
}

static void Export_(
	const fields_ &Fields,
	writer_ &Writer )
{
	bso::integer_buffer__ Buffer;
	ctn::E_CITEMt( field_, field_row__ ) Field;
	field_row__ FieldRow = Fields.First();

	Field.Init( Fields );

	Writer.PushTag( DL( FieldsTagName ) );
	Writer.PutAttribute( DL( FieldsAmountAttributeName ), bso::Convert( Fields.Amount(), Buffer ) );

	while ( FieldRow != NONE ) {
		Export_( Field( FieldRow ), FieldRow, Writer );

		FieldRow = Fields.Next( FieldRow );
	}

	Writer.PopTag();
}

void mbddsc::Export(
	const mbdstr::structure_ &Structure,
	writer_ &Writer,
	bso::bool__ WithInternals,
	bso::bool__ AsOrphan )
{
	if ( AsOrphan ) {
		DescriptionHeader_( Writer );
	}

	Writer.PushTag( DL( StructureTagName ) );

	if ( WithInternals ) {
		Writer.PushTag( DL( EngineTagName ) );
		Writer.PutAttribute( DL( EngineVersionAttributeName ), MBDDSC_ENGINE_VERSION );
		Writer.PopTag();
	}

	Export_( Structure.Fields, Writer );

	if ( AsOrphan ) {
		DescriptionFooter_( Writer );
	}
}

