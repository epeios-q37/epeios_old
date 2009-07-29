/*
	'ui_struct.cpp' by Claude SIMON (http://zeusw.org/).

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

#include "ui_struct.h"
#include "kernel.h"

using namespace ui_struct;
using kernel::kernel___;
using nsxpcm::event__;
using mbdkernl::target__;

using namespace kernel;

bso::bool__ ui_struct::structure__::GetSelectedItem( target__ &Target )
{
	bso::bool__ Selected = false;
ERRProlog
	str::string Type, Row, TableRow;
ERRBegin

	if ( BrowseTree.IsThereSelected() ) {
		Type.Init();
		BrowseTree.GetCurrentItemAttribute( "Type", Type );

		if ( Type == "Field" ) {
			Row.Init();
			BrowseTree.GetCurrentItemAttribute( "Row", Row );
			Target.Field = mbdkernl::ExtractField( Row );

			TableRow.Init();
			BrowseTree.GetCurrentItemAttribute( "TableRow", TableRow );
			Target.Table = mbdkernl::ExtractTable( TableRow );
		} else if ( Type == "Table" ) {
			Row.Init();
			BrowseTree.GetCurrentItemAttribute( "Row", Row );
			Target.Table = mbdkernl::ExtractTable( Row );
		} else if ( Type != "Database" )
			ERRc();

		Selected = true;
	}
ERRErr
ERREnd
ERREpilog
	return Selected;
}

#if 1
void ui_struct::structure__::UpdateDecks( void )
{
ERRProlog
	str::string Name, Comment;
	target__ Target;
ERRBegin
	mbdkernl::kernel___ &Kernel = UI()._K();

	Name.Init();
	Comment.Init();

	if ( GetSelectedItem( Target ) ) {

		if ( Target.Field != UNDEFINED_FIELD ) {
			field_id__ FieldId = UNDEFINED_FIELD_ID;
			table__ Table = UNDEFINED_TABLE;

			if ( !Kernel.GetFieldInfos( Target.Field, Name, Comment, FieldId, Table ) )
				ERRReturn;

			if ( Table != Target.Table )
				ERRc();

			Broadcasters.Database.Deletion.Disable();
			Broadcasters.Database.Modification.Disable();

			Broadcasters.Table.Creation.Disable();
			Broadcasters.Table.Modification.Disable();
			Broadcasters.Table.Creation.Disable();

			Broadcasters.Field.Creation.Enable();
			Broadcasters.Field.Modification.Enable();
			Broadcasters.Field.Deletion.Enable();

			ActionDeck.SetSelectedPanel( FieldSelectionPanel );
		} else if ( Target.Table != UNDEFINED_TABLE ) {
			table_id__ TableId = UNDEFINED_TABLE_ID;

			if ( !Kernel.GetTableInfos( Target.Table, Name, Comment, TableId ) )
				ERRReturn;

			Broadcasters.Database.Deletion.Disable();
			Broadcasters.Database.Modification.Disable();

			Broadcasters.Table.Creation.Enable();
			Broadcasters.Table.Modification.Enable();
			Broadcasters.Table.Creation.Enable();

			Broadcasters.Field.Creation.Enable();
			Broadcasters.Field.Modification.Disable();
			Broadcasters.Field.Deletion.Disable();

			ActionDeck.SetSelectedPanel( TableSelectionPanel );
		} else {

			if ( !Kernel.GetDatabaseInfos( Name, Comment ) )
				ERRReturn;

			Broadcasters.Database.Deletion.Enable();
			Broadcasters.Database.Modification.Enable();

			Broadcasters.Table.Creation.Enable();
			Broadcasters.Table.Modification.Disable();
			Broadcasters.Table.Creation.Disable();

			Broadcasters.Field.Creation.Disable();
			Broadcasters.Field.Modification.Disable();
			Broadcasters.Field.Creation.Disable();

			ActionDeck.SetSelectedPanel( DatabaseSelectionPanel );
		}

		Kernel.Target() = Target;

		NameTextbox.SetValue( Name );

		CommentTextbox.SetValue( Comment );

	} else {
		ActionDeck.SetSelectedIndex( -1 );
	}

ERRErr
ERREnd
ERREpilog
}
#else
void ui_struct::structure__::UpdateDecks( void )
{
ERRProlog
	str::string Type;
	str::string Name, Comment;
	str::string Row;
	epeios::row__ Error = NONE;
	table__ Table = UNDEFINED_TABLE;
	field__ Field = UNDEFINED_FIELD;
ERRBegin
	mbdkernl::kernel___ &Kernel = UI()._K();

	Name.Init();
	Comment.Init();

	if ( BrowseTree.GetCurrentIndex() != -1 ) {

		Type.Init();
		BrowseTree.GetCurrentItemAttribute( "Type", Type );
		
		if ( Type == "Database" ) {
			if ( !Kernel.GetDatabaseInfos( Name, Comment ) )
				ERRReturn;
		} else if ( Type == "Table" ) {
			table_id__ TableId = UNDEFINED_TABLE_ID;

			Row.Init();
			BrowseTree.GetCurrentItemAttribute( "Row", Row );

			Table = mbdkernl::ExtractTable( Row );

			if ( !Kernel.GetTableInfos( Table, Name, Comment, TableId ) )
				ERRReturn;
		} else if ( Type == "Field" ) {
			field_id__ FieldId = UNDEFINED_FIELD_ID;
			
			Row.Init();
			BrowseTree.GetCurrentItemAttribute( "Row", Row );

			Field = mbdkernl::ExtractField( Row );

			if ( !Kernel.GetFieldInfos( Field, Name, Comment, FieldId, Table ) )
				ERRReturn;
		}

		Kernel.Target().Set( Field, Table );

		NameTextbox.SetValue( Name );

		CommentTextbox.SetValue( Comment );

		if ( Type == "Database" ) {
			Broadcasters.Database.Deletion.Enable();
			Broadcasters.Database.Modification.Enable();

			Broadcasters.Table.Creation.Enable();
			Broadcasters.Table.Modification.Disable();
			Broadcasters.Table.Creation.Disable();

			Broadcasters.Field.Creation.Disable();
			Broadcasters.Field.Modification.Disable();
			Broadcasters.Field.Creation.Disable();

			ActionDeck.SetSelectedPanel( DatabaseSelectionPanel );
		} else if ( Type == "Table" ) {
			Broadcasters.Database.Deletion.Disable();
			Broadcasters.Database.Modification.Disable();

			Broadcasters.Table.Creation.Enable();
			Broadcasters.Table.Modification.Enable();
			Broadcasters.Table.Creation.Enable();

			Broadcasters.Field.Creation.Enable();
			Broadcasters.Field.Modification.Disable();
			Broadcasters.Field.Deletion.Disable();

			ActionDeck.SetSelectedPanel( TableSelectionPanel );
		}else if ( Type == "Field" ) {
			Broadcasters.Database.Deletion.Disable();
			Broadcasters.Database.Modification.Disable();

			Broadcasters.Table.Creation.Disable();
			Broadcasters.Table.Modification.Disable();
			Broadcasters.Table.Creation.Disable();

			Broadcasters.Field.Creation.Enable();
			Broadcasters.Field.Modification.Enable();
			Broadcasters.Field.Deletion.Enable();

			ActionDeck.SetSelectedPanel( FieldSelectionPanel );
		} else
			ERRu();
	} else {
		ActionDeck.SetSelectedIndex( -1 );
	}

ERRErr
ERREnd
ERREpilog
}
#endif


static void SelectStructItem_(
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

static inline void SelectStructItem_(
	const char *Type,
	const char *Row,
	nsIDOMNode *Root,
	nsxpcm::tree__ &Tree )
{
	SelectStructItem_( str::string( Type ), str::string( Row ), Root, Tree );
}

static inline void SelectStructItem_(
	field__ Field,
	nsIDOMNode *Root,
	nsxpcm::tree__ &Tree )
{
	bso::integer_buffer__ Buffer;

	SelectStructItem_( "Field", bso::Convert( **Field, Buffer ), Root, Tree );
}

static inline void SelectStructItem_(
	table__ Table,
	nsIDOMNode *Root,
	nsxpcm::tree__ &Tree )
{
	bso::integer_buffer__ Buffer;

	SelectStructItem_( "Table", bso::Convert( **Table, Buffer ), Root, Tree );
}

static inline void SelectStructItem_(
	const target__ &Target,
	nsIDOMNode *Root,
	nsxpcm::tree__ &Tree )
{
	if ( Target.Field != UNDEFINED_FIELD )
		SelectStructItem_( Target.Field, Root, Tree );
	else  if ( Target.Table != UNDEFINED_TABLE )
		SelectStructItem_( Target.Table, Root, Tree );
}

void ui_struct::structure__::FillView( void )
{
ERRProlog
	nsxpcm::xslt_parameters Parameters;
	str::string XML;
	target__ Target;
ERRBegin
	mbdkernl::kernel___ &Kernel = UI()._K();

	if ( Kernel.GetTransientContext() == mbdtrnsnt::cStructureManagement )
		Target = Kernel.Target();
	else
		GetSelectedItem( Target );

	XML.Init();

	Kernel.DumpAsXML( XML, false );

	// XML.Append( "<Structure><Tables><Table Name='T1'><Fields><Field Name='T1 F1'/><Field Name='T1 F2'/></Fields></Table><Table Name='T2'><Fields><Field Name='T2 F1'/><Field Name='T2 F2'/><Field Name='T2 F3'/></Fields></Table></Tables></Structure>" );

	Parameters.Init();

	nsxpcm::RemoveChildren( Items );	// Launch an event which reset '_Target'.

	nsxpcm::AppendChild( Items, nsxpcm::XSLTTransform( XML, str::string( "chrome://emobda/content/StructureView.xsl" ), Document, Parameters ) );

	UpdateDecks();

	Broadcasters.ItemEdition.Disable();

	SelectStructItem_( Kernel.Target(), Items, BrowseTree );	// Launch an event which set '_Target'.

	Kernel.ResetTransient();
ERRErr
ERREnd
ERREpilog
}



void ui_struct::modify_database_command__::NSXPCMOnEvent( event__ )
{
}

void ui_struct::delete_database_command__::NSXPCMOnEvent( event__ )
{
}

void ui_struct::create_table_command__::NSXPCMOnEvent( event__ )
{
	UI().K().StructureManagement().SetState( mbdkernl::smsCreation );
	UI().K().Target().reset();
	UI().DefineTable();
}

void ui_struct::modify_table_command__::NSXPCMOnEvent( event__ )
{
	UI().K().StructureManagement().SetState( mbdkernl::smsModification );
	UI().DefineTable();
}

void ui_struct::delete_table_command__::NSXPCMOnEvent( event__ )
{
	if ( UI().Confirm( mbdkernl::mDeleteTableConfirmation ) ) {
		UI().DeleteTable();
	}
}

void ui_struct::create_field_command__::NSXPCMOnEvent( event__ )
{
	UI().K().StructureManagement().SetState( mbdkernl::smsCreation );
	UI().K().Target().Set( UNDEFINED_FIELD );
	UI().DefineField();
}

void ui_struct::modify_field_command__::NSXPCMOnEvent( event__ )
{
	UI().K().StructureManagement().SetState( mbdkernl::smsModification );
	UI().DefineField();
}

void ui_struct::delete_field_command__::NSXPCMOnEvent( event__ )
{
	if ( UI().Confirm( mbdkernl::mDeleteFieldConfirmation ) ) {
		UI().DeleteField();
	}
}

void ui_struct::apply_item_command__::NSXPCMOnEvent( event__ )
{
	UI().ApplyStructureItem();
}

void ui_struct::cancel_item_command__::NSXPCMOnEvent( event__ )
{
	if ( UI().Confirm( mbdkernl::mDropStructureItemConfirmation ) ) {
		UI().DropStructureItem();
	}
}

void ui_struct::browse_tree__::NSXPCMOnEvent( event__ Event )
{
	switch ( Event ) {
		case nsxpcm::eSelect:
			UI().BrowseStructureItem();
			break;
		case nsxpcm::eDblClick:
			UI().K().StructureManagement().SetState( mbdkernl::smsModification );
			UI().DefineStructureItem();
			break;
		default:
			ERRc();
			break;
	}
}

/* UI Registrations */

