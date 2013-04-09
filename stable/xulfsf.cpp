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

#include "cpe.h"
#include "fnm.h"

#include "nsIDOMEventTarget.h"

using namespace xulfsf;

using nsxpcm::event__;

using xulftk::trunk___;

void xulfsf::backend_type_selection_eh__::NSXPCMOnEvent( nsxpcm::event__ Event )
{
ERRProlog
	str::string Value;
	frdkrn::backend_extended_type__ Type = frdkrn::bxt_Undefined;
ERRBegin
	Value.Init();
	nsxpcm::GetAttribute( EventData().GetTarget(), "value", Value );

	switch ( Type = frdkrn::GetBackendExtendedType( Value ) ) {
	case frdkrn::bxtNone:
	case frdkrn::bxtPredefined:
	case frdkrn::bxtDaemon:
	case frdkrn::bxtEmbedded:
		Trunk().UI().SessionForm().SetBackendType( Type );
		break;
	default:
		ERRFwk();
		break;
	}

	Trunk().UpdateUI();
ERRErr
ERREnd
ERREpilog
}

void xulfsf::emebedded_backend_selection_eh__::NSXPCMOnEvent( nsxpcm::event__ Event )
{
ERRProlog
	str::string FileName;
	str::string Translation;
ERRBegin
	FileName.Init();
	Translation.Init();

	if ( nsxpcm::DynamicLibraryFileOpenDialogBox( Trunk().UI().SessionForm().Window(), str::string( Trunk().Kernel().GetTranslation( XULFDG_NAME "_EmbeddedBackendFileSelectionDialogBoxTitle", Translation ) ), Trunk().Kernel().Locale(), Trunk().Kernel().Language(), FileName ) )
		Trunk().UI().SessionForm().Widgets.txbEmbeddedBackend.SetValue( FileName );
ERRErr
ERREnd
ERREpilog
}

void xulfsf::apply_eh__::NSXPCMOnEvent( nsxpcm::event__ Event )
{
	Trunk().ApplySession();
}

