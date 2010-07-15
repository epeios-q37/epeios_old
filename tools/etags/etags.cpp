/*
	'tags' by Claude SIMON(csimon@zeusw.org)
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
#include "txmtbl.h"
#include "xml.h"

#define NAME			"etags"
#define VERSION			"0.1.0"
#define COPYRIGHT_YEARS	"2008"
#define DESCRIPTION		"FLAC file tagging utility"
#define INFO			EPSMSC_EPEIOS_AFFILIATION
#define AUTHOR_NAME		EPSMSC_AUTHOR_NAME
#define AUTHOR_CONTACT	EPSMSC_AUTHOR_CONTACT
#define HELP			EPSMSC_HELP_INVITATION( NAME )
#define COPYRIGHT		"Copyright (c) " COPYRIGHT_YEARS " " AUTHOR_NAME " (" AUTHOR_CONTACT ")."
#define CVS_DETAILS		("$Id$\b " + 5)

using cio::cin;
using cio::cout;
using cio::cerr;

using txmtbl::table_;
using txmtbl::table;
using txmtbl::line_ ;
using txmtbl::line;
using txmtbl::cell_;
using txmtbl::cell;

using xml::writer_;
using xml::browser___;

#define FILES_TAG			"Files"
#define FILE_NAME_ATTRIBUTE	"Name"
#define FILE_TAG			"File"
#define TAGS_TAG			"Tags"
#define TAG_TAG				"Tag"
#define TAG_NAME_ATTRIBUTE	"Name"
#define RAW_VALUE_TAG		"Value"
#define HANDLED_VALUE_TAG	"HandledValue"


/* Beginning of the part which handles command line arguments. */

enum command {
	cHelp,
	cVersion,
	cLicense,
	cGet,
	cSet

};

enum option {
	// o
};

struct parameters {
	const char *XMLFile;
	STR_BUFFER___ XMLLFileBuffer;
	int Command;
	parameters( void )
	{
		XMLFile = NULL;
		Command = CLNARG_NONE;
	}
};

