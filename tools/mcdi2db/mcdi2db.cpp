// $Id$

#include "str.h"
#include "ctn.h"
#include "txmtbl.h"
#include "fil.h"
#include "epsmsc.h"
#include "clnarg.h"

typedef str_string_	title_;
typedef str_string	title;

typedef XMCONTAINER_( title_ ) titles_;
typedef XMCONTAINER( title_ ) titles;

template <typename content_> class generic_
{
public:
	str_string_ Name;
	content_ Content;
	struct s {
		str_string_::s Name;
		typename content_::s Content;
	};
	generic_( s &S )
	: Name( S.Name ),
	  Content( S.Content )
	{}
	void reset( bso__bool P = true )
	{
		Name.reset( P );
		Content.reset( P );
	}
	void plug( mmm_multimemory_ &M )
	{
		Name.plug( M );
		Content.plug( M );
	}
	generic_ &operator =( const generic_ &G )
	{
		Name = G.Name;
		Content = G.Content;
		
		return *this;
	}
	void Init( void )
	{
		Name.Init();
		Content.Init();
	}
};

AUTO1( generic )

typedef generic_<titles_>	album_;
typedef generic<titles_>	album;

typedef XCONTAINER_( album_ )	albums_;
typedef XCONTAINER( album_ ) 	albums;

typedef generic_<albums_>	disc_;
typedef generic<albums_> 	disc;
	

#define NAME			"mcdi2db"
#define VERSION			"0.0.1"
#define AUTHOR_LINK		EPSMSC_AUTHOR_LINK
#define AUTHOR			EPSMSC_AUTHOR
#define AUTHOR_EMAIL	EPSMSC_AUTHOR_EMAIL

#define DEFAULT_FIELD_DELIMITER	'\t'
#define DEFAULT_LINE_DELIMITER	'\n'
#define DEFAULT_ALBUM_MARKER	'a'
#define DEFAULT_TITLE_MARKER	't'

enum command {
	cHelp,
	cVersion,
	cConvert,
};

enum option {
	oFieldDelimiter,
	oLineDelimiter,
	oDiscName,
	oAlbumMarker,
	oTitleMarker,
	o_amount
};



void FillTable(
	const char *Src,
	txmtbl_table_ &Table )
{
ERRProlog
	fil::file_iflow___ FFlow;
	xtf::extended_text_iflow___ TFlow;
ERRBegin
	FFlow.Init( Src );
	FFlow.EOFT( XTF_EOXT );
	TFlow.Init( FFlow );
	TXMTBLGetTable( TFlow, Table );
	
	Table.DeleteEmptyCells();
ERRErr
ERREnd
ERREpilog
}

const txmtbl_cell_ &GetCell( const txmtbl_line_ &Line )
{
	static CMITEM( txmtbl_cell_ ) Cell;
	
	Cell.Init( Line );
	
	if ( Line.Amount() != 1 )
		ERRu();
		
	return Cell( Line.First() );
}
	
#define TEST	if ( P == NONE ) ERRu();

void FillDisc(
	const txmtbl_table_ &Table,
	disc_ &Disc )
{
ERRProlog
	CITEM( txmtbl_line_ ) Line;
	album Album;
	POSITION__ P;
ERRBegin
	Line.Init( Table );

	P = Table.First();

	TEST	
		
	if ( GetCell( Line( P ) ) != str_string(  ".:" ) )
		ERRu();
		
	P = Table.Next( P );

	while( ( P != NONE ) && ( Line( P ).Amount() != 0 ) )
		P = Table.Next( P );

	TEST		
		
	P = Table.Next( P );
	
	while( P != NONE ) {
	
		Album.Init();
		Album.Name = GetCell( Line( P ) );
		
		if ( Album.Name.Amount() == 0 )
			ERRu();
			
		Album.Name.Truncate();
		
		P = Table.Next( P );
		
		while( ( P != NONE ) && ( Line( P ).Amount() != 0 ) ) {
			Album.Content.Add( GetCell( Line( P ) ) );
			P = Table.Next( P );
		}
		
		if ( P != NONE )
			P = Table.Next( P);
			
		Disc.Content.Add( Album );
	}
			
ERRErr
ERREnd
ERREpilog
}

