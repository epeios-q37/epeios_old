/* Part of the Epeios Project */
/* See http://www.epeios.org/ for more details. */

/*
	V1.1.4 (23/05/2000):	Adapting for BCC V5.5. Adapting to ERR V2.0.0.
	V1.1.3 (20/04/2000):	Fixing bug in memory freeing.
	V1.1.2 (18/12/1999):	Some little modification on options/command name.
	V1.1.0 (23/10/1999):	GNUification and adding external hyperlink."
	V? (?):					First release.
*/

#include "tol.h"
#include "gnumll.h"
#include "fil.h"
#include "tagdtc.h"
#include "clnarg.h"
#include "epsmsc.h"

#define NAME			"mll2html"
#define VERSION			"V1.1.4"
#define AUTHOR_LINK		EPSMSC_AUTHOR_LINK
#define AUTHOR			EPSMSC_AUTHOR_NAME
#define AUTHOR_EMAIL	EPSMSC_AUTHOR_EMAIL

#define	DELIMITER	'$'

typedef txf::text_oflow___			oflow_;
typedef tagdtc::tag_detector_		detector_;
typedef tagdtc::tag_detector		detector;
typedef txmpgh::paragraph_			paragraph_;
typedef gnumll::head_section_		head_;
typedef gnumll::section_header_		header_;
typedef gnumll::general_section_	general_;
typedef gnumll::list_section_		list_;
typedef gnumll::general_sections_	generals_;
typedef gnumll::list_sections_		lists_;
typedef gnumll::list_sections_		lists_;
typedef gnumll::section_			section_;
typedef gnumll::head_section_		head_section_;
typedef gnumll::mailing_lists_		mailing_lists_;
typedef gnumll::mailing_lists		mailing_lists;
typedef gnumll::titles_				titles_;
typedef gnumll::misc_				misc_;
typedef str::string_				string_;
typedef str::string					string;

enum command {
	cHelp,
	cVersion,
	cConvert,
};

enum option {
	oListsFile,
	oGeneralFile,
	o_amount
};

static inline void WriteEmail(
	const string_ &S,
	oflow_ &Flow )
{
	Flow << "<A HREF=\"mailto:" << S << "\">" << S << "</A>";
}

static inline void WriteNews(
	const string_ &S,
	oflow_ &Flow )
{
	Flow << "<A HREF=\"news://" << S << "\">" << S << "</A>";
}

static inline void WriteURL(
	const string_ &S,
	oflow_ &Flow )
{
	Flow << "<A HREF=\"" << S << "\" target=_blank>" << S << "</A>";
}


static inline bso::bool__ IsEmailAdress( const string_ &S )
{
	epeios::row__ PAt, PPoint;
	bso::bool__ R = false;

	if ( ( PAt = S.Search( '@' ) ) != NONE )
		if ( ( PPoint = S.Search( '.' ) ) != NONE )
			if ( ( S.Search( '*' ) == NONE ) && ( PAt.V < PPoint.V ) )
				R = true;

	return R;
}

static inline bso::bool__ IsURL( const string_ &S )
{
	return S.Search( string( "://" ) ) != NONE;
}

static void Write(
	const string_ &S,
	oflow_ &F )
{
ERRProlog
	string T;
	epeios::row__ P = 0;
	epeios::size__ L = S.Amount();
	char C;
ERRBegin

	while( P.V < L) {
		T.Init();
		while( ( P.V < L ) && !isspace( C = ( S( P ) ) ) ) {
			if ( C == '<' )
				T.Add( "&lt;" );
			else if ( C == '>' )
				T.Add( "&gt;" );
			else
				T.Add( C );
			P.V++;
		}

		if ( IsEmailAdress( T ) ) { 
			if ( !isalnum( C = T( T.Amount() - 1 ) ) )
				T.Delete( T.Amount() - 1 );

			WriteEmail( T, F );

			if ( !isalnum( C ) )
				F << C;
		}
		else if ( IsURL( T ) ) {
			if ( ispunct( C = T( T.Amount() - 1 ) ) )
				T.Delete( T.Amount() - 1 );

			WriteURL( T, F );

			if ( ispunct( C ) )
				F << C;
		}
		else
			F << T;

		while( ( P.V < L ) && isspace( C = ( S( P ) ) ) ) {
			F << C;
			P.V++;
		}
	}
ERRErr
ERREnd
ERREpilog
}


