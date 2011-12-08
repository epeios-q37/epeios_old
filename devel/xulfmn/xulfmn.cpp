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
	Broadcasters.CloseProject.Enable( _Trunk->Kernel().IsProjectInProgress() );
}

void xulfmn::window_eh__::NSXPCMOnEvent( event__ )
{
	Target().Exit();
	EventData().EventPreventDefault();	// Si l'application doit effectivement �tre ferm�e, ce sera fait par la fonctions pr�c�dente; inutile de poursuivre la prod�dure de fermeture.
}

void xulfmn::new_project_eh__::NSXPCMOnEvent( event__ )
{
ERRProlog
	str::string Id;
ERRBegin
	Id.Init();

	Target().UI().Alert( "A impl�menter !" );
ERRErr
ERREnd
ERREpilog
}

void xulfmn::open_project_eh__::NSXPCMOnEvent( event__ )
{
	Target().ApplySession( xpp::criterions___() );
}

void xulfmn::close_project_eh__::NSXPCMOnEvent( event__ )
{
	Target().DropSession();
}

void xulfmn::exit_eh__::NSXPCMOnEvent( event__ )
{
	Target().Exit();
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

void xulfmn::test_eh__::NSXPCMOnEvent( event__ )
{
	Target().UI().Alert( "TEST !" );
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
	Register_( Trunk, Broadcasters.CloseProject, "bdcCloseProject" );
}

template <int events> static void Register_(
	trunk___ &Trunk,
	xulfbs::event_handler__<events> &EventHandler,
	const char *Id )
{
	EventHandler.Init( Trunk );
	EventHandler.Add( Trunk.UI().Main().Window(), Id );
}

#define I( name ) Register_( Trunk, EventHandlers.name, "cmd" #name );	

static void Register_(
	trunk___ &Trunk,
	main__::event_handlers__ &EventHandlers )
{
	I( NewProject );
	I( OpenProject );
	I( CloseProject );
	I( Exit );
	I( About );
	I( WebSite );
	I( Debug );

	EventHandlers.Test.Init( Trunk );

	nsxpcm::Attach( Trunk.UI().Main().Document(), str::string( "ehTest" ), EventHandlers.Test );
}

static void Register_(
	trunk___ &Trunk,
	main__::widgets__ &Widgets,
	nsIDOMWindow *Window )
{
	Widgets.Window.Init( Trunk, Window, Window );
	Widgets.MainDeck.Init( Trunk, Window, "dckMain" );
}

void xulfmn::RegisterMainUI(
	trunk___ &Trunk,
	nsIDOMWindow *Window )
{
	Trunk.UI().Main().Init( Window, Trunk );

	Trunk.UI().Main().EventHandlers.Window.Init( Trunk );
	Trunk.UI().Main().EventHandlers.Window.Add( Window );


	Register_( Trunk, Trunk.UI().Main().Broadcasters );
	Register_( Trunk, Trunk.UI().Main().EventHandlers );
	Register_( Trunk, Trunk.UI().Main().Widgets, Window );

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
