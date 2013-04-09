/*
	'getbkdapi' by Claude L. SIMON (http://zeusw.org/intl/contact.html)
	Copyright (C) 2001-2003 Claude SIMON

	This file is part of the Epeios project (http://www.zeusw.org/epeios/).

	This program is free software; you can redistribute it and/or
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

// $Id$

#include "getbkdapi.h"
#include "locale.h"
#include "scltool.h"
#include "sclerror.h"

#include "err.h"
#include "cio.h"
#include "epsmsc.h"
#include "clnarg.h"

#include "fblovl.h"
#include "fblfaq.h"
#include "fblfub.h"
#include "xml.h"
#include "sck.h"
#include "csducl.h"
#include "flf.h"

using namespace fblfaq;

using cio::CErr;
using cio::COut;
using cio::CIn;

using xml::writer_;

/* Beginning of the part which handles command line arguments. */

enum command {
	cHelp,
	cVersion,
	cLicense,
	cLibrary,
	cDaemon,
	c_amount,
	c_Undefined
};

enum option {
};

#define STRING_PARAM___( name )	CLNARG_STRING_PARAM___( name )

struct parameters___ {
	STRING_PARAM___( BackendLocation );
	STRING_PARAM___( FileName );
	parameters___( void )
	{
	}
};

static void PrintSpecialsCommandsDescriptions_(
	const clnarg::description_ &Description,
	const lcl::locale_ &Locale,
	const char *Language )
{
ERRProlog
	CLNARG_BUFFER__ Buffer;
	lcl::meaning Meaning;
	str::string Translation;
ERRBegin
	Translation.Init();
	COut << Locale.GetTranslation( "ProgramDescription", Language, Translation ) << '.'  << txf::nl;
	COut << txf::nl;

	COut << NAME " " << Description.GetCommandLabels( cVersion, Buffer ) << txf::nl;
	Meaning.Init();
	clnarg::GetVersionCommandDescription( Meaning );
	Translation.Init();
	Locale.GetTranslation( Meaning, Language, Translation );
	COut << txf::pad << Translation << '.' << txf::nl;

	COut << NAME " " << Description.GetCommandLabels( cLicense, Buffer ) << txf::nl;
	Meaning.Init();
	clnarg::GetLicenseCommandDescription( Meaning );
	Translation.Init();
	Locale.GetTranslation( Meaning, Language, Translation );
	COut << txf::pad << Translation << '.' << txf::nl;

	COut << NAME " " << Description.GetCommandLabels( cHelp, Buffer ) << txf::nl;
	Meaning.Init();
	clnarg::GetHelpCommandDescription( Meaning );
	Translation.Init();
	Locale.GetTranslation( Meaning, Language, Translation );
	COut << txf::pad << Translation << '.' << txf::nl;

ERRErr
ERREnd
ERREpilog
}

