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

#define SETUP_LABEL		FRDKRN_SETUP_LABEL

#define SETUPS_LABEL	SETUP_LABEL "s"

static void GetAuthorText_(
	const char *Name,
	const char *Contact,
	const lcl::locale_ &Locale,
	const char *Language,
	str::string_ &Text )
{
ERRProlog
	lcl::meaning Meaning;
ERRBegin
	Meaning.Init();
	Meaning.SetValue( XULFTK_NAME "_AuthorText" );
	Meaning.AddTag( Name );
	Meaning.AddTag( Contact );

	Locale.GetTranslation( Meaning, Language, Text );
ERRErr
ERREnd
ERREpilog
}

static void GetAffiliationText_(
	const char *Tag,
	const char *Name,
	const char *URL,
	const lcl::locale_ &Locale,
	const char *Language,
	str::string_ &Text )
{
ERRProlog
	lcl::meaning Meaning;
ERRBegin
	Meaning.Init();
	Meaning.SetValue( Tag );
	Meaning.AddTag( Name );
	Meaning.AddTag( URL );

	Locale.GetTranslation( Meaning, Language, Text );
ERRErr
ERREnd
ERREpilog
}

const str::string_ &xulftk::BuildAboutText(
	const char *LauncherIdentification,
	const char *BuildInformations,
	const char *AppName,
	const char *Version,
	const char *DebugFlag,
	const char *AuthorName,
	const char *AuthorContact,
	const char *Copyright,
	const char *AffiliatedSoftwareName,
	const char *AffiliatedSoftwareURL,
	const char *AffiliatedProjectName,
	const char *AffiliatedProjectURL,
	xulftk::trunk___ &Trunk,
	str::string_ &Text )
{
ERRProlog
	str::string AuthorText, ProjectAffiliationText, SoftwareAffiliationText, ProtocolVersion, BackendLabel, APIVersion, BackendInformations;
ERRBegin
	Text.Append( AppName );
	Text.Append(" V" );
	Text.Append( Version );
	Text.Append( DebugFlag );
	Text.Append( " Build " __DATE__ " " __TIME__ );
	Text.Append( " (" );
	Text.Append( BuildInformations );
	Text.Append( ")\n\t" );

	AuthorText.Init();
	GetAuthorText_( AuthorName, AuthorContact, Trunk.Kernel().Locale(), Trunk.Kernel().Language(), AuthorText );
	Text.Append( AuthorText );
	Text.Append( "\n\t" );

	Text.Append( " (" );
	Text.Append( LauncherIdentification );
	Text.Append( ")\n" );
	Text.Append( Copyright );

	Text.Append( "\n\n\t" );
	SoftwareAffiliationText.Init();
	GetAffiliationText_( XULFTK_NAME "_AffiliatedSoftwareText", AffiliatedSoftwareName, AffiliatedSoftwareURL, Trunk.Kernel().Locale(), Trunk.Kernel().Language(), SoftwareAffiliationText );
	Text.Append( SoftwareAffiliationText );
	Text.Append( '.' );

	if ( ( AffiliatedProjectName != NULL ) && ( *AffiliatedProjectName != 0 ) ) {
		Text.Append( "\n\t" );
		ProjectAffiliationText.Init();
		GetAffiliationText_( XULFTK_NAME "_AffiliatedProjectText", AffiliatedProjectName, AffiliatedProjectURL, Trunk.Kernel().Locale(), Trunk.Kernel().Language(), ProjectAffiliationText );
		Text.Append( ProjectAffiliationText );
		Text.Append( '.' );
	}

	if ( Trunk.Kernel().IsConnected() ) {
		Text.Append( '\n' );
		Text.Append( '\n' );

		ProtocolVersion.Init();
		BackendLabel.Init();
		APIVersion.Init();
		BackendInformations.Init();

		Trunk.Kernel().AboutBackend( ProtocolVersion, BackendLabel, APIVersion, BackendInformations );

		Text.Append( "Backend : " );
		Text.Append( BackendInformations );
	}
ERRErr
ERREnd
ERREpilog
	return Text;
}


void xulftk::reporting_functions__::FRDKRNReportBackendError( const str::string_ &Message )
{
	if ( _Trunk == NULL )
		ERRc();

	_Trunk->UI().LogAndPrompt( Message );

	ERRAbort();
}

