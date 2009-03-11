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
		Writer.PutAttribute( DL( FieldIdAttribute ), bso::Convert( *Field.GetId(), Buffer ) );

		Writer.PushTag( DL( FieldNameTag ) );
		{
			Writer.PutValue( Field.Name );
		}
		Writer.PopTag();

		Writer.PushTag( DL( FieldCommentTag ) );
		{
			Writer.PutValue( Field.Comment );
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
	ctn::E_CITEMt( field_, field_row__ ) Field;
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
		Writer.PutAttribute( DL( TableIdAttribute ), bso::Convert( *Table.GetId(), Buffer ) );

		Writer.PushTag( DL( TableNameTag ) );
		{
			Writer.PutValue( Table.Name );
		}
		Writer.PopTag();

		Writer.PushTag( DL( TableCommentTag ) );
		{
			Writer.PutValue( Table.Comment );
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

		Writer.PushTag( DL( StructureCommentTag ) );
		{
			Writer.PutValue( Structure.Comment );
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

static void ImportValue_(
	xml::browser___ &Browser,
	str::string_ &Value,
	xml::status__ &Status )
{
ERRProlog
	str::string TagName, AttributeName, ValueBuffer;
	xml::dump Dump;
	bso::bool__ Continue = true;
ERRBegin

	TagName.Init();
	AttributeName.Init();
	Dump.Init();
	ValueBuffer.Init();

	while ( Continue ) {
		switch ( Browser.Browse( TagName, AttributeName, ValueBuffer, Dump, Status ) ) {
		case tProcessingInstruction:
			ERRI( iBeam );
			break;
		case tStartTag:
			ERRI( iBeam );
			break;
		case tStartTagClosed:
			break;
		case tAttribute:
			ERRI( iBeam );
			break;
		case tValue:
			Value = ValueBuffer;
			break;
		case tEndTag:
			Continue = false;
			break;
		case tProcessed:
			ERRc();
			break;
		case tError:
			ERRI( iBeam );
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
	
static inline void ImportName_(
	xml::browser___ &Browser,
	str::string_ &Name,
	xml::status__ &Status )
{
	ImportValue_( Browser, Name, Status );
}

static void ImprtComment_(
	xml::browser___ &Browser,
	str::string_ &Comment,
	xml::status__ &Status )
{
	ImportValue_( Browser, Comment, Status );
}

static void ImportFieldName_(
	xml::browser___ &Browser,
	str::string_ &Name,
	xml::status__ &Status )
{
	ImportValue_( Browser, Name, Status );
}

static void ImportFieldComment_(
	xml::browser___ &Browser,
	str::string_ &Comment,
	xml::status__ &Status )
{
	ImportValue_( Browser, Comment, Status );
}

static inline bso::bool__ GetId_(
	const str::string_ &Value,
	mbdbsc::id_t__ &Id )
{
	epeios::row__ Error = NONE;

	Id = Value.ToUB( &Error );

	return Error == NONE;
}

static inline bso::bool__ GetFieldId_(
	const str::string_ &Value,
	field_id__ &Id )
{
	return GetId_( Value, *Id );
}

static inline bso::bool__ GetTableId_(
	const str::string_ &Value,
	table_id__ &Id )
{
	return GetId_( Value, *Id );
}

static void ImportFieldDescription_(
	xml::browser___ &Browser,
	mbddsc::field_description_ &Field,
	version__ Version,
	xml::status__ &Status )
{
ERRProlog
	bso::bool__ Continue = true;
	str::string TagName, AttributeName, Value;
	xml::dump Dump;
ERRBegin

	TagName.Init();
	AttributeName.Init();
	Value.Init();
	Dump.Init();

	while ( Continue ) {
		switch ( Browser.Browse( TagName, AttributeName, Value, Dump, Status ) ) {
		case tProcessingInstruction:
			ERRI( iBeam );
			break;
		case tStartTag:
			if ( TagName == L( FieldNameTag ) )
				ImportFieldName_( Browser, Field.Name, Status );
			else if ( TagName == L( FieldCommentTag ) )
				ImportFieldComment_( Browser, Field.Comment, Status );
			else
				ERRI( iBeam );
			break;
		case tStartTagClosed:
			break;
		case tAttribute:
			if ( AttributeName == L( FieldIdAttribute ) )
				GetFieldId_( Value, Field.Id() );
			else
				ERRI( iBeam );
			break;
		case tValue:
			ERRI( iBeam );
			break;
		case tEndTag:
			Continue = false;
			break;
		case tProcessed:
			ERRc();
			break;
		case tError:
			ERRI( iBeam );
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

static void ImportFieldDescriptions_(
	xml::browser___ &Browser,
	mbddsc::field_descriptions_ &Fields,
	version__ Version,
	xml::status__ &Status)
{
ERRProlog
	bso::bool__ Continue = true;
	str::string TagName, AttributeName, Value;
	xml::dump Dump;
	mbddsc::field_description Field;
ERRBegin

	TagName.Init();
	AttributeName.Init();
	Value.Init();
	Dump.Init();

	while ( Continue ) {
		switch ( Browser.Browse( TagName, AttributeName, Value, Dump, Status ) ) {
		case tProcessingInstruction:
			ERRI( iBeam );
			break;
		case tStartTag:
			if ( TagName == str::string( L( FieldTag ) ) ) {
				Field.Init();
				ImportFieldDescription_( Browser, Field, Version, Status );
				Fields.Append( Field );
			} else
				ERRI( iBeam );
			break;
		case tStartTagClosed:
			break;
		case tAttribute:
			break;
		case tValue:
			ERRI( iBeam );
			break;
		case tEndTag:
			Continue = false;
			break;
		case tProcessed:
			ERRc();
			break;
		case tError:
			ERRI( iBeam );
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

static void ImportTableName_(
	xml::browser___ &Browser,
	str::string_ &Name,
	xml::status__ &Status )
{
	ImportValue_( Browser, Name, Status );
}

static void ImportTableComment_(
	xml::browser___ &Browser,
	str::string_ &Comment,
	xml::status__ &Status )
{
	ImportValue_( Browser, Comment, Status );
}

static void ImportTableDescription_(
	xml::browser___ &Browser,
	mbddsc::table_description_ &Table,
	version__ Version,
	xml::status__ &Status )
{
ERRProlog
	bso::bool__ Continue = true;
	str::string TagName, AttributeName, Value;
	xml::dump Dump;
ERRBegin

	TagName.Init();
	AttributeName.Init();
	Value.Init();
	Dump.Init();

	while ( Continue ) {
		switch ( Browser.Browse( TagName, AttributeName, Value, Dump, Status ) ) {
		case tProcessingInstruction:
			ERRI( iBeam );
			break;
		case tStartTag:
			if ( TagName == L( TableNameTag ) )
				ImportTableName_( Browser, Table.Name, Status );
			else if ( TagName == L( TableCommentTag ) )
				ImportTableComment_( Browser, Table.Comment, Status );
			else if ( TagName == L( FieldsTag ) )
				ImportFieldDescriptions_( Browser, Table.Fields, Version, Status );
			else
				ERRI( iBeam );
			break;
		case tStartTagClosed:
			break;
		case tAttribute:
			if ( AttributeName == L( TableIdAttribute ) )
				GetTableId_( Value, Table.Id() );
			else
				ERRI( iBeam );
			break;
		case tValue:
			ERRI( iBeam );
			break;
		case tEndTag:
			Continue = false;
			break;
		case tProcessed:
			ERRc();
			break;
		case tError:
			ERRI( iBeam );
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

static void ImportTableDescriptions_(
	xml::browser___ &Browser,
	mbddsc::table_descriptions_ &Tables,
	version__ Version,
	xml::status__ &Status )
{
ERRProlog
	bso::bool__ Continue = true;
	str::string TagName, AttributeName, Value;
	xml::dump Dump;
	mbddsc::table_description Table;
ERRBegin

	TagName.Init();
	AttributeName.Init();
	Value.Init();
	Dump.Init();

	while ( Continue ) {
		switch ( Browser.Browse( TagName, AttributeName, Value, Dump, Status ) ) {
		case tProcessingInstruction:
			ERRI( iBeam );
			break;
		case tStartTag:
			if ( TagName == str::string( L( TableTag ) ) ) {
				Table.Init();
				ImportTableDescription_( Browser, Table, Version, Status );
				Tables.Append( Table );
			} else
				ERRI( iBeam );
			break;
		case tStartTagClosed:
			break;
		case tAttribute:
			break;
		case tValue:
			ERRI( iBeam );
			break;
		case tEndTag:
			Continue = false;
			break;
		case tProcessed:
			ERRc();
			break;
		case tError:
			ERRI( iBeam );
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


static void ImportStructureName_(
	xml::browser___ &Browser,
	str::string_ &Name,
	xml::status__ &Status )
{
	ImportValue_( Browser, Name, Status );
}

static void ImportStructureComment_(
	xml::browser___ &Browser,
	str::string_ &Comment,
	xml::status__ &Status )
{
	ImportValue_( Browser, Comment, Status );
}

static void ImportStructureDescription_(
	xml::browser___ &Browser,
	description_ &Description,
	version__ Version,
	xml::status__ &Status )
{
ERRProlog
	bso::bool__ Continue = true;
	str::string TagName, AttributeName, Value;
	xml::dump Dump;
ERRBegin
	TagName.Init();
	AttributeName.Init();
	Value.Init();
	Dump.Init();

	while ( Continue ) {
		switch ( Browser.Browse( TagName, AttributeName, Value, Dump, Status ) ) {
		case tProcessingInstruction:
			break;
		case tStartTag:
			if ( TagName == str::string( L( StructureNameTag ) ) )
				ImportStructureName_( Browser, Description.Name, Status );
			else if ( TagName == str::string( L( StructureCommentTag ) ) )
				ImportStructureComment_( Browser, Description.Comment, Status );
			else if ( TagName == str::string( L( TablesTag ) ) ) {
				if ( Version == v_Undefined )
					ERRI( iBeam );

				ImportTableDescriptions_( Browser, Description.Tables, Version, Status );
			}
			break;
		case tStartTagClosed:
			break;
		case tAttribute:
			if ( AttributeName == str::string( MBDDSC_VERSION_ATTRIBUTE ) )
				if ( Version != v_Undefined )
					ERRI( iBeam );
			break;
		case tValue:
			ERRI( iBeam );
			break;
		case tEndTag:
			Continue = false;
			break;
		case tProcessed:
			ERRc();
			break;
		case tError:
			ERRI( iBeam );
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


static void ImportDescription_(
	xml::browser___ &Browser,
	description_ &Description,
	xml::status__ &Status )
{
ERRProlog
	bso::bool__ Continue = true;
	str::string TagName, AttributeName, Value;
	xml::dump Dump;
	version__ Version = v_Undefined;
	STR_BUFFER___ ValueBuffer;
ERRBegin

	TagName.Init();
	AttributeName.Init();
	Value.Init();
	Dump.Init();

	while ( Continue) {
		switch ( Browser.Browse( TagName, AttributeName, Value, Dump, Status ) ) {
		case tProcessingInstruction:
			break;
		case tStartTag:
			if ( TagName == str::string( L( StructureTag ) ) )
				ImportStructureDescription_( Browser, Description, Version, Status );
			break;
		case tStartTagClosed:
			break;
		case tAttribute:
			if ( AttributeName == str::string( MBDDSC_VERSION_ATTRIBUTE ) )
				if ( Version != v_Undefined )
					ERRI( iBeam );
				else
					Version = Convert( Value.Convert( ValueBuffer ) );
			break;
		case tValue:
			ERRI( iBeam );
			break;
		case tEndTag:
			if ( TagName == str::string( MBDDSC_DESCRIPTION_TAG ) )
				Continue = false;
			break;
		case tProcessed:
			ERRc();
			break;
		case tError:
			ERRI( iBeam );
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

bso::bool__ mbddsc::Import(
	xtf::extended_text_iflow__ &Flow,
	description_ &Description )
{
	bso::bool__ Error = false;
ERRProlog
	xml::browser___ Browser;
	bso::bool__ Continue = true;
	str::string TagName, AttributeName, Value;
	xml::dump Dump;
	xml::status__ Status = s_Undefined;
ERRBegin
	Browser.Init( Flow );

	TagName.Init();
	AttributeName.Init();
	Value.Init();
	Dump.Init();

	while ( Continue ) {
		switch ( Browser.Browse( TagName, AttributeName, Value, Dump, Status ) ) {
		case tProcessingInstruction:
			break;
		case tStartTag:
			if ( TagName == str::string( MBDDSC_DESCRIPTION_TAG ) )
				ImportDescription_( Browser, Description, Status );
			break;
		case tStartTagClosed:
			break;
		case tAttribute:
			break;
		case tValue:
			break;
		case tEndTag:
			break;
		case tProcessed:
			Continue = false;
			break;
		case tError:
			Continue = false;
			Error = true;
			break;
		default:
			ERRc();
			break;
		}
	}

	
ERRErr
	if ( ( ERRMajor == err::itn ) && ( ERRMinor == err::iBeam ) ) {
		Error = true;
		ERRRst();
	}
ERREnd
ERREpilog
	return !Error;
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


