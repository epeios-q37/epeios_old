/*
	'emobdacgid' module by Claude SIMON (develop@logiplus.fr)

	 This file is part of 'emobda' software, which is part
	 of the Epeios project (http://zeusw.org/epeios/).

    'emobda' is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'emobda' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with 'emobda'.  If not, see <http://www.gnu.org/licenses/>.
*/

// $Id$

#include "emobdacgid.h"

#include "err.h"
#include "cio.h"
#include "csdbns.h"
#include "clnarg.h"
#include "xtf.h"
#include "csdsnc.h"
#include "lgg.h"
#include "cgimng.h"

#ifdef CPE__P_MS
#	include "wintol.h"
#	include "fnm.h"
#endif

#ifdef CPE__T_MT
cio::aware_cout___ AwareCout;
cio::aware_cerr___ AwareCerr;
#endif

txf::text_oflow__ *CErr = NULL;
txf::text_oflow__ *COut = NULL;

#define SERVICE_NAME	"EMobDaCGI"
#define SERVICE_LABEL	"EMobDa CGI frontend daemon"
#define SERVICE_DESCRIPTION	"Daemon handling EMobDa CGI requests."

#define DEFAULT_CONFIGURATION_FILENAME	"emobdacgid.xcf"


/* Beginning of the part which handles command line arguments. */

enum command {
	c_none,
	cHelp,
	cVersion,
	cLicense,
	cLaunch,
#ifdef CPE__P_MS
	cService,	// Installation comme service.
#endif
	c_amount
};

enum option {
	oConfiguration,
};

struct parameters___ {
	command Command;
	tol::E_FPOINTER___( char ) Configuration;
	parameters___( void )
	{
		Command = c_none;
	}
	~parameters___( void )
	{
	}
} Parameters;

static void PrintUsage( const clnarg::description_ &Description )
{
	*COut << DESCRIPTION << txf::nl;
	*COut << NAME << " --version|--license|--help" << txf::nl;
	clnarg::PrintCommandUsage( Description, cVersion, "print version of " NAME " components.", clnarg::vSplit, false, *COut );
	clnarg::PrintCommandUsage( Description, cLicense, "print the license.", clnarg::vSplit, false, *COut );
	clnarg::PrintCommandUsage( Description, cHelp, "print this message.", clnarg::vOneLine, false, *COut );
	*COut << NAME << " [" << Description.GetCommandLabels( cLaunch ) << "]";
	*COut << " [" << Description.GetOptionLabels( oConfiguration ) << " <file>]" << txf::nl;
	*COut << txf::tab << "Launch the CGI deamon, using configuration file 'file'.";
	*COut << txf::nl << txf::tab << "('" DEFAULT_CONFIGURATION_FILENAME "' by default)." << txf::nl;
#ifdef CPE__P_MS
	clnarg::PrintCommandUsage( Description, cService, "install as Windows Service, using configuration file '" DEFAULT_CONFIGURATION_FILENAME "'", clnarg::vSplit, false , *COut);
//	*COut << NAME << ' ' << Description.GetCommandLabels( cService ) << txf::nl;
//	*COut << txf::tab << "Install as Windows Service, using configuration file '" << DEFAULT_CONFIGURATION_FILENAME << "'." << txf::nl;
#endif
}

static void PrintHeader( void )
{
	*COut << NAME " V" VERSION " "__DATE__ " " __TIME__;
	*COut << " by "AUTHOR_NAME " (" AUTHOR_CONTACT ")" << txf::nl;
	*COut << COPYRIGHT << txf::nl;
	*COut << INFO << txf::nl;
	*COut << "CVS file details : " << CVS_DETAILS << txf::nl;
}


