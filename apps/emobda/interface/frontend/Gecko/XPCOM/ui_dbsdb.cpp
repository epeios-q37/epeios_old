/*
	'ui_dbsdb.cpp' by Claude SIMON (http://zeusw.org/).

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

#include "ui_dbsdb.h"
#include "kernel.h"

using namespace ui_dbsdb;
using kernel::kernel___;
using nsxpcm::event__;

using namespace kernel;

void ui_dbsdb::database_tree__::NSXPCMOnEvent( event__ Event )
{
	switch ( Event ) {
		case nsxpcm::eSelect:
			K().Alert( "Select !" );
			break;
		case nsxpcm::eDblClick:
			K().Alert( "DblClickj !" );
			break;
		default:
			ERRc();
			break;
	}
}

void ui_dbsdb::apply_command__::NSXPCMOnEvent( event__ )
{
	K().Alert( "OK !" );
}

void ui_dbsdb::cancel_command__::NSXPCMOnEvent( event__ )
{
	K().Alert( "Cancel !" );
}

/* UI Registrations */

static void Register_(
	kernel___ &Kernel,
	broadcaster__ &Broadcaster,
	nsIDOMDocument *Document,
	const char *Id )
{
	ui_base::Register( Kernel, Broadcaster, Document, Id );
}

static void Register_(
	kernel___ &Kernel,
	command__ &Command,
	nsIDOMDocument *Document,
	const char *Id )
{
	ui_base::Register( Kernel, Command, Document, Id );
}

static void Register_(
	kernel___ &Kernel,
	database_selection__::broadcasters__ &UI,
	nsIDOMDocument *Document )
{
	Register_( Kernel, UI.DatabaseSelection, Document, "bcrDatabaseSelection" );
}

static void Register_(
	kernel___ &Kernel,
	database_selection__::commands__ &UI,
	nsIDOMDocument *Document )
{
	Register_( Kernel, UI.Apply, Document, "cmdApply" );
	Register_( Kernel, UI.Cancel, Document, "cmdCancel" );
}

void ui_dbsdb::Register(
	kernel::kernel___ &Kernel,
	database_selection__ &UI,
	nsIDOMWindow *Window )
{
	UI.Set( Window );

	ui_base::Register( Kernel, UI.DatabaseTree, UI.Document, "treDatabases" );

	Register_( Kernel, UI.Broadcasters, UI.Document );
	Register_( Kernel, UI.Commands, UI.Document );
}


