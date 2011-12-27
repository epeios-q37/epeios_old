/*
	'xulfmn' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'xulfmn' header file ('xulfmn.h').
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

void xulfmn::main__::Update( void )
{
	Broadcasters.bdcCloseProject.Enable( Trunk().Kernel().IsProjectInProgress() );
}
#if 0
void xulfmn::window_eh__::NSXPCMOnEvent( event__ )
{
	Target().Exit();
	EventData().EventPreventDefault();	// Si l'application doit effectivement être fermée, ce sera fait par la fonctions précédente; inutile de poursuivre la prodédure de fermeture.
}
#endif

void xulfmn::new_project_eh__::NSXPCMOnEvent( event__ )
{
	Target().DefineSession( str::string(), xpp::criterions___() );
}

void xulfmn::open_project_eh__::NSXPCMOnEvent( event__ )
{
ERRProlog
	str::string Translation, FileName;
ERRBegin
	Translation.Init();
	FileName.Init();

	if ( nsxpcm::XPRJFileOpenDialogBox( Target().UI().Main().Window(), GetTranslation( xulfkl::mSelectProjectFile, Target().Kernel().LocaleRack(), Translation ), Target().Kernel().LocaleRack(), FileName ) )
		Target().DefineSession( FileName, xpp::criterions___() );
ERRErr
ERREnd
ERREpilog
}

void xulfmn::close_project_eh__::NSXPCMOnEvent( event__ )
{
	Target().DropSession();
}

void xulfmn::exit_eh__::NSXPCMOnEvent( event__ )
{
	Target().Exit();
	EventData().EventPreventDefault();	// Si l'application doit effectivement être fermée, ce sera fait par la fonctions précédente; inutile de poursuivre la prodédure de fermeture.
}

void xulfmn::about_eh__::NSXPCMOnEvent( event__ )
{
	Target().BrowseInformations();
}

void xulfmn::web_site_eh__::NSXPCMOnEvent( event__ )
{
	Target().BrowseWEBSite();
}

void xulfmn::debug_eh__::NSXPCMOnEvent( event__ )
{
	Target().UI().CreateDebugDialog();
	Target().ExposeSteering();
	nsxpcm::OpenDialog( Target().UI().Main().Window(), "debug.xul", "_blank" );
	Target().UI().DeleteDebugDialog();
}

/* Registrations */

/* 'broadcaster's */

static void Register_(
	trunk___ &Trunk,
	broadcaster__ &Broadcaster,
	const char *Id )
{
	Broadcaster.Init( Trunk, Trunk.UI().Main().Window(), Id );
}

static void Register_(
	trunk___ &Trunk,
	main__::broadcasters__ &Broadcasters )
{
	Register_( Trunk, Broadcasters.bdcCloseProject, "bdcCloseProject" );
}

static void Register_(
	trunk___ &Trunk,
	xulfbs::event_handler__ &EventHandler,
	const char *Id )
{
	EventHandler.Init( Trunk );
	nsxpcm::Attach( Trunk.UI().Main().Document(), Id, EventHandler );
}

#define A( name ) Register_( Trunk, EventHandlers.name, #name );	

static void Register_(
	trunk___ &Trunk,
	main__::event_handlers__ &EventHandlers )
{
	A( ehNewProject );
	A( ehOpenProject );
	A( ehCloseProject );
	A( ehExit );
	A( ehAbout );
	A( ehWebSite );
	A( ehDebug );
}

#define R( name ) Widgets.name.Init( Trunk, Trunk.UI().Main().Window(), #name );

static void Register_(
	trunk___ &Trunk,
	main__::widgets__ &Widgets,
	nsIDOMWindow *Window )
{
	Widgets.wdwMain.Init( Trunk, Window, Window );	// Le type n'est pas correct di on passe par l'id.
	R( dckMain );
	R( ifrSessionView );
	R( ifrSessionForm );
}

void xulfmn::RegisterMainUI(
	trunk___ &Trunk,
	nsIDOMWindow *Window )
{
	Trunk.UI().Main().Init( Window, Trunk );

#if 0
	Trunk.UI().Main().EventHandlers.Window.Init( Trunk );
	Trunk.UI().Main().EventHandlers.Window.Add( Window );
#endif

	Register_( Trunk, Trunk.UI().Main().Broadcasters );
	Register_( Trunk, Trunk.UI().Main().EventHandlers );
	Register_( Trunk, Trunk.UI().Main().Widgets, Window );

	Trunk.UI().Main().EventHandlers.ehExit.Add( Window, nsxpcm::efClose );	// Remplace le 'xex:onclose="..."' inopérant sur la balise 'windonw'.

	nsxpcm::PatchOverallBadCommandBehavior( Trunk.UI().Main().Document() );
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
