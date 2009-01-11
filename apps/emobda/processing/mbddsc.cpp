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
using namespace xml;

using xml::writer_;

static inline const char *GetDefaultLabel_( item__ Item )
{
	return GetLabel( Item, vDefault );
}

#define DL( item )	GetDefaultLabel_( i##item )

static inline void DescriptionHeader_( writer_ &Writer )
{
	Writer.PushTag( MBDDSC_DESCRIPTION_TAG );
	Writer.PutAttribute( MBDDSC_NAMESPACE_ATTRIBUTE, MBDDSC_NAMESPACE_URI );
	Writer.PutAttribute( MBDDSC_VERSION_ATTRIBUTE, GetLabel( vDefault ) );

	Writer.PushTag( DL( MiscellaneousTag ) );
	{
			Writer.PushTag( DL( EngineTag ) );
			{
				Writer.PutAttribute( DL( EngineVersionAttribute ), MBDDSC_ENGINE_VERSION );
			}

			Writer.PopTag();
	}
	Writer.PopTag();
}

static inline void DescriptionFooter_( writer_ &Writer )
{
	Writer.PopTag();
}

static void ExportField_( 
	const field_ &Field,
	writer_ &Writer )
{
	bso::integer_buffer__ Buffer;

	Writer.PushTag( DL( FieldTag ) );
	{
		Writer.PutAttribute( DL( FieldIdAttribute ), bso::Convert( *Field.GetFieldId(), Buffer ) );

		Writer.PushTag( DL( FieldNameTag ) );
		{
			Writer.PutValue( Field.Name );
		}
		Writer.PopTag();
	}
	Writer.PopTag();

}

static void ExportFields_(
	const field_rows_ &FieldRows,
	const fields_ &Fields,
	writer_ &Writer )
{
	bso::integer_buffer__ Buffer;
	ctn::E_CMITEMt( field_, field_row__ ) Field;
	epeios::row__ Row = FieldRows.First();

	Field.Init( Fields );

	Writer.PushTag( DL( FieldsTag ) );
	{
		Writer.PutAttribute( DL( FieldsAmountAttribute ), bso::Convert( FieldRows.Amount(), Buffer ) );

		while ( Row != NONE ) {
			ExportField_( Field( FieldRows( Row ) ), Writer );

			Row = FieldRows.Next( Row );
		}
	}
	Writer.PopTag();
}

static void ExportTable_(
	const table_ &Table,
	const fields_ &Fields,
	writer_ &Writer )
{
	bso::integer_buffer__ Buffer;

	Writer.PushTag( DL( TableTag ) );
	{
		Writer.PutAttribute( DL( TableIdAttribute ), bso::Convert( *Table.GetTableId(), Buffer ) );

		Writer.PushTag( DL( TableNameTag ) );
		{
			Writer.PutValue( Table.Name );
		}
		Writer.PopTag();

		ExportFields_( Table.Fields, Fields, Writer );
	}
	Writer.PopTag();
}

static void ExportTables_(
	const tables_ &Tables,
	const fields_ &Fields,
	writer_ &Writer )
{
	bso::integer_buffer__ Buffer;
	ctn::E_CITEMt( table_ , table_row__ ) Table;
	table_row__ Row = Tables.First();

	Table.Init( Tables );

	Writer.PushTag( DL( TablesTag ) );
	{
		Writer.PutAttribute( DL( TablesAmountAttribute ), bso::Convert( Tables.Amount(), Buffer ) );

		while ( Row != NONE ) {
			ExportTable_( Table( Row ), Fields, Writer );

			Row = Tables.Next( Row );
		}
	}
	Writer.PopTag();
}

void mbddsc::Export(
	const mbdstr::structure_ &Structure,
	writer_ &Writer,
	bso::bool__ AsOrphan )
{
	if ( AsOrphan )
		DescriptionHeader_( Writer );

	{
		Writer.PushTag( DL( StructureTag ) );

		Writer.PushTag( DL( StructureNameTag ) );
		{
			Writer.PutValue( Structure.Name );
		}
		Writer.PopTag();

		ExportTables_( Structure.Tables, Structure.Fields, Writer );
	}

	if ( AsOrphan )
		DescriptionFooter_( Writer );
}

