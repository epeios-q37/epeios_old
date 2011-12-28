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

cio::cerr___ CErr;
cio::cout___ COut;

using xml::writer_;

#define NAME			"getbkdapi"
#define VERSION			"0.2.4"
#define COPYRIGHT_YEARS	"2001-2006; 2009-2011"
#define DESCRIPTION		"Get the API from an Epeios-driven backend."
#define INFO			EPSMSC_EPEIOS_PROJECT_AFFILIATION
#define AUTHOR_NAME		EPSMSC_AUTHOR_NAME
#define AUTHOR_CONTACT	EPSMSC_AUTHOR_CONTACT
#define HELP			EPSMSC_HELP_INVITATION( NAME )
#define COPYRIGHT		EPSMSC_COPYRIGHT( COPYRIGHT_YEARS )
#define CVS_DETAILS		("$Id$\b " + 5)
#define COPYRIGHT_OWNER	AUTHOR_NAME " (" AUTHOR_CONTACT ")"
#define URL				"http://zeusw.org/"
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

struct arguments {
	const char *BackendLocation;
	STR_BUFFER___ BackendLocationBuffer;
	const char *FileName;
	STR_BUFFER___ FileNameBuffer;
	arguments( void )
	{
		BackendLocation = FileName = NULL;
	}
};

void PrintUsage( const clnarg::description_ &Description )
{
	clnarg::buffer__ Buffer;

	COut << DESCRIPTION << txf::nl;
	COut << NAME << " --version|--license|--help" << txf::nl;
	clnarg::PrintCommandUsage( Description, cVersion, "print version of " NAME " components.", clnarg::vSplit, false, COut );
	clnarg::PrintCommandUsage( Description, cLicense, "print the license.", clnarg::vSplit, false, COut );
	clnarg::PrintCommandUsage( Description, cHelp, "print this message.", clnarg::vOneLine, false, COut );
	COut << NAME << " <command> <backend> [file]" << txf::nl;
	COut << "command:" << txf::nl;
//	clnarg::PrintCommandUsage( Description, c, "", false, true );
	clnarg::PrintCommandUsage( Description, cDaemon, "get the API from a daemon-type backend.", clnarg::vSplit, false, COut );
	clnarg::PrintCommandUsage( Description, cLibrary, "get the API from a library-type backend.", clnarg::vSplit, false, COut );
	// Free argument description.
	COut << "backend:" << txf::nl << txf::tab << "the backend location." << txf::nl;
	COut << txf::tab << "- for " << Description.GetCommandLabels( cDaemon, Buffer ) << " command, backend's host:port (ex.: 'localhost:1234')." << txf::nl; 
	COut << txf::tab << "- for " << Description.GetCommandLabels( cLibrary, Buffer ) << " command, backend's library name." << txf::nl; 
	COut << "file:" << txf::tab << "The XML output file. Standard output if not given." << txf::nl;
////	COut << "options:" << txf::nl;
//	clnarg::PrintOptionUsage( Description, o, "", false );
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
	arguments &Arguments )
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
#pragma warning ( disable : 4065 )
		switch( Option = Options( P ) ) {
//		case o:
		default:
			ERRc();
		}
#pragma warning ( default : 4065 )

		P = Options.Next( P );
	}
	
ERRErr
ERREnd
ERREpilog
}

