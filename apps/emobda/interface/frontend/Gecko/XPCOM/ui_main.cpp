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

#include "epsmsc.h"

#define NAME "emobdacom"
#define VERSION "0.1.0"
#define COPYRIGHT_YEARS	"2009"
#define EMOBDA_URL	"http://zeusw.org/intl/emobda/"
#ifdef XXX_DBG
#	define DEBUG_FLAG	" (D)"
#else
#	define DEBUG_FLAG	""
#endif
#define AUTHOR_NAME		EPSMSC_AUTHOR_NAME
#define AUTHOR_CONTACT	EPSMSC_AUTHOR_CONTACT
#define INFO			EPSMSC_EPEIOS_TEXT
#define COPYRIGHT		EPSMSC_COPYRIGHT( COPYRIGHT_YEARS )
#define EMOBDA_TEXT		"Part of the 'EMobDa' software (http://zeusw.org/intl/emobda/)"

void ui_main::version_command__::NSXPCMOnEvent( event__ )
{
ERRProlog
	str::string Version;
	tol::buffer__ Buffer;
	str::string BackendVersion;
ERRBegin
	Version.Init();

	Version.Append( NAME " V" VERSION DEBUG_FLAG " "__DATE__ " " __TIME__ );
	Version.Append( " <" );
	Version.Append( tol::DateAndTime( Buffer ) );
	Version.Append( "> \n" );
	Version.Append( " by " AUTHOR_NAME " (" AUTHOR_CONTACT ")""\n" );
	Version.Append( COPYRIGHT "\n" );
	Version.Append( INFO "\n" );
	Version.Append( EMOBDA_TEXT );

	if ( UI().K().IsConnected() ) {
		Version.Append( '\n' );
		Version.Append( '\n' );
		Version.Append( "Backend : " );

		BackendVersion.Init();
		Version.Append( UI().K().BackendVersion( BackendVersion ) );

		Version.Append( '\n' );
	}
/*
	Version.Append( "Frontend access mode to backend : " );
	Version.Append( gsbsessn::GetBackendAccessModeLabel( K().GetBackendAccessMode() ) );
*/
	UI().Alert( Version );

	
ERRErr
ERREnd
ERREpilog
}

void ui_main::web_site_command__::NSXPCMOnEvent( event__ )
{
	nsxpcm::LaunchURI( EMOBDA_URL );
}

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
	str::string Message;
ERRBegin
	RemoteHostServiceOrLocalLibraryPath.Init();

	if ( UI().GetSelectedBackend( Type, RemoteHostServiceOrLocalLibraryPath ) ) {
		if ( !UI().K().Connect( RemoteHostServiceOrLocalLibraryPath.Convert( Buffer ), Type ) ) {
			Message.Init();
			UI().K().GetMessage( frdkernl::mUnableToConnectToBackend_1, Message );
			lcl::ReplaceTag( Message, 1, RemoteHostServiceOrLocalLibraryPath );
			UI().Alert( Message );
		} else
			UI().ApplySession();
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
	ui___ &UI,
	broadcaster__<ui___> &Broadcaster,
	nsIDOMDocument *Document,
	const char *Id )
{
	ui_base::Register( UI, Broadcaster, Document, Id, nsxpcm::efNone );
}


static void Register_(
	ui___ &UI,
	main__::broadcasters__ &BUI,
	nsIDOMDocument *Document )
{
	Register_( UI, BUI.ProjectOpened, Document, "bcrProjectOpened" );
	Register_( UI, BUI.DatabaseOpened, Document, "bcrDatabaseOpened" );
	Register_( UI, BUI.TableWithFieldSelected, Document, "bcrTableWithFieldSelected" );
	Register_( UI, BUI.RecordSelected, Document, "bcrRecordSelected" );
}

static void Register_(
	ui___ &UI,
	command__<ui___> &Command,
	nsIDOMDocument *Document,
	const char *Id )
{
	ui_base::Register( UI, Command, Document, Id, nsxpcm::efCommand );
}

