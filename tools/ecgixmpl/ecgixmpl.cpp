/*
	'gesbibcgi' module by Claude SIMON (develop@logiplus.fr)
	CGI deamon for the databases manager.
	Copyright (C) 2003  LOGI+, 6 rue de Stockholm, 67000 Strasbourg, France.
*/

// $Id$

#include "cpe.h"
#include "err.h"
#include "stf.h"
#include "epsmsc.h"
#include "clnarg.h"
#include "srv.h"

#define VERSION			"0.1.0"
#define NAME			"ecgixml"
#define COPYRIGHT_YEARS	"2004"
#define DESCRIPTION		"Exapmle of a CGI deamon."
#define INFO			EPSMSC_EPEIOS_TEXT
#define AUTHOR_NAME		EPSMSC_AUTHOR_NAME
#define AUTHOR_EMAIL	EPSMSC_AUTHOR_EMAIL
#define HELP			EPSMSC_HELP_INVITATION( NAME )
#define COPYRIGHT		EPSMSC_COPYRIGHT( COPYRIGHT_YEARS )
#define CVS_DETAILS		("$Id$\b " + 5)

/*$RAW$*/

/* Beginning of the part which handles command line arguments. */

enum command {
	c_none,
	cHelp,
	cVersion,
	cLicense,
	cLaunch,
	c_amount
};

struct parameters___ {
	command Command;
	srv::service__ Service;
	parameters___( void )
	{
		Command = c_none;
		Service = 0;
	}
	~parameters___( void )
	{
	}
} Parameters;

static void PrintUsage( const clnarg::description_ &Description )
{
	stf::cout << DESCRIPTION << txf::nl;
	stf::cout << NAME << " --version|--license|--help" << txf::nl;
	clnarg::PrintCommandUsage( Description, cVersion, "print version of " NAME " components.", clnarg::vSplit, false );
	clnarg::PrintCommandUsage( Description, cLicense, "print the license.", clnarg::vSplit, false );
	clnarg::PrintCommandUsage( Description, cHelp, "print this message.", clnarg::vOneLine, false );
	stf::cout << NAME << " [command] <port> <server>" << txf::nl;
	stf::cout << txf::tab << "port:" << txf::tab << "port to listen to." << txf::nl;
	stf::cout << "Command: " << txf::nl;
	clnarg::PrintCommandUsage( Description, cLaunch, "launch the CGI deamon.", clnarg::vSplit, true );
}

