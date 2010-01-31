/*
	'ui_dbs_f.cpp' by Claude SIMON (http://zeusw.org/).

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

#include "ui_dbs_f.h"
#include "kernel.h"

using namespace ui_dbs_f;
using kernel::kernel___;
using nsxpcm::event__;

using namespace kernel;

void ui_dbs_f::window__::NSXPCMOnEvent( event__ Event )
{
	switch ( Event ) {
	case nsxpcm::eClose:
		UI().K().DatabaseIdentification().SetCancelledState();
		break;
	default:
		break;
	}
}

void ui_dbs_f::apply_command__::NSXPCMOnEvent( event__ )
{
ERRProlog
	str::string Name, Path, Comment;
ERRBegin
	Name.Init();
	Path.Init();
	Comment.Init();

	UI().DatabaseForm.Textboxes.Name.GetValue( Name );
	UI().DatabaseForm.Textboxes.Path.GetValue( Path );
	UI().DatabaseForm.Textboxes.Comment.GetValue( Comment );

	Name.StripCharacter( ' ' );
	Path.StripCharacter( ' ' );

	if ( Name.Amount() == 0 ) {
		UI().Alert( frdkernl::mMissingDatabaseName );
		UI().DatabaseForm.Textboxes.Name.Select();
	} else if ( Path.Amount() == 0 ) {
		UI().Alert( frdkernl::mMissingDatabasePath );
		UI().DatabaseForm.Textboxes.Path.Select();
	} else {
		UI().K().DatabaseIdentification().Set( Name, Path, Comment );
		UI().DatabaseForm.Window.Close();
	}
ERRErr
ERREnd
ERREpilog
}

void ui_dbs_f::cancel_command__::NSXPCMOnEvent( event__ )
{
	UI().K().DatabaseIdentification().SetCancelledState();
	UI().DatabaseForm.Window.Close();
}

/* UI Registrations */

static void Register_(
	ui___ &UI,
	broadcaster__<ui___> &Broadcaster,
	nsIDOMDocument *Document,
	const char *Id )
{
	ui_base::Register( UI, Broadcaster, Document, Id );
}

static void Register_(
	ui___ &UI,
	command__<ui___> &Command,
	nsIDOMDocument *Document,
	const char *Id )
{
	ui_base::Register( UI, Command, Document, Id );
}

static void Register_(
	ui___ &UI,
	textbox__<ui___> &Textbox,
	nsIDOMDocument *Document,
	const char *Id )
{
	ui_base::Register( UI, Textbox, Document, Id );
}

static void Register_(
	ui___ &UI,
	database_form__::broadcasters__ &BUI,
	nsIDOMDocument *Document )
{
	Register_( UI, BUI.DatabaseNaming, Document, "bcrDatabaseNaming" );
}

static void Register_(
	ui___ &UI,
	database_form__::commands__ &DFUI,
	nsIDOMDocument *Document )
{
	Register_( UI, DFUI.Apply, Document, "cmdApply" );
	Register_( UI, DFUI.Cancel, Document, "cmdCancel" );
}

static void Register_(
	ui___ &UI,
	database_form__::textboxes__ &TUI,
	nsIDOMDocument *Document )
{
	Register_( UI, TUI.Name, Document, "txbName" );
	Register_( UI, TUI.Path, Document, "txbPath" );
	Register_( UI, TUI.Comment, Document, "txbComment" );
}

static void Register_(
	ui___ &UI,
	database_form__ &DFUI )
{
	Register_( UI, DFUI.Broadcasters, DFUI.Document );
	Register_( UI, DFUI.Commands, DFUI.Document );
	Register_( UI, DFUI.Textboxes, DFUI.Document );
}

void ui_dbs_f::RegisterDatabaseFormUI(
	ui___ &UI,
	nsIDOMWindow *Window )
{
	UI.DatabaseForm.Init( UI, Window );

	ui_base::Register( UI, UI.DatabaseForm.Window, Window );

	Register_( UI, UI.DatabaseForm );
}


