/*
	'compose' by Claude SIMON (csimon@epeios.org)
	create images compsed by 4 (2x2) images.
	Copyright (C) 2004 Claude SIMON

$NOTICE$
*/

// $$Id$$

#include "err.h"
#include "cio.h"
#include "epsmsc.h"
#include "clnarg.h"
#include "fil.h"
#include "xtf.h"
#include "txmtbl.h"

#define NAME			"compose"
#define VERSION			"0.1.0"
#define COPYRIGHT_YEARS	"2004"
#define DESCRIPTION		"geénerate images composed by 4 (2x2) sub-images."
#define INFO			EPSMSC_EPEIOS_TEXT
#define AUTHOR_NAME		EPSMSC_AUTHOR_NAME
#define AUTHOR_EMAIL	EPSMSC_AUTHOR_EMAIL
#define HELP			EPSMSC_HELP_INVITATION( NAME )
#define COPYRIGHT		"Copyright (c) " COPYRIGHT_YEARS " " AUTHOR_NAME " (" AUTHOR_EMAIL ")."
#define CVS_DETAILS		("$Id$\b " + 5)

using cio::cin;
using cio::cout;
using cio::cerr;

/* Beginning of the part which handles command line arguments. */

enum command {
	cHelp,
	cVersion,
	cLicense
};

enum option {
	// o
};

struct parameters {
	str::string FileName;
	parameters( void )
	{
	}
	void Init( void )
	{
		FileName.Init();
	}
};

