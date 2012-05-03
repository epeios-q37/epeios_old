/*
	'expp' by Claude SIMON (claude.simon@zeusw.org)
	XML Preprocessor.
	Copyright (C) 2007-2011 Claude SIMON

	This file is part of the Epeios project (http://zeusw.org/epeios/).

    This file is part of 'expp'.

    'expp' is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'expp' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with 'expp'.  If not, see <http://www.gnu.org/licenses/>.
*/

// $$Id$$

#include "expp.h"
#include "global.h"

#include "err.h"
#include "cio.h"
#include "epsmsc.h"
#include "clnarg.h"
#include "xpp.h"
#include "fnm.h"
#include "flf.h"

using namespace global;

#define DEFAULT_NAMESPACE	XPP__PREPROCESSOR_DEFAULT_NAMESPACE

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
	cProcess,
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

static void PrintSpecialsCommandsDescriptions( const clnarg::description_ &Description )
{
ERRProlog
	str::string Text;
	STR_BUFFER___ TranslationBuffer;
	CLNARG_BUFFER__ Buffer;
ERRBegin
	COut << LocaleRack.GetTranslation( "ProgramDescription", "", TranslationBuffer ) << '.'  << txf::nl;
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


void PrintUsage( const clnarg::description_ &Description )
{
ERRProlog
	str::string Translation;
	STR_BUFFER___ TBuffer;
	CLNARG_BUFFER__ Buffer;
ERRBegin
	PrintSpecialsCommandsDescriptions( Description );

	// Commands.
	COut << NAME << " [" << Description.GetCommandLabels( cProcess, Buffer );
	COut << "] [" << Description.GetOptionLabels( oNamespace, Buffer ) << " <ns>]";
	COut << " [" << Description.GetOptionLabels( oNoIndent, Buffer );
	COut << "] [<src> [<dst>]]";
	COut << txf::nl;
	Translation.Init();
	COut << txf::pad << global::GetTranslation( global::mProcessCommandDescription, Translation ) << '.' << txf::nl;

	COut << NAME << ' ' << Description.GetCommandLabels( cEncrypt, Buffer );
	COut << " [" << Description.GetOptionLabels( oNamespace, Buffer ) << " <ns>]";
	COut << " [" << Description.GetOptionLabels( oNoIndent, Buffer );
	COut << "] [<src> [<dst>]]";
	COut << txf::nl;
	Translation.Init();
	COut << txf::pad << global::GetTranslation( global::mEncryptCommandDescription, Translation ) << '.' << txf::nl;

	COut << txf::nl;

// Options.
	Translation.Init();
	COut << clnarg::GetOptionsWordingTranslation( LocaleRack, Translation );
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
	Translation.Init();
	COut << clnarg::GetArgumentsWordingTranslation( LocaleRack, Translation );
	COut << " :" << txf::nl;

	COut << txf::pad << "<src> :" << txf::nl;
	COut << txf::tab;
	global::Display( mSourceFileArgumentDescription );
	COut << '.' << txf::nl;

	COut << txf::pad << "<dst> :" << txf::nl;
	COut << txf::tab;
	global::Display( mDestFileArgumentDescription );
	COut << '.' << txf::nl;

ERRErr
ERREnd
ERREpilog
}

void PrintHeader( void )
{
	COut << NAME " V" VERSION << " by "AUTHOR_NAME " (" AUTHOR_CONTACT ")." << txf::nl;
	COut << COPYRIGHT << txf::nl;
	COut << txf::pad << AFFILIATION << txf::nl;
	COut << txf::pad << DEPENDENCE << txf::nl;
	COut << txf::tab << "Build : "__DATE__ " " __TIME__ << txf::nl;
}

static void AnalyzeOptions(
	clnarg::analyzer___ &Analyzer,
	parameters &Parameters )
{
ERRProlog
	mdr::row__ P;
	clnarg::option_list Options;
	clnarg::id__ Option;
	const bso::char__ *Unknown = NULL;
	clnarg::argument Argument;
	clnarg::buffer__ Buffer;
ERRBegin
	Options.Init();

	if ( ( Unknown = Analyzer.GetOptions( Options ) ) != NULL )
		clnarg::ReportUnknownOptionError( Unknown, NAME, LocaleRack );

	P = Options.First();

	while( P != NONE ) {
		Argument.Init();

		switch( Option = Options( P ) ) {
		case oNamespace:
			Analyzer.GetArgument( Option, Argument );

			if ( Argument.Amount() == 0 )
				clnarg::ReportMissingOptionArgumentError( Analyzer.Description().GetOptionLabels( oNamespace, Buffer ), NAME, LocaleRack );

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
	mdr::row__ P;
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
		clnarg::ReportWrongNumberOfArgumentsError( NAME, LocaleRack );
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
	Description.AddCommand( CLNARG_NO_SHORT, "encrypt", cEncrypt );
//	Description.AddOption( '', "", o );
	Description.AddOption( 'n', "namespace", oNamespace );
	Description.AddOption( CLNARG_NO_SHORT, "no-indent", oNoIndent );

	Analyzer.Init( argc, argv, Description );

	switch ( Analyzer.GetCommand() ) {
	case cVersion:
		PrintHeader();
//		TTR.Advertise( COut );
		ERRExit( EXIT_SUCCESS );
		break;
	case cHelp:
		PrintUsage( Description );
		ERRExit( EXIT_SUCCESS );
		break;
	case cLicense:
		epsmsc::PrintLicense( COut );
		ERRExit( EXIT_SUCCESS );
		break;
	case cProcess:
	case cEncrypt:
		Parameters.Command = (command__)Analyzer.GetCommand();
		break;
	case CLNARG_NONE:
		Parameters.Command = cProcess;
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
	flw::iflow__ &IFlow,
	const char *Namespace,
	const char *Directory,
	xml::outfit__ Outfit,
	txf::text_oflow__ &OFlow )
{
ERRProlog
	xpp::status__ Status = xpp::s_Undefined;
	xpp::context___ Context;
	xtf::extended_text_iflow__ XFlow;
ERRBegin
	Context.Init();

	XFlow.Init( IFlow );

	if ( ( Status = xpp::Process( XFlow, xpp::criterions___( str::string( Directory == NULL ? (const char *)"" : Directory ), str::string(),
															 str::string( Namespace == NULL ? DEFAULT_NAMESPACE : Namespace ) ),
								  Outfit, OFlow,  Context ) ) != xpp::sOK )
		Report( eProcessingError, &Context );
ERRErr
ERREnd
ERREpilog
}

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
	const char *Directory;
	bso::bool__ BackedUp = false;
	FNM_BUFFER___ Buffer;
ERRBegin
	if ( Source != NULL ) {
		if ( IFlow.Init( Source, err::hUserDefined ) != fil::sSuccess )
			Report( eUnableToOpenFile, Source );

		Directory = fnm::GetLocation( Source, Buffer );
	}

	if ( Destination != NULL ) {
		global::CreateBackupFile( Destination );

		BackedUp = true;

		if ( OFlow.Init( Destination, err::hUserDefined ) != fil::sSuccess )
			Report( eUnableToOpenFile, Destination );

		TOFlow.Init( OFlow );
	}

	Process_( Source == NULL ? CIn.Flow() : IFlow, Namespace, Directory, Indent ? xml::oIndent : xml::oCompact, Destination == NULL ? COut : TOFlow );

ERRErr
	if ( BackedUp )
		global::RecoverBackupFile( Destination );
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
	xpp::context___ Context;
ERRBegin
	if ( Source != NULL )
		if ( IFlow.Init( Source, err::hUserDefined ) != fil::sSuccess )
			Report( eUnableToOpenFile, Source );

	if ( Destination != NULL ) {
		global::CreateBackupFile( Destination );

		BackedUp = true;

		if ( OFlow.Init( Destination, err::hUserDefined ) != fil::sSuccess )
			Report( eUnableToOpenFile, Destination );

		TOFlow.Init( OFlow );
	}

	Context.Init();

	if ( xpp::Encrypt( str::string( Namespace == NULL ? DEFAULT_NAMESPACE : Namespace ),
								  IFlow,
								  Indent ? xml::oIndent : xml::oCompact,
								  ( Destination == NULL ? COut : TOFlow ),  Context ) != xpp::sOK )
		Report( eEncryptionError, &Context );
ERRErr
	if ( BackedUp )
		global::RecoverBackupFile( Destination );
ERREnd
ERREpilog
}

void Go( const parameters &Parameters )
{
ERRProlog
ERRBegin
	switch ( Parameters.Command ) {
	case cProcess:
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

void Main(
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
