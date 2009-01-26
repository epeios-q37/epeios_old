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

void ui_struct::structure__::UpdateDecks( void )
{
ERRProlog
	str::string Type;
ERRBegin
	structure__ &UI = K().UI.Structure;

	if ( UI.BrowseTree.GetCurrentIndex() != -1 ) {
		Type.Init();
		UI.BrowseTree.GetCurrentItemAttribute( "Type", Type );

		if ( Type == "Database" ) {
			UI.Broadcasters.DatabaseSelection.Show();
			UI.Broadcasters.TableSelection.Hide();
			UI.Broadcasters.FieldSelection.Hide();
			UI.FormDeck.SetSelectedPanel( UI.DatabaseFormPanel );
		} else if ( Type == "Table" ) {
			UI.Broadcasters.DatabaseSelection.Hide();
			UI.Broadcasters.TableSelection.Show();
			UI.Broadcasters.FieldSelection.Hide();
			UI.FormDeck.SetSelectedPanel( UI.TableFormPanel );
		}else if ( Type == "Field" ) {
			UI.Broadcasters.DatabaseSelection.Hide();
			UI.Broadcasters.TableSelection.Hide();
			UI.Broadcasters.FieldSelection.Show();
			UI.FormDeck.SetSelectedPanel( UI.FieldFormPanel );
		} else
			ERRu();
	} else {
		UI.Broadcasters.DatabaseSelection.Hide();
		UI.Broadcasters.TableSelection.Hide();
		UI.Broadcasters.FieldSelection.Hide();
		UI.FormDeck.SetSelectedIndex( -1 );
	}

ERRErr
ERREnd
ERREpilog
}

void ui_struct::structure_browsing_broadcaster__::NSXPCMOnEvent( event__ )
{
	nsxpcm::Alert( K().UI.Structure.Window, "Attribute change !" );
}

void ui_struct::rename_database_command__::NSXPCMOnEvent( event__ )
{
	nsxpcm::Alert( K().UI.Structure.Window, "Rename Database !" );
}

void ui_struct::create_table_command__::NSXPCMOnEvent( event__ )
{
	nsxpcm::Alert( K().UI.Structure.Window, "Create Table !" );
	K().UI.Structure.Broadcasters.StructureBrowsing.Disable();
	K().UI.Structure.Broadcasters.ItemEdition.Enable();
}

void ui_struct::rename_table_command__::NSXPCMOnEvent( event__ )
{
	nsxpcm::Alert( K().UI.Structure.Window, "Rename Table !" );
}

void ui_struct::delete_table_command__::NSXPCMOnEvent( event__ )
{
	nsxpcm::Alert( K().UI.Structure.Window, "Delete Table !" );
}

void ui_struct::create_field_command__::NSXPCMOnEvent( event__ )
{
	nsxpcm::Alert( K().UI.Structure.Window, "Create Field !" );
}

void ui_struct::modify_field_command__::NSXPCMOnEvent( event__ )
{
	nsxpcm::Alert( K().UI.Structure.Window, "Modify Field !" );
}

void ui_struct::delete_field_command__::NSXPCMOnEvent( event__ )
{
	nsxpcm::Alert( K().UI.Structure.Window, "Delete Field !" );
}

void ui_struct::browse_tree__::NSXPCMOnEvent( event__ )
{
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
	structure_browsing_broadcaster__ &Broadcaster,
	nsIDOMDocument *Document,
	const char *Id )
{
	ui_base::Register( Kernel, Broadcaster, Document, Id, nsxpcm::efAttributeChange );
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
	button__ &Button,
	nsIDOMDocument *Document,
	const char *Id )
{
	ui_base::Register( Kernel, Button, Document, Id, nsxpcm::efCommand );
}

/* 'broadcaster's */

static void Register_(
	kernel___ &Kernel,
	ui_struct::structure__::broadcasters__ &UI,
	nsIDOMDocument *Document )
{
	Register_( Kernel, UI.ItemEdition, Document, "bcrItemEdition" );
	Register_( Kernel, UI.StructureBrowsing, Document, "bcrStructureBrowsing" );
	Register_( Kernel, UI.DatabaseSelection,  Document, "bcrDatabaseSelection" );
	Register_( Kernel, UI.TableSelection, Document, "bcrTableSelection" );
	Register_( Kernel, UI.FieldSelection, Document, "bcrFieldSelection" );
}

/* 'command's */

static void Register_(
	kernel___ &Kernel,
	ui_struct::structure__::commands__::database__ &UI,
	nsIDOMDocument *Document )
{
	Register_( Kernel, UI.Rename, Document, "cmdRenameDatabase" );
}

static void Register_(
	kernel___ &Kernel,
	ui_struct::structure__::commands__::table__ &UI,
	nsIDOMDocument *Document )
{
	Register_( Kernel, UI.Create, Document, "cmdCreateTable" );
	Register_( Kernel, UI.Rename, Document, "cmdRenameTable" );
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
	ui_struct::structure__::commands__ &UI,
	nsIDOMDocument *Document )
{
	Register_( Kernel, UI.Database, Document );
	Register_( Kernel, UI.Table, Document );
	Register_( Kernel, UI.Field, Document );
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

	Register_( Kernel, UI.FormDeck, UI.Document, "formDeck" );

	UI.DatabaseFormPanel = nsxpcm::GetElementById( UI.Document, "databaseFormPanel" );
	UI.TableFormPanel = nsxpcm::GetElementById( UI.Document, "tableFormPanel" );
	UI.FieldFormPanel = nsxpcm::GetElementById( UI.Document, "fieldFormPanel" );

	Register_( Kernel, UI.ApplyFormButton, UI.Document, "applyFormButton" );
	Register_( Kernel, UI.CancelFormButton, UI.Document, "cancelFormButton" );
}
