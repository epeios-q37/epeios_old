/*
	'edaemon' by Claude SIMON.
	(description)
	Copyright (C) 2011 Claude SIMON

	This file is part of the Epeios (http://zeusw.org/epeios/) project.

    This file is part of 'edaemon'.

    'edaemon' is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'edaemon' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with 'edaemon'.  If not, see <http://www.gnu.org/licenses/>.
*/

//$Id$

#include "edaemon.h"
#include "locale.h"
#include "registry.h"

#include "scltool.h"
#include "sclerror.h"

#include "err.h"
#include "cio.h"
#include "epsmsc.h"
#include "clnarg.h"
#include "dir.h"
#include "mtk.h"

#include "csdleo.h"
#include "csdbns.h"
#include "csdsns.h"
#include "csdlec.h"

using namespace locale;

using cio::CErr;
using cio::COut;
using cio::CIn;

/* Beginning of the part which handles command line arguments. */

enum command__ {
	cHelp,
	cVersion,
	cLicense,
	c_amount,
	c_Undefined
};

enum option__ {
	o_amount,
	o_Undefined,
};

struct parameters {
	parameters( void )
	{
	}
};

static void PrintSpecialsCommandsDescriptions_(	const clnarg::description_ &Description,
	const lcl::locale_ &Locale,
	const char *Language )
{
ERRProlog
	CLNARG_BUFFER__ Buffer;
	lcl::meaning Meaning;
	str::string Translation;
ERRBegin
	Translation.Init();
	COut << Locale.GetTranslation( "ProgramDescription", Language, Translation ) << '.'  << txf::nl;
	COut << txf::nl;

	COut << NAME " " << Description.GetCommandLabels( cVersion, Buffer ) << txf::nl;
	Meaning.Init();
	clnarg::GetVersionCommandDescription( Meaning );
	Translation.Init();
	Locale.GetTranslation( Meaning, Language, Translation );
	COut << txf::pad << Translation << '.' << txf::nl;

	COut << NAME " " << Description.GetCommandLabels( cLicense, Buffer ) << txf::nl;
	Meaning.Init();
	clnarg::GetLicenseCommandDescription( Meaning );
	Translation.Init();
	Locale.GetTranslation( Meaning, Language, Translation );
	COut << txf::pad << Translation << '.' << txf::nl;

	COut << NAME " " << Description.GetCommandLabels( cHelp, Buffer ) << txf::nl;
	Meaning.Init();
	clnarg::GetHelpCommandDescription( Meaning );
	Translation.Init();
	Locale.GetTranslation( Meaning, Language, Translation );
	COut << txf::pad << Translation << '.' << txf::nl;
ERRErr
ERREnd
ERREpilog
}

#pragma warning ( disable : 4101 ) 
static void PrintUsage_( const clnarg::description_ &Description )
{
ERRProlog
ERRBegin
	PrintSpecialsCommandsDescriptions_( Description, scllocale::GetLocale(), scltool::GetLanguage() );
ERRErr
ERREnd
ERREpilog
}
#pragma warning ( default : 4101 ) 

static void PrintHeader_( void )
{
	COut << NAME " V" VERSION << " (" APP_URL ")" << txf::nl;
	COut << COPYRIGHT << txf::nl;
	COut << txf::pad << "Build : "__DATE__ " " __TIME__ << txf::nl;
}

static void AnalyzeOptions_(
	clnarg::analyzer___ &Analyzer,
	parameters &Parameters )
{
ERRProlog
//	mdr::row__ P;
	clnarg::option_list Options;
//	clnarg::id__ Option;
	const bso::char__ *Unknown = NULL;
	clnarg::argument Argument;
//	clnarg::buffer__ Buffer;
ERRBegin
	Options.Init();

	if ( ( Unknown = Analyzer.GetOptions( Options ) ) != NULL )
		clnarg::ReportUnknownOptionError( Unknown, NAME, scllocale::GetLocale(), scltool::GetLanguage() );
#if 0
	P = Options.First();

	while( P != NONE ) {
		Argument.Init();

		switch( Option = Options( P ) ) {
		default:
			ERRc();
			break;
		}

		P = Options.Next( P );
	}
#endif
ERRErr
ERREnd
ERREpilog
}

static void AnalyzeFreeArguments_(
	clnarg::analyzer___ &Analyzer,
	parameters &Parameters )
{
ERRProlog
	clnarg::arguments Free;
	sdr::row__ P;
ERRBegin
	Free.Init();

	Analyzer.GetArguments( Free );

	P = Free.Last();

	switch( Free.Amount() ) {
	case 0:
		break;
	default:
		clnarg::ReportWrongNumberOfArgumentsError( NAME, scllocale::GetLocale(), scltool::GetLanguage() );
		break;
	}

ERRErr
ERREnd
ERREpilog
}