void PrintUsage( const clnarg::description_ &Description )
{
	cout << DESCRIPTION << txf::nl;
	cout << NAME << " --version|--license|--help" << txf::nl;
	clnarg::PrintCommandUsage( Description, cVersion, "print version of " NAME " components.", clnarg::vSplit, false );
	clnarg::PrintCommandUsage( Description, cLicense, "print the license.", clnarg::vSplit, false );
	clnarg::PrintCommandUsage( Description, cHelp, "print this message.", clnarg::vOneLine, false );
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

	switch( Free.Amount() ) {
	case 1:
		Parameters.FileName = Free( P ).Convert();
		break;
	default:
		cerr << "Wrong number of arguments." << txf::nl;
		cout << HELP << txf::nl;
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
//	Description.AddOption( '', "", o );

	Analyzer.Init( argc, argv, Description );

	switch ( Analyzer.GetCommand() ) {
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
		epsmsc::PrintLicense();
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

void Execute( const str::string_ &Command )
{
ERRProlog
	tol::E_POINTER___( char ) Buffer;
ERRBegin
	Buffer = Command.Convert();

	cout << Buffer << "                    " << txf::rfl << txf::sync;

	if ( system( Buffer ) != 0 )
		ERRx();
ERRErr
ERREnd
ERREpilog
}

bso::ulong__ GetImageWidth( const str::string_ &FileName )
{
	bso::ulong__ Width = 0;
ERRProlog
	str::string Command;
	fil::file_iflow___ IFlow;
	xtf::extended_text_iflow__ TFlow;
	epeios::row__ Error = NONE;
ERRBegin
	Command.Init();

	Command.Append( "identify -format \"%w\" " );
	Command.Append( FileName );
	Command.Append( " >width" );

	Execute( Command );

	IFlow.Init( "width" );

	IFlow.EOFD( XTF_EOXT );

	TFlow.Init( IFlow );

	Command.Init();

	TFlow.GetLine( Command );

	Width = Command.ToUL( &Error );

	if ( Error != NONE )
		ERRx();
ERRErr
ERREnd
ERREpilog
	return Width;
}

bso::ulong__ GetImageHeight( const str::string_ &FileName )
{
	bso::ulong__ Height = 0;
ERRProlog
	str::string Command;
	fil::file_iflow___ IFlow;
	xtf::extended_text_iflow__ TFlow;
	epeios::row__ Error = NONE;
ERRBegin
	Command.Init();

	Command.Append( "identify -format \"%h\" " );
	Command.Append( FileName );
	Command.Append( " >height" );

	Execute( Command );

	IFlow.Init( "height" );

	IFlow.EOFD( XTF_EOXT );

	TFlow.Init( IFlow );

	Command.Init();

	TFlow.GetLine( Command );

	Height = Command.ToUL( &Error );

	if ( Error != NONE )
		ERRx();
ERRErr
ERREnd
ERREpilog
	return Height;
}


void Compose(
	const char *FileName1,
	const char *FileName2,
	bso::sign__ Sign,
	const char *OutputFileName )
{
ERRProlog
	str::string Command;
ERRBegin
	Command.Init();

	Command.Append( "convert " );
	Command.Append( FileName1 );
	Command.Append( ' ' );
	Command.Append( FileName2 );

	Command.Append( ' ' );

	switch( Sign ) {
	case -1:
		Command.Append( '-' );
		break;
	case 1:
		Command.Append( '+' );
		break;
	default:
		ERRc();
		break;
	}

	Command.Append( "append " );

	Command.Append( OutputFileName );

	Execute( Command );
ERRErr
ERREnd
ERREpilog
}

void ComposeHorizontally(
	 const char *FileName1,
	 const char *FileName2,
	 const char *OutputFileName )
{
	Compose( FileName1, FileName2, +1, OutputFileName );
}

void ComposeVertically(
	 const char *FileName1,
	 const char *FileName2,
	 const char *OutputFileName )
{
	Compose( FileName1, FileName2, -1, OutputFileName );
}

void Convert(
	 const str::string_ &FileName,
	 const char *Option,
	 const char *OutputFileName )
{
ERRProlog
	str::string Command;
ERRBegin
	Command.Init();

	Command.Append( "convert " );
	Command.Append( Option );
	Command.Append( ' ' );
	Command.Append( FileName );
	Command.Append( ' ' );
	Command.Append( OutputFileName );

	Execute( Command );
ERRErr
ERREnd
ERREpilog
}

void Convert(
	 const str::string_ &FileName,
	 const char *OutputFileName )
{
	Convert( FileName, "", OutputFileName );
}

void Rotate(
	 const str::string_ &FileName,
	 const char *OutputFileName )
{
	Convert( FileName, "-rotate 90", OutputFileName );
}

typedef ctn::E_XMCONTAINER_( str::string_ ) list_;
E_AUTO( list )

void GetImageList(
	const str::string_ &ListFileName,
	list_ &List )
{
ERRProlog
	fil::file_iflow___ Flow;
	xtf::extended_text_iflow__ TFlow;
	txmtbl::table Table;
	epeios::row__ Row = NONE;
	txmtbl::cell Cell;
	tol::E_POINTER___( char ) Buffer;
ERRBegin

	Buffer = ListFileName.Convert();

	if ( !tol::FileExists( Buffer ) ) {
		cerr << "Unable to open file '" << Buffer << "'." << txf::nl;
		ERRExit( EXIT_FAILURE );
	}


	Flow.Init( Buffer );

	Flow.EOFD( XTF_EOXT );

	TFlow.Init( Flow );

	Table.Init();

	txmtbl::GetTable( TFlow, Table );

	Table.Purge( '#' );

	if ( Table.Amount() % 4 ) {
		cerr << "The amount of files in '" << Buffer << "' list file must be a multiple of 4." << txf::nl;
		ERRExit( EXIT_FAILURE );
	}

	Row = Table.First();

	while ( Row != NONE ) {
		Cell.Init();

		if ( !Table.GetUniqueCell( Row, Cell ) ) {
			cerr << "Misformed '" << Buffer << "' list file at line " << Table.Location( Row ) << txf::nl;
			ERRExit( EXIT_FAILURE );
		}

		List.Append( Cell );

		Row = Table.Next( Row );
	}



ERRErr
ERREnd
ERREpilog
}

void ConvertAndRotateIfNeeded(
	const str::string_ &FileName,
	const char *OutputFileName )
{
	if ( GetImageWidth( FileName ) < GetImageHeight( FileName ) )
		Convert( FileName, OutputFileName );
	else
		Rotate( FileName, OutputFileName );
}


void Compose(
	const str::string_ &FileName1,
	const str::string_ &FileName2,
	const str::string_ &FileName3,
	const str::string_ &FileName4,
	const char *OutputFileName )
{
	ConvertAndRotateIfNeeded( FileName1, "UL.bmp" );
	ConvertAndRotateIfNeeded( FileName2, "UR.bmp" );
	ConvertAndRotateIfNeeded( FileName3, "LL.bmp" );
	ConvertAndRotateIfNeeded( FileName4, "LR.bmp" );
	
	ComposeHorizontally( "UL.bmp", "UR.bmp", "U.bmp" );
	ComposeHorizontally( "LL.bmp", "LR.bmp", "L.bmp" );

	ComposeVertically( "U.bmp", "L.bmp", OutputFileName );

	cout << FileName1 << ", " << FileName2 << ", " << FileName3 << ", " << FileName4 << " -> " << OutputFileName << txf::nl;
}

void HandleList( const list_ &List )
{
ERRProlog
	epeios::row__ Row = NONE;
	ctn::E_CMITEM( str::string_ ) FN1, FN2, FN3, FN4;
	str::string OutputFileName;
	bso::ulong__ Counter = 0;
	tol::E_POINTER___( char ) Buffer;
ERRBegin
	FN1.Init( List );
	FN2.Init( List );
	FN3.Init( List );
	FN4.Init( List );

	Row = List.First();

	while ( Row != NONE ) {
		FN1( Row );

		if ( ( Row = List.Next( Row ) ) == NONE )
			ERRc();

		FN2( Row );
		
		if ( ( Row = List.Next( Row ) ) == NONE )
			ERRc();

		FN3( Row );

		if ( ( Row = List.Next( Row ) ) == NONE )
			ERRc();

		FN4( Row );

		Row = List.Next( Row );

		OutputFileName.Init();

		OutputFileName.Append( "result" );
		OutputFileName.Append( bso::Convert( ++Counter ) );
		OutputFileName.Append( ".bmp" );

		Buffer = OutputFileName.Convert();


		Compose( FN1(), FN2(), FN3(), FN4(), Buffer);
	}
ERRErr
ERREnd
ERREpilog
}

void Go( const parameters &Parameters )
{
ERRProlog
	list List;
ERRBegin
	List.Init();

	GetImageList( Parameters.FileName, List );
	HandleList( List );
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
	Parameters.Init();

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