static void AnalyzeOptions(
	clnarg::analyzer___ &Analyzer,
	parameters___ &Parameters )
{
ERRProlog
	epeios::row__ P;
	clnarg::option_list Options;
	clnarg::id__ Option;
	const char *Unknow = NULL;
	clnarg::argument Argument;
ERRBegin
	Options.Init();

	if ( ( Unknow = Analyzer.GetOptions( Options ) ) != NULL ) {
		*CErr << '\'' << Unknow << "': unknow option." << txf::nl;
		*COut << HELP << txf::nl;
		ERRi();
	}

	P = Options.First();

	while( P != NONE ) {
		Argument.Init();
		switch( Option = Options( P ) ) {
		case oConfiguration:
			Analyzer.GetArgument( Option, Argument );
			if ( Argument.Amount() == 0 ) {
				*CErr << "'" << Analyzer.Description().GetOptionLabels( oConfiguration ) << "' option must have an argument!" << txf::nl;
				ERRExit( evArguments );
			}
			Argument.Convert( Parameters.Configuration );
			break;
		default:
			ERRc();
		}

		P = Options.Next( P );
	}

ERRErr
ERREnd
ERREpilog
}

static void AnalyzeFreeArguments(
	clnarg::analyzer___ &Analyzer,
	parameters___ &Parameters )
{
ERRProlog
	clnarg::arguments Free;
	epeios::row__ P;
ERRBegin
	Free.Init();

	Analyzer.GetArguments( Free );

	P = Free.Last();

	switch ( Parameters.Command ) {
	case cLaunch:
#ifdef CPE__P_MS
	case cService:
#endif
		if ( Free.Amount() != 0 ) {
			*CErr << "Too many arguments." << txf::nl;
			*COut << "Try '" NAME " --help' to get more informations." << txf::nl;
			ERRExit( evArguments );
		}
		break;
	default:
		ERRc();
		break;
	}

ERRErr
ERREnd
ERREpilog
}



static void AnalyzeArgs(
	int argc,
	const char *argv[],
	parameters___ &Parameters )
{
ERRProlog
	clnarg::description Description;
	clnarg::analyzer___ Analyzer;
ERRBegin
	Description.Init();

	Description.AddCommand( 'l', "launch", cLaunch );
#ifdef CPE__P_MS
	Description.AddCommand( CLNARG_NO_SHORT, "service", cService );
#endif
	Description.AddCommand( CLNARG_NO_SHORT, "version", cVersion );
	Description.AddCommand( CLNARG_NO_SHORT, "help", cHelp );
	Description.AddCommand( CLNARG_NO_SHORT, "license", cLicense );
	Description.AddOption( CLNARG_NO_SHORT, "cfg", oConfiguration );

	Analyzer.Init( argc, argv, Description );

	switch ( Parameters.Command = (command)Analyzer.GetCommand() ) {
	case cVersion:
		PrintHeader();
		TTR.Advertise( *COut );
		ERRi();
		break;
	case cHelp:
		PrintUsage( Description );
		ERRi();
		break;
	case cLicense:
		epsmsc::PrintLicense( *COut );
		ERRi();
		break;
	case CLNARG_NONE:
		Parameters.Command = cLaunch;
		break;
	default:
		if ( Parameters.Command >= c_amount )
			ERRc();
		break;
	}

	AnalyzeOptions( Analyzer, Parameters );

	AnalyzeFreeArguments( Analyzer, Parameters );

ERRErr
ERREnd
ERREpilog
}

/* End of the part which handles command line arguments. */



