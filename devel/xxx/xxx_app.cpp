/*
	$NAME$' by Claude L. Simon (simon@epeios.org)
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
#define COPYRIGHT		"copyright (c) 2001 $COPYRIGHT$."
#define AUTHOR_NAME		EPSMSC_AUTHOR_NAME
#define AUTHOR_EMAIL	EPSMSC_AUTHOR_EMAIL
#define HELP			EPSMSC_HELP_INVITATION( NAME )

/*$RAW$*/

/* Beginning of the part which handles command line arguments. */

enum command {
	cHelp,
	cVersion,
};

enum option {
};

struct parameters {
	parameters( txf::text_oflow___ &OFlow )
	{
	}
};

void PrintUsage( const clnarg::description_ &Description )
{
	fout << "Usage: " << NAME << " [command] [options] ..." << txf::nl;
	fout << "(description)" << txf::nl;
	fout << "Command:" << txf::nl;
	clnarg::PrintCommandUsage( Description, c, "", false, true );
	clnarg::PrintCommandUsage( Description, cVersion, "print version of " NAME " components.", false, false );
	clnarg::PrintCommandUsage( Description, cHelp, "print this message.", true, false );
	fout << "Options:" << txf::nl;
	clnarg::PrintOptionUsage( Description, o, "", false );
}

void PrintHeader( void )
{
	fout << NAME " V" VERSION " "__DATE__ " " __TIME__ " " COPYRIGHT << txf::nl;
//	fout << EPSMSC_IDEALX_TEXT <<txf::nl;
	fout << EPSMSC_EPEIOS_TEXT <<txf::nl;
}

static void AnalyzeOptions(
	clnarg::analyzer___ &Analyzer,
	parameters &Parameters )
{
ERRProlog
	tym::row__ P;
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
		case o:
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
	tym::row__ P;
ERRBegin
	Free.Init();

	Analyzer.GetArguments( Free );

	P = Free.Last();

	switch( Free.Amount() ) {
	default:
		ferr << "Too much arguments." << txf::nl;
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

	Description.AddCommand( '', "", c );
	Description.AddCommand( CLNARG_NO_SHORT, "version", cVersion );
	Description.AddCommand( CLNARG_NO_SHORT, "help", cHelp );
	Description.AddOption( '', "", o );

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
	case c:
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
