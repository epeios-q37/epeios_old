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
	case s##m:\
	return #m;\
	break

const char *GetLabel_( status__ Status )
{
#if FRDKRN__S_AMOUNT != 8
#	error "'status__' modified !"
#endif

	switch( Status ) {
		CASE( ConfigurationParsingError );
		CASE( LocaleParsingError );
		CASE( NoLocaleFileDefined );
		CASE( ProjectParsingError );
		CASE( NoBackendDefined );
		CASE( NoBackendLocation );
		default:
			ERRu();
			break;
	}

	return NULL;	// Pour éviter un 'warning'.
}

const str::string_ &frdkrn::GetTranslation(
	status__ Status,
	const lcl::locale_rack___ &Locale,
	str::string_ &Translation )
{
ERRProlog
	str::string RawText;
ERRBegin
	RawText.Init( "FRDKRN_" );
	RawText.Append( GetLabel_( Status ) );

	Locale.GetTranslation( RawText,Translation );
ERRErr
ERREnd
ERREpilog
	return Translation;
}

const str::string_ &frdkrn::GetTranslation(
	status__ Status,
	const error_set___ &ErrorSet,
	const lcl::locale_rack___ &Locale,
	str::string_ &Translation )
{
	GetTranslation( Status, Locale, Translation );
	
	if ( IsErrorSetRelevant( Status ) ) {
		Translation.Append( " :\n" );
		GetTranslation( ErrorSet, Locale, Translation );
	}

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
	FileName.Append( FRDKRN_PROJECT_FILE_EXTENSION );

	Exists = fil::FileExists( FileName.Convert( Buffer ) );
ERRErr
ERREnd
ERREpilog
	return Exists;
}

/*
void frdkrn::kernel___::Init(
	const str::string_ &Language,
	const char *LocalesFileName )
{
ERRProlog
	flf::file_iflow___ FIFlow;
ERRBegin
	reset();

	_Language.Init( Language );

	_Registry.Init();

	if ( FIFlow.Init( LocalesFileName, err::hSkip ) == fil::sSuccess ) {
		FIFlow.EOFD( XTF_EOXT );

		_Locales.Init( FIFlow, "Locale[target=\"" FRDKERNL_FRONTEND_NAME "\"]" );
	}

	// Other initialisations happend in 'OpenProject'.
ERRErr
ERREnd
ERREpilog
	// Other initialisations happend in 'OpenProject'.
}
*/

/*
bso::bool__ frdkrn::kernel___::LoadConfigAndLocales(
	const char *ConfigFileName,
	const char *RootPath,
	const char *DefaultLocalesFileName,
	const char *LocalesRootPath,
	str::string_ &ErrorMessage )
{
	bso::bool__ NoProblem = false;
ERRProlog
	rgstry::error_details ErrorDetails;
	rgstry::error__ Error = rgstry::e_Undefined;
	str::string LocalesFileName;
	STR_BUFFER___ Buffer;
ERRBegin
	ErrorDetails.Init();

	if ( ( Error = _Registry.FillConfiguration( ConfigFileName, RootPath, ErrorDetails ) ) == rgstry::eOK ) {
		LocalesFileName.Init();

		if ( !frdrgy::GetLocalesFileName( _Registry, LocalesFileName ) )
			LocalesFileName.Init( DefaultLocalesFileName );

		Error = _Locales.Init( LocalesFileName.Convert( Buffer ), LocalesRootPath, ErrorDetails );
	}

	if ( Error != rgstry::eOK )
		rgstry::GetTranslation( Error, ErrorDetails, _Language, _Locales, ErrorMessage );
	else
		NoProblem = true;
ERRErr
ERREnd
ERREpilog
	return NoProblem;
}
*/

