/*
	'emqtz' by Claude SIMON (csimon@zeusw.org)
	Generates a XML file describing a melody in a customized MIDI file.
	Copyright (C) 2007 Claude SIMON.

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

// $$Id$$

#include "err.h"
#include "cio.h"
#include "epsmsc.h"
#include "clnarg.h"
#include "flf.h"
#include "mscmld.h"
#include "msccmq.h"

#define NAME			"emqtz"
#define VERSION			"0.0.1"
#define COPYRIGHT_YEARS	"2007"
#define DESCRIPTION		"Generates an XML file describing a melody stored in a customized MIDI file."
#define INFO			EPSMSC_EPEIOS_PROJECT_AFFILIATION
#define AUTHOR_NAME		EPSMSC_AUTHOR_NAME
#define AUTHOR_CONTACT	EPSMSC_AUTHOR_CONTACT
#define HELP			EPSMSC_HELP_INVITATION( NAME )
#define COPYRIGHT		"Copyright (c) " COPYRIGHT_YEARS " " AUTHOR_NAME " (" AUTHOR_CONTACT ")."
#define CVS_DETAILS		("$Id$\b " + 5)
#define URL				"http://zeusw.org/intl/emqtz/"

/*$RAW$*/

using cio::CIn;
using cio::COut;
using cio::CErr;

/* Beginning of the part which handles command line arguments. */

enum command {
	cHelp,
	cVersion,
	cLicense,
	cQuantize
};

enum option {
	// o
};

struct parameters {
	STR_BUFFER___ MIDIFileBuffer, XMLFileBuffer;
	const char *MIDIFile, *XMLFile;
	parameters( void )
	{
		MIDIFile = XMLFile = NULL;
	}
};

void PrintUsage( const clnarg::description_ &Description )
{
	CLNARG_BUFFER__ Buffer;

	COut << DESCRIPTION << txf::nl;
	COut << NAME << " --version|--license|--help" << txf::nl;
	clnarg::PrintCommandUsage( Description, cVersion, "print version of " NAME " components.", clnarg::vSplit, false );
	clnarg::PrintCommandUsage( Description, cLicense, "print the license.", clnarg::vSplit, false );
	clnarg::PrintCommandUsage( Description, cHelp, "print this message.", clnarg::vOneLine, false );

	COut << NAME << " [" << Description.GetCommandLabels( cQuantize, Buffer, "," ) << "] <MIDI-source-file> [XML-target-file]" << txf::nl;
	COut << txf::tab << "converts melody in a customized MIDI file to XML." << txf::nl;

	COut << txf::nl;

	COut << "- MIDI-source-file :" << txf::nl;
	COut << txf::tab << "the MIDI source file containing the melody (see" << txf::nl;
	COut << txf::tab << URL << " for more details)." << txf::nl;

	COut << "- XML-target-file :" << txf::nl;
	COut << txf::tab << "the target XML file (if not given, standard output is used)." << txf::nl;
//	COut << NAME << " <command> [options] ..." << txf::nl;
	// Free argument description.
//	COut << "command:" << txf::nl;
//	clnarg::PrintCommandUsage( Description, c, "", false, true );
//	COut << "options:" << txf::nl;
//	clnarg::PrintOptionUsage( Description, o, "", clnarg::vSplit );
}

void PrintHeader( void )
{
	COut << NAME " V" VERSION " "__DATE__ " " __TIME__;
	COut << " by "AUTHOR_NAME " (" AUTHOR_CONTACT ")" << txf::nl;
	COut << COPYRIGHT << txf::nl;
	COut << INFO << txf::nl;
	COut << "CVS file details : " << CVS_DETAILS << txf::nl;
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
		CErr << '\'' << Unknow << "': unknow option." << txf::nl;
		COut << HELP << txf::nl;
		ERRi();
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
	case 2:
		Parameters.XMLFile = Free( P ).Convert( Parameters.XMLFileBuffer );
		P = Free.Previous( P );
	case 1:
		Parameters.MIDIFile = Free( P ).Convert( Parameters.MIDIFileBuffer );
		break;
	default:
		CErr << "Bad amount of arguments." << txf::nl;
		COut << HELP << txf::nl;
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
	Description.AddCommand( 'q', "quantize", cQuantize );
//	Description.AddOption( '', "", o );

	Analyzer.Init( argc, argv, Description );

	switch ( Analyzer.GetCommand() ) {
	case cVersion:
		PrintHeader();
		TTR.Advertise( cio::COut );
		ERRi();
		break;
	case cHelp:
		PrintUsage( Description );
		ERRi();
		break;
	case cLicense:
		epsmsc::PrintLicense( COut );
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

static void GetMelody(
	flw::iflow__ &IFlow,
	mscmld::melody_ &Melody )
{
ERRProlog
	mscmld::melody MelodyBuffer;
ERRBegin
	MelodyBuffer.Init();
	msccmq::Parse( IFlow, MelodyBuffer );

	mscmld::SplitToMatchBars( MelodyBuffer, Melody );
ERRErr
ERREnd
ERREpilog
}

static void WriteXML(
	const mscmld::melody_ &Melody,
	txf::text_oflow__ &TOFlow )
{
ERRProlog
	xml::writer Writer;
ERRBegin
	Writer.Init( TOFlow, xml::oIndent, xml::e_Default );

	mscmld::WriteXML( Melody, Writer );
ERRErr
ERREnd
ERREpilog
}

static void Go(
	flw::iflow__ &IFlow,
	txf::text_oflow__ &TOFlow )
{
ERRProlog
	mscmld::melody Melody;
ERRBegin
	Melody.Init();

	GetMelody( IFlow, Melody );

	WriteXML( Melody, TOFlow );
ERRErr
ERREnd
ERREpilog
}


static void Go(
	flw::iflow__ &IFlow,				
	const char *XMLFile )
{
ERRProlog
	bso::bool__ Backuped = false;
	flf::file_oflow___ OFlow;
	txf::text_oflow__ TOFlow;
ERRBegin
	fil::CreateBackupFile( XMLFile, fil::bmRename );
	Backuped = true;

	if ( OFlow.Init( XMLFile, err::hUserDefined ) != fil::sSuccess ) {
		CErr << "Unable to open target file '" << XMLFile << "' !" << txf::nl;
		ERRExit( EXIT_FAILURE );
	}

	TOFlow.Init( OFlow );

	Go( IFlow, TOFlow );
ERRErr
	if ( Backuped ) 
		fil::RecoverBackupFile( XMLFile );
ERREnd
ERREpilog
}

static void Go(
	const char *MIDIFile,
	const char *XMLFile )
{
ERRProlog
	flf::file_iflow___ IFlow;
ERRBegin
	if ( IFlow.Init( MIDIFile, err::hUserDefined ) != fil::sSuccess ) {
		CErr << "Unable to open file '" << MIDIFile << "' !" << txf::nl;
		ERRExit( EXIT_FAILURE );
	}

	if ( XMLFile != NULL ) {
		Go( IFlow, XMLFile );
	} else
		Go( IFlow, COut );
ERRErr
ERREnd
ERREpilog
}




void Go( const parameters &Parameters )
{
ERRProlog
ERRBegin
	Go( Parameters.MIDIFile, Parameters.XMLFile );
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
