/*
	'xulfsf' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'xulfsf' header file ('xulfsf.h').
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

#define XULFSF__COMPILATION

#include "xulfsf.h"

class xulfsftutor
: public ttr_tutor
{
public:
	xulfsftutor( void )
	: ttr_tutor( XULFSF_NAME )
	{
#ifdef XULFSF_DBG
		Version = XULFSF_VERSION "\b\bD $";
#else
		Version = XULFSF_VERSION;
#endif
		Owner = XULFSF_OWNER;
		Date = "$Date$";
	}
	virtual ~xulfsftutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#include "xulftk.h"

using namespace xulfsf;

using nsxpcm::event__;

static void DisableAllButSelected_(
	const str::string_ &Value,
	session_form__::widgets__::broadcasters__ &Broadcasters )
{
	bso::bool__ Embedded = true, Daemon = true, Predefined = true;

	if ( Value == "Embedded" )
		Embedded = false;
	else if ( Value == "Daemon" )
		Daemon = false;
	else if ( Value == "Predefined" )
		Predefined = false;

	Broadcasters.Embedded.Disable( Embedded );
	Broadcasters.Daemon.Disable( Daemon );
	Broadcasters.Predefined.Disable( Predefined );
}


void xulfsf::backend_location_radiogroup__::XULWDGOnEvent( nsxpcm::event__ Event )
{
	Target().UI().SessionForm().Update();
}

void xulfsf::session_form__::Update( void )
{
ERRProlog
	str::string Value;
ERRBegin
	Value.Init();

	_Trunk->UI().SessionForm().Widgets.BackendLocationRadiogroup.GetSelectedItemValue( Value );

	DisableAllButSelected_( Value, _Trunk->UI().SessionForm().Widgets.Broadcasters );
ERRErr
ERREnd
ERREpilog
}

void xulfsf::select_project_command__::XULWDGOnEvent( event__ )
{
ERRProlog
	str::string Translation;
	str::string FileName;
ERRBegin
	Translation.Init();
	FileName.Init();

	if ( nsxpcm::XPRJFileOpenDialogBox( Target().UI().Main().Window(), Target().Kernel().GetTranslation( xulfkl::mSelectProjectFile, Translation ), Target().Kernel().LocaleRack(), FileName ) ) {
		Target().UI().SessionForm().Widgets.ProjectFileNameTextbox.SetValue( FileName );
	}
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

static void Register_(
	trunk___ &Trunk,
	session_form__::widgets__::broadcasters__ &Broadcasters )
{
	Register_( Trunk, Broadcasters.Embedded ,"bdcEmbedded" );
	Register_( Trunk, Broadcasters.Daemon ,"bdcDaemon" );
	Register_( Trunk, Broadcasters.Predefined ,"bdcPredefined" );
}

static void Register_(
	trunk___ &Trunk,
	command__ &Command,
	const char *Id )
{
	Command.Init( Trunk, Trunk.UI().SessionForm().Window(), Id );
}

static void Register_(
	trunk___ &Trunk,
	session_form__::widgets__::commands__ &Commands )
{
	Register_( Trunk, Commands.SelectProject, "cmdSelectProject" );
}

static void Register_(
	trunk___ &Trunk,
	session_form__::widgets__ &Widgets,
	nsIDOMWindow *Window )
{
	Register_( Trunk, Widgets.Broadcasters );
	Register_( Trunk, Widgets.Commands );

	Widgets.ProjectFileNameTextbox.Init( Trunk, Window, "txbProjectFileName" );
	Widgets.BackendLocationRadiogroup.Init( Trunk, Window, "rgpBackendLocation" );
}

void xulfsf::RegisterSessionFormUI(
	trunk___ &Trunk,
	nsIDOMWindow *Window )
{
	Trunk.UI().SessionForm().Init( Window, Trunk );

	Register_( Trunk, Trunk.UI().SessionForm().Widgets, Window );

	nsxpcm::PatchOverallBadCommandBehavior( Trunk.UI().SessionForm().Document() );
}

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class xulfsfpersonnalization
: public xulfsftutor
{
public:
	xulfsfpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~xulfsfpersonnalization( void )
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

static xulfsfpersonnalization Tutor;

ttr_tutor &XULFSFTutor = Tutor;
