/*
	'ui_dbcdb.cpp' by Claude SIMON (http://zeusw.org/).

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

#include "ui_dbcdb.h"
#include "kernel.h"

using namespace ui_dbcdb;
using kernel::kernel___;
using nsxpcm::event__;

using namespace kernel;

void ui_dbcdb::window__::NSXPCMOnEvent( event__ Event )
{
	switch ( Event ) {
	case nsxpcm::eClose:
		UI().K().DatabaseIdentification().SetCancelledState();
		break;
	default:
		break;
	}
}

void ui_dbcdb::apply_command__::NSXPCMOnEvent( event__ )
{
ERRProlog
	str::string Name, Path, Comment;
ERRBegin
	Name.Init();
	Path.Init();
	Comment.Init();

	UI().DatabaseCreation.Textboxes.Name.GetValue( Name );
	UI().DatabaseCreation.Textboxes.Path.GetValue( Path );
	UI().DatabaseCreation.Textboxes.Comment.GetValue( Comment );

	Name.StripCharacter( ' ' );
	Path.StripCharacter( ' ' );

	if ( Name.Amount() == 0 ) {
		UI().Alert( UI().DatabaseCreation.Window.GetObject(), mbdkernl::mMissingDatabaseName );
		UI().DatabaseCreation.Textboxes.Name.Select();
	} else if ( Path.Amount() == 0 ) {
		UI().Alert( UI().DatabaseCreation.Window.GetObject(), mbdkernl::mMissingDatabasePath );
		UI().DatabaseCreation.Textboxes.Path.Select();
	} else {
		UI().K().DatabaseIdentification().Set( Name, Path, Comment );
		UI().DatabaseCreation.Window.Close();
	}
ERRErr
ERREnd
ERREpilog
}

void ui_dbcdb::cancel_command__::NSXPCMOnEvent( event__ )
{
	UI().K().DatabaseIdentification().SetCancelledState();
	UI().DatabaseCreation.Window.Close();
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
	textbox__ &Textbox,
	nsIDOMDocument *Document,
	const char *Id )
{
	ui_base::Register( Functions, Textbox, Document, Id );
}

static void Register_(
	bridge_functions__ &Functions,
	database_creation__::broadcasters__ &UI,
	nsIDOMDocument *Document )
{
	Register_( Functions, UI.DatabaseNaming, Document, "bcrDatabaseNaming" );
}

static void Register_(
	bridge_functions__ &Functions,
	database_creation__::commands__ &UI,
	nsIDOMDocument *Document )
{
	Register_( Functions, UI.Apply, Document, "cmdApply" );
	Register_( Functions, UI.Cancel, Document, "cmdCancel" );
}

static void Register_(
	bridge_functions__ &Functions,
	database_creation__::textboxes__ &UI,
	nsIDOMDocument *Document )
{
	Register_( Functions, UI.Name, Document, "txbName" );
	Register_( Functions, UI.Path, Document, "txbPath" );
	Register_( Functions, UI.Comment, Document, "txbComment" );
}

static void Register_(
	bridge_functions__ &Functions,
	database_creation__ &UI )
{
	Register_( Functions, UI.Broadcasters, UI.Document );
	Register_( Functions, UI.Commands, UI.Document );
	Register_( Functions, UI.Textboxes, UI.Document );
}

void ui_dbcdb::RegisterDatabaseCreationUI(
	ui::ui___ &UI,
	nsIDOMWindow *Window )
{
	UI.DatabaseCreation.Set( Window );

	ui_base::Register( UI, UI.DatabaseCreation.Window, Window );

	Register_( UI, UI.DatabaseCreation );
}