static void AnalyzeFreeArguments(
	clnarg::analyzer___ &Analyzer,
	arguments &Arguments )
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
		Arguments.FileName = Free( P ).Convert( Arguments.FileNameBuffer);
		P = Free.Previous( P );
	case 1:
		Arguments.BackendLocation = Free( P ).Convert( Arguments.BackendLocationBuffer );
		break;
	case 0:
		CErr << "Too few arguments." << txf::nl;
		COut << HELP << txf::nl;
		ERRi();
	default:
		CErr << "Too many arguments." << txf::nl;
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
	arguments &Arguments,
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
		PrintHeader();
		TTR.Advertise( COut );
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
	case cLibrary:
		Type = csducl::tLibrary;
		break;
	case cDaemon:
		Type = csducl::tDaemon;
		break;
	case CLNARG_NONE:
		CErr << "Command required !" << txf::nl;
		COut << HELP << txf::nl;
		ERRi();
		break;
	default:
		ERRc();
	}

	AnalyzeOptions( Analyzer, Arguments );

	AnalyzeFreeArguments( Analyzer, Arguments );

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
		ERRc();
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
	case fblcst::cSLong:
		TypeName = "slong";
		break;
	case fblcst::cSLongs:
		TypeName = "slongs";
		CastType = ctBunch;
		break;
	case fblcst::cULong:
		TypeName = "ulong";
		break;
	case fblcst::cULongs:
		TypeName = "ulongs";
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
		ERRc();
		break;
	}

	return TypeName;
}

void Generate(
	const fblcst::cast &Cast,
	bso::ubyte__ ID,
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
		ERRc();
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
	epeios::row__ Current )
{
	if ( ( Current = Parameters.Next( Current ) ) == NONE )
		return true;

	if ( *Parameters( Current ) != fblcst::cEnd )
		return false;

	return Parameters.Next( Current ) == NONE;
}