void PrintUsage( const clnarg::description_ &Description )
{
ERRProlog
	STR_BUFFER___ TBuffer;
	CLNARG_BUFFER__ Buffer;
	lcl::meaning Meaning;
	str::string Translation;
ERRBegin
	PrintSpecialsCommandsDescriptions_( Description, scllocale::GetLocale(), scltool::GetLanguage() );

// Commands.
	COut << NAME " " << Description.GetCommandLabels( cDaemon, Buffer ) << " <daemon backend> [file]" << txf::nl;;
	Translation.Init();
	COut << txf::pad << scllocale::GetTranslation( locale::Label( locale::mDaemonCommandDescription ), scltool::GetLanguage(), Translation ) << '.' << txf::nl;

	COut << NAME " "  << Description.GetCommandLabels( cLibrary, Buffer ) << " <library backend> [file]" << txf::nl;;
	Translation.Init();
	COut << txf::pad << scllocale::GetTranslation( locale::Label( locale::mLibraryCommandDescription ), scltool::GetLanguage(), Translation ) << '.' << txf::nl;

	COut << txf::nl;

// Arguments.
	Meaning.Init();
	clnarg::GetArgumentsWordingMeaning( Meaning );
	Translation.Init();
	COut << scllocale::GetTranslation( Meaning, scltool::GetLanguage(), Translation ) << " :" << txf::nl;

	COut << txf::pad << "deamon backend :" << txf::nl;
	COut << txf::tab;
	Translation.Init();
	COut << scllocale::GetTranslation( locale::Label( locale::mDaemonBackendArgumentDescription ), scltool::GetLanguage(), Translation ) << '.' << txf::nl;

	COut << txf::pad << "library backend :" << txf::nl;
	COut << txf::tab;
	Translation.Init();
	COut << scllocale::GetTranslation( locale::Label( locale::mLibraryBackendArgumentDescription ), scltool::GetLanguage(), Translation ) << '.' << txf::nl;

	COut << txf::pad << "file :" << txf::nl;
	COut << txf::tab;
	Translation.Init();
	COut << scllocale::GetTranslation( locale::Label( locale::mFileArgumentDescription ), scltool::GetLanguage(), Translation ) << '.' << txf::nl;
ERRErr
ERREpilog
ERREnd
}

static void PrintHeader_( void )
{
	COut << NAME " V" VERSION << " (" APP_URL ")" << txf::nl;
	COut << COPYRIGHT << txf::nl;
	COut << txf::pad << "Build : "__DATE__ " " __TIME__ << " (" << cpe::GetDescription() << ')' << txf::nl;
}

static void AnalyzeOptions(
	clnarg::analyzer___ &Analyzer,
	parameters___ &Parameters )
{
ERRProlog
	sdr::row__ P;
	clnarg::option_list Options;
	clnarg::id__ Option;
	const bso::char__ *Unknown = NULL;
	clnarg::argument Argument;
	clnarg::buffer__ Buffer;
ERRBegin
	Options.Init();

	if ( ( Unknown = Analyzer.GetOptions( Options ) ) != NULL )
		clnarg::ReportUnknownOptionError( Unknown, NAME, scllocale::GetLocale(), scltool::GetLanguage() );

	P = Options.First();

	while( P != NONE ) {
		Argument.Init();

		switch( Option = Options( P ) ) {
		default:
			ERRFwk();
		}

		P = Options.Next( P );
	}

ERRErr
ERREnd
ERREpilog
}

static void AnalyzeFreeArguments(
	clnarg::analyzer___ &Analyzer,
	parameters___ &Parameters )
{
ERRProlog
	clnarg::arguments Free;
	sdr::row__ P;
ERRBegin
	Free.Init();

	Analyzer.GetArguments( Free );

	P = Free.Last();

	switch( Free.Amount() ) {
	case 2:
		Free( P ).Convert( Parameters.FileName );
		P = Free.Previous( P );
	case 1:
		Free( P ).Convert( Parameters.BackendLocation );
		break;
	default:
		clnarg::ReportWrongNumberOfArgumentsError( NAME, scllocale::GetLocale(), scltool::GetLanguage() );
		break;
	}

ERRErr
ERREnd
ERREpilog
}

static void AnalyzeArgs(
	int argc,
	const char *argv[],
	parameters___ &Parameters,
	csducl::type__ &Type )
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
	Description.AddCommand( 'd', "daemon", cDaemon );
	Description.AddCommand( 'l', "library", cLibrary );
//	Description.AddOption( '', "", o );

	Analyzer.Init( argc, argv, Description );

	switch ( Analyzer.GetCommand() ) {
	case cVersion:
		PrintHeader_();
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
	case cLibrary:
		Type = csducl::tLibrary;
		break;
	case cDaemon:
		Type = csducl::tDaemon;
		break;
	case CLNARG_NONE:
		break;
	default:
		ERRFwk();
	}

	AnalyzeOptions( Analyzer, Parameters );

	AnalyzeFreeArguments( Analyzer, Parameters );