static void Write(
	const paragraph_ &P,
	oflow_ &F )
{
	epeios::row__ Position = P.First();
	ctn::E_CMITEM( string_ ) Element;

	Element.Init( P );

	while ( Position != NONE ) {
		Write( Element( Position ), F );
		F << ' ';
		Position = P.Next( Position );
	}
}

static void WriteTitles(
	const paragraph_ &Paragraph,
	oflow_ &Flow )
{
	bso::ushort__ Size = 3;
	epeios::row__ P = Paragraph.First();
	ctn::E_CMITEM( string_ ) Element;

	Element.Init( Paragraph );

	while( P != NONE )
	{
		Flow << "<CENTER><H" << Size << '>';
		Write( Element( P ), Flow );
		Flow << "</H" << Size <<"></CENTER>" << txf::nl;
		Size++;
		P = Paragraph.Next( P );
	}
}

static void WriteSubTitles(
	const paragraph_ &Paragraph,
	oflow_ &Flow )
{
	epeios::row__ P = Paragraph.First();
	ctn::E_CMITEM( string_ ) Element;

	Element.Init( Paragraph );

	while( P != NONE )
	{
		Flow << "<CENTER>";
		Write( Element( P ), Flow );
		Flow << "</CENTER>" << txf::nl;
		P = Paragraph.Next( P );
	}
}

static void WriteHead(
	const head_section_ &Head,
	oflow_ &Flow )
{
	epeios::row__ P = Head.First();
	ctn::E_CITEM( paragraph_ ) Element;

	Element.Init( Head );

	if ( P != NONE ) {
		WriteTitles( Element( P ), Flow );

		P = Head.Next( P );

		while( P != NONE ) {
			WriteSubTitles( Element( P ), Flow );
			P = Head.Next( P );
		}
	}
}		

static void WriteTOCGenerals(
	const generals_ &Generals,
	const string_ &GeneralsFile,
	oflow_ &Flow )
{
	epeios::row__ P = Generals.First();
	ctn::E_CITEM( general_ ) General;

	General.Init( Generals );

	Flow << "<UL>";

	while( P != NONE ) {
		Flow << "<LI><A HREF=\"" << GeneralsFile << '#' << General( P ).Title << "\">" << General( P ).Title << "</A></LI>" << txf::nl;
		P = Generals.Next( P );
	}

	Flow << "</UL>";
}

static void WriteTOCListHeaderMisc(
	const misc_ &Misc,
	oflow_ &Flow )
{
	ctn::E_CMITEM( string_ ) I;
	epeios::row__ P = Misc.First();

	I.Init( Misc );

	while( P != NONE ) {
		Flow << "<li>" << I( P ) << "</li>" << txf::nl;
		P = Misc.Next( P );
	}
}

