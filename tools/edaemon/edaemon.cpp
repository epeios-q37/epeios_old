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

#include  "scltool.h"

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

static void PrintSpecialsCommandsDescriptions_(
	const clnarg::description_ &Description,
	const lcl::rack__ LocaleRack )
{
ERRProlog
	str::string Text;
	CLNARG_BUFFER__ Buffer;
ERRBegin
	COut << GetTranslation( "ProgramDescription" ) << '.'  << txf::nl;
	COut << txf::nl;

	COut << NAME " " << Description.GetCommandLabels( cVersion, Buffer ) << txf::nl;
	Text.Init();
	clnarg::GetVersionCommandDescription( LocaleRack, Text );
	COut << txf::pad << Text << '.' << txf::nl;

	COut << NAME " " << Description.GetCommandLabels( cLicense, Buffer ) << txf::nl;
	Text.Init();
	clnarg::GetLicenseCommandDescription( LocaleRack, Text );
	COut << txf::pad << Text << '.' << txf::nl;

	COut << NAME " " << Description.GetCommandLabels( cHelp, Buffer ) << txf::nl;
	Text.Init();
	clnarg::GetHelpCommandDescription( LocaleRack, Text );
	COut << txf::pad << Text << '.' << txf::nl;

ERRErr
ERREnd
ERREpilog
}

#pragma warning ( disable : 4101 ) 
static void PrintUsage_(
	const clnarg::description_ &Description,
	const lcl::rack__ &LocaleRack )
{
ERRProlog
	STR_BUFFER___ TBuffer;
	CLNARG_BUFFER__ Buffer;
ERRBegin
	PrintSpecialsCommandsDescriptions_( Description, LocaleRack );

#if 0	// Exemples.
// Commands.
	COut << NAME << " [" << Description.GetCommandLabels( cProcess, Buffer );
	COut << "] [" << Description.GetOptionLabels( oNamespace, Buffer ) << " <ns>]";
	COut << " [" << Description.GetOptionLabels( oNoIndent, Buffer );
	COut << "] [<src> [<dst>]]";
	COut << txf::nl;
	COut << txf::pad << global::GetTranslation( global::mProcessCommandDescription ) << '.' << txf::nl;

	COut << NAME << ' ' << Description.GetCommandLabels( cEncrypt, Buffer );
	COut << " [" << Description.GetOptionLabels( oNamespace, Buffer ) << " <ns>]";
	COut << " [" << Description.GetOptionLabels( oNoIndent, Buffer );
	COut << "] [<src> [<dst>]]";
	COut << txf::nl;
	COut << txf::pad << global::GetTranslation( global::mEncryptCommandDescription ) << '.' << txf::nl;

	COut << txf::nl;

// Options.
	COut << clnarg::GetOptionsWordingTranslation( LocaleRack );
	COut << " :" << txf::nl;

	COut << txf::pad << Description.GetOptionLabels( oNamespace, Buffer ) << " <ns> :" << txf::nl;
	COut << txf::tab;
	global::Display( mNamespaceOptionDescription );
	COut << '.' << txf::nl;

	COut << txf::pad << Description.GetOptionLabels( oNoIndent, Buffer ) << " :" << txf::nl;
	COut << txf::tab;
	global::Display( mNoIndentOptionDescription );
	COut << '.' << txf::nl;

	COut << txf::nl;

// Arguments.
	COut << clnarg::GetArgumentsWordingTranslation( LocaleRack );
	COut << " :" << txf::nl;

	COut << txf::pad << "<src> :" << txf::nl;
	COut << txf::tab;
	global::Display( mSourceFileArgumentDescription );
	COut << '.' << txf::nl;

	COut << txf::pad << "<dst> :" << txf::nl;
	COut << txf::tab;
	global::Display( mDestFileArgumentDescription );
	COut << '.' << txf::nl;
#endif

ERRErr
ERREnd
ERREpilog
}
#pragma warning ( default : 4101 ) 

static void PrintHeader_( void )
{
	COut << NAME " V" VERSION << " by "AUTHOR_NAME " (" AUTHOR_CONTACT ")." << txf::nl;
	COut << COPYRIGHT << txf::nl;
	COut << txf::pad << AFFILIATION << txf::nl;
	COut << txf::pad << DEPENDENCE << txf::nl;
	COut << txf::tab << "Build : "__DATE__ " " __TIME__ << txf::nl;
}

