/*
	'etmc' by Claude SIMON (csimon@zeusw.org)
	Epeiso timecode calculator.
	Copyright (C) 2007 Claude SIMON

	This file is part of the Epeios (http://zeusw.org/epeios/) project.

	This software is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 3
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

// $Id$

#include "err.h"
#include "cio.h"
#include "epsmsc.h"
#include "clnarg.h"
#include "mthtmc.h"

#define NAME			"etmccalc"
#define VERSION			"0.7.2"
#define COPYRIGHT_YEARS	"2007-2009"
#define DESCRIPTION		"Epeios timecode calculator"
#define INFO			EPSMSC_EPEIOS_AFFILIATION
#define AUTHOR_NAME		EPSMSC_AUTHOR_NAME
#define AUTHOR_CONTACT	EPSMSC_AUTHOR_CONTACT
#define HELP			EPSMSC_HELP_INVITATION( NAME )
#define COPYRIGHT		EPSMSC_COPYRIGHT( COPYRIGHT_YEARS )
#define CVS_DETAILS		("$Id$\b " + 5)

#define FORMAT_ENV			"ETMCCALC_FORMAT"
#define PUNCTUATIONS_ENV	"ETMCCALC_PUNCTUATIONS"

#define FCP_MARKER	'P'
#define FCP_FORMAT	"%2H:%2m:%2s%p%2f"

#define AE_MARKER	'E'
#define AE_FORMAT	"%2H%p%2m%p%2s%p%2f"

#define DEFAULT_FORMAT			"%P"
#define DEFAULT_PUNCTUATIONS	";:"

using cio::cin;
using cio::cout;
using cio::cerr;

/* Beginning of the part which handles command line arguments. */

enum command {
	cHelp,
	cVersion,
	cLicense,
	cHelpFormat,		// Help page about formating.
	cHelpPunctuations,	// Help page about punctuations.
};

enum option {
	oFormat,	// Format specification.
	oPunctuations,	// Punctuation.

};

enum exit_value__ {
	evParameters = 2,
};


typedef ctn::E_XMCONTAINER_( str::string_ ) strings_;
E_AUTO( strings )

#define STRING_PARAM___( name )	CLNARG_STRING_PARAM___( name )

struct parameters {
	strings Strings;
	STRING_PARAM___( Format );
	STRING_PARAM___( Punctuations );
	parameters( void )
	{
		Strings.Init();
	}
};

void PrintUsage( const clnarg::description_ &Description )
{
	cout << DESCRIPTION << txf::nl;
	cout << NAME << " --version|--license|--help|" << Description.GetCommandLabels( cHelpFormat) << '|';
	cout << Description.GetCommandLabels( cHelpPunctuations) << txf::nl;
	clnarg::PrintCommandUsage( Description, cVersion, "print version of " NAME " components.", clnarg::vSplit, false );
	clnarg::PrintCommandUsage( Description, cLicense, "print the license.", clnarg::vSplit, false );
	clnarg::PrintCommandUsage( Description, cHelp, "print this message.", clnarg::vSplit, false );
	clnarg::PrintCommandUsage( Description, cHelpFormat, "help page about format (see below).", clnarg::vSplit, false );
	clnarg::PrintCommandUsage( Description, cHelpPunctuations, "help page about punctuations (see below).", clnarg::vSplit, false );
	cout << NAME << " [<options>] <fps> [<timecode> [<timecode> [...]]]" << txf::nl;
	cout << txf::pad << "options :" << txf::nl;
	cout << txf::tab << '(' << Description.GetOptionLabels( oFormat ) << ") <format> :" << txf::nl;
	cout << txf::tab << txf::pad << "formating specification. Default is the content of enviroment" << txf::nl;
	cout << txf::tab << txf::pad << "variable '" << FORMAT_ENV << "' or, if empty or not defined," << txf::nl;
	cout << txf::tab << txf::pad << "'" << DEFAULT_FORMAT << "'. '" << NAME << ' ' << Description.GetCommandLabels( cHelpFormat) << "' for more details." << txf::nl;
	cout << txf::tab << '(' << Description.GetOptionLabels( oPunctuations ) << ") <punctuations> :" << txf::nl;
	cout << txf::tab << txf::pad << "punctuations used for '%p' (see above). Default is the content of" << txf::nl;
	cout << txf::tab << txf::pad << "enviroment variable '" << PUNCTUATIONS_ENV << "' or, if empty or not" << txf::nl;
	cout << txf::tab << txf::pad << "defined, '" << DEFAULT_PUNCTUATIONS << "'. '" << NAME << ' ' << Description.GetCommandLabels( cHelpPunctuations) << "' for more details." << txf::nl;
	cout << txf::pad << "<fps> : frames-per-second for output (see below for more details)." << txf::nl;
	cout << txf::pad << "<timecode> :" << txf::nl;
	cout << txf::tab << "timecode ('d:h:m:s:f@fps')." << txf::nl;
	cout << txf::pad << "Values for fps :" << txf::nl;
	cout << txf::tab << "an integer, '23n', '29(n|d)', '30d', '59(n|d)' or '60d'." << txf::nl << txf::nl;

	// Free argument description.
//	cout << "command:" << txf::nl;
//	clnarg::PrintCommandUsage( Description, c, "", false, true );
//	cout << "options:" << txf::nl;
//	clnarg::PrintOptionUsage( Description, o, "", clnarg::vSplit );
}

