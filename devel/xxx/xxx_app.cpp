/*
	'$NAME$' by Claude L. SIMON ($EMAIL$)
	Copyright (C) 2001 $COPYRIGHT$

	$ADDENDUM1$
	$ADDENDUM$

	This program is free software; you can redistribute it and/or
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

// $$Id$$

#include "err.h"
#include "stf.h"
#include "epsmsc.h"
#include "clnarg.h"

#define NAME			"$NAME$"
#define VERSION			"0.1.0"
#define COPYRIGHT_YEARS	"2001"
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
};

struct parameters {
	parameters( void )
	{
	}
};

void PrintUsage( const clnarg::description_ &Description )
{
	fout << "(description)" << txf::nl;
	fout << "Usage:";
	fout << txf::tab << NAME << " --version|--license|--help" << txf::nl;
	fout << txf::tab << NAME << " [command] [options] ..." << txf::nl;
	// Free argument description.
	fout << "Command:" << txf::nl;
	// Free argument description.
//	clnarg::PrintCommandUsage( Description, c, "", false, true );
	clnarg::PrintCommandUsage( Description, cVersion, "print version of " NAME " components.", clnarg::vSplit, false );
	clnarg::PrintCommandUsage( Description, cLicense, "print text about the license.", clnarg::vSplit, false );
	clnarg::PrintCommandUsage( Description, cHelp, "print this message.", clnarg::vOneLine, false );
	fout << "Options:" << txf::nl;
//	clnarg::PrintOptionUsage( Description, o, "", false );
}

void PrintHeader( void )
{
	fout << NAME " V" VERSION " "__DATE__ " " __TIME__;
	fout << " by "AUTHOR_NAME " (" AUTHOR_EMAIL ")" << txf::nl;
	fout << COPYRIGHT << txf::nl;
	fout << "CVS file details : " << CVS_DETAILS << txf::nl;
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
		ferr << '\'' << Unknow << "': unknow option." << txf::nl;
		fout << HELP << txf::nl;
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
		ferr << "Too many arguments." << txf::nl;
		fout << HELP << txf::nl;
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
	fout << txf::sync;

	return ExitValue;
}
