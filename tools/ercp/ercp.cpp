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

#define NAME			"ercp"
#define VERSION			"0.1.0"
#define COPYRIGHT_YEARS	"2008"
#define DESCRIPTION		"(description)"
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
	parameters( void )
	{
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
	case 0:
		break;
	default:
		cerr << "Too many arguments." << txf::nl;
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
		epsmsc::PrintLicense( cout );
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
	tol::E_FPOINTER___( char ) Buffer;
	file File;
ERRBegin
	LocalizedFileName.Init( RootDir );

	Build( Path, LocalizedFileName );

	Buffer = LocalizedFileName.Convert();

	Name = dir::GetFirstFile( Buffer, Handle );

	while ( ( Name != NULL ) && *Name ) {

		if ( Name[strlen( Name )-1] != '.' ) {
			LocalizedFileName.Init( RootDir );

			Build( Path, LocalizedFileName );

			if ( LocalizedFileName.Amount() != 0 )
				LocalizedFileName.Append( FNM_DIRECTORY_SEPARATOR_CHARACTER );

			LocalizedFileName.Append( Name );

			Buffer = LocalizedFileName.Convert();

			if ( fil::IsDirectory( Buffer ) ) {
				Path.Push( str::string( Name ) );
				GetFiles( RootDir, Path, Files );
				Path.Pop();
			} else if ( fil::IsFile( Buffer ) ) {
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

void Display( const files_ &Files )
{
ERRProlog
	ctn::E_CITEM( file_ ) File;
	epeios::row__ Row = NONE;
	str::string String;
ERRBegin
	File.Init( Files );

	Row = Files.First();

	while ( Row != NONE ) {
		String.Init();

		Build( File( Row ), String );

		cout << String << txf::nl;

		Row = Files.Next( Row );
	}
ERRErr
ERREnd
ERREpilog
}

inline bso::bool__ Match(
	const char *RootDir,
	const file_ &File,
	bso::size__ MinSize,
	bso::size__ MaxSize,
	bso::size__ TotalSize )
{
	bso::bool__ Match = false;
ERRProlog
	bso::size__ FileSize = BSO_SIZE_MAX;
	str::string FileName;
	tol::E_FPOINTER___( bso::char__ ) FileNameBuffer;
ERRBegin
	FileName.Init( RootDir );

	Build( File, FileName );

	FileNameBuffer = FileName.Convert();

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
	tol::E_FPOINTER___( bso::char__) DirectoryBuffer;
ERRBegin
	LastRow = File.Last();
	Row = File.First();

	Item.Init( File );

	Directory.Init( Root );

	while ( Row != LastRow ) {	// Fonctionne aussi si 'File' vide.
		if ( Directory.Amount() != 0 )
			Directory.Append( FNM_DIRECTORY_SEPARATOR_CHARACTER );

		Directory.Append( Item( Row ) );

		DirectoryBuffer = Directory.Convert();

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

bso::size__ Copy(
	const str::string_ &Source,
	const str::string_ &Dest )
{
	bso::size__ Extent = 0;
ERRProlog
	tol::E_FPOINTER___( bso::char__ ) SourceBuffer, DestBuffer;
ERRBegin
	SourceBuffer = Source.Convert();
	DestBuffer = Dest.Convert();

	if ( !fil::FileExists( DestBuffer ) ) {
		Extent = fil::GetFileSize( SourceBuffer );

		Copy( SourceBuffer, DestBuffer, Extent );
	}
ERRErr
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

void RandomCopy(
	const char *Source,
	files_ &Files,
	const char *Dest,
	bso::size__ MinSize,
	bso::size__ MaxSize,
	bso::size__ TotalSize )
{
	epeios::row__ Row = NONE;
	ctn::E_CITEM( file_ ) File;
	bso::size__ FileExtent = 0;

	File.Init( Files );

	tol::InitializeRandomGenerator();

	while ( Files.Amount() ) {
		Row = Files.First( rand() % Files.Amount() );

		if ( Match( Source, File( Row ), MinSize, MaxSize, TotalSize ) ) {
			FileExtent = CopyCreatingMissingDirectories( Source, File( Row ), Dest );

			if ( FileExtent >= TotalSize )
				break;
			else
				TotalSize -= FileExtent;
		}

		Files.Remove( Row );
		File.Flush();
	}
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
	bso::size__ MinSize,
	bso::size__ MaxSize,
	bso::size__ TotalSize )
{
ERRProlog
	files Files;
ERRBegin
	Files.Init();

	GetFiles( Source, Files );

	Display( Files );

	cout << "--------------------" << txf::nl;

	RandomCopy( Source, Files, Dest, MinSize, MaxSize, TotalSize );
ERRErr
ERREnd
ERREpilog
}


void Go( const parameters &Parameters )
{
ERRProlog
ERRBegin
RandomCopy( "z:\\Music\\OGG", "g:\\MUSIC", 3000000, 40000000, 4114706432 );
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