void PrintPunctuationsHelpPage( void )
{
	cout << "The punctuations option is used for the '%p' format marker. For drop frame," << txf::nl;
	cout << "the first character is used. For not real-time, the second character is used," << txf::nl;
	cout << "or the one for drop frame if inexistant. For non drop frame, the third character" << "is used, or the one for non real-time if inexistant." << txf::nl;
}

void PrintFormatHelpPage( void )
{
	cout << "The output format is a string which can contains following markers :" << txf::nl;
	cout << txf::pad << "- %% : literal '%'," << txf::nl;
	cout << txf::pad << "- %<[<p>]<t> : round down," << txf::nl;
	cout << txf::pad << "- %=[<p>]<t> : mathematical rounding (default rounding)," << txf::nl;
	cout << txf::pad << "- %>[<p>]<t> : round up," << txf::nl;
	cout << txf::pad << "- %[<f>]D : amount of days," << txf::nl;
	cout << txf::pad << "- %[<f>]d : same as '%[<f>]D'," << txf::nl;
	cout << txf::pad << "- %[<f>]H : total amount of hours," << txf::nl;
	cout << txf::pad << "- %[<f>]h : hours in the day," << txf::nl;
	cout << txf::pad << "- %[<f>]M : total amount of minutes," << txf::nl;
	cout << txf::pad << "- %[<f>]m : minutes in the hour," << txf::nl;
	cout << txf::pad << "- %[<f>]S : total amount of seconds," << txf::nl;
	cout << txf::pad << "- %[<f>]s : seconds in the minute," << txf::nl;
	cout << txf::pad << "- %[<f>]F : total amount of frames," << txf::nl;
	cout << txf::pad << "- %[<f>]f : amount of frames in the second," << txf::nl;
	cout << txf::pad << "- %" << FCP_MARKER << " : 'Final Cut Pro' format ('" << FCP_FORMAT << "')." << txf::nl;
	cout << txf::pad << "- %" << AE_MARKER << " : 'After Effects' format ('" << AE_FORMAT << "')." << txf::nl;
	cout << txf::pad << "- %p : punctuation depending of the output fps designation," << txf::nl;
	cout << txf::pad << "- %r : remainder as fraction (for testing purpose)," << txf::nl;
	cout << txf::pad << "- %R : remainder as float (for testing purpose)." << txf::nl;
	cout << txf::nl;
	cout << "- <p> : rounding precision ('0' to '9', '0' if missing)," << txf::nl;
	cout << "- <t> : rounding target ('d' for days, 'h' for hours, and so on ...)," << txf::nl;
	cout << "- <f> : [<w>][(.|,)[<p>]] ('1' by default). Optional set of following flags :" << txf::nl;
	cout << txf::pad << "- <w> : a digit as the minimum amount of digits for the integer part," << txf::nl;
	cout << txf::pad << "- '.' or ',' : the decimal separator to use," << txf::nl;
	cout << txf::pad << "- <p> : a digit as the maximum amount of digits for the fractional part." << txf::nl;
}

