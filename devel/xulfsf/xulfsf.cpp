/*
	'xulfsf' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'xulfsf' header file ('xulfsf.h').
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

#include "cpe.h"

#include "nsIDOMEventTarget.h"

#define PREFIX XULFSF_NAME	"_"

using namespace xulfsf;

using nsxpcm::event__;

void xulfsf::backend_type_selection_eh__::NSXPCMOnEvent( nsxpcm::event__ Event )
{
ERRProlog
	str::string Value;
	frdkrn::backend_extended_type__ Type = frdkrn::bxt_Undefined;
ERRBegin
	Value.Init();
	nsxpcm::GetAttribute( &EventData().GetTarget(), "value", Value );

	switch ( Type = frdkrn::GetBackendExtendedType( Value ) ) {
	case frdkrn::bxtPredefined:
	case frdkrn::bxtDaemon:
	case frdkrn::bxtEmbedded:
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

void xulfsf::emebedded_backend_selection_eh__::NSXPCMOnEvent( nsxpcm::event__ Event )
{
ERRProlog
	str::string FileName;
	STR_BUFFER___ Buffer;
ERRBegin
	FileName.Init();

	if ( nsxpcm::DynamicLibraryFileOpenDialogBox( Target().UI().SessionForm().Window(), str::string( Target().Kernel().LocaleRack().GetTranslation( "EmbeddedBackendFileSelectionDialogBoxTitle", PREFIX, Buffer ) ), Target().Kernel().LocaleRack(), FileName ) )
		Target().UI().SessionForm().Widgets.EmbeddedBackendFileNameTextbox.SetValue( FileName );
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
	bsmBasic,		// Sélection dans une liste seulement.
	bsmAdvanced,	// + possibilité de sasir adresse:port pour un 'daemon'.
	bsmExpert,		// + possibilité de sélctionné une bibliothèque dynamique.
	bsm_amount,
	bsm_Undefined
};

static backend_selection_mode__ GetBackendSelectionMode_( const rgstry::multi_level_registry_ &Registry )
{
	backend_selection_mode__ Mode = bsmBasic;	// Mode par défaut.
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
	Broadcasters.DaemonBackend.Show( Daemon );
	Broadcasters.PredefinedBackend.Show();	// A minima, celui-ci est toujours affiché.

	return Success;

}

void xulfsf::session_form__::Update( frdkrn::backend_extended_type__ Type )
{
ERRProlog
	STR_BUFFER___ Buffer;
ERRBegin
	if ( !HideUnusedBackendSelectionMode_( _Trunk->Registry(), Broadcasters ) )
		_Trunk->UI().LogAndPrompt( _Trunk->Kernel().LocaleRack().GetTranslation( "BadValueForBackendSelectionMode", PREFIX, Buffer ) );

	if ( Type == frdkrn::bxt_Undefined )
		ERRReturn;

	switch ( Type ) {
	case frdkrn::bxtPredefined:
		Broadcasters.PredefinedBackend.Show();
		Widgets.BackendTypeSwitchMenulist.SetSelectedItem( Widgets.PredefinedBackendSwitchMenuitem );
		Widgets.BackendTypeDeck.SetSelectedIndex( Type );
		break;
	case frdkrn::bxtDaemon:
		Broadcasters.DaemonBackend.Show();
		Widgets.BackendTypeSwitchMenulist.SetSelectedItem( Widgets.DaemonBackendSwitchMenuitem );
		Widgets.BackendTypeDeck.SetSelectedIndex( Type );
		break;
	case frdkrn::bxtEmbedded:
		Broadcasters.EmbeddedBackend.Show();
		Widgets.BackendTypeSwitchMenulist.SetSelectedItem( Widgets.EmbeddedBackendSwitchMenuitem );
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
static void Register_(
	trunk___ &Trunk,
	session_form__::broadcasters__ &Broadcasters )
{
	R( PredefinedBackend );
	R( DaemonBackend );
	R( EmbeddedBackend );
	R( EmbeddedBackendSelection );
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
	session_form__::event_handlers__ &EventHandlers )
{
	A( BackendTypeSelection );
	A( EmbeddedBackendSelection );
}

#undef R

#define R( name, suffix, prefix ) Widgets.name##suffix.Init( Trunk, Trunk.UI().SessionForm().Window(), #prefix #name );

static void Register_(
	trunk___ &Trunk,
	session_form__::widgets__ &Widgets )
{
	R( BackendTypeSwitch, Menulist, mlt );
	R( PredefinedBackendSwitch, Menuitem, mim );
	R( DaemonBackendSwitch, Menuitem, mim );
	R( EmbeddedBackendSwitch, Menuitem, mim );
	R( BackendType, Deck, dck );
	R( DaemonBackendLocation, Textbox, txb );
	R( EmbeddedBackendFileName, Textbox, txb );
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