static void Register_(
	bridge_functions__ &Functions,
	broadcaster__ &Broadcaster,
	nsIDOMDocument *Document,
	const char *Id )
{
	ui_base::Register( Functions, Broadcaster, Document, Id );
}

static void Register_(
	bridge_functions__ &Functions,
	command__ &Command,
	nsIDOMDocument *Document,
	const char *Id )
{
	ui_base::Register( Functions, Command, Document, Id );
}

static void Register_(
	bridge_functions__ &Functions,
	tree__ &Tree,
	nsIDOMDocument *Document,
	const char *Id )
{
	ui_base::Register( Functions, Tree, Document, Id );
}

static void Register_(
	bridge_functions__ &Functions,
	deck__ &Deck,
	nsIDOMDocument *Document,
	const char *Id )
{
	ui_base::Register( Functions, Deck, Document, Id );
}

static void Register_(
	bridge_functions__ &Functions,
	textbox__ &Textbox,
	nsIDOMDocument *Document,
	const char *Id )
{
	ui_base::Register( Functions, Textbox, Document, Id );
}

static void Register_(
	bridge_functions__ &Functions,
	button__ &Button,
	nsIDOMDocument *Document,
	const char *Id )
{
	ui_base::Register( Functions, Button, Document, Id );
}

/* 'broadcaster's */