static inline void WriteTOCListHeader(
	const header_ &Header,
	oflow_ &Flow,
	bso::ulong__ Ref,
	const string_ &ListsFile )
{
	ctn::E_CMITEM( string_ ) Title;
	bso::bool__ Link = ( &ListsFile != NULL );

	Flow << "<LI>";

	if ( Link )
		Flow << "<A HREF=\"" << ListsFile << '#' << Ref << "\">";

	if ( Header.Name.Amount() )
		Flow << "<B>" << Header.Name << "</B>";
	else
		Flow << "<em>no name</em>";

	if ( Link )
		Flow << "</A>";

	Flow << ' ' << txf::nl;

	if ( Header.Email.Amount() ) {
			Flow << " (" ;
			WriteEmail( Header.Email, Flow );
			Flow << ')' << txf::nl;
	}


	Flow << "<ul>" << txf::nl;

	if ( Header.Newsgroup.Amount() )
	{
		Flow << "<li>gnUSENET newsgroup: ";

		WriteNews( Header.Newsgroup, Flow );

		if ( Header.NewsgroupComment.Amount() )
			Flow << ' ' << Header.NewsgroupComment;

		Flow << "</li>";
	} else if ( Header.NewsgroupComment.Amount() ) {
		Flow << "<li>gnUSENET newsgroup: ";

		Flow << Header.NewsgroupComment;

		Flow << "</li>";
	}

	if ( Header.OtherEmail.Amount() )
	{
		Flow << "<li>";

		if ( Header.TextOtherEmail.Amount() )
			Flow << Header.TextOtherEmail;

		WriteEmail( Header.OtherEmail, Flow );

		Flow << "</li>";
	}

	if ( Header.Contribution.Amount() )
	{
		Flow << "<li>Send contribution to: ";

		WriteEmail( Header.Contribution, Flow );

		Flow << "</li>";
	}

	if ( Header.Announcement.Amount() )
	{
		Flow << "<li>Send announcement to: ";

		WriteEmail( Header.Announcement, Flow );

		Flow << "</li>";
	}

	if ( Header.FAQ_URL.Amount() )
	{
		Flow << "<li>FAQ-URL: ";

		if ( IsURL( Header.FAQ_URL ) )
			WriteURL( Header.FAQ_URL, Flow );
		else
			Flow << Header.FAQ_URL;

		Flow << "</li>";
	}

	if ( Header.Misc.Amount() )
		WriteTOCListHeaderMisc( Header.Misc, Flow );

	Flow << "</ul></LI></A>";
}


static void WriteTOCLists(
	const lists_ &Lists,
	oflow_ &Flow,
	const string_ &ListsFile )
{
	epeios::row__ P = Lists.First();
	ctn::E_CITEM( list_ ) List;

	List.Init( Lists );

	while( P != NONE ) {
		Flow << "<UL>";
		WriteTOCListHeader( List( P ).Header, Flow, P.V, ListsFile );
		Flow << "</UL>" << txf::nl;
		P = Lists.Next( P );
	}
}



static inline bso::bool__ IsListItem( const string_ &Line )
{
	return ( ( Line.Amount() > 2 )
		      && ( Line( 0 ) == '\t' )
			  && ( Line( 1 ) == '-' ) );
}

static const string_ &AsListItem( const string_ &Line )
{
	static ::string String;

	String.Init();

	String.WriteAndAdjust( Line, Line.Amount() - 2, 2 );

	return String;
}


static void WriteGeneralParagraph(
	const paragraph_ &Paragraph,
	oflow_ &Flow )
{
	epeios::row__ P = Paragraph.First();
	ctn::E_CMITEM( string_ ) Line;
	bso::bool__ List = false;

	Line.Init( Paragraph );

	while ( P != NONE ) {
		if ( IsListItem( Line( P ) ) ) {
			if ( List )
				Flow << "</LI>" << txf::nl << "<LI>";
			else
				Flow << "<UL><LI>";

			Flow << AsListItem( Line( P ) );

			List = true;
		}
		else {
			Write( Line( P ), Flow );
			Flow << ' ' << txf::nl;
		}

		P = Paragraph.Next( P );
	}

	if ( List )
		Flow << "</UL>";

}


static void WriteGeneralSection(
	const section_ &Section,
	oflow_ &Flow )
{
	epeios::row__ P = Section.First();
	ctn::E_CITEM( paragraph_ ) Paragraph;

	Paragraph.Init( Section );

	while( P != NONE ) {
		WriteGeneralParagraph( Paragraph( P ), Flow );
		Flow << "<P>" << txf::nl;
		P = Section.Next( P );
	}
}