static void AnalyzeArgs_(
	int argc,
	const char *argv[],
	parameters &Parameters )
{
ERRProlog
	clnarg::description Description;
	clnarg::analyzer___ Analyzer;
ERRBegin
	Description.Init();

//	Description.AddCommand( '', "", c );
	Description.AddCommand( CLNARG_NO_SHORT, "version", cVersion );
	Description.AddCommand( CLNARG_NO_SHORT, "help", cHelp );
	Description.AddCommand( CLNARG_NO_SHORT, "license", cLicense );
//	Description.AddOption( '', "", o );

	Analyzer.Init( argc, argv, Description );

	switch ( Analyzer.GetCommand() ) {
	case cVersion:
		PrintHeader_();
//		TTR.Advertise( COut );
		ERRExit( EXIT_SUCCESS );
		break;
	case cHelp:
		PrintUsage_( Description );
		ERRExit( EXIT_SUCCESS );
		break;
	case cLicense:
		epsmsc::PrintLicense( COut );
		ERRExit( EXIT_SUCCESS );
		break;
	case CLNARG_NONE:
		break;
	default:
		ERRFwk();
	}

	AnalyzeOptions_( Analyzer, Parameters );

	AnalyzeFreeArguments_( Analyzer, Parameters );

ERRErr
ERREnd
ERREpilog
}

/* End of the part which handles command line arguments. */

enum backend_connection_type__ {
	bctStraight,	
	bctSwitched,
	bct_amount,
	bct_Undefined
};

static backend_connection_type__ GetModuleConnectionType_( void )
{
	backend_connection_type__ Type = bct_Undefined;
ERRProlog
	str::string Value;
ERRBegin
	Value.Init();

	registry::GetRawModuleServiceType( Value );

	if ( Value == "Straight" )
		Type = bctStraight;
	else if ( Value == "Switched" )
		Type = bctSwitched;
	else {
		Value.Init();
		sclrgstry::ReportBadOrNoValueForEntryError( registry::ModuleServiceType );
		ERRExit( EXIT_FAILURE );
	}
ERRErr
ERREnd
ERREpilog
	return Type;
}

enum log_file_handling__ {
	lfhAppend,	// New log are appended to the current ones.
	lfhDrop,		// Old logs are destroyed.
	lfh_amount,
	lfh_Undefined,
};

static log_file_handling__ GetLogFileHandling_( void )
{
	log_file_handling__ Handling = lfh_Undefined;
ERRProlog
	str::string Value;
ERRBegin
	Value.Init();

	registry::GetRawModuleLogMode( Value );

	if ( ( Value.Amount() == 0 ) ||( Value == "Append" ) )
		Handling = lfhAppend;
	else if ( Value == "Drop" )
		Handling = lfhDrop;
	else {
		sclrgstry::ReportBadOrNoValueForEntryError( registry::ModuleLogMode );
		ERRExit( EXIT_FAILURE );
	}
ERRErr
ERREnd
ERREpilog
	return Handling;
}

static void UseStraightConnections_(
	csdsuf::user_functions__ &UserFunctions,
	const bso::char__ *Backend,
	csdbns::port__ Port )
{
ERRProlog
	csdbns::server___ Server;
ERRBegin
	Server.Init( Port, UserFunctions );

	Server.Process();
ERRErr
ERREnd
ERREpilog
}

static void UseSwitchingConnections_(
	csdsuf::user_functions__ &UserFunctions,
	csdsns::log_functions__ &LogFunctions,
	const bso::char__ *Backend,
	csdbns::port__ Port )
{
ERRProlog
	csdsns::server___ Server;
ERRBegin
	Server.Init( Port, UserFunctions, LogFunctions );

	Server.Process();
ERRErr
ERREnd
ERREpilog
}

typedef csdsns::log_functions__ _log_functions__;

class log_functions__
: public _log_functions__
{
private:
	txf::text_oflow__ *_Flow;
protected:
	virtual void CSDSNSLog(
		csdsns::log__ Log,
		csdsns::id__ Id,
		void *UP,
		sdr::size__ Amount )
	{
		tol::buffer__ Buffer;

		if ( _Flow == NULL )
			ERRFwk();

		*_Flow << '[' << tol::DateAndTime( Buffer ) << "] " << csdsns::GetLogLabel( Log ) << ' ' << Id << '/' << Amount << txf::nl << txf::commit;
	}
public:
	void reset( bso::bool__ P = true )
	{
		_log_functions__::reset( P );
		_Flow = NULL;
	}
	log_functions__( void )
	{
		reset( false );
	}
	~log_functions__( void )
	{
		reset();
	}
	void Init( txf::text_oflow__ &Flow )
	{
		_log_functions__::Init();
		_Flow = &Flow;
	}
};

