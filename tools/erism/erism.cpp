/*
	'erism' by Claude SIMON
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

#include "erism.h"

#include "err.h"
#include "cio.h"
#include "clnarg.h"
#include "dir.h"
#include "mscmdd.h"

#include "common.h"
#include "identity.h"
#include "get.h"
#include "set.h"

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
	cWrite,		// Set settings to device.
	cRandom,	// Set a range of settings with random value (between 2 values).
	cDetail,	// Print the MIDI implemntations in details.
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
	PARAM( OutputFileName );	// Output file name.
	PARAM( SettingsFileName );	// Settings file name.
	PARAM( AddressRange );	// Adress range for the '--random' command.
	PARAM( ValueRange );	// Value range for the '--random' command.
	parameters( void )
	{
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
	cout << common::LocaleRack.GetTranslation( "ProgramDescription", "", TranslationBuffer ) << '.'  << txf::nl;
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

	cout << NAME << ' ' << Description.GetCommandLabels( cDevices, Buffer, "," );
	cout << txf::nl;
	Translation.Init();
	cout << txf::pad << common::GetTranslation( common::mDevicesCommandDescription, Translation ) << '.' << txf::nl;


	cout << NAME << ' ' << Description.GetCommandLabels( cIdentify, Buffer, "," );
	cout << " " << Description.GetOptionLabels( oDIn, Buffer ) << " <din>";
	cout << " " << Description.GetOptionLabels( oDOut, Buffer ) << " <dout>";
	cout << txf::nl;
	Translation.Init();
	cout << txf::pad << common::GetTranslation( common::mIdentifyCommandDescription, Translation ) << '.' << txf::nl;

	cout << NAME << ' ' << Description.GetCommandLabels( cRetrieve, Buffer, "," );
	cout << " " << Description.GetOptionLabels( oDIn, Buffer ) << " <din>";
	cout << " " << Description.GetOptionLabels( oDOut, Buffer ) << " <dout>";
	cout << " [<target>]";
	cout << txf::nl;
	Translation.Init();
	cout << txf::pad << common::GetTranslation( common::mRetrieveCommandDescription, Translation ) << '.' << txf::nl;

	cout << NAME << ' ' << Description.GetCommandLabels( cWrite, Buffer, "," );
	cout << " " << Description.GetOptionLabels( oDIn, Buffer ) << " <din>";
	cout << " " << Description.GetOptionLabels( oDOut, Buffer ) << " <dout>";
	cout << " <settings>";
	cout << txf::nl;
	Translation.Init();
	cout << txf::pad << common::GetTranslation( common::mWriteCommandDescription, Translation ) << '.' << txf::nl;

	cout << NAME << ' ' << Description.GetCommandLabels( cRandom, Buffer, "," );
	cout << " " << Description.GetOptionLabels( oDIn, Buffer ) << " <din>";
	cout << " " << Description.GetOptionLabels( oDOut, Buffer ) << " <dout>";
	cout << " <address-range> <value-range>";
	cout << txf::nl;
	Translation.Init();
	cout << txf::pad << common::GetTranslation( common::mRandomCommandDescription, Translation ) << '.' << txf::nl;

	cout << NAME << ' ' << Description.GetCommandLabels( cDetail, Buffer, "," );
	cout << txf::nl;
	Translation.Init();
	cout << txf::pad << common::GetTranslation( common::mDetailCommandDescription, Translation ) << '.' << txf::nl;

	cout << txf::nl;

	Translation.Init();
	cout << clnarg::GetOptionsWordingTranslation( LocaleRack, Translation );
	cout << " :" << txf::nl;

	cout << txf::pad << "- <din> :" << txf::nl;
	cout << txf::tab;
	common::Display( mDInOptionDescription, Description.GetCommandLabels( cDevices, Buffer ) );
	cout << '.' << txf::nl;

	cout << txf::pad << "- <dout> :" << txf::nl;
	cout << txf::tab;
	common::Display( mDOutOptionDescription, Description.GetCommandLabels( cDevices, Buffer ) );
	cout << '.' << txf::nl;

	cout << txf::nl;

	Translation.Init();
	cout << clnarg::GetArgumentsWordingTranslation( LocaleRack, Translation );
	cout << " :" << txf::nl;

	cout << txf::pad << "- <target> :" << txf::nl;
	cout << txf::tab;
	common::Display( mTargetArgumentDescription );
	cout << '.' << txf::nl;

	cout << txf::pad << "- <settings> :" << txf::nl;
	cout << txf::tab;
	common::Display( mSettingsArgumentDescription );
	cout << '.' << txf::nl;

	cout << txf::pad << "- <address-range> :" << txf::nl;
	cout << txf::tab;
	common::Display( mAddressRangeArgumentDescription );
	cout << '.' << txf::nl;

	cout << txf::pad << "- <value-range> :" << txf::nl;
	cout << txf::tab;
	common::Display( mValueRangeArgumentDescription );
	cout << '.' << txf::nl;

ERRErr
ERREnd
ERREpilog
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
	CLNARG_BUFFER__ Buffer;
ERRBegin
	Options.Init();

	if ( ( Unknow = Analyzer.GetOptions( Options ) ) != NULL )
		clnarg::ReportUnexpectedOptionError( Unknow, NAME, LocaleRack );

	P = Options.First();

	while( P != NONE ) {
		Argument.Init();

		switch( Option = Options( P ) ) {
		case oDIn:
			Analyzer.GetArgument( Option, Argument );
			if ( Argument.Amount() == 0 )
				clnarg::ReportMissingOptionArgumentError( Analyzer.Description().GetOptionLabels( oDIn, Buffer ), NAME, LocaleRack );
			Argument.Convert( Parameters.DIn );
			break;
		case oDOut:
			Analyzer.GetArgument( Option, Argument );
			if ( Argument.Amount() == 0 )
				clnarg::ReportMissingOptionArgumentError( Analyzer.Description().GetOptionLabels( oDOut, Buffer ), NAME, LocaleRack );
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
			clnarg::ReportWrongNumberOfArgumentsError( NAME, LocaleRack );
			break;
		}
		break;
	case cIdentify:
		switch( Free.Amount() ) {
		case 0:
			break;
		default:
			clnarg::ReportWrongNumberOfArgumentsError( NAME, LocaleRack );
			break;
		}
		break;
	case cRetrieve:
		switch( Free.Amount() ) {
		case 1:
			Free( P ).Convert( Parameters.OutputFileName );
			break;
		case 0:
			break;
		default:
			clnarg::ReportWrongNumberOfArgumentsError( NAME, LocaleRack );
			break;
		}
		break;
	case cWrite:
		switch( Free.Amount() ) {
		case 1:
			Free( P ).Convert( Parameters.SettingsFileName );
			break;
		default:
			clnarg::ReportWrongNumberOfArgumentsError( NAME, LocaleRack );
			break;
		}
		break;
	case cRandom:
		switch( Free.Amount() ) {
		case 2:
			Free( P ).Convert( Parameters.ValueRange );
			P = Free.Previous( P );
			Free( P ).Convert( Parameters.AddressRange );
			break;
		default:
			clnarg::ReportWrongNumberOfArgumentsError( NAME, LocaleRack );
			break;
		}
	case cDetail:
		switch( Free.Amount() ) {
		case 0:
			break;
		default:
			clnarg::ReportWrongNumberOfArgumentsError( NAME, LocaleRack );
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
	Description.AddCommand( CLNARG_NO_SHORT, "retrieve", cRetrieve );
	Description.AddCommand( CLNARG_NO_SHORT, "write", cWrite );
	Description.AddCommand( CLNARG_NO_SHORT, "random", cRandom );
	Description.AddCommand( CLNARG_NO_SHORT, "detail", cDetail );
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
	case cWrite:
	case cRandom:
	case cDetail:
		Parameters.Command = (command__)Analyzer.GetCommand();
		break;
//	case c:
	case CLNARG_NONE:
		clnarg::ReportMissingCommandError( NAME, LocaleRack );
		break;
	default:
		ERRc();
		break;
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

	cout << " : " << txf::nl;


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

	cout << " : " << txf::nl;

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

void Detail( void )
{
ERRProlog
	mscrmi::midi_implementations Implementations;
	xml::writer Writer;
ERRBegin
	Implementations.Init();

	common::GetImplementations( Implementations );

	Writer.Init( common::cout, xml::oIndent, xml::e_Default );
	mscrmi::Print( Implementations, Writer );
ERRErr
ERREnd
ERREpilog
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
		get::ReadSettings( NULL, Parameters.DIn, Parameters.DOut, Parameters.OutputFileName );
		break;
	case cWrite:
		set::WriteSettings( NULL, Parameters.DIn, Parameters.DOut, Parameters.SettingsFileName );
		break;
	case cRandom:
		set::Randomize( NULL, Parameters.DIn, Parameters.DOut, Parameters.AddressRange, Parameters.ValueRange );
		break;
	case cDetail:
		Detail();
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
	Initialize();

	Main( argc, argv );
ERRFErr
ERRFEnd
	Release();
ERRFEpilog
	return ERRExitValue;
}
