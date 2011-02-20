/*
	'ermi' by Claude SIMON
	Handles Roland devices parameters.
	Copyright (C) 2011 Claude SIMON

	This file is part of the Epeios (http://zeusw.org/epeios/) project.

	This library is free software; you can redistribute it and/or
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
           59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.*/

// $Id$

#include "err.h"
#include "cio.h"
#include "epsmsc.h"
#include "clnarg.h"
#include "mscmdd.h"

#define NAME			"ermi"
#define VERSION			"0.1.0"
#define COPYRIGHT_YEARS	"2011"
#define DESCRIPTION		"(description)"
#define AFFILIATION		EPSMSC_EPEIOS_AFFILIATION
#define AUTHOR_NAME		EPSMSC_AUTHOR_NAME
#define AUTHOR_CONTACT	EPSMSC_AUTHOR_CONTACT
#define HELP			EPSMSC_HELP_INVITATION( NAME )
#define COPYRIGHT		EPSMSC_COPYRIGHT( COPYRIGHT_YEARS )
#define CVS_DETAILS		("$Id$\b " + 5)

/*$RAW$*/

cio::cin___ cin;
cio::cout___ cout;
cio::cerr___ cerr;

/* Beginning of the part which handles command line arguments. */

enum command__ {
	cHelp,
	cVersion,
	cLicense,
	cDevices,
	c_amount,
	c_Undefined
};

enum option {
	// o
};

struct parameters {
	command__ Command;
	parameters( void )
	{
		Command = c_Undefined;
	}
};

void PrintUsage( const clnarg::description_ &Description )
{
	cout << DESCRIPTION << txf::nl;
	cout << NAME << " --version|--license|--help" << txf::nl;
	clnarg::PrintCommandUsage( Description, cVersion, "print version of " NAME " components.", clnarg::vSplit, false, cout );
	clnarg::PrintCommandUsage( Description, cLicense, "print the license.", clnarg::vSplit, false, cout );
	clnarg::PrintCommandUsage( Description, cHelp, "print this message.", clnarg::vOneLine, false, cout );

	cout << NAME << ' ' << Description.GetCommandLabels( cDevices, "," ) << txf::nl;
	cout << txf::tab << "displays MIDI out devices id and name." << txf::nl;

	cout << NAME << " <command> [options] ..." << txf::nl;
	// Free argument description.
	cout << "command:" << txf::nl;
//	clnarg::PrintCommandUsage( Description, c, "", false, true );
	cout << "options:" << txf::nl;
//	clnarg::PrintOptionUsage( Description, o, "", clnarg::vSplit );
}

void PrintHeader( void )
{
	cout << NAME " V" VERSION " "__DATE__ " " __TIME__;
	cout << " by "AUTHOR_NAME " (" AUTHOR_CONTACT ")" << txf::nl;
	cout << COPYRIGHT << txf::nl;
	cout << AFFILIATION << txf::nl;
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
/*	case cNormalize:
	case cMIDI:
		switch ( Free.Amount() ) {
		case 2:
			Free( P ).Convert( Parameters.TargetFile );

			P = Free.Previous( P );
			Free( P ).Convert( Parameters.XVKFile );
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
			Free( P ).Convert( Parameters.XVKFile );
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
			Free( P ).Convert( Parameters.XVKFile );
			P = Free.Previous( P );
			Free( P ).Convert( Parameters.MidiFile );
			break;
		case 1:
		default:
			cerr << "Bad amount of arguments." << txf::nl;
			cout << HELP << txf::nl;
			ERRExit( EXIT_FAILURE );
			break;
		}
		break;
*/	case cDevices:
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
	Description.AddCommand( CLNARG_NO_SHORT, "version", cVersion );
	Description.AddCommand( CLNARG_NO_SHORT, "help", cHelp );
	Description.AddCommand( CLNARG_NO_SHORT, "license", cLicense );
	Description.AddCommand( CLNARG_NO_SHORT, "devices", cDevices );
//	Description.AddOption( '', "", o );

	Analyzer.Init( argc, argv, Description );

	switch ( Analyzer.GetCommand() ) {
	case cVersion:
		PrintHeader();
		TTR.Advertise( cout );
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
	case cDevices:
		Parameters.Command = (command__)Analyzer.GetCommand();
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

	cout << txf::commit;
ERRErr
ERREnd
ERREpilog
}

void DisplayMidiInDevices( void )
{
ERRProlog
	mscmdd::descriptions Descriptions;	
	epeios::row__ Row = NONE;
ERRBegin
	Descriptions.Init();

	mscmdd::GetMidiInDeviceDescriptions( Descriptions );

	if ( Descriptions.Amount() == 0 ) {
		cout << "No MIDI in devices availables :" << txf::nl;
		ERRReturn;
	} else
		cout << "MIDI in devices available : " << txf::nl;


	Row = Descriptions.First();

	while ( Row != NONE ) {
		cout << *Row << " : " << Descriptions( Row ) << txf::nl;

		Row = Descriptions.Next( Row );
	}

	cout << txf::commit;
ERRErr
ERREnd
ERREpilog
}

void DisplayMidiDevices( void )
{
	DisplayMidiInDevices();
	DisplayMidiOutDevices();
}

void Go( const parameters &Parameters )
{
ERRProlog
ERRBegin
ERRProlog
ERRBegin
	switch ( Parameters.Command ) {
	case cDevices:
		DisplayMidiDevices();
		break;
	default:
		ERRc();
		break;
	}
ERRErr
ERREnd
ERREpilog
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
	cin.Init();
	cout.Init();
	cerr.Init();
	Main( argc, argv );
ERRFErr
ERRFEnd
	cout.Commit();
	cerr.Commit();
ERRFEpilog
	return ERRExitValue;
}