ERRErr
ERREnd
ERREpilog
}

/* End of the part which handles command line arguments. */

enum cast_type {
	ctUnknow,
	ctStatic,
	ctBunch,
	ctContainer,
	ct_amount
};

static inline const char *GetTypeName(
	fblcst::cast Cast,
	cast_type &CastType)
{
	static const char *TypeName = NULL;
	CastType = ctStatic;

	switch ( Cast ) {
	case fblcst::cEnd:
		ERRFwk();
		break;
	case fblcst::cObject:
		TypeName = "object";
		break;
	case fblcst::cBoolean:
		TypeName = "boolean";
		break;
	case fblcst::cBooleans:
		TypeName = "booleans";
		CastType = ctBunch;
		break;
	case fblcst::cSInt:
		TypeName = "sint";
		break;
	case fblcst::cSInts:
		TypeName = "sints";
		CastType = ctBunch;
		break;
	case fblcst::cUInt:
		TypeName = "uint";
		break;
	case fblcst::cUInts:
		TypeName = "uints";
		CastType = ctBunch;
		break;
	case fblcst::cId8:
		TypeName = "id8";
		break;
	case fblcst::cIds8:
		TypeName = "ids8";
		CastType = ctBunch;
		break;
	case fblcst::cXIds8:
		TypeName = "xids8";
		CastType = ctContainer;
		break;
	case fblcst::cId16:
		TypeName = "id16";
		break;
	case fblcst::cIds16:
		TypeName = "ids16";
		CastType = ctBunch;
		break;
	case fblcst::cXIds16:
		TypeName = "xids16";
		CastType = ctContainer;
		break;
	case fblcst::cId32:
		TypeName = "id32";
		break;
	case fblcst::cIds32:
		TypeName = "ids32";
		CastType = ctBunch;
		break;
	case fblcst::cXIds32:
		TypeName = "xids32";
		CastType = ctContainer;
		break;
	case fblcst::cChar:
		TypeName = "char";
		break;
	case fblcst::cString:
		TypeName = "string";
		CastType = ctBunch;
		break;
	case fblcst::cStrings:
		TypeName = "strings";
		CastType = ctContainer;
		break;
	case fblcst::cXStrings:
		TypeName = "xstrings";
		CastType = ctBunch;
		break;
	case fblcst::cByte:
		TypeName = "byte";
		break;
	case fblcst::cBinary:
		TypeName = "binary";
		CastType = ctBunch;
		break;
	case fblcst::cBinaries:
		TypeName = "binaries";
		break;
	case fblcst::cItems8:
		TypeName = "items8";
		CastType = ctContainer;
		break;
	case fblcst::cItems16:
		TypeName = "items16";
		CastType = ctContainer;
		break;
	case fblcst::cItems32:
		TypeName = "items32";
		CastType = ctContainer;
		break;
	case fblcst::cXItems8:
		TypeName = "xitems8";
		CastType = ctContainer;
		break;
	case fblcst::cXItems16:
		TypeName = "xitems16";
		CastType = ctContainer;
		break;
	case fblcst::cXItems32:
		TypeName = "xitems32";
		CastType = ctContainer;
		break;
	case fblcst::cCommandsDetails:
		TypeName = "commands_details";
		CastType = ctBunch;
		break;
	case fblcst::cObjectsReferences:
		TypeName = "objects_references";
		CastType = ctContainer;
		break;
	default:
		ERRFwk();
		break;
	}

	return TypeName;
}

