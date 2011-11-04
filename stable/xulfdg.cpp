/*
	'xulfdg' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'xulfdg' header file ('xulfdg.h').
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

#define XULFDG__COMPILATION

#include "xulfdg.h"

class xulfdgtutor
: public ttr_tutor
{
public:
	xulfdgtutor( void )
	: ttr_tutor( XULFDG_NAME )
	{
#ifdef XULFDG_DBG
		Version = XULFDG_VERSION "\b\bD $";
#else
		Version = XULFDG_VERSION;
#endif
		Owner = XULFDG_OWNER;
		Date = "$Date$";
	}
	virtual ~xulfdgtutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#include "xulftk.h"

using namespace xulfdg;
using nsxpcm::event__;

using namespace xulftk;

void xulfdg::debug_dialog__::UpdateUI( void )
{
	Widgets.Commands.BackendError.Enable( _Trunk->Kernel().IsConnected() );
}


void xulfdg::jsconsole_command__::NSXPCMOnEvent( event__ )
{
	nsxpcm::GetJSConsole( Target().UI().Main().Window );
	nsxpcm::Close( Target().UI().Debug.Window );
}

void xulfdg::dominspector_command__::NSXPCMOnEvent( event__ )
{
	nsxpcm::GetDOMInspector( Target().UI().Main().Window );
	nsxpcm::Close( Target().UI().Debug.Window );
}

void xulfdg::frontend_error_command__::NSXPCMOnEvent( event__ )
{
ERRProlog
ERRBegin
	ERRI( iTest );
ERRErr
ERREnd
	nsxpcm::Close( Target().UI().Debug.Window );
ERREpilog
}

void xulfdg::backend_error_command__::NSXPCMOnEvent( event__ )
{
ERRProlog
ERRBegin
	ERRL( lNotImplemented );
ERRErr
ERREnd
	nsxpcm::Close( Target().UI().Debug.Window );
ERREpilog
}
/* UI Registrations */

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
	debug_dialog__::widgets__::broadcasters__ &Broadcasters,
	nsIDOMWindow *Window )
{
}

static void Register_(
	trunk___ &Trunk,
	debug_dialog__::widgets__::commands__ &Commands,
	nsIDOMWindow *Window )
{
	Register_( Trunk, Commands.JSConsole, Window, "cmdJSConsole" );
	Register_( Trunk, Commands.DOMInspector, Window, "cmdDOMInspector" );
	Register_( Trunk, Commands.FrontendError, Window, "cmdFrontendError" );
	Register_( Trunk, Commands.BackendError, Window, "cmdBackendError" );
}

static void Register_(
	trunk___ &Trunk,
	debug_dialog__::widgets__ &Widgets,
	nsIDOMWindow *Window )
{
	Register_( Trunk, Widgets.Broadcasters, Window );
	Register_( Trunk, Widgets.Commands, Window );
}

void xulfdg::Register(
	trunk___ &Trunk,
	nsIDOMWindow *Window )
{
	Trunk.UI().Debug.Init( Trunk, Window );

	Register_( Trunk, Trunk.UI().Debug.Widgets, Window );

	Trunk.UI().Debug.UpdateUI();
}



/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class xulfdgpersonnalization
: public xulfdgtutor
{
public:
	xulfdgpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~xulfdgpersonnalization( void )
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

static xulfdgpersonnalization Tutor;

ttr_tutor &XULFDGTutor = Tutor;