class user_functions__
: public cgimng::user_functions__
{
	virtual void CGIMNGHandle(
		const cgi_args_ &CGIArgs,
		cgi_tool___ &Tool,
		flw::oflow__ &Client )
	{
	ERRProlog
		cgi_params CGIParams;
		xmld Out;
		xmlf__ OutFiller;
		txf::text_oflow__ TFlow( Client );
		lgg::language__ Language = lgg::l_undefined;
		cgidat::data CGIData;
		bkdacc::id32__ CGIFacility = UNDEFINED_ID32;
		str::string XSLFileName;
		xmlval::vrow__ Root = NONE;
		str::string Version;
	ERRBegin
		CGIParams.Init();

		switch( cgitol::Fill( CGIParams, CGIArgs ) ) {
		case cgitol::sNoTarget:
			ERRx();
			break;
		case cgitol::sNoSession:
		case cgitol::sOK:
			break;
		default:
			ERRc();
			break;
		}

		Out.Init();

		CGIData.Init();
		XSLFileName.Init();

		Root = OutFiller.Init( Out, "gesbib" );

		Version.Init( VERSION " ("__DATE__ " " __TIME__ ") (" );
		Version.Append( gsbsessn::GetBackendAccessModeLabel( BackendAccessMode ) );
		Version.Append( ")" );

		OutFiller.PutAttribute( "FrontendVersion", Version  );

		Handle_( CGIArgs, BackendAccessMode, Tool, CGIParams.Session, Language, CGIData, CGIFacility, XSLFileName, Out, Root );

		DumpCGIContent_( CGIParams, CGIArgs, CGIData, CGIFacility, Language, OutFiller );

		OutFiller.PopTag();

		TFlow << XSLFileName << ' ' << lgg::GetLanguageLabel( Language != lgg::l_undefined ? Language : lgg::lRaw );
	
		xmlbsc::WriteXMLHeader( TFlow );
		xmldoc::WriteXML( Out, OutFiller.GetRoot(), TFlow );

	ERRErr
	//	AClient << "<br>" << ERRMessage() << "<br>";
	//	ERRRst();
	ERREnd
	ERREpilog
	}

	virtual void CGIMNGDelete( void *UP )
	{
		delete (context *)UP;
	}
	public:
	gsbsessn::backend_access_mode__ BackendAccessMode;
	user_functions__( void )
	{
		BackendAccessMode = gsbsessn::bam_Undefined;
	}
};

class csdsnc_log_functions__
: public csdsnc::log_functions__
{
private:
	cio::aware_cout___ _Flow;
protected:
	virtual void CSDSNCLog(
		csdsnc::log__ Log,
		const void *Flow,
		epeios::size__ Amount )
	{
		tol::buffer__ Buffer;

		if ( cio::IsInitialized() )
			_Flow << '[' << tol::DateAndTime( Buffer ) << "] : " << csdsnc::GetLogLabel( Log ) << txf::tab << Flow << txf::tab << Amount << txf::nl << txf::sync;
	}
};

class cgimng_log_functions__
: public cgimng::log_functions__
{
private:
	cio::aware_cout___ _Flow;
protected:
	virtual void CGIMNGLog(
		cgimng::log__ Log,
		const str::string_ &SessionId )
	{
		tol::buffer__ Buffer;

		if ( cio::IsInitialized() )
			_Flow << '[' << tol::DateAndTime( Buffer ) << "] : " << cgimng::GetLogLabel( Log ) << " '" <<  SessionId << "'." << txf::nl;
	}
};


#define ROOT_PATH		"Configurations" GSBRGSTRY_PATH_SEPARATOR "Configuration[target=\"gesbibcgi\"]" GSBRGSTRY_PATH_SEPARATOR

class kernel___
{
private:
	cgimng::cgi_manager _Manager;
	user_functions__ _UserFunctions;
public:
	void Init( 
		csdbns::service__ Service,
		bso::ushort__ Relative,
		bso::ushort__ Absolute,
		gsbsessn::backend_access_mode__ BackendAccessMode,
		cgimng::log_functions__ &LogFunctions )
	{
		_Manager.Init( Service, _UserFunctions, Relative, Absolute, LogFunctions );
		_UserFunctions.BackendAccessMode = BackendAccessMode;
	}
	void Process( sck::duration__ TimeOut )
	{
		_Manager.Process( TimeOut );
	}
};

#ifdef CPE__P_MS

class win_service__
: public wintol::service__
{
private:
	kernel___ _Kernel;
protected:
	virtual void WINTOLProcess( void )
	{
		while ( !TestTermination() ) {
			_Kernel.Process( 2500 );
		}

//		gsbdbm_::DisconnectFromFiles();	// Launched by the signal launched by an 'Exit fucntion'.
	}
	virtual void WINTOLShutdown( void )
	{
		// Nothing to do !
	}
public:
	void Init(
		csdbns::service__ Service,
		bso::ushort__ Relative,
		bso::ushort__ Absolute,
		gsbsessn::backend_access_mode__ BackendAccessMode,
		cgimng::log_functions__ &LogFunctions )	// Initialisation pour le LANCEMENT du service.
	{
		_Kernel.Init( Service, Relative, Absolute, BackendAccessMode, LogFunctions );
	}
	bso::bool__ Init( const char *ServiceName )	// Initialisation pur l'INSTALLATION du service.
	{
		return wintol::service__::Init( ServiceName );
	}
} WinService;

