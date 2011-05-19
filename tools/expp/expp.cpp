/*
	'expp' by Claude SIMON (claude.simon@zeusw.org)
	Preprocesseur XML
	Copyright (C) 2007-2010 Claude SIMON

	This file is part of the Epeios project (http://zeusw.org/epeios/).

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 3
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
#include "xpp.h"
#include "fnm.h"
#include "flf.h"

#define NAME			"expp"
#define VERSION			"0.3.1"
#define COPYRIGHT_YEARS	"2007-2011"
#define DESCRIPTION		"Epeios XML preprocessor"
#define AFFILIATION		EPSMSC_EPEIOS_AFFILIATION
#define AUTHOR_NAME		EPSMSC_AUTHOR_NAME
#define AUTHOR_CONTACT	EPSMSC_AUTHOR_CONTACT
#define HELP			EPSMSC_HELP_INVITATION( NAME )
#define COPYRIGHT		EPSMSC_COPYRIGHT( COPYRIGHT_YEARS )
#define CVS_DETAILS		("$Id$\b " + 5)

#define DEFAULT_NAMESPACE	XPP_PREPROCESSOR_DEFAULT_NAMESPACE

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

enum command__ {
	cHelp,
	cVersion,
	cLicense,
	cProces,
	cEncrypt,
	c_amount,
	c_Undefined
};

enum option {
	// Définition du namespace.
	oNamespace,
	// Suppression de l'indentation
	oNoIndent

};

#define STRING_PARAM___( name )	CLNARG_STRING_PARAM___( name )

struct parameters {
	STRING_PARAM___( Namespace );
	STRING_PARAM___(  Source );
	STRING_PARAM___( Destination );
	bso::bool__ NoIndent;
	command__ Command;
	parameters( void )
	{
		NoIndent = false;
		Command = c_Undefined;
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
	cout << txf::pad << "source-file:" << txf::nl << txf::tab << "source file; stdin if none." << txf::nl;
	cout << txf::pad << "dest-file:" << txf::nl << txf::tab << "destination file; stdout if none." << txf::nl;
	cout << "command: " << txf::nl;
	clnarg::PrintCommandUsage( Description, cProces, "Process XML file.", clnarg::vSplit, true );
	clnarg::PrintCommandUsage( Description, cEncrypt, "Encrypt XML file.", clnarg::vSplit, true );
//	clnarg::PrintCommandUsage( Description, c, "", false, true );
	cout << "options:" << txf::nl;
	clnarg::PrintOptionUsage( Description, oNamespace, "<namespace>", "<namespace> becomes tags namespace; '" DEFAULT_NAMESPACE "' by default.", clnarg::vSplit );
	clnarg::PrintOptionUsage( Description, oNoIndent, "suppress indentation.", clnarg::vSplit );
//	clnarg::PrintOptionUsage( Description, o, "", clnarg::vSplit );
}

void PrintHeader( void )
{
	cout << NAME " V" VERSION " "__DATE__ " " __TIME__;
	cout << " by "AUTHOR_NAME " (" AUTHOR_CONTACT ")" << txf::nl;
	cout << COPYRIGHT << txf::nl;
	cout << AFFILIATION << txf::nl;
//	cout << "CVS file details : " << CVS_DETAILS << txf::nl;
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
	clnarg::buffer__ Buffer;
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
				cout << "Option " << Analyzer.Description().GetOptionLabels( oNamespace, Buffer ) << " must have one argument !" << txf::nl;
				ERRExit( evParameters );
			}

			Argument.Convert( Parameters.Namespace );

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
		Free( P ).Convert( Parameters.Destination );
		P = Free.Previous( P );
	case 1:
		Free( P ).Convert( Parameters.Source );
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
	Description.AddCommand( 'p', "process", cProces );
	Description.AddCommand( CLNARG_NO_SHORT, "encrypt", cEncrypt );
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
	case cProces:
	case cEncrypt:
		Parameters.Command = (command__)Analyzer.GetCommand();
		break;
	case CLNARG_NONE:
		Parameters.Command = cProces;
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
	txf::text_oflow__ TOFlow;
	flf::file_iflow___ IFlow;
	str::string ErrorFileName;
	const char *Directory;
	bso::bool__ BackedUp = false;
	xpp::status__ Status = xpp::s_Undefined;
	FNM_BUFFER___ Buffer;
	xtf::coord__ Coord;
ERRBegin
	if ( Source != NULL ) {
		if ( IFlow.Init( Source, err::hUserDefined ) != fil::sSuccess ) {
			cerr << "Unable to open file '" << Source << "' for reading !" << txf::nl;
			ERRExit( evInputOutput );
		}

		Directory = fnm::GetLocation( Source, Buffer );
	}

	if ( Destination != NULL ) {
		fil::CreateBackupFile( Destination, fil::bmDuplicate );

		BackedUp = true;

		if ( OFlow.Init( Destination, err::hUserDefined ) != fil::sSuccess ) {
			cerr << "Unable to open file '" << Destination << "' for writing !" << txf::nl;
			ERRExit( evInputOutput );
		}

		TOFlow.Init( OFlow );
	}

	ErrorFileName.Init();


	xml::WriteXMLHeader( Destination == NULL ? cio::cout : TOFlow, xml::encoding__() );

	( Destination == NULL ? cout : TOFlow ) << txf::nl;

	if ( ( Status = xpp::Process( str::string( Namespace == NULL ? DEFAULT_NAMESPACE : Namespace ),
								  IFlow,
								  str::string( Directory == NULL ? (const char *)"" : Directory ),
								  Indent ? xml::oIndent : xml::oCompact,
								  ( Destination == NULL ? cout : TOFlow ),  Coord, ErrorFileName ) ) != xpp::sOK ) {
		cio::cerr << "Error ";

		if ( ErrorFileName.Amount() != 0 )
			cerr << "in file '" << ErrorFileName << "' ";

		cerr << "at line " << Coord.Line << ", column " << Coord.Column << " : " << xpp::Label( Status ) << " !" << txf::nl;

		ERRExit( evProcessing );
	}
ERRErr
	if ( BackedUp )
		fil::RecoverBackupFile( Destination );
ERREnd
ERREpilog
}

static void Encrypt_(
	const char *Source,
	const char *Destination,
	const char *Namespace,
	bso::bool__ Indent )
{
ERRProlog
	flf::file_oflow___ OFlow;
	txf::text_oflow__ TOFlow;
	flf::file_iflow___ IFlow;
	bso::bool__ BackedUp = false;
	xpp::status__ Status = xpp::s_Undefined;
	xtf::coord__ Coord;
ERRBegin
	if ( Source != NULL ) {
		if ( IFlow.Init( Source, err::hUserDefined ) != fil::sSuccess ) {
			cerr << "Unable to open file '" << Source << "' for reading !" << txf::nl;
			ERRExit( evInputOutput );
		}
	}

	if ( Destination != NULL ) {
		fil::CreateBackupFile( Destination, fil::bmDuplicate );

		BackedUp = true;

		if ( OFlow.Init( Destination, err::hUserDefined ) != fil::sSuccess ) {
			cerr << "Unable to open file '" << Destination << "' for writing !" << txf::nl;
			ERRExit( evInputOutput );
		}

		TOFlow.Init( OFlow );
	}

	if ( ( Status = xpp::Encrypt( str::string( Namespace == NULL ? DEFAULT_NAMESPACE : Namespace ),
								  IFlow,
								  Indent ? xml::oIndent : xml::oCompact,
								  ( Destination == NULL ? cout : TOFlow ),  Coord ) ) != xpp::sOK ) {
		cio::cerr << "Error at line " << Coord.Line << ", column " << Coord.Column << " : " << xpp::Label( Status ) << " !" << txf::nl;

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
	switch ( Parameters.Command ) {
	case cProces:
		Process_( Parameters.Source, Parameters.Destination, Parameters.Namespace, !Parameters.NoIndent );
		break;
	case cEncrypt:
		Encrypt_( Parameters.Source, Parameters.Destination, Parameters.Namespace, !Parameters.NoIndent );
		break;
	default:
		ERRc();
		break;
}

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
	cio::cout.reset();
	cio::cerr.reset();
ERRFEpilog
	return ERRExitValue;
}
