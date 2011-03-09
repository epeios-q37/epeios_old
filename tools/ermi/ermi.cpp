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

#include "ermi.h"

#include "err.h"
#include "cio.h"
#include "clnarg.h"
#include "dir.h"
#include "mscmdd.h"

#include "common.h"
#include "identity.h"
#include "get.h"

using namespace common;


/*$RAW$*/

/* Beginning of the part which handles command line arguments. */

enum command__ {
	cHelp,
	cVersion,
	cLicense,
	cDevices,	// Diplay available MIDI devices.
	cIdentify,	// Retrieve identity from a device.
	cRetrieve,	// Retrieve settings from device.
	c_amount,
	c_Undefined
};

enum option {
	oDIn,	// Device in.
	oDOut,	// Device out.
	// o
};

#define PARAM( name )	CLNARG_STRING_PARAM___( name )

struct parameters {
	command__ Command;
	PARAM( DIn );		// MIDI device in.
	PARAM( DOut );		// MIDI device out.
	PARAM( Output );	// Output file.
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
	cout << txf::tab << "displays available MIDI devices id and name." << txf::nl;
	cout << NAME << ' ' << Description.GetCommandLabels( cIdentify, "," ) << txf::nl;
	cout << txf::tab << "displays identity of a MIDI device." << txf::nl;
	cout << NAME << ' ' << Description.GetCommandLabels( cRetrieve, "," ) << txf::nl;
	cout << txf::tab << "retrieve settings from device." << txf::nl;

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

	if ( ( Unknow = Analyzer.GetOptions( Options ) ) != NULL )
		clnarg::ReportUnexpectedOption( Unknow, LocaleRack );

	P = Options.First();

	while( P != NONE ) {
		Argument.Init();

		switch( Option = Options( P ) ) {
		case oDIn:
			Analyzer.GetArgument( Option, Argument );
			if ( Argument.Amount() == 0 )
				clnarg::ReportMissingOptionArgument( Analyzer.Description().GetOptionLabels( oDIn ), LocaleRack );
			Argument.Convert( Parameters.DIn );
			break;
		case oDOut:
			Analyzer.GetArgument( Option, Argument );
			if ( Argument.Amount() == 0 )
				clnarg::ReportMissingOptionArgument( Analyzer.Description().GetOptionLabels( oDOut ), LocaleRack );
			Argument.Convert( Parameters.DOut );
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
*/
	case cDevices:
		switch( Free.Amount() ) {
		case 0:
			break;
		default:
			clnarg::ReportWrongNumberOfArguments( LocaleRack );
			break;
		}
		break;
	case cIdentify:
		switch( Free.Amount() ) {
		case 0:
			break;
		default:
			clnarg::ReportWrongNumberOfArguments( LocaleRack );
			break;
		}
		break;
	case cRetrieve:
		switch( Free.Amount() ) {
		case 1:
			Free( P ).Convert( Parameters.Output );
			break;
		case 0:
			break;
		default:
			clnarg::ReportWrongNumberOfArguments( LocaleRack );
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

	Description.AddCommand( CLNARG_NO_SHORT, "version", cVersion );
	Description.AddCommand( CLNARG_NO_SHORT, "help", cHelp );
	Description.AddCommand( CLNARG_NO_SHORT, "license", cLicense );
	Description.AddCommand( CLNARG_NO_SHORT, "devices", cDevices );
	Description.AddCommand( CLNARG_NO_SHORT, "identify", cIdentify );
//	Description.AddCommand( '', "", c );

	Description.AddOption( CLNARG_NO_SHORT, "din", oDIn );
	Description.AddOption( CLNARG_NO_SHORT, "dout", oDOut );
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
	case cIdentify:
	case cRetrieve:
		Parameters.Command = (command__)Analyzer.GetCommand();
		break;
//	case c:
	case CLNARG_NONE:
		clnarg::ReportMissingCommand( LocaleRack );
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

	if ( Descriptions.Amount() == 0 )
		Display( mNoMIDIOutDevicesAvailable );
	else
		Display( mAvailableMIDIOutDevices );


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

	if ( Descriptions.Amount() == 0 )
		Display( mNoMIDIInDevicesAvailable );
	else
		Display( mAvailableMIDIInDevices );



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
	case cIdentify:
		identity::Identify( NULL, Parameters.DIn, Parameters.DOut );
		break;
	case cRetrieve:
		get::GetSettings( NULL, Parameters.DIn, Parameters.DOut, Parameters.Output );
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
	GlobalInitization();

	Main( argc, argv );
ERRFErr
ERRFEnd
	GlobalRelease();
ERRFEpilog
	return ERRExitValue;
}
