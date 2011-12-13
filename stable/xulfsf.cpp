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
#include "xulfrg.h"

#include "nsIDOMEventTarget.h"

#define PREFIX XULFSF_NAME	"_"

using namespace xulfsf;

using nsxpcm::event__;

static backend_type__ GetBackendType_( const str::string_ &Type )
{
	if ( Type == "Embedded" )
		return btEmbedded;
	else if ( Type == "Daemon" )
		return btDaemon;
	else if ( Type == "Predefined" )
		return btPredefined;
	else
		return bt_Undefined;
}


void xulfsf::backend_type_selection_eh__::NSXPCMOnEvent( nsxpcm::event__ Event )
{
ERRProlog
	str::string Value;
	backend_type__ Type = bt_Undefined;
ERRBegin
	Value.Init();
	nsxpcm::GetAttribute( &EventData().GetTarget(), "value", Value );

	switch ( Type = GetBackendType_( Value ) ) {
	case btPredefined:
	case btDaemon:
	case btEmbedded:
		this->Target().UI().SessionForm().Update( Type );
		break;
	default:
		ERRc();
		break;
	}
ERRErr
ERREnd
ERREpilog
}

static void DisableAllButSelected_(
	const str::string_ &Value,
	session_form__::broadcasters__ &Broadcasters )
{
	bso::bool__ Embedded = true, Daemon = true, Predefined = true;

	if ( Value == "Embedded" )
		Embedded = false;
	else if ( Value == "Daemon" )
		Daemon = false;
	else if ( Value == "Predefined" )
		Predefined = false;

	Broadcasters.EmbeddedBackend.Disable( Embedded );
	Broadcasters.DaemonBackend.Disable( Daemon );
	Broadcasters.PredefinedBackend.Disable( Predefined );
}

enum backend_selection_mode__
{
	bsmBasic,		// S�lection dans une liste seulement.
	bsmAdvanced,	// + possibilit� de sasir adresse:port pour un 'daemon'.
	bsmExpert,		// + possibilit� de s�lctionn� une biblioth�que dynamique.
	bsm_amount,
	bsm_Undefined
};

static backend_selection_mode__ GetBackendSelectionMode_( const rgstry::multi_level_registry_ &Registry )
{
	backend_selection_mode__ Mode = bsmBasic;	// Mode par d�faut.
ERRProlog
	str::string RawMode;
ERRBegin
	RawMode.Init();
	
	if ( xulfrg::GetRawBackendSelectionMode( Registry, RawMode ) ) {
		if ( RawMode == "Expert" )
			Mode = bsmExpert;
		else if ( RawMode == "Advanced" )
			Mode = bsmAdvanced;
		else if ( RawMode == "Basic" ) 
			Mode = bsmBasic;
		else
			Mode = bsm_Undefined;
	}
ERRErr
ERREnd
ERREpilog
	return Mode;
}

bso::bool__ HideUnusedBackendSelectionMode_( 
	const rgstry::multi_level_registry_ &Registry,
	xulfsf::session_form__::broadcasters__ &Broadcasters )
{
	bso::bool__ Success = true;
	bso::bool__ Embedded = false, Daemon = false;

	switch ( GetBackendSelectionMode_( Registry ) ) {
	case bsmExpert:
		Embedded = true;
	case bsmAdvanced:
		Daemon = true;
		break;
	default:
		Success = false;
	case bsmBasic:
		break;
	}

	Broadcasters.EmbeddedBackend.Show( Embedded );
	Broadcasters.EmbeddedBackendSwitch.Show( Embedded );

	Broadcasters.DaemonBackendSwitch.Show( Daemon );
	Broadcasters.DaemonBackend.Show( Daemon );

	Broadcasters.PredefinedBackendSwitch.Show();	// A minima, celui-ci est toujours affich�.
	Broadcasters.PredefinedBackend.Show();	// A minima, celui-ci est toujours affich�.

	return Success;

}

void xulfsf::session_form__::Update( backend_type__ Type )
{
ERRProlog
	STR_BUFFER___ Buffer;
ERRBegin
	if ( !HideUnusedBackendSelectionMode_( _Trunk->Registry(), Broadcasters ) )
		_Trunk->UI().LogAndPrompt( _Trunk->Kernel().LocaleRack().GetTranslation( "BadValueForBackendSelectionMode", PREFIX, Buffer ) );

	if ( Type == bt_Undefined )
		ERRReturn;


	switch ( Type ) {
	case btPredefined:
	case btDaemon:
	case btEmbedded:
		Widgets.BackendTypeDeck.SetSelectedIndex( Type );
		break;
	default:
		ERRc();
		break;
	}

ERRErr
ERREnd
ERREpilog
}

#if 0
void xulfsf::select_project_eh__::NSXPCMOnEvent( event__ )
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
#endif

static void Register_(
	trunk___ &Trunk,
	broadcaster__ &Broadcaster,
	const char *Id )
{
	Broadcaster.Init( Trunk, Trunk.UI().SessionForm().Window(), Id );
}

#define R( name ) Register_( Trunk, Broadcasters.name, "bdc" #name );
#define RX( name ) R( name );R( name##Switch )
static void Register_(
	trunk___ &Trunk,
	session_form__::broadcasters__ &Broadcasters )
{
	RX( EmbeddedBackend );
	RX( DaemonBackend );
	RX( PredefinedBackend );
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

#define R( name ) Register_( Trunk, EventHandlers.name, "eh" #name )

static void Register_(
	trunk___ &Trunk,
	session_form__::event_handlers__ &EventHandlers )
{
	R( BackendTypeSelection );
}

static void Register_(
	trunk___ &Trunk,
	session_form__::widgets__ &Widgets )
{
	Widgets.BackendTypeDeck.Init( Trunk, Trunk.UI().SessionForm().Window(), "dckBackendType" );
	Widgets.EmbeddedBackendFileNameTextbox.Init( Trunk, Trunk.UI().SessionForm().Window(), "txbEmbeddedBackendFileName" );
}

void xulfsf::RegisterSessionFormUI(
	trunk___ &Trunk,
	nsIDOMWindow *Window )
{
	Trunk.UI().SessionForm().Init( Window, Trunk );

	Register_( Trunk, Trunk.UI().SessionForm().Broadcasters );
	Register_( Trunk, Trunk.UI().SessionForm().EventHandlers );
	Register_( Trunk, Trunk.UI().SessionForm().Widgets );

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