void Generate(
	const fblcst::cast &Cast,
	bso::u8__ ID,
	writer_ &Writer,
	bso::bool__ IsLast )
{
ERRProlog
	char Buffer[20];
	cast_type CastType = ctUnknow;
	str::string TypeName;
ERRBegin
	sprintf( Buffer, "%u", ID );
	Writer.PushTag( "Parameter" );
	Writer.PutAttribute( "Discriminator", Buffer );

	if ( IsLast )
		Writer.PutAttribute( "Position", "Last" );

	TypeName.Init( GetTypeName( Cast, CastType ) );

	switch ( CastType ) {
	case ctStatic:
		Writer.PutAttribute( "Type", "Static" );
		break;
	case ctBunch:
		Writer.PutAttribute( "Type", "Bunch" );
		break;
	case ctContainer:
		Writer.PutAttribute( "Type", "Container" );
		break;
	default:
		ERRFwk();
		break;
	}

	Writer.PutValue( TypeName, "Name" );

	sprintf( Buffer, "%u", Cast );
	Writer.PutValue( Buffer, "ID" );
	Writer.PopTag();
ERRErr
ERREnd
ERREpilog
}

static inline bso::bool__ IsLast(
	const parameters_ &Parameters,
	sdr::row__ Current )
{
	if ( ( Current = Parameters.Next( Current ) ) == NONE )
		return true;

	if ( Parameters( Current ) != fblcst::cEnd )
		return false;

	return Parameters.Next( Current ) == NONE;
}

void Generate(
	const parameters_ &Parameters,
	writer_ &Writer )
{
	sdr::row__ P = Parameters.First();
	fblcst::cast Cast;
	bso::u8__ ID = 1;
	
	if ( ( Cast = (fblcst::cast)Parameters( P ) ) == fblcst::cEnd )
		Writer.PushTag( "Out" );
	else {
		Writer.PushTag( "In" );
		Generate( Cast, ID, Writer, IsLast( Parameters, P ) );
		ID++;
	}
		
	P = Parameters.Next( P );
		
	while( P != NONE ) {
		if ( ( Cast = (fblcst::cast)Parameters( P ) ) == fblcst::cEnd ) {
			
			if ( Parameters.Next( P ) != NONE ) {
				ID = 1;
				Writer.PopTag();
				Writer.PushTag( "Out" );
			}
		} else {
			Generate( Cast, ID, Writer, IsLast( Parameters, P ) );
			ID++;
		}
	
		P = Parameters.Next( P );
	}
	
	Writer.PopTag();
}



void Generate(
	const parameters_ &Parameters,
	writer_ &Writer,
	sdr::size__ &Position )
{
	char Buffer[20];
	
	Writer.PushTag( "Parameters" );

	sprintf( Buffer, "%lu", Parameters.Amount() );	// Don't forget ; there is the 'end of input parameters' cast.
	Writer.PutAttribute( "Amount", Buffer );
	
	sprintf( Buffer, "%lu", Position );
	Writer.PutAttribute( "Position", Buffer );

	Position += Parameters.Amount();

	if ( Parameters.Amount() > 1 )
		Generate( Parameters, Writer );
	
	Writer.PopTag();
}

const char *Convert( unsigned long V )
{
	static char Buffer[20];
	
	sprintf( Buffer, "%lu",  V );
	
	return Buffer;
}

void Generate(
	const command_ &Command,
	writer_ &Writer,
	sdr::size__ &Position )
{
	Writer.PushTag( "Command" );
	
	Writer.PutValue( Command.Identification.Value, "Name" );
	Writer.PutValue( Convert( Command.Identification.ID() ), "ID" );
	
	Generate( Command.Parameters, Writer, Position );
	
	Writer.PopTag();
}

void Generate(
	const commands_ &Commands,
	writer_ &Writer )
{
ERRProlog
	ctn::E_CITEM( command_ ) Command;
	sdr::row__ P;
	char Buffer[20];
	sdr::size__ Position = 0;
ERRBegin
	Writer.PushTag( "Commands" );
	
	sprintf( Buffer, "%lu", Commands.Amount() );
	
	Writer.PutAttribute( "Amount", Buffer );
	P = Commands.First();
	Command.Init( Commands );
	
	while( P != NONE ) {
		Generate( Command( P ), Writer, Position );
		P = Commands.Next( P );
	}
	
	Writer.PopTag();
ERRErr
ERREnd
ERREpilog
}

