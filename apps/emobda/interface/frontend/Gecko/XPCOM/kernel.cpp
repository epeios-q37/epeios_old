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
using xml::writer_;

#define CASE( message )	\
	case m##message:\
		Message = #message;\
		break

static const char *GetRawMessage_( kernel::message__ MessageId )
{
	const char *Message = NULL;

	switch ( MessageId ) {
	CASE( DropStructureItemConfirmation );
	CASE( DeleteRecordConfirmation );
	CASE( DropRecordConfirmation );
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

static inline void PutInt_(
	const char *Name,
	bso::ulong__ Value,
	writer_ &Writer )
{
	bso::integer_buffer__ Buffer;

	Writer.PutAttribute( Name, bso::Convert( Value, Buffer ) );
}

template <typename id> static inline void PutId_(
	const char *Name,
	id Id,
	writer_ &Writer )
{
	PutInt_( Name, **Id, Writer );
}

static void DumpField_(
	table__ Table,
	field__ Field,
	const bkdacc::string_ &Name,
	const bkdacc::string_ &Comment,
	field_id__ Id,
	writer_ &Writer )
{
	Writer.PushTag( "Field" );

	PutId_( "Row", Field, Writer );
	PutId_( "TableRow", Table, Writer );
	PutId_( "Id", Id, Writer );

	Writer.PushTag( "Name" );
	Writer.PutValue( Name );
	Writer.PopTag();

	Writer.PushTag( "Comment" );
	Writer.PutValue( Comment );
	Writer.PopTag();

	Writer.PopTag();
}


static void DumpFields_(
	const fields_ &Fields,
	backend___ &Backend,
	writer_ &Writer )
{
ERRProlog
	bkdacc::strings Names, Comments;
	field_ids Ids;
	tables Tables;
	epeios::row__ Row = NONE;
ERRBegin
	Writer.PushTag( "Fields" );
	PutInt_( "Amount", Fields.Amount(), Writer );

	Names.Init();
	Comments.Init();
	Ids.Init();
	Tables.Init();

	Backend.GetFieldsInfos( Fields, Names, Comments, Ids, Tables );

	Row = Fields.First();

	while ( Row != NONE ) {
		DumpField_( Tables( Row ), Fields( Row ), Names( Row ), Comments( Row ), Ids( Row ), Writer );

		Row = Fields.Next( Row );
	}

	Writer.PopTag();

ERRErr
ERREnd
ERREpilog
}


static void DumpFields_(
	backend___ &Backend,
	writer_ &Writer )
{
ERRProlog
	fields Fields;
ERRBegin	
	Fields.Init();

	Backend.GetFields( UNDEFINED_TABLE, Fields );

	DumpFields_( Fields, Backend, Writer );
ERRErr
ERREnd
ERREpilog
}


static void DumpTable_(
	const bkdacc::string_ &Name,
	const bkdacc::string_ &Comment,
	table__ Table,
	table_id__ Id,
	backend___ &Backend,
	writer_ &Writer )
{
	PutId_( "Id", Id, Writer );
	PutId_( "Row", Table, Writer );

	Writer.PushTag( "Name" );
	Writer.PutValue( Name );
	Writer.PopTag();

	Writer.PushTag( "Comment" );
	Writer.PutValue( Comment );
	Writer.PopTag();
}

static void DumpTables_(
	const bkdacc::strings_ &Names,
	const bkdacc::strings_ &Comments,
	const tables_ &Tables,
	const table_ids_ &Ids,
	backend___ &Backend,
	writer_ &Writer )
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

		DumpTable_( Name( Row ), Comment( Row ), Tables( Row ), Ids( Row ), Backend, Writer );

		Writer.PopTag();

		Row = Names.Next( Row );
	}
}

static void DumpTables_(
	backend___ &Backend,
	writer_ &Writer )
{
ERRProlog
	tables Tables;
	bkdacc::strings Names, Comments;
	table_ids Ids;
ERRBegin
	Tables.Init();

	Backend.GetTables( Tables );

	Names.Init();
	Comments.Init();
	Ids.Init();

	Backend.GetTablesInfos( Tables, Names, Comments, Ids );

	if ( Tables.Amount() != Names.Amount() )
		ERRc();

	if ( Tables.Amount() != Ids.Amount() )
		ERRc();

	Writer.PushTag( "Tables" );

	DumpTables_( Names, Comments, Tables, Ids, Backend, Writer );

	Writer.PopTag();

ERRErr
ERREnd
ERREpilog
}