void xulftk::reporting_functions__::FRDKRNReportFrontendError( const str::string_ &Message )
{
	if ( _Trunk == NULL )
		ERRc();

	_Trunk->UI().Alert( Message );

	ERRAbort();
}

void xulftk::reporting_functions__::Init( trunk___ &Trunk )
{
	_Trunk = &Trunk;

	_reporting_functions__::Init( Trunk.Kernel() );
}

void xulftk::trunk___::Handle_( frdkrn::status__ Status )
{
ERRProlog
	str::string Translation;
ERRBegin
	Translation.Init();

	_Kernel->GetTranslatedMeaning( Translation );

	switch( Status ) {
	case frdkrn::sOK:
		break;
	case frdkrn::sWarning:
		_UI->LogAndPrompt( Translation );
		break;
	case frdkrn::sError:
		_UI->Alert( Translation );
		ERRAbort();
		break;
	default:
		ERRc();
		break;
	}
ERRErr
ERREnd
ERREpilog
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
	STR_BUFFER___ &Buffer )
{
	const char *Result = NULL;
ERRProlog
	str::string Target, Translation;
ERRBegin
	Target.Init();

	Registry.GetValue( xulfrg::AnnexTarget, Target );

	if ( !IsValid_( Target, true ) ) {
		Translation.Init();
		UI.LogAndPrompt( Locale.GetTranslation( XULFTK_NAME "_MissingOrBadAnnexTarget", Language, Translation ) );

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
	if ( ( Target = GetAnnexTarget_( UI, Registry, Locale, Language, Buffer ) ) == NULL )
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
	if ( ( Target = GetAnnexTarget_( UI, Registry, Locale, Language, Buffer ) ) == NULL )
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
			UI.LogAndPrompt( Locale.GetTranslation( XULFTK_NAME "_MissingOrBadAnnexTargetType", Language, Translation ) );

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

static rgstry::row__ ReadSetups_(
	flw::iflow__ &Flow,
	rgstry::registry_ &Setups )
{
	rgstry::row__ Root = NONE;
ERRProlog
	xtf::extended_text_iflow__ XFlow;
ERRBegin
	XFlow.Init( Flow );

	if ( rgstry::FillRegistry( XFlow, xpp::criterions___(), SETUPS_LABEL, Setups, Root ) != rgstry::sOK )
		Root = NONE;
ERRErr
ERREnd
ERREpilog
	return Root;
}

static const str::string_ &BuildSetupPath_(
	const str::string_ &Id,
	str::string_ &Path )
{
	Path.Append( SETUP_LABEL "[id=\"" );
	Path.Append( Id );
	Path.Append( "\"]" );

	return Path;
}

static bso::bool__ GetSetup_(
	const str::string_ &Id,
	const rgstry::registry_ &Setups,
	rgstry::row__ Root,
	str::string_ &Setup )
{
	bso::bool__ Success = false;
ERRProlog
	str::string Path;
ERRBegin
	Path.Init();

	BuildSetupPath_( Id, Path );

	Success = Setups.GetValue( Path, Root, Setup );
ERRErr
ERREnd
ERREpilog
	return Success;
}

static void SetSetup_(
	const str::string_ &Id,
	const str::string_ &Setup,
	rgstry::registry_ &Setups,
	rgstry::row__ Root )
{
ERRProlog
	str::string Path;
ERRBegin
	Path.Init();

	BuildSetupPath_( Id, Path );

	Setups.SetValue( Path, Setup, Root );
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

static rgstry::row__ RetrieveSetups_(
	xulfui::ui___ &UI,
	const rgstry::multi_level_registry_ &Registry,
	const lcl::locale_ &Locale,
	const char *Language,
	rgstry::registry_ &Setups )
{
	rgstry::row__ Root = NONE;
ERRProlog
	flw::iflow__ *Flow = NULL;
	ibag___ Bag;
	str::string Translation;
ERRBegin
	Bag.Init();
	
	if ( ( Flow = GetFlow_<ibag___,flw::iflow__>( dIn, Bag, UI, Registry, Locale, Language ) ) == NULL ) {
		Translation.Init();
		UI.LogAndPrompt( Locale.GetTranslation( XULFTK_NAME "_UnableToRetrieve" SETUPS_LABEL, Language, Translation ) );
		ERRReturn;
	}

	if ( ( Root = ReadSetups_( *Flow, Setups ) ) == NONE ) {
		Translation.Init();
		UI.LogAndPrompt( Locale.GetTranslation( XULFTK_NAME "_UnableToFind" SETUPS_LABEL, Language, Translation) );
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
		ERRv();
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
	str::string Translation;
ERRBegin
	if ( !Trunk.Registry().GetValue( frdrgy::ProjectId, ProjectId ) || (  ProjectId.Amount() == 0 ) ) {
		Translation.Init();
		Trunk.UI().LogAndPrompt( Trunk.Kernel().Locale().GetTranslation( XULFTK_NAME "_BadOrNoProjectId", Trunk.Kernel().Language(), Translation) );

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
	rgstry::registry Setups;
	rgstry::row__ Root = NONE;
	str::string Setup;
	flx::E_STRING_IFLOW__ Flow;
	str::string Value;
	frdkrn::backend_extended_type__ Type = frdkrn::bxt_Undefined;
	str::string ProjectId;
	str::string Translation;
ERRBegin
	Setups.Init();

	if ( ( Root = RetrieveSetups_( UI(), Registry(), Kernel().Locale(), Kernel().Language(), Setups ) ) == NONE )
		ERRAbort();

	ProjectId.Init();
	GetProjectId_( *this, ProjectId );

	Setup.Init();
	GetSetup_( ProjectId, Setups, Root, Setup );

	if ( Setup.Amount() != 0 ) {
		Flow.Init( Setup );

		XFlow.Init( Flow );

		Handle_( Kernel().FillSetupRegistry( XFlow, xpp::criterions___() ) );
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
		Translation.Init();
		UI().LogAndPrompt( Kernel().Locale().GetTranslation( XULFTK_NAME "_BadOrNoBackendType", Kernel().Language(), Translation ) );
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
ERRBegin
	Translation.Init();
	Translation.Init( Kernel().Locale().GetTranslation( XULFTK_NAME "_ProjectClosingAsking", Kernel().Language(), Translation) );
	Translation.Append( ' ' );
	Translation.Append( Kernel().Locale().GetTranslation( XULFTK_NAME "_ProjectClosingWarning", Kernel().Language(), Translation ) );

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

static bso::bool__ StoreSetups_(
	const rgstry::registry_ &Setups,
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
	str::string Translation;
	STR_BUFFER___ Buffer;
ERRBegin
	Bag.Init();
	
	if ( ( Flow = GetFlow_<obag___,flw::oflow__>( dOut, Bag, UI, Registry, Locale, Language ) ) == NULL ) {
		Translation.Init();
		UI.LogAndPrompt( Locale.GetTranslation( XULFTK_NAME "_UnableToStore" SETUPS_LABEL, Language, Translation ) );

		ERRReturn;
	}

	Write_( Setups, Root, *Flow );

	Bag.EmbeddedFlow.reset();

	UI.SaveAnnex( GetAnnexTarget_( UI, Registry, Locale, Language, Buffer), Bag.Embedded );

	Success = true;
ERRErr
ERREnd
ERREpilog
	return Success;
}

static void GetSetup_(
	const xulfkl::kernel___ &Kernel,
	str::string_ &Setup )
{
ERRProlog
	flx::E_STRING_OFLOW___ Flow;
	txf::text_oflow__ TFlow;
	xml::writer Writer;
ERRBegin
	Flow.Init( Setup );
	TFlow.Init( Flow );
	Writer.Init( TFlow, xml::oCompact, xml::e_Default );

	Kernel.DumpSetupRegistry( Writer );
ERRErr
ERREnd
ERREpilog
}


void xulftk::trunk___::_DropSession( void )
{
ERRProlog
	str::string Setup;
	rgstry::registry Setups;
	rgstry::row__ Root = NONE;
	str::string ProjectId;
ERRBegin

	if ( !Kernel().IsProjectInProgress() )
		ERRReturn;

	Setups.Init();

	if ( ( Root = RetrieveSetups_( UI(), Registry(), Kernel().Locale(),Kernel().Language(), Setups ) ) != NONE ) {
		Setup.Init();
		GetSetup_( Kernel(), Setup );

		ProjectId.Init();
		GetProjectId_( *this, ProjectId );

		SetSetup_( ProjectId, Setup, Setups, Root );

		StoreSetups_( Setups, Root, UI(), Registry(), Kernel().Locale(), Kernel().Language() );
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
