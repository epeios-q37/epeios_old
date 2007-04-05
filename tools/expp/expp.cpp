/*
	'expp' by Claude SIMON (claude.simon@zeusw.org)
	Preprocesseur XML
	Copyright (C) 2007 Claude SIMON

	This file is part of the Epeios project (http://zeusw.org/epeios/).

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.
 
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, go to http://www.fsf.org or write to the
  
                        Free Software Foundation, Inc.,
           59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

// $$Id$$

#include "err.h"
#include "cio.h"
#include "epsmsc.h"
#include "clnarg.h"
#include "xml.h"
#include "fnm.h"
#include "flf.h"

#define NAME			"expp"
#define VERSION			"0.1.0"
#define COPYRIGHT_YEARS	"2007"
#define DESCRIPTION		"Epeios XML preprocessor"
#define INFO			EPSMSC_EPEIOS_TEXT
#define AUTHOR_NAME		EPSMSC_AUTHOR_NAME
#define AUTHOR_EMAIL	EPSMSC_AUTHOR_EMAIL
#define HELP			EPSMSC_HELP_INVITATION( NAME )
#define COPYRIGHT		"Copyright (c) " COPYRIGHT_YEARS " " AUTHOR_NAME " (" AUTHOR_EMAIL ")."
#define CVS_DETAILS		("$Id$\b " + 5)

#define DEFAULT_NAMESPACE	"xpp"

using cio::cin;
using cio::cout;
using cio::cerr;

/* Beginning of the part which handles command line arguments. */

enum exit_value__ {
	evSuccess = EXIT_SUCCESS,
	evGenericFailure = EXIT_FAILURE,
	// Erreur dans les paramètres d'entrée.
	evParameters,
	// Erreur lors de l'ouverture des fichiers d'entrée ou de sortie.
	evInputOutput,
	// Erreur lors du traitement.
	evProcessing,
	ev_amount
};

enum command {
	cHelp,
	cVersion,
	cLicense,
	cProcess
};

enum option {
	// Définition du namespace.
	oNamespace,
	// Suppression de l'indentation
	oNoIndent

};

struct parameters {
	tol::E_FPOINTER___( char ) Namespace;
	tol::E_FPOINTER___( char ) Source;
	tol::E_FPOINTER___( char ) Destination;
	bso::bool__ NoIndent;
	parameters( void )
	{
		NoIndent = false;
	}
};

void PrintUsage( const clnarg::description_ &Description )
{
	cout << DESCRIPTION << txf::nl;
	cout << NAME << " --version|--license|--help" << txf::nl;
	clnarg::PrintCommandUsage( Description, cVersion, "print version of " NAME " components.", clnarg::vSplit, false );
	clnarg::PrintCommandUsage( Description, cLicense, "print the license.", clnarg::vSplit, false );
	clnarg::PrintCommandUsage( Description, cHelp, "print this message.", clnarg::vOneLine, false );
	cout << NAME << " <command> [options] [source-file [dest-file]]" << txf::nl;
	cout << txf::tab << "source-file:" << txf::tab << "source file; stdin if none." << txf::nl;
	cout << txf::tab << "dest-file:" << txf::tab << "destination file; stdout if none." << txf::nl;
	cout << "command: " << txf::nl;
	clnarg::PrintCommandUsage( Description, cProcess, "Process XML file.", clnarg::vSplit, true );
//	clnarg::PrintCommandUsage( Description, c, "", false, true );
	cout << "options:" << txf::nl;
	clnarg::PrintOptionUsage( Description, oNamespace, "xpp tags namespace; '" DEFAULT_NAMESPACE "' by default.", clnarg::vSplit );
	clnarg::PrintOptionUsage( Description, oNoIndent, "suppress indentation.", clnarg::vSplit );
//	clnarg::PrintOptionUsage( Description, o, "", clnarg::vSplit );
}

void PrintHeader( void )
{
	cout << NAME " V" VERSION " "__DATE__ " " __TIME__;
	cout << " by "AUTHOR_NAME " (" AUTHOR_EMAIL ")" << txf::nl;
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
		case oNamespace:
			Analyzer.GetArgument( Option, Argument );

			if ( Argument.Amount() == 0 ) {
				cout << "Option " << Analyzer.Description().GetOptionLabels( oNamespace ) << " must have one argument !" << txf::nl;
				ERRExit( evParameters );
			}

			Parameters.Namespace = Argument.Convert();

			break;
		case oNoIndent:
			Parameters.NoIndent = true;
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
	case 2:
		Parameters.Destination = Free( P ).Convert();
		P = Free.Previous( P );
	case 1:
		Parameters.Source = Free( P ).Convert();
		break;
	case 0:
		break;
	default:
		cerr << "Wrong amount of arguments." << txf::nl;
		cout << HELP << txf::nl;
		ERRi();
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
	Description.AddCommand( 'p', "process", cProcess );
//	Description.AddOption( '', "", o );
	Description.AddOption( 'n', "namespace", oNamespace );
	Description.AddOption( CLNARG_NO_SHORT, "no-indent", oNoIndent );

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
	case cLicense:
		epsmsc::PrintLicense( cout );
		ERRi();
		break;
	case cProcess:
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

static void Process_(
	const char *Source,
	const char *Destination,
	const char *Namespace,
	bso::bool__ Indent )
{
ERRProlog
	flf::file_oflow___ OFlow;
	txf::text_oflow__ TOFlow( OFlow );
	flf::file_iflow___ IFlow;
	xtf::extended_text_iflow__ XTFlow;
	str::string ErrorFileName;
	tol::E_FPOINTER___( char ) Directory;
	bso::bool__ BackedUp = false;
ERRBegin
	if ( Source != NULL ) {
		if ( IFlow.Init( Source, err::hSkip ) != fil::sSuccess ) {
			cerr << "Unable to open file '" << Source << "' for reading !" << txf::nl;
			ERRExit( evInputOutput );
		}

		XTFlow.Init( IFlow );

		Directory = fnm::GetLocation( Source );
	} else
		XTFlow.Init( cio::cinf );

	if ( Destination != NULL ) {
		fil::CreateBackupFile( Destination, fil::hbfDuplicate );

		BackedUp = true;

		if ( OFlow.Init( Destination, err::hSkip ) != fil::sSuccess ) {
			cerr << "Unable to open file '" << Destination << "' for writing !" << txf::nl;
			ERRExit( evInputOutput );
		}
	}

	ErrorFileName.Init();

	if ( !xml::Normalize( XTFlow, str::string( Namespace == NULL ? DEFAULT_NAMESPACE : Namespace ), str::string( Directory == NULL ? "" : Directory ), Indent, ( Destination == NULL ? cout : TOFlow ), ErrorFileName ) ) {
		cerr << "Error ";

		if ( ErrorFileName.Amount() != 0 )
			cerr << "in file '" << ErrorFileName << "' ";

		cerr << "at line " << XTFlow.Line() << " position " << XTFlow.Column() << " !" << txf::nl;

		ERRExit( evProcessing );
	}
ERRErr
	if ( BackedUp )
		fil::RecoverBackupFile( Destination );
ERREnd
ERREpilog
}

void Go( const parameters &Parameters )
{
ERRProlog
ERRBegin
	Process_( Parameters.Source, Parameters.Destination, Parameters.Namespace, !Parameters.NoIndent );
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
