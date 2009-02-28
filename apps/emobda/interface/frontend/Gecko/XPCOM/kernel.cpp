/*
	'kernel.cpp' by Claude SIMON (http://zeusw.org/).

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

#include "kernel.h"
#include "flx.h"

using namespace kernel;

#define CASE( message )	\
	case m##message:\
		Message = #message;\
		break

static const char *GetRawMessage_( kernel::message__ MessageId )
{
	const char *Message = NULL;

	switch ( MessageId ) {
	CASE( CancelInputConfirmation );
	default:
		ERRu();
		break;
	}

	return Message;
}

typedef msg::i18_messages_ _messages_;
typedef msg::i18_messages _messages;

static class messages
: public _messages
{
protected:
	const char *MSGGetRawMessage( int MessageId ) const
	{
		return GetRawMessage_( (kernel::message__)MessageId );
	}
} Messages_;

static const char *GetMessage_(
							   kernel::message__ Message,
	lgg::language__ Language,
	msg::buffer__ &Buffer )
{
	return ::Messages_.GetMessage( Message, Language, Buffer );
}

const char *kernel::kernel___::GetMessage( kernel::message__ MessageId )
{
	return GetMessage_( MessageId, _Language, _MessageBuffer );
}

void kernel::kernel___::GetTableInfo(
	table__ Table,
	str::string_ &Name,
	str::string_ &Comment,
	table_id__ &Id )
{
ERRProlog
	tables Tables;
	bkdacc::strings Names, Comments;
	table_ids Ids;
ERRBegin
	Tables.Init();
	Tables.Append( Table );

	Names.Init();
	Comments.Init();
	Ids.Init();

	GetTablesInfos( Tables, Names, Comments, Ids );

	Name = Names( Names.First() );
	Comment = Comments( Comments.First() );
	Id = Ids( Ids.First() );
ERRErr
ERREnd
ERREpilog
}

void kernel::kernel___::GetFieldInfo(
	field__ Field,
	str::string_ &Name,
	str::string_ &Comment,
	field_id__ &Id )
{
ERRProlog
	fields Fields;
	bkdacc::strings Names, Comments;
	field_ids Ids;
ERRBegin
	Fields.Init();
	Fields.Append( Field );

	Names.Init();
	Comments.Init();
	Ids.Init();

	GetFieldsInfos( Fields, Names, Comments, Ids );

	Name = Names( Names.First() );
	Comment = Comments( Comments.First() );
	Id = Ids( Ids.First() );
ERRErr
ERREnd
ERREpilog
}

static void DumpFieldStructure_(
	table__ Table,
	field__ Field,
	const bkdacc::string_ &Name,
	const bkdacc::string_ &Comment,
	field_id__ Id,
	xml::writer_ &Writer )
{
	bso::integer_buffer__ Buffer;

	Writer.PushTag( "Field" );

	Writer.PutAttribute( "Row", bso::Convert( **Field, Buffer ) );
	Writer.PutAttribute( "TableRow", bso::Convert( **Table, Buffer ) );
	Writer.PutAttribute( "Id", bso::Convert( **Id, Buffer ) );

	Writer.PushTag( "Name" );
	Writer.PutValue( Name );
	Writer.PopTag();

	Writer.PushTag( "Comment" );
	Writer.PutValue( Comment );
	Writer.PopTag();

	Writer.PopTag();
}


static void DumpFieldsStructure_(
	table__ Table,
	const fields_ &Fields,
	kernel___ &Kernel,
	xml::writer_ &Writer )
{
ERRProlog
	bkdacc::strings Names, Comments;
	field_ids Ids;
	epeios::row__ Row = NONE;
	bso::integer_buffer__ Buffer;
ERRBegin
	Writer.PushTag( "Fields" );
	Writer.PutAttribute( "Amount", bso::Convert( Fields.Amount(), Buffer ) );

	Names.Init();
	Comments.Init();
	Ids.Init();

	Kernel.GetFieldsInfos( Fields, Names, Comments, Ids );

	Row = Fields.First();

	while ( Row != NONE ) {
		DumpFieldStructure_( Table, Fields( Row ), Names( Row ), Comments( Row ), Ids( Row ), Writer );

		Row = Fields.Next( Row );
	}

	Writer.PopTag();

ERRErr
ERREnd
ERREpilog
}


static void DumpFieldsStructure_(
	table__ Table,
	kernel___ &Kernel,
	xml::writer_ &Writer )
{
ERRProlog
	fields Fields;
ERRBegin	
	Fields.Init();

	Kernel.GetFields( Table, Fields );

	DumpFieldsStructure_( Table, Fields, Kernel, Writer );
ERRErr
ERREnd
ERREpilog
}


static void DumpTableStructure_(
	const bkdacc::string_ &Name,
	const bkdacc::string_ &Comment,
	table__ Table,
	table_id__ Id,
	kernel___ &Kernel,
	xml::writer_ &Writer )
{
	bso::integer_buffer__ Buffer;

	Writer.PutAttribute( "Id", bso::Convert( **Id, Buffer ) );
	Writer.PutAttribute( "Row", bso::Convert( **Table, Buffer ) );

	Writer.PushTag( "Name" );
	Writer.PutValue( Name );
	Writer.PopTag();

	Writer.PushTag( "Comment" );
	Writer.PutValue( Comment );
	Writer.PopTag();

	DumpFieldsStructure_( Table, Kernel, Writer );
}

static void DumpTablesStructure_(
	const bkdacc::strings_ &Names,
	const bkdacc::strings_ &Comments,
	const tables_ &Tables,
	const table_ids_ &Ids,
	kernel___ &Kernel,
	xml::writer_ &Writer )
{
	ctn::E_CMITEM( bkdacc::string_ ) Name, Comment;
	epeios::row__ Row = Names.First();

	if ( Names.Amount() != Comments.Amount() )
		ERRc();

	if ( Names.Amount() != Ids.Amount() )
		ERRc();

	Name.Init( Names );
	Comment.Init( Comments );

	while ( Row != NONE ) {
		Writer.PushTag( "Table" );

		DumpTableStructure_( Name( Row ), Comment( Row ), Tables( Row ), Ids( Row ), Kernel, Writer );

		Writer.PopTag();

		Row = Names.Next( Row );
	}
}

static void DumpTablesStructure_(
	kernel___ &Kernel,
	xml::writer_ &Writer )
{
ERRProlog
	tables Tables;
	bkdacc::strings Names, Comments;
	table_ids Ids;
ERRBegin
	Tables.Init();

	Kernel.GetTables( Tables );

	Names.Init();
	Comments.Init();
	Ids.Init();

	Kernel.GetTablesInfos( Tables, Names, Comments, Ids );

	if ( Tables.Amount() != Names.Amount() )
		ERRc();

	if ( Tables.Amount() != Ids.Amount() )
		ERRc();

	Writer.PushTag( "Tables" );

	DumpTablesStructure_( Names, Comments, Tables, Ids, Kernel, Writer );

	Writer.PopTag();

ERRErr
ERREnd
ERREpilog
}

static void DumpDatabaseStructure_(
	kernel___ &Kernel,
	xml::writer_ &Writer )
{
ERRProlog
	bkdacc::string Name, Comment;
ERRBegin
	Name.Init();
	Comment.Init();

	Kernel.GetDatabaseInfos( Name, Comment );

	if ( Name.Amount() != 0 ) {

		Writer.PushTag( "Database" );

		Writer.PushTag( "Name" );
		Writer.PutValue( Name );
		Writer.PopTag();

		Writer.PushTag( "Comment" );
		Writer.PutValue( Comment );
		Writer.PopTag();

		DumpTablesStructure_( Kernel, Writer );

		Writer.PopTag();
	}
ERRErr
ERREnd
ERREpilog
}

void kernel::kernel___::_DumpStructureAsXML(
	const target__ &Current,
	str::string_ &XML )
{
ERRProlog
	flx::E_STRING_OFLOW___ Flow;
	txf::text_oflow__ TFlow( Flow );
	xml::writer Writer;
	bso::integer_buffer__ Buffer;
ERRBegin
	Flow.Init( XML );
	Writer.Init( TFlow, false );

	Writer.PushTag( "Structure" );

	if ( Current.Table != UNDEFINED_TABLE )
		Writer.PutAttribute( "CurrentTable", bso::Convert( **Current.Table, Buffer ) );

	if ( Current.Field != UNDEFINED_FIELD )
		Writer.PutAttribute( "CurrentField", bso::Convert( **Current.Field, Buffer ) );

	DumpDatabaseStructure_( *this, Writer );

	Writer.PopTag();
ERRErr
ERREnd
ERREpilog
}

static void SelectItem_(
	const str::string_ &Type,
	const str::string_ &Row,
	nsIDOMNode *Root,
	nsxpcm::tree__ &Tree )
{
ERRProlog
	str::string RowBuffer, TypeBuffer;
	nsxpcm::browser__ Browser;
	nsIDOMNode *&Node = Root;
ERRBegin
	Browser.Init( Root );

	TypeBuffer.Init();
	RowBuffer.Init();


	while ( ( ( Node = Browser.GetNext() ) != NULL )
		&& ( ( nsxpcm::GetAttribute( Node, "Type", TypeBuffer ) != Type )
		      || ( nsxpcm::GetAttribute( Node, "Row", RowBuffer ) != Row ) ) ) {
		TypeBuffer.Init();
		RowBuffer.Init();
	}

	if ( Node == NULL )
		ERRc();

	Tree.Select( Node );
ERRErr
ERREnd
ERREpilog
}

static inline void SelectItem_(
	const char *Type,
	const char *Row,
	nsIDOMNode *Root,
	nsxpcm::tree__ &Tree )
{
	SelectItem_( str::string( Type ), str::string( Row ), Root, Tree );
}

static inline void SelectItem_(
	field__ Field,
	nsIDOMNode *Root,
	nsxpcm::tree__ &Tree )
{
	bso::integer_buffer__ Buffer;

	SelectItem_( "Field", bso::Convert( **Field, Buffer ), Root, Tree );
}

static inline void SelectItem_(
	table__ Table,
	nsIDOMNode *Root,
	nsxpcm::tree__ &Tree )
{
	bso::integer_buffer__ Buffer;

	SelectItem_( "Table", bso::Convert( **Table, Buffer ), Root, Tree );
}

static inline void SelectItem_(
	const target__ &Target,
	nsIDOMNode *Root,
	nsxpcm::tree__ &Tree )
{
	if ( Target.Field != UNDEFINED_FIELD )
		SelectItem_( Target.Field, Root, Tree );
	else  if ( Target.Table != UNDEFINED_TABLE )
		SelectItem_( Target.Table, Root, Tree );
}

void kernel::kernel___::FillStructureView( void )
{
ERRProlog
	nsxpcm::xslt_parameters Parameters;
	str::string XML;
	target__ Buffer;
ERRBegin
	XML.Init();

	_DumpStructureAsXML( _Current, XML );

	nsxpcm::Log( XML );

	// XML.Append( "<Structure><Tables><Table Name='T1'><Fields><Field Name='T1 F1'/><Field Name='T1 F2'/></Fields></Table><Table Name='T2'><Fields><Field Name='T2 F1'/><Field Name='T2 F2'/><Field Name='T2 F3'/></Fields></Table></Tables></Structure>" );

	Parameters.Init();

	Buffer = _Current;

	nsxpcm::RemoveChildren( UI.Structure.Items );	// Lance un évènement 'Select' qui remet à 0 '_Current'.

	nsxpcm::AppendChild( UI.Structure.Items, nsxpcm::XSLTTransform( XML, str::string( "chrome://emobda/content/StructureView.xsl" ), UI.Structure.Document, Parameters ) );

	_Current = Buffer;

	UI.Structure.UpdateDecks();

	UI.Structure.Broadcasters.ItemEdition.Disable();

	SelectItem_( _Current, UI.Structure.Items, UI.Structure.BrowseTree );
ERRErr
ERREnd
ERREpilog
}

void kernel::kernel___::FillTableMenu( void )
{
ERRProlog
	nsxpcm::xslt_parameters Parameters;
	str::string XML;
ERRBegin
	XML.Init();

	_DumpStructureAsXML( target__(), XML );

	// XML.Append( "<Structure><Tables><Table Name='T1'><Fields><Field Name='T1 F1'/><Field Name='T1 F2'/></Fields></Table><Table Name='T2'><Fields><Field Name='T2 F1'/><Field Name='T2 F2'/><Field Name='T2 F3'/></Fields></Table></Tables></Structure>" );

	Parameters.Init();

	UI.Main.TableMenu.RemoveChildren();

	UI.Main.TableMenu.AppendChild( nsxpcm::XSLTTransform( XML, str::string( "chrome://emobda/content/TableMenu.xsl" ), UI.Structure.Document, Parameters ) );
ERRErr
ERREnd
ERREpilog
}

void kernel::kernel___::SetCurrent( void )
{
ERRProlog
	str::string Type, Row, TableRow;
	field__ Field = UNDEFINED_FIELD;
	table__ Table = UNDEFINED_TABLE;
	ui_struct::browse_tree__ &Tree = UI.Structure.BrowseTree;
ERRBegin

	if ( Tree.IsThereSelected() ) {
		Type.Init();
		Tree.GetCurrentItemAttribute( "Type", Type );

		if ( Type == "Field" ) {
			Row.Init();
			Tree.GetCurrentItemAttribute( "Row", Row );
			Field = ExtractField( Row );

			TableRow.Init();
			Tree.GetCurrentItemAttribute( "TableRow", TableRow );
			Table = ExtractTable( Row );
		} else if ( Type == "Table" ) {
			Row.Init();
			Tree.GetCurrentItemAttribute( "Row", Row );
			Table = ExtractTable( Row );
		} else if ( Type != "Database" )
			ERRc();
	}

	SetCurrent( target__( Field, Table ) );
ERRErr
ERREnd
ERREpilog
}





static class starter 
{
public:
	starter( void )
	{
		::Messages_.Init( m_amount );
	}
	~starter( void )
	{}
} Starter_;