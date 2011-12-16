/*
	'xulfsv' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'xulfsv' header file ('xulfsv.h').
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

#define XULFSV__COMPILATION

#include "xulfsv.h"

class xulfsvtutor
: public ttr_tutor
{
public:
	xulfsvtutor( void )
	: ttr_tutor( XULFSV_NAME )
	{
#ifdef XULFSV_DBG
		Version = XULFSV_VERSION "\b\bD $";
#else
		Version = XULFSV_VERSION;
#endif
		Owner = XULFSV_OWNER;
		Date = "$Date$";
	}
	virtual ~xulfsvtutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#include "xulftk.h"
#include "xulfrg.h"

using namespace xulfsv;

void xulfsv::session_view__::Update( frdkrn::backend_extended_type__ Type )
{
ERRProlog
ERRBegin
ERRErr
ERREnd
ERREpilog
}

static void Register_(
	trunk___ &Trunk,
	broadcaster__ &Broadcaster,
	const char *Id )
{
	Broadcaster.Init( Trunk, Trunk.UI().SessionForm().Window(), Id );
}

#define R( name ) Register_( Trunk, Broadcasters.name, "bdc" #name );

static void Register_(
	trunk___ &Trunk,
	session_view__::broadcasters__ &Broadcasters )
{
}

static void Register_(
	trunk___ &Trunk,
	xulfbs::event_handler__ &EventHandler,
	const char *Id )
{
	EventHandler.Init( Trunk );
	nsxpcm::Attach( Trunk.UI().SessionForm().Document(), Id, EventHandler );
}

#undef R

#define A( name ) Register_( Trunk, EventHandlers.name, "eh" #name )

static void Register_(
	trunk___ &Trunk,
	session_view__::event_handlers__ &EventHandlers )
{
}

#undef R

#define R( name, suffix, prefix ) Widgets.name##suffix.Init( Trunk, Trunk.UI().SessionForm().Window(), #prefix #name );

static void Register_(
	trunk___ &Trunk,
	session_view__::widgets__ &Widgets )
{
}

void xulfsv::RegisterSessionViewUI(
	trunk___ &Trunk,
	nsIDOMWindow *Window )
{
	Trunk.UI().SessionForm().Init( Window, Trunk );

	Register_( Trunk, Trunk.UI().SessionView().Broadcasters );
	Register_( Trunk, Trunk.UI().SessionView().EventHandlers );
	Register_( Trunk, Trunk.UI().SessionView().Widgets );

	nsxpcm::PatchOverallBadCommandBehavior( Trunk.UI().SessionForm().Document() );
}


/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class xulfsvpersonnalization
: public xulfsvtutor
{
public:
	xulfsvpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~xulfsvpersonnalization( void )
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

static xulfsvpersonnalization Tutor;

ttr_tutor &XULFSVTutor = Tutor;
