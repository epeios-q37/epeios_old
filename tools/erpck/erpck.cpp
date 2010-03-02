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
#include "xml.h"
#include "fnm.h"
#include "flf.h"
#include "dtr.h"
#include "lcl.h"

#define NAME					"erpck"
#define VERSION					"0.1.0"
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

#define STRING_PARAM( name )	CLNARG_STRING_PARAM( name )

struct parameters {
	STRING_PARAM( Project );
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
	cout << NAME << " <command> [options] <project-file>" << txf::nl;
	cout << txf::tab << "project-file:" << txf::tab << "file containing the project defintions." << txf::nl;
	cout << "command: " << txf::nl;
	clnarg::PrintCommandUsage( Description, cPick, "Pick a random entry according to project file.", clnarg::vSplit, true );
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
	epeios::row__ P;
ERRBegin
	Free.Init();

	Analyzer.GetArguments( Free );

	P = Free.Last();

	switch( Free.Amount() ) {
	case 1:
		Parameters.Project = Free( P ).Convert( Parameters.ProjectBuffer );
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

typedef bso::ubyte__ weight__;
#define DEFAULT_WEIGHT	1

E_ROW( rrow__ );	// 'record row'.
E_ROW( trow__ );	// 'table row'.


class record_alias_
{
public:
	struct s {
		str::string_::s Label;
		trow__ TableRow;
		rrow__ RecordRow;
	}&S_;
	str::string_ Label;
	record_alias_( s &S )
	: S_( S ),
	  Label( S.Label )
	{}
	void reset( bso::bool__ P = true )
	{
		Label.reset( P );

		S_.TableRow = NONE;
		S_.RecordRow = NONE;
	}
	void plug( mdr::E_MEMORY_DRIVER__ &MD )
	{
		Label.plug( MD );
	}
	void plug( mmm::E_MULTIMEMORY_ &MM )
	{
		Label.plug( MM );
	}
	record_alias_ &operator =( const record_alias_ &RA )
	{
		Label = RA.Label;

		S_.TableRow = RA.S_.TableRow;
		S_.RecordRow = RA.S_.RecordRow;

		return *this;
	}
	void Init( void )
	{
		reset();

		Label.Init();
	}
	void Init(
		const str::string_ &LAbel,
		trow__ TableRow,
		rrow__ RecordRow )
	{
		reset();

		this->Label.Init( Label );

		S_.TableRow = TableRow;
		S_.RecordRow = RecordRow;
	}
	E_RODISCLOSE_( trow__, TableRow );
	E_RODISCLOSE_( rrow__, RecordRow );
};

E_AUTO( record_alias );

typedef ctn::E_XMCONTAINER_( record_alias_ ) record_aliases_;
E_AUTO( record_aliases )

class table_alias_
{
public:
	struct s {
		str::string_::s Label;
		trow__ TableRow;
	}&S_;
	str::string_ Label;
	table_alias_( s &S )
	: S_( S ),
	  Label( S.Label )
	{}
	void reset( bso::bool__ P = true )
	{
		Label.reset( P );

		S_.TableRow = NONE;
	}
	void plug( mdr::E_MEMORY_DRIVER__ &MD )
	{
		Label.plug( MD );
	}
	void plug( mmm::E_MULTIMEMORY_ &MM )
	{
		Label.plug( MM );
	}
	table_alias_ &operator =( const table_alias_ &TA )
	{
		Label = TA.Label;
		S_.TableRow = TA.S_.TableRow;

		return *this;
	}
	void Init( void )
	{
		reset();

		Label.Init();
	}
	void Init(
		const str::string_ &Label,
		trow__ TableRow )
	{
		reset();

		this->Label.Init( Label );
		S_.TableRow = TableRow;
	}
	E_RODISCLOSE_( trow__, TableRow );
};

E_AUTO( table_alias );


typedef ctn::E_XMCONTAINER_( table_alias_ ) table_aliases_;
E_AUTO( table_aliases )

class aliases_ {
public:
	struct s {
		record_aliases_::s Records;
		table_aliases_::s Tables;
	};
	record_aliases_ Records;
	table_aliases_ Tables;
	aliases_ ( s &S )
	: Records( S.Records ),
	  Tables( S.Tables )
	{}
	void reset( bso::bool__ P = true )
	{
		Records.reset( P );
		Tables.reset( P );
	}
	void plug( mmm::E_MULTIMEMORY_ &MM )
	{
		Records.plug( MM );
		Tables.plug( MM );
	}
	aliases_ &operator =( const aliases_ &A )
	{
		Records = A.Records;
		Tables = A.Tables;

		return *this;
	}
	void Init( void )
	{
		reset();

		Records.Init();
		Tables.Init();
	}
};

E_AUTO( aliases )

class record_
{
public:
	struct s
	{
		str::string_::s
			Label,
			Content;
		weight__ Weight;
	} &S_;
	str::string_
		Label,
		Content;
	record_( s &S )
	: S_( S ),
	  Label( S.Label ),
	  Content( S.Content )
	{
	}
	void reset( bso::bool__ P = true )
	{
		Label.reset( P );
		Content.reset( P );

		S_.Weight = DEFAULT_WEIGHT;
	}
	void plug( mmm::E_MULTIMEMORY_ &MM )
	{
		Label.plug( MM );
		Content.plug( MM );
	}
	record_ &operator =( const record_ &R )
	{
		Label = R.Label;
		Content = R.Content;

		S_.Weight = R.S_.Weight;

		return *this;
	}
	void Init( void )
	{
		reset();

		Label.Init();
		Content.Init();

	}
	void Init(
		const str::string_ &Label,
		const str::string_ &Content,
		weight__ Weight )
	{
		reset();

		this->Label.Init( Label );
		this->Content.Init( Content );

	}
	E_RWDISCLOSE_( weight__, Weight );
};

E_AUTO( record )

typedef ctn:: E_XCONTAINERt_( record_, rrow__ ) records_;
E_AUTO( records )

class table_ {
public:
	struct s {
		str::string_::s Label;
		records_::s Records;
		aliases_::s Aliases;
	};
	str::string_ Label;
	records_ Records;
	aliases_ Aliases;
	table_( s &S )
	: Label( S.Label ),
	  Records( S.Records ),
	  Aliases( S.Aliases )
	{};
	void reset( bso::bool__ P = true )
	{
		Label.reset( P );
		Records.reset( P );
		Aliases.reset( P );
	}
	void plug( mmm::E_MULTIMEMORY_ &MM )
	{
		Label.plug( MM );
		Records.plug( MM );
		Aliases.plug( MM );
	}
	table_ &operator =(const table_ &T )
	{
		Label = T.Label;
		Records = T.Records;
		Aliases = T.Aliases;

		return *this;
	}
	void Init( void )
	{
		reset();

		Label.Init();
		Records.Init();
		Aliases.Init();
	}
};

E_AUTO( table )

typedef ctn::E_XCONTAINERt_( table_, trow__ ) tables_;
E_AUTO( tables );

typedef tables_	data_;
E_AUTO( data )

#define NAMESPACE	NAME ":"

#define DATA_TAG	NAMESPACE "data"

#define TABLE_TAG				NAMESPACE "table"
#define TABLE_LABEL_ATTRIBUTE	"label"

#define ALIASES_TAG	NAMESPACE "aliases"

#define ALIAS_TAG									NAMESPACE "alias"
#define ALIAS_TABLE_LABEL_ATTRIBUTE					"TableLabel"
#define ALIAS_TABLE_ALIAS_ATTRIBUTE					"TableAlias"
#define ALIAS_RECORD_LABEL_ATTRIBUTE				"RecordLabel"

#define CONTENT_TAG									NAMESPACE "content"
#define CONTENT_DEFAULT_RECORD_LABEL_TAG_ATTRIBUTE	"DefaultRecordLabelTag"

#define RECORD_WEIGHT_ATTRIBUTE					NAMESPACE "weight"
#define RECORD_LABEL_ATTRIBUTE					NAMESPACE "label"

#define INSERTION_TAG						NAMESPACE "insert"
#define INSERTION_TABLE_LABEL_ATTRIBUTE		"TableLabel"
#define INSERTION_RECORD_LABEL_ATTRIBUTE	"RecordLabel"
#define INSERTION_TABLE_ALIAS_ATTRIBUTE		"TableLabel"
#define INSERTION_RECORD_ALIAS_ATTRIBUTE	"RecordLabel"


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
	const xml::browser___ &Browser,
	txf::text_oflow__ &TFlow )
{
	PrintPosition_( Browser.GetCurrentCoord(), TFlow );
}

static void ReportErrorAndExit_( 
	const char *Message,
	xml::browser___ &Browser )
{
	cerr << Message << " at ";
	PrintPosition_( Browser.Dump().Coord(), cerr );
	cerr << " !" << txf::nl;
	ERRExit( EXIT_FAILURE );
}

static void ReportErrorAndExit_( xml::browser___ &Browser )
{
ERRProlog
	lcl::locales Locales;
	str::string ErrorMessage;
ERRBegin
	Locales.Init();
	ErrorMessage.Init();
	cerr << xml::GetTranslation( Browser.Status(), str::string(), Locales, ErrorMessage ) << " at ";
	PrintPosition_( Browser, cerr );
	cerr << " !" << txf::nl;
	ERRExit( EXIT_FAILURE );
ERRErr
ERREnd
ERREpilog
}

template <typename container, typename item, typename row> row Search_(
	const str::string_ &Label,
	const container &Container )
{
	ctn::E_CITEMt( item, row ) Item;
	row Row = Container.First();

	Item.Init( Container );

	while ( ( Row != NONE ) && ( Item( Row ).Label != Label ) )
		Row = Container.Next( Row );

	return Row;
}

static trow__ SearchTable_(
	const str::string_ &Label,
	const tables_ &Tables )
{
	return Search_<tables_, table_, trow__>( Label, Tables );
}

static trow__ SearchTable_(
	const str::string_ &Label,
	const table_aliases_ &Aliases )
{
	ctn::E_CMITEM( table_alias_ ) Alias;
	epeios::row__ Row = Aliases.First();

	Alias.Init( Aliases );

	if ( ( Row != NONE ) && ( Alias( Row ).Label != Label ) )
		Row = Aliases.Next( Row );

	if ( Row != NONE )
		return Alias( Row ).TableRow();
	else
		return NONE;
}


static rrow__ SearchRecord_(
	const str::string_ &Label,
	const records_ &Records )
{
	return Search_<records_, record_, rrow__>( Label, Records );
}

static rrow__ SearchRecord_(
	const str::string_ &Label,
	trow__ TableRow,
	const tables_ &Tables )
{
	ctn::E_CITEMt( table_, trow__ ) Table;

	Table.Init( Tables );

	return SearchRecord_( Label, Table( TableRow ).Records );
}

static void Insert_(
	rrow__ Row,
	const records_ &Records,
	record_ &Record )
{
	ctn::E_CITEMt( record_, rrow__ ) SourceRecord;

	SourceRecord.Init( Records );

	Record.Content.Append( SourceRecord( Row ).Content );
}

static void Insert_(
	const str::string_ &RecordLabel,
	const records_ &Records,
	xml::browser___ &Browser,
	record_ &Record )
{
	rrow__ Row = SearchRecord_( RecordLabel, Records );

	if ( Row == NONE )
		ReportErrorAndExit_( "Unable to find record", Browser );

	Insert_( Row, Records, Record );
}

static void Insert_(
	const str::string_ &RecordLabel,
	const table_ &Table,
	xml::browser___ &Browser,
	record_ &Record )
{
	Insert_( RecordLabel, Table.Records, Browser, Record );
}

static void Insert_(
	const str::string_ &TableLabel,
	const str::string_ &RecordLabel,
	const tables_ &Tables,
	xml::browser___ &Browser,
	record_ &Record )
{
	trow__ Row = SearchTable_( TableLabel, Tables );
	ctn::E_CITEMt( table_, trow__ ) Table;

	if ( Row == NONE )
		ReportErrorAndExit_( "Unable to find table", Browser );

	Table.Init( Tables );

	Insert_( RecordLabel, Table( Row ), Browser, Record );
}
					

// '...<erpck:insert ...>...' -> '...</erpck:insert>...'
//                   ^                              ^
static void ProcessInsertion_(
	xml::browser___ &Browser,
	const tables_ &Tables,
	record_ &Record )
{
ERRProlog
	str::string TableLabel, RecordLabel;
	bso::bool__ Continue = true;
ERRBegin
	TableLabel.Init();
	RecordLabel.Init();

	while ( Continue ) {
		switch ( Browser.Browse( xml::tfStartTag |xml::tfAttribute | xml::tfValue | xml::tfEndTag ) ) {
		case xml::tStartTag:
			ReportErrorAndExit_( "No tag allowed", Browser );
			break;
		case xml::tAttribute:
			if ( Browser.AttributeName() == INSERTION_TABLE_LABEL_ATTRIBUTE ) {
				if ( TableLabel.Amount() != 0 )
					ReportErrorAndExit_( "Table label already define", Browser );

				if ( Browser.Value().Amount() == 0 )
					ReportErrorAndExit_( "Attribute value cannot be empty", Browser );

				TableLabel = Browser.Value();
			} else if ( Browser.AttributeName() == INSERTION_RECORD_LABEL_ATTRIBUTE ) {
				if ( RecordLabel.Amount() != 0 )
					ReportErrorAndExit_( "Record label already define", Browser );

				if ( Browser.Value().Amount() == 0 )
					ReportErrorAndExit_( "Attribute value cannot be empty", Browser );

				RecordLabel = Browser.Value();
			} else
				ReportErrorAndExit_( "Unknown attribute", Browser );
			break;
		case xml::tEndTag:
			Insert_( TableLabel, RecordLabel, Tables, Browser, Record );
			Continue = false;
			break;
		case xml::tError:
			ReportErrorAndExit_( Browser );
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
	const str::string_ &DefaultRecordLabelTag,
	const tables_ &Tables,
	record_ &Record )
{
	bso::ulong__ Level = 0;
	bso::bool__ Continue = true;

	while ( Continue ) {
		switch ( Browser.Browse( xml::tfStartTag | xml::tfAttribute | xml::tfValue | xml::tfEndTag ) ) {
		case xml::tStartTag:
			if ( Browser.TagName() == INSERTION_TAG )
				ProcessInsertion_( Browser, Tables, Record );	// '...<erpck:insert ...
			else {												//                   ^
				Level++;
				Record.Content.Append( Browser.Dump().RawData );
			}
			break;
		case xml::tAttribute:
			if ( BelongsToNamespace_( Browser.AttributeName() ) )
				ReportErrorAndExit_( "Unexpected attribute", Browser );

			Record.Content.Append( Browser.Dump().RawData );
			break;
		case xml::tValue:
			if ( Level == 0 )
				ReportErrorAndExit_( "Missing tag", Browser );

			if ( Record.Label.Amount() == 0 )
				if ( Browser.TagName() == DefaultRecordLabelTag )
					Record.Label = Browser.Value();

			Record.Content.Append( Browser.Dump().RawData );
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
			Record.Content.Append( Browser.Dump().RawData );
			break;
		case xml::tError:
			ReportErrorAndExit_( Browser );
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
	const str::string_ &DefaultRecordLabelTag,
	const tables_ &Tables,
	records_ &Records )
{
ERRProlog
	bso::bool__ Continue = true;
	record Record;
	weight__ Weight = DEFAULT_WEIGHT;
ERRBegin
	Record.Init();

	while ( Continue ) {
		switch ( Browser.Browse( xml::tfStartTag | xml::tfStartTagClosed | xml::tfAttribute | xml::tfEndTag ) ) {
		case xml::tStartTag:	// Mandatory, otherwise the tag name is missed when there is an attribute.
			Record.Content.Append( Browser.Dump().RawData );
			break;
		case xml::tAttribute:
			if ( Browser.AttributeName() == RECORD_WEIGHT_ATTRIBUTE ) {
				epeios::row__ Error = NONE;

				Weight = Browser.Value().ToUB( &Error );

				if ( Error != NONE )
					ReportErrorAndExit_( "Bad attribute value", Browser );
			} else if ( Browser.AttributeName() == RECORD_LABEL_ATTRIBUTE ) {
				if ( Record.Label.Amount() != 0 )
					ReportErrorAndExit_( "Record label already defined", Browser );

				if ( Browser.Value().Amount() == 0 )
					ReportErrorAndExit_( "Attribute alue cannot be empty", Browser );

				Record.Label = Browser.Value();
			} else if ( BelongsToNamespace_( Browser.AttributeName() ) ) {
				ReportErrorAndExit_( "Unknown attribute", Browser );
			} else
				Record.Content.Append( Browser.Dump().RawData );
			break;
		case xml::tStartTagClosed:
			Record.Content.Append( Browser.Dump().RawData );
			ProcessRecord_( Browser, DefaultRecordLabelTag, Tables, Record );	// '...<ercp:content ...>...<TAG ...>...' -> '...</TAG>...'
			Records.Append( Record );									//                                   ^                 ^
			Record.Init();
			break;														
		case xml::tEndTag:
			Continue = false;	// '...<ercp:content ...></ercp:content>...'
			break;				//                                      ^
		case xml::tError:
			ReportErrorAndExit_( Browser );
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
	table_ &Table,
	const tables_ &Tables )
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
				if ( DefaultRecordLabelTag.Amount() != 0 )
					ReportErrorAndExit_( "Default record label tag value already defined", Browser );

				if ( Browser.Value().Amount() == 0 )
					ReportErrorAndExit_( "Attribute value can not be empty", Browser );

				DefaultRecordLabelTag = Browser.Value();
			} else
				ReportErrorAndExit_( "Unknown attribute", Browser );
			break;
		case xml::tStartTagClosed:
			if ( Browser.TagName() == CONTENT_TAG ) {
				ProcessRecords_( Browser, DefaultRecordLabelTag, Tables, Table.Records );	// '<ercp:content ...><...' -> '</erpck:content>...'
				Continue = false;
			}  else														        			//                    ^                         ^
				ERRc();
			break;
		case xml::tEndTag:
			Continue = false;	// '</erpck:content>...'
			break;				//                  ^
		case xml::tError:
			ReportErrorAndExit_( Browser );
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
	str::string TableAliasLabel, TableLabel, RecordLabel;
ERRBegin
	TableAliasLabel.Init();
	TableLabel.Init();
	RecordLabel.Init();

	while ( Continue ) {
		switch ( Browser.Browse( xml::tfStartTag | xml::tfStartTagClosed | xml::tfAttribute | xml::tfValue | xml::tfEndTag ) ) {
		case xml::tStartTag:
			ReportErrorAndExit_( "No tag allowed here", Browser );
			break;
		case xml::tAttribute:
			if ( Browser.AttributeName() == ALIAS_TABLE_ALIAS_ATTRIBUTE ) {
				if ( Browser.Value().Amount() == 0 )
					ReportErrorAndExit_( "Value cannot be empty", Browser );

				if ( TableAliasLabel.Amount() != 0  )
					ReportErrorAndExit_( "Attribute already defined", Browser );

				if ( TableLabel.Amount() != 0 )
					ReportErrorAndExit_( "Both '" ALIAS_TABLE_LABEL_ATTRIBUTE "' and '" ALIAS_TABLE_ALIAS_ATTRIBUTE "' cannot be defined together", Browser );

				TableAliasLabel = Browser.Value();
			} else if ( Browser.AttributeName() == ALIAS_TABLE_LABEL_ATTRIBUTE ) {
				if ( Browser.Value().Amount() == 0 )
					ReportErrorAndExit_( "Value cannot be empty", Browser );

				if ( TableLabel.Amount() != 0  )
					ReportErrorAndExit_( "Attribute already defined", Browser );

				if ( TableAliasLabel.Amount() != 0 )
					ReportErrorAndExit_( "Both '" ALIAS_TABLE_LABEL_ATTRIBUTE "' and '" ALIAS_TABLE_ALIAS_ATTRIBUTE "' cannot be defined together", Browser );

				TableLabel = Browser.Value();
			} else if ( Browser.AttributeName() == ALIAS_RECORD_LABEL_ATTRIBUTE ) {
				if ( Browser.Value().Amount() == 0 )
					ReportErrorAndExit_( "Value cannot be empty", Browser );

				if ( RecordLabel.Amount() != 0  )
					ReportErrorAndExit_( "Attribute already defined", Browser );

				RecordLabel = Browser.Value();
			} else
				ReportErrorAndExit_( "Unknown attribute", Browser );
			break;
		case xml::tStartTagClosed:
			if ( TableLabel.Amount() != 0 ) {
				if ( ( TableRow = SearchTable_( TableLabel, Tables ) ) == NONE )
					ReportErrorAndExit_( "Unable to find table", Browser );
			} else if ( TableAliasLabel.Amount() != 0 ) {
				if ( ( TableRow = SearchTable_( TableAliasLabel, TableAliases ) ) == NONE )
					ReportErrorAndExit_( "Unable to find table", Browser );
			} else
				ReportErrorAndExit_( "Missing table reference", Browser );

			if ( RecordLabel.Amount() ) {
				if ( ( RecordRow = SearchRecord_( RecordLabel, TableRow, Tables ) ) == NONE )
					ReportErrorAndExit_( "Unable to find record", Browser );

				AliasType = atRecord;
			} else
				AliasType = atTable;
			break;
		case xml::tValue:
			switch ( AliasType ) {
			case atRecord:
				RecordAlias.Init( Browser.Value(), TableRow, RecordRow );
				break;
			case atTable:
				TableAlias.Init( Browser.Value(), TableRow );
				break;
			break;
			default:
				ERRc();
			}
			break;
		case xml::tEndTag:
			switch ( AliasType ) {
			case atRecord:
			case atTable:
				Continue = false;
				break;
			case at_Undefined:
				ReportErrorAndExit_( "Incomplete alias definition", Browser );
				break;
			default:
				ERRc();
				break;
			}
			break;
		case xml::tError:
			ReportErrorAndExit_( Browser );
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
 static void ProcessAliases_(
	xml::browser___ &Browser,
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

				switch ( ProcessAlias_( Browser, Aliases.Tables, Tables, RecordAlias, TableAlias ) ) {	// '...<erpck:alias ...>...' -> '...</alias>...'
				case atRecord:																			//                  ^                       ^
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
				ReportErrorAndExit_( "Unknow tag", Browser );
			break;
		case xml::tEndTag:
			Continue = false;
			break;
		case xml::tError:
			ReportErrorAndExit_( Browser );
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
	table_ &Table,
	const tables_ &Tables )
{

	bso::bool__ Continue = true;

	while ( Continue ) {
		switch ( Browser.Browse( xml::tfStartTag | xml::tfAttribute | xml::tfEndTag ) ) {
		case xml::tStartTag:
			if ( Browser.TagName() == ALIASES_TAG )
				ProcessAliases_( Browser, Tables, Table.Aliases );	// '<erpck:table ...>...<erpck:aliases ...>...'
			else if ( Browser.TagName() == CONTENT_TAG )			//                                     ^
				ProcessContent_( Browser, Table, Tables );			// '<erpck:table ...>...<erpck:content ...>...' -> '...</erpc:content>...'
			else													//                                     ^                              ^
				ReportErrorAndExit_( "Unknown tag", Browser );
			break;
		case xml::tAttribute:
			if ( Browser.TagName() != TABLE_TAG )
				ERRc();

			if ( Browser.AttributeName() == TABLE_LABEL_ATTRIBUTE ) {
				if ( Table.Label.Amount() != 0 )
					ReportErrorAndExit_( "Table label already defined", Browser );

				if ( Browser.Value().Amount() == 0 )
					ReportErrorAndExit_( "Attribute value can not be empty", Browser );

				Table.Label = Browser.Value();
			} else
				ReportErrorAndExit_( "Unknown attribute", Browser );
			break;
		case xml::tEndTag:
			Continue = false;
			break;
		case xml::tError:
			ReportErrorAndExit_( Browser );
			break;
		default:
			ERRc();
			break;
		}
	}
}

// '...<erpck:data><erpck:table ...>' -> '...</erpck:table>...'
//                 ^                                       ^
static void ProcessData_(
	xml::browser___ &Browser,
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
				ProcessTable_( Browser, Table, Data );	// '...<erpck::table ...><erpck:content>...' -> '....</erpck:table>...'
				Data.Append( Table );					//                   ^                                             ^
			} else {
				cerr << "No '" TABLE_TAG "' in data file !" << txf::nl;
				ERRExit( EXIT_FAILURE );
			} 
			break;
		case xml::tEndTag:
			if ( !TableDetected ) {
				cerr << "No '" TABLE_TAG "' tag in data file !" << txf::nl;
				ERRExit( EXIT_FAILURE );
			} else
				Continue = false;
			break;
		case xml::tError:
			ReportErrorAndExit_( Browser );
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
	xtf::extended_text_iflow__ XFlow;
	xml::browser___ Browser;
	bso::bool__ Continue = true;
	lcl::locales Locales;
	bso::bool__ DataDetected = false;
ERRBegin
	if ( FFlow.Init( DataFileName, err::hSkip ) != fil::sSuccess ) {
		cerr << "Unable to open data file '" << DataFileName << "' !" << txf::nl;
		ERRExit( EXIT_FAILURE );
	}

	FFlow.EOFD( XTF_EOXT );

	XFlow.Init( FFlow );

	Browser.Init( XFlow );

	Data.Init();

	while ( Continue ) {
		switch ( Browser.Browse( xml::tfStartTagClosed ) ) {
		case xml::tStartTagClosed:
			if ( ( Browser.TagName() == DATA_TAG ) ) {
				ProcessData_( Browser, Data );	// '...<erpck:data><erpck:table ...>' -> '...</erpck:table>...'
				DataDetected = true;			//                 ^                                       ^
			} else {
				cerr << '\'' << DataFileName << "' is not a '" NAME "' data file !" << txf::nl;
				ERRExit( EXIT_FAILURE );
			} 
			break;
		case xml::tProcessed:
			if ( !DataDetected ) {
				cerr << "No '" << NAME << "' data in '" << DataFileName << "' !" << txf::nl;
				ERRExit( EXIT_FAILURE );
			} else
				Continue = false;
			break;
		case xml::tError:
			ReportErrorAndExit_( Browser );
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

static void Pick_( const records_ &Records )
{
	ctn::E_CITEMt( record_, rrow__ ) Record;
	rrow__ Row = NONE;

	Record.Init( Records );

	tol::InitializeRandomGenerator();

	Row = Records.First( rand() % Records.Amount() );

	cout << Record( Row ).Label << ':' << txf::nl << Record( Row ).Content << txf::nl;
}

static void Pick_( const table_ &Table )
{
	Pick_( Table.Records );	
}

static void Pick_( const data_ &Data )
{
	ctn::E_CITEMt( table_, trow__ ) Table;

	Table.Init( Data );

	Pick_( Table( Data.Last() ) );
}

#define DATA_FILENAME_TAG	"Data"

void Process_(
	const rgstry::registry_ &Registry,
	rgstry::row__ RegistryRoot )
{
ERRProlog
	str::string DataFileName;
	STR_BUFFER___ Buffer;
	data Data;
ERRBegin
	DataFileName.Init();

	if ( !Registry.GetValue( str::string( DATA_FILENAME_TAG ), RegistryRoot, DataFileName ) ) {
		cerr << "No '" << DATA_FILENAME_TAG << "' tag found !" << txf::nl;
		ERRExit( EXIT_FAILURE );
	}

	Data.Init();

	RetrieveData_( DataFileName.Convert( Buffer ), Data );

	Pick_( Data );
ERRErr
ERREnd
ERREpilog
}

static void Process_( const char *Project )
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

	if ( ( Error = rgstry::FillRegistry( Project, "Projects/Project[@target=\"" NAME "\"]", Registry, RegistryRoot, ErrorDetails ) ) != rgstry::eOK ) {
		Locales.Init();
		ErrorMessage.Init();
		cerr << rgstry::GetTranslation( Error, ErrorDetails,  str::string(), Locales, ErrorMessage ) << txf::nl;
		ERRExit( EXIT_FAILURE );
	}

	Process_( Registry, RegistryRoot );
ERRErr
ERREnd
ERREpilog
}

void Go( const parameters &Parameters )
{
ERRProlog
ERRBegin
	Process_( Parameters.Project );
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
