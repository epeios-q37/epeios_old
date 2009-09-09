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

#include "ui.h"

using namespace ui_main;

using nsxpcm::event__;

void ui_main::jsconsole_command__::NSXPCMOnEvent( event__ )
{
	nsxpcm::GetJSConsole();
}

void ui_main::dom_inspector_command__::NSXPCMOnEvent( event__ )
{
	nsxpcm::GetDOMInspector();
}

void ui_main::open_project_command__::NSXPCMOnEvent( event__ )
{
ERRProlog
	str::string FileName;
	STR_BUFFER___ Buffer;
ERRBegin
	FileName.Init();

	if ( nsxpcm::FileOpenDialogBox( UI().Main.Window, UI().K().GetMessage( frdkernl::mSelectProjectFile, Buffer ), FileName ) )
		UI().OpenProject( FileName.Convert( Buffer ) );

ERRErr
ERREnd
ERREpilog
}

void ui_main::close_project_command__::NSXPCMOnEvent( event__ )
{
	UI().Close();
	UI().DefineSession();
}

void ui_main::connect_to_backend_command__::NSXPCMOnEvent( event__ )
{
ERRProlog
	csducl::type__ Type = csducl::t_Undefined;
	str::string RemoteHostServiceOrLocalLibraryPath;
	STR_BUFFER___ Buffer;
ERRBegin
	RemoteHostServiceOrLocalLibraryPath.Init();

	if ( UI().GetSelectedBackend( Type, RemoteHostServiceOrLocalLibraryPath ) ) {
		if ( Type == csducl::tLibrary ) {
			UI().Alert( frdkernl::mNotImplementedYet );
			UI().DefineSession();
		} else 
		{
			UI().K().Connect( RemoteHostServiceOrLocalLibraryPath.Convert( Buffer ), Type );
			UI().ApplySession();
		}
	} else
		UI().DefineSession();
ERRErr
ERREnd
ERREpilog
}

void ui_main::create_database_command__::NSXPCMOnEvent( event__ )
{
	UI().CreateDatabase();
}

void ui_main::open_database_command__::NSXPCMOnEvent( event__ )
{
	UI().BrowseDatabase();
}

void ui_main::define_database_command__::NSXPCMOnEvent( event__ )
{
	UI().DefineDatabase();
}

void ui_main::close_database_command__::NSXPCMOnEvent( event__ )
{
	UI().CloseDatabase();
}

void ui_main::table_menu_item__::NSXPCMOnEvent( event__ ) 
{
	UI().SelectTable( *this );
}

void ui_main::create_record_command__::NSXPCMOnEvent( event__ )
{
	UI().K().RecordInput().SetState( frdtrnsnt::risCreation );
	UI().DefineRecord();
}

void ui_main::modify_record_command__::NSXPCMOnEvent( event__ )
{
	UI().K().RecordInput().SetState( frdtrnsnt::risModification );
	UI().DefineRecord();
}

void ui_main::duplicate_record_command__::NSXPCMOnEvent( event__ )
{
	UI().K().RecordInput().SetState( frdtrnsnt::risDuplication );
	UI().DefineRecord();
}

void ui_main::browse_record_command__::NSXPCMOnEvent( event__ )
{
	UI().BrowseRecord();
}

void ui_main::delete_record_command__::NSXPCMOnEvent( event__ )
{
	if ( UI().Confirm( frdkernl::mDeleteRecordConfirmation ) )
		UI().RemoveRecord();
}

/* Registrations */

/* 'broadcaster's */

static void Register_(
	bridge_functions__ &Functions,
	broadcaster__ &Broadcaster,
	nsIDOMDocument *Document,
	const char *Id )
{
	ui_base::Register( Functions, Broadcaster, Document, Id, nsxpcm::efNone );
}


static void Register_(
	bridge_functions__ &Functions,
	main__::broadcasters__ &UI,
	nsIDOMDocument *Document )
{
	Register_( Functions, UI.ProjectOpened, Document, "bcrProjectOpened" );
	Register_( Functions, UI.DatabaseOpened, Document, "bcrDatabaseOpened" );
	Register_( Functions, UI.TableWithFieldSelected, Document, "bcrTableWithFieldSelected" );
	Register_( Functions, UI.RecordSelected, Document, "bcrRecordSelected" );
}

