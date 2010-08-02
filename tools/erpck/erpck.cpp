/*
	'erpck' by Claude SIMON (claude.simon@zeusw.org)
	Preprocesseur XML
	Copyright (C) 2007 Claude SIMON

	This file is part of the Epeios project (http://zeusw.org/epeios/).

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

#include "err.h"
#include "cio.h"
#include "epsmsc.h"
#include "clnarg.h"
#include "xpp.h"
#include "fnm.h"
#include "flf.h"
#include "dtr.h"
#include "lcl.h"

#include "rpkbsc.h"
#include "rpkals.h"
#include "rpkrcd.h"
#include "rpktbl.h"
#include "rpkctx.h"

#define NAME					"erpck"
#define VERSION					"0.2.0"
#define COPYRIGHT_YEARS			"2010"
#define DESCRIPTION				"Epeios random picker"
#define PROJECT_AFFILIATION		EPSMSC_EPEIOS_AFFILIATION
#define AUTHOR_NAME				EPSMSC_AUTHOR_NAME
#define AUTHOR_CONTACT			EPSMSC_AUTHOR_CONTACT
#define HELP					EPSMSC_HELP_INVITATION( NAME )
#define COPYRIGHT				EPSMSC_COPYRIGHT( COPYRIGHT_YEARS )
#define CVS_DETAILS				("$Id$\b " + 5)

#define DEFAULT_NAMESPACE	XML_EXTENDED_PARSER_DEFAULT_NAMESPACE

using cio::cin;
using cio::cout;
using cio::cerr;

typedef bso::ulong__	id__;
#define ALL				BSO_ULONG_MAX

/* Beginning of the part which handles command line arguments. */

enum exit_value__ {
	evSuccess = EXIT_SUCCESS,
	evGenericFailure = EXIT_FAILURE,
	// Erreur dans les paramètres d'entrée.
	evParameters,
	// Erreur lors de l'ouverture des fichiers d'entrée ou de sortie.
	evInputOutput,
	// Erreur lors du traitement.
	evProcessing,
	ev_amount
};

enum command {
	cHelp,
	cVersion,
	cLicense,
	cPick,
	c_amount,
	c_Undefined
};

/*
enum option {
};
*/

#define STRING_PARAM___( name )	CLNARG_STRING_PARAM___( name )

struct parameters {
	STRING_PARAM___( Project );
	id__ Id;
	parameters( void )
	{
		Id = 0;	// By default, a randomrecord is displayed.
	}
};

void PrintUsage( const clnarg::description_ &Description )
{
	cout << DESCRIPTION << txf::nl;
	cout << NAME << " --version|--license|--help" << txf::nl;
	clnarg::PrintCommandUsage( Description, cVersion, "print version of " NAME " components.", clnarg::vSplit, false );
	clnarg::PrintCommandUsage( Description, cLicense, "print the license.", clnarg::vSplit, false );
	clnarg::PrintCommandUsage( Description, cHelp, "print this message.", clnarg::vOneLine, false );
	cout << NAME << " <command> [options] <project-file> [id]" << txf::nl;
	cout << txf::tab << "project-file:" << txf::tab << "file containing the project defintions." << txf::nl;
	cout << "command: " << txf::nl;
	clnarg::PrintCommandUsage( Description, cPick, "Pick a random entry according to project file.", clnarg::vSplit, true );
	cout << txf::tab << "Id : the id of the reocrd to diaplay; if 0, all records are displayed; if absent, a random record is diaplayed." << txf::nl;
//	clnarg::PrintCommandUsage( Description, c, "", false, true );
//	cout << "options:" << txf::nl;
//	clnarg::PrintOptionUsage( Description, o, "", clnarg::vSplit );
}