static version__ GetVersion_( const str::string_ &Version )
{
	int I = v_amount;

	while ( ( I-- ) && ( str::string( GetLabel( (version__)I ) ) != Version ) );

	if ( I < 0 )
		I = v_Undefined;

	return (version__)I;
}

#define L( name )	GetLabel( i##name, Version )

static bso::bool__ ImportFieldName_(
	xml::ehanced_parser___ &Parser,
	str::string_ &FieldName,
	version__ Version )
{
	bso::bool__ Continue = true;
ERRProlog
	xml::ehanced_parser___ Parser;
	bso::bool__ Continue = true;
	str::string Name, Value;
	xml::dump Dump;
	xml::status__ Status = s_Undefined;
	bso::bool__ Handled = false;
ERRBegin

	Name.Init();
	Value.Init();
	Dump.Init();

	while ( !Handled ) {
		switch ( Parser.Parse( Name, Value, Dump, Status ) ) {
		case cProcessingIntruction:
			Continue = false;
			ERRReturn;
			break;
		case cStartTag:
			Continue = false;
			ERRReturn;
			break;
		case cStartTagClosed:
			break;
		case cAttribute:
			Continue = false;
			ERRReturn;
			break;
		case cValue:
			FieldName = Name;
			break;
		case cEndTag:
			Handled = true;
			break;
		case cProcessed:
			ERRc();
			break;
		case cError:
			Continue = false;
			ERRReturn;
			break;
		default:
			ERRc();
			break;
		}
	}

ERRErr
ERREnd
ERREpilog
	return Continue;
}

static inline bso::bool__ GetFieldRow_(
	const str::string_ &Value,
	field_row__ &FieldRow )
{
	epeios::row__ Error = NONE;

	FieldRow = Value.ToUL( &Error );

	return Error == NONE;
}


static bso::bool__ ImportField_(
	xml::ehanced_parser___ &Parser,
	mbddsc::field_description_ &Field,
	version__ Version )
{
	bso::bool__ Continue = true;
ERRProlog
	xml::ehanced_parser___ Parser;
	bso::bool__ Continue = true;
	str::string Name, Value;
	xml::dump Dump;
	xml::status__ Status = s_Undefined;
	bso::bool__ Handled = false;
ERRBegin

	Name.Init();
	Value.Init();
	Dump.Init();

	while ( !Handled ) {
		switch ( Parser.Parse( Name, Value, Dump, Status ) ) {
		case cProcessingIntruction:
			Continue = false;
			ERRReturn;
			break;
		case cStartTag:
			if ( Name == str::string( L( FieldNameTag ) ) )
				Continue = ImportFieldName_( Parser, Field.Name, Version );
			else {
				Continue = false;
				ERRReturn;
			}
			break;
		case cStartTagClosed:
			break;
		case cAttribute:
			if ( Name == L( FieldIdAttribute ) )
				Continue = GetFieldRow_( Value, Field.FieldRow() );
			else {
				Continue = false;
				ERRReturn;
			}
			break;
		case cValue:
			Continue = false;
			ERRReturn;
			break;
		case cEndTag:
			break;
		case cProcessed:
			ERRc();
			break;
		case cError:
			Continue = false;
			ERRReturn;
			break;
		default:
			ERRc();
			break;
		}
	}

ERRErr
ERREnd
ERREpilog
	return Continue;
}