void Print(
	const album_ &Album,
	txf__text_oflow_ &Flow,
	int AlbumNumber,
	const char *DiscID,
	char LineDelimiter,
	char FieldDelimiter,
	char AlbumMarker,
	char TitleMarker,
	const str_string_ &DiscName )
{
ERRProlog
	CMITEM( title_ ) Title;
	POSITION__ P;
	char Buffer[50];
	int T = 0;
ERRBegin
	Title.Init( Album.Content );
	
	P = Album.Content.First();

	while( P != NONE ) {
		
		if ( T == 99 )
			ERRl();
				
		T++;
							
		sprintf( Buffer, "%c%.2i%c%.2i", AlbumMarker, AlbumNumber, TitleMarker, T );
		Flow << DiscID << Buffer << FieldDelimiter << DiscName << FieldDelimiter << Album.Name << FieldDelimiter << Title( P ) << LineDelimiter;
		P = Album.Content.Next( P );
	}
ERRErr
ERREnd
ERREpilog
}

void Print(
	const disc &Disc,
	txf__text_oflow_ &Flow,
	const char *DiscID,
	char LineDelimiter,
	char FieldDelimiter,
	char AlbumMarker,
	char TitleMarker )
{
ERRProlog
	CITEM( album_ ) Album;
	POSITION__ P;
	int A = 0;
	char Buffer[50];
ERRBegin
	Album.Init( Disc.Content );
	
	sprintf( Buffer, "%c%.2i%c%.2i", AlbumMarker, 0, TitleMarker, 0 );
	
	Flow << DiscID << Buffer << FieldDelimiter << Disc.Name << FieldDelimiter << FieldDelimiter << LineDelimiter;
	
	P = Disc.Content.First();
	
	while( P != NONE ) {
	
		if ( A == 99 )
			ERRl();
			
		A++;
		
		sprintf( Buffer, "%c%.2i%c%.2i", AlbumMarker, A, TitleMarker, 0 );

		
		Flow << DiscID << Buffer << FieldDelimiter << Disc.Name << FieldDelimiter << Album( P ).Name << FieldDelimiter << LineDelimiter;
		
		Print( Album( P ), Flow, A, DiscID, LineDelimiter, FieldDelimiter, AlbumMarker, TitleMarker, Disc.Name );
		
		P = Disc.Content.Next( P );
	}
			
		
	
ERRErr
ERREnd
ERREpilog
}

void FillDisc(
	const char *Src,
	disc_ &Disc )
{
ERRProlog
	txmtbl_table Table;
ERRBegin
	Table.Init();
	FillTable( Src, Table );
	FillDisc( Table, Disc );
ERRErr
ERREnd
ERREpilog
}

void Go(
	const char *Src,
	txf__text_oflow_ &Flow,
	const char *DiscID,
	char LineDelimiter,
	char FieldDelimiter,
	char AlbumMarker,
	char TitleMarker,
	const str_string_ &DiscName )
{
ERRProlog
	disc Disc;
ERRBegin
	Disc.Init();
	Disc.Name = DiscName;

	FillDisc( Src, Disc );	
	Print( Disc, Flow, DiscID, LineDelimiter, FieldDelimiter, AlbumMarker, TitleMarker );
ERRErr
ERREnd
ERREpilog
}


void PrintUsage( void )
{
	fout << "Usage: " << NAME << " [command] [options] source-file disc-id [dest-file]" << nl;
	fout << "source-file:" << tab << "file with the disc index." << nl;
	fout << "disc-id:" << tab << "ID of the disc." << nl;
	fout << "dest-file:" << tab << "destination file; stdout if none." << nl;
	fout << "command: " << nl;
	fout << tab << "<none>, -c, --convert:" << nl << tab << tab << "convert disc index in ASCII DB friend file." << nl;
	fout << tab << "--version:" << nl << tab << tab << "print version of " NAME " components." << nl;
	fout << tab << "--help:" << tab << "print this message." << nl;
	fout << "options:" << nl;
	fout << tab << "-a, --album-marker CHAR:" << nl << tab << tab << "CHAR is used to mark album ID ('" << DEFAULT_ALBUM_MARKER << "' by default)." << nl;
	fout << tab << "-f, --field-delimiter CHAR:" << nl << tab << tab << "CHAR is the delimiter between field ('\\t' by default)." << nl;
	fout << tab << "-l, --line-delimiter CHAR:" << nl << tab << tab << "CHAR is the delimiter between line ('\\n' by default)." << nl;
	fout << tab << "-n, --disc-name TEXT:" << nl << tab << tab << "TEXT becomes the disc name (none by default)." << nl;
	fout << tab << "-t, --title-marker CHAR:" << nl << tab << tab << "CHAR is used to mark title ID ('" << DEFAULT_TITLE_MARKER << "' by default)." << nl;
}

