/*
	'rvk' by Claude SIMON (csimon@zeusw.org)
	Roland VK-7(7)(m) parameters management tool.
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
#include "mscmdd.h"
#include "flf.h"
#include "mscvkp.h"
#include "mscvkt.h"
#include "mscvkx.h"
#include "mscmdf.h"
#include "xml.h"
#include "ltf.h"
#include "fnm.h"

#define NAME			"rvk"
#define VERSION			"0.1.2"
#define COPYRIGHT_YEARS	"2007"
#define DESCRIPTION		"Roland VK(77) parameters management tool"
#define INFO			EPSMSC_EPEIOS_TEXT
#define AUTHOR_NAME		EPSMSC_AUTHOR_NAME
#define AUTHOR_EMAIL	EPSMSC_AUTHOR_EMAIL
#define HELP			EPSMSC_HELP_INVITATION( NAME )
#define COPYRIGHT		"Copyright (c) " COPYRIGHT_YEARS " " AUTHOR_NAME " (" AUTHOR_EMAIL ")."
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
	// Affiche la liste de 'MIDI devices'.
	cDevices,
	// Convertit un fichier midi en fichier XML.
	cConvert,
	// Envoie les données vers l'orgue.
	cSend,
	// Génère un fichier MIDI avec les données.
	cMIDI,
	// Génère un flux dont tous les 'xvk:...' ont été traité.
	cNormalize,
	c_amount,
	c_Undefined
};

enum option {
	// o
};

struct parameters {
	command__ Command;
	tol::E_FPOINTER___( char ) MidiFile, XVKFile, TargetFile;
	bso::sbyte__ MidiOutDeviceId;
	parameters( void )
	{
		Command = c_Undefined;
		MidiOutDeviceId = 0;
	}
};

void PrintUsage( const clnarg::description_ &Description )
{
	cout << DESCRIPTION << txf::nl;
	cout << NAME << " --version|--license|--help" << txf::nl;
	clnarg::PrintCommandUsage( Description, cVersion, "print version of " NAME " components.", clnarg::vSplit, false );
	clnarg::PrintCommandUsage( Description, cLicense, "print the license.", clnarg::vSplit, false );
	clnarg::PrintCommandUsage( Description, cHelp, "print this message.", clnarg::vOneLine, false );

	cout << NAME << " [" << Description.GetCommandLabels( cSend, "," ) << "] <XVK-File> <device-id>" << txf::nl;
	cout << txf::tab << "send parameters to the organ through given MIDI device." << txf::nl;

	cout << NAME << ' ' << Description.GetCommandLabels( cMIDI, "," ) << " <XVK-File> <MIDI-file>" << txf::nl;
	cout << txf::tab << "write parameters to MIDI file." << txf::nl;

	cout << NAME << ' ' << Description.GetCommandLabels( cDevices, "," ) << txf::nl;
	cout << txf::tab << "displays MIDI out devices id and name." << txf::nl;

	cout << NAME << ' ' << Description.GetCommandLabels( cConvert, "," ) << " <MIDI-file> <XVK-file>" << txf::nl;
	cout << txf::tab << "converts MIDI file in XVK file." << txf::nl;

	cout << NAME << ' ' << Description.GetCommandLabels( cNormalize, "," ) << " <XVK-file> <Normalized-XVK-file>" << txf::nl;
	cout << txf::tab << "generates a file where all 'xvk:...' tags are handled." << txf::nl;

	cout << txf::nl;

	cout << txf::tab << "device-id : id of the MIDI out device (see '" << Description.GetCommandLabels( cDevices, "," ) << "' command)." << txf::nl;
	cout << txf::tab << "XVK-file : XML file containing the parameters of the VK organ." << txf::nl;
	cout << txf::tab << "MIDI-file : MIDI file containing the SysEx from of for the VK organ." << txf::nl;
	cout << txf::tab << "normalized-XVK-file : like <XVK-file>, but without 'xvk:...' tags." << txf::nl;

	// Free argument description.
//	clnarg::PrintCommandUsage( Description, c, "", false, true );
//	cout << "options:" << txf::nl;
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
	case cNormalize:
	case cMIDI:
		switch ( Free.Amount() ) {
		case 2:
			Parameters.TargetFile = Free( P ).Convert();

			P = Free.Previous( P );
			Parameters.XVKFile = Free( P ).Convert();
			break;
		default:
			cerr << "Bad amount of arguments." << txf::nl;
			cout << HELP << txf::nl;
			ERRExit( EXIT_FAILURE );
			break;
			}
		break;
	case cSend:
		switch ( Free.Amount() ) {
		case 2:
		{
			epeios::row__ Error = NONE;

			Parameters.MidiOutDeviceId = Free( P ).ToSB( &Error );

			if ( Error != NONE ) {
				cerr << "Bad value for MIDI device id ('" << Free( P ) << "') !" << txf::nl;
				ERRExit( EXIT_FAILURE );
			}
			break;

			P = Free.Previous( P );
			Parameters.XVKFile = Free( P ).Convert();
		}
		case 1:
		default:
			cerr << "Bad amount of arguments." << txf::nl;
			cout << HELP << txf::nl;
			ERRExit( EXIT_FAILURE );
			break;
		}
		break;
	case cConvert:
		switch ( Free.Amount() ) {
		case 2:
			Parameters.XVKFile = Free( P ).Convert();
			P = Free.Previous( P );
			Parameters.MidiFile = Free( P ).Convert();
			break;
		case 1:
		default:
			cerr << "Bad amount of arguments." << txf::nl;
			cout << HELP << txf::nl;
			ERRExit( EXIT_FAILURE );
			break;
		}
		break;
	case cDevices:
		switch( Free.Amount() ) {
		case 0:
			break;
		default:
			cerr << "Too many arguments." << txf::nl;
			cout << HELP << txf::nl;
			ERRi();
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
	Description.AddCommand( 's', "send", cSend );
	Description.AddCommand( CLNARG_NO_SHORT, "midi", cMIDI );
	Description.AddCommand( CLNARG_NO_SHORT, "devices", cDevices );
	Description.AddCommand( CLNARG_NO_SHORT, "convert", cConvert );
	Description.AddCommand( CLNARG_NO_SHORT, "normalize", cNormalize );
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
	case cDevices:
	case cConvert:
	case cSend:
	case cMIDI:
	case cNormalize:
		Parameters.Command = (command__)Analyzer.GetCommand();
		break;
//	case c:
	case CLNARG_NONE:
		Parameters.Command = cSend;
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

void DisplayMidiOutDevices( void )
{
ERRProlog
	mscmdd::descriptions Descriptions;	
	epeios::row__ Row = NONE;
ERRBegin
	Descriptions.Init();

	mscmdd::GetMidiOutDeviceDescriptions( Descriptions );

	if ( Descriptions.Amount() == 0 ) {
		cout << "No MIDI out devices availables :" << txf::nl;
		ERRReturn;
	} else
		cout << "MIDI out devices available : " << txf::nl;


	Row = Descriptions.First();

	while ( Row != NONE ) {
		cout << *Row << " : " << Descriptions( Row ) << txf::nl;

		Row = Descriptions.Next( Row );
	}

	cout << txf::sync;
ERRErr
ERREnd
ERREpilog
}

void GetDataSets(
	const char *MidiFile,
	mscvkp::data_sets &DataSets )
{
ERRProlog
	flf::file_iflow___ Flow;
ERRBegin
	if ( Flow.Init( MidiFile, fil::mReadOnly, err::hSkip ) != fil::sSuccess ) {
		cerr << "Unable to open input file '" << MidiFile << "' !" << txf::nl;
		ERRExit( EXIT_FAILURE );
	}

	mscvkp::Fill( Flow, mscmdm::oFile, DataSets );
ERRErr
ERREnd
ERREpilog
}

void Write(
	const mscvkp::data_sets_ &DataSets,
	const char *TargetFile )
{
ERRProlog
	flf::file_oflow___ Flow;
	txf::text_oflow__ TFlow( Flow );
	bso::bool__ BackupCreated = false;
ERRBegin
	fil::CreateBackupFile( TargetFile, fil::hbfRename );

	BackupCreated = true;

	if ( Flow.Init( TargetFile, fil::mRemove, err::hSkip ) != fil::sSuccess ) {
		cerr << "Unable to open target file '" << TargetFile << "' !" << txf::nl;
		ERRExit( EXIT_FAILURE );
	}

	xml::WriteXMLHeader( TFlow );
	TFlow << txf::nl;

	mscvkx::Write( DataSets, TFlow );
ERRErr
	if ( BackupCreated )
		fil::RecoverBackupFile( TargetFile );
ERREnd
ERREpilog
}

void Convert(
	const char *MidiFile,
	const char *TargetFile )
{
ERRProlog
	mscvkp::data_sets DataSets;
ERRBegin
	DataSets.Init();

	GetDataSets( MidiFile, DataSets );

	Write( DataSets, TargetFile );	
ERRErr
ERREnd
ERREpilog
}

static void Handle_(
	xml::extended_status__ Status,
	xtf::extended_text_iflow__ &XFlow,
	const str::string_ &GuiltyFileName,
	txf::text_oflow__ &OFlow )
{
	if ( Status != xml::xsOK ) {
		OFlow << "Error at line " << XFlow.Line() << " position " << XFlow.Column() << " ";

		if ( GuiltyFileName.Amount() != 0 )
			OFlow << "in file '" << GuiltyFileName << "' ";

		OFlow << ": " << xml::GetLabel( Status ) << " ! " << txf::nl << txf::sync;
		ERRExit( EXIT_FAILURE );
	}
}

void Read(
	const char*FileName,
	mscvkp::data_sets_ &DataSets )
{
ERRProlog
	flf::file_iflow___ Flow;
	xtf::extended_text_iflow__ XFlow;
	xml::extended_status__ Status = xml::xs_Undefined;
	str::string GuiltyFileName;
	FNM_BUFFER___ LocationBuffer;
	const char *Location = NULL;
ERRBegin
	if ( Flow.Init( FileName, fil::mReadOnly, err::hSkip ) != fil::sSuccess ) {
		cerr << "Unable to open input file '" << FileName << "' !" << txf::nl;
		ERRExit( EXIT_FAILURE );
	}

	Location = fnm::GetLocation( FileName, LocationBuffer );	

	XFlow.Init( Flow );

	GuiltyFileName.Init();

	Status = mscvkx::Parse( XFlow, str::string( Location ), DataSets, GuiltyFileName );

	Handle_( Status, XFlow, GuiltyFileName, cerr );

ERRErr
ERREnd
ERREpilog
}

void Send(
	const mscvkp::data_sets_ &DataSets,
	bso::sbyte__ MidiOutDeviceId )
{
ERRProlog
	mscmdd::unsafe_midi_oflow___ Flow;
ERRBegin
	if ( !Flow.Init( MidiOutDeviceId, err::hSkip ) ) {
		cerr << "Unable to open MIDI out device of id " << (bso::slong__)MidiOutDeviceId << " !" << txf::nl;
		ERRExit( EXIT_FAILURE );
	}

	mscvkp::Send( DataSets, Flow );
ERRErr
ERREnd
ERREpilog
}


void Send(
	const char *FileName,
	int MidiOutDeviceId )
{
ERRProlog
	mscvkp::data_sets DataSets;
	ltf::line_text_oflow___<> Flow( cout );
ERRBegin
	DataSets.Init();

	Flow.Init();

	Flow  << "Parsing parameters file ..." << txf::sync;

	Flow.CR();

	Read( FileName, DataSets );

	Flow.Clear();

	Flow  << "Sending data to organ ..." << txf::sync;

	Flow.CR();

	Send( DataSets, MidiOutDeviceId );

	Flow.Clear();

	Flow << "Data sent !" << txf::nl;
ERRErr
ERREnd
ERREpilog
}



/**/


