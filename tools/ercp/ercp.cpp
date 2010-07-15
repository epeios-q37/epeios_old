/*
	'ercp' by Claude SIMON(csimon@zeusw.org)
	(description)
	Copyright (C) 2008 Claude SIMON.

$NOTICE$
*/

// $$Id$$

#include "err.h"
#include "cio.h"
#include "epsmsc.h"
#include "clnarg.h"
#include "stk.h"
#include "dir.h"
#include "fnm.h"
#include "flf.h"
#include "ltf.h"

#define NAME			"ercp"
#define VERSION			"0.1.0"
#define COPYRIGHT_YEARS	"2008"
#define DESCRIPTION		"copy files randomly"
#define INFO			EPSMSC_EPEIOS_AFFILIATION
#define AUTHOR_NAME		EPSMSC_AUTHOR_NAME
#define AUTHOR_CONTACT	EPSMSC_AUTHOR_CONTACT
#define HELP			EPSMSC_HELP_INVITATION( NAME )
#define COPYRIGHT		"Copyright (c) " COPYRIGHT_YEARS " " AUTHOR_NAME " (" AUTHOR_CONTACT ")."
#define CVS_DETAILS		("$Id$\b " + 5)

using cio::cin;
using cio::cout;
using cio::cerr;

#ifdef CPE__64_BITS_TYPES_ALLOWED
// Disk space size.
typedef bso::ullong__ dssize__;
#	define DSSIZE_MAX	BSO_ULLONG_MAX
#else
typedef bso::ulong__ dssize__;
#	define DSSIZE_MAX	BSO_ULONG_MAX
#endif

inline dssize__ ApplyFactor(
	dssize__ Size,
	dssize__ Factor )
{
	if ( ( DSSIZE_MAX / Factor ) < Size ) {
		cerr << "Total amount too big" << txf::nl;
		ERRExit( EXIT_FAILURE );
	}

	return Size * Factor;
}

dssize__ GetSize(
	str::string_ &SizeString,
	const char *ErrorMessageTarget )
{
	epeios::row__ ErrP = NONE;

	dssize__ Size = SizeString.ToULL( &ErrP );

	if ( ErrP != NONE ) {
		if ( SizeString( ErrP ) == 'T' ) 
			Size = ApplyFactor( Size, 1024ULL * 1024 * 1024 * 1024 );
		else if ( SizeString( ErrP ) == 't' )
			Size = ApplyFactor( Size, 1000ULL * 1000 * 1000 * 1000 );
		else if ( SizeString( ErrP ) == 'G' ) 
			Size = ApplyFactor( Size, 1024 * 1024 * 1024 );
		else if ( SizeString( ErrP ) == 'g' )
			Size = ApplyFactor( Size, 1000 * 1000 * 1000 );
		else if ( SizeString( ErrP ) == 'M' )
			Size = ApplyFactor( Size, 1024 * 1024 );
		else if ( SizeString( ErrP ) == 'm' )
			Size = ApplyFactor( Size, 1000 * 1000 );
		else if ( SizeString( ErrP ) == 'K' )
			Size = ApplyFactor( Size, 1024 );
		else if ( SizeString( ErrP ) == 'k' )
			Size = ApplyFactor( Size, 1000 );
		else {
			cerr << "Bad " << ErrorMessageTarget << " value !" << txf::nl;
			ERRExit( EXIT_FAILURE );
		}

		if ( ErrP != SizeString.Last() ) {
			cerr << "Bad " << ErrorMessageTarget << " value !" << txf::nl;
			ERRExit( EXIT_FAILURE );
		}
	}

	return Size;
}

/* Beginning of the part which handles command line arguments. */

enum command {
	cHelp,
	cVersion,
	cLicense,
	cCopy,	// Copy files.
};

enum option {
	// o
	oMinSize,	// Minimum size of the file.
	oMaxSize	// Maximum size of the file.
};

struct parameters {
	STR_BUFFER___ SourceDir;
	STR_BUFFER___ DestDir;
	dssize__ MinSize;
	dssize__ MaxSize;
	dssize__ TotalSize;
	parameters( void )
	{
		MinSize = TotalSize = 0;
		MaxSize = DSSIZE_MAX;

	}
};