static void ProceedAsService(
	csdbns::service__ Service,
	bso::ushort__ Relative,
	bso::ushort__ Absolute,
	gsbsessn::backend_access_mode__ BackendAccessMode,
	cgimng::log_functions__ &LogFunctions )
{
ERRProlog
ERRBegin
	WinService.Init( Service, Relative, Absolute, BackendAccessMode, LogFunctions );

	WinService.Launch();
ERRErr
ERREnd
ERREpilog
}
#endif


void ProceedAsApp(
	csdbns::service__ Service,
	bso::ushort__ Relative,
	bso::ushort__ Absolute,
	gsbsessn::backend_access_mode__ BackendAccessMode,
	cgimng::log_functions__ &LogFunctions )
{
ERRProlog
	kernel___ Kernel;
ERRBegin
	Kernel.Init( Service, Relative, Absolute, BackendAccessMode, LogFunctions );
	*COut << "CGI daemon initialized." << txf::nl << txf::sync;
	Kernel.Process( SCK_INFINITE );
ERRErr
ERREnd
ERREpilog
}

static void Go(
	const parameters___ &Parameters,
	bso::bool__ AsWinService,
	const char *ExecutableLocalisation )
{
ERRProlog
	csdsnc_log_functions__ CSTSNCLogFunctions;
	cgimng_log_functions__ CGIMNGLogFunctions;
	cio::aware_cerr___ Flow;
	tol::E_FPOINTER___( char ) NotificationMessages;
	tol::E_FPOINTER___( char ) ErrorMessages;
	bso::bool__ Error = false;
	bso::ushort__ Service;
	bso::ushort__ Absolute;
	bso::ushort__ Relative;
	const char *ConfigurationFileName = Parameters.Configuration;
	rgstry::term_buffer Buffer;
	bso::bool__ Missing = false;
	gsbsessn::backend_access_mode__ BackendAccessMode = gsbsessn::bam_Undefined;
#ifdef CPE__P_MS
	FNM_BUFFER___ FileNameBuffer, TempBuffer;
	const char *Temp = NULL;
#endif
ERRBegin

	if ( Parameters.Configuration == NULL ) {
#ifdef CPE__P_MS
		if ( AsWinService || ( Parameters.Command == cService ) ) {
			if ( ExecutableLocalisation == NULL )
				ERRu();
			Temp = fnm::GetLocation( ExecutableLocalisation, TempBuffer );
			ConfigurationFileName = fnm::BuildFileName( Temp, DEFAULT_CONFIGURATION_FILENAME, NULL, FileNameBuffer );
		} else
#endif
			ConfigurationFileName = DEFAULT_CONFIGURATION_FILENAME;			
	}

	if ( !InitializeGlobalRegistryAndBackend( ConfigurationFileName, ROOT_PATH, CSTSNCLogFunctions, registry::Registry, registry::RegistryRoot, ::Core, Flow ) )
		ERRExit( evInitialization );

	BackendAccessMode = gsbmisc::GetBackendAccessMode( registry::Registry, registry::RegistryRoot );

	switch ( BackendAccessMode ) {
	case gsbsessn::bamReadOnly:
	case gsbsessn::bamReadWrite:
	case gsbsessn::bamAdmin:
		break;
	case gsbsessn::bam_Undefined:
		BackendAccessMode = gsbsessn::bamReadOnly;
		break;
	case gsbsessn::bam_Unknown:
		Flow << "Bad value in configuration file for '" << registry::Paths.Backend.AccessMode << "' entry !" << txf::nl;
		ERRExit( evInitialization );
		break;
	default:
		ERRc();
		break;
	}

	registry::GetRegistryValue_( gsbrgstry::Paths.Messages.Notifications, registry::Registry, registry::RegistryRoot, Buffer, Missing ).Convert( NotificationMessages );
	registry::GetRegistryValue_( gsbrgstry::Paths.Messages.Errors, registry::Registry, registry::RegistryRoot, Buffer, Missing ).Convert( ErrorMessages );

	LoadMessages( NotificationMessages, ErrorMessages, *CErr );

	Missing = false;

	Service = registry::GetUShort_( registry::Paths.Service, registry::Registry, registry::RegistryRoot, 0, Missing, Error );

	if ( Missing || Error ) {
		Flow << "No or bad value in configuration file for '" << gsbrgstry::Paths.Service << "' entry !" << txf::nl;
		ERRExit( evInitialization );
	}

	Relative = registry::GetUShort_( registry::Paths.Durations.Relative, registry::Registry, registry::RegistryRoot, DEFAULT_RELATIVE_DURATION, Missing, Error );

	if ( Missing )
		Flow << "No value in configuration file for '" << registry::Paths.Durations.Relative << "' entry !" << txf::nl;
	else if ( Error ) {
		Flow << "Bad value in configuration file for '" << registry::Paths.Durations.Relative << "' entry !" << txf::nl;
		ERRExit( evInitialization );
	}

	Absolute = registry::GetUShort_(registry::Paths.Durations.Absolute, registry::Registry, registry::RegistryRoot, DEFAULT_ABSOLUTE_DURATION, Missing, Error );

	if ( Missing )
		Flow << "No value in configuration file for '" << registry::Paths.Durations.Absolute << "' entry !" << txf::nl;
	else if ( Error ) {
		Flow << "Bad value in configuration file for '" << registry::Paths.Durations.Absolute << "' entry !" << txf::nl;
		ERRExit( evInitialization );
	}

	if ( ( gsbrgstry::LOGOFlowFunctions = GetRegistryAccessesLogFlow( registry::Registry, registry::RegistryRoot, Flow ) ) == NULL )
		ERRExit( evInitialization );

#ifdef CPE__P_MS
	if ( AsWinService ){
		if ( !WinService.Init( SERVICE_NAME ) ) {
			*CErr << "Unable to initialize the service !" << txf::nl;
			ERRExit( EXIT_FAILURE );
		}
		ProceedAsService( Service, Relative, Absolute, BackendAccessMode, CGIMNGLogFunctions );
	} else
#endif
		ProceedAsApp( Service, Relative, Absolute, BackendAccessMode, CGIMNGLogFunctions );
ERRErr
ERREnd
ERREpilog
}

