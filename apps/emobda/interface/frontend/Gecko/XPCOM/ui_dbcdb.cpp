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

void ui_dbcdb::apply_command__::NSXPCMOnEvent( event__ )
{
ERRProlog
	str::string Name, Path, Comment;
ERRBegin
	Name.Init();
	Path.Init();
	Comment.Init();

	K().UI.DatabaseCreation.Textboxes.Name.GetValue( Name );
	K().UI.DatabaseCreation.Textboxes.Path.GetValue( Path );
	K().UI.DatabaseCreation.Textboxes.Comment.GetValue( Comment );

	Name.StripCharacter( ' ' );
	Path.StripCharacter( ' ' );

	if ( Name.Amount() == 0 ) {
		K().Alert( K().UI.DatabaseCreation.Window, kernel::mMissingDatabaseName );
		K().UI.DatabaseCreation.Textboxes.Name.Select();
	} else if ( Path.Amount() == 0 ) {
		K().Alert( K().UI.DatabaseCreation.Window, kernel::mMissingDatabasePath );
		K().UI.DatabaseCreation.Textboxes.Path.Select();
	} else {
		K().SetDatabaseIdentification( Name, Path, Comment );
		nsxpcm::Close( K().UI.DatabaseCreation.Window );
	}
ERRErr
ERREnd
ERREpilog
}

void ui_dbcdb::cancel_command__::NSXPCMOnEvent( event__ )
{
	K().SetCancelledDatabaseIdentificationState();
	nsxpcm::Close( K().UI.DatabaseCreation.Window );
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
	textbox__ &Textbox,
	nsIDOMDocument *Document,
	const char *Id )
{
	ui_base::Register( Kernel, Textbox, Document, Id );
}

static void Register_(
	kernel___ &Kernel,
	database_creation__::broadcasters__ &UI,
	nsIDOMDocument *Document )
{
	Register_( Kernel, UI.DatabaseNaming, Document, "bcrDatabaseNaming" );
}

static void Register_(
	kernel___ &Kernel,
	database_creation__::commands__ &UI,
	nsIDOMDocument *Document )
{
	Register_( Kernel, UI.Apply, Document, "cmdApply" );
	Register_( Kernel, UI.Cancel, Document, "cmdCancel" );
}

static void Register_(
	kernel___ &Kernel,
	database_creation__::textboxes__ &UI,
	nsIDOMDocument *Document )
{
	Register_( Kernel, UI.Name, Document, "txbName" );
	Register_( Kernel, UI.Path, Document, "txbPath" );
	Register_( Kernel, UI.Comment, Document, "txbComment" );
}

void ui_dbcdb::Register(
	kernel::kernel___ &Kernel,
	database_creation__ &UI,
	nsIDOMWindow *Window )
{
	UI.Set( Window );

	Register_( Kernel, UI.Broadcasters, UI.Document );
	Register_( Kernel, UI.Commands, UI.Document );
	Register_( Kernel, UI.Textboxes, UI.Document );
}


