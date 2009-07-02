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

void ui_struct::modify_database_command__::NSXPCMOnEvent( event__ )
{
}

void ui_struct::delete_database_command__::NSXPCMOnEvent( event__ )
{
}

void ui_struct::create_table_command__::NSXPCMOnEvent( event__ )
{
	UI().K().StructureManagement().SetState( mbdkernl::smsCreation );
	UI().K().StructureManagement().Target.reset();
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
	UI().K().StructureManagement().Target.Set( UNDEFINED_FIELD );
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
	UI.Items = nsxpcm::GetElementById( UI.Document, "items" );

	Register_( Functions, UI.BrowseTree, UI.Document, "browseTree" );

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
