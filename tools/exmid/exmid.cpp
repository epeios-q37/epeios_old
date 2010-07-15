/*
	'exmid' by Claude SIMON (csimon@zeusw.org)
	MIDI to XML and back converter.
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

// $$Id$$

#include "err.h"
#include "cio.h"
#include "epsmsc.h"
#include "clnarg.h"
#include "mscmdx.h"
#include "flf.h"
#include "xml.h"
#include "ltf.h"
#include "fnm.h"

#define NAME			"exmid"
#define VERSION			"0.1.2"
#define COPYRIGHT_YEARS	"2007; 2009"
#define DESCRIPTION		"MIDI to XML and XML to MIDI converter."
#define INFO			EPSMSC_EPEIOS_TEXT
#define AUTHOR_NAME		EPSMSC_AUTHOR_NAME
#define AUTHOR_CONTACT	EPSMSC_AUTHOR_CONTACT
#define HELP			EPSMSC_HELP_INVITATION( NAME )
#define COPYRIGHT		EPSMSC_COPYRIGHT( COPYRIGHT_YEARS )
#define CVS_DETAILS		("$Id$\b " + 5)

/*$RAW$*/

using cio::cin;
using cio::cout;
using cio::cerr;

/* Beginning of the part which handles command line arguments. */

enum command__ {
	cHelp,
	cVersion,
	cLicense,
	// Convert from MIDI to XMID.
	cMIDToX,
	// Convert from XMID to MID.
	cXToMID,
	// Déduit l'action à réaliser de l'extension du fichier.
	cAutomatic,
	c_amount,
	c_Undefined
};

enum option {
	// o
};

#define STRING_PARAM( name )	CLNARG_STRING_PARAM( name )