static inline void WriteGeneral( 
	const general_ &General,
	oflow_ &Flow )
{
	Flow << "<H3><A NAME=\"" << General.Title << "\">" << General.Title << "</H3>" << txf::nl;

	WriteGeneralSection( General.Section, Flow );
}

static void WriteGenerals(
	const generals_ &Generals,
	oflow_ &Flow )
{
	epeios::row__ P = Generals.First();
	ctn::E_CITEM( general_ ) General;

	General.Init( Generals );

	while( P != NONE ) {
		WriteGeneral( General( P ), Flow  );
		P = Generals.Next( P );
	}
}


static void WriteListParagraph(
	const paragraph_ &Paragraph,
	oflow_ &Flow )
{
	epeios::row__ P = Paragraph.First();
	ctn::E_MITEM( string_ ) Line;

	while ( P != NONE ) {
		Write( Line( P ), Flow );
		Flow << ' ' << txf::nl;
		P = Paragraph.Next( P );
	}
}

static void WriteListSection(
	const section_ &Section,
	oflow_ &Flow )
{
	epeios::row__ P = Section.First();
	ctn::E_CITEM( paragraph_ ) Paragraph;

	Paragraph.Init( Section );

	while( P != NONE ) {
		Write( Paragraph( P ), Flow );
		Flow << "<P>" << txf::nl;
		P = Section.Next( P );
	}
}

static inline void WriteListHeader(
	const header_ &Header,
	oflow_ &Flow,
	bso::ulong__ Ref )
{
	Flow << "<H4><A NAME=\"" << Ref << "\">";

	WriteTOCListHeader( Header, Flow, Ref, *(string_ *)NULL );

	Flow << "</H4>" << txf::nl;
}


static inline void WriteList( 
	const list_ &List,
	oflow_ &Flow,
	bso::ulong__ Ref )
{
	WriteListHeader( List.Header, Flow, Ref );

	WriteListSection( List.Section, Flow );
}

static void WriteLists(
	const lists_ &Lists,
	oflow_ &Flow )
{
	epeios::row__ P = Lists.First();
	ctn::E_CITEM( list_ ) List;

	List.Init( Lists );

	while( P != NONE ) {
		WriteList( List( P ), Flow, P.V  );
		P = Lists.Next( P );
	}
}



static void GetMailingLists(
	const char *Source,
	mailing_lists &MLL )
{
ERRProlog
	fil::file_iflow___ File;
	xtf::extended_text_iflow___ Flow;
ERRBegin
	if ( !File.Init( Source, err::hSkip ) )
	{
		ferr << "Error while opening '" << Source << "' for reading." << txf::nl;
		ERRu();
	}

	File.EOFT( XTF_EOXT );
	Flow.Init( File );

	MLL.Init();
	MLL.Parse( Flow );
ERRErr
ERREnd
ERREpilog
}

enum {
	tHead,
	tGeneralTOC,
	tListsTOC,
	tGeneralBody,
	tListsBody,
	tDelimiter,
	tCopyright,
	tUpdate,
	tName,
	tAuthor,
	tEnd,
};

static inline void FillDetector( detector_ &Detector )
{
	Detector.Add( "HEAD", tHead );
	Detector.Add( "GENERAL_TOC", tGeneralTOC );
	Detector.Add( "LISTS_TOC", tListsTOC );
	Detector.Add( "GENERAL_BODY", tGeneralBody );
	Detector.Add( "LISTS_BODY", tListsBody );
	Detector.Add( "", tDelimiter );
	Detector.Add( "COPYRIGHT", tCopyright );
	Detector.Add( "UPDATE", tUpdate );
	Detector.Add( "NAME", tName );
	Detector.Add( "AUTHOR", tAuthor );
	Detector.Add( "_END_", tEnd );
//	Detector.Add( "", t );
}

