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
#define INFO			EPSMSC_EPEIOS_PROJECT_AFFILIATION
#define AUTHOR_NAME		EPSMSC_AUTHOR_NAME
#define AUTHOR_CONTACT	EPSMSC_AUTHOR_CONTACT
#define HELP			EPSMSC_HELP_INVITATION( NAME )
#define COPYRIGHT		EPSMSC_COPYRIGHT( COPYRIGHT_YEARS )
#define CVS_DETAILS		("$Id$\b " + 5)

/*$RAW$*/

using cio::CIn;
using cio::COut;
using cio::CErr;

/* Beginning of the part which handles command line arguments. */

enum command__ {
	cHelp,
	cVersion,
	cLicense,
	// Convert from MIDI to XMID.
	cMIDToX,
	// Convert from XMID to MID.
	cXToMID,
	// D�duit l'action � r�aliser de l'extension du fichier.
	cAutomatic,
	c_amount,
	c_Undefined
};

enum option {
	// o
};

#define STRING_PARAM___( name )	CLNARG_STRING_PARAM___( name )

struct parameters {
	command__ Command;
	STRING_PARAM___( MIDIFileName );
	STRING_PARAM___( XMIDFileName );
	STRING_PARAM___( AutomaticFileName );
	parameters( void )
	{
		Command = c_Undefined;
	}
};

void PrintUsage( const clnarg::description_ &Description )
{
	clnarg::buffer__ Buffer;

	COut << DESCRIPTION << txf::nl;
	COut << NAME << " --version|--license|--help" << txf::nl;
	clnarg::PrintCommandUsage( Description, cVersion, "print version of " NAME " components.", clnarg::vSplit, false );
	clnarg::PrintCommandUsage( Description, cLicense, "print the license.", clnarg::vSplit, false );
	clnarg::PrintCommandUsage( Description, cHelp, "print this message.", clnarg::vOneLine, false );

	COut << NAME << " " << Description.GetCommandLabels( cMIDToX, Buffer, "," ) << " <MIDI-source-file> [XMID-target-file]" << txf::nl;
	COut << txf::tab << "converts file in MIDI format to XMID format." << txf::nl;

	COut << NAME << " " << Description.GetCommandLabels( cXToMID, Buffer, "," ) << " <XMID-source-file> <MIDI-target-file>" << txf::nl;
	COut << txf::tab << "converts file in XMID format to MIDI file." << txf::nl;

	COut << NAME << " " << Description.GetCommandLabels( cAutomatic, Buffer, "," ) << " <source-file>" << txf::nl;
	COut << txf::tab << "make conversion depending from 'source-file' extension." << txf::nl;

	COut << txf::nl;

	COut << "- MIDI-source-file : file in MIDI format to convert to XMID format." << txf::nl;
	COut << "- XMID-source-file : file in XMID format to convert to MIDI format." << txf::nl;
	COut << "- XMID-target-file : target XMID file (standard output if not given)." << txf::nl;
	COut << "- MIDI-target-file : target MIDI file." << txf::nl;
	COut << "- source-file : " << txf::nl
		 << txf::tab << "- with 'mid' or 'MID' extension : file in MIDI format to convert in" << txf::nl << txf::tab << "  XMID format (with 'xmid' or 'XMID' extension)." << txf::nl
		 << txf::tab << "- with 'xmid' or 'XMID' extension : file in XMID format to convert in" << txf::nl << txf::tab << "  MIDI format (with 'mid' or 'MID' extension)." << txf::nl;


	// Free argument description.
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
	mdr::row__ P;
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
	mdr::row__ P;
ERRBegin
	Free.Init();

	Analyzer.GetArguments( Free );

	P = Free.Last();

	switch ( Parameters.Command ) {
	case cMIDToX:
		switch ( Free.Amount() ) {
		case 2:
			Free( P ).Convert( Parameters.AutomaticFileName );
			P = Free.Previous( P );
		case 1:
			Free( P ).Convert( Parameters.MIDIFileName );
			break;
		default:
			CErr << "Bad amount of arguments." << txf::nl;
			COut << HELP << txf::nl;
			ERRExit( EXIT_FAILURE );
			break;
			}
		break;
	case cXToMID:
		switch ( Free.Amount() ) {
		case 2:
			Free( P ).Convert( Parameters.MIDIFileName );
			P = Free.Previous( P );
			Free( P ).Convert( Parameters.MIDIFileName );
			break;
		default:
			CErr << "Bad amount of arguments." << txf::nl;
			COut << HELP << txf::nl;
			ERRExit( EXIT_FAILURE );
			break;
		}
		break;
	case cAutomatic:
		switch ( Free.Amount() ) {
		case 1:
			Free( P ).Convert( Parameters.AutomaticFileName );
			break;
		default:
			CErr << "Bad amount of arguments." << txf::nl;
			COut << HELP << txf::nl;
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
		TTR.Advertise( cio::COut );
		ERRi();
		break;
	case cHelp:
		PrintUsage( Description );
		ERRi();
		break;
	case cLicense:
		epsmsc::PrintLicense( cio::COut );
		ERRi();
		break;
	case cMIDToX:
	case cXToMID:
	case cAutomatic:
		Parameters.Command = (command__)Analyzer.GetCommand();
		break;
//	case c:
	case CLNARG_NONE:
		CErr << "Missing command !" << txf::nl;
		COut << HELP << txf::nl;
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
	xml::WriteXMLHeader( OFlow, xml::e_Default );
	OFlow << txf::nl;

	Writer.Init( OFlow, xml::oIndent, xml::e_Default );

	mscmdx::MIDIToXMID( IFlow, mscmdm::xTicks, Writer );
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
	txf::text_oflow__ TOFlow;
	bso::bool__ BackupCreated = false;
ERRBegin
	if ( TargetFile == NULL )
		MIDToX( IFlow, COut );
	else {
		fil::CreateBackupFile( TargetFile, fil::bmRename );

		BackupCreated = true;

		if ( OFlow.Init( TargetFile, fil::mRemove, err::hUserDefined ) != fil::sSuccess ) {
			CErr << "Unable to open target file '" << TargetFile << "' !" << txf::nl;
			ERRExit( EXIT_FAILURE );
		}

		TOFlow.Init( OFlow );

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
	if ( IFlow.Init( SourceFile, err::hUserDefined ) != fil::sSuccess ) {
		CErr << "Unable to open source file '" << SourceFile << "' !" << txf::nl;
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
	if ( ( Status = mscmdx::XMIDToMIDI( IFlow, mscmdm::xTicks, OFlow ) ) != xml::sOK ) {
		CErr << "Error " << xml::Label( Status ) << " : line " << IFlow.Coord().Line << ", character " << IFlow.Coord().Column << txf::nl;
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

	fil::CreateBackupFile( TargetFile, fil::bmRename );

	BackupCreated = true;

	if ( OFlow.Init( TargetFile, fil::mRemove, err::hUserDefined ) != fil::sSuccess ) {
		CErr << "Unable to open target file '" << TargetFile << "' !" << txf::nl;
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
	if ( IFlow.Init( SourceFile, err::hUserDefined ) != fil::sSuccess ) {
		CErr << "Unable to open source file '" << SourceFile << "' !" << txf::nl;
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
		CErr << "Unable to determine extensions !" << txf::nl;
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
		CErr << "Given file has no 'mid', 'MID', xmid or 'XMID' extension !" << txf::nl;
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
	cio::CErr.reset();
	cio::COut.reset();
ERRFEpilog
	return ERRExitValue;
}