static void Register_(
	ui___ &UI,
	button__<ui___> &Button,
	nsIDOMDocument *Document,
	const char *Id )
{
	ui_base::Register( UI, Button, Document, Id, nsxpcm::efCommand );
}

static void Register_(
	ui___ &UI,
	textbox__<ui___> &Textbox,
	nsIDOMDocument *Document,
	const char *Id )
{
	ui_base::Register( UI, Textbox, Document, Id, nsxpcm::efCommand );
}

static void Register_(
	ui___ &UI,
	listbox__<ui___> &Listbox,
	nsIDOMDocument *Document,
	const char *Id )
{
	ui_base::Register( UI, Listbox, Document, Id, nsxpcm::efSelect );
}

static void Register_(
	ui___ &UI,
	main__::commands__::backend__ &CUI,
	nsIDOMDocument *Document )
{
	Register_( UI, CUI.OpenProject, Document, "cmdOpenProject" );
	Register_( UI, CUI.CloseProject, Document, "cmdCloseProject" );
	Register_( UI, CUI.ConnectToBackend, Document, "cmdConnectToBackend" );
}

static void Register_(
	ui___ &UI,
	main__::commands__::database__ &DUI,
	nsIDOMDocument *Document )
{
	Register_( UI, DUI.Create, Document, "cmdCreateDatabase" );
	Register_( UI, DUI.Open, Document, "cmdOpenDatabase" );
	Register_( UI, DUI.Define, Document, "cmdDefineDatabase" );
	Register_( UI, DUI.Close, Document, "cmdCloseDatabase" );
}

static void Register_(
	ui___ &UI,
	main__::commands__::record__ &RUI,
	nsIDOMDocument *Document )
{
	Register_( UI, RUI.Create, Document, "cmdCreateRecord" );
	Register_( UI, RUI.Modify, Document, "cmdModifyRecord" );
	Register_( UI, RUI.Duplicate, Document, "cmdDuplicateRecord" );
	Register_( UI, RUI.Browse, Document, "cmdBrowseRecord" );
	Register_( UI, RUI.Delete, Document, "cmdDeleteRecord" );
}

static void Register_(
	ui___ &UI,
	main__::commands__ &CUI,
	nsIDOMDocument *Document )
{
	Register_( UI, CUI.Version, Document, "cmdVersion" );
	Register_( UI, CUI.WebSite, Document, "cmdWebSite" );
	Register_( UI, CUI.JSConsole, Document, "cmdJSConsole" );
	Register_( UI, CUI.DOMInspector, Document, "cmdDOMInspector" );

	Register_( UI, CUI.Backend, Document );
	Register_( UI, CUI.Database, Document );
	Register_( UI, CUI.Record, Document );
}

/* 'panel's */

static void Register_(
	ui___ &UI,
	panel__<ui___> &Panel,
	nsIDOMDocument *Document,
	const char *Id )
{
	ui_base::Register( UI, Panel, Document, Id, nsxpcm::efNone );
}

static void Register_(
	ui___ &UI,
	main__::panels__ &PUI,
	nsIDOMDocument *Document )
{
	Register_( UI, PUI.Home, Document, "pnlHome" );
	Register_( UI, PUI.StructureFormAndView, Document, "pnlStructureFormAndView" );
	Register_( UI, PUI.ListView, Document, "pnlListView" );
	Register_( UI, PUI.RecordForm, Document, "pnlRecordForm" );
	Register_( UI, PUI.RecordView, Document, "pnlRecordView" );
}

static void Register_(
	ui___ &UI,
	main__ &MUI )
{
	Register_( UI, MUI.Broadcasters, MUI.Document );
	Register_( UI, MUI.Commands, MUI.Document );

	ui_base::Register( UI, MUI.TableMenu, MUI.Document, "mnuTable", nsxpcm::efNone );
	ui_base::Register( UI, MUI.MainDeck, MUI.Document, "dckMain", nsxpcm::efNone );

	Register_( UI, MUI.Panels, MUI.Document );
}

void ui_main::RegisterMainUI(
	ui___ &UI,
	nsIDOMWindow *Window )
{
	UI.Main.Init( Window );

	Register_( UI, UI.Main );
}