void PrintUsage( const clnarg::description_ &Description )
{
	cout << DESCRIPTION << txf::nl;
	cout << NAME << " --version|--license|--help" << txf::nl;
	clnarg::PrintCommandUsage( Description, cVersion, "print version of " NAME " components.", clnarg::vSplit, false );
	clnarg::PrintCommandUsage( Description, cLicense, "print the license.", clnarg::vSplit, false );
	clnarg::PrintCommandUsage( Description, cHelp, "print this message.", clnarg::vOneLine, false );
	cout << NAME << " <command> [options] source-dir target-dir size" << txf::nl;
	// Free argument description.
	cout << "command:" << txf::nl;
	clnarg::PrintCommandUsage( Description, cCopy, "copy up to size bytes of files from source-dir to dest-dir.", clnarg::vSplit, true );
	cout << "options:" << txf::nl;
	clnarg::PrintOptionUsage( Description, oMinSize, "minimum size of the files to copy", clnarg::vOneLine );
	clnarg::PrintOptionUsage( Description, oMaxSize, "maximum size of the files to copy", clnarg::vOneLine );
	cout << txf::nl << "For the sizes, specify an integer, to which you can append :" << txf::nl;
	cout << txf::pad << "- 'T', 'G', 'M', or 'K', for base 1024 based multiplicator," << txf::nl;
	cout << "or" << txf::nl;
	cout << txf::pad << "- 't', 'g', 'm', or 'k', for base 1000 based multiplicator." << txf::nl;
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
		case oMaxSize:
			Analyzer.GetArgument( Option, Argument );
			if ( Argument.Amount() == 0 ) {
				cerr << "'" << Analyzer.Description().GetOptionLabels( oMaxSize ) << "' option must have an argument!" << txf::nl;
				ERRExit( EXIT_FAILURE );
			}
			Parameters.MaxSize = GetSize( Argument, "max size option" );
			break;
		case oMinSize:
			Analyzer.GetArgument( Option, Argument );
			if ( Argument.Amount() == 0 ) {
				cerr << "'" << Analyzer.Description().GetOptionLabels( oMinSize ) << "' option must have an argument!" << txf::nl;
				ERRExit( EXIT_FAILURE );
			}
			Parameters.MinSize = GetSize( Argument, "min size option" );
			break;
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
	case 3:
		Parameters.TotalSize = GetSize( Free( P ), "total amount argument" );

		P = Free.Previous( P );
		Free( P ).Convert( Parameters.DestDir );

		P = Free.Previous( P );
		Free( P ).Convert( Parameters.SourceDir );
		break;
	default:
		cerr << "Bad amount of arguments." << txf::nl;
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
	Description.AddCommand( CLNARG_NO_SHORT, "copy", cCopy );
	Description.AddOption( 'S', "MaxSize", oMaxSize );
	Description.AddOption( 's', "MinSize", oMinSize );

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
		epsmsc::PrintLicense( cout );
		ERRi();
		break;
	case cCopy:
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

typedef stk::E_XMCSTACK_( str::string_ ) path_;
E_AUTO( path );

typedef ctn::E_XMCONTAINER_( str::string_ ) file_;
E_AUTO( file );

typedef ctn::E_XCONTAINER_( file_ ) files_;
E_AUTO( files );

void Fill(
	const path_ &Path,
	file_ &File )
{
	ctn::E_CMITEMt( str::string_, stk::row__ ) Item;
	stk::row__ Row = Path.First();

	Item.Init( Path );

	while ( Row != NONE ) {
		File.Append( Item( Row ) );

		Row = Path.Next( Row );
	}
}

template <typename row__> void Build(
	const ctn::E_XMCONTAINERt_( str::string_, row__ ) &Items,
	str::string_ &String )
{
	ctn::E_CMITEMt( str::string_, row__ ) Item;
	row__ Row = Items.First();

	Item.Init( Items );

	while ( Row != NONE ) {
		if ( String.Amount() != 0 )
			String.Append( FNM_DIRECTORY_SEPARATOR_CHARACTER );

		String.Append( Item( Row ) );

		Row = Items.Next( Row );
	}

}

void Build(
	const path_ &Path,
	str::string_ &String )
{
	Build<stk::row__>( Path, String );
}

void Build(
	const file_ &File,
	str::string_ &String )
{
	Build<epeios::row__>( File, String );
}

void GetFiles(
	const char *RootDir,
	path_ &Path,
	files_ &Files )
{
ERRProlog
	str::string LocalizedFileName;
	dir::handle___ Handle = DIR_INVALID_HANDLE;
	const char *Name;
	STR_BUFFER___ LocalizedFileNameBuffer;
	file File;
ERRBegin
	LocalizedFileName.Init( RootDir );

	Build( Path, LocalizedFileName );

	LocalizedFileName.Convert( LocalizedFileNameBuffer );

	Name = dir::GetFirstFile( LocalizedFileNameBuffer, Handle );

	if ( Name == NULL ) {
		cerr << "Unable to parse directory '" << LocalizedFileNameBuffer << "' !" << txf::nl;
		ERRExit( EXIT_FAILURE );
	}

	while ( ( Name != NULL ) && *Name ) {

		if ( Name[strlen( Name )-1] != '.' ) {
			LocalizedFileName.Init( RootDir );

			Build( Path, LocalizedFileName );

			if ( LocalizedFileName.Amount() != 0 )
				LocalizedFileName.Append( FNM_DIRECTORY_SEPARATOR_CHARACTER );

			LocalizedFileName.Append( Name );

			LocalizedFileName.Convert( LocalizedFileNameBuffer );

			if ( fil::IsDirectory( LocalizedFileNameBuffer ) ) {
				Path.Push( str::string( Name ) );
				GetFiles( RootDir, Path, Files );
				Path.Pop();
			} else if ( fil::IsFile( LocalizedFileNameBuffer ) ) {
				File.Init();

				Fill( Path, File );

				File.Append( str::string( Name ) );

				Files.Append( File );
			}
		}

		Name = dir::GetNextFile( Handle );
	}

	if ( Name == NULL )
		ERRc();

ERRErr
ERREnd
	if ( Handle != DIR_INVALID_HANDLE )
		dir::Close( Handle );
ERREpilog
}

void Display(
	const file_ &File,
	txf::text_oflow__ &TOFlow )
{
ERRProlog
	str::string String;
ERRBegin
	String.Init();

	Build( File, String );

	TOFlow << String;
ERRErr
ERREnd
ERREpilog
}



void Display(
	const files_ &Files,
	txf::text_oflow__ &TOFlow )
{

	ctn::E_CITEM( file_ ) File;
	epeios::row__ Row = NONE;

	File.Init( Files );

	Row = Files.First();

	while ( Row != NONE ) {
		Display( File( Row ), TOFlow );

		cout << txf::nl;

		Row = Files.Next( Row );
	}
}

inline bso::bool__ Match(
	const char *RootDir,
	const file_ &File,
	dssize__ MinSize,
	dssize__ MaxSize,
	dssize__ TotalSize )
{
	bso::bool__ Match = false;
ERRProlog
	bso::size__ FileSize = BSO_SIZE_MAX;
	str::string FileName;
	STR_BUFFER___ FileNameBuffer;
ERRBegin
	FileName.Init( RootDir );

	Build( File, FileName );

	FileName.Convert( FileNameBuffer );

	FileSize = fil::GetFileSize( FileNameBuffer );

	Match = ( FileSize >= MinSize ) && ( FileSize <= MaxSize ) && ( FileSize <= TotalSize );
ERRErr
ERREnd
ERREpilog
	return Match;
}

void CreateMissingDirectories(
	const char *Root,
	const file_ &File )
{
ERRProlog
	epeios::row__ LastRow = NONE;
	epeios::row__ Row = NONE;
	ctn::E_CMITEM( str::string_) Item;
	str::string Directory;
	STR_BUFFER___ DirectoryBuffer;
ERRBegin
	LastRow = File.Last();
	Row = File.First();

	Item.Init( File );

	Directory.Init( Root );

	while ( Row != LastRow ) {	// Fonctionne aussi si 'File' vide.
		if ( Directory.Amount() != 0 )
			Directory.Append( FNM_DIRECTORY_SEPARATOR_CHARACTER );

		Directory.Append( Item( Row ) );

		Directory.Convert( DirectoryBuffer );

		switch ( dir::CreateDir( DirectoryBuffer ) ) {
		case dir::sOK:
		case dir::sExists:
			break;
		default:
			cerr << "Impossible de créer le répertoire '" << DirectoryBuffer << "' !" << txf::nl;
			ERRExit( EXIT_FAILURE );
		}

		Row = File.Next( Row );
	}
ERRErr
ERREnd
ERREpilog
}

void Copy(
	const char *Source,
	const char *Dest,
	bso::size__ Size )
{
ERRProlog
	flf::file_iflow___ IFlow;
	flf::file_oflow___ OFlow;
ERRBegin
	if ( IFlow.Init( Source, err::hSkip ) != fil::sSuccess ) {
		cerr << "Unable to open '" << Source << "for reading." << txf::nl;
		ERRExit( EXIT_FAILURE );
	}

	if ( OFlow.Init( Dest, err::hSkip ) != fil::sSuccess ) {
		cerr << "Unable to open '" << Dest << "for writing." << txf::nl;
		ERRExit( EXIT_FAILURE );
	}

	flw::Copy( IFlow, Size, OFlow );

ERRErr
ERREnd
ERREpilog
}

void HandleError( const char *FileName )
{
ERRProlog
ERRBegin
	cerr << txf::nl << "Unable to copy '" << FileName << "'. Not enough  space left ?" << txf::nl;

	if ( fil::FileExists( FileName ) )
		fil::RemoveFile( FileName );
ERRErr
ERREnd
	ERRExit( EXIT_FAILURE );
ERREpilog
}

bso::size__ Copy(
	const str::string_ &Source,
	const str::string_ &Dest )
{
	bso::size__ Extent = 0;
ERRProlog
	STR_BUFFER___ SourceBuffer, DestBuffer;
ERRBegin
	Source.Convert( SourceBuffer );
	Dest.Convert( DestBuffer );

	if ( !fil::FileExists( DestBuffer ) ) {
		Extent = fil::GetFileSize( SourceBuffer );

		Copy( SourceBuffer, DestBuffer, Extent );
	}
ERRErr
	HandleError( DestBuffer );
ERREnd
ERREpilog
	return Extent;	// Devrait en fait contenir la taille occupée sur le disque du fichier destination ... Contient 0 si le fichier existe déjà.
}

bso::size__ Copy(
	const char *Source,
	const file_ &File,
	const char *Dest )
{
	bso::size__ Extent = 0;
ERRProlog
	str::string SourceFileName, DestFileName;
ERRBegin
	SourceFileName.Init( Source );
	Build( File, SourceFileName );

	DestFileName.Init( Dest );
	Build( File, DestFileName );

	Extent = Copy( SourceFileName, DestFileName );
ERRErr
ERREnd
ERREpilog
	return Extent;
}

// Supposé retourner la taille qu'occuppe le fichier sur le disque, lais je ne sais pas comment obtenir cette information sous Windows ...
bso::size__ CopyCreatingMissingDirectories(
	const char *Source,
	const file_ &File,
	const char *Dest )
{
	bso::size__ Extent;
ERRProlog
	str::string FileName;
ERRBegin
	FileName.Init();

	CreateMissingDirectories( Dest, File );

	Build( File, FileName );

	Extent = Copy( Source, File, Dest );
ERRErr
ERREnd
ERREpilog
	return Extent;
}



static inline void DisplayTime(
	time_t Time,
	txf::text_oflow__ &TOFlow,
	bso::bool__ Compact = true )
{
	bso::integer_buffer__ IBuffer;
	bso::ulong__ H = (bso::ulong__)( Time / 3600 ), M = (bso::ulong__)( ( Time % 3600 ) / 60 );

	if ( H  != 0 )
		TOFlow << ( bso::Convert( H, IBuffer ) ) << 'h';

	if ( ( M != 0 ) || ( H != 0 ) )
		TOFlow << bso::Convert( M, IBuffer ) << 'm';

	if ( ( H == 0 ) && ( ( M < 10 ) || !Compact ) )
		TOFlow <<  bso::Convert( (bso::ulong__)( Time % 60 ), IBuffer ) << 's';
}

inline void DisplaySize(
	dssize__ Size,
	txf::text_oflow__ &TOFlow )
{
	if ( Size > ( 10ULL * 1024 * 1024 * 1024 * 1024 ) )
		TOFlow <<  Size / ( 1024ULL * 1024 * 1024 * 1024 ) << 'T';
	else if ( Size > ( 10ULL * 1024 * 1024 * 1024 ) )
		TOFlow <<  Size / ( 1024 * 1024 * 1024 ) << 'G';
	else if ( Size > ( 10 * 1024 * 1024 ) )
		TOFlow <<  Size / ( 1024 * 1024 ) << 'M';
	else if ( Size > ( 10 * 1024 ) )
		TOFlow <<  Size / ( 1024 ) << 'K';
	else
		TOFlow <<  Size;

	TOFlow << "B";
}

inline void DisplayDebit(
	dssize__ Debit,
	txf::text_oflow__ &TOFlow )
{
	DisplaySize( Debit, TOFlow );

	TOFlow << "/s";
}


void RandomCopy(
	const char *Source,
	files_ &Files,
	const char *Dest,
	dssize__ MinSize,
	dssize__ MaxSize,
	dssize__ TotalSize )
{
	epeios::row__ Row = NONE;
	ctn::E_CITEM( file_ ) File;
	bso::size__ FileExtent = 0;
	dssize__ OriginalTotalSize = TotalSize;
	ltf::line_text_oflow___<> LTFlow( cout );
	time_t Start = tol::Clock(false), Diff = 0;
	dssize__ Debit = 0;
	bso::ulong__ FileAmount = 0;

	LTFlow.Init();

	File.Init( Files );

	tol::InitializeRandomGenerator();

	if ( ( TotalSize * 100 ) > DSSIZE_MAX )
		ERRl();	// Algorithme d'affichage ne fonctionne pas.

	while ( Files.Amount() ) {
		Row = Files.First( rand() % Files.Amount() );

		if ( Match( Source, File( Row ), MinSize, MaxSize, TotalSize ) ) {
			Diff = tol::Clock(false) - Start;

			if ( Diff != 0 )
				Debit = 1000 * ( OriginalTotalSize - TotalSize ) / Diff;

			LTFlow.Clear();

			LTFlow << ( 100 - ( TotalSize * 100 ) / OriginalTotalSize ) << "% ";

			if ( Debit != 0 ) {
				LTFlow << '(';

				DisplayDebit( Debit, LTFlow );

				LTFlow << ", ";

				DisplayTime( TotalSize / Debit, LTFlow );
				LTFlow << " left) ";
			}

			LTFlow.Freeze();

			Display( File( Row ), LTFlow );

			LTFlow << txf::sync;

			FileExtent = CopyCreatingMissingDirectories( Source, File( Row ), Dest );

			if ( FileExtent != 0 )	// Sinon le fichier existait déjà.
				FileAmount++;

			if ( FileExtent >= TotalSize )
				break;
			else
				TotalSize -= FileExtent;
		}

		Files.Remove( Row );
		File.Flush();
	}

	LTFlow.Clear();

	cout << FileAmount << " files (";
	DisplaySize( OriginalTotalSize - TotalSize, LTFlow );
	cout << ") copied in ";

	DisplayTime( ( tol::Clock( false ) - Start ) / 1000, cout, false );
	cout << " seconds (";
	DisplayDebit( Debit, LTFlow );
	cout << ")." << txf::nl;




}

void GetFiles(
	const char *RootDir,
	files_ &Files )
{
ERRProlog
	path Path;
ERRBegin
	Path.Init();

	GetFiles( RootDir, Path, Files );
ERRErr
ERREnd
ERREpilog
}

void RandomCopy(
	const char *Source,
	const char *Dest,
	dssize__ MinSize,
	dssize__ MaxSize,
	dssize__ TotalSize )
{
ERRProlog
	files Files;
ERRBegin
	Files.Init();

	GetFiles( Source, Files );

/*	Display( Files );

	cout << "--------------------" << txf::nl;

	cout << MinSize << txf::tab << MaxSize << txf::tab << TotalSize << txf::nl;
*/
	RandomCopy( Source, Files, Dest, MinSize, MaxSize, TotalSize );
ERRErr
ERREnd
ERREpilog
}


void Go( const parameters &Parameters )
{
ERRProlog
ERRBegin
	RandomCopy( Parameters.SourceDir, Parameters.DestDir, Parameters.MinSize, Parameters.MaxSize, Parameters.TotalSize );
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