void PrintHeader( void )
{
	fout << NAME " V" VERSION " "__DATE__ " " __TIME__;
	fout << " by "AUTHOR " (" AUTHOR_EMAIL ")" << nl;
	fout << "Copyright the Epeios project (" EPSMSC_EPEIOS_URL "). " << nl;
//	fout << EPSMSC_GNU_TEXT << nl;
}

static void AnalyzeOptions(
	clnarg__analyzer_ &Analyzer,
	char &LineDelimiter,
	char &FieldDelimiter,
	char &AlbumMarker,
	char &TitleMarker,
	str_string_ &DiscName )
{
ERRProlog
	POSITION__ P;
	clnarg_option_list Options;
	clnarg__id Option;
	const char *Unknow = NULL;
	clnarg_argument Argument;
ERRBegin
	Options.Init();

	if ( ( Unknow = Analyzer.GetOptions( Options ) ) != NULL ) {
		ferr << '\'' << Unknow << "': unknow option." << nl;
		ferr << "Try '" NAME " --help' to get more informations." << nl;
		ERRt();
	}

	P = Options.First();

	while( P != NONE ) {
		Argument.Init();

		switch( Option = Options( P ) ) {
		case oLineDelimiter:
			Analyzer.GetArgument( Option, Argument );
			if( Argument.Amount() == 0 ) {
				ferr << "Option '-l,--line-delimiter' must have one argument." << nl;
				ERRt();
			}
			else if ( Argument.Amount() > 1 ) {
				ferr << "Argument of option '-l,--line-delimiter' must be a character." << nl;
				ERRt();
			}
			else
				LineDelimiter = Argument( 0 );
			break;
		case oFieldDelimiter:
			Analyzer.GetArgument( Option, Argument );
			if( Argument.Amount() == 0 ) {
				ferr << "Option '-f,--field-delimiter' must have one argument." << nl;
				ERRt();
			}
			else if ( Argument.Amount() > 1 ) {
				ferr << "Argument of option '-f,--field-delimiter' must be a character." << nl;
				ERRt();
			}
			else
				FieldDelimiter = Argument( 0 );
			break;
		case oAlbumMarker:
			Analyzer.GetArgument( Option, Argument );
			if( Argument.Amount() == 0 ) {
				ferr << "Option '-a,--album-marker' must have one argument." << nl;
				ERRt();
			}
			else if ( Argument.Amount() > 1 ) {
				ferr << "Argument of option '-a,--album-marker' must be a character." << nl;
				ERRt();
			}
			else
				AlbumMarker = Argument( 0 );
		case oTitleMarker:
			Analyzer.GetArgument( Option, Argument );
			if( Argument.Amount() == 0 ) {
				ferr << "Option '-t,--title-marker' must have one argument." << nl;
				ERRt();
			}
			else if ( Argument.Amount() > 1 ) {
				ferr << "Argument of option '-t,--title-marker' must be a character." << nl;
				ERRt();
			}
			else
				TitleMarker = Argument( 0 );
			break;
		case oDiscName:
			Analyzer.GetArgument( Option, Argument );
			if( Argument.Amount() == 0 ) {
				ferr << "Option '-n,--disc-name' must have one argument." << nl;
				ERRt();
			}
			else
				DiscName = Argument;
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
	clnarg__analyzer_ &Analyzer,
	char *&Source,
	char *&DiscID,
	char *&Dest )
{
ERRProlog
	clnarg_arguments Free;
	POSITION__ P;
ERRBegin
	Free.Init();

	Analyzer.GetArguments( Free );

	P = Free.Last();

	switch( Free.Amount() ) {
	case 3:
		Dest = Free( P ).Convert();
		P = Free.Previous( P );
	case 2:
		DiscID = Free( P ).Convert();
		P = Free.Previous( P );
		Source = Free( P ).Convert();
		P = Free.Previous( P );
		break;
	case 1:
	case 0:
		ferr << "Too few arguments." << nl;
		fout << "Try '" NAME " --help' to get more informations." << nl;
		ERRt();
		break;
	default:
		ferr << "Too many arguments." << nl;
		fout << "Try '" NAME " --help' to get more informations." << nl;
		ERRt();
		break;
	}

ERRErr
ERREnd
ERREpilog
}

static void AnalyzeArgs(
	int argc,
	const char *argv[],
	char *&Source,
	char *&DiscID,
	char *&Dest,
	char &LineDelimiter,
	char &FieldDelimiter,
	char &AlbumMarker,
	char &TitleMarker,
	str_string_ &DiscName )
{
ERRProlog
	clnarg_description Description;
	clnarg__analyzer_ Analyzer;
ERRBegin
	Description.Init();

	Description.AddOption( 'f', "field-delimiter", oFieldDelimiter );
	Description.AddOption( 'l', "line-delimiter", oLineDelimiter );
	Description.AddOption( 'a', "album-marker", oAlbumMarker );
	Description.AddOption( 't', "title-marker", oTitleMarker );
	Description.AddOption( 'n', "disc-name", oDiscName );
	Description.AddCommand( 'c', "convert", cConvert );
	Description.AddCommand( CLNARG_NO_SHORT, "version", cVersion );
	Description.AddCommand( CLNARG_NO_SHORT, "help", cHelp );

	Analyzer.Init( argc, argv, Description );

	switch ( Analyzer.GetCommand() ) {
	case cVersion:
		PrintHeader();
		TTR.Advertise();
		ERRt();
		break;
	case cHelp:
		PrintUsage();
		ERRt();
		break;
	case cConvert:
	case CLNARG_NONE:
		break;
	default:
		ERRc();
	}

	AnalyzeOptions( Analyzer, LineDelimiter, FieldDelimiter, AlbumMarker, TitleMarker, DiscName );

	AnalyzeFreeArguments( Analyzer, Source, DiscID, Dest );

ERRErr
ERREnd
ERREpilog
}

static inline void Main(
	int argc,
	const char *argv[] )
{
ERRProlog
	fil::file_oflow___ OFile;
	txf__text_oflow_ OText;
	txf__text_oflow_ *OFlow = NULL;
	bso__bool Backup = false;
	char *Source = NULL;
	char *DiscID = NULL;
	char *Dest = NULL;
	char LineDelimiter = DEFAULT_LINE_DELIMITER;
	char FieldDelimiter = DEFAULT_FIELD_DELIMITER;
	char AlbumMarker = DEFAULT_ALBUM_MARKER;
	char TitleMarker = DEFAULT_TITLE_MARKER;
	str_string DiscName;
ERRBegin
	DiscName.Init();

	AnalyzeArgs( argc, argv, Source, DiscID, Dest, LineDelimiter, FieldDelimiter, AlbumMarker, TitleMarker, DiscName );

	if ( Dest ) {

		TOLCreateBackupFile( Dest, tol::hbfRename );
		Backup = true;

		if ( OFile.Init( Dest, err::hSkip ) != fil::sSuccess )
		{
			ferr << "Error while opening '" << Dest << "' for writing." << nl;
			ERRt();
		}

		OText.Init( OFile );

		OFlow = &OText;
	}
	else
		OFlow = &fout;

	Go( Source, *OFlow, DiscID, LineDelimiter, FieldDelimiter, AlbumMarker, TitleMarker, DiscName );

ERRErr
	OText.reset();
	OFile.reset();

	if ( Backup )
		TOLRecoverBackupFile( Dest );
ERREnd
	if ( Source != NULL )
		free( Source );
		
	if ( Dest != NULL )
		free( Dest );
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
	
	if ( ERRMajor == err::thw )
		ERRRst();

ERRFEnd
ERRFEpilog
}
