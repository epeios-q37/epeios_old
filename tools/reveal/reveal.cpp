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
#define VERSION			"0.1.6"
#define AUTHOR_LINK		EPSMSC_AUTHOR_LINK
#define AUTHOR_NAME			EPSMSC_AUTHOR_NAME
#define AUTHOR_EMAIL	EPSMSC_AUTHOR_EMAIL

#define DEFAULT_TAG_DELIMITER		'$'
#define DEFAULT_COMMENTARY_MARKER	'#'
#define DEFAULT_TEXT_MARKER			'&'
#define DEFAULT_FILE_MARKER			'%'
#define BUFFER_SIZE					100
#define RESERVED_TAG_PREFIX			'_'
#define UNKNOW_RESERVED_TAG			tagexp::t_amount

using tagdtc::id__;

using str::string;
using str::string_;

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
};

enum option {
	oCommentaryMarker,
	oTagDelimiter,
	oTextMarker,
	oFileMarker,
	oSkip,
	o_amount
};

id__ GetReservedTagId( const string_ &Tag )
{
	int i = 0;

	while( ( i < tagexp::t_amount ) && ( Tag != string( tagexp::TagLabel( (tagexp::tag_id)i ) ) ) )
		i++;

	return i;
}

bso::bool__ TestTag( const string_ &Tag )
{
	tym::row__ P = Tag.First();
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
	string_ &Value,
	bso::char__ Text,
	bso::char__ File )
{
	bso::char__ C;

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
	bso::char__ Text,
	bso::char__ File,
	cell_ &ValueBuffer )
{
	tym::row__ P;
	ctn::E_CMITEM( cell_ ) Tag, &Buffer = Tag;
	cell_ &Value = ValueBuffer;
	id__ Id;
	tagexp::nature Nature;

	Value.Init();

	Buffer.Init( Line );

	if ( Line.Amount() != 2 ) {
		ferr << "Error on line " << Line.Location() << " in the tag description flow." << txf::nl;
		ERRt();
	}

	P = Line.First();
	Value = Buffer( Line.Next( P ) );

	if ( !TestTag( Tag( P ) ) ) {
		ferr << "Incorrect tag at line " << Line.Location() << " column " << Tag().Location() << " in the tag descriptor flow." << txf::nl;
		ERRt();
	}

	Nature = TestAndFormTagValue( Value, Text, File );

	if ( Nature == tagexp::nUnknow ) {
		ferr << "Incorrect tag value at line " << Line.Location() << " column " << Value.Location() << " in the tag descriptor flow." << txf::nl;
		ERRt();
	}
	else if ( Tag()( 0 ) == RESERVED_TAG_PREFIX )
		if ( ( Id = GetReservedTagId( Tag() ) ) == UNKNOW_RESERVED_TAG ) {
			ferr << "Incorrect reserved tag at line " << Line.Location() <<  " column " << Tag().Location() << " in the description flow." << txf::nl;
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
	Expander.Add( string( Delimiter ), tagexp::nText, string( "" ) );
	Expander.Add( string( TOLDate() ), tagexp::nText, string( "_DATE_" ) );
	Expander.Add( string( TOLTime() ), tagexp::nText, string( "_TIME_" ) );
	Expander.Add( string( NAME ), tagexp::nText, string( "_NAME_" ) );
	Expander.Add( string( AUTHOR_NAME ), tagexp::nText, string( "_AUTHOR_" ) );
	Expander.Add( string( AUTHOR_EMAIL ), tagexp::nText, string( "_EMAIL_" ) );
	Expander.Add( string( EPSMSC_EPEIOS_URL ), tagexp::nText, string( "_LINK_" ) );
	Expander.Add( string( VERSION ), tagexp::nText, string( "_VERSION_" ) );
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
	tym::row__ P = Table.First();
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
	xtf::extended_text_iflow___ &IFlow,
	txf::text_oflow___ &OFlow )
{
ERRProlog
	string FileName;
ERRBegin
	FileName.Init();

	switch( Expander.Expand( IFlow, OFlow, Delimiter, FileName, Action, err::hSkip ) ) {
	case tagexp::sOK:
		break;
	case tagexp::sBadFile:
		ferr << "Error with file '" << FileName << "'." << txf::nl;
		ERRt();
		break;
	case tagexp::sUnknowTag:
		ferr << "Error with tag at line " << IFlow.Line() << " column " << IFlow.Column() << '.' << txf::nl;
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
	bso::char__ Delimiter,
	tagexp::action Action,
	bso::char__ Text,
	bso::char__ File,
	flw::iflow___ &IFlow,
	txf::text_oflow___ &OFlow )
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
	bso::char__ Delimiter,
	const line_ &Line )
{
	ctn::E_CMITEM( cell_ ) Tag;
	tym::row__ P = Line.First();
	tym::row__ PV;
	id__ Id;

	Tag.Init( Line );
	Ids.Allocate( Line.Amount() );

	PV = Ids.First();

	while ( P != NONE ) {
		if ( !TestTag( Tag( P ) ) ) {
			ferr << "Incorrect tag at line " << Line.Location() <<  " column " << Tag().Location() << " in the description flow." << txf::nl;
			ERRt();
		}

		if ( Tag()( 0 ) == RESERVED_TAG_PREFIX )
			if ( ( Id = GetReservedTagId( Tag() ) ) == UNKNOW_RESERVED_TAG ) {
				ferr << "Incorrect reserved tag at line " << Line.Location() <<  " column " << Tag().Location() << " in the description flow." << txf::nl;
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
	flx::bunch_ &Set,
	flw::iflow___ &IFlow )
{
ERRProlog
	bso::char__ Buffer[BUFFER_SIZE];
	flw::amount__ Amount;
	flx::bunch_oflow___ OFlow;
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
	bso::char__ Text,
	bso::char__ File )
{
ERRProlog
	ctn::E_CMITEM( cell_ ) Value;
	tym::row__ P;
	tym::row__ PV;
	string TagValue;
	tagexp::nature Nature;
ERRBegin
	Value.Init( Line );

	P = Line.First();
	PV = Ids.First();

	while( PV != NONE ) {

		if ( P == NONE ) {
			ferr << "Not enough tag value at line " << Line.Location() << " in tag description file." << txf::nl;
			ERRt();
		}

		TagValue.Init();
		TagValue = Value( P );

		Nature = TestAndFormTagValue( TagValue, Text, File );

		if ( Nature == tagexp::nUnknow ) {
			ferr << "Incorrect tag value at line " << Line.Location() << " column " << Value().Location() << " in the tag descriptor flow." << txf::nl;
			ERRt();
		}
		else
			Expander.Assign( TagValue, Nature, Ids( PV ) );

		PV = Ids.Next( PV );
		P = Line.Next( P );
	}

	if ( P != NONE ) {
		ferr << "Unassigned tag value at line " << Line.Location() << " column " << Value( P ).Location() << " in the tag descriptor flow." << txf::nl;
		ERRt();
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
	flw::iflow___ &IFlow,
	txf::text_oflow___ &OFlow )
{
ERRProlog
	expander Expander;
	flx::bunch Set;
	flx::bunch_iflow___ ISFlow;
	ctn::E_CITEM( line_ ) Line;
	tym::row__ P;
	ids Ids;
	xtf::extended_text_iflow___ TIFlow;
ERRBegin
	Set.Init();

	FillSet( Set, IFlow );

	Expander.Init();

	P = Table.First();

	if ( P == NONE ) {
		ferr << "Tag descriptor file contents no data." << txf::nl;
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
	tym::row__ P;
	ctn::E_CMITEM( cell_ ) Cell;

	if ( Line.Amount() < 2 ) {
		ferr << "The content of the tag description file is incorrect." << txf::nl;
		ERRt();
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
	tym::row__ P;
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
	flw::iflow___ &IFlow,
	txf::text_oflow___ &OFlow )
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
	bso::char__ Commentary,
	table_ &Table )
{
	txmtbl::GetTable( Desc, Table );

	Table.DeleteCommentaries( Commentary );
	Table.DeleteEmptyCells();
	Table.DeleteEmptyLines();
}

void PrintUsage( void )
{
	fout << "Usage: " << NAME << " [command] [options] desc-file [source-file [dest-file]]" << txf::nl;
	fout << "desc-file:" << txf::tab << "tag description file." << txf::nl;
	fout << "source-file:" << txf::tab << "source file; stdin if none." << txf::nl;
	fout << "dest-file:" << txf::tab << "destination file; stdout if none." << txf::nl;
	fout << "Command: " << txf::nl;
	fout << txf::tab << "<none>, -r, --reveal:" << txf::nl << txf::tab << txf::tab << "write source file to destination file revealing tags" << txf::nl << txf::tab << txf::tab << "according to tag description file." << txf::nl;
	fout << txf::tab << "--version:" << txf::nl << txf::tab << txf::tab << "print version of " NAME " components." << txf::nl;
	fout << txf::tab << "--help:" << txf::tab << "print this message." << txf::nl;
	fout << "Options:" << txf::nl;
	fout << txf::tab << "-d, --tag-delimiter CHAR:" << txf::nl << txf::tab << txf::tab << "CHAR becomes the tag delimiter ('" << DEFAULT_TAG_DELIMITER << "' by default)." << txf::nl;
	fout  << txf::tab << "-s, --skip:" << txf::nl << txf::tab << txf::tab << "don't write to output before next 'print' or 'raw' tag." << txf::nl;
	fout << txf::tab << "-c, --commentary-marker CHAR:" << txf::nl << txf::tab << txf::tab << "CHAR becomes the marker for commentary ('" << DEFAULT_COMMENTARY_MARKER << "' by default)." << txf::nl;
	fout << txf::tab << "-t, --text-marker CHAR:" << txf::nl << txf::tab << txf::tab << "CHAR becomes the marker for text ('" << DEFAULT_TEXT_MARKER << "' by default)." << txf::nl;
	fout << txf::tab << "-f, --file-marker CHAR:" << txf::nl << txf::tab << txf::tab << "CHAR becomes the marker for file ('" << DEFAULT_FILE_MARKER << "' by default)." << txf::nl;
}

void PrintHeader( void )
{
	fout << NAME " V" VERSION " "__DATE__ " " __TIME__;
	fout << " by "AUTHOR_NAME " (" AUTHOR_EMAIL ")" << txf::nl;
	fout << "Copyright the Epeios project (" EPSMSC_EPEIOS_URL "). " << txf::nl;
//	fout << EPSMSC_GNU_TEXT << txf::nl;
}

static void AnalyzeOptions(
	clnarg::analyzer___ &Analyzer,
	tagexp::action &Action,
	char &Delimiter,
	char &Commentary,
	char &Text,
	char &File )
{
ERRProlog
	tym::row__ P;
	clnarg::option_list Options;
	clnarg::id__ Option;
	const char *Unknow = NULL;
	clnarg::argument Argument;
ERRBegin
	Options.Init();

	if ( ( Unknow = Analyzer.GetOptions( Options ) ) != NULL ) {
		ferr << '\'' << Unknow << "': unknow option." << txf::nl;
		ferr << "Try '" NAME " --help' to get more informations." << txf::nl;
		ERRt();
	}

	P = Options.First();

	while( P != NONE ) {
		Argument.Init();

		switch( Option = Options( P ) ) {
		case oTagDelimiter:
			Analyzer.GetArgument( Option, Argument );
			if( Argument.Amount() == 0 ) {
				ferr << "Option '-d,--tag-delimiter' must have one argument." << txf::nl;
				ERRt();
			}
			else if ( Argument.Amount() > 1 ) {
				ferr << "Argument of option '-d,--tag-delimiter' must be a character." << txf::nl;
				ERRt();
			}
			else
				Delimiter = Argument( 0 );
			break;
		case oCommentaryMarker:
			Analyzer.GetArgument( Option, Argument );
			if( Argument.Amount() == 0 ) {
				ferr << "Option '-c,--commentary-marker' must have one argument." << txf::nl;
				ERRt();
			}
			else if ( Argument.Amount() > 1 ) {
				ferr << "Argument of option '-c,--commentary-marker' must be a character." << txf::nl;
				ERRt();
			}
			else
				Commentary = Argument( 0 );
			break;
		case oTextMarker:
			Analyzer.GetArgument( Option, Argument );
			if( Argument.Amount() == 0 ) {
				ferr << "Option '-t,--text-marker' must have one argument." << txf::nl;
				ERRt();
			}
			else if ( Argument.Amount() > 1 ) {
				ferr << "Argument of option '-t,--text-marker' must be a character." << txf::nl;
				ERRt();
			}
			else
				Text = Argument( 0 );
			break;
		case oFileMarker:
			Analyzer.GetArgument( Option, Argument );
			if( Argument.Amount() == 0 ) {
				ferr << "Option '-f,--file-marker' must have one argument." << txf::nl;
				ERRt();
			}
			else if ( Argument.Amount() > 1 ) {
				ferr << "Argument of option '-f,--file-marker' must be a character." << txf::nl;
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
	clnarg::analyzer___ &Analyzer,
	char *&Desc,
	char *&Source,
	char *&Dest )
{
ERRProlog
	clnarg::arguments Free;
	tym::row__ P;
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
		ferr << "Too few arguments." << txf::nl;
		fout << "Try '" NAME " --help' to get more informations." << txf::nl;
		ERRt();
		break;
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
	clnarg::description Description;
	clnarg::analyzer___ Analyzer;
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
	txf::text_oflow___ OText;
	fil::file_iflow___ DFile;
	xtf::extended_text_iflow___ DText;
	txf::text_oflow___ *OFlow = NULL;
	flw::iflow___ *IFlow = NULL;
	bso::bool__ Backup = false;
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
		ferr << "Error while opening '" << Desc << "' for reading." << txf::nl;
		ERRt();
	}

	DFile.EOFD( XTF_EOXT );

	DText.Init( DFile );

	Table.Init();

	GetTable( DText, Commentary, Table );

	if ( Source ) {

		if ( IFile.Init( Source, err::hSkip ) != fil::sSuccess )
		{
			ferr << "Error while opening '" << Source << "' for reading." << txf::nl;
			ERRt();
		}

		IFile.EOFD( XTF_EOXT );

		IFlow = &IFile;
	}
	else
	{
		stf::finF.EOFD( XTF_EOXT );

		IFlow = &stf::finF;
	}


	if ( Dest ) {

		TOLCreateBackupFile( Dest, tol::hbfRename );
		Backup = true;

		if ( OFile.Init( Dest, err::hSkip ) != fil::sSuccess )
		{
			ferr << "Error while opening '" << Dest << "' for writing." << txf::nl;
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
	fout << txf::sync;

	return Retour;
}
