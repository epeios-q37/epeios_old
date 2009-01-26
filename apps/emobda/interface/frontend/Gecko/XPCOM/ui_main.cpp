/*
	'ui_main.cpp' by Claude SIMON (http://zeusw.org/).

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

#include "ui_main.h"
#include "kernel.h"

using namespace ui_main;
using kernel::kernel___;

using nsxpcm::event__;

void ui_main::create_database_command__::NSXPCMOnEvent( event__ )
{
//	nsxpcm::Alert( K().UI.Structure.Window, "Create Database !" );
	K().CreateDatabase( str::string( "h:\\temp\\emobda" ), str::string( "Ceci est le nom de la base de données !" ) );
	K().RefreshStructureView();
	K().UI.Structure.Broadcasters.StructureBrowsing.Enable();
	K().UI.Structure.Broadcasters.ItemEdition.Disable();
}

void ui_main::open_database_command__::NSXPCMOnEvent( event__ )
{
	nsxpcm::Alert( K().UI.Structure.Window, "Open Database !" );
}

void ui_main::close_database_command__::NSXPCMOnEvent( event__ )
{
	nsxpcm::Alert( K().UI.Structure.Window, "Close Database !" );
}

void ui_main::table_location_textbox__::NSXPCMOnEvent( event__ )
{
	K().CreateTable();
}

void ui_main::create_table_button__::NSXPCMOnEvent( event__ )
{
	K().CreateTable();
}

void ui_main::field_name_textbox__::NSXPCMOnEvent( event__ )
{
	K().AddField();
}

void ui_main::add_field_button__::NSXPCMOnEvent( event__ )
{
	K().AddField();
}

void ui_main::field_list_listbox__::NSXPCMOnEvent( event__ )
{
ERRProlog
	str::string Value;
ERRBegin
	Value.Init();
	nsxpcm::Alert( K().UI.Main.Window, K().UI.Main.FieldListListbox.GetCurrentItemAttribute( "FieldId", Value ) );
ERRErr
ERREnd
ERREpilog
}

void ui_main::remove_field_button__::NSXPCMOnEvent( event__ )
{
ERRProlog
	str::string Value;
ERRBegin
	if ( K().UI.Main.FieldListListbox.GetSelectedCount() == 0 )
		nsxpcm::Alert( K().UI.Main.Window, "No item selected" );
	else {
		Value.Init();
		nsxpcm::Alert( K().UI.Main.Window, K().UI.Main.FieldListListbox.GetCurrentItemAttribute( "FieldId", Value ) );
	}
ERRErr
ERREnd
ERREpilog
}

/* Registrations */

static void Register_(
	kernel___ &Kernel,
	broadcaster__ &Broadcaster,
	nsIDOMDocument *Document,
	const char *Id )
{
	ui_base::Register( Kernel, Broadcaster, Document, Id, nsxpcm::efNone );
}

/* 'broadcaster's */

static void Register_(
	kernel___ &Kernel,
	main__::broadcasters__ &UI,
	nsIDOMDocument *Document )
{
	Register_( Kernel, UI.DatabaseOpened, Document, "bcrDatabaseOpened" );
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
	button__ &Button,
	nsIDOMDocument *Document,
	const char *Id )
{
	ui_base::Register( Kernel, Button, Document, Id, nsxpcm::efCommand );
}

static void Register_(
	kernel___ &Kernel,
	textbox__ &Textbox,
	nsIDOMDocument *Document,
	const char *Id )
{
	ui_base::Register( Kernel, Textbox, Document, Id, nsxpcm::efCommand );
}

static void Register_(
	kernel___ &Kernel,
	listbox__ &Listbox,
	nsIDOMDocument *Document,
	const char *Id )
{
	ui_base::Register( Kernel, Listbox, Document, Id, nsxpcm::efSelect );
}

/* 'command's */

static void Register_(
	kernel___ &Kernel,
	main__::commands__::database__ &UI,
	nsIDOMDocument *Document )
{
	Register_( Kernel, UI.Create, Document, "cmdCreateDatabase" );
	Register_( Kernel, UI.Open, Document, "cmdOpenDatabase" );
	Register_( Kernel, UI.Close, Document, "cmdCloseDatabase" );
}

static void Register_(
	kernel___ &Kernel,
	main__::commands__ &UI,
	nsIDOMDocument *Document )
{
	Register_( Kernel, UI.Database, Document );
}


void ui_main::Register(
	kernel___ &Kernel,
	ui_main::main__ &UI,
	nsIDOMWindow *Window )
{
	UI.Set( Window );

	Register_( Kernel, UI.Broadcasters, UI.Document );
	Register_( Kernel, UI.Commands, UI.Document );

	Register_( Kernel, UI.TableLocationTextbox, UI.Document, "tableLocation" );
	Register_( Kernel, UI.CreateTableButton, UI.Document, "createTable" );

	Register_( Kernel, UI.FieldNameTextbox, UI.Document, "fieldName" );
	Register_( Kernel, UI.AddFieldButton, UI.Document, "addField" );

	Register_( Kernel, UI.FieldListListbox, UI.Document, "fieldList" );
	Register_( Kernel, UI.RemoveFieldButton, UI.Document, "removeField" );
}