static void Register_(
	bridge_functions__ &Functions,
	ui_struct::structure__::broadcasters__::database__ &UI,
	nsIDOMDocument *Document )
{
	Register_( Functions, UI.Modification, Document, "bcrDatabaseModification" );
	Register_( Functions, UI.Deletion, Document, "bcrDatabaseDeletion" );
}

static void Register_(
	bridge_functions__ &Functions,
	ui_struct::structure__::broadcasters__::table__ &UI,
	nsIDOMDocument *Document )
{
	Register_( Functions, UI.Creation, Document, "bcrTableModification" );
	Register_( Functions, UI.Modification, Document, "bcrTableModification" );
	Register_( Functions, UI.Deletion, Document, "bcrTableDeletion" );
}

static void Register_(
	bridge_functions__ &Functions,
	ui_struct::structure__::broadcasters__::field__ &UI,
	nsIDOMDocument *Document )
{
	Register_( Functions, UI.Creation, Document, "bcrFieldCreation" );
	Register_( Functions, UI.Modification, Document, "bcrFieldModification" );
	Register_( Functions, UI.Deletion, Document, "bcrFieldDeletion" );
}

static void Register_(
	bridge_functions__ &Functions,
	ui_struct::structure__::broadcasters__ &UI,
	nsIDOMDocument *Document )
{
	Register_( Functions, UI.ItemEdition, Document, "bcrItemEdition" );
	Register_( Functions, UI.ItemBrowsing, Document, "bcrItemBrowsing" );
	Register_( Functions, UI.Database, Document );
	Register_( Functions, UI.Table, Document );
	Register_( Functions, UI.Field, Document );
}