static void DumpDatabases_(
	backend___ &Backend,
	writer_ &Writer )
{
ERRProlog
	bkdacc::string Name, Comment;
ERRBegin
	Name.Init();
	Comment.Init();

	Backend.GetDatabaseInfos( Name, Comment );

	Writer.PushTag( "Databases" );

	if ( Name.Amount() != 0 ) {

		Writer.PushTag( "Database" );

		Writer.PushTag( "Name" );
		Writer.PutValue( Name );
		Writer.PopTag();

		Writer.PushTag( "Comment" );
		Writer.PutValue( Comment );
		Writer.PopTag();

		Writer.PopTag();
	}

	Writer.PopTag();
ERRErr
ERREnd
ERREpilog
}

void kernel::kernel___::_DumpStructure( writer_ &Writer )
{
	Writer.PushTag( "Structure" );

	DumpDatabases_( *this, Writer );
	DumpTables_( *this, Writer );
	DumpFields_( *this, Writer );

	Writer.PopTag();
}

static void Dump_(
	const target__ &Target,
	writer_ &Writer )
{
	if ( Target.Table != UNDEFINED_TABLE )
		PutId_( "Table", Target.Table, Writer );

	if ( Target.Field != UNDEFINED_FIELD )
		PutId_( "Field", Target.Field, Writer );

	if ( Target.Record != UNDEFINED_RECORD )
		PutId_( "Record", Target.Record, Writer );
}

void kernel::kernel___::_DumpCurrent( writer_ &Writer )
{
	Writer.PushTag( "Current" );

	Dump_( _Target, Writer );

	Writer.PopTag();
}

static void inline Dump_(
	field__ Field,
	const bkdacc::string_ &Datum,
	writer_ &Writer )
{
	Writer.PushTag( "Datum" );

	PutId_( "Field", Field, Writer );

	Writer.PutValue( Datum );

	Writer.PopTag();
}


static void Dump_(
	record__ Record,
	const fields_ &Fields,
	const bkdacc::strings_ &Data,
	writer_ &Writer )
{
	ctn::E_CMITEM( bkdacc::string_ ) Datum;
	epeios::row__ Row = Data.First();

	if ( Fields.Amount() != Data.Amount() )
		ERRc();

	Datum.Init( Data );

	Writer.PushTag( "Record" );

	PutId_( "Id", Record, Writer );

	Writer.PushTag( "Data" );

	while ( Row != NONE ) {
		Dump_( Fields( Row ), Datum( Row ), Writer );

		Row = Fields.Next( Row );
	}

	Writer.PopTag();

	Writer.PopTag();
}

static void Dump_(
	const records_ &Records,
	const fields_ &Fields,
	const bkdacc::xstrings_ &DataCluster,
	writer_ &Writer )
{
	ctn::E_CITEM( bkdacc::strings_ ) Data;
	epeios::row__ Row = Records.First();

	Data.Init( DataCluster );

	if ( Records.Amount() != DataCluster.Amount() )
		ERRc();

	Writer.PushTag( "Records" );

	PutInt_( "Amount", Records.Amount(), Writer );

	while ( Row != NONE ) {
		Dump_( Records( Row ), Fields, Data( Row ), Writer );

		Row = Records.Next( Row );
	}

	Writer.PopTag();
}


void kernel::kernel___::_DumpContent( writer_ &Writer )
{
ERRProlog
	fields Fields;
	records Records;
	bkdacc::xstrings DataCluster;
	table__ Table = UNDEFINED_TABLE;
ERRBegin
	Writer.PushTag( "Content" );

	if ( ( Table = Target().Table ) != UNDEFINED_TABLE ) {

		Fields.Init();
		this->GetFields( Table, Fields );

		Records.Init();

		if ( Target().Record == UNDEFINED_RECORD )
			this->GetRecords( Table, Records );
		else
			Records.Append( Target().Record );

		DataCluster.Init();
		this->GetRecordsData( Table, Fields, Records, DataCluster );

		Dump_( Records, Fields, DataCluster, Writer );
	}

	Writer.PopTag();
ERRErr
ERREnd
ERREpilog
}


