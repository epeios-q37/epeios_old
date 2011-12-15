/*
	'frdkrn' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'frdkrn' header file ('frdkrn.h').
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

#define CASE( m )\
	case r##m:\
	return #m;\
	break

const char *frdkrn::GetLabel( report__ Report )
{
#if FRDKRN__R_AMOUNT != 9
#	error "'report__' modified !"
#endif

	switch( Report ) {
		CASE( ProjectParsingError );
		CASE( ParametersParsingError );
		CASE( NoOrBadProjectId );
		CASE( NoOrBadBackendDefinition );
		CASE( NoBackendLocation );
		CASE( UnableToConnect );
		CASE( IncompatibleBackend );
		CASE( BackendError );
		CASE( UnableToOpenFile );
		default:
			ERRu();
			break;
	}

	return NULL;	// Pour �viter un 'warning'.
}


#if FRDKRN__R_AMOUNT != 9
# error "'report__' modified !"
#endif

static const str::string_ &frdkrn::GetTranslation(
	report__ Report,
	const error_set___ &ErrorSet,
	const lcl::rack__ &LocaleRack,
	str::string_ &Translation )
{
ERRProlog
	str::string EmbeddedMessage;
ERRBegin
	GetTranslation( Report, LocaleRack, Translation );

	switch ( Report ) {
		case rProjectParsingError:
		case rParametersParsingError:
			EmbeddedMessage.Init();
			rgstry::GetTranslation( ErrorSet.Context, LocaleRack, EmbeddedMessage );
			lcl::ReplaceTag( Translation, 1, EmbeddedMessage );
			break;
		case rNoOrBadProjectId:
			lcl::ReplaceTag( Translation, 1, ErrorSet.Misc );
			break;
		case rNoOrBadBackendDefinition:
			break;
		case rNoBackendLocation:
			break;
		case rUnableToConnect:
		case rIncompatibleBackend:
		case rBackendError:
		case rUnableToOpenFile:
			lcl::ReplaceTag( Translation, 1, ErrorSet.Misc );
			break;
		default:
			ERRc();
			break;
		}

ERRErr
ERREnd
ERREpilog
	return Translation;
}

backend_extended_type__ frdkrn::GetBackendExtendedType( const frdrgy::_registry_ &Registry )
{
	backend_extended_type__ Type = bxt_Undefined;
ERRProlog
	str::string RawType;
ERRBegin
	RawType.Init();

	frdrgy::GetRawBackendExtendedType( Registry, RawType );

	Type = GetBackendExtendedType( RawType );
ERRErr
ERREnd
ERREpilog
	return Type;
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

report__ frdkrn::kernel___::_Connect(
	const char *RemoteHostServiceOrLocalLibraryPath,
	const compatibility_informations__ &CompatibilityInformations,
	csducl::type__ Type,
	frdkrn::error_reporting_functions__ &ErrorReportingFunctions,
	error_set___ &ErrorSet,
	csdsnc::log_functions__ &LogFunctions )
{
	report__ Report = r_Undefined;
ERRProlog
	flx::E_STRING_OFLOW_DRIVER___ OFlowDriver;
	csdlec::library_data__ LibraryData;
	csdleo::mode__ Mode = csdleo::m_Undefined;
ERRBegin
	OFlowDriver.Init( ErrorSet.Misc, fdr::ts_Default );
	LibraryData.Init( csdleo::mEmbedded, flx::VoidOFlowDriver, OFlowDriver );

	if ( !_ClientCore.Init( RemoteHostServiceOrLocalLibraryPath, LibraryData, LogFunctions, Type, frdrgy::GetBackendPingDelay( Registry() ) ) ) {
		OFlowDriver.reset();	// Pour vider les caches.
		if ( ErrorSet.Misc.Amount() != 0 )
			Report = rBackendError;
		else
			Report = rUnableToConnect;
		ERRReturn;
	}

	if ( !_Backend.Init( LocaleRack().Language(), CompatibilityInformations, _ClientCore, ErrorReportingFunctions, ErrorSet.IncompatibilityInformations ) ) {
		Report = rIncompatibleBackend;
		ERRReturn;
	}

	Report = r_OK;
ERRErr
ERREnd
	if ( Report != r_OK )
		ErrorSet.Misc.Init( RemoteHostServiceOrLocalLibraryPath );
ERREpilog
	return Report;
}

report__ frdkrn::kernel___::_Connect(
	const str::string_ &RemoteHostServiceOrLocalLibraryPath,
	const compatibility_informations__ &CompatibilityInformations,
	csducl::type__ Type,
	frdkrn::error_reporting_functions__ &ErrorReportingFunctions,
	error_set___ &ErrorSet,
	csdsnc::log_functions__ &LogFunctions )
{
	report__ Report = r_Undefined;
ERRProlog
	STR_BUFFER___ RemoteHostServiceOrLocalLibraryPathBuffer;
ERRBegin
	Report = _Connect( RemoteHostServiceOrLocalLibraryPath.Convert( RemoteHostServiceOrLocalLibraryPathBuffer ), CompatibilityInformations, Type, ErrorReportingFunctions, ErrorSet, LogFunctions );
ERRErr
ERREnd
ERREpilog
	return Report;
}

csducl::type__ frdkrn::GetBackendTypeAndLocation(
	const frdrgy::registry_ &Registry,
	str::string_ &Location )
{
	csducl::type__ Type = csducl::t_Undefined;

	switch ( GetBackendExtendedType( Registry ) ) {
	case bxtEmbedded:
		Type = csducl::tLibrary;
		frdrgy::GetBackendLocation( Registry, Location );
		break;
	case bxtDaemon:
		Type = csducl::tDaemon;
		frdrgy::GetBackendLocation( Registry, Location );
		break;
	case bxtPredefined:
		ERRl();
		break;
	case bxt_Undefined:
		break;
	default:
		ERRc();
		break;
	}

	return Type;
}


report__ frdkrn::kernel___::_Connect(
	const compatibility_informations__ &CompatibilityInformations,
	error_reporting_functions__ &ErrorReportingFunctions,
	error_set___ &ErrorSet,
	csdsnc::log_functions__ &LogFunctions )
{
	report__ Report = r_Undefined;
ERRProlog
	str::string Location;
	csducl::type__ Type = csducl::t_Undefined;
ERRBegin
	Location.Init();

	switch ( Type = GetBackendTypeAndLocation( _Registry, Location ) ) {
	case csducl::tLibrary:
	case csducl::tDaemon:
		Report = _Connect( Location, CompatibilityInformations, Type, ErrorReportingFunctions, ErrorSet, LogFunctions );
		break;
	case csducl::t_Undefined:
		Report = rNoOrBadBackendDefinition;
		break;
	default:
		ERRc();
		break;
	}
ERRErr
ERREnd
ERREpilog
	return Report;
}

inline static const str::string_ &Report_(
	report__ Report,
	const error_set___ &ErrorSet,
	const lcl::rack__ &Locale,
	str::string_ &Message )
{
	GetTranslation( Report, ErrorSet, Locale, Message );

	return Message;
}

static bso::bool__ IsProjectIdValid_( const str::string_ &Id )
{
	epeios::row__ Row = Id.First();

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

report__ frdkrn::kernel___::_FillProjectRegistry(
	const str::string_ &FileName,
	const char *Target,
	const xpp::criterions___ &Criterions,
	str::string_ &Id,
	error_set___ &ErrorSet )
{
	report__ Report = r_Undefined;
ERRProlog
	str::string Path;
	STR_BUFFER___ FileNameBuffer, PathBuffer;
ERRBegin
	Path.Init( PROJECT_ROOT_PATH );
	str::ReplaceTag( Path, 1, str::string( Target ), '%' );

	if ( _Registry.FillProject( FileName.Convert( FileNameBuffer ), Criterions, Path.Convert( PathBuffer ), ErrorSet.Context ) != rgstry::sOK ) {
		Report = rProjectParsingError;
		ERRReturn;
	}

	if ( !_Registry.GetProjectValue( str::string( PROJECT_ID_RELATIVE_PATH ), Id ) || !IsProjectIdValid_( Id ) ) {
		Report = rNoOrBadProjectId;
		Path.Append( '/' );
		Path.Append( PROJECT_ID_RELATIVE_PATH );
		ErrorSet.Misc.Init( Path );
		ERRReturn;
	}

	Report = r_OK;
ERRErr
ERREnd
ERREpilog
	return Report;
}

report__ frdkrn::kernel___::_DumpProjectRegistry(
	const str::string_ &FileName,
	const char *TargetName,
	const str::string_ &Id,
	error_set___ &ErrorSet )
{
	report__ Report = r_Undefined;
ERRProlog
	flf::file_oflow___ FFlow;
	txf::text_oflow__ TFlow;
	xml::writer Writer;
	bso::bool__ Backuped = false;
	STR_BUFFER___ FileNameBuffer;
ERRBegin

	if ( fil::CreateBackupFile( FileName.Convert( FileNameBuffer ), fil::bmRename, LocaleRack(), txf::text_oflow__( flx::VoidOFlow ) ) == fil::bsOK )
		Backuped = true;

	if ( FFlow.Init( FileNameBuffer, err::hUserDefined ) != fil::sSuccess ) {
		Report = rUnableToOpenFile;
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
	
	Report = r_OK;
ERRErr
	if ( Backuped )
		fil::RecoverBackupFile( FileNameBuffer, LocaleRack(), txf::text_oflow__( flx::VoidOFlow ) );
ERREnd
ERREpilog
	return Report;
}


#define PARAMETERS_ROOT_PATH	"Parameters"

report__ frdkrn::kernel___::FillParametersRegistry(
	xtf::extended_text_iflow__ &ParametersXFlow,
	const xpp::criterions___ &Criterions,
	error_set___ &ErrorSet )
{
	report__ Report = r_Undefined;
ERRProlog
	STR_BUFFER___ FileNameBuffer, PathBuffer;
ERRBegin
	
	switch ( _Registry.FillParameters( ParametersXFlow, Criterions, PARAMETERS_ROOT_PATH, ErrorSet.Context ) ) {
	case rgstry::sOK:
		break;
	case rgstry::sUnableToFindRootPath:
		_Registry.CreateParametersPath( str::string( PARAMETERS_ROOT_PATH ) );
		break;
	default:
		Report = rParametersParsingError;
		ERRReturn;
		break;
	}

	Report = r_OK;
ERRErr
ERREnd
ERREpilog
	return Report;
}

status__ frdkrn::kernel___::FillParametersRegistry(
	xtf::extended_text_iflow__ &ParametersXFlow,
	const xpp::criterions___ &Criterions )
{
	status__ Status = s_Undefined;
ERRProlog
	error_set___ ErrorSet;
	report__ Report = r_Undefined;
ERRBegin
	ErrorSet.Init();

	if ( ( Report = FillParametersRegistry( ParametersXFlow, Criterions, ErrorSet ) ) != r_OK ) {
		_Message.Init();
		GetTranslation( Report, ErrorSet, LocaleRack(), _Message );
		Status = sWarning;
		ERRReturn;
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
	const xpp::criterions___ &Criterions )
{
	status__ Status = s_Undefined;
ERRProlog
	error_set___ ErrorSet;
	report__ Report = r_Undefined;
ERRBegin
	ErrorSet.Init();

	if ( ( Report = LoadProject( FileName, TargetName, Criterions, ErrorSet ) ) != r_OK ) {
		_Message.Init();
		GetTranslation( Report, ErrorSet, LocaleRack(), _Message );
		Status = sError;
		ERRReturn;
	} else
		Status = sOK;
ERRErr
ERREnd
ERREpilog
	return Status;
}

status__ frdkrn::kernel___::Connect( const compatibility_informations__ &CompatibilityInformations,
	error_reporting_functions__ &ErrorReportingFunctions )
{
	status__ Status = s_Undefined;
ERRProlog
	error_set___ ErrorSet;
	report__ Report = r_Undefined;
ERRBegin
	ErrorSet.Init();

	if ( ( Report = Connect( CompatibilityInformations, *_ErrorReportingFunctions, ErrorSet ) ) != r_OK ) {
		_Message.Init();
		GetTranslation( Report, ErrorSet, LocaleRack(), _Message );
		Status = sError;
		ERRReturn;
	} else
		Status = sOK;
ERRErr
ERREnd
ERREpilog
	return Status;
}

report__ frdkrn::kernel___::SaveProject(
	const str::string_ &FileName,
	const char *TargetName,
	error_set___ &ErrorSet )
{
	report__ Report = r_Undefined;

	if ( ( Report = _DumpProjectRegistry( FileName, TargetName, _Id, ErrorSet ) ) == r_OK ) {
		_ProjectOriginalTimeStamp = time( NULL );
		_ProjectModificationTimeStamp = 0;
	}

	return Report;
}

status__ frdkrn::kernel___::SaveProject(
	const str::string_ &FileName,
	const char *TargetName  )
{
	status__ Status = s_Undefined;
ERRProlog
	error_set___ ErrorSet;
	report__ Report = r_Undefined;
ERRBegin
	ErrorSet.Init();

	if ( ( Report = SaveProject( FileName, TargetName, ErrorSet ) ) != r_OK ) {
		_Message.Init();
		GetTranslation( Report, ErrorSet, LocaleRack(), _Message );
		_Message.Append( " !" );
		Status = sError;
		ERRReturn;
	} else
		Status = sOK;
ERRErr
ERREnd
ERREpilog
	return Status;
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
			ERRc();	// 
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