/* 'command's */

static void Register_(
	bridge_functions__ &Functions,
	ui_struct::structure__::commands__::database__ &UI,
	nsIDOMDocument *Document )
{
	Register_( Functions, UI.Modify, Document, "cmdModifyDatabase" );
	Register_( Functions, UI.Delete, Document, "cmdDeleteDatabase" );
}

static void Register_(
	bridge_functions__ &Functions,
	ui_struct::structure__::commands__::table__ &UI,
	nsIDOMDocument *Document )
{
	Register_( Functions, UI.Create, Document, "cmdCreateTable" );
	Register_( Functions, UI.Modify, Document, "cmdModifyTable" );
	Register_( Functions, UI.Delete, Document, "cmdDeleteTable" );
}

static void Register_(
	bridge_functions__ &Functions,
	ui_struct::structure__::commands__::field__ &UI,
	nsIDOMDocument *Document )
{
	Register_( Functions, UI.Create, Document, "cmdCreateField" );
	Register_( Functions, UI.Modify, Document, "cmdModifyField" );
	Register_( Functions, UI.Delete, Document, "cmdDeleteField" );
}

static void Register_(
	bridge_functions__ &Functions,
	ui_struct::structure__::commands__::item__ &UI,
	nsIDOMDocument *Document )
{
	Register_( Functions, UI.Apply, Document, "cmdApplyItem" );
	Register_( Functions, UI.Cancel, Document, "cmdCancelItem" );
}

static void Register_(
	bridge_functions__ &Functions,
	ui_struct::structure__::commands__ &UI,
	nsIDOMDocument *Document )
{
	Register_( Functions, UI.Database, Document );
	Register_( Functions, UI.Table, Document );
	Register_( Functions, UI.Field, Document );
	Register_( Functions, UI.Item, Document );
}

static void Register_(
	bridge_functions__ &Functions,
	ui_struct::structure__ &UI )
{
	UI.Items = nsxpcm::GetElementById( UI.Document, "tchItems" );

	Register_( Functions, UI.BrowseTree, UI.Document, "treBrowse" );

	Register_( Functions, UI.Broadcasters, UI.Document );
	Register_( Functions, UI.Commands, UI.Document );

	Register_( Functions, UI.ActionDeck, UI.Document, "itemActionDeck" );

	UI.DatabaseSelectionPanel = nsxpcm::GetElementById( UI.Document, "databaseSelectionPanel" );
	UI.TableSelectionPanel = nsxpcm::GetElementById( UI.Document, "tableSelectionPanel" );
	UI.FieldSelectionPanel = nsxpcm::GetElementById( UI.Document, "fieldSelectionPanel" );

	Register_( Functions, UI.FormDeck, UI.Document, "formDeck" );

	Register_( Functions, UI.NameTextbox, UI.Document, "txbName" );
	Register_( Functions, UI.CommentTextbox, UI.Document, "txbComment" );

	UI.DatabaseFormPanel = nsxpcm::GetElementById( UI.Document, "databaseFormPanel" );
	UI.TableFormPanel = nsxpcm::GetElementById( UI.Document, "tableFormPanel" );
	UI.FieldFormPanel = nsxpcm::GetElementById( UI.Document, "fieldFormPanel" );
}

void ui_struct::RegisterStructureUI(
	ui::ui___ &UI,
	nsIDOMWindow *Window )
{
	UI.Structure.Init( UI, Window );

	Register_( UI, UI.Structure );
}