void kernel::kernel___::_DumpAsXML( str::string_ &XML )
{
ERRProlog
	flx::E_STRING_OFLOW___ Flow;
	txf::text_oflow__ TFlow( Flow );
	xml::writer Writer;
ERRBegin
	Flow.Init( XML );
	Writer.Init( TFlow, true );

	Writer.PushTag( "emobda" );

	_DumpCurrent( Writer );

	_DumpStructure( Writer );

	_DumpContent( Writer );

	Writer.PopTag();

	Writer.reset();

	Flow.reset();

	// TFlow.reset();

	nsxpcm::Log( XML );
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

	_DumpAsXML( XML );

	// XML.Append( "<Structure><Tables><Table Name='T1'><Fields><Field Name='T1 F1'/><Field Name='T1 F2'/></Fields></Table><Table Name='T2'><Fields><Field Name='T2 F1'/><Field Name='T2 F2'/><Field Name='T2 F3'/></Fields></Table></Tables></Structure>" );

	Parameters.Init();

	Buffer = _Target;

	nsxpcm::RemoveChildren( UI.Structure.Items );	// Lance un évènement 'Select' qui remet à 0 '_Current'.

	nsxpcm::AppendChild( UI.Structure.Items, nsxpcm::XSLTTransform( XML, str::string( "chrome://emobda/content/StructureView.xsl" ), UI.Structure.Document, Parameters ) );

	_Target = Buffer;

	UpdateDecks();

	UI.Structure.Broadcasters.ItemEdition.Disable();

	SelectItem_( _Target, UI.Structure.Items, UI.Structure.BrowseTree );
ERRErr
ERREnd
ERREpilog
}

void kernel::kernel___::FillRecordForm( void )
{
ERRProlog
	nsxpcm::xslt_parameters Parameters;
	str::string XML;
ERRBegin
	XML.Init();

	_DumpAsXML( XML );

	// XML.Append( "<Structure><Tables><Table Name='T1'><Fields><Field Name='T1 F1'/><Field Name='T1 F2'/></Fields></Table><Table Name='T2'><Fields><Field Name='T2 F1'/><Field Name='T2 F2'/><Field Name='T2 F3'/></Fields></Table></Tables></Structure>" );

	Parameters.Init();

	UI.RecordForm.RecordBox.RemoveChildren();

	UI.RecordForm.RecordBox.AppendChild( nsxpcm::XSLTTransform( XML, str::string( "chrome://emobda/content/RecordForm.xsl" ), UI.Structure.Document, Parameters ) );
ERRErr
ERREnd
ERREpilog
}

void kernel::kernel___::FillRecordView( void )
{
ERRProlog
	nsxpcm::xslt_parameters Parameters;
	str::string XML;
ERRBegin
	XML.Init();

	_DumpAsXML( XML );

	Parameters.Init();

	UI.RecordView.RecordBox.RemoveChildren();

	UI.RecordView.RecordBox.AppendChild( nsxpcm::XSLTTransform( XML, str::string( "chrome://emobda/content/RecordView.xsl" ), UI.Structure.Document, Parameters ) );
ERRErr
ERREnd
ERREpilog
}

void kernel::kernel___::FillListView( void )
{
ERRProlog
	nsxpcm::xslt_parameters Parameters;
	str::string XML;
ERRBegin
	XML.Init();

	_DumpAsXML( XML );

	// XML.Append( "<Structure><Tables><Table Name='T1'><Fields><Field Name='T1 F1'/><Field Name='T1 F2'/></Fields></Table><Table Name='T2'><Fields><Field Name='T2 F1'/><Field Name='T2 F2'/><Field Name='T2 F3'/></Fields></Table></Tables></Structure>" );

	Parameters.Init();

	UI.ListView.ContentTree.RemoveChildren();

	UI.ListView.ContentTree.AppendChild( nsxpcm::XSLTTransform( XML, str::string( "chrome://emobda/content/ListView.xsl" ), UI.Structure.Document, Parameters ) );
ERRErr
ERREnd
ERREpilog
}

static void Register_(
	nsIDOMNode *Node,
	kernel___ &Kernel )
{
ERRProlog
	ui_main::table_menu_item__ *Item = NULL;
	nsxpcm::browser__ Browser;
	str::string Row;
ERRBegin 
	Browser.Init( Node );

	while ( ( Node = Browser.GetNext() ) != NULL ) {
		Row.Init();

		nsxpcm::GetAttribute( Node, "Row", Row );

		if ( Row.Amount() ) {
			Item = new ui_main::table_menu_item__;

			ui_base::Register( Kernel, *Item, nsxpcm::QueryInterface<nsIDOMElement>( Node ), nsxpcm::efCommand );
		}
	}

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

	_DumpAsXML( XML );

	// XML.Append( "<Structure><Tables><Table Name='T1'><Fields><Field Name='T1 F1'/><Field Name='T1 F2'/></Fields></Table><Table Name='T2'><Fields><Field Name='T2 F1'/><Field Name='T2 F2'/><Field Name='T2 F3'/></Fields></Table></Tables></Structure>" );

	Parameters.Init();

	UI.Main.TableMenu.RemoveChildren();

	UI.Main.TableMenu.AppendChild( nsxpcm::XSLTTransform( XML, str::string( "chrome://emobda/content/TableMenu.xsl" ), UI.Structure.Document, Parameters ) );

	Register_( UI.Main.TableMenu.GetElement(), *this );
ERRErr
ERREnd
ERREpilog
}