void WriteMIDI(
	const mscvkp::data_sets_ &DataSets,
	const char *TargetMIDIFile )
{
ERRProlog
	flf::file_oflow___ OFlow;
	mscmdm::track Track;
	mscmdm::tracks Tracks;
	bso::bool__ BackupCreated = false;
ERRBegin
	fil::CreateBackupFile( TargetMIDIFile, fil::hbfRename );

	BackupCreated = true;

	if ( !OFlow.Init( TargetMIDIFile, err::hSkip ) ) {
		cerr << "Unable to open '" << TargetMIDIFile << "' for writing !" << txf::nl;
		ERRExit( EXIT_FAILURE );
	}

	Track.Init();

	mscvkp::BuildTrack( DataSets, Track );

	mscmdf::PutHeaderChunk( 0, 1, 120, OFlow );

	mscmdm::PutTrack( Track, OFlow );

ERRErr
	if ( BackupCreated )
		fil::RecoverBackupFile( TargetMIDIFile );
ERREnd
ERREpilog
}


void WriteMIDI(
	const char *FileName,
	const char *TargetMIDIFile )
{
ERRProlog
	mscvkp::data_sets DataSets;
	ltf::line_text_oflow___<> Flow( cout );
ERRBegin
	DataSets.Init();

	Flow.Init();

	Flow  << "Parsing parameters file ..." << txf::sync;

	Flow.CR();

	Read( FileName, DataSets );

	Flow.Clear();

	Flow  << "Sending data to organ ..." << txf::sync;

	Flow.CR();

	WriteMIDI( DataSets, TargetMIDIFile );

	Flow.Clear();

	Flow << "Data sent !" << txf::nl;
ERRErr
ERREnd
ERREpilog
}

