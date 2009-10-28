/*
	'emobdadmn.cpp' by Claude SIMON (http://zeusw.org/).

	 This file is part of 'emobda' software.

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

#include "bkdcore.h"
#include "registry.h"

#include "epsmsc.h"
#include "csdsns.h"
#include "clnarg.h"
#include "cio.h"

#define VERSION			BKDCORE_BACKEND_VERSION
#define COPYRIGHT_YEARS	BKDCORE_COPYRIGHT_YEARS

#define NAME			"emobdadmn"
#define DESCRIPTION		"Backend daemon for 'EMobDa' software"
#define INFO			EPSMSC_EPEIOS_TEXT
#define AUTHOR_NAME		EPSMSC_AUTHOR_NAME
#define AUTHOR_CONTACT	EPSMSC_AUTHOR_CONTACT
#define HELP			EPSMSC_HELP_INVITATION( NAME )
#define COPYRIGHT		EPSMSC_COPYRIGHT( COPYRIGHT_YEARS )
#define CVS_DETAILS		("$Id$\b " + 5)

#define DEFAULT_CONFIGURATION_FILENAME NAME ".xcf"
#define CONFIGURATION_ROOT_PATH	"Configuration[target=\"" NAME "\"]"

#define DEFAULT_LOCALES_FILENAME	NAME ".xlc"
#define LOCALES_PATH_ROOT	"Locale[target=\"" NAME "\"]"

#define SERVICE_NAME	NAME
#define SERVICE_LABEL	"EMobDa backend daemon"
#define SERVICE_DESCRIPTION	"Daemon handling 'EMobDa' frontend requests."

const char *bkdmnger::UserDefinedBackendVersion = NAME " V" VERSION" (" __DATE__ " "  __TIME__ ")";


#ifdef CPE__T_MT
cio::aware_cout___ AwareCout;
cio::aware_cerr___ AwareCerr;
#endif

txf::text_oflow__ *CErr = NULL;
txf::text_oflow__ *COut = NULL;


/* Beginning of the part which handles command line arguments. */

enum command {
	cHelp,
	cVersion,
	cLicense,
	cLaunch,	// Launch the deamon.
#ifdef CPE__P_MS
	cService,	// Installe comme service.
#endif
//	cRemove,	// Enlève le service.
	c_amount,
	c_Undefined,
};

enum option {
	oConfiguration,
};

enum exit_value__ {
	evParameters = 2,
	evConfiguration,
};

struct parameters___ {
	command Command;
	STR_BUFFER___ ConfigurationFileBuffer;
	const char *ConfigurationFile;
	parameters___( void )
	{
		Command = c_Undefined;
		ConfigurationFile = NULL;
	}
};

