/*
  'reveal' by Claude L. Simon (epeios@epeios.org).
  Replaces tags in a file by text or file content.
  Copyright (C) 1999, 2000 Claude Simon.

  This file is part of the Epeios project (http://www.epeios.org/).

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
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
#include "fil.h"
#include "tagexp.h"
#include "clnarg.h"
#include "epsmsc.h"
#include "txmtbl.h"
#include "flx.h"

#define NAME			"reveal"
#define VERSION			"0.1.4"
#define AUTHOR_LINK		EPSMSC_AUTHOR_LINK
#define AUTHOR			EPSMSC_AUTHOR
#define AUTHOR_EMAIL	EPSMSC_AUTHOR_EMAIL

#define DEFAULT_TAG_DELIMITER		'$'
#define DEFAULT_COMMENTARY_MARKER	'#'
#define DEFAULT_TEXT_MARKER			'&'
#define DEFAULT_FILE_MARKER			'%'
#define BUFFER_SIZE					100
#define RESERVED_TAG_PREFIX			'_'
#define UNKNOW_RESERVED_TAG			tagexp::t_amount

typedef tagdtc::id__	id__;

typedef str_string		string;
typedef str_string_		string_;

typedef txmtbl_cell		cell;
typedef txmtbl_cell_	cell_;

typedef txmtbl_table	table;
typedef txmtbl_table_	table_;

typedef txmtbl_line		line;
typedef txmtbl_line_	line_;

typedef tagexp::tag_expander_	expander_;
typedef tagexp::tag_expander	expander;

typedef SET_( id__ )	ids_;
typedef SET( id__ )		ids;


enum command {
	cHelp,
	cVersion,
	cReveal,
};

enum option {
	oCommentaryMarker,
	oTagDelimiter,
	oTextMarker,
	oFileMarker,
	oSkip,
	o_amount
};

id__ GetReservedTagId( const str_string_ &Tag )
{
	int i = 0;

	while( ( i < tagexp::t_amount ) && ( Tag != str_string( tagexp::TagLabel( (tagexp::tag_id)i ) ) ) )
		i++;

	return i;
}

bso__bool TestTag( const string_ &Tag )
{
	POSITION__ P = Tag.First();
	bso__char C;

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
	string_ &Value,
	bso__char Text,
	bso__char File )
{
	bso__char C;

	if ( Value.Amount() < 1 )
		return tagexp::nUnknow;
	else {
		C = Value( Value.First() );

		Value.Delete( 0 );

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
	bso__char Text,
	bso__char File,
	cell_ &ValueBuffer )
{
	POSITION__ P;
	CMITEM( cell_ ) Tag, &Buffer = Tag;
	cell_ &Value = ValueBuffer;
	id__ Id;
	tagexp::nature Nature;

	Value.Init();

	Buffer.Init( Line );

	if ( Line.Amount() != 2 ) {
		ferr << "Error on line " << Line.Location() << " in the tag description flow." << nl;
		ERRt();
	}

	P = Line.First();
	Value = Buffer( Line.Next( P ) );

	if ( !TestTag( Tag( P ) ) ) {
		ferr << "Incorrect tag at line " << Line.Location() << " column " << Tag().Location() << " in the tag descriptor flow." << nl;
		ERRt();
	}

	Nature = TestAndFormTagValue( Value, Text, File );

	if ( Nature == tagexp::nUnknow ) {
		ferr << "Incorrect tag value at line " << Line.Location() << " column " << Value.Location() << " in the tag descriptor flow." << nl;
		ERRt();
	}
	else if ( Tag()( 0 ) == RESERVED_TAG_PREFIX )
		if ( ( Id = GetReservedTagId( Tag() ) ) == UNKNOW_RESERVED_TAG ) {
			ferr << "Incorrect reserved tag at line " << Line.Location() <<  " column " << Tag().Location() << " in the description flow." << nl;
			ERRt();
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
	Expander.Add( str_string( Delimiter ), tagexp::nText, str_string( "" ) );
	Expander.Add( str_string( TOLDate() ), tagexp::nText, str_string( "_DATE_" ) );
	Expander.Add( str_string( TOLTime() ), tagexp::nText, str_string( "_TIME_" ) );
	Expander.Add( str_string( NAME ), tagexp::nText, str_string( "_NAME_" ) );
	Expander.Add( str_string( AUTHOR ), tagexp::nText, str_string( "_AUTHOR_" ) );
	Expander.Add( str_string( AUTHOR_EMAIL ), tagexp::nText, str_string( "_EMAIL_" ) );
	Expander.Add( str_string( EPSMSC_EPEIOS_URL ), tagexp::nText, str_string( "_LINK_" ) );
	Expander.Add( str_string( VERSION ), tagexp::nText, str_string( "_VERSION_" ) );
}


void FillExpander( 
	const table_ &Table,
	bso__char Delimiter,
	bso__char Text,
	bso__char File,
	expander_ &Expander )
{
ERRProlog
	cell ValueBuffer;
	CITEM( line_ ) Line;
	POSITION__ P = Table.First();
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
	bso__char Delimiter,
	tagexp::action Action,
	xtf::extended_text_iflow___ &IFlow,
	txf__text_oflow_ &OFlow )
{
ERRProlog
	str_string FileName;
ERRBegin
	FileName.Init();

	switch( Expander.Expand( IFlow, OFlow, Delimiter, FileName, Action, err::hSkip ) ) {
	case tagexp::rOK:
		break;
	case tagexp::rFile:
		ferr << "Error with file '" << FileName << "'." << nl;
		ERRt();
		break;
	case tagexp::rTag:
		ferr << "Error with tag at line " << IFlow.Line() << " column " << IFlow.Column() << '.' << nl;
		ERRt();
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
	bso__char Delimiter,
	tagexp::action Action,
	bso__char Text,
	bso__char File,
	flw__iflow_ &IFlow,
	txf__text_oflow_ &OFlow )
{
ERRProlog
	expander Expander;
	xtf::extended_text_iflow___ TIFlow;
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
	bso__char Delimiter,
	const line_ &Line )
{
	CMITEM( cell_ ) Tag;
	POSITION__ P = Line.First();
	POSITION__ PV;
	id__ Id;

	Tag.Init( Line );
	Ids.Allocate( Line.Amount() );

	PV = Ids.First();

	while ( P != NONE ) {
		if ( !TestTag( Tag( P ) ) ) {
			ferr << "Incorrect tag at line " << Line.Location() <<  " column " << Tag().Location() << " in the description flow." << nl;
			ERRt();
		}

		if ( Tag()( 0 ) == RESERVED_TAG_PREFIX )
			if ( ( Id = GetReservedTagId( Tag() ) ) == UNKNOW_RESERVED_TAG ) {
				ferr << "Incorrect reserved tag at line " << Line.Location() <<  " column " << Tag().Location() << " in the description flow." << nl;
				ERRt();
			}
			else
				Ids.Write( Id, PV );
		else Ids.Write( Expander.Create( Tag() ), PV );

		P = Line.Next( P );
		PV = Ids.Next( PV );
	}

	AdditionalTags( Expander, Delimiter );
}


void FillSet(
	flx_set_ &Set,
	flw__iflow_ &IFlow )
{
ERRProlog
	bso__char Buffer[BUFFER_SIZE];
	flw__amount Amount;
	flx__set_oflow_ OFlow;
ERRBegin
	OFlow.Init( Set );

	do {
		Amount = IFlow.GetUpTo( BUFFER_SIZE, Buffer, 1 );
		OFlow.Put( Buffer, Amount );
	} while ( Buffer[Amount-1] != XTF_EOXC );
ERRErr
ERREnd
ERREpilog
}

void CompleteExpander(
	expander_ &Expander,
	const ids_ &Ids,
	const line_ &Line,
	bso__char Text,
	bso__char File )
{
ERRProlog
	CMITEM( cell_ ) Value;
	POSITION__ P;
	POSITION__ PV;
	str_string TagValue;
	tagexp::nature Nature;
ERRBegin
	Value.Init( Line );

	P = Line.First();
	PV = Ids.First();

	while( PV != NONE ) {

		if ( P == NONE ) {
			ferr << "Not enough tag value at line " << Line.Location() << " in tag description file." << nl;
			ERRt();
		}

		TagValue.Init();
		TagValue = Value( P );

		Nature = TestAndFormTagValue( TagValue, Text, File );

		if ( Nature == tagexp::nUnknow ) {
			ferr << "Incorrect tag value at line " << Line.Location() << " column " << Value().Location() << " in the tag descriptor flow." << nl;
			ERRt();
		}
		else
			Expander.Assign( TagValue, Nature, Ids( PV ) );

		PV = Ids.Next( PV );
		P = Line.Next( P );
	}

	if ( P != NONE ) {
		ferr << "Unassigned tag value at line " << Line.Location() << " column " << Value( P ).Location() << " in the tag descriptor flow." << nl;
		ERRt();
	}
ERRErr
ERREnd
ERREpilog
}



void HandleMulti( 
	const table_ &Table,
	bso__char Delimiter,
	tagexp::action Action,
	bso__char Text,
	bso__char File,
	flw__iflow_ &IFlow,
	txf__text_oflow_ &OFlow )
{
ERRProlog
	expander Expander;
	flx_set Set;
	flx__set_iflow_ ISFlow;
	CITEM( line_ ) Line;
	POSITION__ P;
	ids Ids;
	xtf::extended_text_iflow___ TIFlow;
ERRBegin
	Set.Init();

	FillSet( Set, IFlow );

	Expander.Init();

	P = Table.First();

	if ( P == NONE ) {
		ferr << "Tag descriptor file contents no data." << nl;
		ERRt();
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

inline bso__bool IsMulti(
	const cell_ &Cell,
	bso__char Text,
	bso__char File )
{
	return ( Cell( 0 ) != Text ) && ( Cell( 0 ) != File );
}

inline bso__bool IsMulti(
	const line_ &Line,
	bso__char Text,
	bso__char File )
{
	POSITION__ P;
	CMITEM( cell_ ) Cell;

	if ( Line.Amount() < 2 ) {
		ferr << "The content of the tag description file is incorrect." << nl;
		ERRt();
		return false;
	}
	else {
		P = Line.Next( Line.First() );

		return IsMulti( Line( P, Cell ), Text, File );
	}
}

inline bso__bool IsMulti(
	const table_ &Table,
	bso__char Text,
	bso__char File )
{
	POSITION__ P;
	CITEM( line_ ) Line;

	if ( Table.IsEmpty() )
		return false;
	else {
		P = Table.First();

		return IsMulti( Table( P, Line ), Text, File );
	}
}


void Go(
	const table_ &Table,
	bso__char Delimiter,
	tagexp::action Action,
	bso__char Text,
	bso__char File,
	flw__iflow_ &IFlow,
	txf__text_oflow_ &OFlow )
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
	xtf::extended_text_iflow___ &Desc,
	bso__char Commentary,
	table_ &Table )
{
	TXMTBLGetTable( Desc, Table );

	Table.DeleteCommentaries( Commentary );
	Table.DeleteEmptyCells();
	Table.DeleteEmptyLines();
}

void PrintUsage( void )
{
	fout << "Usage: " << NAME << " [command] [options] desc-file [source-file [dest-file]]" << nl;
	fout << "desc-file:" << tab << "tag description file." << nl;
	fout << "source-file:" << tab << "source file; stdin if none." << nl;
	fout << "dest-file:" << tab << "destination file; stdout if none." << nl;
	fout << "Command: " << nl;
	fout << tab << "<none>, -r, --reveal:" << nl << tab << tab << "write source file to destination file revealing tags" << nl << tab << tab << "according to tag description file." << nl;
	fout << tab << "--version:" << nl << tab << tab << "print version of " NAME " components." << nl;
	fout << tab << "--help:" << tab << "print this message." << nl;
	fout << "Options:" << nl;
	fout << tab << "-d, --tag-delimiter CHAR:" << nl << tab << tab << "CHAR becomes the tag delimiter ('" << DEFAULT_TAG_DELIMITER << "' by default)." << nl;
	fout  << tab << "-s, --skip:" << nl << tab << tab << "don't write to output before next 'print' or 'raw' tag." << nl;
	fout << tab << "-c, --commentary-marker CHAR:" << nl << tab << tab << "CHAR becomes the marker for commentary ('" << DEFAULT_COMMENTARY_MARKER << "' by default)." << nl;
	fout << tab << "-t, --text-marker CHAR:" << nl << tab << tab << "CHAR becomes the marker for text ('" << DEFAULT_TEXT_MARKER << "' by default)." << nl;
	fout << tab << "-f, --file-marker CHAR:" << nl << tab << tab << "CHAR becomes the marker for file ('" << DEFAULT_FILE_MARKER << "' by default)." << nl;
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
	tagexp::action &Action,
	char &Delimiter,
	char &Commentary,
	char &Text,
	char &File )
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
		case oTagDelimiter:
			Analyzer.GetArgument( Option, Argument );
			if( Argument.Amount() == 0 ) {
				ferr << "Option '-d,--tag-delimiter' must have one argument." << nl;
				ERRt();
			}
			else if ( Argument.Amount() > 1 ) {
				ferr << "Argument of option '-d,--tag-delimiter' must be a character." << nl;
				ERRt();
			}
			else
				Delimiter = Argument( 0 );
			break;
		case oCommentaryMarker:
			Analyzer.GetArgument( Option, Argument );
			if( Argument.Amount() == 0 ) {
				ferr << "Option '-c,--commentary-marker' must have one argument." << nl;
				ERRt();
			}
			else if ( Argument.Amount() > 1 ) {
				ferr << "Argument of option '-c,--commentary-marker' must be a character." << nl;
				ERRt();
			}
			else
				Commentary = Argument( 0 );
			break;
		case oTextMarker:
			Analyzer.GetArgument( Option, Argument );
			if( Argument.Amount() == 0 ) {
				ferr << "Option '-t,--text-marker' must have one argument." << nl;
				ERRt();
			}
			else if ( Argument.Amount() > 1 ) {
				ferr << "Argument of option '-t,--text-marker' must be a character." << nl;
				ERRt();
			}
			else
				Text = Argument( 0 );
			break;
		case oFileMarker:
			Analyzer.GetArgument( Option, Argument );
			if( Argument.Amount() == 0 ) {
				ferr << "Option '-f,--file-marker' must have one argument." << nl;
				ERRt();
			}
			else if ( Argument.Amount() > 1 ) {
				ferr << "Argument of option '-f,--file-marker' must be a character." << nl;
				ERRt();
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
	clnarg__analyzer_ &Analyzer,
	char *&Desc,
	char *&Source,
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
		Source = Free( P ).Convert();
		P = Free.Previous( P );
	case 1:
		Desc = Free( P ).Convert();
		P = Free.Previous( P );
		break;
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
	char *&Desc,
	char *&Source,
	char *&Dest,
	tagexp::action &Action,
	char &Delimiter,
	char &Commentary,
	char &Text,
	char &File )
{
ERRProlog
	clnarg_description Description;
	clnarg__analyzer_ Analyzer;
ERRBegin
	Description.Init();

	Description.AddOption( 's', "skip", oSkip );
	Description.AddOption( 'd', "tag-delimiter", oTagDelimiter );
	Description.AddOption( 'c', "commentary-marker", oCommentaryMarker );
	Description.AddOption( 't', "text-marker", oTextMarker );
	Description.AddOption( 'f', "file-marker", oFileMarker );
	Description.AddOption( 'l', "correct-location", cVersion );
	Description.AddCommand( 'r', "reveal", cReveal );
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
	case cReveal:
	case CLNARG_NONE:
		break;
	default:
		ERRc();
	}

	AnalyzeOptions( Analyzer, Action, Delimiter, Commentary, Text, File );

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
	fil::file_iflow___ IFile;
	fil::file_oflow___ OFile;
	txf__text_oflow_ OText;
	fil::file_iflow___ DFile;
	xtf::extended_text_iflow___ DText;
	txf__text_oflow_ *OFlow = NULL;
	flw__iflow_ *IFlow = NULL;
	bso__bool Backup = false;
	char *Desc = NULL;
	char *Source = NULL;
	char *Dest = NULL;
	tagexp::action Action = tagexp::aPrint;
	char Delimiter = DEFAULT_TAG_DELIMITER;
	char Commentary = DEFAULT_COMMENTARY_MARKER;
	char Text = DEFAULT_TEXT_MARKER;
	char File = DEFAULT_FILE_MARKER;
	table Table;
ERRBegin
	AnalyzeArgs( argc, argv, Desc, Source, Dest, Action, Delimiter, Commentary, Text, File );

	if ( DFile.Init( Desc, err::hSkip ) != fil::sSuccess)
	{
		ferr << "Error while opening '" << Desc << "' for reading." << nl;
		ERRt();
	}

	DFile.EOFT( XTF_EOXT );

	DText.Init( DFile );

	Table.Init();

	GetTable( DText, Commentary, Table );

	if ( Source ) {

		if ( IFile.Init( Source, err::hSkip ) != fil::sSuccess )
		{
			ferr << "Error while opening '" << Source << "' for reading." << nl;
			ERRt();
		}

		IFile.EOFT( XTF_EOXT );

		IFlow = &IFile;
	}
	else
	{
		STFfin.EOFT( XTF_EOXT );

		IFlow = &STFfin;
	}


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

	Go( Table, Delimiter, Action, Text, File, *IFlow, *OFlow );

ERRErr
	OText.reset();
	DText.reset();

	IFile.reset();
	OFile.reset();
	DFile.reset();

	if ( Backup )
		TOLRecoverBackupFile( Dest );
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
	int Retour = EXIT_SUCCESS;
ERRFProlog
ERRFBegin
	Main( argc, argv );
ERRFErr
	Retour = EXIT_FAILURE;

	if ( ERRMajor == err::thw )
		ERRRst();

ERRFEnd
ERRFEpilog
	fout << sync;

	return Retour;
}