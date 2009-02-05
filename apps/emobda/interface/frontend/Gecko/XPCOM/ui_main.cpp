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
	K().CreateDatabase( str::string( "h:\\temp\\emobda" ), str::string( "Ceci est le nom de la base de données !" ), str::string( "Ceci est le commentaire de la basqe de données !" ) );
	K().RefreshStructureView();
	K().UI.Structure.Broadcasters.StructureItemBrowsing.Enable();
	K().UI.Structure.Broadcasters.StructureItemEdition.Disable();
}

void ui_main::open_database_command__::NSXPCMOnEvent( event__ )
{
	nsxpcm::Alert( K().UI.Structure.Window, "Open Database !" );
}

void ui_main::close_database_command__::NSXPCMOnEvent( event__ )
{
	nsxpcm::Alert( K().UI.Structure.Window, "Close Database !" );
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
}