static void Register_(
	bridge_functions__ &Functions,
	command__ &Command,
	nsIDOMDocument *Document,
	const char *Id )
{
	ui_base::Register( Functions, Command, Document, Id, nsxpcm::efCommand );
}

static void Register_(
	bridge_functions__ &Functions,
	button__ &Button,
	nsIDOMDocument *Document,
	const char *Id )
{
	ui_base::Register( Functions, Button, Document, Id, nsxpcm::efCommand );
}

static void Register_(
	bridge_functions__ &Functions,
	textbox__ &Textbox,
	nsIDOMDocument *Document,
	const char *Id )
{
	ui_base::Register( Functions, Textbox, Document, Id, nsxpcm::efCommand );
}

static void Register_(
	bridge_functions__ &Functions,
	listbox__ &Listbox,
	nsIDOMDocument *Document,
	const char *Id )
{
	ui_base::Register( Functions, Listbox, Document, Id, nsxpcm::efSelect );
}

static void Register_(
	bridge_functions__ &Functions,
	main__::commands__::backend__ &UI,
	nsIDOMDocument *Document )
{
	Register_( Functions, UI.OpenProject, Document, "cmdOpenProject" );
	Register_( Functions, UI.CloseProject, Document, "cmdCloseProject" );
	Register_( Functions, UI.ConnectToBackend, Document, "cmdConnectToBackend" );
}

static void Register_(
	bridge_functions__ &Functions,
	main__::commands__::database__ &UI,
	nsIDOMDocument *Document )
{
	Register_( Functions, UI.Create, Document, "cmdCreateDatabase" );
	Register_( Functions, UI.Open, Document, "cmdOpenDatabase" );
	Register_( Functions, UI.Define, Document, "cmdDefineDatabase" );
	Register_( Functions, UI.Close, Document, "cmdCloseDatabase" );
}

static void Register_(
	bridge_functions__ &Functions,
	main__::commands__::record__ &UI,
	nsIDOMDocument *Document )
{
	Register_( Functions, UI.Create, Document, "cmdCreateRecord" );
	Register_( Functions, UI.Modify, Document, "cmdModifyRecord" );
	Register_( Functions, UI.Duplicate, Document, "cmdDuplicateRecord" );
	Register_( Functions, UI.Browse, Document, "cmdBrowseRecord" );
	Register_( Functions, UI.Delete, Document, "cmdDeleteRecord" );
}

static void Register_(
	bridge_functions__ &Functions,
	main__::commands__ &UI,
	nsIDOMDocument *Document )
{
	Register_( Functions, UI.JSConsole, Document, "cmdJSConsole" );
	Register_( Functions, UI.DOMInspector, Document, "cmdDOMInspector" );

	Register_( Functions, UI.Backend, Document );
	Register_( Functions, UI.Database, Document );
	Register_( Functions, UI.Record, Document );
}

/* 'panel's */

static void Register_(
	bridge_functions__ &Functions,
	panel__ &Panel,
	nsIDOMDocument *Document,
	const char *Id )
{
	ui_base::Register( Functions, Panel, Document, Id, nsxpcm::efNone );
}

static void Register_(
	bridge_functions__ &Functions,
	main__::panels__ &UI,
	nsIDOMDocument *Document )
{
	Register_( Functions, UI.Home, Document, "pnlHome" );
	Register_( Functions, UI.StructureFormAndView, Document, "pnlStructureFormAndView" );
	Register_( Functions, UI.ListView, Document, "pnlListView" );
	Register_( Functions, UI.RecordForm, Document, "pnlRecordForm" );
	Register_( Functions, UI.RecordView, Document, "pnlRecordView" );
}

static void Register_(
	bridge_functions__ &Functions,
	main__ &UI )
{
	Register_( Functions, UI.Broadcasters, UI.Document );
	Register_( Functions, UI.Commands, UI.Document );

	ui_base::Register( Functions, UI.TableMenu, UI.Document, "mnuTable", nsxpcm::efNone );
	ui_base::Register( Functions, UI.MainDeck, UI.Document, "dckMain", nsxpcm::efNone );

	Register_( Functions, UI.Panels, UI.Document );
}

void ui_main::RegisterMainUI(
	ui::ui___ &UI,
	nsIDOMWindow *Window )
{
	UI.Main.Init( Window );

	Register_( UI, UI.Main );
}