static void PrintUsage( const clnarg::description_ &Description )
{
	*COut << DESCRIPTION << txf::nl;
	*COut << NAME << " --version|--license|--help" << txf::nl;
	clnarg::PrintCommandUsage( Description, cVersion, "print version of " NAME " components.", clnarg::vSplit, false, *COut );
	clnarg::PrintCommandUsage( Description, cLicense, "print the license.", clnarg::vSplit, false, *COut );
	clnarg::PrintCommandUsage( Description, cHelp, "print this message.", clnarg::vOneLine, false, *COut );
//	clnarg::PrintCommandUsage( Description, cRemove, "remove the service.", clnarg::vOneLine, false, *COut );
	*COut << NAME << " [" << Description.GetCommandLabels( cLaunch ) << "]";
	*COut << " [" << Description.GetOptionLabels( oConfiguration ) << " <file>]";
	*COut << txf::nl;
	*COut << txf::tab << "Launch the backend deamon, using configuration file 'file'.";
	*COut << txf::nl << txf::tab << "('" DEFAULT_CONFIGURATION_FILENAME "' by default)." << txf::nl;;
#ifdef CPE__P_MS
	*COut << NAME << ' ' << Description.GetCommandLabels( cService ) << txf::nl;
	*COut << txf::tab << "Install as Windows Service, using configuration file '" << DEFAULT_CONFIGURATION_FILENAME << "'." << txf::nl;
#endif
	*COut << txf::tab << "Displays the messages and exits." << txf::nl;
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
				ERRExit( evParameters );
			}
			Parameters.ConfigurationFile = Argument.Convert( Parameters.ConfigurationFileBuffer );
			break;
		default:
			ERRc();
			break;
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

	switch( Free.Amount() ) {
	case 0:
		break;
	default:
		*CErr << txf::nl << "Bad amount of argument." << txf::nl;
		ERRExit( evParameters) ;
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

//	Description.AddOption( '', "", o );
	Description.AddCommand( CLNARG_NO_SHORT, "launch", cLaunch );
#ifdef CPE__P_MS
	Description.AddCommand( CLNARG_NO_SHORT, "service", cService );
#endif
//	Description.AddCommand( CLNARG_NO_SHORT, "remove", cRemove );
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
	case cLaunch:
		Parameters.Command = cLaunch;
		break;
#ifdef CPE__P_MS
	case cService:
		Parameters.Command = cService;
		break;
#endif
	default:
		ERRc();
	}

	AnalyzeOptions( Analyzer, Parameters );
	AnalyzeFreeArguments( Analyzer, Parameters );
ERRErr
ERREnd
ERREpilog
}

/* End of the part which handles command line arguments. */


class log_functions__
: public csdsns::log_functions__
{
	virtual void CSDSNSLog(
		csdsns::log__ Log,
		csdsns::id__ Id,
		void *UP,
		epeios::size__ Amount )
	{
	}
public:
} LogFunctions;

class kernel___
{
public:
	csdsns::core Core;
	csdsns::server___ Server;
	bkdcore::backend_functions__ Functions;
	void Init(
		csdsns::service__ Service,
		const lcl::locales_ &Locales )
	{
		Functions.Init( Locales );
		Core.Init( ::LogFunctions );
		Server.Init( Service, Functions, ::LogFunctions );
	}
	void Process( sck::duration__ TimeOut )
	{
		Server.Process( TimeOut );
	}

} Kernel_;

void Main(
	int argc,
	const char *argv[] )
{
ERRProlog
	parameters___ Parameters;
	csdsns::service__ Service = 0;
	str::string Repository;
	bso::bool__ AsWinService = false;
	lcl::locales Locales;
	rgstry::error__ Error = rgstry::e_Undefined;
	rgstry::error_details ErrorDetails;
	str::string Message;
ERRBegin
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

	if ( !registry::FillRegistry( Parameters.ConfigurationFile == NULL ? DEFAULT_CONFIGURATION_FILENAME : Parameters.ConfigurationFile, CONFIGURATION_ROOT_PATH, *CErr ) )
		ERRExit( evConfiguration );

	if ( !registry::GetService( Service ) ) {
		*CErr << "Unable to find '" << registry::paths::Service << "'entry in configuration file !";
		ERRExit( evConfiguration );
	}

	Repository.Init();

	if ( !registry::GetRepository( Repository ) ) {
		*CErr << "Unable to find '" << registry::GetRepositoryPath() << "' entry in configuration file !" << txf::nl;
		ERRExit( evConfiguration );
	}

	ErrorDetails.Init();

	switch( Error = Locales.Init( DEFAULT_LOCALES_FILENAME, LOCALES_PATH_ROOT, ErrorDetails ) ) {
	case rgstry::eOK:
		break;
	default:
		Message.Init();
		// Weird 'language' parameter, because, if we are here, it's because the 'locales' file could not be correctly open
		// so no message translations are available ...
		*CErr << "Failed to load '" DEFAULT_LOCALES_FILENAME "' locales file : " << rgstry::GetTranslation( Error, ErrorDetails, str::string( "" ), Locales, Message ) << txf::nl;
		break;
	}

	::Kernel_.Init( Service, Locales );

	::Kernel_.Process( SCK_INFINITE );
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
