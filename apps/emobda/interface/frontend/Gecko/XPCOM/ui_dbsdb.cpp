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

void ui_dbsdb::database_selection__::ExtractSelectedDatabase( void )
{
ERRProlog
	str::string Path;
ERRBegin
	Path.Init();

	if ( !UI().DatabaseSelection.DatabaseTree.IsThereSelected() ) {
		UI().Alert( UI().DatabaseSelection.Window.GetObject(), frdkernl::mNoDatabaseSelected );
		ERRReturn;
	}

	UI().DatabaseSelection.DatabaseTree.GetCurrentItemAttribute( "Path", Path );
	
	Path.StripCharacter( ' ' );

	if ( Path.Amount() == 0 ) {
		UI().Alert( UI().DatabaseSelection.Window.GetObject(), frdkernl::mBadDatabasePath );
		ERRReturn;
	}

	UI().K().DatabaseSelection().Set( Path );
	UI().DatabaseSelection.Window.Close();
ERRErr
ERREnd
ERREpilog

}


void ui_dbsdb::window__::NSXPCMOnEvent( event__ Event )
{
	switch ( Event ) {
	case nsxpcm::eClose:
		UI().K().DatabaseSelection().SetCancelledState();
		break;
	default:
		break;
	}
}

void ui_dbsdb::database_tree__::NSXPCMOnEvent( event__ Event )
{
	switch ( Event ) {
		case nsxpcm::eSelect:
			UI().DatabaseSelection.Broadcasters.DatabaseSelection.Enable();
			break;
		case nsxpcm::eDblClick:
			UI().DatabaseSelection.ExtractSelectedDatabase();
			break;
		default:
			ERRc();
			break;
	}
}

void ui_dbsdb::apply_command__::NSXPCMOnEvent( event__ )
{
	UI().DatabaseSelection.ExtractSelectedDatabase();
}

void ui_dbsdb::cancel_command__::NSXPCMOnEvent( event__ )
{
	UI().K().DatabaseSelection().SetCancelledState();
	UI().DatabaseSelection.Window.Close();
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
	database_selection__::broadcasters__ &UI,
	nsIDOMDocument *Document )
{
	Register_( Functions, UI.DatabaseSelection, Document, "bcrDatabaseSelection" );
}

static void Register_(
	bridge_functions__ &Functions,
	database_selection__::commands__ &UI,
	nsIDOMDocument *Document )
{
	Register_( Functions, UI.Apply, Document, "cmdApply" );
	Register_( Functions, UI.Cancel, Document, "cmdCancel" );
}

static void Register_(
	bridge_functions__ &Functions,
	database_selection__ &UI )
{
	ui_base::Register( Functions, UI.DatabaseTree, UI.Document, "treDatabases" );

	Register_( Functions, UI.Broadcasters, UI.Document );
	Register_( Functions, UI.Commands, UI.Document );
}

void ui_dbsdb::RegisterDatabaseSelectionUI(
	ui::ui___ &UI,
	nsIDOMWindow *Window )
{
	UI.DatabaseSelection.Init( UI, Window );

	ui_base::Register( UI, UI.DatabaseSelection.Window, Window );

	Register_( UI, UI.DatabaseSelection );
}


