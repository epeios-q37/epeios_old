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

using kernel::message_id__;

void ui_struct::structure__::UpdateDecks( void )
{
ERRProlog
	str::string Type;
	str::string Name;
ERRBegin
	structure__ &UI = K().UI.Structure;

	if ( UI.BrowseTree.GetCurrentIndex() != -1 ) {
		Type.Init();
		UI.BrowseTree.GetCurrentItemAttribute( "Type", Type );

		UI.Broadcasters.StructureItemCreation.Enable();
		UI.Broadcasters.StructureItemModification.Enable();
		UI.Broadcasters.StructureItemDeletion.Enable();

		Name.Init();
		UI.BrowseTree.GetCurrentItemAttribute( "Name", Name );
		UI.NameTextbox.SetValue( Name );

		if ( Type == "Database" ) {
			UI.Broadcasters.StructureItemModification.SetLabel( K().GetMessage( kernel::miModifyDatabase ) );
			UI.Broadcasters.StructureItemDeletion.SetLabel( K().GetMessage( kernel::miDeleteDatabase ) );
			UI.Broadcasters.StructureItemCreation.SetLabel( K().GetMessage( kernel::miCreateTable ) );
			UI.FormDeck.SetSelectedPanel( UI.DatabaseFormPanel );
		} else if ( Type == "Table" ) {
			UI.Broadcasters.StructureItemModification.SetLabel( K().GetMessage( kernel::miModifyTable ) );
			UI.Broadcasters.StructureItemDeletion.SetLabel( K().GetMessage( kernel::miDeleteTable ) );
			UI.Broadcasters.StructureItemCreation.SetLabel( K().GetMessage( kernel::miCreateField ) );
			UI.FormDeck.SetSelectedPanel( UI.TableFormPanel );
		}else if ( Type == "Field" ) {
			UI.Broadcasters.StructureItemModification.SetLabel( K().GetMessage( kernel::miModifyField ) );
			UI.Broadcasters.StructureItemDeletion.SetLabel( K().GetMessage( kernel::miDeleteField ) );
			UI.Broadcasters.StructureItemCreation.SetLabel( K().GetMessage( kernel::miCreateField ) );
			UI.FormDeck.SetSelectedPanel( UI.FieldFormPanel );
		} else
			ERRu();
	} else {
		UI.Broadcasters.StructureItemCreation.Disable();
		UI.Broadcasters.StructureItemModification.Disable();
		UI.Broadcasters.StructureItemDeletion.Disable();
		UI.FormDeck.SetSelectedIndex( -1 );
	}

ERRErr
ERREnd
ERREpilog
}

void ui_struct::create_structure_item_command__::NSXPCMOnEvent( event__ )
{
	nsxpcm::Alert( K().UI.Structure.Window, "Create !" );

	K().UI.Structure.Broadcasters.StructureItemBrowsing.Disable();
	K().UI.Structure.Broadcasters.StructureItemEdition.Enable();
}

void ui_struct::modify_structure_item_command__::NSXPCMOnEvent( event__ )
{
	nsxpcm::Alert( K().UI.Structure.Window, "Modify !" );
}

void ui_struct::delete_structure_item_command__::NSXPCMOnEvent( event__ )
{
	nsxpcm::Alert( K().UI.Structure.Window, "Delete !" );
}

void ui_struct::apply_structure_item_command__::NSXPCMOnEvent( event__ )
{
ERRProlog
	str::string Buffer;
ERRBegin
	Buffer.Init();

	K().CreateTable( K().UI.Structure.NameTextbox.GetValue( Buffer ) );
	K().RefreshStructureView();
	K().UI.Structure.Broadcasters.StructureItemBrowsing.Enable();
	K().UI.Structure.Broadcasters.StructureItemEdition.Disable();
ERRErr
ERREnd
ERREpilog
}

void ui_struct::cancel_structure_item_command__::NSXPCMOnEvent( event__ )
{
	nsxpcm::Alert( K().UI.Structure.Window, "Cancel !" );
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
	ui_struct::structure__::broadcasters__ &UI,
	nsIDOMDocument *Document )
{
	Register_( Kernel, UI.StructureItemEdition, Document, "bcrStructureItemEdition" );
	Register_( Kernel, UI.StructureItemBrowsing, Document, "bcrStructureItemBrowsing" );
	Register_( Kernel, UI.StructureItemCreation, Document, "bcrStructureItemCreation" );
	Register_( Kernel, UI.StructureItemModification, Document, "bcrStructureItemModification" );
	Register_( Kernel, UI.StructureItemDeletion, Document, "bcrStructureItemDeletion" );
}

/* 'command's */

static void Register_(
	kernel___ &Kernel,
	ui_struct::structure__::commands__ &UI,
	nsIDOMDocument *Document )
{
	Register_( Kernel, UI.CreateStructureItem, Document, "cmdCreateStructureItem" );
	Register_( Kernel, UI.ModifyStructureItem, Document, "cmdModifyStructureItem" );
	Register_( Kernel, UI.DeleteStructureItem, Document, "cmdDeleteStructureItem" );

	Register_( Kernel, UI.ApplyStructureItem, Document, "cmdApplyStructureItem" );
	Register_( Kernel, UI.CancelStructureItem, Document, "cmdCancelStructureItem" );

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

	Register_( Kernel, UI.NameTextbox, UI.Document, "nameTextbox" );

	UI.DatabaseFormPanel = nsxpcm::GetElementById( UI.Document, "databaseFormPanel" );
	UI.TableFormPanel = nsxpcm::GetElementById( UI.Document, "tableFormPanel" );
	UI.FieldFormPanel = nsxpcm::GetElementById( UI.Document, "fieldFormPanel" );
}
