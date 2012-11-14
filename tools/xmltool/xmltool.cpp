/*
	'xmltool' by Claude SIMON (http://zeusw.org/intl/contact.html)
	Copyright (C) 2001 IDEALX (http://www.IDEALX.com)

	This program is part of an IDEALX (http://www.IDEALX.org/) project
	which uses Epeios (http://zeusw.org/epeios/) technology.

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
#include "stf.h"
#include "epsmsc.h"
#include "clnarg.h"
#include "fnm.h"

#include "fil.h"
#include "txf.h"
#include "xmldcm.h"
#include "xmldbs.h"
#include "xmlprs.h"
#include "xmltol.h"

typedef xmldcm::xml_document	document;
typedef xmldcm::xml_document_	document_;

typedef xmldbs::xml_database	database;
typedef xmldbs::xml_database_	database_;

#define NAME			"xmltool"
#define VERSION			"0.2.1"
#define COPYRIGHT		"Copyright (c) 2001 IDEALX (" EPSMSC_IDEALX_URL ")."
//#define COPYRIGHT		"Copyright (c) 2001 the Epeios project (" EPSMSC_EPEIOS_URL ")."
#define AUTHOR_NAME		EPSMSC_AUTHOR_NAME
#define AUTHOR_EMAIL	EPSMSC_AUTHOR_EMAIL
#define HELP			EPSMSC_HELP_INVITATION( NAME )
#define CVS_DETAILS		("$Id$\b " + 5)

/* Beginning of the part which handles command line arguments. */

enum command {
	cHelp,	// Print help message.
	cVersion,	// Print version of components.
	cLicense,	// Print licsense.
	cTags,	// Print structure of tags.
	cReplace,	// Replace tags.
	cSplit,	// Split in several files.
};

enum option {
};

struct parameters {
	void Free( char *P )
	{
		tol::Free( P );
	}
	command Command;
	char *SourceFileName;	// The source file name.
	char *DestFileName;		// The destination file name.
	char *MapFileName;		// The correspondances between old and new tags file name (Replace command).
	char *SplitTag;			// The tag which is use for splitting.
	char *NameTag;			// Tag or attribute used for file naming.
	parameters( void )
	{
		SourceFileName = NULL;
		DestFileName = NULL;
		MapFileName = NULL;
		SplitTag = NULL;
		NameTag = NULL;
	}
	~parameters( void )
	{
		Free( SourceFileName );
		Free( DestFileName );
		Free( MapFileName );
		Free( SplitTag );
		Free( NameTag );
	}
};

void PrintUsage( const clnarg::description_ &Description )
{
	fout << "XML handling tool" << txf::nl;
	fout << "Usage: " << txf::nl;
	fout << txf::tab << NAME << " --help|--version|--license" << txf::nl;
	fout << txf::tab << NAME << " -t|--tags [options] <source> [<target>]" << txf::nl;
	fout << txf::tab << NAME << " -r|--replace [options] <map> <source> [<target>]" << txf::nl;
	fout << txf::tab << NAME << " -s|--split [options] <split tag> <name tag>|@<name attribute> <source>" << txf::nl;
	fout << txf::nl;
	fout << txf::tab << "map:" << txf::tab << "name of the file containing the new names of the tags." << txf::nl;
	fout << txf::tab << "split tag:" << txf::nl << txf::tab << txf::tab << "tag after which the splitting occurs." << txf::nl;
	fout << txf::tab << "name tag:" << txf::nl << txf::tab << txf::tab << "tag which value is used for the name of the files." << txf::nl;
	fout << txf::tab << "name attribute:" << txf::nl << txf::tab << txf::tab << "attribute which value is used for the name of the files." << txf::nl;
	fout << txf::tab << "source:" << txf::tab << "name of the file which contents is handled." << txf::nl;
	fout << txf::tab << "target:" << txf::tab << "name of the file in which the result is stored." << txf::nl;
	fout << txf::tab << txf::tab << "If no name given, the standard output is used." << txf::nl;
	fout << "Command:" << txf::nl;
//	clnarg::PrintCommandUsage( Description, c, "", false, true );
	clnarg::PrintCommandUsage( Description, cTags, "print tags of a XML file.", false, false );
	clnarg::PrintCommandUsage( Description, cReplace, "replace all tags of a XML file.", false, false );
	clnarg::PrintCommandUsage( Description, cSplit, "split a XML file in several files.", false, false );
	clnarg::PrintCommandUsage( Description, cVersion, "print version of " NAME " components.", false, false );
	clnarg::PrintCommandUsage( Description, cLicense, "print text about the license.", false, false );
	clnarg::PrintCommandUsage( Description, cHelp, "print this message.", true, false );
//	fout << "Options:" << txf::nl;
//	clnarg::PrintOptionUsage( Description, o, "", false );
}

