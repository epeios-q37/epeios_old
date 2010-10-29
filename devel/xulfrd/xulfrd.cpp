/*
	'xulfrd' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'xulfrd' header file ('xulfrd.h').
	Copyright (C) $COPYRIGHT_DATES$Claude SIMON.
$_RAW_$
	This file is part of the Epeios (http://zeusw.org/epeios/) project.

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.
 
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, go to http://www.fsf.org/
	or write to the:
  
         	         Free Software Foundation, Inc.,
           59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/



//	$Id$

#define XULFRD__COMPILATION

#include "xulfrd.h"

class xulfrdtutor
: public ttr_tutor
{
public:
	xulfrdtutor( void )
	: ttr_tutor( XULFRD_NAME )
	{
#ifdef XULFRD_DBG
		Version = XULFRD_VERSION "\b\bD $";
#else
		Version = XULFRD_VERSION;
#endif
		Owner = XULFRD_OWNER;
		Date = "$Date$";
	}
	virtual ~xulfrdtutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace xulfrd;

using nsxpcm::event__;

void xulfrd::window__::NSXPCMOnEvent( event__ )
{
	Target().CloseSession();
	EventData().EventPreventDefault();	// Si l'application doit effectivement être fermée, ce sera fait par la fonctions précédente; inutile de poursuivre la prodédure de fermeture.
}

void ui_main::new_project_command__::NSXPCMOnEvent( event__ )
{
ERRProlog
	str::string Id;
ERRBegin
	Id.Init();

	nsxpcm::Alert( GetId( Id ) );
ERRErr
ERREnd
ERREpilog
}

void ui_main::open_project_command__::NSXPCMOnEvent( event__ )
{
ERRProlog
	str::string Translation;
	str::string FileName;
ERRBegin
	Translation.Init();
	FileName.Init();

	if ( nsxpcm::XPRJFileOpenDialogBox( Window(), Target().GetTranslation( frdkernl::mSelectProjectFile, Translation ), Target().Locale(), FileName ) )
		Target().ApplySession( FileName );

ERRErr
ERREnd
ERREpilog
}

void ui_main::close_project_command__::NSXPCMOnEvent( event__ )
{
ERRProlog
	str::string Id;
ERRBegin
	Id.Init();

	nsxpcm::Alert( GetId( Id ) );
ERRErr
ERREnd
ERREpilog
}

void ui_main::exit_command__::NSXPCMOnEvent( event__ )
{
	Target().CloseSession();
}

void ui_main::about_command__::NSXPCMOnEvent( event__ )
{
ERRProlog
	str::string Version;
ERRBegin
	Version.Init();

	Target().UI().Alert( Target().GetVersionFormatedText( Version ) );
ERRErr
ERREnd
ERREpilog
}

void ui_main::web_site_command__::NSXPCMOnEvent( event__ )
{
	nsxpcm::LaunchURI( SKTINF_URL );
}

void ui_main::debug_command__::NSXPCMOnEvent( event__ )
{
	Target().Expose();
	nsxpcm::OpenDialog( Window(), "debug.xul", "_blank" );
}

void ui_main::main__::UpdateUI( void )
{
	Widgets.Commands.CloseProject.Enable( _Trunk->IsProjectInProgress() );
}


/* Registrations */

/* 'broadcaster's */

static void Register_(
	trunk___ &Trunk,
	broadcast__ &Broadcast,
	nsIDOMWindow *Window,
	const char *Id )
{
	ui_base::Register( Trunk, Broadcast, Window, Id );
}

static void Register_(
	trunk___ &Trunk,
	main__::widgets__::broadcasters__ &BroadcastersI,
	nsIDOMWindow *Window )
{
}

static void Register_(
	trunk___ &Trunk,
	command__ &Command,
	nsIDOMWindow *Window,
	const char *Id )
{
	ui_base::Register( Trunk, Command, Window, Id );
}

static void Register_(
	trunk___ &Trunk,
	button__ &Button,
	nsIDOMWindow *Window,
	const char *Id )
{
	ui_base::Register( Trunk, Button, Window, Id );
}

static void Register_(
	trunk___ &Trunk,
	textbox__ &Textbox,
	nsIDOMWindow *Window,
	const char *Id )
{
	ui_base::Register( Trunk, Textbox, Window, Id );
}

static void Register_(
	trunk___ &Trunk,
	listbox__ &Listbox,
	nsIDOMWindow *Window,
	const char *Id )
{
	ui_base::Register( Trunk, Listbox, Window, Id );
}

static void Register_(
	trunk___ &Trunk,
	main__::widgets__::commands__ &Commands,
	nsIDOMWindow *Window )
{
	Register_( Trunk, Commands.NewProject, Window, "cmdNewProject" );
	Register_( Trunk, Commands.OpenProject, Window, "cmdOpenProject" );
	Register_( Trunk, Commands.CloseProject, Window, "cmdCloseProject" );
	Register_( Trunk, Commands.Exit, Window, "cmdExit" );
	Register_( Trunk, Commands.About, Window, "cmdAbout" );
	Register_( Trunk, Commands.WebSite, Window, "cmdWebSite" );
	Register_( Trunk, Commands.Debug, Window, "cmdDebug" );
}

static void Register_(
	trunk___ &Trunk,
	tree__ &Tree,
	nsIDOMWindow *Window,
	const char *Id )
{
	ui_base::Register( Trunk, Tree, Window, Id );
}

static void Register_(
	trunk___ &Trunk,
	main__::widgets__ &Widgets,
	nsIDOMWindow *Window )
{
	ui_base::Register( Trunk, Widgets.Window, Window, Window );
	Register_( Trunk, Widgets.Broadcasters, Window );
	Register_( Trunk, Widgets.Commands, Window );
}

void ui_main::RegisterMainUI(
	trunk___ &Trunk,
	nsIDOMWindow *Window )
{
	Trunk.UI().Main.Init( Window, Trunk );

	Register_( Trunk, Trunk.UI().Main.Widgets, Window );

	nsxpcm::PatchOverallBadCommandBehavior( Trunk.UI().Main.Document );
}


/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class xulfrdpersonnalization
: public xulfrdtutor
{
public:
	xulfrdpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~xulfrdpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the ending of the application  */
	}
};


/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

// 'static' by GNU C++.

static xulfrdpersonnalization Tutor;

ttr_tutor &XULFRDTutor = Tutor;
