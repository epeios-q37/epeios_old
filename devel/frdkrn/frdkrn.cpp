/*
	'frdkrn' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'frdkrn' header file ('frdkrn.h').
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

#define FRDKRN__COMPILATION

#include "frdkrn.h"

class frdkrntutor
: public ttr_tutor
{
public:
	frdkrntutor( void )
	: ttr_tutor( FRDKRN_NAME )
	{
#ifdef FRDKRN_DBG
		Version = FRDKRN_VERSION "\b\bD $";
#else
		Version = FRDKRN_VERSION;
#endif
		Owner = FRDKRN_OWNER;
		Date = "$Date$";
	}
	virtual ~frdkrntutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace frdkrn;

using rgstry::tentry__;

#define CASE( m )\
	case r##m:\
	return FRDKRN_NAME "_" #m;\
	break

const char *frdkrn::GetLabel( recap__ Recap )
{
#if FRDKRN__R_AMOUNT != 9
#	error "'report__' modified !"
#endif

	switch( Recap ) {
		CASE( ProjectParsingError );
		CASE( SetupParsingError );
		CASE( NoOrBadProjectId );
		CASE( NoOrBadBackendDefinition );
		CASE( NoBackendLocation );
		CASE( UnableToConnect );
		CASE( IncompatibleBackend );
		CASE( BackendError );
		CASE( UnableToOpenFile );
		default:
			ERRCcp();
			break;
	}

	return NULL;	// Pour éviter un 'warning'.
}


#if FRDKRN__R_AMOUNT != 9
# error "'report__' modified !"
#endif

void frdkrn::GetMeaning(
	recap__ Recap,
	const error_set___ &ErrorSet,
	lcl::meaning_ &Meaning )
{
ERRProlog
	lcl::meaning MeaningBuffer;
ERRBegin
	Meaning.SetValue( GetLabel( Recap ) );

	switch ( Recap ) {
		case rProjectParsingError:
		case rSetupParsingError:
			MeaningBuffer.Init();
			rgstry::GetMeaning( ErrorSet.Context, MeaningBuffer );
			Meaning.AddTag( MeaningBuffer );
			break;
		case rNoOrBadProjectId:
			Meaning.AddTag( ErrorSet.Misc );
			break;
		case rNoOrBadBackendDefinition:
			break;
		case rNoBackendLocation:
			break;
		case rUnableToConnect:
		case rIncompatibleBackend:
		case rBackendError:
		case rUnableToOpenFile:
			Meaning.AddTag( ErrorSet.Misc );
			break;
		default:
			ERRCcp();
			break;
		}

ERRErr
ERREnd
ERREpilog
}

#define PROJECT_TYPE_NEW		"New"
#define PROJECT_TYPE_PREDEFINED	"Predefined"
#define PROJECT_TYPE_USER		"User"

project_type__ frdkrn::GetProjectType(
	const str::string_ &Label,
	err::handling__ ErrHandling )
{
	if ( Label == PROJECT_TYPE_NEW )
		return ptNew;
	else if ( Label == PROJECT_TYPE_PREDEFINED )
		return ptPredefined;
	else if ( Label == PROJECT_TYPE_USER )
		return ptUser;
	else if ( ErrHandling = err::hThrowException )
		ERRCcp();
	else if ( ErrHandling != err::hUserDefined )
		ERRCcp();

	return pt_Undefined;
}

const str::string_ &frdkrn::GetPredefinedProjectLocation(
	const str::string_ &Id,
	const frdrgy::registry_ &Registry,
	str::string_ &Location )
{
ERRProlog
	rgstry::tags Tags;
ERRBegin
	Tags.Init();
	Tags.Append( Id );

	Registry.GetValue( tentry__( frdrgy::PredefinedProject, Tags ), Location );
ERRErr
ERREnd
ERREpilog
	return Location;
}


#define BACKEND_EXTENDED_TYPE_NONE		"None"
#define BACKEND_EXTENDED_TYPE_EMBEDDED		"Embedded"
#define BACKEND_EXTENDED_TYPE_DAEMON		"Daemon"
#define BACKEND_EXTENDED_TYPE_PREDEFINED	"Predefined"

inline backend_extended_type__ frdkrn::GetBackendExtendedType( const str::string_ &RawType )
{
	if ( RawType == BACKEND_EXTENDED_TYPE_EMBEDDED )
		return bxtEmbedded;
	else if ( RawType == BACKEND_EXTENDED_TYPE_DAEMON )
		return bxtDaemon;
	else if ( RawType == BACKEND_EXTENDED_TYPE_PREDEFINED )
		return bxtPredefined;
	else if ( RawType == BACKEND_EXTENDED_TYPE_NONE )
		return bxtNone;
	else
		return bxt_Undefined;
}

static const char *GetBackendExtendedTypeLabel_( backend_extended_type__ Type )
{
	switch ( Type ) {
	case bxtNone:
		return BACKEND_EXTENDED_TYPE_NONE;
		break;
	case bxtPredefined:
		return BACKEND_EXTENDED_TYPE_PREDEFINED;
		break;
	case bxtDaemon:
		return BACKEND_EXTENDED_TYPE_DAEMON;
		break;
	case bxtEmbedded:
		return BACKEND_EXTENDED_TYPE_EMBEDDED;
		break;
	default:
		ERRCcp();
		break;
	}

	return NULL;	// Pour éviter un 'warning'.
}


backend_extended_type__ frdkrn::GetBackendExtendedType( const frdrgy::registry_ &Registry )
{
	backend_extended_type__ Type = bxt_Undefined;
ERRProlog
	str::string RawType;
ERRBegin
	RawType.Init();

	Registry.GetValue( frdrgy::BackendType, RawType );

	Type = GetBackendExtendedType( RawType );
ERRErr
ERREnd
ERREpilog
	return Type;
}

void frdkrn::SetBackendExtendedType(
	frdrgy::_registry_ &Registry,
	backend_extended_type__ Type )
{
	Registry.SetValue( frdrgy::BackendType, str::string( GetBackendExtendedTypeLabel_( Type ) ) );
}


bso::bool__ frdkrn::GetDefaultConfigurationFileName(
	const char *Affix,
	str::string_ &FileName )
{
	bso::bool__ Exists = false;
ERRProlog
	STR_BUFFER___ Buffer;
ERRBegin
	FileName.Init( Affix );
	FileName.Append( '.' );
	FileName.Append( FRDKRN_CONFIGURATION_FILE_EXTENSION );

	Exists = fil::FileExists( FileName.Convert( Buffer ) );
ERRErr
ERREnd
ERREpilog
	return Exists;
}

static str::string_ &AppendTargetAttributePathItem_(
	const char *TargetName,
	str::string_ &Target )
{
	Target.Append( "[target=\"" );
	Target.Append( TargetName );
	Target.Append( "\"]" );

	return Target;
}

void frdkrn::reporting_functions__::FBLFRDReport(
	fblovl::reply__ Reply,
	const char *Message )
{
ERRProlog
	lcl::meaning Meaning;
	str::string Translation;
ERRBegin
	if ( _Kernel == NULL )
		ERRCcp();

	Meaning.Init();
	Meaning.SetValue( fblovl::GetLabel( Reply ) );
	Meaning.AddTag( Message );

	Translation.Init();
	_Kernel->Locale().GetTranslation( Meaning, _Kernel->Language(), Translation );

	this->FRDKRNReportBackendError( Translation );
ERRErr
ERREnd
ERREpilog
}


static inline bso::uint__ GetBackendPingDelay_( rgstry::multi_level_registry_ &Registry )
{
	return rgstry::GetUInt( Registry, frdrgy::BackendPingDelay, 0 );
}

recap__ frdkrn::kernel___::_Connect(
	const char *RemoteHostServiceOrLocalLibraryPath,
	const compatibility_informations__ &CompatibilityInformations,
	csducl::type__ Type,
	frdkrn::reporting_functions__ &ReportingFunctions,
	const char *Language, 
	error_set___ &ErrorSet,
	csdsnc::log_functions__ &LogFunctions )
{
	recap__ Recap = r_Undefined;
ERRProlog
	flx::E_STRING_OFLOW_DRIVER___ OFlowDriver;
	csdlec::library_data__ LibraryData;
	csdleo::mode__ Mode = csdleo::m_Undefined;
	str::string PingDelay, Buffer;
ERRBegin
	OFlowDriver.Init( ErrorSet.Misc, fdr::ts_Default );
	LibraryData.Init( csdleo::mEmbedded, flx::VoidOFlowDriver, OFlowDriver, false, (void *)RemoteHostServiceOrLocalLibraryPath );

	Buffer.Init();

	if ( !_ClientCore.Init( RemoteHostServiceOrLocalLibraryPath, LibraryData, LogFunctions, Type, GetBackendPingDelay_( Registry() ) ) ) {
		OFlowDriver.reset();	// Pour vider les caches.
		if ( ErrorSet.Misc.Amount() != 0 )
			Recap = rBackendError;
		else
			Recap = rUnableToConnect;
		ERRReturn;
	}

	if ( !_Backend.Init( Language, CompatibilityInformations, _ClientCore, ReportingFunctions, ErrorSet.IncompatibilityInformations ) ) {
		Recap = rIncompatibleBackend;
		ERRReturn;
	}

	FRDKRNConnection( BackendAccess() );

	Recap = r_OK;
ERRErr
ERREnd
	if ( Recap != r_OK )
		ErrorSet.Misc.Init( RemoteHostServiceOrLocalLibraryPath );
ERREpilog
	return Recap;
}

recap__ frdkrn::kernel___::_Connect(
	const str::string_ &RemoteHostServiceOrLocalLibraryPath,
	const compatibility_informations__ &CompatibilityInformations,
	csducl::type__ Type,
	frdkrn::reporting_functions__ &ReportingFunctions,
	const char *Language,
	error_set___ &ErrorSet,
	csdsnc::log_functions__ &LogFunctions )
{
	recap__ Recap = r_Undefined;
ERRProlog
	STR_BUFFER___ RemoteHostServiceOrLocalLibraryPathBuffer;
ERRBegin
	Recap = _Connect( RemoteHostServiceOrLocalLibraryPath.Convert( RemoteHostServiceOrLocalLibraryPathBuffer ), CompatibilityInformations, Type, ReportingFunctions, Language, ErrorSet, LogFunctions );
ERRErr
ERREnd
ERREpilog
	return Recap;
}

static csducl::type__ GetPredefinedBackendTypeAndLocation_(
	const frdrgy::registry_ &Registry,
	str::string_ &Location )
{
	csducl::type__ Type = csducl::t_Undefined;
ERRProlog
	str::string Id;
	str::string RawType;
	rgstry::tags Tags;
ERRBegin
	Id.Init();

	if ( !Registry.GetValue( frdrgy::Backend, Id ) )
		ERRReturn;

	Tags.Init();
	Tags.Append( Id );

	RawType.Init();
	if ( !Registry.GetValue( tentry__( frdrgy::PredefinedBackendType , Tags) , RawType ) )
		ERRReturn;

	if ( !Registry.GetValue( tentry__( frdrgy::PredefinedBackend, Tags ), Location ) )
		ERRReturn;

	switch ( GetBackendExtendedType( RawType ) ) {
	case bxtEmbedded:
		Type = csducl::tLibrary;
		break;
	case bxtDaemon:
		Type = csducl::tDaemon;
		break;
	default:
		ERRReturn;
		break;
	}

ERRErr
ERREnd
ERREpilog
	return Type;
}

csducl::type__ frdkrn::GetBackendTypeAndLocation(
	const frdrgy::registry_ &Registry,
	str::string_ &Location )
{
	csducl::type__ Type = csducl::t_Undefined;

	switch ( GetBackendExtendedType( Registry ) ) {
	case bxtEmbedded:
		Type = csducl::tLibrary;
		Registry.GetValue( frdrgy::Backend, Location );
		break;
	case bxtDaemon:
		Type = csducl::tDaemon;
		Registry.GetValue( frdrgy::Backend, Location );
		break;
	case bxtPredefined:
		Type = GetPredefinedBackendTypeAndLocation_( Registry, Location );
		break;
	case bxtNone:
	case bxt_Undefined:
		break;
	default:
		ERRCcp();
		break;
	}

	return Type;
}


recap__ frdkrn::kernel___::_Connect(
	const compatibility_informations__ &CompatibilityInformations,
	reporting_functions__ &ReportingFunctions,
	const char *Language,
	error_set___ &ErrorSet,
	csdsnc::log_functions__ &LogFunctions )
{
	recap__ Recap = r_Undefined;
ERRProlog
	str::string Location;
	csducl::type__ Type = csducl::t_Undefined;
ERRBegin
	Location.Init();

	switch ( Type = GetBackendTypeAndLocation( _Registry, Location ) ) {
	case csducl::tLibrary:
	case csducl::tDaemon:
		Recap = _Connect( Location, CompatibilityInformations, Type, ReportingFunctions, Language, ErrorSet, LogFunctions );
		break;
	case csducl::t_Undefined:
		Recap = rNoOrBadBackendDefinition;
		break;
	default:
		ERRCcp();
		break;
	}
ERRErr
ERREnd
ERREpilog
	return Recap;
}

static bso::bool__ IsProjectIdValid_( const str::string_ &Id )
{
	sdr::row__ Row = Id.First();

	if ( Id.Amount() == 0 )
		return false;

	while ( Row != NONE ) {
		if ( !isalnum( Id( Row ) ) && ( Id( Row ) != '_' ) )
			return false;

		Row = Id.Next( Row );
	}

	return true;
}

#define PROJECT_ROOT_PATH	"Projects/Project[@target=\"%1\"]"

#define PROJECT_ID_RELATIVE_PATH "@id"

recap__ frdkrn::kernel___::_FillProjectRegistry(
	const str::string_ &FileName,
	const char *Target,
	const xpp::criterions___ &Criterions,
	str::string_ &Id,
	error_set___ &ErrorSet )
{
	recap__ Recap = r_Undefined;
ERRProlog
	str::string Path;
	STR_BUFFER___ FileNameBuffer, PathBuffer;
ERRBegin
	Path.Init( PROJECT_ROOT_PATH );
	str::ReplaceTag( Path, 1, str::string( Target ), '%' );

	if ( _Registry.FillProject( FileName.Convert( FileNameBuffer ), Criterions, Path.Convert( PathBuffer ), ErrorSet.Context ) != rgstry::sOK ) {
		Recap = rProjectParsingError;
		ERRReturn;
	}

	if ( !_Registry.GetProjectValue( str::string( PROJECT_ID_RELATIVE_PATH ), Id ) || !IsProjectIdValid_( Id ) ) {
		Recap = rNoOrBadProjectId;
		Path.Append( '/' );
		Path.Append( PROJECT_ID_RELATIVE_PATH );
		ErrorSet.Misc.Init( Path );
		ERRReturn;
	}

	Recap = r_OK;
ERRErr
ERREnd
ERREpilog
	return Recap;
}

recap__ frdkrn::kernel___::_DumpProjectRegistry(
	const str::string_ &FileName,
	const char *TargetName,
	const str::string_ &Id,
	error_set___ &ErrorSet )
{
	recap__ Recap = r_Undefined;
ERRProlog
	flf::file_oflow___ FFlow;
	txf::text_oflow__ TFlow;
	xml::writer Writer;
	bso::bool__ Backuped = false;
	STR_BUFFER___ FileNameBuffer;
	txf::text_oflow__ TVoidOFlow;
ERRBegin
	TVoidOFlow.Init( flx::VoidOFlow );

	if ( fil::CreateBackupFile( FileName.Convert( FileNameBuffer ), fil::bmRename, err::hUserDefined ) == fil::bsOK )
		Backuped = true;

	if ( FFlow.Init( FileNameBuffer, err::hUserDefined ) != fil::sSuccess ) {
		Recap = rUnableToOpenFile;
		ErrorSet.Misc = FileName;
		ERRReturn;
	}

	TFlow.Init( FFlow );

	Writer.Init( TFlow, xml::oCompact, xml::e_Default );

	Writer.PushTag( "Projects" );

	Writer.PushTag( "Project" );

	Writer.PutAttribute( "target", TargetName );
	Writer.PutAttribute( "id", Id );

	_Registry.DumpProject( false, Writer );
	
	Recap = r_OK;
ERRErr
	if ( Backuped )
		fil::RecoverBackupFile( FileNameBuffer, err::hUserDefined );
ERREnd
ERREpilog
	return Recap;
}


recap__ frdkrn::kernel___::FillSetupRegistry(
	xtf::extended_text_iflow__ &SettingsXFlow,
	const xpp::criterions___ &Criterions,
	error_set___ &ErrorSet )
{
	recap__ Recap = r_Undefined;
ERRProlog
	STR_BUFFER___ FileNameBuffer, PathBuffer;
ERRBegin
	
	switch ( _Registry.FillSetup( SettingsXFlow, Criterions, FRDKRN_SETUP_LABEL, ErrorSet.Context ) ) {
	case rgstry::sOK:
		break;
	case rgstry::sUnableToFindRootPath:
		ERRCcp();
		break;
	default:
		Recap = rSetupParsingError;
		ERRReturn;
		break;
	}

	Recap = r_OK;
ERRErr
ERREnd
ERREpilog
	return Recap;
}

status__ frdkrn::kernel___::FillSetupRegistry(
	xtf::extended_text_iflow__ &SettingsXFlow,
	const xpp::criterions___ &Criterions )
{
	status__ Status = s_Undefined;
ERRProlog
	error_set___ ErrorSet;
	recap__ Recap = r_Undefined;
ERRBegin
	ErrorSet.Init();

	if ( ( Recap = FillSetupRegistry( SettingsXFlow, Criterions, ErrorSet ) ) != r_OK ) {
		_Meaning.Init();
		frdkrn::GetMeaning( Recap, ErrorSet, _Meaning );
		Status = sWarning;
		ERRReturn;
	} else
		Status = sOK;
ERRErr
ERREnd
ERREpilog
	return Status;
}

static bso::bool__ LoadProjectLocale_(
	const str::string_ &LocaleContent,
	lcl::locale_ &Locale,
	lcl::context___ &Context )
{
	bso::bool__ Success = false;
ERRProlog
	flx::E_STRING_IFLOW__ Flow;
	xtf::extended_text_iflow__ XFlow;
ERRBegin
	Flow.Init( LocaleContent );
	XFlow.Init( Flow );

	if ( Locale.Push( XFlow, xpp::criterions___(), "Locale", Context ) != LCL_UNDEFINED_LEVEL )
		Success = true;
ERRErr
ERREnd
ERREpilog
	return Success;
}

status__ frdkrn::kernel___::_LoadProjectLocale( void )
{
	status__ Status = s_Undefined;
ERRProlog
	str::string Locale;
	rgstry::context___ Context;
ERRBegin
	Locale.Init();

	Registry().GetValue( frdrgy::Locale, Locale );

	if ( Locale.Amount() != 0 ) {
		Context.Init();
		if ( !LoadProjectLocale_( Locale, _Locale, Context ) ) {
			_Meaning.Init();
			rgstry::GetMeaning( Context, _Meaning );
			Status = sWarning;
		} else
			Status = sOK;
	} else
		Status = sOK;

ERRErr
ERREnd
ERREpilog
	return Status;
}

status__ frdkrn::kernel___::LoadProject(
	const str::string_ &FileName,
	const char *TargetName,
	const xpp::criterions___ &Criterions,
	str::string_ &Id )
{
	status__ Status = s_Undefined;
ERRProlog
	error_set___ ErrorSet;
	recap__ Report = r_Undefined;
ERRBegin
	ErrorSet.Init();

	if ( ( Report = LoadProject( FileName, TargetName, Criterions, Id, ErrorSet ) ) != r_OK ) {
		_Meaning.Init();
		frdkrn::GetMeaning( Report, ErrorSet, _Meaning );
		Status = sError;
		ERRReturn;
	} else
		Status = _LoadProjectLocale();
ERRErr
ERREnd
ERREpilog
	return Status;
}

status__ frdkrn::kernel___::LaunchProject(
	const compatibility_informations__ &CompatibilityInformations,
	reporting_functions__ &ReportingFunctions )
{
	status__ Status = s_Undefined;
ERRProlog
	error_set___ ErrorSet;
	recap__ Recap = r_Undefined;
ERRBegin
	ErrorSet.Init();

	if ( ( Recap = LaunchProject( CompatibilityInformations, *_ReportingFunctions, ErrorSet ) ) != r_OK ) {
		_Meaning.Init();
		frdkrn::GetMeaning( Recap, ErrorSet, _Meaning );
		Status = sError;
		ERRReturn;
	} else
		Status = sOK;
ERRErr
ERREnd
ERREpilog
	return Status;
}

recap__ frdkrn::kernel___::SaveProject(
	const str::string_ &FileName,
	const char *TargetName,
	const str::string_ &Id,
	error_set___ &ErrorSet )
{
	recap__ Recap = r_Undefined;

	if ( ( Recap = _DumpProjectRegistry( FileName, TargetName, Id, ErrorSet ) ) == r_OK ) {
		_ProjectOriginalTimeStamp = time( NULL );
		_ProjectModificationTimeStamp = 0;
	}

	return Recap;
}

status__ frdkrn::kernel___::SaveProject(
	const str::string_ &FileName,
	const char *TargetName,
	const str::string_ &Id )
{
	status__ Status = s_Undefined;
ERRProlog
	error_set___ ErrorSet;
	recap__ Recap = r_Undefined;
ERRBegin
	ErrorSet.Init();

	if ( ( Recap = SaveProject( FileName, TargetName, Id, ErrorSet ) ) != r_OK ) {
		_Meaning.Init();
		frdkrn::GetMeaning( Recap, ErrorSet, _Meaning );
		Status = sError;
		ERRReturn;
	} else
		Status = sOK;
ERRErr
ERREnd
ERREpilog
	return Status;
}

authentication_prompt_mode__ frdkrn::GetAuthenticationPromptMode( const frdrgy::registry_ &Registry )
{
	authentication_prompt_mode__ Mode = apm_Undefined;
ERRProlog
	rgstry::value Value;
ERRBegin
	Value.Init();

	if ( !Registry.GetValue( frdrgy::AuthenticationMode , Value ) )
		Mode = apmNone;
	else {
		if ( Value == "None" )
			Mode = apmNone;
		else if ( Value =="Auto" )
			Mode = apmAuto;
		else if ( Value == "Empty" )
			Mode = apmEmpty;
		else if ( Value == "Partial" )
			Mode = apmPartial;
		else if ( Value == "Full" )
			Mode = apmFull;
	}
ERRErr
ERREnd
ERREpilog
	return Mode;
}

static void GetPredefinedProject_(
	const str::string_ &Id,
	const frdrgy::registry_ &Registry,
	const lcl::locale_ &Locale,
	const char *Language,
	xml::writer_ &Writer )
{
ERRProlog
	str::string Value;
	str::string Translation;
	rgstry::tags Tags;
	STR_BUFFER___ Buffer;
ERRBegin
	Tags.Init();
	Tags.Append( Id );

	Value.Init();
	Registry.GetValue( tentry__( frdrgy::PredefinedProjectAlias, Tags ), Value );

	Translation.Init();
	Locale.GetTranslation( Value.Convert( Buffer ), Language, Translation );

	Writer.PutAttribute( "Alias", Translation );
ERRErr
ERREnd
ERREpilog
}

static void GetPredefinedProjects_(
	const rgstry::values_ &Ids,
	const frdrgy::registry_ &Registry,
	const lcl::locale_ &Locale,
	const char *Language,
	xml::writer_ &Writer )
{
	ctn::E_CMITEM( rgstry::value_ ) Id;
	sdr::row__ Row = Ids.First();

	Id.Init( Ids );

	while ( Row != NONE ) {
		Writer.PushTag( "PredefinedProject" );
		Writer.PutAttribute( "id", Id( Row ) );

		GetPredefinedProject_( Id( Row ), Registry, Locale, Language, Writer );

		Writer.PopTag();

		Row = Ids.Next( Row );
	}
}

void frdkrn::GetPredefinedProjects(
	const frdrgy::registry_ &Registry,
	const lcl::locale_ &Locale,
	const char *Language,
	xml::writer_ &Writer )
{
ERRProlog
	rgstry::values Ids;
ERRBegin
	Ids.Init();

	Registry.GetValues( frdrgy::PredefinedProjectId, Ids );

	GetPredefinedProjects_( Ids, Registry, Locale, Language, Writer );
ERRErr
ERREnd
ERREpilog
}
/**/