static void AnalyzeOptions_(
	clnarg::analyzer___ &Analyzer,
	parameters &Parameters,
	const lcl::rack__ &LocaleRack )
{
ERRProlog
	epeios::row__ P;
	clnarg::option_list Options;
	clnarg::id__ Option;
	const bso::char__ *Unknown = NULL;
	clnarg::argument Argument;
ERRBegin
	Options.Init();

	if ( ( Unknown = Analyzer.GetOptions( Options ) ) != NULL )
		clnarg::ReportUnknownOptionError( Unknown, NAME, LocaleRack );

	P = Options.First();

	while( P != NONE ) {
		Argument.Init();

#pragma warning ( disable : 4065 ) 
		switch( Option = Options( P ) ) {
//		case o:
		default:
			ERRc();
		}

		P = Options.Next( P );
	}
#pragma warning ( default : 4065 ) 

ERRErr
ERREnd
ERREpilog
}

static void AnalyzeFreeArguments_(
	clnarg::analyzer___ &Analyzer,
	parameters &Parameters,
	const lcl::rack__ &LocaleRack )
{
ERRProlog
	clnarg::arguments Free;
	epeios::row__ P;
ERRBegin
	Free.Init();

	Analyzer.GetArguments( Free );

	P = Free.Last();

#pragma warning ( disable : 4065 ) 
	switch( Free.Amount() ) {
	case 0:
		break;
	default:
		clnarg::ReportWrongNumberOfArgumentsError( NAME, LocaleRack );
		break;
	}
#pragma warning ( default : 4065 ) 

ERRErr
ERREnd
ERREpilog
}

static void AnalyzeArgs_(
	int argc,
	const char *argv[],
	parameters &Parameters,
	const lcl::rack__ &LocaleRack )
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
		PrintUsage_( Description, LocaleRack );
		ERRExit( EXIT_SUCCESS );
		break;
	case cLicense:
		epsmsc::PrintLicense();
		ERRExit( EXIT_SUCCESS );
		break;
//	case c:
	case CLNARG_NONE:
		break;
	default:
		ERRc();
	}

	AnalyzeOptions_( Analyzer, Parameters, LocaleRack );

	AnalyzeFreeArguments_( Analyzer, Parameters, LocaleRack );

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

static backend_connection_type__ GetModuleConnectionType_( const str::string_ &Id )
{
	backend_connection_type__ Type = bct_Undefined;
ERRProlog
	str::string Value;
ERRBegin
	Value.Init();

	registry::GetRawModuleConnectionType( Value );

	if ( Value == "Straight" )
		Type = bctStraight;
	if ( Value == "Switched" )
		Type = bctSwitched;
	else {
		Value.Init();
		CErr << GetTranslation( mBadOrNoValueForRegistryEntry, registry::xpath___( registry::paths::modules::module::ConnectionType, Id ).GetTargetedPath( Value ) ) << txf::nl;
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

	registry::GetRawLogFileHandling( Value );

	if ( Value == "Append" )
		Handling = lfhAppend;
	else if ( Value == "Drop" )
		Handling = lfhDrop;
	else {
		CErr << GetTranslation( mBadOrNoValueForRegistryEntry, registry::paths::log::Mode ) << '.' << txf::nl;
		ERRExit( EXIT_FAILURE );
	}
ERRErr
ERREnd
ERREpilog
	return Handling;
}


using csdleo::shared_data__;

static void UseStraightConnections_(
	csdsuf::user_functions__ &UserFunctions,
	const bso::char__ *Backend,
	csdbns::port__ Port,
	shared_data__ &SharedData )
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
	csdbns::port__ Port,
	shared_data__ &SharedData )
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
		epeios::size__ Amount )
	{
		tol::buffer__ Buffer;

		if ( _Flow == NULL )
			ERRc();

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
	csdbns::port__ Port,
	shared_data__ &SharedData )
{
ERRProlog
	flf::file_oflow___ FFlow;
	txf::text_oflow__ TFlow;
	log_functions__ LogFunctions;
	fil::mode__ Mode = fil::m_Undefined;
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
			ERRc();
			break;
		}

		if ( FFlow.Init( LogFileName, Mode, err::hUserDefined ) != fil::sSuccess ) {
			GetTranslation( mUnableToOpenLogFile, LogFileName );
			LogFileName = NULL;	// To notify no to use log functions.
		} else
			TFlow.Init( FFlow );
	}

	UseSwitchingConnections_( UserFunctions, LogFileName == NULL ? *(csdsns::log_functions__ *)NULL : LogFunctions, Backend, Port, SharedData );