/**/

#if 1
void Normalize(
	const char *SourceFileName,
	const char *TargetFileName )
{
ERRProlog
	mscvkp::data_sets DataSets;
ERRBegin
	DataSets.Init();

	Read( SourceFileName, DataSets );

	Write( DataSets, TargetFileName );	
ERRErr
ERREnd
ERREpilog
}
#else	// Original
void Normalize(
	const char *SourceFileName,
	const char *TargetFileName )
{
ERRProlog
	flf::file_iflow___ IFlow;
	xtf::extended_text_iflow__ XFlow;
	flf::file_oflow___ OFlow;
	txf::text_oflow__ TFlow( OFlow );
	bso::bool__ BackupCreated = false;
	str::string GuiltyFileName;
	xml::extended_status__ Status = xml::xs_Undefined;
	tol::E_FPOINTER___( char ) Location;
ERRBegin
	if ( IFlow.Init( SourceFileName, fil::mReadOnly, err::hSkip ) != fil::sSuccess ) {
		cerr << "Unable to open input file '" << SourceFileName << "' !" << txf::nl;
		ERRExit( EXIT_FAILURE );
	}

	XFlow.Init( IFlow );

	fil::CreateBackupFile( TargetFileName, fil::hbfRename );

	BackupCreated = true;

	if ( OFlow.Init( TargetFileName, fil::mRemove, err::hSkip ) != fil::sSuccess ) {
		cerr << "Unable to open target file '" << TargetFileName << "' !" << txf::nl;
		ERRExit( EXIT_FAILURE );
	}

	Location = fnm::GetLocation( SourceFileName );	

	GuiltyFileName.Init();

	Status = xml::Normalize( XFlow, str::string( XML_EXTENDED_PARSER_DEFAULT_NAMESPACE ), true, str::string( Location ), TFlow, GuiltyFileName );

	Handle_( Status, XFlow, GuiltyFileName, cerr );

ERRErr
	if ( BackupCreated )  {
		OFlow.reset();
		fil::RecoverBackupFile( TargetFileName );
	}
ERREnd
ERREpilog
}
#endif

void Go( const parameters &Parameters )
{
ERRProlog
ERRBegin
	switch ( Parameters.Command ) {
	case cSend:
		Send( Parameters.XVKFile, Parameters.MidiOutDeviceId );
		break;
	case cDevices:
		DisplayMidiOutDevices();
		break;
	case cConvert:
		Convert( Parameters.MidiFile, Parameters.XVKFile );
		break;
	case cNormalize:
		Normalize( Parameters.XVKFile, Parameters.TargetFile );
		break;
	case cMIDI:
		WriteMIDI( Parameters.XVKFile, Parameters.TargetFile );
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