struct parameters {
	command__ Command;
	STRING_PARAM( MIDIFileName );
	STRING_PARAM( XMIDFileName );
	STRING_PARAM( AutomaticFileName );
	parameters( void )
	{
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

	cout << NAME << " " << Description.GetCommandLabels( cMIDToX, "," ) << " <MIDI-source-file> [XMID-target-file]" << txf::nl;
	cout << txf::tab << "converts file in MIDI format to XMID format." << txf::nl;

	cout << NAME << " " << Description.GetCommandLabels( cXToMID, "," ) << " <XMID-source-file> <MIDI-target-file>" << txf::nl;
	cout << txf::tab << "converts file in XMID format to MIDI file." << txf::nl;

	cout << NAME << " " << Description.GetCommandLabels( cAutomatic, "," ) << " <source-file>" << txf::nl;
	cout << txf::tab << "make conversion depending from 'source-file' extension." << txf::nl;

	cout << txf::nl;

	cout << "- MIDI-source-file : file in MIDI format to convert to XMID format." << txf::nl;
	cout << "- XMID-source-file : file in XMID format to convert to MIDI format." << txf::nl;
	cout << "- XMID-target-file : target XMID file (standard output if not given)." << txf::nl;
	cout << "- MIDI-target-file : target MIDI file." << txf::nl;
	cout << "- source-file : " << txf::nl
		 << txf::tab << "- with 'mid' or 'MID' extension : file in MIDI format to convert in" << txf::nl << txf::tab << "  XMID format (with 'xmid' or 'XMID' extension)." << txf::nl
		 << txf::tab << "- with 'xmid' or 'XMID' extension : file in XMID format to convert in" << txf::nl << txf::tab << "  MIDI format (with 'mid' or 'MID' extension)." << txf::nl;


	// Free argument description.
//	clnarg::PrintCommandUsage( Description, c, "", false, true );
//	cout << "options:" << txf::nl;
//	clnarg::PrintOptionUsage( Description, o, "", clnarg::vSplit );
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

	switch ( Parameters.Command ) {
	case cMIDToX:
		switch ( Free.Amount() ) {
		case 2:
			Parameters.XMIDFileName = Free( P ).Convert( Parameters.AutomaticFileNameBuffer );
			P = Free.Previous( P );
		case 1:
			Parameters.MIDIFileName = Free( P ).Convert( Parameters.MIDIFileNameBuffer );
			break;
		default:
			cerr << "Bad amount of arguments." << txf::nl;
			cout << HELP << txf::nl;
			ERRExit( EXIT_FAILURE );
			break;
			}
		break;
	case cXToMID:
		switch ( Free.Amount() ) {
		case 2:
			Parameters.MIDIFileName = Free( P ).Convert( Parameters.MIDIFileNameBuffer );
			P = Free.Previous( P );
			Parameters.XMIDFileName = Free( P ).Convert( Parameters.MIDIFileNameBuffer );
			break;
		default:
			cerr << "Bad amount of arguments." << txf::nl;
			cout << HELP << txf::nl;
			ERRExit( EXIT_FAILURE );
			break;
		}
		break;
	case cAutomatic:
		switch ( Free.Amount() ) {
		case 1:
			Parameters.AutomaticFileName = Free( P ).Convert( Parameters.AutomaticFileNameBuffer );
			break;
		default:
			cerr << "Bad amount of arguments." << txf::nl;
			cout << HELP << txf::nl;
			ERRExit( EXIT_FAILURE );
			break;
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
	parameters &Parameters )
{
ERRProlog
	clnarg::description Description;
	clnarg::analyzer___ Analyzer;
ERRBegin
	Description.Init();

//	Description.AddCommand( '', "", c );
	Description.AddCommand( 'x', "MIDToX", cMIDToX );
	Description.AddCommand( 'm', "XToMID", cXToMID );
	Description.AddCommand( 'a', "automatic", cAutomatic );
	Description.AddCommand( CLNARG_NO_SHORT, "version", cVersion );
	Description.AddCommand( CLNARG_NO_SHORT, "help", cHelp );
	Description.AddCommand( CLNARG_NO_SHORT, "license", cLicense );
//	Description.AddOption( '', "", o );

	Analyzer.Init( argc, argv, Description );

	switch ( Parameters.Command = (command__)Analyzer.GetCommand() ) {
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
		epsmsc::PrintLicense( cio::cout );
		ERRi();
		break;
	case cMIDToX:
	case cXToMID:
	case cAutomatic:
		Parameters.Command = (command__)Analyzer.GetCommand();
		break;
//	case c:
	case CLNARG_NONE:
		cerr << "Missing command !" << txf::nl;
		cout << HELP << txf::nl;
		ERRExit( EXIT_FAILURE );
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

void MIDToX(
	flw::iflow__ &IFlow,
	txf::text_oflow__ &OFlow )
{
ERRProlog
	xml::writer Writer;
ERRBegin
	xml::WriteXMLHeader( OFlow );
	OFlow << txf::nl;

	Writer.Init( OFlow );

	mscmdx::MIDIToXMID( IFlow, mscmdm::oFile, Writer );
ERRErr
ERREnd
ERREpilog
}

void MIDToX(
	flw::iflow__ &IFlow,
	const char *TargetFile )
{
ERRProlog
	flf::file_oflow___ OFlow;
	txf::text_oflow__ TOFlow( OFlow );
	bso::bool__ BackupCreated = false;
ERRBegin
	if ( TargetFile == NULL )
		MIDToX( IFlow, cout );
	else {
		fil::CreateBackupFile( TargetFile, fil::hbfRename );

		BackupCreated = true;

		if ( OFlow.Init( TargetFile, fil::mRemove, err::hSkip ) != fil::sSuccess ) {
			cerr << "Unable to open target file '" << TargetFile << "' !" << txf::nl;
			ERRExit( EXIT_FAILURE );
		}

		MIDToX( IFlow, TOFlow );
	}
ERRErr
	if ( BackupCreated )
		fil::RecoverBackupFile( TargetFile );
ERREnd
ERREpilog
}

void MIDToX(
	const char *SourceFile,
	const char *TargetFile )
{
ERRProlog
	flf::file_iflow___ IFlow;
ERRBegin
	if ( IFlow.Init( SourceFile, err::hSkip ) != fil::sSuccess ) {
		cerr << "Unable to open source file '" << SourceFile << "' !" << txf::nl;
		ERRExit( EXIT_FAILURE );
	}

	MIDToX( IFlow, TargetFile );
ERRErr
ERREnd
ERREpilog
}

/**/

void XToMID(
	xtf::extended_text_iflow__ &IFlow,
	flw::oflow__ &OFlow )
{
ERRProlog
	xml::status__ Status = xml::s_Undefined;
ERRBegin
	if ( ( Status = mscmdx::XMIDToMIDI( IFlow, OFlow ) ) != xml::sOK ) {
		cerr << "Error " << xml::GetLabel( Status ) << " : line " << IFlow.Coord().Line << ", character " << IFlow.Coord().Column << txf::nl;
		ERRExit( EXIT_FAILURE );
	}
ERRErr
ERREnd
ERREpilog
}

void XToMID(
	xtf::extended_text_iflow__ &IFlow,
	const char *TargetFile )
{
ERRProlog
	flf::file_oflow___ OFlow;
	bso::bool__ BackupCreated = false;
ERRBegin
	if ( TargetFile == NULL )
		ERRc();

	fil::CreateBackupFile( TargetFile, fil::hbfRename );

	BackupCreated = true;

	if ( OFlow.Init( TargetFile, fil::mRemove, err::hSkip ) != fil::sSuccess ) {
		cerr << "Unable to open target file '" << TargetFile << "' !" << txf::nl;
		ERRExit( EXIT_FAILURE );
	}

	XToMID( IFlow, OFlow );
ERRErr
	if ( BackupCreated )
		fil::RecoverBackupFile( TargetFile );
ERREnd
ERREpilog
}

void XToMID(
	const char *SourceFile,
	const char *TargetFile )
{
ERRProlog
	flf::file_iflow___ IFlow;
	xtf::extended_text_iflow__ TFlow;
ERRBegin
	if ( IFlow.Init( SourceFile, err::hSkip ) != fil::sSuccess ) {
		cerr << "Unable to open source file '" << SourceFile << "' !" << txf::nl;
		ERRExit( EXIT_FAILURE );
	}

	TFlow.Init( IFlow );

	XToMID( TFlow, TargetFile );
ERRErr
ERREnd
ERREpilog
}

/**/

void Automatic(
	const char *FileNameRoot,
	const char *Location,
	bso::bool__ IsMID,
	bso::bool__ IsUppercase )
{
ERRProlog
	FNM_BUFFER___ SourceFileBuffer, DestFileBuffer;
	const char *SourceFile = NULL, *DestFile = NULL;
	const char *SourceFileExtension = NULL, *DestFileExtension = NULL;
ERRBegin
	if ( IsMID ) {
		if ( IsUppercase ) {
			SourceFileExtension = ".MID";
			DestFileExtension = ".XMID";
		} else {
			SourceFileExtension = ".mid";
			DestFileExtension = ".xmid";
		}
	} else {
		if ( IsUppercase ) {
			SourceFileExtension = ".XMID";
			DestFileExtension = ".MID";
		} else {
			SourceFileExtension = ".xmid";
			DestFileExtension = ".mid";
		}
	}

	SourceFile = fnm::BuildFileName( Location, FileNameRoot, SourceFileExtension, SourceFileBuffer );
	DestFile = fnm::BuildFileName( Location, FileNameRoot, DestFileExtension, DestFileBuffer );

	if ( IsMID )
		MIDToX( SourceFile, DestFile );
	else
		XToMID( SourceFile, DestFile );
ERRErr
ERREnd
ERREpilog
}

void Automatic( const char *SourceFile )
{
ERRProlog
	bso::bool__ IsMID = false;
	bso::bool__ IsUpperCase = false;
	FNM_BUFFER___ Location, FileNameRoot;
ERRBegin
	const char *Extension = fnm::GetExtension( SourceFile );

	if ( Extension == NULL ) {
		cerr << "Unable to determine extensions !" << txf::nl;
		ERRExit( EXIT_FAILURE )
	}

	if ( !strcmp( Extension, ".mid" ) ) {
		IsMID = true;
		IsUpperCase = false;
	} else if ( !strcmp( Extension, ".MID" ) ) {
		IsMID = true;
		IsUpperCase = true;
	} else if ( !strcmp( Extension, ".XMID" ) ) {
		IsMID = false;
		IsUpperCase = false;
	} else if ( !strcmp( Extension, ".xmid" ) ) {
		IsMID = false;
		IsUpperCase = false;
	} else {
		cerr << "Given file has no 'mid', 'MID', xmid or 'XMID' extension !" << txf::nl;
		ERRExit( EXIT_FAILURE );
	}

	Automatic( fnm::GetFileNameRoot( SourceFile, FileNameRoot ), fnm::GetLocation( SourceFile, Location ), IsMID, IsUpperCase );
ERRErr
ERREnd
ERREpilog
}

void Go( const parameters &Parameters )
{
ERRProlog
ERRBegin
	switch ( Parameters.Command ) {
	case cMIDToX:
		MIDToX( Parameters.MIDIFileName, Parameters.XMIDFileName );
		break;
	case cXToMID:
		XToMID( Parameters.XMIDFileName, Parameters.MIDIFileName );
		break;
	case cAutomatic:
		Automatic( Parameters.AutomaticFileName );
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
ERRFEpilog
	return ERRExitValue;
}