static void Go(
	const char *Source,
	const char *Template,
	const string_ &GeneralsFile,
	const string_ &ListsFile,
	oflow_ &Dest )
{
ERRProlog
	fil::file_iflow___ File;
	xtf::extended_text_iflow___ Flow;
	mailing_lists MLL;
	detector Detector;
	bso::bool__ Cont = true;
ERRBegin
	if ( !File.Init( Template, err::hSkip ) )
	{
		ferr << "Error while opening '" << Template << "' for reading." << txf::nl;
		ERRu();
	}

	File.EOFT( XTF_EOXT );
	Flow.Init( File );

	MLL.Init();
	GetMailingLists( Source, MLL );

	Detector.Init();

	FillDetector( Detector );

	while( Cont ) {
		switch( Detector.Parse( Flow, Dest, DELIMITER ) ) {
		case tHead:
			WriteHead( MLL.Head, Dest );
			break;
		case tGeneralTOC:
			WriteTOCGenerals( MLL.Generals, GeneralsFile, Dest );
			break;
		case tListsTOC:
			WriteTOCLists( MLL.Lists, Dest, ListsFile );
			break;
		case tGeneralBody:
			WriteGenerals( MLL.Generals, Dest );
			break;
		case tListsBody:
			WriteLists( MLL.Lists, Dest );
			break;
		case tDelimiter:
			Dest << DELIMITER;
			break;
		case tCopyright:
			Dest << "Copyright © 1999 the " EPSMSC_EPEIOS_LINK;
			break;
		case tUpdate:
			Dest << TOLDateAndTime();
			break;
		case tName:
			Dest << EPSMSC_APP_LINK( NAME ) " " VERSION;
			break;
		case tAuthor:
			Dest << AUTHOR_LINK;
			break;
		case TAGDTC_EOF:
		case tEnd:
			Cont = false;
			break;
		default:
			ERRu();
			break;
		}
	}
ERRErr
ERREnd
ERREpilog
}

void PrintUsage( void )
{
	fout << "Usage: " << NAME << " [command] [options] source-file template-file [dest-file]" << txf::nl;
	fout << "source-file:" << txf::tab << "mailinglists source file." << txf::nl;
	fout << "template-file:" << txf::tab << "HTML template source file." << txf::nl;
	fout << "dest-file:" << txf::tab << "destination file; stdout if none." << txf::nl;
	fout << "Command: " << txf::nl;
	fout << txf::tab << "<none>, -c, --convert" << txf::nl << txf::tab << txf::tab << "convert mailinglists to HTML file." << txf::nl;
	fout << txf::tab << "--version" << txf::nl << txf::tab << txf::tab << "print version of " NAME " components." << txf::nl;
	fout << txf::tab << "--help" << txf::tab << "print this message." << txf::nl;
	fout << "Options:" << txf::nl;
	fout << txf::tab << "-l, --lists-files FILE" << txf::nl << txf::tab << txf::tab << "name of the lists file for lists TOC links." << txf::nl;
	fout << txf::tab << "-g, --general-files FILE" << txf::nl << txf::tab << txf::tab << "name of the general file for general TOC links." << txf::nl;
}

void PrintHeader( void )
{
	fout << NAME " " VERSION " "__DATE__ " " __TIME__;
	fout << " by "AUTHOR " (" AUTHOR_EMAIL ")" << txf::nl;
	fout << "Copyright the Epeios project (" EPSMSC_EPEIOS_URL "). " << txf::nl;
	fout << EPSMSC_GNU_TEXT << txf::nl;
}