static void UseSwitchingConnections_(
	csdsuf::user_functions__ &UserFunctions,
	const char *LogFileName,
	log_file_handling__ LogFileHandling,
	const bso::char__ *Backend,
	csdbns::port__ Port )
{
ERRProlog
	flf::file_oflow___ FFlow;
	txf::text_oflow__ TFlow;
	log_functions__ LogFunctions;
	fil::mode__ Mode = fil::m_Undefined;
	lcl::meaning ErrorMeaning;
	str::string ErrorTranslation;
ERRBegin

	if ( LogFileName != NULL ) {
		switch ( LogFileHandling ) {
		case lfhAppend:
			fil::mAppend;
			break;
		case lfhDrop:
			fil::mRemove;
			break;
		default:
			if ( LogFileName != NULL )
			ERRPrm();
			break;
		}

		if ( FFlow.Init( LogFileName, Mode, err::hUserDefined ) != fil::sSuccess ) {
			ErrorMeaning.Init();
			ErrorTranslation.Init();
			CErr << scllocale::GetTranslation( locale::GetUnableToOpenLogFileMeaning( LogFileName, ErrorMeaning), scltool::GetLanguage(), ErrorTranslation ) << txf::nl << txf::commit;
			LogFileName = NULL;	// To notify no to use log functions.
		} else
			TFlow.Init( FFlow );
	}

	UseSwitchingConnections_( UserFunctions, LogFileName == NULL ? *(csdsns::log_functions__ *)NULL : LogFunctions, Backend, Port );
ERRErr
ERREnd
ERREpilog
}

static csdlec::library_embedded_client_core__ *Go_(
	const bso::char__ *BackendFileName,
	csdbns::port__ Port,
	backend_connection_type__ ConnectionType,
	const char *LogFileName,
	log_file_handling__ LogFileHandling )
{
	csdlec::library_embedded_client_core__ *Core = NULL;
ERRProlog
	lcl::locale SharedLocale;
	rgstry::registry SharedRegistry;
	csdlec::library_data__ LibraryData;
	lcl::meaning Meaning;
	str::string Translation;
	err::buffer__ ErrBuffer;
ERRBegin
	SharedLocale.Init();
	SharedRegistry.Init();

	LibraryData.Init( csdleo::mRemote, cio::COutDriver, cio::CErrDriver, false, (void *)BackendFileName );

	if ( ( Core = new csdlec::library_embedded_client_core__ ) == NULL )
		ERRAlc();

	if ( !Core->Init( BackendFileName, LibraryData, err::hUserDefined ) ) {
		Meaning.Init();
		locale::GetUnableToLoadBackendMeaning( BackendFileName, Meaning );
		sclerror::SetMeaning( Meaning );
		ERRExit( EXIT_FAILURE );
	}

	switch ( ConnectionType ) {
	case bctStraight:
		UseStraightConnections_( Core->GetSteering(), BackendFileName, Port );
		break;
	case bctSwitched:
		UseSwitchingConnections_( Core->GetSteering(), LogFileName, LogFileHandling, BackendFileName, Port );
		break;
	default:
		ERRFwk();
		break;
	}
ERRErr
	Meaning.Init();
	Meaning.SetValue( "ModuleError" );

	if ( ERRType >= err::t_amount ) {
		if ( sclerror::IsErrorPending() )
			Meaning.AddTag( sclerror::GetMeaning() );
		else {
			Translation.Init();
			Meaning.AddTag( scllocale::GetLocale().GetTranslation( "UnkonwnError", scltool::GetLanguage(), Translation ) );
		}
	} else {
		Meaning.AddTag( err::Message( ErrBuffer ) );
	}

	Translation.Init();
	scllocale::GetTranslation( Meaning, scltool::GetLanguage(), Translation );

	cio::CErr << Translation << txf::nl << txf::commit;
ERREnd
ERREpilog
	return Core;
}

static inline csdbns::port__ GetService_( void )
{
	return registry::GetRawModuleService();
}

static csdlec::library_embedded_client_core__ *Go_(
	const char *LogFileName,
	log_file_handling__ LogFileHandling )
{
	csdlec::library_embedded_client_core__ *Core = NULL;
ERRProlog
	str::string ModuleFileName;
	STR_BUFFER___ Buffer;
ERRBegin
	ModuleFileName.Init();

	Core = Go_( registry::GetModuleFileName( ModuleFileName ).Convert( Buffer ), GetService_(), GetModuleConnectionType_(), LogFileName, LogFileHandling );
ERRErr
ERREnd
ERREpilog
	return Core;
}

static csdlec::library_embedded_client_core__ *Core_ = NULL;

static csdlec::library_embedded_client_core__ *Go_( void )
{
	csdlec::library_embedded_client_core__ *Core = NULL;
ERRProlog
	STR_BUFFER___ Buffer;
	const char *LogFileName = NULL;
ERRBegin
	Core = Go_( registry::GetModuleLogFileName( Buffer ), GetLogFileHandling_() );
ERRErr
ERREnd
ERREpilog
	return Core;
}

static void ExitFunction_( void )
{
	if ( Core_ != NULL )
		delete Core_;

	Core_ = NULL;
}


static void Go_( const parameters &Parameters )
{
	atexit( ExitFunction_ );

	Go_();

	while ( 1 )
		tht::Suspend( 1000 );
}

const char *scltool::TargetName = NAME;

void scltool::Main(
	int argc,
	const char *argv[] )
{
ERRProlog
	parameters Parameters;
ERRBegin
	AnalyzeArgs_( argc, argv, Parameters );

	Go_( Parameters );
ERRErr
ERREnd
ERREpilog
}
