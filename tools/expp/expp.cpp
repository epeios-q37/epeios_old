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
	cout << LocaleRack.GetTranslation( "ProgramDescription", "", TranslationBuffer ) << '.'  << txf::nl;
	cout << txf::nl;

	cout << NAME " " << Description.GetCommandLabels( cVersion, Buffer ) << txf::nl;
	Text.Init();
	clnarg::GetVersionCommandDescription( LocaleRack, Text );
	cout << txf::pad << Text << '.' << txf::nl;

	cout << NAME " " << Description.GetCommandLabels( cLicense, Buffer ) << txf::nl;
	Text.Init();
	clnarg::GetLicenseCommandDescription( LocaleRack, Text );
	cout << txf::pad << Text << '.' << txf::nl;

	cout << NAME " " << Description.GetCommandLabels( cHelp, Buffer ) << txf::nl;
	Text.Init();
	clnarg::GetHelpCommandDescription( LocaleRack, Text );
	cout << txf::pad << Text << '.' << txf::nl;

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
	cout << NAME << " [" << Description.GetCommandLabels( cProcess, Buffer );
	cout << "] [" << Description.GetOptionLabels( oNamespace, Buffer ) << " <ns>]";
	cout << " [" << Description.GetOptionLabels( oNoIndent, Buffer );
	cout << "] [<src> [<dst>]]";
	cout << txf::nl;
	Translation.Init();
	cout << txf::pad << global::GetTranslation( global::mProcessCommandDescription, Translation ) << '.' << txf::nl;

	cout << NAME << ' ' << Description.GetCommandLabels( cEncrypt, Buffer );
	cout << " [" << Description.GetOptionLabels( oNamespace, Buffer ) << " <ns>]";
	cout << " [" << Description.GetOptionLabels( oNoIndent, Buffer );
	cout << "] [<src> [<dst>]]";
	cout << txf::nl;
	Translation.Init();
	cout << txf::pad << global::GetTranslation( global::mEncryptCommandDescription, Translation ) << '.' << txf::nl;

	cout << txf::nl;

// Options.
	Translation.Init();
	cout << clnarg::GetOptionsWordingTranslation( LocaleRack, Translation );
	cout << " :" << txf::nl;

	cout << txf::pad << Description.GetOptionLabels( oNamespace, Buffer ) << " <ns> :" << txf::nl;
	cout << txf::tab;
	global::Display( mNamespaceOptionDescription );
	cout << '.' << txf::nl;

	cout << txf::pad << Description.GetOptionLabels( oNoIndent, Buffer ) << " :" << txf::nl;
	cout << txf::tab;
	global::Display( mNoIndentOptionDescription );
	cout << '.' << txf::nl;

	cout << txf::nl;

// Arguments.
	Translation.Init();
	cout << clnarg::GetArgumentsWordingTranslation( LocaleRack, Translation );
	cout << " :" << txf::nl;

	cout << txf::pad << "<src> :" << txf::nl;
	cout << txf::tab;
	global::Display( mSourceFileArgumentDescription );
	cout << '.' << txf::nl;

	cout << txf::pad << "<dst> :" << txf::nl;
	cout << txf::tab;
	global::Display( mDestFileArgumentDescription );
	cout << '.' << txf::nl;

ERRErr
ERREnd
ERREpilog
}

void PrintHeader( void )
{
	cout << NAME " V" VERSION << " by "AUTHOR_NAME " (" AUTHOR_CONTACT ")" << txf::nl;
	cout << COPYRIGHT << txf::nl;
	cout << AFFILIATION << txf::nl;
	cout << txf::pad << "Build : "__DATE__ " " __TIME__ << txf::nl;
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
//		TTR.Advertise( cout );
		ERRExit( EXIT_SUCCESS );
		break;
	case cHelp:
		PrintUsage( Description );
		ERRExit( EXIT_SUCCESS );
		break;
	case cLicense:
		epsmsc::PrintLicense( cout );
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
ERRBegin
	Context.Init();

	xml::WriteXMLHeader( OFlow, xml::encoding__() );

	OFlow << txf::nl;

	if ( ( Status = xpp::Process( IFlow, xpp::criterions___( str::string( Directory == NULL ? (const char *)"" : Directory ), str::string(),
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

	Process_( Source == NULL ? cin.Flow() : IFlow, Namespace, Directory, Indent ? xml::oIndent : xml::oCompact, Destination == NULL ? cout : TOFlow );

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
								  ( Destination == NULL ? cout : TOFlow ),  Context ) != xpp::sOK )
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
	Initialize();
	Main( argc, argv );
ERRFErr
ERRFEnd
	Release();
ERRFEpilog
	return ERRExitValue;
}