static void GetPredefinedBackend_(
	const str::string_ &Id,
	const frdrgy::registry_ &Registry,
	const lcl::locale_ &Locale,
	const char *Language,
	xml::writer_ &Writer )
{
ERRProlog
	str::string Value;
	str::string Translation;
	rgstry::tags Tags;
	STR_BUFFER___ Buffer;
ERRBegin
	Tags.Init();
	Tags.Append( Id );

	Value.Init();
	Registry.GetValue( tentry__( frdrgy::PredefinedBackendAlias, Tags ), Value );

	Translation.Init();
	Locale.GetTranslation( Value.Convert( Buffer ), Language, Translation );

	Writer.PutAttribute( "Alias", Translation );
ERRErr
ERREnd
ERREpilog
}

static void GetPredefinedBackends_(
	const rgstry::values_ &Ids,
	const frdrgy::registry_ &Registry,
	const lcl::locale_ &Locale,
	const char *Language,
	xml::writer_ &Writer )
{
	ctn::E_CMITEM( rgstry::value_ ) Id;
	sdr::row__ Row = Ids.First();

	Id.Init( Ids );

	while ( Row != NONE ) {
		Writer.PushTag( "PredefinedBackend" );
		Writer.PutAttribute( "id", Id( Row ) );

		GetPredefinedBackend_( Id( Row ), Registry, Locale, Language, Writer );

		Writer.PopTag();

		Row = Ids.Next( Row );
	}
}

void frdkrn::GetPredefinedBackends(
	const frdrgy::registry_ &Registry,
	const lcl::locale_ &Locale,
	const char *Language,
	xml::writer_ &Writer )
{
ERRProlog
	rgstry::values Ids;
ERRBegin
	Ids.Init();

	Registry.GetValues( frdrgy::PredefinedBackendId, Ids );

	GetPredefinedBackends_( Ids, Registry, Locale, Language, Writer );
ERRErr
ERREnd
ERREpilog
}

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class frdkrnpersonnalization
: public frdkrntutor
{
public:
	frdkrnpersonnalization( void )
	{
		if ( FRDKRN__R_AMOUNT != r_amount )
			ERRCcp();	// 
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~frdkrnpersonnalization( void )
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

static frdkrnpersonnalization Tutor;

ttr_tutor &FRDKRNTutor = Tutor;