ERRErr
ERREnd
ERREpilog
}

struct data__
{
	const bso::char__ *BackendFileName;
	csdbns::port__ Port;
	backend_connection_type__ ConnectionType;
	const char *LogFileName;
	log_file_handling__ LogFileHandling;
	mtx::mutex_handler__ Mutex;
};

static void Go_( void *UP )
{
ERRProlog
	csdlec::library_embedded_client_core__ Core;
	csdsns::server___ Server;
	lcl::locale SharedLocale;
	rgstry::registry SharedRegistry;
	csdleo::shared_data__ SharedData;
	data__ &Data = *(data__ *)UP;
ERRBegin
	SharedLocale.Init();
	SharedRegistry.Init();

	SharedData.Init( csdleo::mRemote );

	SharedData.CErr = &cio::CErrDriver;
	SharedData.COut = &cio::COutDriver;

	if ( !Core.Init( Data.BackendFileName, &SharedData, err::hUserDefined ) ) {
		CErr << GetTranslation( mUnableToLoadBackend, Data.BackendFileName ) << txf::nl;
		ERRExit( EXIT_FAILURE );
	}

	switch ( Data.ConnectionType ) {
	case bctStraight:
		UseStraightConnections_( Core.GetSteering(), Data.BackendFileName, Data.Port, SharedData );
		break;
	case bctSwitched:
		UseSwitchingConnections_( Core.GetSteering(), Data.LogFileName, Data.LogFileHandling, Data.BackendFileName, Data.Port, SharedData );
		break;
	default:
		ERRc();
		break;
	}

	mtx::Unlock( Data.Mutex );

	Server.Process();
ERRErr
ERREnd
ERREpilog
}

static void Go_(
	const bso::char__ *BackendFileName,
	csdbns::port__ Port,
	backend_connection_type__ ConnectionType,
	const char *LogFileName,
	log_file_handling__ LogFileHandling )
{
	data__ Data;

	Data.BackendFileName = BackendFileName;
	Data.Port = Port;
	Data.ConnectionType = ConnectionType;
	Data.LogFileName = LogFileName;
	Data.LogFileHandling = LogFileHandling;

	Data.Mutex = mtx::Create( mtx::mFree );

	mtx::Lock( Data.Mutex );

	mtk::Launch( Go_, &Data );

	mtx::Lock( Data.Mutex );
}


static inline csdbns::port__ GetService_( const str::string_ &Id )
{
	return registry::GetRawService( Id );
}

static void Go_(
	const str::string_ &Id,
	const char *LogFileName,
	log_file_handling__ LogFileHandling )
{
ERRProlog
	str::string ModuleFileName;
	STR_BUFFER___ Buffer;
ERRBegin
	ModuleFileName.Init();

	Go_( registry::GetModuleFileName( Id, ModuleFileName ).Convert( Buffer ), GetService_( Id ), GetModuleConnectionType_( Id ), LogFileName, LogFileHandling );
ERRErr
ERREnd
ERREpilog
}


static void Go_(
	const str::strings_ &Ids,
	const char *LogFileName,
	log_file_handling__ LogFileHandling )
{
	epeios::row__ Row = Ids.First();
	ctn::E_CMITEM( str::string_ ) Id;

	Id.Init( Ids );

	while( Row != NONE ) {
		Go_( Id( Row ), LogFileName, LogFileHandling );

		Row = Ids.Next( Row );
	}

	while ( 1 )
		tht::Suspend( 1000 );
}

static void Go_( const parameters &Parameters )
{
ERRProlog
	STR_BUFFER___ BackendFileNameBuffer, LogFileNameBuffer;
	const char *LogFileName = NULL;
	str::strings Ids;
ERRBegin
	Ids.Init();

	registry::GetModulesIds( Ids );

	LogFileName = registry::GetLogFileName( LogFileNameBuffer );

	Go_( Ids, LogFileName, ( LogFileName == NULL ? lfh_Undefined : GetLogFileHandling_() ) );
ERRErr
ERREnd
ERREpilog
}

const char *scltool::TargetName = NAME;

void scltool::Main(
	int argc,
	const char *argv[],
	const lcl::rack__ &LocaleRack )
{
ERRProlog
	parameters Parameters;
ERRBegin
	AnalyzeArgs_( argc, argv, Parameters, LocaleRack );

	Go_( Parameters );
ERRErr
ERREnd
ERREpilog
}
