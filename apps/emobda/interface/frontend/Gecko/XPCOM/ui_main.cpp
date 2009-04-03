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
	K().ApplyDatabase();
}

void ui_main::open_database_command__::NSXPCMOnEvent( event__ )
{
	K().BrowseDatabase();
}

void ui_main::define_database_command__::NSXPCMOnEvent( event__ )
{
	K().DefineDatabase();
}

void ui_main::close_database_command__::NSXPCMOnEvent( event__ )
{
	nsxpcm::Alert( K().UI.Structure.Window, "Close Database !" );
}

void ui_main::table_menu_item__::NSXPCMOnEvent( event__ ) 
{
	str::string Row;
	epeios::row__ Error = NONE;
	kernel::table__ Table;

	Row.Init();
	nsxpcm::GetAttribute( GetElement(), "Row", Row );

	*Table = Row.ToUL( &Error );

	if ( Error != NONE )
		ERRc();

	K().SelectTable( Table );

}

void ui_main::define_record_command__::NSXPCMOnEvent( event__ )
{
	K().DefineRecord();
}

void ui_main::browse_record_command__::NSXPCMOnEvent( event__ )
{
	K().BrowseRecord();
}

/* Registrations */

/* 'broadcaster's */

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
	main__::broadcasters__ &UI,
	nsIDOMDocument *Document )
{
	Register_( Kernel, UI.DatabaseOpened, Document, "bcrDatabaseOpened" );
	Register_( Kernel, UI.TableSelected, Document, "bcrTableSelected" );
	Register_( Kernel, UI.RecordSelected, Document, "bcrRecordSelected" );
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

static void Register_(
	kernel___ &Kernel,
	main__::commands__::database__ &UI,
	nsIDOMDocument *Document )
{
	Register_( Kernel, UI.Create, Document, "cmdCreateDatabase" );
	Register_( Kernel, UI.Open, Document, "cmdOpenDatabase" );
	Register_( Kernel, UI.Define, Document, "cmdDefineDatabase" );
	Register_( Kernel, UI.Close, Document, "cmdCloseDatabase" );
}

static void Register_(
	kernel___ &Kernel,
	main__::commands__::record__ &UI,
	nsIDOMDocument *Document )
{
	Register_( Kernel, UI.Defined, Document, "cmdDefineRecord" );
	Register_( Kernel, UI.Browse, Document, "cmdBrowseRecord" );
}

static void Register_(
	kernel___ &Kernel,
	main__::commands__ &UI,
	nsIDOMDocument *Document )
{
	Register_( Kernel, UI.Database, Document );
	Register_( Kernel, UI.Record, Document );
}

/* 'panel's */

static void Register_(
	kernel___ &Kernel,
	panel__ &Panel,
	nsIDOMDocument *Document,
	const char *Id )
{
	ui_base::Register( Kernel, Panel, Document, Id, nsxpcm::efNone );
}

static void Register_(
	kernel___ &Kernel,
	main__::panels__ &UI,
	nsIDOMDocument *Document )
{
	Register_( Kernel, UI.StructureFormAndView, Document, "pnlStructureFormAndView" );
	Register_( Kernel, UI.ListView, Document, "pnlListView" );
	Register_( Kernel, UI.RecordForm, Document, "pnlRecordForm" );
	Register_( Kernel, UI.RecordView, Document, "pnlRecordView" );
}


void ui_main::Register(
	kernel___ &Kernel,
	ui_main::main__ &UI,
	nsIDOMWindow *Window )
{
	UI.Set( Window );

	Register_( Kernel, UI.Broadcasters, UI.Document );
	Register_( Kernel, UI.Commands, UI.Document );

	ui_base::Register( Kernel, UI.TableMenu, UI.Document, "mnuTable", nsxpcm::efNone );
	ui_base::Register( Kernel, UI.MainDeck, UI.Document, "dckMain", nsxpcm::efNone );

	Register_( Kernel, UI.Panels, UI.Document );
}
