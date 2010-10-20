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

const char *GetLabel_( report__ Report )
{
#if FRDKRN__R_AMOUNT != 8
#	error "'report__' modified !"
#endif

	switch( Report ) {
		// CASE( OK );	// Cette fonction n'a pas à être appelée dans ce cas.
		CASE( ConfigurationParsingError );
		CASE( LocaleParsingError );
		CASE( NoLocaleFileDefined );
		CASE( ProjectParsingError );
		CASE( NoOrBadBackendDefinition );
		CASE( NoBackendLocation );
		CASE( UnableToConnect );
		default:
			ERRu();
			break;
	}

	return NULL;	// Pour éviter un 'warning'.
}

const str::string_ &frdkrn::GetTranslation(
	report__ Report,
	const lcl::locale_rack___ &Locale,
	str::string_ &Translation )
{
ERRProlog
	str::string RawText;
ERRBegin
	RawText.Init( "FRDKRN_" );
	RawText.Append( GetLabel_( Report ) );

	Locale.GetTranslation( RawText,Translation );
ERRErr
ERREnd
ERREpilog
	return Translation;
}

const str::string_ &frdkrn::GetTranslation(
	report__ Report,
	const error_set___ &ErrorSet,
	const lcl::locale_rack___ &Locale,
	str::string_ &Translation )
{
	GetTranslation( Report, Locale, Translation );
	
	if ( IsErrorSetRelevant( Report ) ) {
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

report__ frdkrn::kernel___::_LoadConfiguration(
	const str::string_ &FileName,
	const char *TargetName,
	error_set___ &ErrorSet )
{
	report__ Report = r_Undefined;
ERRProlog
	str::string Path;
	STR_BUFFER___ PathBuffer, FileNameBuffer;
ERRBegin
	Path.Init( "Configurations/Configuration" );
	AppendTargetAttributePathItem_( TargetName, Path );

	if ( ( ErrorSet.Error = _Registry.FillConfiguration( FileName.Convert( FileNameBuffer ), Path.Convert( PathBuffer ), ErrorSet.Details ) ) != rgstry::eOK ) {
		Report = rConfigurationParsingError;
		ERRReturn;
	}

	Report = rOK;
ERRErr
ERREnd
ERREpilog
	return Report;
}

report__ frdkrn::kernel___::_LoadLocale(
	const char *TargetName,
	error_set___ &ErrorSet )
{
	report__ Report = r_Undefined;
ERRProlog
	str::string FileName;
ERRBegin
	FileName.Init();

	if ( !frdrgy::GetLocalesFileName( _Registry, FileName ) )
		Report = rNoLocaleFileDefined;
	else
		Report = _LoadLocale( FileName, TargetName, ErrorSet );
ERRErr
ERREnd
ERREpilog
	return Report;
}

status__ frdkrn::kernel___::Init(
	const str::string_ &ConfigurationFileName,
	const char *TargetName,
	const str::string_ &Language )
{
	status__ Status = s_Undefined;
ERRProlog
	report__ Report = r_Undefined;
	frdkrn::error_set___ ErrorSet;
ERRBegin
	ErrorSet.Init();

	switch( Report = Init( ConfigurationFileName, TargetName, Language, ErrorSet ) ) {
	case rOK:
		Status = sOK;
		break;
	case rConfigurationParsingError:
		Status = sError;
		break;
	case rNoLocaleFileDefined:
		Status = sWarning;
		break;
	case rLocaleParsingError:
		Status = sError;
		break;
	default:
		ERRc();
		break;
	}

	if ( Report != sOK ) {
		_Message.Init();
		frdkrn::GetTranslation( Report, ErrorSet, Locale(), _Message );
		_Message.Append( " !" );
	}
ERRErr
ERREnd
ERREpilog
	return Status;
}

report__ frdkrn::kernel___::_Connect(
	const char *RemoteHostServiceOrLocalLibraryPath,
	csducl::type__ Type,
	frdkrn::error_reporting_functions___ &ErrorReportingFunctions,
	csdsnc::log_functions__ &LogFunctions )
{
	report__ Report = r_Undefined;
ERRProlog
ERRBegin
	if ( !_ClientCore.Init( RemoteHostServiceOrLocalLibraryPath, NULL, LogFunctions, Type ) ) {
		Report = rUnableToConnect;
		ERRReturn;
	}

	_Backend.Init( _ClientCore, ErrorReportingFunctions );

	Report = rOK;
ERRErr
ERREnd
ERREpilog
	return Report;
}

report__ frdkrn::kernel___::_Connect(
	const str::string_ &RemoteHostServiceOrLocalLibraryPath,
	csducl::type__ Type,
	frdbkd::error_reporting_functions___ &ErrorReportingFunctions,
	csdsnc::log_functions__ &LogFunctions )
{
	report__ Report = r_Undefined;
ERRProlog
	STR_BUFFER___ RemoteHostServiceOrLocalLibraryPathBuffer;
ERRBegin
	Report = _Connect( RemoteHostServiceOrLocalLibraryPath.Convert( RemoteHostServiceOrLocalLibraryPathBuffer ), Type, ErrorReportingFunctions, LogFunctions );
ERRErr
ERREnd
ERREpilog
	return Report;
}

report__ frdkrn::kernel___::_Connect(
	error_reporting_functions___ &ErrorReportingFunctions,
	csdsnc::log_functions__ &LogFunctions )
{
	report__ Report = r_Undefined;
ERRProlog
	str::string Location;
	csducl::type__ Type = csducl::t_Undefined;
ERRBegin
	switch ( Type = GetBackendType( _Registry ) ) {
	case csducl::tDaemon:
	case csducl::tLibrary:
		Location.Init();
		if ( !frdrgy::GetBackendLocation( _Registry, Location ) ) {
			Report = rNoBackendLocation;
			ERRReturn;
		}

		Report = _Connect( Location, Type, ErrorReportingFunctions, LogFunctions );
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

status__ frdkrn::kernel___::LoadProject(
	const str::string_ &FileName,
	const char *TargetName )
{
	status__ Status = s_Undefined;
ERRProlog
	error_set___ ErrorSet;
	report__ Report = r_Undefined;
ERRBegin
	ErrorSet.Init();

	if ( ( Report = LoadProject( FileName, TargetName, ErrorSet ) ) != rOK ) {
		_Message.Init();
		GetTranslation( Report, ErrorSet, Locale(), _Message );
		_Message.Append( " !" );
		Status = sError;
		ERRReturn;
	}

	Status = sOK;
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

report__ frdkrn::kernel___::_FillProjectRegistry(
	const str::string_ &FileName,
	const char *Target,
	error_set___ &ErrorSet )
{
	report__ Report = r_Undefined;
ERRProlog
	str::string Path;
	STR_BUFFER___ FileNameBuffer, PathBuffer;
ERRBegin
	Path.Init( "Projects/Project[@target=\"" );
	Path.Append( Target );
	Path.Append( "\"]" );

	if ( ( ErrorSet.Error = _Registry.FillProject( FileName.Convert( FileNameBuffer ), Path.Convert( PathBuffer ), ErrorSet.Details ) ) != rgstry::eOK ) {
		Report = rProjectParsingError;
		ERRReturn;
	}

	Report = rOK;
ERRErr
ERREnd
ERREpilog
	return Report;
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