void PrintUsage( const clnarg::description_ &Description )
{
	cout << DESCRIPTION << txf::nl;
	cout << NAME << " --version|--license|--help" << txf::nl;
	clnarg::PrintCommandUsage( Description, cVersion, "print version of " NAME " components.", clnarg::vSplit, false );
	clnarg::PrintCommandUsage( Description, cLicense, "print the license.", clnarg::vSplit, false );
	clnarg::PrintCommandUsage( Description, cHelp, "print this message.", clnarg::vOneLine, false );
	cout << NAME << " command [options] [tags]" << txf::nl;
	// Free argument description.
	cout << "command:" << txf::nl;
	clnarg::PrintCommandUsage( Description, cGet, "get tags from '*.otags' files in current dir and write it in 'tags'", clnarg::vSplit, true );
	clnarg::PrintCommandUsage( Description, cSet, "set tags from 'tags' in '*.mtags' files from current dir", clnarg::vSplit, true );
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
		Parameters.XMLFile = Free( P ).Convert( Parameters.XMLLFileBuffer);
		break;
	case 0:
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
	Description.AddCommand( CLNARG_NO_SHORT, "get", cGet );
	Description.AddCommand( CLNARG_NO_SHORT, "set", cSet );

	Analyzer.Init( argc, argv, Description );

	switch ( Parameters.Command = Analyzer.GetCommand() ) {
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
	case cSet:
	case cGet:
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

static void Fill_(
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

template <typename row__> static void Build_(
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

static void Build_(
	const path_ &Path,
	str::string_ &String )
{
	Build_<stk::row__>( Path, String );
}

static void Build_(
	const file_ &File,
	str::string_ &String )
{
	Build_<epeios::row__>( File, String );
}

static void GetFiles_(
	const char *RootDir,
	path_ &Path,
	files_ &Files )
{
ERRProlog
	str::string LocalizedFileName;
	dir::handle___ Handle = DIR_INVALID_HANDLE;
	const char *Name;
	STR_BUFFER___ Buffer;
	file File;
ERRBegin
	LocalizedFileName.Init( RootDir );

	Build_( Path, LocalizedFileName );

	Name = dir::GetFirstFile( LocalizedFileName.Convert( Buffer ), Handle );

	if ( Name == NULL ) {
		cerr << "Unable to parse directory '" << Buffer << "' !" << txf::nl;
		ERRExit( EXIT_FAILURE );
	}

	while ( ( Name != NULL ) && *Name ) {

		if ( Name[strlen( Name )-1] != '.' ) {
			LocalizedFileName.Init( RootDir );

			Build_( Path, LocalizedFileName );

			if ( LocalizedFileName.Amount() != 0 )
				LocalizedFileName.Append( FNM_DIRECTORY_SEPARATOR_CHARACTER );

			LocalizedFileName.Append( Name );

			if ( fil::IsDirectory( LocalizedFileName.Convert( Buffer ) ) ) {
				Path.Push( str::string( Name ) );
				GetFiles_( RootDir, Path, Files );
				Path.Pop();
			} else if ( fil::IsFile( Buffer ) ) {
				File.Init();

				Fill_( Path, File );

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


static void GetFiles_(
	const char *RootDir,
	files_ &Files )
{
ERRProlog
	path Path;
ERRBegin
	Path.Init();

	GetFiles_( RootDir, Path, Files );
ERRErr
ERREnd
ERREpilog
}

class tag_ {
public:
	struct s {
		str::string_::s Name, Value;
	};
	str::string_ Name, Value;
	tag_( s &S )
	: Name( S.Name ),
	  Value( S.Value )
	{}
	void reset( bso::bool__ P = true )
	{
		Name.reset( P );
		Value.reset( P );
	}
	void plug( mmm::E_MULTIMEMORY_ &MM )
	{
		Name.plug( MM );
		Value.plug( MM );
	}
	tag_ &operator =( const tag_ &T )
	{
		Name = T.Name;
		Value = T.Value;

		return *this;
	}
	void Init( void )
	{
		reset();

		Name.Init();
		Value.Init();
	}
};
E_AUTO( tag );

typedef ctn::E_XCONTAINER_( tag_ ) tags_;
E_AUTO( tags )

class file_tags_
{
public:
	struct s {
		str::string_::s File;
		tags_::s Tags;
	};
	str::string_ File;
	tags_ Tags;
	file_tags_( s &S )
	: File( S.File ),
	  Tags( S.Tags )
	{}
	void reset( bso::bool__ P = true )
	{
		File.reset( P );
		Tags.reset( P );
	}
	void plug( mmm::E_MULTIMEMORY_ &MM )
	{
		File.plug( MM );
		Tags.plug( MM );
	}
	file_tags_ &operator =( const file_tags_ &FT )
	{
		File = FT.File;
		Tags = FT.Tags;

		return *this;
	}
	void Init( void )
	{
		reset();

		File.Init();
		Tags.Init();
	}
};

E_AUTO( file_tags )

typedef ctn::E_XCONTAINER_( file_tags_ ) files_tags_;
E_AUTO( files_tags )

static void GetTable_(
	const str::string_ &FileName,
	table_ &Table )
{
ERRProlog
	flf::file_iflow___ IFlow;
	xtf::extended_text_iflow__ XFlow;
	STR_BUFFER___ Buffer;
ERRBegin
	if ( IFlow.Init( FileName.Convert( Buffer ), err::hSkip ) != fil::sSuccess ) {
		cerr << "Unable to open '" << FileName << "' !" ;
		ERRExit( EXIT_FAILURE );
	}

	IFlow.EOFD( XTF_EOXT );

	XFlow.Init( IFlow );

	txmtbl::GetTable( XFlow, Table, '=' );

	Table.RemoveEmptyLines();
ERRErr
ERREnd
ERREnd
}

static void ExtractTag_(
	const line_ &Line,
	tag_ &Tag )
{
	ctn::E_CMITEM( cell_ ) Cell;
	epeios::row__ Row = Line.First();

	Cell.Init( Line );

	if ( Row != NONE )
		Tag.Name = Cell( Row );
	else
		ERRc();

	Row = Line.Next( Row );

	if ( Row != NONE ) {
		Tag.Value = Cell( Row );

		Row = Line.Next( Row );
	}

	while ( Row != NONE ) {
		Tag.Value.Append( '=' );

		Tag.Value.Append( Cell( Row ) );

		Row = Line.Next( Row );
	}
}

static void ExtractTags_(
	const table_ &Table,
	tags_ &Tags )
{
ERRProlog
	tag Tag;
	epeios::row__ Row = NONE;
	ctn::E_CITEM( line_ ) Line;
ERRBegin
	Line.Init( Table );
	Row = Table.First();

	while ( Row != NONE ) {
		Tag.Init();

		ExtractTag_( Line( Row ), Tag );

		Tags.Append( Tag );

		Row = Table.Next( Row );
	}
ERRErr
ERREnd
ERREpilog
}

static void Get_(
	const str::string_ &File,
	bso::size__ AmountToTruncate,
	file_tags_ &FileTags )
{
ERRProlog
	table Table;
ERRBegin
	FileTags.File = File;

	FileTags.File.Truncate( File.Last( AmountToTruncate ) );

	Table.Init();
	GetTable_( File, Table );

	ExtractTags_( Table, FileTags.Tags );
ERRErr
ERREnd
ERREpilog
}

static void Get_(
	const str::string_ &File,
	const str::string_ &Pattern,
	files_tags_ &FilesTags )
{
ERRProlog
	str::string CommandLine;
	STR_BUFFER___ Buffer;
	file_tags FileTags;
ERRBegin
	if ( ( File.Amount() > Pattern.Amount() )
		  && ( File.Search( Pattern, File.Last( Pattern.Amount() ) ) != NONE ) )
	{
//		cout << File << txf::nl;

		FileTags.Init();

		Get_( File, Pattern.Amount() - 1, FileTags );

		FilesTags.Append( FileTags );
	}
ERRErr
ERREnd
ERREpilog
}

static void Get_(
	const files_ &Files,
	files_tags_ &FilesTags )
{
ERRProlog
	ctn::E_CITEM( file_ ) File;
	epeios::row__ Row = NONE;
	str::string Path;
ERRBegin
	Row = Files.First();
	File.Init( Files );

	while ( Row != NONE ) {
		Path.Init();

		Build_( File( Row ), Path );
		Get_( Path, str::string( ".otags" ), FilesTags );

		Row = Files.Next( Row );
	}
ERRErr
ERREnd
ERREpilog
}

static void Dump_(
	const tag_ &Tag,
	writer_ &Writer )
{
	Writer.PushTag( TAG_TAG );

	Writer.PutAttribute( TAG_NAME_ATTRIBUTE, Tag.Name );

	Writer.PushTag( RAW_VALUE_TAG );
	Writer.PutValue( Tag.Value );
	Writer.PopTag();

	if ( Tag.Name == "replaygain_track_gain" ) {
		bso::float_buffer__ Buffer;
		epeios::row__ Error = NONE;

		Writer.PushTag( HANDLED_VALUE_TAG );
		Writer.PutValue( bso::Convert( Tag.Value.ToLF( &Error ), Buffer ) );
		Writer.PopTag();

		if ( Error == NONE )
			ERRc();

		if ( Tag.Value( Error ) != ' ' )
			ERRc();
	}

	Writer.PopTag();
}

static void Dump_(
	const tags_ &Tags,
	writer_ &Writer )
{
	ctn::E_CITEM( tag_ ) Tag;
	epeios::row__ Row = Tags.First();

	Tag.Init( Tags );

	while ( Row != NONE ) {
		Dump_( Tag( Row ), Writer );

		Row = Tags.Next( Row );
	}
}

static void Dump_(
	const file_tags_ &FileTags,
	writer_ &Writer )
{
	Writer.PushTag( FILE_TAG );

	Writer.PutAttribute( FILE_NAME_ATTRIBUTE, FileTags.File );

	Writer.PushTag( TAGS_TAG );

	Dump_( FileTags.Tags, Writer );

	Writer.PopTag();

	Writer.PopTag();
}

static void Dump_(
	const files_tags_ &FilesTags,
	writer_ &Writer )
{
	ctn::E_CITEM( file_tags_ ) FileTags;
	epeios::row__ Row = FilesTags.First();

	FileTags.Init( FilesTags );

	while ( Row != NONE ) {
		Dump_( FileTags( Row ), Writer );

		Row = FilesTags.Next( Row );
	}
}

static void Dump_(
	const files_tags_ &FileTags,
	const char *XMLFile )
{
ERRProlog
	xml::writer Writer;
	flf::file_oflow___ Flow;
	txf::text_oflow__ TFlow( Flow );
	bso::bool__ BackupFileCreated;
ERRBegin
	if ( XMLFile == NULL )  {
		xml::WriteXMLHeader( cout );
		cout << txf::nl;
		Writer.Init( cout, xml::oIndent );
	}else {
		fil::CreateBackupFile( XMLFile, fil::hbfRename );
		BackupFileCreated = true;

		if ( Flow.Init( XMLFile, err::hSkip ) != fil::sSuccess ) {
			cerr << "Unable to open '" << XMLFile << "' for writing !" << txf::nl;
			ERRExit( EXIT_FAILURE );
		}

		xml::WriteXMLHeader( TFlow );
		TFlow << txf::nl;
		Writer.Init( TFlow, xml::oIndent );
	}

	Writer.PushTag( FILES_TAG );

	Dump_( FileTags, Writer );

	Writer.PopTag();
ERRErr
	if ( BackupFileCreated )  {
		Writer.reset();
//		TFlow.reset();
		Flow.reset();
		fil::RecoverBackupFile( XMLFile );
	}
ERREnd
ERREpilog
}

static void Get_( const char *XMLFile )
{
ERRProlog
	files Files;
	files_tags FilesTags;
ERRBegin
	Files.Init();

	GetFiles_( ".", Files );

/*	Display( Files );

	cout << "--------------------" << txf::nl;

	cout << MinSize << txf::tab << MaxSize << txf::tab << TotalSize << txf::nl;
*/
//	RandomCopy( Source, Files, Dest, MinSize, MaxSize, TotalSize );

	FilesTags.Init();
	Get_( Files, FilesTags );
	Dump_( FilesTags, XMLFile );
ERRErr
ERREnd
ERREpilog
}

/*
	while ( Continue ) {
		switch ( Browser.Browse() ) {
		case xml::tProcessingInstruction:
			break;
		case xml::tStartTag:
			break;
		case xml::tStartTagClosed:
			break;
		case xml::tAttribute:
			break;
		case xml::tValue:
			break;
		case xml::tEndTag:
			Continue = false;
			break;
		case xml::tProcessed:
			ERRc();
			break;
		case xml::tError:
			Error_( Browser, xml::GetLabel( Browser.Status() ) );
			break;
		default:
			ERRc();
			break;
		}
	}
*/

static void Error_(
	browser___ &Browser,
	const char *Message )
{
	cerr << "Error in XML file line " << Browser.DumpCoord().Line << ", column " << Browser.DumpCoord().Column << " : " << Message << " !" << txf::nl;
	ERRExit( EXIT_FAILURE );
}

static void RetrieveTagValue_(
	browser___ &Browser,
	str::string_ &Value )
{
	bso::bool__ Continue = true;

	while ( Continue ) {
		switch ( Browser.Browse() ) {
		case xml::tProcessingInstruction:
			break;
		case xml::tStartTag:
			break;
		case xml::tStartTagClosed:
			break;
		case xml::tAttribute:
			break;
		case xml::tValue:
			Value = Browser.Value();
			break;
		case xml::tEndTag:
			Continue = false;
			break;
		case xml::tProcessed:
			ERRc();
			break;
		case xml::tError:
			Error_( Browser, xml::GetLabel( Browser.Status() ) );
			break;
		default:
			ERRc();
			break;
		}
	}
}

static void IgnoreTagValue_( browser___ &Browser )
{
ERRProlog
	str::string Dummy;
ERRBegin
	Dummy.Init();

	RetrieveTagValue_( Browser, Dummy );
ERRErr
ERREnd
ERREpilog
}


static void RetrieveTag_(
	browser___ &Browser,
	tag_ &Tag )
{
	bso::bool__ Continue = true;

	while ( Continue ) {
		switch ( Browser.Browse() ) {
		case xml::tProcessingInstruction:
			break;
		case xml::tStartTag:
			if ( Tag.Name.Amount() == 0 )
				Error_( Browser, "Missing attribute" );

			if ( Browser.TagName() == HANDLED_VALUE_TAG ) {
				IgnoreTagValue_( Browser );
			} else if ( Browser.TagName() == RAW_VALUE_TAG ) {
				RetrieveTagValue_( Browser, Tag.Value );
			} else 
				Error_( Browser, "Bad tag" );
			break;
		case xml::tStartTagClosed:
			break;
		case xml::tAttribute:
			if ( Browser.AttributeName() != TAG_NAME_ATTRIBUTE )
				Error_( Browser, "Bad attribute" );

			Tag.Name = Browser.Value();
			break;
		case xml::tValue:
			break;
		case xml::tEndTag:
			Continue = false;
			break;
		case xml::tProcessed:
			ERRc();
			break;
		case xml::tError:
			Error_( Browser, xml::GetLabel( Browser.Status() ) );
			break;
		default:
			ERRc();
			break;
		}
	}
}

static void RetrieveTags_(
	browser___ &Browser,
	tags_ &Tags )
{
ERRProlog
	tag Tag;
	bso::bool__ Continue = true;
ERRBegin
	while ( Continue ) {
		switch ( Browser.Browse() ) {
		case xml::tProcessingInstruction:
			break;
		case xml::tStartTag:
			if ( Browser.TagName() != TAG_TAG )
				Error_( Browser, "Bad tag" );

			Tag.Init();
			RetrieveTag_( Browser, Tag );

			if ( Tag.Name.Amount() != 0 )
				Tags.Append( Tag );
			break;
		case xml::tStartTagClosed:
			break;
		case xml::tAttribute:
			break;
		case xml::tValue:
			break;
		case xml::tEndTag:
			Continue = false;
			break;
		case xml::tProcessed:
			ERRc();
			break;
		case xml::tError:
			Error_( Browser, xml::GetLabel( Browser.Status() ) );
			break;
		default:
			ERRc();
			break;
		}
	}
ERRErr
ERREnd
ERREpilog
}

static void RetrieveFileTags_(
	browser___ &Browser,
	file_tags_ &FileTags )
{
ERRProlog
	bso::bool__ Continue = true;
ERRBegin
	while ( Continue ) {
		switch ( Browser.Browse() ) {
		case xml::tProcessingInstruction:
			break;
		case xml::tStartTag:
			if ( Browser.TagName() != TAGS_TAG )
				Error_( Browser, "Bad tag" );

			if ( FileTags.File.Amount() == 0 )
				Error_( Browser, "Missing attribute" );

			RetrieveTags_( Browser, FileTags.Tags );

			break;
		case xml::tStartTagClosed:
			break;
		case xml::tAttribute:
			if ( Browser.AttributeName() != FILE_NAME_ATTRIBUTE )
				Error_( Browser, "Bad attribute" );

			FileTags.File = Browser.Value();
			break;
		case xml::tValue:
			break;
		case xml::tEndTag:
			Continue = false;
			break;
		case xml::tProcessed:
			ERRc();
			break;
		case xml::tError:
			Error_( Browser, xml::GetLabel( Browser.Status() ) );
			break;
		default:
			ERRc();
			break;
		}
	}
ERRErr
ERREnd
ERREpilog
}

static void RetrieveFilesTags_(
	browser___ &Browser,
	files_tags_ &FilesTags )
{
ERRProlog
	file_tags FileTags;
	bso::bool__ Continue = true;
ERRBegin
	while ( Continue ) {
		switch ( Browser.Browse() ) {
		case xml::tProcessingInstruction:
			break;
		case xml::tStartTag:
			if ( Browser.TagName() != FILE_TAG )
				Error_( Browser, "Bad tag" );

			FileTags.Init();
			RetrieveFileTags_( Browser, FileTags );
			FilesTags.Append( FileTags );
			break;
		case xml::tStartTagClosed:
			break;
		case xml::tAttribute:
			break;
		case xml::tValue:
			break;
		case xml::tEndTag:
			Continue = false;
			break;
		case xml::tProcessed:
			ERRc();
			break;
		case xml::tError:
			Error_( Browser, xml::GetLabel( Browser.Status() ) );
			break;
		default:
			ERRc();
			break;
		}
	}
ERRErr
ERREnd
ERREpilog
}

	

static void Retrieve_(
	browser___ &Browser,
	files_tags_ &FilesTags )
{
	bso::bool__ Continue = true;

	while ( Continue ) {
		switch ( Browser.Browse() ) {
		case xml::tProcessingInstruction:
			break;
		case xml::tStartTag:
			if ( Browser.TagName() == FILES_TAG )
				RetrieveFilesTags_( Browser, FilesTags );
			break;
		case xml::tStartTagClosed:
			break;
		case xml::tAttribute:
			break;
		case xml::tValue:
			break;
		case xml::tEndTag:
			break;
		case xml::tProcessed:
			Continue = false;
			break;
		case xml::tError:
			Error_( Browser, xml::GetLabel( Browser.Status() ) );
			break;
		default:
			ERRc();
			break;
		}
	}
}

static void Retrieve_(
	xtf::extended_text_iflow__ &XFlow,
	files_tags_ &FilesTags )
{
ERRProlog
	browser___ Browser;
	bso::bool__ Continue = true;
ERRBegin
	Browser.Init( XFlow, xml::eh_Default );

	Retrieve_( Browser, FilesTags );
ERRErr
ERREnd
ERREpilog
}

static void Retrieve_(
	const char *XMLFile,
	files_tags_ &FilesTags )
{
ERRProlog
	flf::file_iflow___ Flow;
	xtf::extended_text_iflow__ XFlow;
ERRBegin
	if ( XMLFile == NULL )  {
		cio::cinf.EOFD( XTF_EOXT );
		XFlow.Init( cio::cinf );
	}else {
		if ( Flow.Init( XMLFile, err::hSkip ) != fil::sSuccess ) {
			cerr << "Unable to open '" << XMLFile << "' for reading !" << txf::nl;
			ERRExit( EXIT_FAILURE );
		}

		Flow.EOFD( XTF_EOXT );
		XFlow.Init( Flow );
	}

	Retrieve_( XFlow, FilesTags );
ERRErr
ERREnd
ERREpilog
}

static void Set_(
	const tag_ &Tag,
	txf::text_oflow__ &TFlow )
{
	TFlow << Tag.Name;
	TFlow << '=';
	TFlow << Tag.Value;
}

static void Set_(
	const tags_ &Tags,
	txf::text_oflow__ &TFlow )
{
	ctn::E_CITEM( tag_ ) Tag;
	epeios::row__ Row = Tags.First();
	Tag.Init( Tags );

	while ( Row != NONE ) {
		Set_( Tag( Row ), TFlow );

		TFlow << txf::nl;

		Row = Tags.Next( Row );
	}
}

static void Set_(
	const tags_ &Tags,
	const char *FileName )
{
ERRProlog
	flf::file_oflow___ Flow;
	txf::text_oflow__ TFlow( Flow );
	bso::bool__ BackupCreated = false;
ERRBegin
	fil::CreateBackupFile( FileName, fil::hbfRename );
	BackupCreated = true;

	if ( Flow.Init( FileName, err::hSkip ) != fil::sSuccess ) {
		cerr << "Unable to open '" << FileName << "' for writing !" << txf::nl;
		ERRExit( EXIT_FAILURE );
	}

	Set_( Tags, TFlow );
ERRErr
	if ( BackupCreated )  {
//		TFlow.reset();
		Flow.reset();
		fil::RecoverBackupFile( FileName );
	}
ERREnd
ERREpilog
}

static void Set_(
	const tags_ &Tags,
	const str::string_ &File )
{
ERRProlog
	str::string FileName;
	STR_BUFFER___ Buffer;
ERRBegin
	FileName.Init( File );
	FileName.Append( ".mtags" );

	Set_( Tags, FileName.Convert( Buffer ) );
ERRErr
ERREnd
ERREpilog
}

static void Set_( const file_tags_ &FileTags )
{
	Set_( FileTags.Tags, FileTags.File );
}

static void Set_( const files_tags_ &FilesTags )
{
	ctn::E_CITEM( file_tags_ ) FileTags;
	epeios::row__ Row = FilesTags.First();

	FileTags.Init( FilesTags );

	while ( Row != NONE ) {
		Set_( FileTags( Row ) );

		Row = FilesTags.Next( Row );
	}
}

static void Set_( const char *XMLFile )
{
ERRProlog
	files_tags FilesTags;
ERRBegin
	FilesTags.Init();

	Retrieve_( XMLFile, FilesTags );

	Set_( FilesTags );
ERRErr
ERREnd
ERREpilog
}

static void Go_( const parameters &Parameters )
{
ERRProlog
ERRBegin
	switch ( Parameters.Command ) {
	case cGet:
		Get_( Parameters.XMLFile );
		break;
	case cSet:
		Set_( Parameters.XMLFile );
		break;
	case CLNARG_NONE:
		cerr << "Missing command !" << txf::nl;
		cout << HELP << txf::nl;
		ERRExit( EXIT_FAILURE );
		break;
	default:
		ERRc();
		break;
}
ERRErr
ERREnd
ERREpilog
}

static inline void Main_(
	int argc,
	const char *argv[] )
{
ERRProlog
	parameters Parameters;
ERRBegin
	AnalyzeArgs( argc, argv, Parameters );

	Go_( Parameters );
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
	Main_( argc, argv );
ERRFErr
ERRFEnd
ERRFEpilog
	return ERRExitValue;
}