// 'abc_def_ghi' -> 'AbcDefGhi'.
void Convert( str::string_ &Buffer )
{
ERRProlog
	str::string Temp;
	sdr::row__ P = NONE;
ERRBegin
	Temp.Init();

	P = Buffer.First();

	if ( P != NONE )
		if ( Buffer( P ) != '_' ) {
			Temp.Append( toupper( Buffer( P ) ) );

			P = Buffer.Next( P );

			while ( P != NONE )	{
				if ( Buffer( P ) == '_' ) {
					P = Buffer.Next( P );

				if ( P != NONE )
					Temp.Append( toupper( Buffer( P ) ) );
				} else
					Temp.Append( Buffer( P ) );

				if ( P != NONE )
					P = Buffer.Next(  P );
			}
	
			Buffer = Temp;
		}
ERRErr
ERREnd
ERREpilog
}

void Generate(
	const type_ &Type,
	writer_ &Writer )
{
ERRProlog
	str::string Buffer;
ERRBegin
	Buffer.Init();

	Buffer = Type.Prefix;

	Writer.PushTag( "Prefix" );

	Writer.PutAttribute( "Default", Buffer );

	str::ToLower( Buffer );
	Writer.PutAttribute( "Lower", Buffer );

	str::ToUpper( Buffer );
	Writer.PutAttribute( "Upper", Buffer );

	Writer.PopTag();

	Buffer.Init();

	Buffer.Init();

	Buffer = Type.Name;

	Writer.PushTag( "Name" );

	Writer.PutAttribute( "Raw", Buffer );

	Writer.PutAttribute( "Underscored", Buffer );

	Convert( Buffer );

	Writer.PutAttribute( "Mixedcased", Buffer );

	Writer.PopTag();

ERRErr
ERREnd
ERREpilog
}


void Generate(
	const type_ &Type,
	bso::bool__ IsMaster,
	writer_ &Writer )
{
	Writer.PushTag( "Type" );
	if ( IsMaster )
		Writer.PutAttribute( "Object", "Master" );
	
	Writer.PutValue( Convert( Type.ID() ), "ID" );
	Generate( Type, Writer );
	
	if ( Type.Commands.Amount() != 0 )
		Generate( Type.Commands, Writer );
	
	Writer.PopTag();
}

class dummy_reporting_functions__
: public fblfrd::reporting_functions__
{
protected:
	void FBLFRDReport(
		fblovl::reply__ Reply,
		const char *Message )
	{
	}
};

void GetBackendData(
	const char *Location,
	csducl::type__ Type,
	types_ &Types,
	str::string_ &ProtocolVersion,
	str::string_ &TargetLabel,
	str::string_ &APIVersion,
	str::string_ &BackendInformations )
{
ERRProlog
	csducl::universal_client_core Core;
	csducl::universal_client_ioflow___ Flow;
	fblfub::backend_universal_access___ BackendAccess;
	fblfrd::incompatibility_informations Dummy;
	fblfub::mode__ FBLMode = fblfub::m_Undefined;
	csdleo::mode__ CSDMode = csdleo::m_Undefined;
	dummy_reporting_functions__ DummyReportingFunctions;
	csdlec::library_data__ LibraryData;
	lcl::meaning Meaning;
ERRBegin
	switch ( Type ) {
	case csducl::tDaemon:
		FBLMode = fblfub::mRemote;
		CSDMode = csdleo::mRemote;
		break;
	case csducl::tLibrary:
		FBLMode = fblfub::mEmbedded;
		CSDMode = csdleo::mEmbedded;
		break;
	default:
		ERRPrm();
		break;
	}

	LibraryData.Init( csdleo::mEmbedded, flx::VoidOFlowDriver, flx::VoidOFlowDriver, true );

	if ( !Core.Init( Location, LibraryData, *(csdsnc::log_functions__ *)NULL, Type, 0 ) ) {
		Meaning.Init();

		locale::GetUnableToAccessBackendErrorMeaning( Meaning );

		sclerror::SetMeaning( Meaning );

		ERRExit( EXIT_FAILURE );
	}


	Flow.Init( Core );

	Dummy.Init();

	BackendAccess.Init( "", fblfrd::compatibility_informations__(), Flow, FBLMode, DummyReportingFunctions, Dummy );

	GetDescription( BackendAccess, Types );
	
	BackendAccess.About( ProtocolVersion, TargetLabel, APIVersion, BackendInformations );

	BackendAccess.Disconnect();
ERRErr
	if ( ERRFailure() ) { 
		Meaning.Init();

		locale::GetUnableToCommunicateWithBackendErrorMeaning( Meaning );

		sclerror::SetMeaning( Meaning );

		ERRExit( EXIT_FAILURE );
	}
ERREnd
ERREpilog
}

