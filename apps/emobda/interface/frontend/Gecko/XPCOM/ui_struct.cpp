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
using frdkernl::target__;

using namespace kernel;

bso::bool__ ui_struct::structure__::GetSelectedItem( target__ &Target )
{
	bso::bool__ Selected = false;
ERRProlog
	str::string Type, Row, TableRow;
	table__ Table = UNDEFINED_TABLE;
	field__ Field = UNDEFINED_FIELD;
ERRBegin

	if ( BrowseTree.IsThereSelected() ) {
		Type.Init();
		BrowseTree.GetCurrentItemAttribute( "Type", Type );

		if ( Type == "Field" ) {
			Row.Init();
			BrowseTree.GetCurrentItemAttribute( "Row", Row );
			Field = frdkernl::ExtractField( Row );

			TableRow.Init();
			BrowseTree.GetCurrentItemAttribute( "TableRow", TableRow );
			Table = frdkernl::ExtractTable( TableRow );
		} else if ( Type == "Table" ) {
			Row.Init();
			BrowseTree.GetCurrentItemAttribute( "Row", Row );
			Table = frdkernl::ExtractTable( Row );
		} else if ( Type != "Database" )
			ERRc();

		Target.Set( Field, Table );
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
	frdkernl::kernel___ &Kernel = UI()._K();

	Name.Init();
	Comment.Init();

	Target = Kernel.Target();

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
	frdkernl::kernel___ &Kernel = UI()._K();

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

			Table = frdkernl::ExtractTable( Row );

			if ( !Kernel.GetTableInfos( Table, Name, Comment, TableId ) )
				ERRReturn;
		} else if ( Type == "Field" ) {
			field_id__ FieldId = UNDEFINED_FIELD_ID;
			
			Row.Init();
			BrowseTree.GetCurrentItemAttribute( "Row", Row );

			Field = frdkernl::ExtractField( Row );

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
	frdkernl::kernel___ &Kernel = UI()._K();

	Target = Kernel.Target();

	if ( Kernel.GetTransientContext() != frdtrnsnt::cStructureManagement )
		GetSelectedItem( Target );

	XML.Init();

	Kernel.DumpAsXML( frdkernl::xdfStructure | frdkernl::xdfAllCurrent, XML );

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
	UI().Alert( frdkernl::mNotImplementedYet );
}

void ui_struct::delete_database_command__::NSXPCMOnEvent( event__ )
{
	UI().Alert( frdkernl::mNotImplementedYet );
}

void ui_struct::create_table_command__::NSXPCMOnEvent( event__ )
{
	UI().K().StructureManagement().SetState( frdkernl::smsCreation );
	UI().K().Target().Set( UNDEFINED_TABLE );
	UI().DefineTable();
}

void ui_struct::modify_table_command__::NSXPCMOnEvent( event__ )
{
	UI().K().StructureManagement().SetState( frdkernl::smsModification );
	UI().DefineTable();
}

void ui_struct::delete_table_command__::NSXPCMOnEvent( event__ )
{
	if ( UI().Confirm( frdkernl::mDeleteTableConfirmation ) ) {
		UI().DeleteTable();
	}
}

void ui_struct::create_field_command__::NSXPCMOnEvent( event__ )
{
	UI().K().StructureManagement().SetState( frdkernl::smsCreation );
	UI().K().Target().Set( UNDEFINED_FIELD );
	UI().DefineField();
}

void ui_struct::modify_field_command__::NSXPCMOnEvent( event__ )
{
	UI().K().StructureManagement().SetState( frdkernl::smsModification );
	UI().DefineField();
}

void ui_struct::delete_field_command__::NSXPCMOnEvent( event__ )
{
	if ( UI().Confirm( frdkernl::mDeleteFieldConfirmation ) ) {
		UI().DeleteField();
	}
}

void ui_struct::apply_item_command__::NSXPCMOnEvent( event__ )
{
	UI().ApplyStructureItem();
}

void ui_struct::cancel_item_command__::NSXPCMOnEvent( event__ )
{
	if ( UI().Confirm( frdkernl::mDropStructureItemConfirmation ) ) {
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
			UI().K().StructureManagement().SetState( frdkernl::smsModification );
			UI().DefineStructureItem();
			break;
		default:
			ERRc();
			break;
	}
}

/* UI Registrations */

static void Register_(
	ui___ &UI,
	broadcaster__<ui___> &Broadcaster,
	nsIDOMDocument *Document,
	const char *Id )
{
	ui_base::Register( UI, Broadcaster, Document, Id );
}

static void Register_(
	ui___ &UI,
	command__<ui___> &Command,
	nsIDOMDocument *Document,
	const char *Id )
{
	ui_base::Register( UI, Command, Document, Id );
}

static void Register_(
	ui___ &UI,
	tree__<ui___> &Tree,
	nsIDOMDocument *Document,
	const char *Id )
{
	ui_base::Register( UI, Tree, Document, Id );
}

static void Register_(
	ui___ &UI,
	deck__<ui___> &Deck,
	nsIDOMDocument *Document,
	const char *Id )
{
	ui_base::Register( UI, Deck, Document, Id );
}

static void Register_(
	ui___ &UI,
	textbox__<ui___> &Textbox,
	nsIDOMDocument *Document,
	const char *Id )
{
	ui_base::Register( UI, Textbox, Document, Id );
}

static void Register_(
	ui___ &UI,
	button__<ui___> &Button,
	nsIDOMDocument *Document,
	const char *Id )
{
	ui_base::Register( UI, Button, Document, Id );
}

/* 'broadcaster's */

static void Register_(
	ui___ &UI,
	ui_struct::structure__::broadcasters__::database__ &DUI,
	nsIDOMDocument *Document )
{
	Register_( UI, DUI.Modification, Document, "bcrDatabaseModification" );
	Register_( UI, DUI.Deletion, Document, "bcrDatabaseDeletion" );
}

static void Register_(
	ui___ &UI,
	ui_struct::structure__::broadcasters__::table__ &TUI,
	nsIDOMDocument *Document )
{
	Register_( UI, TUI.Creation, Document, "bcrTableModification" );
	Register_( UI, TUI.Modification, Document, "bcrTableModification" );
	Register_( UI, TUI.Deletion, Document, "bcrTableDeletion" );
}

static void Register_(
	ui___ &UI,
	ui_struct::structure__::broadcasters__::field__ &FUI,
	nsIDOMDocument *Document )
{
	Register_( UI, FUI.Creation, Document, "bcrFieldCreation" );
	Register_( UI, FUI.Modification, Document, "bcrFieldModification" );
	Register_( UI, FUI.Deletion, Document, "bcrFieldDeletion" );
}

static void Register_(
	ui___ &UI,
	ui_struct::structure__::broadcasters__ &BUI,
	nsIDOMDocument *Document )
{
	Register_( UI, BUI.ItemEdition, Document, "bcrItemEdition" );
	Register_( UI, BUI.ItemBrowsing, Document, "bcrItemBrowsing" );
	Register_( UI, BUI.Database, Document );
	Register_( UI, BUI.Table, Document );
	Register_( UI, BUI.Field, Document );
}

/* 'command's */

static void Register_(
	ui___ &UI,
	ui_struct::structure__::commands__::database__ &DUI,
	nsIDOMDocument *Document )
{
	Register_( UI, DUI.Modify, Document, "cmdModifyDatabase" );
	Register_( UI, DUI.Delete, Document, "cmdDeleteDatabase" );
}

static void Register_(
	ui___ &UI,
	ui_struct::structure__::commands__::table__ &TUI,
	nsIDOMDocument *Document )
{
	Register_( UI, TUI.Create, Document, "cmdCreateTable" );
	Register_( UI, TUI.Modify, Document, "cmdModifyTable" );
	Register_( UI, TUI.Delete, Document, "cmdDeleteTable" );
}

static void Register_(
	ui___ &UI,
	ui_struct::structure__::commands__::field__ &FUI,
	nsIDOMDocument *Document )
{
	Register_( UI, FUI.Create, Document, "cmdCreateField" );
	Register_( UI, FUI.Modify, Document, "cmdModifyField" );
	Register_( UI, FUI.Delete, Document, "cmdDeleteField" );
}

static void Register_(
	ui___ &UI,
	ui_struct::structure__::commands__::item__ &IUI,
	nsIDOMDocument *Document )
{
	Register_( UI, IUI.Apply, Document, "cmdApplyItem" );
	Register_( UI, IUI.Cancel, Document, "cmdCancelItem" );
}

static void Register_(
	ui___ &UI,
	ui_struct::structure__::commands__ &CUI,
	nsIDOMDocument *Document )
{
	Register_( UI, CUI.Database, Document );
	Register_( UI, CUI.Table, Document );
	Register_( UI, CUI.Field, Document );
	Register_( UI, CUI.Item, Document );
}

static void Register_(
	ui___ &UI,
	ui_struct::structure__ &SUI )
{
	SUI.Items = nsxpcm::GetElementById( SUI.Document, "tchItems" );

	Register_( UI, SUI.BrowseTree, SUI.Document, "treBrowse" );

	Register_( UI, SUI.Broadcasters, SUI.Document );
	Register_( UI, SUI.Commands, SUI.Document );

	Register_( UI, SUI.ActionDeck, SUI.Document, "itemActionDeck" );

	SUI.DatabaseSelectionPanel = nsxpcm::GetElementById( SUI.Document, "databaseSelectionPanel" );
	SUI.TableSelectionPanel = nsxpcm::GetElementById( SUI.Document, "tableSelectionPanel" );
	SUI.FieldSelectionPanel = nsxpcm::GetElementById( SUI.Document, "fieldSelectionPanel" );

	Register_( UI, SUI.FormDeck, SUI.Document, "formDeck" );

	Register_( UI, SUI.NameTextbox, SUI.Document, "txbName" );
	Register_( UI, SUI.CommentTextbox, SUI.Document, "txbComment" );

	SUI.DatabaseFormPanel = nsxpcm::GetElementById( SUI.Document, "databaseFormPanel" );
	SUI.TableFormPanel = nsxpcm::GetElementById( SUI.Document, "tableFormPanel" );
	SUI.FieldFormPanel = nsxpcm::GetElementById( SUI.Document, "fieldFormPanel" );
}

void ui_struct::RegisterStructureUI(
	ui___ &UI,
	nsIDOMWindow *Window )
{
	UI.Structure.Init( UI, Window );

	Register_( UI, UI.Structure );
}
