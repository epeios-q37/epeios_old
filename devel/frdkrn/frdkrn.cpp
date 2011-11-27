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

#define DAEMON_BACKEND_TYPE		"daemon"
#define LIBRARY_BACKEND_TYPE	"library"

#define CASE( m )\
	case r##m:\
	return #m;\
	break

const char *frdkrn::GetLabel( report__ Report )
{
#if FRDKRN__R_AMOUNT != 7
#	error "'report__' modified !"
#endif

	switch( Report ) {
		CASE( ProjectParsingError );
		CASE( NoOrBadProjectId );
		CASE( NoOrBadBackendDefinition );
		CASE( NoBackendLocation );
		CASE( UnableToConnect );
		CASE( IncompatibleBackend );
		CASE( BackendError );
		default:
			ERRu();
			break;
	}

	return NULL;	// Pour éviter un 'warning'.
}


#if FRDKRN__R_AMOUNT != 7
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

csducl::type__ frdkrn::GetBackendType( const frdrgy::registry_ &Registry )
{
	csducl::type__ Type = csducl::t_Undefined;
ERRProlog
	str::string Target;
ERRBegin
	Target.Init();

	frdrgy::GetBackendType( Registry, Target );

	if ( Target == DAEMON_BACKEND_TYPE )
		Type = csducl::tDaemon;
	else if ( Target == LIBRARY_BACKEND_TYPE )
		Type = csducl::tLibrary;
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
	switch ( Type = GetBackendType( _Registry ) ) {
	case csducl::tLibrary:
	case csducl::tDaemon:
		Location.Init();
		if ( !frdrgy::GetBackendLocation( _Registry, Location ) ) {
			Report = rNoBackendLocation;
			ERRReturn;
		}

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

static const str::string_ &Report_(
	report__ Report,
	const error_set___ &ErrorSet,
	const lcl::rack__ &Locale,
	str::string_ &Message )
{
	GetTranslation( Report, ErrorSet, Locale, Message );

	switch ( Report ) {
	case rProjectParsingError:
		break;
	case rNoOrBadBackendDefinition:
		break;
	case rNoBackendLocation:
		break;
	case rUnableToConnect:
		break;
	case rIncompatibleBackend:
		break;
	default:
		ERRc();
		break;
	}

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
	const char *Target,
	const str::string_ &Id,
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


#define PARAMETERS_ROOT_PATH	"Parameters"

report__ frdkrn::kernel___::_FillParametersRegistry(
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
		Report = rProjectParsingError;
		ERRReturn;
		break;
	}

	Report = r_OK;
ERRErr
ERREnd
ERREpilog
	return Report;
}

class parameters_
{
public:
	struct s {
		str::string_::s
			Id,
			Content;
	};
	str::string_
		Id,
		Content;
	parameters_( s &S )
	: Id( S.Id ),
	  Content( S.Content )
	{}
	void reset( bso::bool__ P = true )
	{
		Id.reset( P );
		Content.reset( P );
	}
	void plug( mmm::E_MULTIMEMORY_ &MM )
	{
		Id.plug( MM );
		Content.plug( MM );
	}
	parameters_ &operator =( const parameters_ &P )
	{
		Id = P.Id;
		Content = P.Content;

		return *this;
	}
	void Init( void )
	{
		Id.Init();
		Content.Init();
	}
};

E_AUTO( parameters )

static void Write_(
	const parameters_ &Parameters,
	flw::oflow__ Flow )
{
	Parameters.Id.NewWriteToFlow( Flow, true );
	Parameters.Content.NewWriteToFlow( Flow, true );
}

static void Read_(
	flw::iflow__ &Flow,
	parameters_ &Parameters )
{
	Parameters.Id.NewReadFromFlow( Flow, 0 );
	Parameters.Content.NewReadFromFlow( Flow, 0 );
}

typedef ctn::E_XCONTAINER_( parameters_ ) parameters_set_;
E_AUTO( parameters_set );

static void Write_(
	const parameters_set_ &Set,
	flw::oflow__ &Flow )
{
	epeios::row__ Row = Set.First();
	ctn::E_CITEM( parameters_ ) Parameters;

	dtfptb::NewPutSize( Set.Amount(), Flow );

	Parameters.Init( Set );

	while ( Row != NONE ) {
		Write_( Parameters( Row ), Flow );

		Row = Set.Next( Row );
	}
}

static void Read_(
	flw::iflow__ &Flow,
	parameters_set_ &Set )
{
ERRProlog
	epeios::size__ Amount = 0;
	parameters Parameters;
ERRBegin
	Amount = dtfptb::NewGetSize( Flow );

	while ( Amount-- != 0 ) {
		Parameters.Init();

		Read_( Flow, Parameters );

		Set.Append( Parameters );
	}
ERRErr
ERREnd
ERREpilog
}

static epeios::row__ Search_(
	const str::string_ &Id,
	const parameters_set_ &Set )
{
	epeios::row__ Row = Set.First();
	ctn::E_CITEM( parameters_ ) Parameters;

	Parameters.Init( Set );

	while ( ( Row != NONE ) && ( Parameters( Row ).Id != Id ) )
		Row = Set.Next( Row );

	return Row != NULL;
}

static bso::bool__ Get_(
	const str::string_ &Id,
	const parameters_set_ &Set,
	str::string_ &Content )
{
	epeios::row__ Row = Search_( Id, Set );
	ctn::E_CITEM( parameters_ ) Parameters;

	if ( Row != NONE ) {
		Parameters.Init( Set );

		Content = Parameters( Row ).Content;
	}

	return Row != NONE;
}

static void Set_(
	const str::string_ &Id,
	const str::string_ &Content,
	parameters_set_ &Set )
{
ERRProlog
	epeios::row__ Row = NONE;
	parameters Parameters;
ERRBegin
	Row = Search_( Id, Set );

	Parameters.Init();

	Parameters.Id = Id;
	Parameters.Content = Content;

	if ( Row != NONE )
		Set.Store( Parameters, Row );
	else
		Set.Append( Parameters );
ERRErr
ERREnd
ERREpilog
}

report__ frdkrn::kernel___::LoadProject(
	const str::string_ &FileName,
	const char *TargetName,
	flw::iflow__ &ParametersSetFlow,
	const xpp::criterions___ &Criterions,
	const compatibility_informations__ &CompatibilityInformations,
	error_reporting_functions__ &ErrorReportingFunctions,
	error_set___ &ErrorSet )
{
	report__ Report = r_Undefined;
ERRProlog
	str::string ProjectId;
	str::string Parameters;
	parameters_set Set;
	flx::E_STRING_IFLOW__ Flow;
	xtf::extended_text_iflow__ XFlow;
ERRBegin
	ProjectId.Init();

	if ( ( Report = _FillProjectRegistry( FileName, TargetName, Criterions, ProjectId, ErrorSet ) ) == r_OK )
		if ( ( Report = _Connect( CompatibilityInformations, ErrorReportingFunctions, ErrorSet ) ) != r_OK )
			ERRReturn;

	Set.Init();
	Read_( ParametersSetFlow, Set );

	Parameters.Init();
	Get_( ProjectId, Set, Parameters );

	Flow.Init( Parameters );
	XFlow.Init( Flow );

	 if ( ( Report = _FillParametersRegistry( XFlow, Criterions, ErrorSet ) ) != r_OK )
		 ERRReturn;

	_ProjectOriginalTimeStamp = time( NULL );
	_ProjectModificationTimeStamp = 0;
ERRErr
ERREnd
ERREpilog
	return Report;
}

status__ frdkrn::kernel___::LoadProject(
	const str::string_ &FileName,
	const char *TargetName,
	flw::iflow__ &ParametersSetFlow,
	const xpp::criterions___ &Criterions,
	const compatibility_informations__ &CompatibilityInformations )
{
	status__ Status = s_Undefined;
ERRProlog
	error_set___ ErrorSet;
	report__ Report = r_Undefined;
ERRBegin
	ErrorSet.Init();

	if ( ( Report = LoadProject( FileName, TargetName, ParametersSetFlow, Criterions, CompatibilityInformations, *_ErrorReportingFunctions, ErrorSet ) ) != r_OK ) {
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

report__ frdkrn::kernel___::SaveProject(
	const str::string_ &FileName,
	const char *TargetName,
	flw::oflow__ &ParametersSetFlow,
	error_set___ &ErrorSet )
{
	report__ Report = r_Undefined;
ERRProlog
	str::string ProjectId;
	str::string Parameters;
	parameters_set Set;
	flx::E_STRING_IFLOW__ Flow;
	xtf::extended_text_iflow__ XFlow;
ERRBegin
	ProjectId.Init();

	if ( ( Report = _DumpProjectRegistry(
		if ( ( Report = _Connect( CompatibilityInformations, ErrorReportingFunctions, ErrorSet ) ) != r_OK )
			ERRReturn;

	Set.Init();
	Read_( ParametersSetFlow, Set );

	Parameters.Init();
	Get_( ProjectId, Set, Parameters );

	Flow.Init( Parameters );
	XFlow.Init( Flow );

	 if ( ( Report = _FillParametersRegistry( XFlow, Criterions, ErrorSet ) ) != r_OK )
		 ERRReturn;

	_ProjectOriginalTimeStamp = time( NULL );
	_ProjectModificationTimeStamp = 0;
ERRErr
ERREnd
ERREpilog
	return Report;
}

status__ frdkrn::kernel___::SaveProject(
	const str::string_ &FileName,
	const char *TargetName,
	flw::oflow__ &ParametersSetFlow )
{
	status__ Status = s_Undefined;
ERRProlog
	error_set___ ErrorSet;
	report__ Report = r_Undefined;
ERRBegin
	ErrorSet.Init();

	if ( ( Report = LoadProject( FileName, TargetName, ParametersSetFlow, Criterions, CompatibilityInformations, *_ErrorReportingFunctions, ErrorSet ) ) != r_OK ) {
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