void Generate(
	const types_ &Types,
	sdr::row__ MasterRow,
	writer_ &Writer )
{
ERRProlog
	sdr::row__ P;
	ctn::E_CITEM( type_ ) Type;
ERRBegin
	Writer.PushTag( "Types" );
	
	P = Types.First();
	Type.Init( Types );
	
	while( P != NONE ) {
		Generate( Type( P ), ( P == MasterRow ), Writer );
		P = Types.Next( P );
	}
	
	Writer.PopTag();

	

ERRErr
ERREnd
ERREpilog
}

#if 0
void Generate(
	const bkdacc::strings_ &RawMessages,
	writer_ &Writer )
{
ERRProlog
	epeios::row__ P;
	ctn::E_CMITEM( bkdacc::string_ ) RawMessage;
	bso::integer_buffer__ Buffer;
ERRBegin
	Writer.PushTag( "Messages" );
	Writer.PutAttribute( "Amount", bso::Convert( RawMessages.Amount(), Buffer ) );
	
	P = RawMessages.First();
	RawMessage.Init( RawMessages );
	
	while( P != NONE ) {
		Writer.PushTag( "Message" );
		Writer.PutAttribute( "Content", RawMessage( P ) );
		Writer.PopTag();

		P = RawMessages.Next( P );
	}
	
	Writer.PopTag();


ERRErr
ERREnd
ERREpilog
}

void Generate(
	const bkdacc::strings_ &RawMessages,
	const types_ &Types,
	epeios::row__ MasterRow,
	writer_ &Writer )
{
	Generate( RawMessages, Writer );
	Generate( Types, MasterRow, Writer );
}
#endif

void GenerateMisc(
	const str::string_ &BackendInformations,
	writer_ &Writer )
{
ERRProlog
	str::string Build;
	tol::buffer__ Buffer;
ERRBegin
	Writer.PushTag( "Misc" );
	Writer.PushTag( "Generator" );
	Writer.PutValue( NAME, "Name" );
	Writer.PutValue( VERSION, "Version" );

	Build.Init();
	Build.Append( __DATE__ " " );
	Build.Append( __TIME__ " (" );
	Build.Append( cpe::GetDescription() );
	Build.Append( ')' );
	Writer.PutValue( Build, "Build" );

	Writer.PutValue( APP_URL, "URL" );
	Writer.PushTag( "Author" );
	Writer.PutValue( AUTHOR_NAME, "Name" );
	Writer.PutValue( AUTHOR_CONTACT, "Contact" );
	Writer.PopTag();
	Writer.PutValue( COPYRIGHT, "Copyright" );
	Writer.PopTag();
	Writer.PutValue( BackendInformations, "Backend" );
	Writer.PutValue( tol::Date( Buffer ), "Date" );
	Writer.PutValue( tol::Time( Buffer ), "Time" );
	Writer.PopTag();
ERRErr
ERREnd
ERREpilog

}