static bso::bool__ ImportFields_(
	xml::ehanced_parser___ &Parser,
	mbddsc::field_descriptions_ &Fields,
	version__ Version )
{
	bso::bool__ Continue = true;
ERRProlog
	xml::ehanced_parser___ Parser;
	bso::bool__ Continue = true;
	str::string Name, Value;
	xml::dump Dump;
	xml::status__ Status = s_Undefined;
	bso::bool__ Handled = false;
	mbddsc::field_description Field;
ERRBegin

	Name.Init();
	Value.Init();
	Dump.Init();

	while ( !Handled ) {
		switch ( Parser.Parse( Name, Value, Dump, Status ) ) {
		case cProcessingIntruction:
			Continue = false;
			ERRReturn;
			break;
		case cStartTag:
			if ( Name == str::string( L( FieldTag ) ) ) {
				Field.Init();
				Continue = ImportField_( Parser, Field, Version );
				if ( Continue )
					Fields.Append( Field );
			} else {
				Continue = false;
				ERRReturn;
			}
			break;
		case cStartTagClosed:
			break;
		case cAttribute:
			Continue = false;
			ERRReturn;
			break;
		case cValue:
			Continue = false;
			ERRReturn;
			break;
		case cEndTag:
			Handled = true;
			break;
		case cProcessed:
			ERRc();
			break;
		case cError:
			Continue = false;
			break;
		default:
			ERRc();
			break;
		}
	}
ERRErr
ERREnd
ERREpilog
	return Continue;
}


static bso::bool__ ImportDescription_(
	xml::ehanced_parser___ &Parser,
	description_ &Description )
{
	bso::bool__ Continue = true;
ERRProlog
	xml::ehanced_parser___ Parser;
	bso::bool__ Continue = true;
	str::string Name, Value;
	xml::dump Dump;
	xml::status__ Status = s_Undefined;
	bso::bool__ Handled = false;
	version__ Version = v_Undefined;
ERRBegin

	Name.Init();
	Value.Init();
	Dump.Init();

	while ( !Handled ) {
		switch ( Parser.Parse( Name, Value, Dump, Status ) ) {
		case cProcessingIntruction:
			break;
		case cStartTag:
			if ( Name == str::string( L( FieldsTag ) ) ) {
				if ( Version = v_Undefined ) {
					Continue = false;
					ERRReturn;
				}

				Continue = ImportFields_( Parser, Description.Fields, Version );
			}
			break;
		case cStartTagClosed:
			break;
		case cAttribute:
			if ( Name == str::string( MBDDSC_VERSION_ATTRIBUTE ) )
				if ( Version != v_Undefined ) {
					Continue = false;
					ERRReturn;
				}
			break;
		case cValue:
			Continue = false;
			ERRReturn;
			break;
		case cEndTag:
			if ( Name == str::string( MBDDSC_DESCRIPTION_TAG ) )
				Handled = true;
			break;
		case cProcessed:
			ERRc();
			break;
		case cError:
			Continue = false;
			ERRReturn;
			break;
		default:
			ERRc();
			break;
		}
	}
ERRErr
ERREnd
ERREpilog
	return Continue;
}


void mbddsc::Import(
	xtf::extended_text_iflow__ &Flow,
	description_ &Description )
{
ERRProlog
	xml::ehanced_parser___ Parser;
	bso::bool__ Continue = true;
	str::string Name, Value;
	xml::dump Dump;
	xml::status__ Status = s_Undefined;
ERRBegin
	Parser.Init( Flow );

	Name.Init();
	Value.Init();
	Dump.Init();

	Parser.Start();

	while ( Continue ) {
		switch ( Parser.Parse( Name, Value, Dump, Status ) ) {
		case cProcessingIntruction:
			break;
		case cStartTag:
			if ( Name == str::string( MBDDSC_DESCRIPTION_TAG ) )
				Continue = ImportDescription_( Parser, Description );
			break;
		case cStartTagClosed:
			break;
		case cAttribute:
			break;
		case cValue:
			break;
		case cEndTag:
			break;
		case cProcessed:
			Continue = false;
			break;
		case cError:
			Continue = false;
			break;
		default:
			ERRc();
			break;
		}
	}

	
ERRErr
ERREnd
ERREpilog
}

#if 0
	while ( !Handled ) {
		switch ( Parser.Parse( Name, Value, Dump, Status ) ) {
		case cProcessingIntruction:
			break;
		case cStartTag:
			break;
		case cStartTagClosed:
			break;
		case cAttribute:
			break;
		case cValue:
			break;
		case cEndTag:
			break;
		case cProcessed:
			break;
		case cError:
			break;
		default:
			ERRc();
			break;
		}
	}
#endif