void kernel::kernel___::SelectStructureItem( void )
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
			Table = ExtractTable( TableRow );
		} else if ( Type == "Table" ) {
			Row.Init();
			Tree.GetCurrentItemAttribute( "Row", Row );
			Table = ExtractTable( Row );
		} else if ( Type != "Database" )
			ERRc();
	}

	Target().Set( Field, Table );
ERRErr
ERREnd
ERREpilog
}

void kernel::kernel___::SelectRecord( void )
{
ERRProlog
	str::string Id;
	record__ Record = UNDEFINED_RECORD;
	ui_lst_v::content_tree__ &Tree = UI.ListView.ContentTree;
ERRBegin

	if ( Tree.IsThereSelected() ) {
		Id.Init();
		Tree.GetCurrentItemAttribute( "Record", Id );

		Record = ExtractRecord( Id );
	}

	Target().Set( Record );

	UI.Main.Broadcasters.RecordSelected.Enable();
ERRErr
ERREnd
ERREpilog
}

void kernel::kernel___::UpdateDecks( void )
{
ERRProlog
	str::string Type;
	str::string Name, Comment;
	str::string Row;
	epeios::row__ Error = NONE;
ERRBegin
	ui_struct::structure__ &UI = this->UI.Structure;

	Name.Init();
	Comment.Init();

	if ( UI.BrowseTree.GetCurrentIndex() != -1 ) {

		Type.Init();
		UI.BrowseTree.GetCurrentItemAttribute( "Type", Type );

		
		if ( Type == "Database" ) {
			K().GetDatabaseInfos( Name, Comment );
		} else if ( Type == "Table" ) {
			table_id__ TableId = UNDEFINED_TABLE_ID;

			Row.Init();
			UI.BrowseTree.GetCurrentItemAttribute( "Row", Row );

			K().GetTableInfo( ExtractTable( Row ), Name, Comment, TableId );
		} else if ( Type == "Field" ) {
			field_id__ FieldId = UNDEFINED_FIELD_ID;
			table__ Table = UNDEFINED_TABLE;
			
			Row.Init();
			UI.BrowseTree.GetCurrentItemAttribute( "Row", Row );

			K().GetFieldInfo( ExtractField( Row ), Name, Comment, FieldId, Table );
		}

		UI.NameTextbox.SetValue( Name );

		UI.CommentTextbox.SetValue( Comment );

		if ( Type == "Database" ) {
			UI.Broadcasters.Database.Deletion.Enable();
			UI.Broadcasters.Database.Modification.Enable();

			UI.Broadcasters.Table.Creation.Enable();
			UI.Broadcasters.Table.Modification.Disable();
			UI.Broadcasters.Table.Creation.Disable();

			UI.Broadcasters.Field.Creation.Disable();
			UI.Broadcasters.Field.Modification.Disable();
			UI.Broadcasters.Field.Creation.Disable();

			UI.ActionDeck.SetSelectedPanel( UI.DatabaseSelectionPanel );
		} else if ( Type == "Table" ) {
			UI.Broadcasters.Database.Deletion.Disable();
			UI.Broadcasters.Database.Modification.Disable();

			UI.Broadcasters.Table.Creation.Enable();
			UI.Broadcasters.Table.Modification.Enable();
			UI.Broadcasters.Table.Creation.Enable();

			UI.Broadcasters.Field.Creation.Enable();
			UI.Broadcasters.Field.Modification.Disable();
			UI.Broadcasters.Field.Deletion.Disable();

			UI.ActionDeck.SetSelectedPanel( UI.TableSelectionPanel );
		}else if ( Type == "Field" ) {
			UI.Broadcasters.Database.Deletion.Disable();
			UI.Broadcasters.Database.Modification.Disable();

			UI.Broadcasters.Table.Creation.Disable();
			UI.Broadcasters.Table.Modification.Disable();
			UI.Broadcasters.Table.Creation.Disable();

			UI.Broadcasters.Field.Creation.Enable();
			UI.Broadcasters.Field.Modification.Enable();
			UI.Broadcasters.Field.Deletion.Enable();

			UI.ActionDeck.SetSelectedPanel( UI.FieldSelectionPanel );
		} else
			ERRu();
	} else {
		UI.ActionDeck.SetSelectedIndex( -1 );
	}

ERRErr
ERREnd
ERREpilog
}

