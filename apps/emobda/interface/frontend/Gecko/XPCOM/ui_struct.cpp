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

using namespace kernel;

void ui_struct::structure__::UpdateDecks( void )
{
ERRProlog
	str::string Type;
	str::string Name, Comment;
	str::string Row;
	epeios::row__ Error = NONE;
ERRBegin
	structure__ &UI = K().UI.Structure;

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
			
			Row.Init();
			UI.BrowseTree.GetCurrentItemAttribute( "Row", Row );

			K().GetFieldInfo( ExtractField( Row ), Name, Comment, FieldId );
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
			UI.Broadcasters.Field.Creation.Enable();

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

void ui_struct::modify_database_command__::NSXPCMOnEvent( event__ )
{
}

void ui_struct::delete_database_command__::NSXPCMOnEvent( event__ )
{
}

void ui_struct::create_table_command__::NSXPCMOnEvent( event__ )
{
	K().SetCurrentItems( UNDEFINED_FIELD, UNDEFINED_TABLE );
	K().DefineTable();
}

void ui_struct::modify_table_command__::NSXPCMOnEvent( event__ )
{
}

void ui_struct::delete_table_command__::NSXPCMOnEvent( event__ )
{
}

void ui_struct::create_field_command__::NSXPCMOnEvent( event__ )
{
	K().SetCurrentItems( UNDEFINED_FIELD, K().GetCurrentTable() );
	K().DefineField();
}

void ui_struct::modify_field_command__::NSXPCMOnEvent( event__ )
{
}

void ui_struct::delete_field_command__::NSXPCMOnEvent( event__ )
{
}

void ui_struct::apply_item_command__::NSXPCMOnEvent( event__ )
{
ERRProlog
	str::string NameBuffer, CommentBuffer;
ERRBegin
	NameBuffer.Init();
	CommentBuffer.Init();

	if ( K().GetCurrentTable() == UNDEFINED_TABLE )
		K().CreateTable();
	else if ( K().GetCurrentField() == UNDEFINED_FIELD )
		K().AddField();
	else
		ERRc();

	K().RefreshStructureView();
	K().UI.Structure.Broadcasters.ItemBrowsing.Enable();
	K().UI.Structure.Broadcasters.ItemEdition.Disable();
ERRErr
ERREnd
ERREpilog
}

void ui_struct::cancel_item_command__::NSXPCMOnEvent( event__ )
{
	nsxpcm::Alert( K().UI.Structure.Window, "Cancel !" );
}

void ui_struct::browse_tree__::NSXPCMOnEvent( event__ Event )
{
	K().SetCurrentItems();
	K().UI.Structure.UpdateDecks();
}

/* UI Registrations */

static void Register_(
	kernel___ &Kernel,
	broadcaster__ &Broadcaster,
	nsIDOMDocument *Document,
	const char *Id )
{
	ui_base::Register( Kernel, Broadcaster, Document, Id, nsxpcm::efNone );
}

static void Register_(
	kernel___ &Kernel,
	command__ &Command,
	nsIDOMDocument *Document,
	const char *Id )
{
	ui_base::Register( Kernel, Command, Document, Id, nsxpcm::efCommand );
}

static void Register_(
	kernel___ &Kernel,
	tree__ &Tree,
	nsIDOMDocument *Document,
	const char *Id )
{
	ui_base::Register( Kernel, Tree, Document, Id, nsxpcm::efSelect );
}

static void Register_(
	kernel___ &Kernel,
	deck__ &Deck,
	nsIDOMDocument *Document,
	const char *Id )
{
	ui_base::Register( Kernel, Deck, Document, Id, nsxpcm::efNone );
}

static void Register_(
	kernel___ &Kernel,
	textbox__ &Textbox,
	nsIDOMDocument *Document,
	const char *Id )
{
	ui_base::Register( Kernel, Textbox, Document, Id, nsxpcm::efNone );
}

static void Register_(
	kernel___ &Kernel,
	button__ &Button,
	nsIDOMDocument *Document,
	const char *Id )
{
	ui_base::Register( Kernel, Button, Document, Id, nsxpcm::efCommand );
}

/* 'broadcaster's */

static void Register_(
	kernel___ &Kernel,
	ui_struct::structure__::broadcasters__::database__ &UI,
	nsIDOMDocument *Document )
{
	Register_( Kernel, UI.Modification, Document, "bcrDatabaseModification" );
	Register_( Kernel, UI.Deletion, Document, "bcrDatabaseDeletion" );
}

static void Register_(
	kernel___ &Kernel,
	ui_struct::structure__::broadcasters__::table__ &UI,
	nsIDOMDocument *Document )
{
	Register_( Kernel, UI.Creation, Document, "bcrTableModification" );
	Register_( Kernel, UI.Modification, Document, "bcrTableModification" );
	Register_( Kernel, UI.Deletion, Document, "bcrTableDeletion" );
}

static void Register_(
	kernel___ &Kernel,
	ui_struct::structure__::broadcasters__::field__ &UI,
	nsIDOMDocument *Document )
{
	Register_( Kernel, UI.Creation, Document, "bcrFieldCreation" );
	Register_( Kernel, UI.Modification, Document, "bcrFieldModification" );
	Register_( Kernel, UI.Deletion, Document, "bcrFieldDeletion" );
}

static void Register_(
	kernel___ &Kernel,
	ui_struct::structure__::broadcasters__ &UI,
	nsIDOMDocument *Document )
{
	Register_( Kernel, UI.ItemEdition, Document, "bcrItemEdition" );
	Register_( Kernel, UI.ItemBrowsing, Document, "bcrItemBrowsing" );
	Register_( Kernel, UI.Database, Document );
	Register_( Kernel, UI.Table, Document );
	Register_( Kernel, UI.Field, Document );
}

/* 'command's */

static void Register_(
	kernel___ &Kernel,
	ui_struct::structure__::commands__::database__ &UI,
	nsIDOMDocument *Document )
{
	Register_( Kernel, UI.Modify, Document, "cmdModifyDatabase" );
	Register_( Kernel, UI.Delete, Document, "cmdDeleteDatabase" );
}

static void Register_(
	kernel___ &Kernel,
	ui_struct::structure__::commands__::table__ &UI,
	nsIDOMDocument *Document )
{
	Register_( Kernel, UI.Create, Document, "cmdCreateTable" );
	Register_( Kernel, UI.Modify, Document, "cmdModifyTable" );
	Register_( Kernel, UI.Delete, Document, "cmdDeleteTable" );
}

static void Register_(
	kernel___ &Kernel,
	ui_struct::structure__::commands__::field__ &UI,
	nsIDOMDocument *Document )
{
	Register_( Kernel, UI.Create, Document, "cmdCreateField" );
	Register_( Kernel, UI.Modify, Document, "cmdModifyField" );
	Register_( Kernel, UI.Delete, Document, "cmdDeleteField" );
}

static void Register_(
	kernel___ &Kernel,
	ui_struct::structure__::commands__::item__ &UI,
	nsIDOMDocument *Document )
{
	Register_( Kernel, UI.Apply, Document, "cmdApplyItem" );
	Register_( Kernel, UI.Cancel, Document, "cmdCancelItem" );
}

static void Register_(
	kernel___ &Kernel,
	ui_struct::structure__::commands__ &UI,
	nsIDOMDocument *Document )
{
	Register_( Kernel, UI.Database, Document );
	Register_( Kernel, UI.Table, Document );
	Register_( Kernel, UI.Field, Document );
	Register_( Kernel, UI.Item, Document );
}

void ui_struct::Register(
	kernel___ &Kernel,
	structure__ &UI,
	nsIDOMWindow *Window )
{
	UI.Set( Window );

	UI.Items = nsxpcm::GetElementById( UI.Document, "items" );

	Register_( Kernel, UI.BrowseTree, UI.Document, "browseTree" );

	Register_( Kernel, UI.Broadcasters, UI.Document );
	Register_( Kernel, UI.Commands, UI.Document );

	Register_( Kernel, UI.ActionDeck, UI.Document, "itemActionDeck" );

	UI.DatabaseSelectionPanel = nsxpcm::GetElementById( UI.Document, "databaseSelectionPanel" );
	UI.TableSelectionPanel = nsxpcm::GetElementById( UI.Document, "tableSelectionPanel" );
	UI.FieldSelectionPanel = nsxpcm::GetElementById( UI.Document, "fieldSelectionPanel" );

	Register_( Kernel, UI.FormDeck, UI.Document, "formDeck" );

	Register_( Kernel, UI.NameTextbox, UI.Document, "nameTextbox" );
	Register_( Kernel, UI.CommentTextbox, UI.Document, "commentTextbox" );

	UI.DatabaseFormPanel = nsxpcm::GetElementById( UI.Document, "databaseFormPanel" );
	UI.TableFormPanel = nsxpcm::GetElementById( UI.Document, "tableFormPanel" );
	UI.FieldFormPanel = nsxpcm::GetElementById( UI.Document, "fieldFormPanel" );
}