void PrintHeader( void )
{
	cout << NAME " V" VERSION " "__DATE__ " " __TIME__;
	cout << " by "AUTHOR_NAME " (" AUTHOR_CONTACT ")" << txf::nl;
	cout << COPYRIGHT << txf::nl;
	cout << PROJECT_AFFILIATION << txf::nl;
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
/*		case o:
			break;
*/		default:
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
	epeios::row__ P = NONE, Error = NONE;
	id__ Id = 0;
ERRBegin
	Free.Init();

	Analyzer.GetArguments( Free );

	P = Free.Last();

	switch( Free.Amount() ) {
	case 2:
		Id = Free( P ).ToUL( &Error );

		if ( Error != NONE ) {
			cerr << "Bad value for record id !" << txf::nl;
			ERRExit( EXIT_FAILURE );
		}

		Parameters.Id = ( Id == 0 ? ALL : Id );
		P = Free.Previous( P );
	case 1:
		Free( P ).Convert( Parameters.Project );
		break;
	default:
		cerr << "Wrong amount of arguments." << txf::nl;
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
	Description.AddCommand( 'p', "pick", cPick );
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
	case cPick:
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

using namespace rpkals;
using namespace rpkrcd;
using namespace rpktbl;


typedef tables_	data_;
E_AUTO( data )

#define NAMESPACE	NAME ":"

#define DATA_TAG	NAMESPACE "data"

#define TABLE_TAG				NAMESPACE "table"
#define TABLE_LABEL_ATTRIBUTE	"label"

#define ALIASES_TAG	NAMESPACE "aliases"

#define ALIAS_TAG									NAMESPACE "alias"
#define ALIAS_LABEL_ATTRIBUTE						"label"
#define ALIAS_TABLE_LABEL_ATTRIBUTE					"TableLabel"
#define ALIAS_TABLE_ALIAS_ATTRIBUTE					"TableAlias"
#define ALIAS_RECORD_LABEL_ATTRIBUTE				"RecordLabel"

#define CONTENT_TAG									NAMESPACE "content"
#define CONTENT_DEFAULT_RECORD_LABEL_TAG_ATTRIBUTE	"DefaultRecordLabelTag"

#define RECORD_LABEL_ATTRIBUTE					NAMESPACE "label"
#define RECORD_WEIGHT_ATTRIBUTE					NAMESPACE "Weight"
#define RECORD_HANDLING_ATTRIBUTE				NAMESPACE "Handling"
#define RECORD_HANDLING_ATTRIBUTE_IGNORE_VALUE	"Ignore"
#define RECORD_HANDLING_ATTRIBUTE_SKIP_VALUE	"Skip"

#define INSERTION_TAG						NAMESPACE "insert"
#define INSERTION_TABLE_LABEL_ATTRIBUTE		"TableLabel"
#define INSERTION_RECORD_LABEL_ATTRIBUTE	"RecordLabel"
#define INSERTION_TABLE_ALIAS_ATTRIBUTE		"TableAlias"
#define INSERTION_RECORD_ALIAS_ATTRIBUTE	"RecordAlias"


static bso::bool__ BelongsToNamespace_( const str::string_ &Name )
{
	return str::Compare( Name, str::string( NAMESPACE ), 0, 0, strlen( NAMESPACE ) ) == 0;
}

static void PrintPosition_(
	const xtf::coord__ &Coord,
	txf::text_oflow__ &TFlow )
{
	TFlow << "line " << Coord.Line << ", column " << Coord.Column;
}

static void PrintPosition_(
	const xpp::preprocessing_iflow___ &IFlow,
	txf::text_oflow__ &TFlow )
{
	PrintPosition_( IFlow.Coord(), TFlow );

	if ( IFlow.LocalizedFileName().Amount() != 0 )
		TFlow << " in file '" << IFlow.LocalizedFileName() << '\'';
}

static void ReportErrorAndExit_( 
	const str::string_ &Message,
	const xpp::preprocessing_iflow___ &IFlow )
{
	cerr << "Parsing interrupted at ";
/*
	PrintPosition_( XFlow.Coord(), cerr );
	if ( XFlow.Preprocessor().LocalizedFileName().Amount() != 0 )
		cerr << " in file '" << XFlow.Preprocessor().LocalizedFileName() << '\'';
*/
	PrintPosition_( IFlow, cerr );
	cerr << " : " << Message << " !" << txf::nl;
	ERRExit( EXIT_FAILURE );

}

static void ReportErrorAndExit_( 
	const char *Message,
	xpp::preprocessing_iflow___ &IFlow )
{
	ReportErrorAndExit_( str::string( Message ), IFlow );
}

static void ReportErrorAndExit_( xpp::preprocessing_iflow___ &IFlow )
{
ERRProlog
	lcl::locales Locales;
	str::string ErrorMessage;
ERRBegin
	Locales.Init();
	ErrorMessage.Init();
	cerr << xpp::GetTranslation( IFlow.Status(), str::string(), Locales, ErrorMessage ) << " at ";
	PrintPosition_( IFlow, cerr );
	cerr << " !" << txf::nl;
	ERRExit( EXIT_FAILURE );
ERRErr
ERREnd
ERREpilog
}

static void Insert_(
	const str::string_ &RecordLabel,
	const records_ &Records,
	xpp::preprocessing_iflow___ &IFlow,
	record_ &Record )
{
	rrow__ Row = rpkrcd::SearchRecord( RecordLabel, Records );

	if ( Row == NONE )
		ReportErrorAndExit_( "Unable to find record", IFlow );

	rpkrcd::Insert( Row, Records, Record );
}

static void InsertUsingRecordAlias_(
	const str::string_ &RecordAlias,
	const record_aliases_ &Aliases,
	const tables_ &Tables,
	xpp::preprocessing_iflow___ &IFlow,
	record_ &Record )
{
	epeios::row__ Row = FindRecordAlias_( RecordAlias, Aliases );

	if ( Row == NONE )
		ReportErrorAndExit_( "Unable to find record alias", IFlow );

	Insert( Row, Aliases, Tables, Record );
}

static void Insert_(
	const str::string_ &RecordLabel,
	const table_ &Table,
	xpp::preprocessing_iflow___ &IFlow,
	record_ &Record )
{
	Insert_( RecordLabel, Table.Records, IFlow, Record );
}

static void Insert_(
	const str::string_ &RecordLabel,	
	trow__ TableRow,
	const tables_ &Tables,
	xpp::preprocessing_iflow___ &IFlow,
	record_ &Record )
{
	ctn::E_CITEMt( table_, trow__ )Table;

	Table.Init( Tables );

	Insert_( RecordLabel, Table( TableRow ), IFlow, Record );
}

static void Insert_(
	const str::string_ &RecordLabel,
	epeios::row__ AliasRow,
	const table_aliases_ &Aliases,
	const tables_ &Tables,
	xpp::preprocessing_iflow___ &IFlow,
	record_ &Record )
{
	ctn::E_CMITEM( table_alias_ ) Alias;

	Alias.Init( Aliases );

	Insert_( RecordLabel, Alias( AliasRow ).TableRow(), Tables, IFlow, Record );
}

static void InsertUsingTableAlias_(
	const str::string_ &TableAlias,
	const str::string_ &RecordLabel,
	const table_aliases_ &Aliases,
	const tables_ &Tables,
	xpp::preprocessing_iflow___ &IFlow,
	record_ &Record )
{
	epeios::row__ Row = FindTableAlias_( TableAlias, Aliases );

	if ( Row == NONE )
		ReportErrorAndExit_( "Unable to find table alias", IFlow );

	Insert_( RecordLabel, Row, Aliases, Tables, IFlow, Record );
}

static void InsertUsingLabels_(
	const str::string_ &TableLabel,
	const str::string_ &RecordLabel,
	const tables_ &Tables,
	xpp::preprocessing_iflow___ &IFlow,
	record_ &Record )
{
	trow__ Row = SearchTable( TableLabel, Tables );
	ctn::E_CITEMt( table_, trow__ ) Table;

	if ( Row == NONE )
		ReportErrorAndExit_( "Unable to find table", IFlow );

	Table.Init( Tables );

	Insert_( RecordLabel, Table( Row ), IFlow, Record );
}

static void Assign_(
	str::string_ &Target,
	const char *TargetLabel,
	xml::browser___ &Browser,
	xpp::preprocessing_iflow___ &IFlow )
{
ERRProlog
	str::string ErrorMessage;
ERRBegin
	ErrorMessage.Init( TargetLabel );

	if ( Target.Amount() != 0 ) {
		ErrorMessage.Append( " already defined" );
		ReportErrorAndExit_( ErrorMessage, IFlow );
	}

	if ( Browser.Value().Amount() == 0 ) {
		ErrorMessage.Append( " value can not be empty" );
		ReportErrorAndExit_( ErrorMessage, IFlow );
	}

	Target = Browser.Value();

ERRErr
ERREnd
ERREpilog
}

static inline bso::bool__ Test_( const str::string_ &Value )
{
	return Value.Amount() != 0;
}

static void ReportInsertionErrorAndExit_(
	const char *ItemLabel1,
	const char *ItemLabel2,
	xpp::preprocessing_iflow___ &IFlow )
{
ERRProlog
	str::string ErrorMessage;
ERRBegin
	ErrorMessage.Init( "Both " );
	ErrorMessage.Append( ItemLabel1 );
	ErrorMessage.Append( " and " );
	ErrorMessage.Append( ItemLabel2 );
	ErrorMessage.Append( " cannot be define together" );

	ReportErrorAndExit_( ErrorMessage, IFlow );
ERRErr
ERREnd
ERREpilog
}

// '...<erpck:insert ...>...' -> '...</erpck:insert>...'
//                   ^                              ^
static void ProcessInsertion_(
	xml::browser___ &Browser,
	xpp::preprocessing_iflow___ &IFlow,
	const aliases_ &Aliases,
	const tables_ &Tables,
	record_ &Record )
{
ERRProlog
	str::string TableLabel, RecordLabel, TableAlias, RecordAlias;
	bso::bool__ Continue = true;
ERRBegin
	TableLabel.Init();
	RecordLabel.Init();
	TableAlias.Init();
	RecordAlias.Init();

	while ( Continue ) {
		switch ( Browser.Browse( xml::tfStartTag |xml::tfAttribute | xml::tfValue | xml::tfEndTag ) ) {
		case xml::tStartTag:
			ReportErrorAndExit_( "No tag allowed", IFlow );
			break;
		case xml::tAttribute:
			if ( Browser.AttributeName() == INSERTION_TABLE_LABEL_ATTRIBUTE ) {
				Assign_( TableLabel, "Table label", Browser, IFlow );
			} else if ( Browser.AttributeName() == INSERTION_RECORD_LABEL_ATTRIBUTE ) {
				Assign_( RecordLabel, "Record label", Browser, IFlow );
			} else if ( Browser.AttributeName() == INSERTION_TABLE_ALIAS_ATTRIBUTE ) {
				Assign_( TableAlias, "Table alias", Browser, IFlow );
			} else if ( Browser.AttributeName() == INSERTION_RECORD_ALIAS_ATTRIBUTE ) {
				Assign_( RecordAlias, "Record alias", Browser, IFlow );
			} else
				ReportErrorAndExit_( "Unknown attribute", IFlow );
			break;
		case xml::tEndTag:
		{
			bso::bool__ 
				TA = Test_( TableAlias ),
				TL = Test_( TableLabel ),
				RA = Test_( RecordAlias ),
				RL = Test_( RecordLabel );

			if ( TA )
				if ( TL )
					ReportInsertionErrorAndExit_( "table alias", "table label", IFlow );
				else if ( RA )
					ReportInsertionErrorAndExit_( "table alias", "record alias", IFlow );
				else if ( !RL )
					ReportErrorAndExit_( "Record label missing", IFlow );
				else
					InsertUsingTableAlias_( TableAlias, RecordLabel, Aliases.Tables, Tables, IFlow, Record );
			else if ( TL )
				if ( RA )
					ReportInsertionErrorAndExit_( "table label", "record alias", IFlow );
				else if ( !RL )
					ReportErrorAndExit_( "Record label missing", IFlow );
				else
					InsertUsingLabels_( TableLabel, RecordLabel, Tables, IFlow, Record );
			else if ( RL )
				if ( RA )
					ReportInsertionErrorAndExit_( "record label", "record alias", IFlow );
				else
					ERRc();	// Normally already handled before.
			else if ( RA )
				InsertUsingRecordAlias_( RecordAlias, Aliases.Records, Tables, IFlow, Record );
			else
				ReportErrorAndExit_( "Missing insertion parameters", IFlow );

			Continue = false;
			break;
		}
		case xml::tValue:
			ReportErrorAndExit_( "No value allowed here", IFlow );
			break;
		case xml::tError:
			ReportErrorAndExit_( IFlow );
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

// '...<ercp:content ...>...<TAG ...>...' -> '...</TAG>...'
//                                   ^                 ^
static void ProcessRecord_(
	xml::browser___ &Browser,
	xpp::preprocessing_iflow___ &IFlow,
	const str::string_ &DefaultRecordLabelTag,
	const aliases_ &Aliases,
	const tables_ &Tables,
	record_ &Record )
{
	bso::ulong__ Level = 0;
	bso::bool__ Continue = true;

	while ( Continue ) {
		switch ( Browser.Browse( xml::tfStartTag | xml::tfAttribute | xml::tfValue | xml::tfEndTag ) ) {
		case xml::tStartTag:
			if ( BelongsToNamespace_( Browser.TagName() ) ) {
				if ( Browser.TagName() == INSERTION_TAG )
					ProcessInsertion_( Browser, IFlow, Aliases, Tables, Record );	// '...<erpck:insert ...
				else																//                   ^
					ReportErrorAndExit_( "'" INSERTION_TAG "' is the only '" NAMESPACE "' tag allowed here", IFlow );
			} else {
				Level++;
				Record.Content.Append( Browser.DumpData() );
			}
			break;
		case xml::tAttribute:
			if ( BelongsToNamespace_( Browser.AttributeName() ) )
				ReportErrorAndExit_( "Unexpected attribute", IFlow );

			Record.Content.Append( Browser.DumpData() );
			break;
		case xml::tValue:
			if ( Level == 0 )
				ReportErrorAndExit_( "Missing tag", IFlow );

			if ( Record.Label.Amount() == 0 )
				if ( Browser.TagName() == DefaultRecordLabelTag )
					Record.Label = Browser.Value();

			Record.Content.Append( Browser.DumpData() );
			break;
		case xml::tEndTag:
			switch ( Level ) {
			case 0:
				Continue = false;
				break;
			default:
				Level--;
				break;
			}
			Record.Content.Append( Browser.DumpData() );
			break;
		case xml::tError:
			ReportErrorAndExit_( IFlow );
			break;
		default:
			ERRc();
			break;
		}
	}
}

// '...<ercp:content ...><...' -> '...</erpck:content>...'
//                       ^                            ^
static void ProcessRecords_(
	xml::browser___ &Browser,
	xpp::preprocessing_iflow___ &IFlow,
	const str::string_ &DefaultRecordLabelTag,
	const aliases_ &Aliases,
	const tables_ &Tables,
	records_ &Records )
{
ERRProlog
	bso::bool__ Continue = true;
	record Record;
	weight__ Weight = DEFAULT_WEIGHT;
	bso::integer_buffer__ Buffer;
	bso::bool__ Ignore = false;
ERRBegin
	Record.Init();

	while ( Continue ) {
		switch ( Browser.Browse( xml::tfStartTag | xml::tfStartTagClosed | xml::tfAttribute | xml::tfEndTag ) ) {
		case xml::tStartTag:	// Mandatory, otherwise the tag name is missed when there is an attribute.
			Record.Content.Append( Browser.DumpData() );
			break;
		case xml::tAttribute:
			if ( Browser.AttributeName() == RECORD_WEIGHT_ATTRIBUTE ) {
				epeios::row__ Error = NONE;

				Weight = Browser.Value().ToUB( &Error );

				if ( Error != NONE )
					ReportErrorAndExit_( "Bad attribute value", IFlow );
			} else if ( Browser.AttributeName() == RECORD_LABEL_ATTRIBUTE ) {
					Assign_( Record.Label, "Record label", Browser, IFlow );
			} else if ( Browser.AttributeName() == RECORD_HANDLING_ATTRIBUTE ) {
				if ( Browser.Value() == RECORD_HANDLING_ATTRIBUTE_SKIP_VALUE )
					Record.Skip() = true;
				else if ( Browser.Value() == RECORD_HANDLING_ATTRIBUTE_IGNORE_VALUE )
					Ignore = true;
				else
					ReportErrorAndExit_( "Unknown attribute value", IFlow );
			} else if ( BelongsToNamespace_( Browser.AttributeName() ) ) {
				ReportErrorAndExit_( "Unknown attribute", IFlow );
			} else
				Record.Content.Append( Browser.DumpData() );
			break;
		case xml::tStartTagClosed:
			Record.Content.Append( " id=\"" );
			Record.Content.Append( bso::Convert( Records.Amount() + 1, Buffer ) );
			Record.Content.Append( '"' );
			Record.Content.Append( Browser.DumpData() );
			ProcessRecord_( Browser, IFlow, DefaultRecordLabelTag, Aliases, Tables, Record );	// '...<ercp:content ...>...<TAG ...>...' -> '...</TAG>...'
			if ( !Ignore )
				Records.Append( Record );															//                                   ^                 ^	
			else
				Ignore = false;
			Record.Init();
			break;														
		case xml::tEndTag:
			Continue = false;	// '...<ercp:content ...></ercp:content>...'
			break;				//                                      ^
		case xml::tError:
			ReportErrorAndExit_( IFlow );
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

// '...<erpck::table ...>...<erpck:content ...>...' -> '...</erpck:content>...'
//                                         ^                               ^
static void ProcessContent_(
	xml::browser___ &Browser,
	xpp::preprocessing_iflow___ &IFlow,
	table_ &Table,
	const tables_ &Tables,
	const aliases_ &Aliases )
{
ERRProlog
	bso::bool__ Continue = true;
	str::string DefaultRecordLabelTag;
ERRBegin
	DefaultRecordLabelTag.Init();

	while ( Continue ) {
		switch ( Browser.Browse( xml::tfAttribute | xml::tfStartTagClosed | xml::tfEndTag ) ) {
		case xml::tAttribute:
			if ( Browser.TagName() != CONTENT_TAG )
				ERRc();

			if ( Browser.AttributeName() == CONTENT_DEFAULT_RECORD_LABEL_TAG_ATTRIBUTE ) {
				Assign_( DefaultRecordLabelTag, "Default record label", Browser, IFlow );
			} else
				ReportErrorAndExit_( "Unknown attribute", IFlow );
			break;
		case xml::tStartTagClosed:
			if ( Browser.TagName() == CONTENT_TAG ) {
				ProcessRecords_( Browser, IFlow, DefaultRecordLabelTag, Aliases, Tables, Table.Records );	// '<ercp:content ...><...' -> '</erpck:content>...'
				Continue = false;
			}  else														        			//                    ^                         ^
				ERRc();
			break;
		case xml::tEndTag:
			Continue = false;	// '</erpck:content>...'
			break;				//                  ^
		case xml::tError:
			ReportErrorAndExit_( IFlow );
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

enum alias_type__ {
	atRecord,
	atTable,
	at_amount,
	at_Undefined
};
// '...<erpck:alias ...>...' -> '...</alias>...'
//                  ^                       ^
static alias_type__ ProcessAlias_(
	xml::browser___ &Browser,
	xpp::preprocessing_iflow___ &IFlow,
	const table_aliases_ &TableAliases,
	const tables_ &Tables,
	record_alias_ &RecordAlias,
	table_alias_ &TableAlias )
{
	alias_type__ AliasType = at_Undefined;
ERRProlog
	bso::bool__ Continue = true;
	trow__ TableRow = NONE;
	rrow__ RecordRow = NONE;
	str::string TableAliasLabel, TableLabel, RecordLabel, AliasLabel;
ERRBegin
	TableAliasLabel.Init();
	TableLabel.Init();
	RecordLabel.Init();
	AliasLabel.Init();

	while ( Continue ) {
		switch ( Browser.Browse( xml::tfStartTag | xml::tfStartTagClosed | xml::tfAttribute | xml::tfValue | xml::tfEndTag ) ) {
		case xml::tStartTag:
			ReportErrorAndExit_( "No tag allowed here", IFlow );
			break;
		case xml::tAttribute:
			if ( Browser.AttributeName() == ALIAS_TABLE_ALIAS_ATTRIBUTE ) {
				if ( TableLabel.Amount() != 0 )
					ReportErrorAndExit_( "Both '" ALIAS_TABLE_LABEL_ATTRIBUTE "' and '" ALIAS_TABLE_ALIAS_ATTRIBUTE "' cannot be defined together", IFlow );

				Assign_( TableAliasLabel, "Table alias", Browser, IFlow );
			} else if ( Browser.AttributeName() == ALIAS_TABLE_LABEL_ATTRIBUTE ) {
				if ( TableAliasLabel.Amount() != 0 )
					ReportErrorAndExit_( "Both '" ALIAS_TABLE_LABEL_ATTRIBUTE "' and '" ALIAS_TABLE_ALIAS_ATTRIBUTE "' cannot be defined together", IFlow );

				Assign_( TableLabel, "Table alias", Browser, IFlow );
			} else if ( Browser.AttributeName() == ALIAS_RECORD_LABEL_ATTRIBUTE ) {
				Assign_( RecordLabel, "Record label", Browser, IFlow );
			} else if ( Browser.AttributeName() == ALIAS_LABEL_ATTRIBUTE ) {
				Assign_( AliasLabel, "Alias label", Browser, IFlow );
			} else
				ReportErrorAndExit_( "Unknown attribute", IFlow );
			break;
		case xml::tStartTagClosed:
			if ( TableLabel.Amount() != 0 ) {
				if ( ( TableRow = SearchTable( TableLabel, Tables ) ) == NONE )
					ReportErrorAndExit_( "Unable to find table", IFlow );
			} else if ( TableAliasLabel.Amount() != 0 ) {
				if ( ( TableRow = SearchTable( TableAliasLabel, TableAliases ) ) == NONE )
					ReportErrorAndExit_( "Unable to find table", IFlow );
			} else
				ReportErrorAndExit_( "Missing table reference", IFlow );

			if ( AliasLabel.Amount() == 0 )
				ReportErrorAndExit_( "Alias label missing", IFlow );

			if ( RecordLabel.Amount() ) {
				if ( ( RecordRow = SearchRecord( RecordLabel, TableRow, Tables ) ) == NONE )
					ReportErrorAndExit_( "Unable to find record", IFlow );

				AliasType = atRecord;
			} else
				AliasType = atTable;

			switch ( AliasType ) {
			case atRecord:
				RecordAlias.Init( AliasLabel, TableRow, RecordRow );
				break;
			case atTable:
				TableAlias.Init( AliasLabel, TableRow );
				break;
			break;
			default:
				ERRc();
			}
			break;
		case xml::tValue:
			ReportErrorAndExit_( "No value allowed here", IFlow );
			break;
		case xml::tEndTag:
			switch ( AliasType ) {
			case atRecord:
			case atTable:
				Continue = false;
				break;
			case at_Undefined:
				ReportErrorAndExit_( "Incomplete alias definition", IFlow );
				break;
			default:
				ERRc();
				break;
			}
			break;
		case xml::tError:
			ReportErrorAndExit_( IFlow );
			break;
		default:
			ERRc();
			break;
		}
	}
ERRErr
ERREnd
ERREpilog
	return AliasType;
}

 // '<erpck:table ...>...<erpck:aliases ...>...'
//                                     ^
 static void ProcessAliases_(
	xml::browser___ &Browser,
	xpp::preprocessing_iflow___ &IFlow,
	const tables_ &Tables,
	aliases_ &Aliases )
{
ERRProlog
	bso::bool__ Continue = true;
	record_alias RecordAlias;
	table_alias TableAlias;
ERRBegin
	while ( Continue ) {
		switch ( Browser.Browse( xml::tfStartTag | xml::tfEndTag ) ) {
		case xml::tStartTag:
			if ( Browser.TagName() == ALIAS_TAG ) {
				RecordAlias.Init();
				TableAlias.Init();

				switch ( ProcessAlias_( Browser, IFlow, Aliases.Tables, Tables, RecordAlias, TableAlias ) ) {	// '...<erpck:alias ...>...' -> '...</alias>...'
				case atRecord:																					//                  ^                       ^
					Aliases.Records.Append( RecordAlias );
					break;
				case atTable:
					Aliases.Tables.Append( TableAlias );
					break;
				default:
					ERRc();
					break;
				}
			} else
				ReportErrorAndExit_( "Unknow tag", IFlow );
			break;
		case xml::tEndTag:
			Continue = false;
			break;
		case xml::tError:
			ReportErrorAndExit_( IFlow );
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

// '...<erpck:data><erpck::table ...>' -> '..</erpck:table>...'
//                               ^                         ^
static void ProcessTable_(
	xml::browser___ &Browser,
	xpp::preprocessing_iflow___ &IFlow,
	table_ &Table,
	const tables_ &Tables )
{
ERRProlog
	bso::bool__ Continue = true;
	aliases Aliases;
ERRBegin
	Aliases.Init();

	while ( Continue ) {
		switch ( Browser.Browse( xml::tfStartTag | xml::tfAttribute | xml::tfEndTag ) ) {
		case xml::tStartTag:
			if ( Browser.TagName() == ALIASES_TAG )
				ProcessAliases_( Browser, IFlow, Tables, Aliases );	// '<erpck:table ...>...<erpck:aliases ...>...'
			else if ( Browser.TagName() == CONTENT_TAG )			//                                     ^
				ProcessContent_( Browser, IFlow, Table, Tables, Aliases );	// '<erpck:table ...>...<erpck:content ...>...' -> '...</erpc:content>...'
			else															//                                     ^                              ^
				ReportErrorAndExit_( "Unknown tag", IFlow );
			break;
		case xml::tAttribute:
			if ( Browser.TagName() != TABLE_TAG )
				ERRc();

			if ( Browser.AttributeName() == TABLE_LABEL_ATTRIBUTE ) {
				Assign_( Table.Label, "Table label", Browser, IFlow );
			} else
				ReportErrorAndExit_( "Unknown attribute", IFlow );
			break;
		case xml::tEndTag:
			Continue = false;
			break;
		case xml::tError:
			ReportErrorAndExit_( IFlow );
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

// '...<erpck:data><erpck:table ...>' -> '...</erpck:table>...'
//                 ^                                       ^
static void ProcessData_(
	xml::browser___ &Browser,
	xpp::preprocessing_iflow___ &IFlow,
	data_ &Data )
{
ERRProlog
	bso::bool__ Continue = true;
	bso::bool__ TableDetected = false;
	table Table;
ERRBegin
	while ( Continue ) {
		switch ( Browser.Browse( xml::tfStartTag | xml::tfEndTag ) ) {
		case xml::tStartTag:
			if ( Browser.TagName() == TABLE_TAG ) {
				TableDetected = true;
				Table.Init();
				ProcessTable_( Browser, IFlow, Table, Data );	// '...<erpck::table ...><erpck:content>...' -> '....</erpck:table>...'
				Data.Append( Table );							//                   ^                                             ^
			} else
				ReportErrorAndExit_( "Missing '" TABLE_TAG "'", IFlow );
			break;
		case xml::tEndTag:
			if ( !TableDetected ) {
				cerr << "No '" TABLE_TAG "' tag in data file !" << txf::nl;
				ERRExit( EXIT_FAILURE );
			} else
				Continue = false;
			break;
		case xml::tError:
			ReportErrorAndExit_( IFlow );
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

static void RetrieveData_(
	const char *DataFileName,
	data_ &Data )
{
ERRProlog
	flf::file_iflow___ FFlow;
	xpp::preprocessing_iflow___ IFlow;
	xml::browser___ Browser;
	bso::bool__ Continue = true;
	lcl::locales Locales;
	bso::bool__ DataDetected = false;
	FNM_BUFFER___ Buffer;
	xtf::extended_text_iflow__ XFlow;
ERRBegin
	if ( FFlow.Init( DataFileName, err::hSkip ) != fil::sSuccess ) {
		cerr << "Unable to open data file '" << DataFileName << "' !" << txf::nl;
		ERRExit( EXIT_FAILURE );
	}

	FFlow.EOFD( XTF_EOXT );

	IFlow.Init( FFlow, str::string( fnm::GetLocation( DataFileName, Buffer ) ) );

	XFlow.Init( IFlow );

	Browser.Init( XFlow, xml::eh_Default );

	Data.Init();

	while ( Continue ) {
		switch ( Browser.Browse( xml::tfStartTagClosed |xml::tfAttribute ) ) {
		case xml::tStartTagClosed:
			if ( ( Browser.TagName() == DATA_TAG ) ) {
				ProcessData_( Browser, IFlow, Data );	// '...<erpck:data><erpck:table ...>' -> '...</erpck:table>...'
				DataDetected = true;					//                 ^                                       ^
			} else {
				ReportErrorAndExit_( "Unexpected tag", IFlow );
			} 
			break;
		case xml::tAttribute:
			ReportErrorAndExit_( "Unexpected attribute", IFlow );
			break;
		case xml::tProcessed:
			if ( !DataDetected ) {
				cerr << "No '" << NAME << "' data in '" << DataFileName << "' !" << txf::nl;
				ERRExit( EXIT_FAILURE );
			} else
				Continue = false;
			break;
		case xml::tError:
			ReportErrorAndExit_( IFlow );
			break;
		default:
			ERRc();
			break;
		}
	}

	tol::InitializeRandomGenerator();
ERRErr
ERREnd
ERREpilog
}

static void Display_(
	const str::string_ &XML,
	xml::writer_ &Writer )
{
ERRProlog
	flx::E_STRING_IFLOW__ IFlow;
ERRBegin
	IFlow.Init( XML );

	if ( xpp::Process( IFlow, Writer ) != xpp::sOK )
		ERRc();
ERRErr
ERREnd
ERREpilog
}

static void Display_(
	const record_ &Record,
	xml::writer_ &Writer )
{
	Display_( Record.Content, Writer );
}

static void Display_(
	rrow__ Row,						
	const records_ &Records,
	xml::writer_ &Writer )
{
	ctn::E_CITEMt( record_, rrow__ ) Record;

	Record.Init( Records );

	Display_( Record( Row ), Writer );
}

static void DisplayAll_(
	const records_ &Records,
	xml::writer_ &Writer )
{
	ctn::E_CITEMt( record_, rrow__ ) Record;
	rrow__ Row = Records.First();

	Record.Init( Records );

	while ( Row != NONE ) {
		Display_( Record( Row ), Writer );

		Row = Records.Next( Row );
	}
}

id__  Display_(
	id__ Id,
	const records_ &Records,
	rpkctx::context_ &Context,
	xml::writer_ &Writer )
{
	rrow__ Row = NONE;
	bso::integer_buffer__ Buffer;
	ctn::E_CITEMt( record_, rrow__ ) Record;
	epeios::size__ Counter = 0;

	Record.Init( Records );

	Writer.PutAttribute( "TotalAmount", bso::Convert( Records.Amount(), Buffer ) );

	if ( Id == ALL ) {
		Writer.PutAttribute( "Amount", bso::Convert( Records.Amount(), Buffer ) );
		DisplayAll_( Records, Writer );
	} else {
		Writer.PutAttribute( "Amount", "1" );

		if ( Id == 0 ) {
/*
			tol::InitializeRandomGenerator();
			Row = Records.First( rand() % Records.Amount() );
*/
			do {
				Row = Context.Pick( Records.Amount() );
				Counter++;
			} while ( Record( Row ).GetSkip() && ( Counter < Records.Amount() ) );

			if ( Record( Row ).GetSkip() ) {
				cout << "Unable to pick a record : all are marked as skipped !" << txf::nl;
				ERRExit( EXIT_SUCCESS );
			}

			Id = *Row + 1;

		} else {
			if ( Id > Records.Amount() ) {
				cerr << "No record of id '" << Id << "'! " << txf::nl;
				ERRExit( EXIT_FAILURE )
			} else
				Row = Id - 1;
		}

		Display_( Row, Records, Writer );
	}

	return Id;
}

static id__ Display_(
	id__ Id,
	const table_ &Table,
	const str::string_ &XSLFileName,
	rpkctx::context_ &Context,
	txf::text_oflow__ &Output )
{
ERRProlog
	xml::writer Writer;
ERRBegin
	xml::WriteXMLHeader( Output );
	Output << txf::nl;

	if ( XSLFileName.Amount() != 0 ) {
		xml::WriteXSLDeclaration( XSLFileName, Output );
		Output << txf::nl;
	}

	Writer.Init( Output, xml::oIndent );
	Writer.PushTag( Table.Label );

	Id = Display_( Id, Table.Records, Context, Writer );	

	Writer.PopTag();

ERRErr
ERREnd
ERREpilog
	return Id;
}

static id__ Display_(
	id__ Id,
	const data_ &Data,
	const str::string_ &XSLFileName,
	rpkctx::context_ &Context,
	txf::text_oflow__ &Output )
{
	ctn::E_CITEMt( table_, trow__ ) Table;

	Table.Init( Data );

	return  Display_( Id, Table( Data.Last() ), XSLFileName, Context, Output );
}

static id__ DisplayWithoutBackup_(
	id__ Id,
	const data_ &Data,
	const str::string_ &XSLFileName,
	rpkctx::context_ &Context,
	const char *FileName )
{
ERRProlog
	flf::file_oflow___ FFlow;
	txf::text_oflow__ TFlow( FFlow );
ERRBegin
	if ( FFlow.Init( FileName ) != fil::sSuccess ) {
		cerr << "Unable to open '" << FileName << "' for output !" << txf::nl;
		ERRExit( EXIT_FAILURE );
	}

	Id = Display_( Id, Data, XSLFileName, Context, TFlow );
ERRErr
ERREnd
ERREpilog
	return Id;
}

static id__ Display_(
	id__ Id,
	const data_ &Data,
	const str::string_ &XSLFileName,
	rpkctx::context_ &Context,
	const char *FileName )
{
ERRProlog
	bso::bool__ Backuped = false;
ERRBegin
	fil::CreateBackupFile( FileName, fil::hbfRename );

	Backuped = true;

	Id = DisplayWithoutBackup_( Id, Data, XSLFileName, Context, FileName );
ERRErr
	if ( Backuped )
		fil::RecoverBackupFile( FileName );
ERREnd
ERREpilog
	return Id;
}

static id__ Display_(
	id__ Id,
	const data_ &Data,
	const str::string_ &XSLFileName,
	rpkctx::context_ &Context,
	const str::string_ &OutputFileName )
{
ERRProlog
	STR_BUFFER___ Buffer;
ERRBegin
	if ( OutputFileName.Amount() == 0 )
		Id = Display_( Id, Data, XSLFileName, Context, cout );
	else
		Id = Display_( Id, Data, XSLFileName, Context, OutputFileName.Convert( Buffer ) );
ERRErr
ERREnd
ERREpilog
	return Id;
}

#define DATA_FILENAME_TAG		"Data"
#define OUTPUT_FILENAME_TAG		"Output"
#define XSL_FILENAME_TAG		"XSL"
#define COMMAND_TAG				"Command"
#define CONTEXT_FILENAME_TAG	"Context"

void LaunchCommand_(
	const str::string_ &Command,
	id__ Id,
	const str::string_ &OutputFileName )
{
ERRProlog
	str::string CompleteCommand;
	STR_BUFFER___ Buffer;
ERRBegin
	if ( ( Command.Amount() != 0 ) && ( OutputFileName.Amount() != 0 ) ) {
		CompleteCommand.Init( Command );
		str::ReplaceTag( CompleteCommand, 1, OutputFileName, '$' );
		str::ReplaceTag( CompleteCommand, 2, str::string( bso::Convert( Id ) ), '$' );
		cout << "Launching '" << CompleteCommand << "\"." << txf::nl;
		system( CompleteCommand.Convert( Buffer ) );
	}


ERRErr
ERREnd
ERREpilog
}

static void DumpContext_(
	const rpkctx::context_ &Context,
	xml::writer_ &Writer )
{
	Writer.PushTag( "Context" );
	Writer.PutAttribute( "Target", NAME );

	Dump( Context, Writer );

	Writer.PopTag();
}

static void DumpContextWithoutBackup_(
	const rpkctx::context_ &Context,
	const char *FileName )
{
ERRProlog
	flf::file_oflow___ FFlow;
	txf::text_oflow__ TFlow( FFlow );
	xml::writer Writer;
ERRBegin
	if ( FFlow.Init( FileName ) != fil::sSuccess ) {
		cerr << "Unable to open file '" << FileName << "'!" << txf::nl;
		ERRExit( EXIT_FAILURE );
	}

	Writer.Init( TFlow, xml::oIndent );

	DumpContext_( Context, Writer ); 
ERRErr
ERREnd
ERREpilog
}


static void DumpContext_(
	const rpkctx::context_ &Context,
	const char *FileName )
{
ERRProlog
	bso::bool__ Backuped = false;
ERRBegin
	if ( fil::CreateBackupFile( FileName, fil::hbfRename ) != fil::rbfOK ) {
		cerr << "Unable to create backup file for '" << FileName << "'!" << txf::nl;
		ERRExit( EXIT_FAILURE );
	}

	Backuped = true;

	DumpContextWithoutBackup_( Context, FileName );
ERRErr
	if ( Backuped )
		fil::RecoverBackupFile( FileName );
ERREnd
ERREpilog
}

static void RetrieveContext_(
	xml::browser___ &Browser,							 
	rpkctx::context_ &Context )
{
ERRProlog
	bso::bool__ Continue = true;
	str::string Target;
ERRBegin

	Target.Init();

	while ( Continue ) {
		switch ( Browser.Browse( xml::tfObvious | xml::tfStartTagClosed ) ) {
		case xml::tStartTag:
			if ( Browser.TagName() != "Context" )
				ERRc();

			break;
		case xml::tAttribute:
			if ( Browser.AttributeName() != "Target" )
				ERRc();

			if ( Target.Amount() != 0 )
				ERRc();

			Target = Browser.Value();
			break;
		case xml::tStartTagClosed:
			if ( Target != NAME )
				ERRc();

			rpkctx::Retrieve( Browser, Context );
			break;
		case xml::tProcessed:
			Continue = false;
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

static void RetrieveContext_(
	const char *FileName,
	rpkctx::context_ &Context )
{
ERRProlog
	xml::browser___ Browser;
	flf::file_iflow___ FFlow;
	xtf::extended_text_iflow__ XFlow;
ERRBegin
	if ( !fil::FileExists( FileName ) ) {
		cout << "Unable to find context file '" << FileName << "'! It will be created at exit." << txf::nl;
		ERRReturn;
	}

	if ( FFlow.Init( FileName ) != fil::sSuccess ) {
		cerr << "Unable to open file '" << FileName << "' !" << txf::nl;
		ERRExit( EXIT_FAILURE );
	}

	FFlow.EOFD( XTF_EOXT );

	XFlow.Init( FFlow );

	Browser.Init( XFlow, xml::eh_Default );

	RetrieveContext_( Browser, Context );
ERRErr
ERREnd
ERREpilog
}

void Process_(
	id__ Id,				
	const rgstry::registry_ &Registry,
	rgstry::row__ RegistryRoot )
{
ERRProlog
	str::string DataFileName;
	STR_BUFFER___ Buffer;
	data Data;
	rpkctx::context Context;
	str::string OutputFileName;
	str::string XSLFileName;
	str::string Command;
	str::string ContextFileName;
ERRBegin
	DataFileName.Init();
	if ( !Registry.GetValue( str::string( DATA_FILENAME_TAG ), RegistryRoot, DataFileName ) ) {
		cerr << "No '" << DATA_FILENAME_TAG << "' tag found !" << txf::nl;
		ERRExit( EXIT_FAILURE );
	}

	OutputFileName.Init();
	if ( !Registry.GetValue( str::string( OUTPUT_FILENAME_TAG ), RegistryRoot, OutputFileName ) ) {
		cerr << "No output file defined !" << txf::nl;
		ERRExit( EXIT_FAILURE );
	}

	XSLFileName.Init();
	Registry.GetValue( str::string( XSL_FILENAME_TAG ), RegistryRoot, XSLFileName );

	ContextFileName.Init();
	if ( !Registry.GetValue( str::string( CONTEXT_FILENAME_TAG ), RegistryRoot, ContextFileName ) ) {
		cerr << "No context file name defined !" << txf::nl;
		ERRExit( EXIT_FAILURE );
	}

	Context.Init();
	RetrieveContext_( ContextFileName.Convert( Buffer ), Context );

	Data.Init();
	RetrieveData_( DataFileName.Convert( Buffer ), Data );

	Id = Display_( Id, Data, XSLFileName, Context, OutputFileName );

	Command.Init();
	Registry.GetValue( str::string( COMMAND_TAG ), RegistryRoot, Command );

	DumpContext_( Context, ContextFileName.Convert( Buffer ) );

	LaunchCommand_( Command, Id, OutputFileName );
ERRErr
ERREnd
ERREpilog
}

static void Process_(
	id__ Id,						
	const char *Project )
{
ERRProlog
	rgstry::error__ Error = rgstry::e_Undefined;
	rgstry::error_details ErrorDetails;
	rgstry::registry Registry;
	lcl::locales Locales;
	str::string ErrorMessage;
	rgstry::row__ RegistryRoot = NONE;
ERRBegin
	Registry.Init();
	ErrorDetails.Init();

	if ( ( Error = rgstry::FillRegistry( Project, "Projects/Project[target=\"" NAME "\"]", Registry, RegistryRoot, ErrorDetails ) ) != rgstry::eOK ) {
		Locales.Init();
		ErrorMessage.Init();
		cerr << rgstry::GetTranslation( Error, ErrorDetails,  str::string(), Locales, ErrorMessage ) << txf::nl;
		ERRExit( EXIT_FAILURE );
	}

	Process_( Id, Registry, RegistryRoot );
ERRErr
ERREnd
ERREpilog
}

void Go( const parameters &Parameters )
{
ERRProlog
ERRBegin
	Process_( Parameters.Id, Parameters.Project );
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