void Generate(
	const parameters_ &Parameters,
	writer_ &Writer )
{
	epeios::row__ P = Parameters.First();
	fblcst::cast Cast;
	bso::ubyte__ ID = 1;
	
	if ( ( Cast = (fblcst::cast)*Parameters( P ) ) == fblcst::cEnd )
		Writer.PushTag( "Out" );
	else {
		Writer.PushTag( "In" );
		Generate( Cast, ID, Writer, IsLast( Parameters, P ) );
		ID++;
	}
		
	P = Parameters.Next( P );
		
	while( P != NONE ) {
		if ( ( Cast = (fblcst::cast)*Parameters( P ) ) == fblcst::cEnd ) {
			
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
	epeios::size__ &Position )
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
	epeios::size__ &Position )
{
	Writer.PushTag( "Command" );
	
	Writer.PutValue( Command.Identification.Value, "Name" );
	Writer.PutValue( Convert( *Command.Identification.ID() ), "ID" );
	
	Generate( Command.Parameters, Writer, Position );
	
	Writer.PopTag();
}

void Generate(
	const commands_ &Commands,
	writer_ &Writer )
{
ERRProlog
	ctn::E_CITEM( command_ ) Command;
	epeios::row__ P;
	char Buffer[20];
	epeios::size__ Position = 0;
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
	epeios::row__ P = NONE;
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

class dummy_error_reporting_functions__
: public fblfrd::error_reporting_functions__
{
protected:
	void FBLFRDReportError(
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
	dummy_error_reporting_functions__ DummyErrorReportingFunctions;
	csdlec::library_data__ LibraryData;
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
		ERRc();
		break;
	}

	LibraryData.Init( csdleo::mEmbedded, flx::VoidOFlowDriver, flx::VoidOFlowDriver );

	if ( !Core.Init( Location, LibraryData, *(csdsnc::log_functions__ *)NULL, Type, 0 ) ) {
		CErr << "Unable to access the backend !" << txf::nl;
		ERRExit( EXIT_FAILURE );
	}


	Flow.Init( Core );

	Dummy.Init();

	BackendAccess.Init( "", fblfrd::compatibility_informations__(), Flow, FBLMode, DummyErrorReportingFunctions, Dummy );

	GetDescription( BackendAccess, Types );
	
	BackendAccess.About( ProtocolVersion, TargetLabel, APIVersion, BackendInformations );

	BackendAccess.Disconnect();
ERRErr
	if ( ERRMajor != err::itn ) {
		CErr << "Unable to communicate with the backend !" << txf::nl;
		ERRExit( EXIT_FAILURE );
	}
ERREnd
ERREpilog
}

void Generate(
	const types_ &Types,
	epeios::row__ MasterRow,
	writer_ &Writer )
{
ERRProlog
	epeios::row__ P;
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
	tol::buffer__ Buffer;

	Writer.PushTag( "Misc" );
	Writer.PushTag( "Generator" );
	Writer.PutValue( NAME, "Name" );
	Writer.PutValue( VERSION, "Version" );
	Writer.PushTag( "Author" );
	Writer.PutValue( AUTHOR_NAME, "Name" );
	Writer.PutValue( AUTHOR_CONTACT, "Contact" );
	Writer.PopTag();
	Writer.PushTag( "Copyright" );
	Writer.PutValue( COPYRIGHT_OWNER, "Owner" );
	Writer.PutValue( COPYRIGHT_YEARS, "Date" );
	Writer.PopTag();
	Writer.PopTag();
	Writer.PutValue( BackendInformations, "Backend" );
	Writer.PutValue( tol::Date( Buffer ), "Date" );
	Writer.PutValue( tol::Time( Buffer ), "Time" );
	Writer.PopTag();
}


void Generate(
//	const bkdacc::strings_ &RawMessages,
	const types_ &Types,
	epeios::row__ MasterRow,
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
	
	Flow << "<!-- Don't modify !" << txf::nl << "This document was generated using " NAME " " VERSION " (" URL ") ";
	Flow << "on " << tol::DateAndTime( Buffer ) << ". -->" << txf::nl;
	Flow << "<!-- CVS feature : $Id$ -->" << txf::nl;
	
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

epeios::row__ FindMasterType( const types_ &Types )
{
	epeios::row__ P = NONE;
ERRProlog
	ctn::E_CITEM( type_ ) Type;
ERRBegin
	Type.Init( Types );
	
	P = Types.First();
	
	while( ( P != NONE ) && ( Type( P ).ID() != FBLFRD_MASTER_TYPE ) )
		P = Types.Next( P );
		
	if ( P == NONE )
		ERRb();
ERRErr
ERREnd
ERREpilog
	return P;
}


void Go(
	const arguments &Arguments,
	csducl::type__ Type )
{
ERRProlog
	types Types;
	str::string TargetLabel, ProtocolVersion, APIVersion;
	str::string BackendInformations;
	epeios::row__ MasterRow = NONE;
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

	GetBackendData( Arguments.BackendLocation, Type, Types, ProtocolVersion, TargetLabel, APIVersion, BackendInformations );
	
	MasterRow = FindMasterType( Types );

	if ( Arguments.FileName != NULL ) {
		Dummy.Init();
		if ( fil::CreateBackupFile( Arguments.FileName, fil::bmRename, lcl::rack__( Dummy, "" ), CErr ) != fil::bsOK ) {
			CErr << "Unable to create backup file." << txf::nl;
			ERRi();
		}

		COut << "Backend : " << BackendInformations << txf::nl;

		Backup = true;

		File.Init( Arguments.FileName );
		TFile.Init( File );
		Generate( Types, MasterRow, ProtocolVersion, TargetLabel, APIVersion, BackendInformations, TFile );
	} else
		Generate( Types, MasterRow, ProtocolVersion, TargetLabel, APIVersion, BackendInformations, COut );
ERRErr
	if ( Backup )
		fil::RecoverBackupFile( Arguments.FileName, lcl::rack__( Dummy, "" ), CErr );
ERREnd
ERREpilog
}


static inline void Main(
	int argc,
	const char *argv[] )
{
ERRProlog
	arguments Arguments;
	csducl::type__ Type = csducl::t_Undefined;
ERRBegin
	COut.Init();
	CErr.Init();

	AnalyzeArgs( argc, argv, Arguments, Type );

	Go( Arguments, Type );
ERRErr
ERREnd
ERREpilog
}

int main(
	int argc,
	const char *argv[] )
{
	int ExitValue = EXIT_SUCCESS;
ERRFProlog
ERRFBegin
	Main( argc, argv );
ERRFErr
	ExitValue = EXIT_FAILURE;

	if ( ERRMajor == err::itn )
		ERRRst();
ERRFEnd
ERRFEpilog
	COut << txf::commit;
	CErr << txf::commit;

	return ExitValue;
}