status__ frdkrn::kernel___::Init(
	const str::string_ &ConfigurationFileName,
	const char *TargetName,
	const str::string_ &Language,
	error_set___ &ErrorSet )
{
	status__ Status = s_Undefined;
ERRProlog
	str::string LocaleFileName;
	STR_BUFFER___ LocaleBuffer, PathBuffer, ConfigurationFileNameBuffer;
	str::string TargetAttribute;
	str::string Path;
ERRBegin
	_Registry.Init();
	_LocaleRack.Init( _Locale, Language );	// Initialisé dés mantenant bien que '_Locale' vide, pour pouvoir être utilisé par fonction appelante.

	TargetAttribute.Init( "[target=\"" );
	TargetAttribute.Append( TargetName );
	TargetAttribute.Append( "\"]" );

	Path.Init( "Configurations/Configuration" );
	Path.Append( TargetAttribute );

	if ( ( ErrorSet.Error = _Registry.FillConfiguration( ConfigurationFileName.Convert( ConfigurationFileNameBuffer ), Path.Convert( PathBuffer ), ErrorSet.Details ) ) != rgstry::eOK ) {
		Status = sConfigurationParsingError;
		ERRReturn;
	}

	_Locale.Init();

	LocaleFileName.Init();

	if ( frdrgy::GetLocalesFileName( _Registry, LocaleFileName ) ) {

		Path.Init( "Locales/Locale" );
		Path.Append( TargetAttribute );

		if ( ( ErrorSet.Error = _Locale.Init( LocaleFileName.Convert( LocaleBuffer ), Path.Convert( PathBuffer ), ErrorSet.Details ) ) != rgstry::eOK ) {
			Status = sLocaleParsingError;
			ERRReturn;
		}

		Status = sOK;
	} else
		Status = sNoLocaleFileDefined;

	// L'initialisation de '_Backend' et '_ClientCore' se fait à la connection.
ERRErr
ERREnd
ERREpilog
	return Status;
}

status__ frdkrn::kernel___::_Connect(
	const char *RemoteHostServiceOrLocalLibraryPath,
	csducl::type__ Type,
	csdsnc::log_functions__ &LogFunctions,
	frdkrn::error_reporting_functions___ &ErrorReportingFunctions )
{
	status__ Status = s_Undefined;
ERRProlog
ERRBegin
	if ( !_ClientCore.Init( RemoteHostServiceOrLocalLibraryPath, NULL, LogFunctions, Type ) ) {
		Status = sUnableToConnect;
		ERRReturn;
	}

	_Backend.Init( _ClientCore, ErrorReportingFunctions );

	Status = sOK;
ERRErr
ERREnd
ERREpilog
	return Status;
}

status__ frdkrn::kernel___::_Connect(
	const str::string_ &RemoteHostServiceOrLocalLibraryPath,
	csducl::type__ Type,
	csdsnc::log_functions__ &LogFunctions,
	frdbkd::error_reporting_functions___ &ErrorReportingFunctions )
{
	status__ Status = s_Undefined;
ERRProlog
	STR_BUFFER___ RemoteHostServiceOrLocalLibraryPathBuffer;
ERRBegin
	Status = _Connect( RemoteHostServiceOrLocalLibraryPath.Convert( RemoteHostServiceOrLocalLibraryPathBuffer ), Type, LogFunctions, ErrorReportingFunctions );
ERRErr
ERREnd
ERREpilog
	return Status;
}

status__ frdkrn::kernel___::Connect(
	csdsnc::log_functions__ &LogFunctions,
	error_reporting_functions___ &ErrorReportingFunctions )
{
	status__ Status = s_Undefined;
ERRProlog
	str::string Location;
	csducl::type__ Type = csducl::t_Undefined;
ERRBegin
	switch ( Type = GetBackendType( _Registry ) ) {
	case csducl::tDaemon:
	case csducl::tLibrary:
		Location.Init();
		if ( !frdrgy::GetBackendLocation( _Registry, Location ) ) {
			Status = sNoBackendLocation;
			ERRReturn;
		}

		Status = Connect( Location, Type, LogFunctions, ErrorFunctions );
		break;
	case csducl::t_Undefined:
		Status = sNoBackendDefined;
		break;
	default:
		ERRc();
		break;
	}
ERRErr
ERREnd
ERREpilog
	return Status;
}

void frdkrn::kernel___::FillUserRegistry( flw::iflow__ &User )
{
ERRProlog
	rgstry::error_details ErrorDetails;
ERRBegin
	ErrorDetails.Init();	

	_Registry.FillUser( User, NULL, ErrorDetails );

ERRErr
ERREnd
ERREpilog
}

status__ frdkrn::kernel___::FillProjectRegistry(
	const str::string_ &FileName,
	const char *Target,
	error_set___ &ErrorSet )
{
	status__ Status = s_Undefined;
ERRProlog
	str::string Path;
	STR_BUFFER___ FileNameBuffer, PathBuffer;
ERRBegin
	Path.Init( "Projects/Project[@target=\"" );
	Path.Append( Target );
	Path.Append( "]\"" );

	if ( ( ErrorSet.Error = _Registry.FillProject( FileName.Convert( FileNameBuffer ), Path.Convert( PathBuffer ), ErrorSet.Details ) ) != rgstry::eOK ) {
		Status = sProjectParsingError;
		ERRReturn;
	}

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
		if ( FRDKRN__S_AMOUNT != 6 )
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
