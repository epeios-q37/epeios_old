/*
	'$NAME$' by $AUTHOR_NAME$ ($AUTHOR_EMAIL$)
	(description)
	Copyright (C) $COPYRIGHT_DATES$ $COPYRIGHT_OWNER$.

$NOTICE$
*/

// $$Id$$

#include "err.h"
#include "stf.h"
#include "epsmsc.h"
#include "clnarg.h"

#define NAME			"$NAME$"
#define VERSION			"0.1.0"
#define COPYRIGHT_YEARS	"2002"
#define DESCRIPTION		"(description)"
#define INFO			EPSMSC_EPEIOS_TEXT
#define AUTHOR_NAME		EPSMSC_AUTHOR_NAME
#define AUTHOR_EMAIL	EPSMSC_AUTHOR_EMAIL
#define HELP			EPSMSC_HELP_INVITATION( NAME )
#define COPYRIGHT		"Copyright (c) " COPYRIGHT_YEARS " " AUTHOR_NAME " (" AUTHOR_EMAIL ")."
#define CVS_DETAILS		("$Id$\b " + 5)

/*$RAW$*/

/* Beginning of the part which handles command line arguments. */

enum command {
	cHelp,
	cVersion,
	cLicense
};

enum option {
	// o
};

struct parameters {
	parameters( void )
	{
	}
};

void PrintUsage( const clnarg::description_ &Description )
{
	stf::cout << DESCRIPTION << txf::nl;
	stf::cout << NAME << " --version|--license|--help" << txf::nl;
	clnarg::PrintCommandUsage( Description, cVersion, "print version of " NAME " components.", clnarg::vSplit, false );
	clnarg::PrintCommandUsage( Description, cLicense, "print the license.", clnarg::vSplit, false );
	clnarg::PrintCommandUsage( Description, cHelp, "print this message.", clnarg::vOneLine, false );
	stf::cout << NAME << " <command> [options] ..." << txf::nl;
	// Free argument description.
	stf::cout << "command:" << txf::nl;
//	clnarg::PrintCommandUsage( Description, c, "", false, true );
	stf::cout << "options:" << txf::nl;
//	clnarg::PrintOptionUsage( Description, o, "", clnarg::vSplit );
}

void PrintHeader( void )
{
	stf::cout << NAME " V" VERSION " "__DATE__ " " __TIME__;
	stf::cout << " by "AUTHOR_NAME " (" AUTHOR_EMAIL ")" << txf::nl;
	stf::cout << COPYRIGHT << txf::nl;
	stf::cout << INFO << txf::nl;
	stf::cout << "CVS file details : " << CVS_DETAILS << txf::nl;
}


static void AnalyzeOptions(
	clnarg::analyzer___ &Analyzer,
	parameters &Parameters )
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
		ERRt();
	}

	P = Options.First();

	while( P != NONE ) {
		Argument.Init();

		switch( Option = Options( P ) ) {
//		case o:
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
	parameters &Parameters )
{
ERRProlog
	clnarg::arguments Free;
	epeios::row__ P;
ERRBegin
	Free.Init();

	Analyzer.GetArguments( Free );

	P = Free.Last();

	switch( Free.Amount() ) {
	default:
		stf::cerr << "Too many arguments." << txf::nl;
		stf::cout << HELP << txf::nl;
		ERRt();
		break;
	}

ERRErr
ERREnd
ERREpilog
}

static void AnalyzeArgs(
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
		PrintHeader();
		TTR.Advertise();
		ERRt();
		break;
	case cHelp:
		PrintUsage( Description );
		ERRt();
		break;
	case cLicense:
		epsmsc::PrintLicense();
		ERRt();
		break;
//	case c:
	case CLNARG_NONE:
		break;
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




void Go( const parameters &Parameters )
{
ERRProlog
ERRBegin
ERRErr
ERREnd
ERREpilog
}

static inline void Main(
	int argc,
	const char *argv[] )
{
ERRProlog
	parameters Parameters;
ERRBegin
	AnalyzeArgs( argc, argv, Parameters );

	Go( Parameters );
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

	if ( ERRMajor == err::thw )
		ERRRst();

ERRFEnd
ERRFEpilog
	stf::cout << txf::sync;

	return ExitValue;
}
