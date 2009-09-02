/*
	'ui_bksdb.cpp' by Claude SIMON (http://zeusw.org/).

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

#include "ui_bksdb.h"
#include "kernel.h"

using namespace ui_bksdb;
using kernel::kernel___;
using nsxpcm::event__;

using namespace kernel;

void ui_bksdb::backend_selection__::ExtractSelectedBackend( bso::bool__ Local )
{
ERRProlog
str::string HostService;
ERRBegin
	if ( Local )
		UI().K().BackendSelection().SetLocal();
	else {
		HostService.Init();
		HostServiceTextbox.GetValue( HostService );
		UI().K().BackendSelection().SetRemote( HostService );
	} 

	UI().BackendSelection.Window.Close();
ERRErr
ERREnd
ERREpilog
}


void ui_bksdb::window__::NSXPCMOnEvent( event__ Event )
{
	switch ( Event ) {
	case nsxpcm::eClose:
		UI().K().BackendSelection().SetCancelledState();
		break;
	default:
		break;
	}
}

void ui_bksdb::host_service_textbox__::NSXPCMOnEvent( event__ )
{
}

void ui_bksdb::local_command__::NSXPCMOnEvent( event__ )
{
	UI().BackendSelection.ExtractSelectedBackend( true );
}

void ui_bksdb::remote_command__::NSXPCMOnEvent( event__ )
{
	str::string Value;

	Value.Init();

	UI().BackendSelection.HostServiceTextbox.GetValue( Value );

//	nsxpcm::AddFormHistoryEntry( str::string( "txbHostService" ), Value );	// Désactivé jusqu'à résolution pb FormHistory
	UI().BackendSelection.ExtractSelectedBackend( false );
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
	backend_selection__::commands__ &UI,
	nsIDOMDocument *Document )
{
	Register_( Functions, UI.Local, Document, "cmdLocal" );
	Register_( Functions, UI.Remote, Document, "cmdRemote" );
}

static void Register_(
	bridge_functions__ &Functions,
	backend_selection__ &UI )
{
	Register_( Functions, UI.Commands, UI.Document );
	ui_base::Register( Functions, UI.HostServiceTextbox, UI.Document, "txbHostService" );
}

void ui_bksdb::RegisterBackendSelectionUI(
	ui::ui___ &UI,
	nsIDOMWindow *Window )
{
	UI.BackendSelection.Init( UI, Window );

	ui_base::Register( UI, UI.BackendSelection.Window, Window );

	Register_( UI, UI.BackendSelection );
}