static inline void Main(
	int argc,
	const char *argv[] )
{
ERRProlog
	parameters___ Parameters;
	bso::bool__ AsWinService = false;
ERRBegin
	AsWinService = argc <= 1;

#ifdef CPE__T_MS
	if( !AsWinService )
		cio::Initialize();	/* Since, when compiled for Linux, 'CIO_NO_AUTOMATIC_INITIALIZATION'
							isn't defined, this is automatically made by the 'CIO' library. */
#endif

	if ( cio::IsInitialized() ) {
#ifdef CPE__T_MT
		CErr = &AwareCerr;
		COut = &AwareCout;
#else
		CErr = &cio::cerr;
		COut = &cio::cout;
#endif
	} else {
		CErr = &txf::nul;
		COut = &txf::nul;
	}

	AnalyzeArgs( argc, argv, Parameters );

	switch ( Parameters.Command ) {
	case cNotificationMessages:
		DumpRawNotificationMessages( *COut );
		break;
	case cErrorMessages:
		DumpRawErrorMessages( *COut );
		break;
#ifdef CPE__P_MS
	case cService:
		if ( !WinService.Init( SERVICE_NAME ) ) {
			*CErr << "Unable to initialize the service !" << txf::nl;
			ERRExit( EXIT_FAILURE );
		};

		if ( !WinService.Install( SERVICE_LABEL, SERVICE_DESCRIPTION ) ) {
			*CErr << "Unable to install service !" << txf::nl;
			ERRExit( EXIT_FAILURE );
		}
		break;
#endif
	case cLaunch:
		Go( Parameters, AsWinService, argv[0] );
		break;
	default:
		ERRc();
		break;
	}
ERRErr
ERREnd
ERREpilog
}

int main(
	int argc,
	const char *argv[] )
{
ERRFProlog
ERRFBegin
	Main( argc, argv );
ERRFErr
ERRFEnd
ERRFEpilog
	return ERRExitValue;
}