static void AnalyzeOptions(
	clnarg::analyzer___ &Analyzer,
	string_ &ListsFileName,
	string_ &GeneralsFileName )
{
ERRProlog
	epeios::row__ P;
	clnarg::option_list Options;
	clnarg::id__ Option;
	const char *Unknow = NULL;
ERRBegin
	Options.Init();

	if ( ( Unknow = Analyzer.GetOptions( Options ) ) != NULL ) {
		ferr << '\'' << Unknow << ": unknow option." << txf::nl;
		fout << "Try '" NAME " --help' to get more informations." << txf::nl;
		ERRu();
	}

	P = Options.First();

	while( P != NONE ) {
		switch( Option = Options( P ) ) {
		case oListsFile:
			Analyzer.GetArgument( Option, ListsFileName );
			if( ListsFileName.Amount() == 0 ) {
				ferr << "Option '-l' must have one argument." << txf::nl;
				ERRu();
			}
			break;
		case oGeneralFile:
			Analyzer.GetArgument( Option, GeneralsFileName );
			if( GeneralsFileName.Amount() == 0 ) {
				ferr << "Option '-g' must have one argument." << txf::nl;
				ERRu();
			}
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
	char *&Source,
	char *&Template,
	char *&Dest )
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
		Dest = Free( P ).Convert();
		P = Free.Previous( P );
	case 2:
		Template = Free( P ).Convert();
		P = Free.Previous( P );
		Source = Free( P ).Convert();
		Free.Previous( P );
		break;
	case 1:
	case 0:
		ferr << "Too few arguments." << txf::nl;
		fout << "Try '" NAME " --help' to get more informations." << txf::nl;
		ERRt();
	default:
		ferr << "Too many arguments." << txf::nl;
		fout << "Try '" NAME " --help' to get more informations." << txf::nl;
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
	string_ &ListsFileName,
	string_ &GeneralFileName,
	char *&Source,
	char *&Template,
	char *&Dest )
{
ERRProlog
	clnarg::description Description;
	clnarg::analyzer___ Analyzer;
	clnarg::argument ListsFile;
	clnarg::argument GeneralsFile;
ERRBegin
	Description.Init();

	Description.AddOption( 'l', "lists-file", oListsFile );
	Description.AddOption( 'g', "general-file", oGeneralFile );
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

	AnalyzeOptions( Analyzer, ListsFileName, GeneralFileName );

	AnalyzeFreeArguments( Analyzer, Source, Template, Dest );

ERRErr
ERREnd
ERREpilog
}

static void Main(
	int argc,
	const char *argv[] )

{
ERRProlog
	fil::file_oflow___ File;
	oflow_ Flow;
	bso::bool__ Backup = false;
	char *Dest = NULL;
	char *Source = NULL;
	char *Template = NULL;
	string ListsFileName;
	string GeneralsFileName;
ERRBegin

	ListsFileName.Init();
	GeneralsFileName.Init();

	AnalyzeArgs( argc, argv, ListsFileName, GeneralsFileName, Source, Template, Dest );

	if ( Dest ) {

		TOLCreateBackupFile( Dest, tol::hbfRename );
		Backup = true;

		if ( !File.Init( Dest, err::hSkip ) )
		{
			ferr << "Error while opening '" << Dest << "' for writing." << txf::nl;
			ERRu();
		}

		Flow.Init( File );

		Go( Source, Template, GeneralsFileName, ListsFileName, Flow );
	}else
		Go( Source, Template, GeneralsFileName, ListsFileName, fout );
ERRErr
	Flow.reset();
	File.reset();

	if ( Backup )
		TOLRecoverBackupFile( Dest );

	if ( ERRMajor == err::thw )
		ERRRst();
ERREnd
	if ( Dest != NULL )
		free( Dest );

	if ( Source != NULL )
		free( Source );

	if ( Template != NULL )
		free( Template );

ERREpilog
}

int main(
	int argc,
	const char *argv[] )
{
	int Retour = EXIT_SUCCESS;
ERRFProlog
ERRFBegin
	Main( argc, argv );
ERRFErr
	Retour = EXIT_FAILURE;
ERRFEnd
ERRFEpilog
	return Retour;
}