void PrintHeader( void )
{
	cout << NAME " V" VERSION " "__DATE__ " " __TIME__;
	cout << " by "AUTHOR_NAME " (" AUTHOR_CONTACT ")" << txf::nl;
	cout << COPYRIGHT << txf::nl;
	cout << INFO << txf::nl;
	cout << "CVS file details : " << CVS_DETAILS << txf::nl;
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
		cerr << '\'' << Unknow << "': unknow option." << txf::nl;
		cout << HELP << txf::nl;
		ERRi();
	}

	P = Options.First();

	while( P != NONE ) {
		Argument.Init();

		switch( Option = Options( P ) ) {
		case oFormat:
			Analyzer.GetArgument( Option, Argument );
			if ( Argument.Amount() == 0 ) {
				cerr << "'" << Analyzer.Description().GetOptionLabels( oFormat ) << "' option must have an argument!" << txf::nl;
				ERRExit( evParameters );
			}
			Argument.Convert( Parameters.Format );
			break;
		case oPunctuations:
			Analyzer.GetArgument( Option, Argument );
			if ( Argument.Amount() == 0 ) {
				cerr << "'" << Analyzer.Description().GetOptionLabels( oPunctuations ) << "' option must have an argument!" << txf::nl;
				ERRExit( evParameters );
			}
			Argument.Convert( Parameters.Punctuations );
			break;
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
	case 0:
		cerr << "Wrong amount of arguments arguments." << txf::nl;
		cout << HELP << txf::nl;
		ERRi();
		break;
	default:
		P = Free.First();

		while ( P != NONE ) {
			Parameters.Strings.Append( Free( P ) );

			P = Free.Next( P );
		}
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
	Description.AddCommand( CLNARG_NO_SHORT, "help-format", cHelpFormat );
	Description.AddCommand( CLNARG_NO_SHORT, "help-punctuations", cHelpPunctuations );
//	Description.AddOption( '', "", o );
	Description.AddOption( 'f', "format", oFormat );
	Description.AddOption( 'p', "punctuations", oPunctuations );

	Analyzer.Init( argc, argv, Description );
 
	switch ( Analyzer.GetCommand() ) {
	case cVersion:
		PrintHeader();
		TTR.Advertise( cio::cout );
		ERRi();
		break;
	case cHelp:
		PrintUsage( Description );
		ERRi();
		break;
	case cHelpPunctuations:
		PrintPunctuationsHelpPage();
		ERRi();
		break;
	case cHelpFormat:
		PrintFormatHelpPage();
		ERRi();
		break;
	case cLicense:
		epsmsc::PrintLicense( cout );
		ERRi();
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

static void Init(
	const char *Name,
	const char *Default,
	str::string_ &String )
{
	String.Init();

	if ( getenv( Name ) != NULL )
		String.Append( getenv( Name ) );

	if ( String.Amount() == 0 )
		String.Append( Default );
}

void Preprocess( str::string_ &Format )
{
ERRProlog
	str::string Buffer;
	epeios::row__ Row = NONE;
	bso::char__ C;
ERRBegin
	Buffer.Init( Format );

	Row = Buffer.First();

	Format.Init();

	while ( Row != NONE ) {
		if ( ( C = Buffer( Row ) ) == '%' ) {
			Row = Buffer.Next( Row );

			if ( Row == NONE )
				ERRu();

			switch( C = Buffer( Row ) )  {
				case FCP_MARKER:
					Format.Append( FCP_FORMAT );
					break;
				case AE_MARKER:
					Format.Append( AE_FORMAT );
					break;
				default:
					Format.Append( '%' );
					Format.Append( C );
					break;
			}
		} else
			Format.Append( C );

		Row = Buffer.Next( Row );
	}
ERRErr
ERREnd
ERREpilog
}

void Go(
	const strings_ &Strings,
	const str::string_ &FormatParameter )
{
ERRProlog
	ctn::E_CMITEM( str::string_ ) Item;
	epeios::row__ Row = Strings.First();
	mthtmc::xfps OutputXFPS, XFPS;
	mthtmc::timecode Timecode, TotalTimecode;
	epeios::row__ Position = NONE;
	str::string Format, Punctuations;
	mthtmc::period Period;
ERRBegin
	Item.Init( Strings );

	if ( Row == NONE )
		ERRc();

	Position = Item( Row ).First();

	if ( !OutputXFPS.Init( Item( Row ), Position ) ) {
		cerr << '\'' << Item( Row ) << "' is malformed !" << txf::nl;
		ERRExit( EXIT_FAILURE );
	}

	if ( Position != NONE ) {
		cerr << '\'' << Item( Row ) << "' is malformed !" << txf::nl;
		ERRExit( EXIT_FAILURE );
	}

	XFPS.Init( OutputXFPS );

	Row = Strings.Next( Row );

	Period.Init();
	mthtmc::Convert( XFPS, Period );

	TotalTimecode.Init();

	mthtmc::Convert( mthitg::integer( 0 ), Period, TotalTimecode );

	while ( Row != NONE ) {
		Position = Item( Row ).First();

		Timecode.Init();

		if ( ( !mthtmc::Convert( Item( Row ), Position, XFPS, Timecode ) ) || ( Position != NONE ) ) {
			cerr << '\'' << Item( Row ) << "' is malformed !" << txf::nl;
			ERRExit( EXIT_FAILURE );
		}

		if ( Position != NONE ) {
			cerr << '\'' << Item( Row ) << "' is malformed !" << txf::nl;
			ERRExit( EXIT_FAILURE );
		}

		mthtmc::Add( TotalTimecode, Timecode, TotalTimecode );

//		cio::cout << Timecode << txf::tab << TotalTimecode << txf::nl;
//		XFPS = Timecode.XFPS;

		Row = Strings.Next( Row );
	}

//	cio::cout << TotalTimecode << txf::nl;

	Format.Init();

	if ( FormatParameter.Amount() != 0 )
		Format.Init( FormatParameter );
	else
		Init( FORMAT_ENV, DEFAULT_FORMAT, Format );

	Init( PUNCTUATIONS_ENV, DEFAULT_PUNCTUATIONS, Punctuations );

	switch ( Punctuations.Amount() ) {
	case 0:
		ERRc();
		break;
	case 1 :
		Punctuations.Append( Punctuations( Punctuations.First() ) );
	//	break;
	case 2:
		Punctuations.Append( Punctuations( Punctuations.First( 1 ) ) );
		break;
	default:
		break;
	}

	Preprocess( Format );

	if ( !mthtmc::Display( TotalTimecode, OutputXFPS, Format, Punctuations( Punctuations.First( 2 ) ), Punctuations( Punctuations.First( 0 ) ), Punctuations( Punctuations.First( 1 ) ), tol::GetLocaleDecimalSeparator(), cout ) )
	{
		cerr << '\'' << Format << '\'' << " is not a valid format specification !" << txf::nl;
		ERRExit( EXIT_FAILURE );
	}

	cout << txf::nl << txf::sync;
ERRErr
ERREnd
ERREpilog
}

void Go( const parameters &Parameters )
{
ERRProlog
ERRBegin
	Go( Parameters.Strings, str::string( Parameters.Format ) );
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
ERRFProlog
ERRFBegin
	Main( argc, argv );
ERRFErr
ERRFEnd
ERRFEpilog
	return ERRExitValue;
}