static void PrintHeader( void )
{
	stf::cout << NAME " V" VERSION " "__DATE__ " " __TIME__;
	stf::cout << " by "AUTHOR_NAME " (" AUTHOR_EMAIL ")" << txf::nl;
	stf::cout << COPYRIGHT << txf::nl;
	stf::cout << INFO << txf::nl;
	stf::cout << "CVS file details : " << CVS_DETAILS << txf::nl;
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
		stf::cerr << '\'' << Unknow << "': unknow option." << txf::nl;
		stf::cout << HELP << txf::nl;
		ERRi();
	}

	P = Options.First();

	while( P != NONE ) {
		Argument.Init();
		switch( Option = Options( P ) ) {
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
	tym::row__ P;
ERRBegin
	Free.Init();

	Analyzer.GetArguments( Free );

	P = Free.Last();

	switch ( Parameters.Command ) {
	case cLaunch:
		if ( Free.Amount() == 1 ) {
			Parameters.Service = (srv::service__)atoi( Free( P ).Convert() );
			P = Free.Previous( P );
		} else if ( Free.Amount() < 1 ) {
			stf::cerr << "Too few arguments." << txf::nl;
			stf::cout << "Try '" NAME " --help' to get more informations." << txf::nl;
			ERRi();
		} else {
			stf::cerr << "Too many arguments." << txf::nl;
			stf::cout << "Try '" NAME " --help' to get more informations." << txf::nl;
			ERRi();
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
	Description.AddCommand( CLNARG_NO_SHORT, "version", cVersion );
	Description.AddCommand( CLNARG_NO_SHORT, "help", cHelp );
	Description.AddCommand( CLNARG_NO_SHORT, "license", cLicense );

	Analyzer.Init( argc, argv, Description );

	switch ( Parameters.Command = (command)Analyzer.GetCommand() ) {
	case cVersion:
		PrintHeader();
		TTR.Advertise();
		ERRi();
		break;
	case cHelp:
		PrintUsage( Description );
		ERRi();
		break;
	case cLicense:
		epsmsc::PrintLicense();
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

const char *HostServiceBroker = NULL;


static void Handle_(
	task__ Task,
	indice__ Indice,
	const cgi_args_ &CGIArgs,
	const str::string_ &UserLabel,
	context_ &Context,
	xmlf__ &Content,
	tasks_ &Tasks )
{
	switch ( Task.Action ) {
	case aDefine:
		Define( Task.Target, CGIArgs, Context );
		break;
	case aApply:
		Apply( Task.Target, Indice, CGIArgs, Context );
		break;
	case aBrowse:
		Browse( Task.Target, CGIArgs, Context );
		break;
	case aSummon:
		Summon( Task.Target, Indice, CGIArgs, Context );
		break;
	case aDrop:
		Drop( Task.Target, Indice, CGIArgs, Context );
		break;
	case aSelect:
		Select( Task.Target, CGIArgs, Context );
		break;
	case aSeek:
		Seek( Task.Target, Indice, CGIArgs, Context );
		break;
	default:
		ERRc();
		break;
	}

	if ( UserLabel.Amount() != 0 )
		DumpUserData( UserLabel, Context.Frame, Context.Backend, Content, Tasks );
	else
		DumpTasks( Context.Frame, Context.Backend, Content, Tasks );
}

static lgg::language__ GetLanguage_( const cgi_args_ &CGIArgs )
{
	language__ Language = lgg::lUnknow;
ERRProlog
	char * Buffer = NULL;
	cgiarg::value Value;
ERRBegin
	Value.Init();

	if ( GetArgNamed( "_language", CGIArgs, Value ) ) {
		Buffer = Value.Convert();

		Language = lgg::GetLanguageIDWithLabel( Buffer );
	}

ERRErr
ERREnd
	tol::Free( Buffer );
ERREpilog
	return Language;
}

static bso::bool__ GetContext_(
	const cgi_args_ &CGIArgs,
	cgi_tool___ &Tool,
	str::string_ &Session,
	task__ &Task,
	indice__ &Indice,
	display__ &Display,
	lgg::language__ &Language,
	xmlf__ &Content,
	tasks_ &Tasks,
	xmlf__ &Status,
	context_ *&Context )
{
	bso::bool__ Return = false;

	Context = NULL;

	Language = GetLanguage_( CGIArgs );

	if ( !GetTask( CGIArgs, Task, Indice ) ) {
		Status.PushTag( "Message" );
		Status.PutAttribute( "Value", "Nothing to do ..." );
		Status.PutAttribute( "Type", "Error" );
		Status.PopTag();
	} else 	if ( ( Task.Target == tSession ) && ( Task.Action == aApply ) ) {
		Status.PutAttribute( "Action", GetActionLabel( Task.Action) );
		Status.PutAttribute( "Target", GetTargetLabel( Task.Target ) );
		PutId( "Id", Task.TargetId, Status );
		if ( Language == lgg::lUnknow )
			Language = DEFAULT_LANGUAGE;
		ApplySession( HostServiceBroker, CGIArgs, Session, Display, Language, Tool, Content, Tasks, Context );
		DumpTasks( Context->Frame, Context->Backend, Content, Tasks );
	} else if ( !GetContext( HostServiceBroker, CGIArgs, Tool, Session, Context, Status ) ) {
		Status.PutAttribute( "Action", GetActionLabel( aDrop ) );
		Status.PutAttribute( "Target", GetTargetLabel( tSession ) );
		PutId( "Id", Task.TargetId, Status );
		Display.Init( sForm, tSession );
		// Nothing else to do.
	} else {
		Status.PutAttribute( "Action", GetActionLabel( Task.Action) );
		Status.PutAttribute( "Target", GetTargetLabel( Task.Target ) );
		PutId( "Id", Task.TargetId, Status );

		if ( ( Task.Target == tSession ) && ( Task.Action == aDrop ) ) {
			Display.Init( sForm, tSession );
			DropSession( Context, Content, Tasks );
			Tool.Close( Session );
			// Nothing else to do.
		} else {
			if ( Language != lgg::lUnknow )
				Context->Backend.SetLanguage( Language );
			else
				Language = Context->Backend.GetLanguage();

			Return = true;
		}
	}

	return Return;
}

static target__ GetTargetNamed_(
	const char *Name,
	const cgi_args_ &CGIArgs )
{
	target__ Result = tNone;
ERRProlog
	cgiarg::value Value;
ERRBegin
	Value.Init();

	if ( GetArgNamed( Name, CGIArgs, Value ) )
		Result = GetTarget( Value );
ERRErr
ERREnd
ERREpilog
	return Result;
}

static bso::bool__ Handle_(
	const cgi_args_ &CGIArgs,
	cgi_tool___ &Tool,
	str::string_ &Session,
	display__ &Display,
	lgg::language__ &Language,
	const str::string_ &UserLabel,
	xmlf__ &Content,
	tasks_ &Tasks,
	xmlf__ &Status,
	context_ *&Context )
{
	bso::bool__ Return = true;
ERRProlog
	task__ Task;
	indice__ Indice = NO_INDICE;
ERRBegin
	if ( ( GetContext_( CGIArgs, Tool, Session, Task, Indice, Display, Language, Content, Tasks, Status, Context ) ) ) {
		Handle_( Task, Indice, CGIArgs, UserLabel, *Context, Content, Tasks );
		Display = Context->Frame.GetDisplay();
	}
ERRErr
	if ( ( ERRMajor != err::itn ) || ( ERRMinor != err::iReturn ) ) {
		if ( Context != NULL )
			Context->Backend.SetMessage( err::Message() );
	}

	ERRRst();
	Return = false;
ERREnd
ERREpilog
	return Return;
}

static inline bso::bool__ DumpTask_(
	context_ &Context,
	backend__ &Backend,
	xmlf__ &Content,
	tasks_ &Tasks )
{
	bso::bool__ Return = true;
ERRProlog
ERRBegin
	DumpTasks( Context.Frame, Backend, Content, Tasks );
ERRErr
	if ( ( ERRMajor == err::itn ) && ( ERRMinor == err::iReturn ) ) {
		ERRRst();
		Return = false;
	}
ERREnd
ERREpilog
	return Return;
}

static inline void DumpCGIContent_(
	const cgi_params_ &Params,
	const cgi_args_ &CGIArgs,
	const cgi_data_ &CGIData,
	xmlf__ &Out )
{
	Out.PushTag( "CGI" );
	cgitol::Dump( Params, Out );
	Out.PushTag( "Arguments" );
	cgitol::Dump( CGIArgs, Out );
	Out.PopTag();
	DumpCGIData( CGIData, Out );
	Out.PopTag();
}

static void Handle_(
	const cgi_args_ &CGIArgs,
	const str::string_ &UserLabel,
	cgi_tool___ &Tool,
	str::string_ &Session,
	display__ &Display,
	lgg::language__ &Language,
	cgi_data_ &CGIData,
	xmlf__ &Out )
{
ERRProlog
	context_ *Context = NULL;
	xmld Status;
	xmld Content;
	xmld T;
	xmlf__ StatusFiller;
	xmlf__ ContentFiller;
	xmlf__ TasksFiller;
	tasks Tasks;
	bso::bool__ Error = false;
ERRBegin
	Status.Init();
	StatusFiller.Init( Status );
	StatusFiller.PushTag( "Status" );

	Content.Init();
	ContentFiller.Init( Content );
	ContentFiller.PushTag( "Content" );

	T.Init();
	TasksFiller.Init( T );
	TasksFiller.PushTag( "Tasks" );

	Tasks.Init();

	if ( !Handle_( CGIArgs, Tool, Session, Display, Language, UserLabel, ContentFiller, Tasks, StatusFiller, Context ) ) {
		Error = true;

		if ( Context ) {

			ContentFiller.reset();
			Content.Init();
			ContentFiller.Init( Content );
			ContentFiller.PushTag( "Content" );

			Tasks.Init();

			if ( !DumpTask_( *Context, Context->Backend, ContentFiller, Tasks ) )
				ERRl();

			Display = Context->Frame.GetDisplay();
		}
	}

	if ( Context ) {
		CGIData = Context->Frame.CGIData;

		if ( Context->Backend.GetMessage() != NULL ) {
			if ( Error )
				StatusFiller.PutAttribute( "State", "Failure" );
			else
				StatusFiller.PutAttribute( "State", "Success" );

			StatusFiller.PushTag( "Message" );

			if ( Error )
				StatusFiller.PutAttribute( "Type", xmldbs::value( "Error" ) );
			else
				StatusFiller.PutAttribute( "Type", xmldbs::value( "Notice" ) );

			StatusFiller.PutAttribute( "Value", xmldbs::value( Context->Backend.GetMessage() ) );
			Context->Backend.SetMessage( NULL );

			StatusFiller.PopTag();
		}
	}

	DumpTasks( Tasks, TasksFiller );

	StatusFiller.PopTag();
	ContentFiller.PutAttribute( "Scope", GetScopeLabel( Display.Scope ) );
	ContentFiller.PutAttribute( "Target", GetTargetLabel( Display.Target ) );
	ContentFiller.PopTag();
	TasksFiller.PopTag();

	Out.Put( Status );
	Out.Put( Content );
	Out.Put( T );

#if 0
	if ( Context )
		if ( Parameters.Historic )
			DumpHistoric( Context->Historic, Out );
#endif

ERRErr
ERREnd
ERREpilog
}

class user_functions__
: public cgimng::user_functions__
{
	virtual void CGIMNGHandle(
		const cgi_args_ &CGIArgs,
		cgi_tool___ &Tool,
		flw::oflow___ &Client )
	{
	ERRProlog
		cgi_params CGIParams;
		xmld Out;
		xmlf__ OutFiller;
		txf::text_oflow___ TFlow;
		display__ Display;
		lgg::language__ Language = lgg::lUnknow;
		cgidat::data CGIData;
		session__ Session;
		str::string UserLabel;
	ERRBegin
		CGIParams.Init();

		switch( cgitol::Fill( CGIParams, CGIArgs ) ) {
		case cgitol::sNoTarget:
			ERRc();
			break;
		case cgitol::sNoSession:
		case cgitol::sOK:
			break;
		default:
			ERRc();
			break;
		}

		UserLabel.Init();

		GetArgNamed( "_user", CGIArgs, UserLabel );

		Out.Init();
		OutFiller.Init( Out );

		OutFiller.PushTag( "gesbib" );

		Display.Init();

		CGIData.Init();

		Session.Init();

		Handle_( CGIArgs, UserLabel, Tool, CGIParams.Session, Display, Language, CGIData, OutFiller );

		DumpCGIContent_( CGIParams, CGIArgs, CGIData, OutFiller );

		OutFiller.PopTag();

		TFlow.Init( Client );

		OutFiller.reset();

		if ( UserLabel.Amount() )
			TFlow << UserLabel;
		else
			TFlow << GetTargetLabel( Display.Target ) << GetScopeLabel( Display.Scope );
		
		TFlow << ' ' << lgg::GetLanguageLabel( Language );
	
		xmldbs::WriteXMLHeader( TFlow );
		xmldbs::WriteXML( Out, TFlow );

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
};



static void Go( const parameters___ &Parameters )
{
ERRProlog
	cgimng::cgi_manager Manager;
//	cgifrt::debug_manager___ Manager;
	user_functions__ UserFunctions;
ERRBegin
	tol::InitializeRandomGenerator( 1 );

	HostServiceBroker = Parameters.Server;
	Manager.Init( Parameters.Service, UserFunctions );
	Manager.Process();
//	Debug( Manager.Process() );
ERRErr
ERREnd
ERREpilog
}

static void LoadMessages( void )
{
ERRProlog
	fil::file_iflow___ FFlow;
	xtf::extended_text_iflow___ TFlow;
	xtf::location__ Location = 0;
ERRBegin
	enNotificationMessages.Init();
	frNotificationMessages.Init();
	deNotificationMessages.Init();

	if ( tol::FileExists( NOTIFICATION_MESSAGES_FILE_NAME ) ) {
		FFlow.Init( NOTIFICATION_MESSAGES_FILE_NAME );
		FFlow.EOFD( XTF_EOXT );
		TFlow.Init( FFlow );

		if ( ( Location = LoadNotificationMessages( TFlow ) ) != 0 )
			stf::cerr << "Error in notification translation file '" << NOTIFICATION_MESSAGES_FILE_NAME << "' at line " << Location << "." << txf::nl;
	} else
		stf::cerr << "Warning : notification translation file '" << NOTIFICATION_MESSAGES_FILE_NAME << "' not found !" << txf::nl;

	enErrorMessages.Init();
	frErrorMessages.Init();
	deErrorMessages.Init();

	if ( tol::FileExists( ERROR_MESSAGES_FILE_NAME ) ) {
		FFlow.Init( ERROR_MESSAGES_FILE_NAME );
		FFlow.EOFD( XTF_EOXT );
		TFlow.Init( FFlow );

		if ( ( Location = LoadErrorMessages( TFlow ) ) != 0 )
			stf::cerr << "Error in error translation file '" << ERROR_MESSAGES_FILE_NAME << "' at line " << Location << "." << txf::nl;
	} else
		stf::cerr << "Warning : error translation file '" << ERROR_MESSAGES_FILE_NAME << "' not found !" << txf::nl;

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
ERRBegin
	LoadMessages();

	AnalyzeArgs( argc, argv, Parameters );

	switch ( Parameters.Command ) {
	case cNotificationMessages:
		DumpBaseNotificationMessages( stf::cout );
		break;
	case cErrorMessages:
		DumpBaseErrorMessages( stf::cout );
		break;
	case cLaunch:
		Go( Parameters );
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
	int ExitValue = EXIT_SUCCESS;
ERRFProlog
ERRFBegin
	Main( argc, argv );
ERRFErr
	ExitValue = EXIT_FAILURE;

	if ( ERRMajor == err::itn )
		ERRRst();

ERRFEnd
ERRFEpilog
	stf::cout << txf::sync;

	return ExitValue;
}