void kernel::kernel___::_SwitchTo( context__ Context )
{
	switch ( Context ) {
	case cSessionForm:
		K().FillTableMenu();
		UI.Main.Broadcasters.DatabaseOpened.Disable();
		UI.Main.Broadcasters.TableSelected.Disable();
		UI.Main.Broadcasters.RecordSelected.Disable();
		break;
	case cStructureView:
		FillStructureView();
		K().FillTableMenu();
		UI.Main.Broadcasters.DatabaseOpened.Enable();
		UI.Structure.Broadcasters.ItemBrowsing.Enable();
		UI.Structure.Broadcasters.ItemEdition.Disable();
		UI.Main.MainDeck.SetSelectedPanel( UI.Main.Panels.StructureFormAndView );
		break;
	case cStructureItemView:
		UpdateDecks();
		UI.Structure.Broadcasters.ItemBrowsing.Enable();
		UI.Structure.Broadcasters.ItemEdition.Disable();
		UI.Main.MainDeck.SetSelectedPanel( UI.Main.Panels.StructureFormAndView );
		break;
	case cListView:
		K().FillListView();
		UI.Main.Broadcasters.RecordSelected.Disable();
		UI.Main.MainDeck.SetSelectedPanel( UI.Main.Panels.ListView );

		if ( UI.ListView.ContentTree.IsThereSelected() )
			SelectRecord();
		break;
	case cRecordForm:
		K().FillRecordForm();
		UI.Main.MainDeck.SetSelectedPanel( UI.Main.Panels.RecordForm );
		break;
	case cRecordView:
		K().FillRecordView();
		UI.Main.MainDeck.SetSelectedPanel( UI.Main.Panels.RecordView );
		break;
	default:
		ERRc();
		break;
	}
}

void kernel::kernel___::ApplyStructureItem( void )
{
	target__ Target;

	if ( GetTarget().Table == UNDEFINED_TABLE ) {
		if ( _Temporary.Mode != tmCreation )
			ERRc();

		Target.Table = _CreateOrModifyTable();
	} else if ( GetTarget().Field == UNDEFINED_FIELD ) {
		switch ( _Temporary.Mode ) {
		case tmCreation:
			Target.Set( _CreateOrModifyField(), GetTarget().Table );
			break;
		case tmModification:
			Target.Table = _CreateOrModifyTable();
			break;
		default:
			ERRc();
			break;
		}
	} else {
		if ( _Temporary.Mode != tmModification )
			ERRc();

		Target.Set( _CreateOrModifyField(), GetTarget().Table );
	}

	_Temporary.reset();

	this->Target() = Target;
	_SwitchTo( cStructureView );
}

static void RetrieveData_(
	nsIDOMNode *Node,
	bkdacc::items32_ &Data )
{
ERRProlog
	nsxpcm::browser__ Browser;
	str::string Value;
	epeios::row__ Error = NONE;
	bkdacc::id32__ Id = BKDACC_UNDEFINED_ID32;
	bkdacc::item32 Item;
ERRBegin
	Browser.Init( Node );

	while ( ( Node = Browser.GetNext() ) != NULL ) {
		Value.Init();

		nsxpcm::GetAttribute( Node, "Field", Value, err::hSkip );

		if ( Value.Amount() != 0 ) {
			Id = Value.ToUL( &Error );

			if ( Error != NONE )
				ERRu();

			Value.Init();
			nsxpcm::GetValue( nsxpcm::QueryInterface<nsIDOMXULTextBoxElement>( Node ), Value );

			if ( Value.Amount() != 0 ) {
				Item.Init( Id, Value );
	
				Data.Append( Item );
			}
		}
	}
ERRErr
ERREnd
ERREpilog
}

void kernel::kernel___::ApplyRecord( void )
{
ERRProlog
	bkdacc::items32 Data;
ERRBegin
	Data.Init();
	RetrieveData_( UI.RecordForm.RecordBox.GetObject(), Data );

	if ( Target().Record == UNDEFINED_RECORD )
		Target().Set( InsertRecord( Data, _Target.Table ) );
	else
		ModifyRecord( Target().Record, Data, _Target.Table );

	_SwitchTo( cRecordView );
ERRErr
ERREnd
ERREpilog
}

void kernel::kernel___::SelectTable( ui_main::table_menu_item__ &MenuItem )
{
ERRProlog
	str::string Row;
	epeios::row__ Error = NONE;
	table__ Table;
ERRBegin
	Row.Init();
	nsxpcm::GetAttribute( MenuItem.GetElement(), "Row", Row );

	*Table = Row.ToUL( &Error );

	if ( Error != NONE )
		ERRc();

	SelectTable( Table );
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