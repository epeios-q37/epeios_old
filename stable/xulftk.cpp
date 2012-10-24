/*
	'xulftk' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'xulftk' header file ('xulftk.h').
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

#define XULFTK__COMPILATION

#include "xulftk.h"

class xulftktutor
: public ttr_tutor
{
public:
	xulftktutor( void )
	: ttr_tutor( XULFTK_NAME )
	{
#ifdef XULFTK_DBG
		Version = XULFTK_VERSION "\b\bD $";
#else
		Version = XULFTK_VERSION;
#endif
		Owner = XULFTK_OWNER;
		Date = "$Date$";
	}
	virtual ~xulftktutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#include "xulfrg.h"

#include "fnm.h"

using namespace xulftk;

void xulftk::reporting_functions__::FRDKRNReportBackendError( const char *Message )
{
	if ( _Trunk == NULL )
		ERRc();

	_Trunk->UI().LogAndPrompt( Message );

	ERRAbort();
}

void xulftk::reporting_functions__::FRDKRNReportFrontendError( const char *Message )
{
	if ( _Trunk == NULL )
		ERRc();

	_Trunk->UI().Alert( Message );

	ERRAbort();
}

void xulftk::reporting_functions__::Init( trunk___ &Trunk )
{
	_reporting_functions__::Init( Trunk.Kernel() );
}


enum annex_type__
{
	atEmbedded,
	atFile,
	atVolatile,
	at_amount,
	at_Undefined
};


static annex_type__ GetAnnexType_( const rgstry::multi_level_registry_ &Registry )
{
	annex_type__ Type = at_Undefined;
ERRProlog
	str::string Value;
ERRBegin
	Value.Init();

	if ( Registry.GetValue( xulfrg::AnnexTargetType, Value ) ) {
		if ( Value == "Embedded" )
			Type = atEmbedded;
		else if ( Value == "File" )
			Type = atFile;
		else if ( Value == "Volatile " )
			Type = atVolatile;
	}
ERRErr
ERREnd
ERREpilog
	return Type;
}

static bso::bool__ IsValid_(
	const str::string_ &Value,
	bso::bool__ AlpahaNumericOnly )	// A 'true' si la valeur doit être alpha-numérique, comme un libellé d'attribut, par example.
{
	mdr::row__ Row = Value.First();

	if ( Value.Amount() == 0 )
		return false;

	if ( !AlpahaNumericOnly )
		return true;

	while ( Row != NONE ) {
		if ( !isalnum( Value( Row ) ) && ( Value( Row ) != '_' ) )
			return false;

		Row = Value.Next( Row );
	}

	return true;
}


static const char *GetAnnexTarget_( 
	xulfui::ui___ &UI,
	const rgstry::multi_level_registry_ &Registry,
	const lcl::locale_ &Locale,
	const char *Language,
	bso::bool__ AlphaNumericOnly,	
	STR_BUFFER___ &Buffer )
{
	const char *Result = NULL;
ERRProlog
	str::string Target, Translation;
ERRBegin
	Target.Init();

	Registry.GetValue( xulfrg::AnnexTarget, Target );

	if ( !IsValid_( Target, AlphaNumericOnly ) ) {
		Translation.Init();
		Locale.GetTranslation( XULFTK_NAME "_MissingOrBadAnnexTargetDefinition", Language, Translation );
		UI.LogAndPrompt( Translation );
		ERRReturn;
	}

	Result = Target.Convert( Buffer );
ERRErr
ERREnd
ERREpilog
	return Result;
}

enum dir__
{
	dIn,
	dOut,
	d_amount,
	d_Undefined,
};


template <typename bag, typename flow> static flow *GetEmbeddedFlow_(
	dir__ Direction,
	bag &Bag,
	xulfui::ui___ &UI,
	const rgstry::multi_level_registry_ &Registry,
	const lcl::locale_ &Locale,
	const char *Language )
{
	flow *Flow = NULL;
ERRProlog
	STR_BUFFER___ Buffer;
	const char *Target = NULL;
ERRBegin
	if ( ( Target = GetAnnexTarget_( UI, Registry, Locale, Language, true, Buffer ) ) == NULL )
		ERRReturn;

	Bag.Embedded.Init();
	switch ( Direction ) {
	case dIn:
		nsxpcm::GetAttribute( nsxpcm::GetElement( UI.Main().Window() ), Target, Bag.Embedded );
		break;
	case dOut:
		break;
	default:
		ERRc();
		break;
	}

	Bag.EmbeddedFlow.Init( Bag.Embedded );
	Flow = &Bag.EmbeddedFlow;
ERRErr
ERREnd
ERREpilog
	return Flow;
}

template <typename bag, typename flow> static flow *GetFileFlow_(
	bag &Bag,
	xulfui::ui___ &UI,
	const rgstry::multi_level_registry_ &Registry,
	const lcl::locale_ &Locale,
	const char *Language )
{
	flow *Flow = NULL;
ERRProlog
	STR_BUFFER___ Buffer;
	const char *Target = NULL;
ERRBegin
	if ( ( Target = GetAnnexTarget_( UI, Registry, Locale, Language, false, Buffer ) ) == NULL )
		ERRReturn;

	if ( Bag.FileFlow.Init( Target, err::hUserDefined ) != fil::sSuccess )
		Flow = &Bag.VoidFlow;
	else
		Flow = &Bag.FileFlow;
ERRErr
ERREnd
ERREpilog
	return Flow;
}

template <typename bag, typename flow> static flow  *GetVolatileFlow_( bag &Bag )
{
	return &Bag.VoidFlow;
}

template <typename bag, typename flow> static flow  *GetFlow_(
	dir__ Direction,
	bag &Bag,
	xulfui::ui___ &UI,
	const rgstry::multi_level_registry_ &Registry,
	const lcl::locale_ &Locale,
	const char *Language )
{
	flow *Flow = NULL;
ERRProlog
	str::string Translation;
ERRBegin
	switch ( GetAnnexType_( Registry )  ) {
	case atEmbedded:
		Flow = GetEmbeddedFlow_<bag, flow>( Direction, Bag, UI, Registry, Locale, Language );
		break;
	case atFile:
		Flow = GetFileFlow_<bag, flow>( Bag, UI, Registry, Locale, Language );
		break;
	case atVolatile:
		Flow = GetVolatileFlow_<bag, flow>( Bag );
		break;
	case at_Undefined:
			Translation.Init();
			Locale.GetTranslation( XULFTK_NAME "_MissingOrBadAnnexTypeDefinition", Language, Translation );
			UI.LogAndPrompt( Translation );
			ERRReturn;
		break;
	default:
		ERRc();
		break;
	}
ERRErr
ERREnd
ERREpilog
	return Flow;
}


static void Write_(
	const rgstry::registry_ &Set,
	rgstry::row__ Root,
	flw::oflow__ &Flow )
{
ERRProlog
	txf::text_oflow__ TFlow;
ERRBegin
	TFlow.Init( Flow );
	Set.Dump( Root, true, xml::oCompact, xml::e_None, TFlow );
ERRErr
ERREnd
ERREpilog
}

static rgstry::row__ Read_(
	flw::iflow__ &Flow,
	rgstry::registry_ &Set )
{
	rgstry::row__ Root = NONE;
ERRProlog
	xtf::extended_text_iflow__ XFlow;
ERRBegin
	XFlow.Init( Flow );

	if ( rgstry::FillRegistry( XFlow, xpp::criterions___(), "SettingsSet", Set, Root ) != rgstry::sOK )
		Root = NONE;
ERRErr
ERREnd
ERREpilog
	return Root;
}

static const str::string_ &BuildPath_(
	const str::string_ &Id,
	str::string_ &Path )
{
	Path.Append( "Settings[id=\"" );
	Path.Append( Id );
	Path.Append( "\"]" );

	return Path;
}

static bso::bool__ Get_(
	const str::string_ &Id,
	const rgstry::registry_ &Set,
	rgstry::row__ Root,
	str::string_ &Settings )
{
	bso::bool__ Success = false;
ERRProlog
	str::string Path;
ERRBegin
	Path.Init();

	BuildPath_( Id, Path );

	Success = Set.GetValue( Path, Root, Settings );
ERRErr
ERREnd
ERREpilog
	return Success;
}

static void Set_(
	const str::string_ &Id,
	const str::string_ &Settings,
	rgstry::registry_ &Set,
	rgstry::row__ Root )
{
ERRProlog
	str::string Path;
ERRBegin
	Path.Init();

	BuildPath_( Id, Path );

	Set.SetValue( Path, Settings, Root );
ERRErr
ERREnd
ERREpilog
}

struct ibag___ {
	str::string Embedded;
	flf::file_iflow___ FileFlow;
	flx::E_STRING_IFLOW__  EmbeddedFlow;
	flw::iflow__ &VoidFlow;
	ibag___ ( void )
	: VoidFlow( flx::VoidIFlow )
	{}
	void Init( void )
	{
		// Standardisation.
	}
};

static rgstry::row__ RetrieveSet_(
	xulfui::ui___ &UI,
	const rgstry::multi_level_registry_ &Registry,
	const lcl::locale_ &Locale,
	const char *Language,
	rgstry::registry_ &Set )
{
	rgstry::row__ Root = NONE;
ERRProlog
	flw::iflow__ *Flow = NULL;
	ibag___ Bag;
	STR_BUFFER___ Buffer;
ERRBegin
	Bag.Init();
	
	if ( ( Flow = GetFlow_<ibag___,flw::iflow__>( dIn, Bag, UI, Registry, Locale, Language ) ) == NULL ) {
		UI.LogAndPrompt( Locale.GetTranslation( XULFTK_NAME "_UnableToRetrieveSettings", Language, Buffer ) );
		ERRReturn;
	}

	if ( ( Root = Read_( *Flow, Set ) ) == NONE ) {
		UI.LogAndPrompt( Locale.GetTranslation( XULFTK_NAME "_UnableToFindSettings", Language, Buffer ) );
		ERRReturn;
	}
ERRErr
ERREnd
ERREpilog
	return Root;
}

void xulftk::trunk___::_DefineSession(
	const str::string_ &ProjectFileName,	// Si non vide, contient le nom du fichier projet avec lequel préremplir le 'SessionForm'.
	const xpp::criterions___ &Criterions )
{
ERRProlog
	frdkrn::backend_extended_type__ Type = frdkrn::bxt_Undefined;
	str::string Location;
	str::string ProjectId;
	bso::integer_buffer__ Buffer;
ERRBegin
	ProjectId.Init();

	if ( ProjectFileName.Amount() != 0 ) {
		Handle_( Kernel().LoadProject( ProjectFileName, _TargetName, Criterions, ProjectId ) );
		Location.Init();
		Kernel().Registry().GetValue( frdrgy::Backend, Location );
	} else
		ProjectId.Init( bso::Convert( tol::EpochTime( true ), Buffer ) );

	Kernel().Registry().SetValue( frdrgy::ProjectId, ProjectId );

	switch ( Type = frdkrn::GetBackendExtendedType( Kernel().Registry() ) ) {
	case frdkrn::bxtNone:
		break;
	case frdkrn::bxtPredefined:
		ERRl();
		break;
	case frdkrn::bxtDaemon:
		UI().SessionForm().Widgets.txbDaemonBackend.SetValue( Location );
		break;
	case frdkrn::bxtEmbedded:
		UI().SessionForm().Widgets.txbEmbeddedBackend.SetValue( Location );
		break;
	case frdkrn::bxt_Undefined:
		Type = frdkrn::bxtNone;
		break;
	default:
		ERRc();
		break;
	}

	UI().SessionForm().SetBackendType( Type );
ERRErr
ERREnd
ERREpilog
}

static const str::string_ &GetProjectId_(
	trunk___ &Trunk,
	str::string_ &ProjectId )
{
ERRProlog
	STR_BUFFER___ Buffer;
ERRBegin
	if ( !Trunk.Registry().GetValue( frdrgy::ProjectId, ProjectId ) || (  ProjectId.Amount() == 0 ) ) {
		Trunk.UI().LogAndPrompt( Trunk.Kernel().Locale().GetTranslation( XULFTK_NAME "_BadOrNoProjectId", Trunk.Kernel().Language(), Buffer ) );
		ERRAbort();
	}
ERRErr
ERREnd
ERREpilog
	return ProjectId;
}


void xulftk::trunk___::_ApplySession( const frdkrn::compatibility_informations__ &CompatibilityInformations )
{
ERRProlog
	xtf::extended_text_iflow__ XFlow;
	rgstry::registry Set;
	rgstry::row__ Root = NONE;
	str::string Settings;
	flx::E_STRING_IFLOW__ Flow;
	str::string Value;
	frdkrn::backend_extended_type__ Type = frdkrn::bxt_Undefined;
	str::string ProjectId;
	STR_BUFFER___ Buffer;
ERRBegin
	Set.Init();

	if ( ( Root = RetrieveSet_( UI(), Registry(), Kernel().Locale(), Kernel().Language(), Set ) ) == NONE )
		ERRReturn;

	ProjectId.Init();
	GetProjectId_( *this, ProjectId );

	Settings.Init();
	Get_( ProjectId, Set, Root, Settings );

	if ( Settings.Amount() != 0 ) {
		Flow.Init( Settings );

		XFlow.Init( Flow );

		Handle_( Kernel().FillSettingsRegistry( XFlow, xpp::criterions___() ) );
	}

	Value.Init();
	UI().SessionForm().Widgets.mnlBackendType.GetValue( Value );

	switch ( Type = frdkrn::GetBackendExtendedType( Value ) ) {
	case frdkrn::bxtNone:
		Value.Init();
		break;
	case frdkrn::bxtPredefined:
		Value.Init();
		UI().SessionForm().Widgets.mnlPredefinedBackend.GetValue( Value );
		break;
	case frdkrn::bxtDaemon:
		Value.Init();
		UI().SessionForm().Widgets.txbDaemonBackend.GetValue( Value );
		break;
	case frdkrn::bxtEmbedded:
		Value.Init();
		UI().SessionForm().Widgets.txbEmbeddedBackend.GetValue( Value );
		break;
	default:
		UI().LogAndPrompt( Kernel().Locale().GetTranslation( XULFTK_NAME "_BadOrNoBackendType", Kernel().Language(), Buffer ) );
		ERRAbort();
		break;
	}

	Registry().SetValue( frdrgy::Backend, Value );
	frdkrn::SetBackendExtendedType( Registry(), Type );

	Handle_( Kernel().LaunchProject( CompatibilityInformations, ProjectId, DefaultReportingFunctions() ) );
ERRErr
ERREnd
ERREpilog
}

bso::bool__ xulftk::trunk___::_DefendSession( void )
{
	bso::bool__ Confirmed = false;
ERRProlog
	str::string Translation;
	STR_BUFFER___ Buffer;
ERRBegin
	Translation.Init( Kernel().Locale().GetTranslation( XULFTK_NAME "_ProjectClosingAsking", Kernel().Language(), Buffer ) );
	Translation.Append( ' ' );
	Translation.Append( Kernel().Locale().GetTranslation( XULFTK_NAME "_ProjectClosingWarning", Kernel().Language(), Buffer ) );

	Confirmed = UI().Confirm( Translation );
ERRErr
ERREnd
ERREpilog
	return Confirmed;
}

struct obag___ {
	str::string Embedded;
	flf::file_oflow___ FileFlow;
	flx::E_STRING_OFLOW___  EmbeddedFlow;
	flw::oflow__ &VoidFlow;
	obag___ ( void )
	: VoidFlow( flx::VoidOFlow )
	{}
	void Init( void )
	{
		// Standardisation.
	}
};

static bso::bool__ StoreSet_(
	const rgstry::registry_ &Set,
	rgstry::row__ Root,
	xulfui::ui___ &UI,
	const rgstry::multi_level_registry_ &Registry,
	const lcl::locale_ &Locale,
	const char *Language )
{
	bso::bool__ Success = false;
ERRProlog
	flw::oflow__ *Flow = NULL;
	obag___ Bag;
	STR_BUFFER___ Buffer;
ERRBegin
	Bag.Init();
	
	if ( ( Flow = GetFlow_<obag___,flw::oflow__>( dOut, Bag, UI, Registry, Locale, Language ) ) == NULL ) {
		UI.LogAndPrompt( Locale.GetTranslation( XULFTK_NAME "_UnableToStoreSettings", Language, Buffer ) );
		ERRReturn;
	}

	Write_( Set, Root, *Flow );

	Bag.EmbeddedFlow.reset();

	UI.SaveAnnex( "Annex", Bag.Embedded );

	Success = true;
ERRErr
ERREnd
ERREpilog
	return Success;
}

static void GetSettings_(
	const xulfkl::kernel___ &Kernel,
	str::string_ &Settings )
{
ERRProlog
	flx::E_STRING_OFLOW___ Flow;
	txf::text_oflow__ TFlow;
	xml::writer Writer;
ERRBegin
	Flow.Init( Settings );
	TFlow.Init( Flow );
	Writer.Init( TFlow, xml::oCompact, xml::e_Default );

	Kernel.DumpSettingsRegistry( Writer );
ERRErr
ERREnd
ERREpilog
}


void xulftk::trunk___::_DropSession( void )
{
ERRProlog
	str::string Settings;
	rgstry::registry Set;
	rgstry::row__ Root = NONE;
	str::string ProjectId;
ERRBegin

	if ( !Kernel().IsProjectInProgress() )
		ERRReturn;

	Set.Init();

	if ( ( Root = RetrieveSet_( UI(), Registry(), Kernel().Locale(),Kernel().Language(), Set ) ) != NONE ) {
		Settings.Init();
		GetSettings_( Kernel(), Settings );

		ProjectId.Init();
		GetProjectId_( *this, ProjectId );

		Set_( ProjectId, Settings, Set, Root );

		StoreSet_( Set, Root, UI(), Registry(), Kernel().Locale(), Kernel().Language() );
	}

	Handle_( Kernel().CloseProject() );
ERRErr
ERREnd
ERREpilog
}

const str::string_ &xulftk::trunk___::BuildXSLDefaultLocalizedFileName(
	const char *XSLFileNameAffix,
	str::string_ &LocalizedFileName )
{
ERRProlog
	FNM_BUFFER___ Buffer;
ERRBegin
	LocalizedFileName.Append( fnm::BuildFileName( DefaultXSLRootPath(), XSLFileNameAffix, ".xsl", Buffer ) );
ERRErr
ERREnd
ERREpilog
	return LocalizedFileName;
}



/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class xulftkpersonnalization
: public xulftktutor
{
public:
	xulftkpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~xulftkpersonnalization( void )
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

static xulftkpersonnalization Tutor;

ttr_tutor &XULFTKTutor = Tutor;
