/*
  'reveal' by Claude SIMON (http://zeusw.org/intl/contact.html).
  Replaces tags in a file by text or file content.
  Copyright (C) 1999-2002 Claude SIMON (http://zeusw.org/intl/contact.html)

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
  along with this program; if not, go to http://www.fsf.org or write to the
  
                        Free Software Foundation, Inc.,
           59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

// $Id$


#include "tol.h"
#include "flf.h"
#include "tagexp.h"
#include "clnarg.h"
#include "epsmsc.h"
#include "txmtbl.h"
#include "flx.h"
#include "cio.h"

#define NAME			"reveal"
#define VERSION			"1.0.5"
#define COPYRIGHT_YEARS	"1999-2004, 2006"
#define DESCRIPTION		"Replaces tags in a file by text or file content."
#define INFO EPSMSC_EPEIOS_TEXT
#define AUTHOR_NAME		EPSMSC_AUTHOR_NAME
#define AUTHOR_EMAIL	EPSMSC_AUTHOR_EMAIL
#define HELP			EPSMSC_HELP_INVITATION( NAME )
#define COPYRIGHT		"Copyright (c) " COPYRIGHT_YEARS " " AUTHOR_NAME " (" AUTHOR_EMAIL ")."
#define CVS_DETAILS		("$Id$\b " + 5)

#define DEFAULT_TAG_DELIMITER_S		"$"
#define DEFAULT_COMMENT_MARKER_S	"#"
#define DEFAULT_TEXT_MARKER_S		"&"
#define DEFAULT_FILE_MARKER_S		"%"
#define RESERVED_TAG_PREFIX_S		"_"
#define UNKNOW_RESERVED_TAG			tagexp::t_amount

#define DEFAULT_TAG_DELIMITER		DEFAULT_TAG_DELIMITER_S[0]
#define DEFAULT_COMMENT_MARKER		DEFAULT_COMMENT_MARKER_S[0]
#define DEFAULT_TEXT_MARKER			DEFAULT_TEXT_MARKER_S[0]
#define DEFAULT_FILE_MARKER			DEFAULT_FILE_MARKER_S[0]
#define BUFFER_SIZE					100
#define RESERVED_TAG_PREFIX			RESERVED_TAG_PREFIX_S[0]
#define UNKNOW_RESERVED_TAG			tagexp::t_amount

using tagdtc::id__;

using cio::cerr;
using cio::cin;
using cio::cout;

// This is needed by BC++, to avoid ambiguity between std::string and str::string ...
typedef str::string		estring;
typedef str::string_	estring_;

using txmtbl::cell;
using txmtbl::cell_;

using txmtbl::table;
using txmtbl::table_;

using txmtbl::line;
using txmtbl::line_;

typedef tagexp::tag_expander_	expander_;
typedef tagexp::tag_expander	expander;

typedef bch::E_BUNCH_( id__ )	ids_;
typedef bch::E_BUNCH( id__ )		ids;


enum command {
	cHelp,
	cVersion,
	cReveal,
	cLicense,
};

enum option {
	oCommentMarker,
	oTagDelimiter,
	oTextMarker,
	oFileMarker,
	oSkip,
	o_amount
};

id__ GetReservedTagId( const estring_ &Tag )
{
	int i = 0;

	while( ( i < tagexp::t_amount ) && ( Tag != estring( tagexp::TagLabel( (tagexp::tag_id)i ) ) ) )
		i++;

	return i;
}

bso::bool__ TestTag( const estring_ &Tag )
{
	epeios::row__ P = Tag.First();
	bso::char__ C;

	if ( Tag.Amount() != 0 )
	{
		while( P != NONE ) {
			C = Tag( P );
			if ( !( isalnum( C ) || ( C == '_' ) ) )
				return false;

			P = Tag.Next( P );
		}

		return true;
	}
	else
		return false;
}

inline tagexp::nature TestAndFormTagValue(
	estring_ &Value,
	bso::char__ Text,
	bso::char__ File )
{
	bso::char__ C;

	if ( Value.Amount() < 1 )
		return tagexp::nUnknow;
	else {
		C = Value( Value.First() );

		Value.Remove( 0 );

		if ( C == Text )
			return tagexp::nText;
		else if ( C == File ) 
			return tagexp::nFile;
		else
			return tagexp::nUnknow;
	}
}


inline void Add(
	const line_ &Line,
	expander_ &Expander,
	bso::char__ Text,
	bso::char__ File,
	cell_ &ValueBuffer )
{
	epeios::row__ P;
	ctn::E_CMITEM( cell_ ) Tag, &Buffer = Tag;
	cell_ &Value = ValueBuffer;
	id__ Id;
	tagexp::nature Nature;

	Value.Init();

	Buffer.Init( Line );

	if ( Line.Amount() != 2 ) {
		cerr << "Error on line " << Line.Location() << " in the tag description flow." << txf::nl;
		ERRi();
	}

	P = Line.First();
	Value = Buffer( Line.Next( P ) );

	if ( !TestTag( Tag( P ) ) ) {
		cerr << "Incorrect tag at line " << Line.Location() << " column " << Tag().Location() << " in the tag descriptor flow." << txf::nl;
		ERRi();
	}

	Nature = TestAndFormTagValue( Value, Text, File );

	if ( Nature == tagexp::nUnknow ) {
		cerr << "Incorrect tag value at line " << Line.Location() << " column " << Value.Location() << " in the tag descriptor flow." << txf::nl;
		ERRi();
	}
	else if ( Tag()( 0 ) == RESERVED_TAG_PREFIX )
		if ( ( Id = GetReservedTagId( Tag() ) ) == UNKNOW_RESERVED_TAG ) {
			cerr << "Incorrect reserved tag at line " << Line.Location() <<  " column " << Tag().Location() << " in the description flow." << txf::nl;
			ERRi();
		}
		else
			Expander.Assign( Value, Nature, Id );
	else
		Expander.Add( Value, Nature, Tag() );
}

inline void AdditionalTags(
	expander_ &Expander,
	char Delimiter )
{
	tol::buffer__ Buffer;

	Expander.Add( estring( Delimiter ), tagexp::nText, estring( "" ) );
	Expander.Add( estring( tol::Date( Buffer ) ), tagexp::nText, estring( "_DATE_" ) );
	Expander.Add( estring( tol::Time( Buffer ) ), tagexp::nText, estring( "_TIME_" ) );
	Expander.Add( estring( NAME ), tagexp::nText, estring( "_NAME_" ) );
	Expander.Add( estring( AUTHOR_NAME ), tagexp::nText, estring( "_AUTHOR_" ) );
	Expander.Add( estring( AUTHOR_EMAIL ), tagexp::nText, estring( "_EMAIL_" ) );
	Expander.Add( estring( EPSMSC_EPEIOS_URL ), tagexp::nText, estring( "_LINK_" ) );
	Expander.Add( estring( VERSION ), tagexp::nText, estring( "_VERSION_" ) );
}


void FillExpander( 
	const table_ &Table,
	bso::char__ Delimiter,
	bso::char__ Text,
	bso::char__ File,
	expander_ &Expander )
{
ERRProlog
	cell ValueBuffer;
	ctn::E_CITEM( line_ ) Line;
	epeios::row__ P = Table.First();
ERRBegin
	ValueBuffer.Init();
	Line.Init( Table );

	while( P != NONE ) {
		Add( Line( P ), Expander, Text, File, ValueBuffer );
		P = Table.Next( P );
	}

	AdditionalTags( Expander, Delimiter );
ERRErr
ERREnd
ERREpilog
}

void Expand(
	const expander_ &Expander,
	bso::char__ Delimiter,
	tagexp::action Action,
	xtf::extended_text_iflow__ &IFlow,
	txf::text_oflow__ &OFlow )
{
ERRProlog
	estring FileName;
ERRBegin
	FileName.Init();

	switch( Expander.Expand( IFlow, OFlow, Delimiter, FileName, Action, err::hSkip ) ) {
	case tagexp::sOK:
		break;
	case tagexp::sBadFile:
		cerr << "Error with file '" << FileName << "'." << txf::nl;
		ERRi();
		break;
	case tagexp::sUnknowTag:
		cerr << "Error with tag at line " << IFlow.Line() << " column " << IFlow.Column() << '.' << txf::nl;
		ERRi();
		break;
	default:
		ERRc();
		break;
	}
ERRErr
ERREnd
ERREpilog
}
	
void HandleSingle( 
	const table_ &Table,
	bso::char__ Delimiter,
	tagexp::action Action,
	bso::char__ Text,
	bso::char__ File,
	flw::iflow__ &IFlow,
	txf::text_oflow__ &OFlow )
{
ERRProlog
	expander Expander;
	xtf::extended_text_iflow__ TIFlow;
ERRBegin
	Expander.Init();

	TIFlow.Init( IFlow );

	FillExpander( Table, Delimiter, Text, File, Expander );

	Expand( Expander, Delimiter, Action, TIFlow, OFlow );
ERRErr
ERREnd
ERREpilog
}

void PrepareExpander(
	expander_ &Expander,
	ids_ &Ids,
	bso::char__ Delimiter,
	const line_ &Line )
{
	ctn::E_CMITEM( cell_ ) Tag;
	epeios::row__ P = Line.First();
	epeios::row__ PV;
	id__ Id;

	Tag.Init( Line );
	Ids.Allocate( Line.Amount() );

	PV = Ids.First();

	while ( P != NONE ) {
		if ( !TestTag( Tag( P ) ) ) {
			cerr << "Incorrect tag at line " << Line.Location() <<  " column " << Tag().Location() << " in the description flow." << txf::nl;
			ERRi();
		}

		if ( Tag()( 0 ) == RESERVED_TAG_PREFIX )
			if ( ( Id = GetReservedTagId( Tag() ) ) == UNKNOW_RESERVED_TAG ) {
				cerr << "Incorrect reserved tag at line " << Line.Location() <<  " column " << Tag().Location() << " in the description flow." << txf::nl;
				ERRi();
			}
			else
				Ids.Store( Id, PV );
		else Ids.Store( Expander.Create( Tag() ), PV );

		P = Line.Next( P );
		PV = Ids.Next( PV );
	}

	AdditionalTags( Expander, Delimiter );
}


void FillSet(
	str::string_ &Set,
	flw::iflow__ &IFlow )
{
ERRProlog
	bso::char__ Buffer[BUFFER_SIZE];
	epeios::size__ Amount;
	flx::bunch_oflow___<str::string_, bso::char__> OFlow;
ERRBegin
	OFlow.Init( Set );

	do {
		Amount = IFlow.ReadUpTo( BUFFER_SIZE, Buffer, 1 );
		OFlow.Write( Buffer, Amount );
	} while ( Buffer[Amount-1] != XTF_EOXC );
ERRErr
ERREnd
ERREpilog
}

void CompleteExpander(
	expander_ &Expander,
	const ids_ &Ids,
	const line_ &Line,
	bso::char__ Text,
	bso::char__ File )
{
ERRProlog
	ctn::E_CMITEM( cell_ ) Value;
	epeios::row__ P;
	epeios::row__ PV;
	estring TagValue;
	tagexp::nature Nature;
ERRBegin
	Value.Init( Line );

	P = Line.First();
	PV = Ids.First();

	while( PV != NONE ) {

		if ( P == NONE ) {
			cerr << "Not enough tag value at line " << Line.Location() << " in tag description file." << txf::nl;
			ERRi();
		}

		TagValue.Init();
		TagValue = Value( P );

		Nature = TestAndFormTagValue( TagValue, Text, File );

		if ( Nature == tagexp::nUnknow ) {
			cerr << "Incorrect tag value at line " << Line.Location() << " column " << Value().Location() << " in the tag descriptor flow." << txf::nl;
			ERRi();
		}
		else
			Expander.Assign( TagValue, Nature, Ids( PV ) );

		PV = Ids.Next( PV );
		P = Line.Next( P );
	}

	if ( P != NONE ) {
		cerr << "Unassigned tag value at line " << Line.Location() << " column " << Value( P ).Location() << " in the tag descriptor flow." << txf::nl;
		ERRi();
	}
ERRErr
ERREnd
ERREpilog
}



void HandleMulti( 
	const table_ &Table,
	bso::char__ Delimiter,
	tagexp::action Action,
	bso::char__ Text,
	bso::char__ File,
	flw::iflow__ &IFlow,
	txf::text_oflow__ &OFlow )
{
ERRProlog
	expander Expander;
	str::string Set;
	flx::bunch_iflow__<str::string_, bso::char__> ISFlow;
	ctn::E_CITEM( line_ ) Line;
	epeios::row__ P;
	ids Ids;
	xtf::extended_text_iflow__ TIFlow;
ERRBegin
	Set.Init();

	FillSet( Set, IFlow );

	Expander.Init();

	P = Table.First();

	if ( P == NONE ) {
		cerr << "Tag descriptor file contents no data." << txf::nl;
		ERRi();
	}

	Ids.Init();

	Line.Init( Table );

	PrepareExpander( Expander, Ids, Delimiter, Line( P ) );

	P = Table.Next( P );

	while( P != NONE ) {
		CompleteExpander( Expander, Ids, Line( P ), Text, File );

		ISFlow.Init( Set );
		TIFlow.Init( ISFlow );

		Expand( Expander, Delimiter, Action, TIFlow, OFlow );

		P = Table.Next( P );
	}
ERRErr
ERREnd
ERREpilog
}

inline bso::bool__ IsMulti(
	const cell_ &Cell,
	bso::char__ Text,
	bso::char__ File )
{
	return ( Cell( 0 ) != Text ) && ( Cell( 0 ) != File );
}

inline bso::bool__ IsMulti(
	const line_ &Line,
	bso::char__ Text,
	bso::char__ File )
{
	epeios::row__ P;
	ctn::E_CMITEM( cell_ ) Cell;

	if ( Line.Amount() < 2 ) {
		cerr << "The content of the tag description file is incorrect." << txf::nl;
		ERRi();
		return false;
	}
	else {
		P = Line.Next( Line.First() );

		return IsMulti( Line( P, Cell ), Text, File );
	}
}

inline bso::bool__ IsMulti(
	const table_ &Table,
	bso::char__ Text,
	bso::char__ File )
{
	epeios::row__ P;
	ctn::E_CITEM( line_ ) Line;

	if ( Table.IsEmpty() )
		return false;
	else {
		P = Table.First();

		return IsMulti( Table( P, Line ), Text, File );
	}
}


void Go(
	const table_ &Table,
	bso::char__ Delimiter,
	tagexp::action Action,
	bso::char__ Text,
	bso::char__ File,
	flw::iflow__ &IFlow,
	txf::text_oflow__ &OFlow )
{
ERRProlog
ERRBegin
	if ( IsMulti( Table, Text,File ) )
		HandleMulti( Table, Delimiter, Action, Text, File, IFlow, OFlow );
	else
		HandleSingle( Table, Delimiter, Action, Text, File, IFlow, OFlow );
ERRErr
ERREnd
ERREpilog
}

void GetTable(
	xtf::extended_text_iflow__ &Desc,
	bso::char__ Comment,
	table_ &Table )
{
	txmtbl::GetTable( Desc, Table );

	Table.Purge( Comment );
}

void PrintUsage( const clnarg::description_ &Description )
{
	cout << DESCRIPTION << txf::nl;
	cout << NAME << " --version|--license|--help" << txf::nl;
	clnarg::PrintCommandUsage( Description, cVersion, "print version of " NAME " components.", clnarg::vSplit, false );
	clnarg::PrintCommandUsage( Description, cLicense, "print the license.", clnarg::vSplit, false );
	clnarg::PrintCommandUsage( Description, cHelp, "print this message.", clnarg::vOneLine, false );
	cout << NAME << " <command> [options] desc-file [source-file [dest-file]]" << txf::nl;
	cout << txf::tab << "desc-file:" << txf::tab << "tag description file." << txf::nl;
	cout << txf::tab << "source-file:" << txf::tab << "source file; stdin if none." << txf::nl;
	cout << txf::tab << "dest-file:" << txf::tab << "destination file; stdout if none." << txf::nl;
	cout << "command: " << txf::nl;
	clnarg::PrintCommandUsage( Description, cReveal, "write source file to destination file revealing tags.", clnarg::vSplit, true );
	cout << "options:" << txf::nl;
	clnarg::PrintOptionUsage( Description, oTagDelimiter, "CHAR", "CHAR becomes the tag delimiter ('" DEFAULT_TAG_DELIMITER_S "' by default).", clnarg::vSplit );
	clnarg::PrintOptionUsage( Description, oSkip, "don't write to output before next 'print' or 'raw' tag.", clnarg::vSplit );
	clnarg::PrintOptionUsage( Description, oCommentMarker, "CHAR", "CHAR becomes the marker for comment ('" DEFAULT_COMMENT_MARKER_S "' by default).", clnarg::vSplit );
	clnarg::PrintOptionUsage( Description, oTextMarker, "CHAR", "CHAR becomes the marker for text ('" DEFAULT_TEXT_MARKER_S "' by default).", clnarg::vSplit );
	clnarg::PrintOptionUsage( Description, oFileMarker, "CHAR",  "CHAR becomes the marker for file ('" DEFAULT_FILE_MARKER_S "' by default).", clnarg::vSplit );
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
	tagexp::action &Action,
	char &Delimiter,
	char &Comment,
	char &Text,
	char &File )
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
		cerr << "Try '" NAME " --help' to get more informations." << txf::nl;
		ERRi();
	}

	P = Options.First();

	while( P != NONE ) {
		Argument.Init();

		switch( Option = Options( P ) ) {
		case oTagDelimiter:
			Analyzer.GetArgument( Option, Argument );
			if( Argument.Amount() == 0 ) {
				cerr << "Option '-d,--tag-delimiter' must have one argument." << txf::nl;
				ERRi();
			}
			else if ( Argument.Amount() > 1 ) {
				cerr << "Argument of option '-d,--tag-delimiter' must be a character." << txf::nl;
				ERRi();
			}
			else
				Delimiter = Argument( 0 );
			break;
		case oCommentMarker:
			Analyzer.GetArgument( Option, Argument );
			if( Argument.Amount() == 0 ) {
				cerr << "Option '-c,--comment-marker' must have one argument." << txf::nl;
				ERRi();
			}
			else if ( Argument.Amount() > 1 ) {
				cerr << "Argument of option '-c,--comment-marker' must be a character." << txf::nl;
				ERRi();
			}
			else
				Comment = Argument( 0 );
			break;
		case oTextMarker:
			Analyzer.GetArgument( Option, Argument );
			if( Argument.Amount() == 0 ) {
				cerr << "Option '-t,--text-marker' must have one argument." << txf::nl;
				ERRi();
			}
			else if ( Argument.Amount() > 1 ) {
				cerr << "Argument of option '-t,--text-marker' must be a character." << txf::nl;
				ERRi();
			}
			else
				Text = Argument( 0 );
			break;
		case oFileMarker:
			Analyzer.GetArgument( Option, Argument );
			if( Argument.Amount() == 0 ) {
				cerr << "Option '-f,--file-marker' must have one argument." << txf::nl;
				ERRi();
			}
			else if ( Argument.Amount() > 1 ) {
				cerr << "Argument of option '-f,--file-marker' must be a character." << txf::nl;
				ERRi();
			}
			else
				File = Argument( 0 );
			break;
		case oSkip:
			Action = tagexp::aSkip;
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
	char *&Desc,
	char *&Source,
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
		Source = Free( P ).Convert();
		P = Free.Previous( P );
	case 1:
		Desc = Free( P ).Convert();
		P = Free.Previous( P );
		break;
	case 0:
		cerr << "Too few arguments." << txf::nl;
		cout << "Try '" NAME " --help' to get more informations." << txf::nl;
		ERRi();
		break;
	default:
		cerr << "Too many arguments." << txf::nl;
		cout << "Try '" NAME " --help' to get more informations." << txf::nl;
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
	char *&Desc,
	char *&Source,
	char *&Dest,
	tagexp::action &Action,
	char &Delimiter,
	char &Comment,
	char &Text,
	char &File )
{
ERRProlog
	clnarg::description Description;
	clnarg::analyzer___ Analyzer;
ERRBegin
	Description.Init();

	Description.AddOption( 's', "skip", oSkip );
	Description.AddOption( 'd', "tag-delimiter", oTagDelimiter );
	Description.AddOption( 'c', "comment-marker", oCommentMarker );
	Description.AddOption( 't', "text-marker", oTextMarker );
	Description.AddOption( 'f', "file-marker", oFileMarker );
	Description.AddOption( 'l', "correct-location", cVersion );
	Description.AddCommand( 'r', "reveal", cReveal );
	Description.AddCommand( CLNARG_NO_SHORT, "version", cVersion );
	Description.AddCommand( CLNARG_NO_SHORT, "help", cHelp );
	Description.AddCommand( CLNARG_NO_SHORT, "license", cLicense );

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
	case cReveal:
	case CLNARG_NONE:
		break;
	default:
		ERRc();
	}

	AnalyzeOptions( Analyzer, Action, Delimiter, Comment, Text, File );

	AnalyzeFreeArguments( Analyzer, Desc, Source, Dest );

ERRErr
ERREnd
ERREpilog
}

static inline void Main(
	int argc,
	const char *argv[] )
{
ERRProlog
	flf::file_iflow___ IFile;
	flf::file_oflow___ OFile;
	txf::text_oflow__ OText( OFile );
	flf::file_iflow___ DFile;
	xtf::extended_text_iflow__ DText;
	txf::text_oflow__ *OFlow = NULL;
	flw::iflow__ *IFlow = NULL;
	bso::bool__ Backup = false;
	char *Desc = NULL;
	char *Source = NULL;
	char *Dest = NULL;
	tagexp::action Action = tagexp::aPrint;
	char Delimiter = DEFAULT_TAG_DELIMITER;
	char Comment = DEFAULT_COMMENT_MARKER;
	char Text = DEFAULT_TEXT_MARKER;
	char File = DEFAULT_FILE_MARKER;
	table Table;
ERRBegin
	AnalyzeArgs( argc, argv, Desc, Source, Dest, Action, Delimiter, Comment, Text, File );

	if ( DFile.Init( Desc, err::hSkip ) != fil::sSuccess)
	{
		cerr << "Error while opening '" << Desc << "' for reading." << txf::nl;
		ERRi();
	}

	DFile.EOFD( XTF_EOXT );

	DText.Init( DFile );

	Table.Init();

	GetTable( DText, Comment, Table );

	if ( Source ) {

		if ( IFile.Init( Source, err::hSkip ) != fil::sSuccess )
		{
			cerr << "Error while opening '" << Source << "' for reading." << txf::nl;
			ERRi();
		}

		IFile.EOFD( XTF_EOXT );

		IFlow = &IFile;
	}
	else
	{
		cio::cinf.EOFD( XTF_EOXT );

		IFlow = &cio::cinf;
	}


	if ( Dest ) {

		fil::CreateBackupFile( Dest, fil::hbfRename );
		Backup = true;

		if ( OFile.Init( Dest, err::hSkip ) != fil::sSuccess )
		{
			cerr << "Error while opening '" << Dest << "' for writing." << txf::nl;
			ERRi();
		}

//		OText.Init( OFile );

		OFlow = &OText;
	}
	else
		OFlow = &cout;

	Go( Table, Delimiter, Action, Text, File, *IFlow, *OFlow );

ERRErr
	OText.Synchronize();
	DText.reset();

	IFile.reset();
	OFile.reset();
	DFile.reset();

	if ( Backup )
		fil::RecoverBackupFile( Dest );
ERREnd
	if ( Desc != NULL )
		free( Desc );

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
ERRFProlog
ERRFBegin
	Main( argc, argv );
ERRFErr
ERRFEnd
ERRFEpilog
	return ERRExitValue;
}