void PrintHeader( void )
{
	fout << NAME " V" VERSION " ("__DATE__ " " __TIME__ ") by " AUTHOR_NAME " (" << AUTHOR_EMAIL << ") " << txf::nl << COPYRIGHT << txf::nl;
	fout << EPSMSC_IDEALX_TEXT <<txf::nl;
//	fout << EPSMSC_EPEIOS_TEXT <<txf::nl;
	fout << "CVS file details : " << CVS_DETAILS << txf::nl;
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
		ferr << '\'' << Unknow << "': unknow option." << txf::nl;
		fout << HELP << txf::nl;
		ERRt();
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

	
	switch ( Parameters.Command ) {
	case cTags:
		P = Free.Last();
		switch( Free.Amount() ) {
		case 2:
			Parameters.DestFileName = Free( P ).Convert();
			P = Free.Previous( P );
		case 1:
			Parameters.SourceFileName = Free( P ).Convert();
			break;
		case 0:
			ferr << "Too few arguments." << txf::nl;
			fout << HELP << txf::nl;
			ERRt();
			break;
		default:
			ferr << "Too mamy arguments." << txf::nl;
			fout << HELP << txf::nl;
			ERRt();
			break;
		}
		break;
	case cReplace:
		P = Free.Last();
		switch( Free.Amount() ) {
		case 3:
			Parameters.DestFileName = Free( P ).Convert();
			P = Free.Previous( P );
		case 2:
			Parameters.SourceFileName = Free( P ).Convert();
			P = Free.Previous( P );
			Parameters.MapFileName = Free( P ).Convert();
			break;
		case 1:
		case 0:
			ferr << "Too few arguments." << txf::nl;
			fout << HELP << txf::nl;
			ERRt();
			break;
		default:
			ferr << "Too mamy arguments." << txf::nl;
			fout << HELP << txf::nl;
			ERRt();
			break;
		}
		break;
	case cSplit:
		P = Free.First();
		switch( Free.Amount() ) {
		case 3:
			Parameters.SplitTag = Free( P ).Convert();
			P = Free.Next( P );
			Parameters.NameTag = Free( P ).Convert();
			P = Free.Next( P );
			Parameters.SourceFileName =Free( P ).Convert();
			break;
		case 2:
		case 1:
		case 0:
			ferr << "Too few arguments." << txf::nl;
			fout << HELP << txf::nl;
			ERRt();
			break;
		default:
			ferr << "Too mamy arguments." << txf::nl;
			fout << HELP << txf::nl;
			ERRt();
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

//	Description.AddCommand( '', "", c );
	Description.AddCommand( 'r', "replace", cReplace );
	Description.AddCommand( 't', "tags", cTags );
	Description.AddCommand( 's', "split", cSplit );
	Description.AddCommand( CLNARG_NO_SHORT, "version", cVersion );
	Description.AddCommand( CLNARG_NO_SHORT, "help", cHelp );
	Description.AddCommand( CLNARG_NO_SHORT, "license", cLicense );
//	Description.AddOption( '', "", o );

	Analyzer.Init( argc, argv, Description );

	switch ( Analyzer.GetCommand() ) {
	case cVersion:
		PrintHeader();
		TTR.Advertise();
		ERRt();
		break;
	case cHelp:
		PrintUsage( Description );
		ERRt();
		break;
	case cLicense:
		epsmsc::PrintLicense();
		fout << txf::nl << "This program uses 'expat', under following license:" << txf::nl << txf::nl;
		xmlprs::License( fout );
		ERRt();
		break;
	case cTags:
		Parameters.Command = cTags;
		break;
	case cReplace:
		Parameters.Command = cReplace;
		break;
	case cSplit:
		Parameters.Command = cSplit;
		break;
//	case c:
	case CLNARG_NONE:
		ferr << "You have to give a command." << txf::nl;
			fout << HELP << txf::nl;
		ERRt();
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


void GetDocument(
	const char *FileName,
	document_ &Document,
	int BufferSize )
{
ERRProlog
	fil::file_iflow___ IFlow;
	txf::text_iflow___ TFlow;	
ERRBegin
	if ( IFlow.Init( FileName, err::hSkip ) != fil::sSuccess ) {
		ferr << "Unable to open file '" << FileName << "' for reading." << txf::nl;
		ERRt();
	}
	
	IFlow.EOFD( "\x1a" );
	TFlow.Init( IFlow );

	xmlprs::ParseXML( TFlow, Document, BufferSize );
ERRErr
ERREnd
ERREpilog
}

void WriteStructureOfTags(
	const xmltag::tags_ &Tags,
	const char *FileName )
{
ERRProlog
	fil::file_oflow___ OFlow;
	txf::text_oflow___ TFlow;	
	bso::bool__ Backup = false;
ERRBegin
	if ( FileName != NULL ) {
		tol::CreateBackupFile( FileName, tol::hbfRename );
		Backup = true;
	
		if ( OFlow.Init( FileName, err::hSkip ) != fil::sSuccess ) {
			ferr << "Unable to open file '" << FileName << "' for writing." << txf::nl;
			ERRt();
		}
		
		TFlow.Init( OFlow );
		
		xmltag::WriteXML( Tags, TFlow, xmltag::mNoEmptyTag );
	} else {
		xmltag::WriteXML( Tags, fout, xmltag::mNoEmptyTag );
		fout << txf::nl;
	}

ERRErr
	if ( Backup )
		tol::RecoverBackupFile( FileName );
ERREnd
ERREpilog
}

void WriteDocument(
	const document &Document,
	const char *FileName )
{
ERRProlog
	fil::file_oflow___ OFlow;
	txf::text_oflow___ TFlow;	
	bso::bool__ Backup = false;
ERRBegin
	if ( FileName != NULL ) {
		tol::CreateBackupFile( FileName, tol::hbfRename );
		Backup = true;
	
		if ( OFlow.Init( FileName, err::hSkip ) != fil::sSuccess ) {
			ferr << "Unable to open file '" << FileName << "' for writing." << txf::nl;
			ERRt();
		}
		
		TFlow.Init( OFlow );
		
		xmldcm::WriteXML( Document, TFlow );
	} else {
		xmldcm::WriteXML( Document, fout );
		fout << txf::nl;
	}

ERRErr
	if ( Backup )
		tol::RecoverBackupFile( FileName );
ERREnd
ERREpilog
}


void TagsCommand( const parameters &Parameters )
{
ERRProlog
	document Document;
ERRBegin
	Document.Init();

	GetDocument( Parameters.SourceFileName, Document, 1000 );
	
	WriteStructureOfTags( Document.Tags, Parameters.DestFileName );
ERRErr
ERREnd
ERREpilog
}

void ReplaceCommand( const parameters &Parameters )
{
ERRProlog
	document Document;
	database Map;
ERRBegin
	Document.Init();

	GetDocument( Parameters.MapFileName, Document, 1000 );
	
	Map.Init();
	
	xmltol::Convert( Document, Map );
	
	Document.Init();
	
	GetDocument( Parameters.SourceFileName, Document, 1000 );
	
	xmltol::Replace( Document.Tags, Map );
	
	WriteDocument( Document, Parameters.DestFileName );
ERRErr
ERREnd
ERREpilog
}

void CreateFile(
	const document_ &Document,
	str::string_ &RawBaseFileName,
	xmldcm::value_row__ ValueRow )
{
ERRProlog
	char *BaseFileName = NULL;
	char *FileName = NULL;
	fil::file_oflow___ FOFlow;
	txf::text_oflow___ TOFlow;
ERRBegin
	BaseFileName = RawBaseFileName.Convert();
	FileName = fnm::MakeFileName( "", BaseFileName, ".xml" );

	if ( !FOFlow.Init( FileName, err::hSkip ) ) {
		ferr << "Unable to open file '" << FileName << "' for writing." << txf::nl;
		ERRt();
	}

	TOFlow.Init( FOFlow );

	xmldcm::WriteXML( Document, TOFlow, ValueRow );

ERRErr
ERREnd
	tol::Free( BaseFileName );

	tol::Free( FileName );
ERREpilog
}

void CreateFile(
	const document_ &Document,
	xmldcm::value_row__ ValueRow,
	xmldcm::tag_row__ NameTagRow )
{
	xmldcm::value_row__ NameValueRow;
	xmldcm::value Value;

	if ( ( NameValueRow = Document.First( ValueRow, NameTagRow ) ) == NONE ) {
		ferr << "Splitted branch misses name value." << txf::nl;
		ERRt();
	}

	Value.Init();

	Document.GetValue( NameValueRow, Value );

	CreateFile( Document, Value, ValueRow );
}



void Split(
	const document_ &Document,
	xmldcm::tag_row__ SplitTagRow,
	xmldcm::tag_row__ NameTagRow )
{
	xmldcm::value_row__ ValueRow = Document.First( Document.GetRoot(), SplitTagRow );
	
	while( ValueRow != NONE ) {
		CreateFile( Document, ValueRow, NameTagRow );
		ValueRow = Document.Next( ValueRow, SplitTagRow );
	}
}

void Split( const parameters &Parameters )
{
ERRProlog
	document Document;
	xmldcm::tag_row__ SplitTagRow = NONE;
	xmldcm::tag_row__ NameTagRow = NONE;
ERRBegin
	Document.Init();

	GetDocument( Parameters.SourceFileName, Document, 1000 );

	if ( ( SplitTagRow = Document.Tags.Position( xmldcm::name( Parameters.SplitTag ), Document.Tags.GetRoot(), xmldcm::tField ) ) == NONE ) {
		ferr  << "Root tag doesn't have tag named '" << Parameters.SplitTag << "'." << txf::nl;
		ERRt();
	}

	if ( ( NameTagRow = Document.Tags.Position( xmldcm::name( Parameters.NameTag ), SplitTagRow, xmldcm::tField ) ) == NONE ) {
		ferr  << '\'' << Parameters.SplitTag << "' doesn't have tag named '" << Parameters.NameTag << "'." << txf::nl;
		ERRt();
	}

	Split( Document, SplitTagRow, NameTagRow );
ERRErr
ERREnd
ERREpilog
}

void Go( const parameters &Parameters )
{
	switch( Parameters.Command ) {
	case cTags:
		TagsCommand( Parameters );
		break;
	case cReplace:
		ReplaceCommand( Parameters );
		break;
	case cSplit:
		Split( Parameters );
		break;
	default:
		ERRc();
		break;
	}
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
	fout << txf::sync;

	return ExitValue;
}