void Generate(
//	const bkdacc::strings_ &RawMessages,
	const types_ &Types,
	sdr::row__ MasterRow,
	const str::string_ &ProtocolVersion,
	const str::string_ &TargetLabel,
	const str::string_ &APIVersion,
	const str::string_ &BackendInformations,
	txf::text_oflow__ &Flow )
{
ERRProlog
	xml::writer Writer;
	tol::buffer__ Buffer;
	str::string Temp;
ERRBegin
	Writer.Init( Flow, xml::oIndent, xml::e_Default );
	
	Flow << "<!--                    DON'T MODIFY !!! !" << txf::nl;
	Flow << txf::pad << "This document was generated the " << tol::DateAndTime( Buffer ) << txf::nl;
	Flow << txf::pad << "using " NAME " V" VERSION << " (" APP_URL ")" << txf::nl;
	Flow << txf::pad << "Build : "__DATE__ " " __TIME__ << " (" << cpe::GetDescription() << ')' << txf::nl;
	Flow << "-->" << txf::nl;

	Writer.PushTag( "API" );
	Writer.PutAttribute( "target", TargetLabel );

	Temp.Init( TargetLabel );
	Writer.PutAttribute( "TargetUppercased", str::ToUpper( Temp ) );

	Writer.PutAttribute( "ProtocolVersion", ProtocolVersion );
	Writer.PutAttribute( "APIVersion", APIVersion );

	GenerateMisc( BackendInformations, Writer );	
	Generate( Types, MasterRow, Writer );

	Writer.PopTag();
ERRErr
ERREnd
ERREpilog
}

sdr::row__ FindMasterType( const types_ &Types )
{
	sdr::row__ P = NONE;
ERRProlog
	ctn::E_CITEM( type_ ) Type;
ERRBegin
	Type.Init( Types );
	
	P = Types.First();
	
	while( ( P != NONE ) && ( Type( P ).ID() != FBLFRD_MASTER_TYPE ) )
		P = Types.Next( P );
		
	if ( P == NONE )
		ERRFwk();
ERRErr
ERREnd
ERREpilog
	return P;
}


void Go(
	const parameters___ &Parameters,
	csducl::type__ Type )
{
ERRProlog
	types Types;
	str::string TargetLabel, ProtocolVersion, APIVersion;
	str::string BackendInformations;
	sdr::row__ MasterRow = NONE;
	bso::bool__ Backup = false;
	flf::file_oflow___ File;
	txf::text_oflow__ TFile;
	lcl::locale Dummy;
ERRBegin
	Types.Init();

	TargetLabel.Init();
	ProtocolVersion.Init();
	APIVersion.Init();
	BackendInformations.Init();

	GetBackendData( Parameters.BackendLocation, Type, Types, ProtocolVersion, TargetLabel, APIVersion, BackendInformations );
	
	MasterRow = FindMasterType( Types );

	if ( Parameters.FileName != NULL ) {
		scltool::CreateBackupFile( Parameters.FileName );
		Backup = true;

		COut << "Backend : " << BackendInformations << txf::nl << txf::commit;


		File.Init( Parameters.FileName );
		TFile.Init( File );
		Generate( Types, MasterRow, ProtocolVersion, TargetLabel, APIVersion, BackendInformations, TFile );
	} else
		Generate( Types, MasterRow, ProtocolVersion, TargetLabel, APIVersion, BackendInformations, COut );
ERRErr
	if ( Backup )
		scltool::RecoverBackupFile( Parameters.FileName );
ERREnd
ERREpilog
}

const char *scltool::TargetName = NAME;

void scltool::Main(
	int argc,
	const char *argv[] )
{
ERRProlog
	parameters___ Parameters;
	csducl::type__ Type = csducl::t_Undefined;
ERRBegin
	AnalyzeArgs( argc, argv, Parameters, Type );

	Go( Parameters, Type );
ERRErr
ERREnd
ERREpilog
}