void xulfsf::cancel_eh__::NSXPCMOnEvent( nsxpcm::event__ Event )
{
	Trunk().CancelSession();
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

	Broadcasters.bdcEmbeddedBackend.Disable( Embedded );
	Broadcasters.bdcDaemonBackend.Disable( Daemon );
	Broadcasters.bdcPredefinedBackend.Disable( Predefined );
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
	
	if ( Registry.GetValue( xulfrg::BackendSelectionMode, RawMode ) ) {
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

	Broadcasters.bdcEmbeddedBackend.Show( Embedded );
	Broadcasters.bdcDaemonBackend.Show( Daemon );
	Broadcasters.bdcPredefinedBackend.Show();	// A minima, celui-ci est toujours affiché.
	Broadcasters.bdcNoBackend.Show();			// A minima, celui-ci est toujours affiché.

	return Success;

}

void HandleAuthenticationSubForm_(
	const frdrgy::registry_ &Registry,
	session_form__::broadcasters__ &Broadcasters,
	session_form__::widgets__ &Widgets )
{
ERRProlog
	str::string Login, Password;
	bso::bool__ Disable = false;
ERRBegin
	Login.Init();
	Password.Init();

	switch ( frdkrn::GetAuthenticationPromptMode( Registry ) ) {
	case frdkrn::apmNone:
	case frdkrn::apmAuto:
		Disable = true;
		break;
	case frdkrn::apmFull:
		if ( !Registry.GetValue( frdrgy::AuthenticationPassword, Password ) )
			ERRDta();
	case frdkrn::apmPartial:
		if ( !Registry.GetValue( frdrgy::AuthenticationLogin, Login ) )
			ERRDta();
	case frdkrn::apmEmpty:
		break;
	case frdkrn::apm_Undefined:
		ERRLmt();
		// Affichage d'un message d'erreur normalement.
		break;
	default:
		ERRFwk();
		break;
	}

	Widgets.txbLogin.SetValue( Login );
	Widgets.txbPassword.SetValue( Password );
	Broadcasters.bdcAuthentication.Disable( Disable );
ERRErr
ERREnd
ERREpilog
}

static void GetPredefinedBackends_(
	const frdrgy::registry_ &Registry,
	const lcl::locale_ &Locale,
	const char *Language,
	str::string_ &PredefinedBackends )
{
ERRProlog
	flx::E_STRING_OFLOW___ OFlow;
	txf::text_oflow__ TFlow;
	xml::writer Writer;
	str::string Default;
ERRBegin
	OFlow.Init( PredefinedBackends );
	TFlow.Init( OFlow );
	Writer.Init( TFlow, xml::oIndent, xml::e_Default );

	Writer.PushTag( "PredefinedBackends" );

	Default.Init();
	Registry.GetValue( frdrgy::DefaultPredefinedBackend, Default );

	if ( Default.Amount() != 0 )
		Writer.PutAttribute( "Default", Default );

	frdkrn::GetPredefinedBackends( Registry, Locale, Language, Writer );

	Writer.PopTag();
ERRErr
ERREnd
ERREpilog
}

static void FillPredefinedBackendsWidget_( trunk___ &Trunk )
{
ERRProlog
	str::string PredefinedBackends;
	nsIDOMDocumentFragment *Fragment;
	FNM_BUFFER___ FileNameBuffer;
ERRBegin
	PredefinedBackends.Init();

	GetPredefinedBackends_( Trunk.Registry(),Trunk.Kernel().Locale(), Trunk.Kernel().Language(), PredefinedBackends );

	Trunk.UI().LogQuietly( PredefinedBackends );

	nsxpcm::RemoveChildren( Trunk.UI().SessionForm().Widgets.mnlPredefinedBackend );

	Fragment = nsxpcm::XSLTransformByFileName( PredefinedBackends, str::string( fnm::BuildFileName( Trunk.DefaultXSLRootPath(), "PredefinedBackendMenuList", ".xsl", FileNameBuffer ) ), Trunk.UI().SessionForm().Document(), nsxpcm::xslt_parameters() );

	nsxpcm::AppendChild( Trunk.UI().SessionForm().Widgets.mnlPredefinedBackend, Fragment );

	nsxpcm::SetSelectedItem( Trunk.UI().SessionForm().Widgets.mnlPredefinedBackend );
ERRErr
ERREnd
ERREpilog
}

void xulfsf::session_form__::Update( void )
{
ERRProlog
	str::string Translation;
ERRBegin
	Translation.Init();

	if ( !HideUnusedBackendSelectionMode_( Trunk().Registry(), Broadcasters ) )
		Trunk().UI().LogAndPrompt( Trunk().Kernel().GetTranslation( XULFSF_NAME "_BadValueForBackendSelectionMode", Translation ) );

	HandleAuthenticationSubForm_( Trunk().Registry(), Broadcasters, Widgets );

	FillPredefinedBackendsWidget_( Trunk() );
#if 0
	if ( _BackendExtendedType == frdkrn::bxt_Undefined )
		ERRReturn;
#endif
	switch ( _BackendExtendedType ) {
	case frdkrn::bxtNone:
		Broadcasters.bdcNoBackend.Show();
		Widgets.mnlBackendType.SetSelectedItem( Widgets.mniNoBackend );
		break;
	case frdkrn::bxtPredefined:
		Broadcasters.bdcPredefinedBackend.Show();
		Widgets.mnlBackendType.SetSelectedItem( Widgets.mniPredefinedBackend );
		break;
	case frdkrn::bxtDaemon:
		Broadcasters.bdcDaemonBackend.Show();
		Widgets.mnlBackendType.SetSelectedItem( Widgets.mniDaemonBackend );
		break;
	case frdkrn::bxtEmbedded:
		Broadcasters.bdcEmbeddedBackend.Show();
		Widgets.mnlBackendType.SetSelectedItem( Widgets.mniEmbeddedBackend );
		break;
	default:
		ERRFwk();
		break;
	}

	Widgets.dckBackendType.SetSelectedIndex( _BackendExtendedType );

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

	if ( nsxpcm::XPRJFileOpenDialogBox( Trunk().UI().Main().Window(), Trunk().Kernel().GetTranslation( xulfkl::mSelectProjectFile, Translation ), Trunk().Kernel().LocaleRack(), FileName ) ) {
		Trunk().UI().SessionForm().Widgets.ProjectFileNameTextbox.SetValue( FileName );
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
	Broadcaster.Init( Trunk, nsxpcm::supports__( Trunk.UI().SessionForm().Window(), Id ) );
}

#define R( name ) Register_( Trunk, Broadcasters.name, #name );
static void Register_(
	trunk___ &Trunk,
	session_form__::broadcasters__ &Broadcasters )
{
	R( bdcNoBackend );
	R( bdcPredefinedBackend );
	R( bdcDaemonBackend );
	R( bdcEmbeddedBackend );
	R( bdcEmbeddedBackendSelection );
	R( bdcAuthentication );
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

#define A( name ) Register_( Trunk, EventHandlers.name, #name )

static void Register_(
	trunk___ &Trunk,
	session_form__::event_handlers__ &EventHandlers )
{
	A( ehBackendTypeSelection );
	A( ehEmbeddedBackendSelection );
	A( ehApply );
	A( ehCancel );
}

#undef R

#define R( name ) Widgets.name.Init( Trunk, nsxpcm::supports__( Trunk.UI().SessionForm().Window(), #name ) );

static void Register_(
	trunk___ &Trunk,
	session_form__::widgets__ &Widgets )
{
	R( mnlBackendType );
	R( mniNoBackend );
	R( mniPredefinedBackend );
	R( mniDaemonBackend );
	R( mniEmbeddedBackend );
	R( dckBackendType );
	R( mnlPredefinedBackend );
	R( txbDaemonBackend );
	R( txbEmbeddedBackend );
	R( txbLogin );
	R( txbPassword );
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
