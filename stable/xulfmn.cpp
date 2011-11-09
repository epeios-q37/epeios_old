/*
	'xulfmn' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'xulfmn' header file ('xulfmn.h').
	Copyright (C) 2004 Claude SIMON.

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

#define XULFMN__COMPILATION

#include "xulfmn.h"

class xulfmntutor
: public ttr_tutor
{
public:
	xulfmntutor( void )
	: ttr_tutor( XULFMN_NAME )
	{
#ifdef XULFMN_DBG
		Version = XULFMN_VERSION "\b\bD $";
#else
		Version = XULFMN_VERSION;
#endif
		Owner = XULFMN_OWNER;
		Date = "$Date$";
	}
	virtual ~xulfmntutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#include "xulftk.h"

using namespace xulfmn;

using nsxpcm::event__;

void xulfmn::window__::XULWDGOnEvent( event__ )
{
	Target().CloseSession();
	EventData().EventPreventDefault();	// Si l'application doit effectivement être fermée, ce sera fait par la fonctions précédente; inutile de poursuivre la prodédure de fermeture.
}

void xulfmn::new_project_command__::XULWDGOnEvent( event__ )
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

void xulfmn::open_project_command__::XULWDGOnEvent( event__ )
{
	Target().ApplySession( xpp::criterions___() );
}

void xulfmn::close_project_command__::XULWDGOnEvent( event__ )
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

void xulfmn::exit_command__::XULWDGOnEvent( event__ )
{
	Target().CloseSession();
}

void xulfmn::about_command__::XULWDGOnEvent( event__ )
{
	Target().BrowseInformations();
}

void xulfmn::web_site_command__::XULWDGOnEvent( event__ )
{
	Target().BrowseWEBSite();
}

void xulfmn::debug_command__::XULWDGOnEvent( event__ )
{
	Target().ExposeSteering();
	nsxpcm::OpenDialog( GetWindow(), "debug.xul", "_blank" );
}


/* Registrations */

/* 'broadcaster's */

static void Register_(
	trunk___ &Trunk,
	broadcast__ &Broadcast,
	nsIDOMWindow *Window,
	const char *Id )
{
	Broadcast.Init( Trunk, Window, Id );
}

static void Register_(
	trunk___ &Trunk,
	command__ &Command,
	nsIDOMWindow *Window,
	const char *Id )
{
	Command.Init( Trunk, Window, Id );
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
	main__::widgets__ &Widgets,
	nsIDOMWindow *Window )
{
	Widgets.Window.Init( Trunk, Window, Window );
	Register_( Trunk, Widgets.Commands, Window );
}

void xulfmn::RegisterMainUI(
	trunk___ &Trunk,
	nsIDOMWindow *Window )
{
	Trunk.UI().Main().Init( Window, Trunk );

	Register_( Trunk, Trunk.UI().Main().Widgets, Window );

	nsxpcm::PatchOverallBadCommandBehavior( Trunk.UI().Main().Document );
}


/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class xulfmnpersonnalization
: public xulfmntutor
{
public:
	xulfmnpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~xulfmnpersonnalization( void )
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

static xulfmnpersonnalization Tutor;

ttr_tutor &XULFMNTutor = Tutor;
