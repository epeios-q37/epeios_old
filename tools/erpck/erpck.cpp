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
		const str::string_ &Label,
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
		bso::bool__ Skip;
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
		S_.Skip = false;
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
		S_.Skip = R.S_.Skip;

		return *this;
	}
	void Init( void )
	{
		S_.Weight = DEFAULT_WEIGHT;
		S_.Skip = false;

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
	E_RWDISCLOSE_( bso::bool__, Skip );
};

E_AUTO( record )

typedef ctn:: E_XCONTAINERt_( record_, rrow__ ) records_;
E_AUTO( records )

class table_ {
public:
	struct s {
		str::string_::s Label;
		records_::s Records;
//		aliases_::s Aliases;
	};
	str::string_ Label;
	records_ Records;
//	aliases_ Aliases;
	table_( s &S )
	: Label( S.Label ),
	  Records( S.Records )/*,
	  Aliases( S.Aliases ) */
	{};
	void reset( bso::bool__ P = true )
	{
		Label.reset( P );
		Records.reset( P );
//		Aliases.reset( P );
	}
	void plug( mmm::E_MULTIMEMORY_ &MM )
	{
		Label.plug( MM );
		Records.plug( MM );
//		Aliases.plug( MM );
	}
	table_ &operator =(const table_ &T )
	{
		Label = T.Label;
		Records = T.Records;
//		Aliases = T.Aliases;

		return *this;
	}
	void Init( void )
	{
		reset();

		Label.Init();
		Records.Init();
//		Aliases.Init();
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
	const xpp::preprocessing_extended_text_iflow___ &XFlow,
	txf::text_oflow__ &TFlow )
{
	PrintPosition_( XFlow.Preprocessor().Coord(), TFlow );

	if ( XFlow.Preprocessor().LocalizedFileName().Amount() != 0 )
		TFlow << " in file '" << XFlow.Preprocessor().LocalizedFileName() << '\'';
}

static void ReportErrorAndExit_( 
	const str::string_ &Message,
	xpp::preprocessing_extended_text_iflow___ &XFlow )
{
	cerr << "Parsing interrupted at ";
/*
	PrintPosition_( XFlow.Coord(), cerr );
	if ( XFlow.Preprocessor().LocalizedFileName().Amount() != 0 )
		cerr << " in file '" << XFlow.Preprocessor().LocalizedFileName() << '\'';
*/
	PrintPosition_( XFlow, cerr );
	cerr << " : " << Message << " !" << txf::nl;
	ERRExit( EXIT_FAILURE );

}

static void ReportErrorAndExit_( 
	const char *Message,
	xpp::preprocessing_extended_text_iflow___ &XFlow )
{
	ReportErrorAndExit_( str::string( Message ), XFlow );
}

static void ReportErrorAndExit_( xpp::preprocessing_extended_text_iflow___ &XFlow )
{
ERRProlog
	lcl::locales Locales;
	str::string ErrorMessage;
ERRBegin
	Locales.Init();
	ErrorMessage.Init();
	cerr << xpp::GetTranslation( XFlow.Preprocessor().Status(), str::string(), Locales, ErrorMessage ) << " at ";
	PrintPosition_( XFlow, cerr );
	cerr << " !" << txf::nl;
	ERRExit( EXIT_FAILURE );
ERRErr
ERREnd
ERREpilog
}

template <typename container, typename item, typename row> row BaseSearch_(
	const str::string_ &Label,
	const container &Container )
{
	item Item;
	row Row = Container.First();

	Item.Init( Container );

	while ( ( Row != NONE ) && ( Item( Row ).Label != Label ) )
		Row = Container.Next( Row );

	return Row;
}

template <typename container, typename item, typename row> row SearchInMulti_(
	const str::string_ &Label,
	const container &Container )
{
	return BaseSearch_<container, ctn::E_CITEMt( item, row ), row >( Label, Container );
}

template <typename container, typename item, typename row> row SearchInMono_(
	const str::string_ &Label,
	const container &Container )
{
	return BaseSearch_<container, ctn::E_CMITEMt( item, row ), row >( Label, Container );
}

static trow__ SearchTable_(
	const str::string_ &Label,
	const tables_ &Tables )
{
	return SearchInMulti_<tables_, table_, trow__>( Label, Tables );
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
	return SearchInMulti_<records_, record_, rrow__>( Label, Records );
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
	rrow__ RecordRow,
	trow__ TableRow,
	const tables_ &Tables,
	record_ &Record )
{
	ctn::E_CITEMt( table_, trow__ ) Table;

	Table.Init( Tables );

	Insert_( RecordRow, Table( TableRow ).Records, Record );
}

static void Insert_(
	const str::string_ &RecordLabel,
	const records_ &Records,
	xpp::preprocessing_extended_text_iflow___ &XFlow,
	record_ &Record )
{
	rrow__ Row = SearchRecord_( RecordLabel, Records );

	if ( Row == NONE )
		ReportErrorAndExit_( "Unable to find record", XFlow );

	Insert_( Row, Records, Record );
}

static epeios::row__ FindRecordAlias_(
	const str::string_ &Label,
	const record_aliases_ &Aliases )
{
	return SearchInMono_<record_aliases_, record_alias_, epeios::row__>( Label, Aliases );
}

static epeios::row__ FindTableAlias_(
	const str::string_ &Label,
	const table_aliases_ &Aliases )
{
	return SearchInMono_<table_aliases_, table_alias_, epeios::row__>( Label, Aliases );
}

static void Insert_(
	epeios::row__ AliasRow,
	const record_aliases_ &Aliases,
	const tables_ &Tables,
	record_ &Record )
{
	ctn::E_CMITEM( record_alias_ ) Alias;

	Alias.Init( Aliases );

	Insert_( Alias( AliasRow ).RecordRow(), Alias( AliasRow ).TableRow(), Tables, Record );
}

static void InsertUsingRecordAlias_(
	const str::string_ &RecordAlias,
	const record_aliases_ &Aliases,
	const tables_ &Tables,
	xpp::preprocessing_extended_text_iflow___ &XFlow,
	record_ &Record )
{
	epeios::row__ Row = FindRecordAlias_( RecordAlias, Aliases );

	if ( Row == NONE )
		ReportErrorAndExit_( "Unable to find record alias", XFlow );

	Insert_( Row, Aliases, Tables, Record );
}

static void Insert_(
	const str::string_ &RecordLabel,
	const table_ &Table,
	xpp::preprocessing_extended_text_iflow___ &XFlow,
	record_ &Record )
{
	Insert_( RecordLabel, Table.Records, XFlow, Record );
}

static void Insert_(
	const str::string_ &RecordLabel,	
	trow__ TableRow,
	const tables_ &Tables,
	xpp::preprocessing_extended_text_iflow___ &XFlow,
	record_ &Record )
{
	ctn::E_CITEMt( table_, trow__ )Table;

	Table.Init( Tables );

	Insert_( RecordLabel, Table( TableRow ), XFlow, Record );
}

static void Insert_(
	const str::string_ &RecordLabel,
	epeios::row__ AliasRow,
	const table_aliases_ &Aliases,
	const tables_ &Tables,
	xpp::preprocessing_extended_text_iflow___ &XFlow,
	record_ &Record )
{
	ctn::E_CMITEM( table_alias_ ) Alias;

	Alias.Init( Aliases );

	Insert_( RecordLabel, Alias( AliasRow ).TableRow(), Tables, XFlow, Record );
}

static void InsertUsingTableAlias_(
	const str::string_ &TableAlias,
	const str::string_ &RecordLabel,
	const table_aliases_ &Aliases,
	const tables_ &Tables,
	xpp::preprocessing_extended_text_iflow___ &XFlow,
	record_ &Record )
{
	epeios::row__ Row = FindTableAlias_( TableAlias, Aliases );

	if ( Row == NONE )
		ReportErrorAndExit_( "Unable to find table alias", XFlow );

	Insert_( RecordLabel, Row, Aliases, Tables, XFlow, Record );
}

static void InsertUsingLabels_(
	const str::string_ &TableLabel,
	const str::string_ &RecordLabel,
	const tables_ &Tables,
	xpp::preprocessing_extended_text_iflow___ &XFlow,
	record_ &Record )
{
	trow__ Row = SearchTable_( TableLabel, Tables );
	ctn::E_CITEMt( table_, trow__ ) Table;

	if ( Row == NONE )
		ReportErrorAndExit_( "Unable to find table", XFlow );

	Table.Init( Tables );

	Insert_( RecordLabel, Table( Row ), XFlow, Record );
}

static void Assign_(
	str::string_ &Target,
	const char *TargetLabel,
	xml::browser___ &Browser,
	xpp::preprocessing_extended_text_iflow___ &XFlow )
{
ERRProlog
	str::string ErrorMessage;
ERRBegin
	ErrorMessage.Init( TargetLabel );

	if ( Target.Amount() != 0 ) {
		ErrorMessage.Append( " already defined" );
		ReportErrorAndExit_( ErrorMessage, XFlow );
	}

	if ( Browser.Value().Amount() == 0 ) {
		ErrorMessage.Append( " value can not be empty" );
		ReportErrorAndExit_( ErrorMessage, XFlow );
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
	xpp::preprocessing_extended_text_iflow___ &XFlow )
{
ERRProlog
	str::string ErrorMessage;
ERRBegin
	ErrorMessage.Init( "Both " );
	ErrorMessage.Append( ItemLabel1 );
	ErrorMessage.Append( " and " );
	ErrorMessage.Append( ItemLabel2 );
	ErrorMessage.Append( " cannot be define together" );

	ReportErrorAndExit_( ErrorMessage, XFlow );
ERRErr
ERREnd
ERREpilog
}

// '...<erpck:insert ...>...' -> '...</erpck:insert>...'
//                   ^                              ^
static void ProcessInsertion_(
	xml::browser___ &Browser,
	xpp::preprocessing_extended_text_iflow___ &XFlow,
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
			ReportErrorAndExit_( "No tag allowed", XFlow );
			break;
		case xml::tAttribute:
			if ( Browser.AttributeName() == INSERTION_TABLE_LABEL_ATTRIBUTE ) {
				Assign_( TableLabel, "Table label", Browser, XFlow );
			} else if ( Browser.AttributeName() == INSERTION_RECORD_LABEL_ATTRIBUTE ) {
				Assign_( RecordLabel, "Record label", Browser, XFlow );
			} else if ( Browser.AttributeName() == INSERTION_TABLE_ALIAS_ATTRIBUTE ) {
				Assign_( TableAlias, "Table alias", Browser, XFlow );
			} else if ( Browser.AttributeName() == INSERTION_RECORD_ALIAS_ATTRIBUTE ) {
				Assign_( RecordAlias, "Record alias", Browser, XFlow );
			} else
				ReportErrorAndExit_( "Unknown attribute", XFlow );
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
					ReportInsertionErrorAndExit_( "table alias", "table label", XFlow );
				else if ( RA )
					ReportInsertionErrorAndExit_( "table alias", "record alias", XFlow );
				else if ( !RL )
					ReportErrorAndExit_( "Record label missing", XFlow );
				else
					InsertUsingTableAlias_( TableAlias, RecordLabel, Aliases.Tables, Tables, XFlow, Record );
			else if ( TL )
				if ( RA )
					ReportInsertionErrorAndExit_( "table label", "record alias", XFlow );
				else if ( !RL )
					ReportErrorAndExit_( "Record label missing", XFlow );
				else
					InsertUsingLabels_( TableLabel, RecordLabel, Tables, XFlow, Record );
			else if ( RL )
				if ( RA )
					ReportInsertionErrorAndExit_( "record label", "record alias", XFlow );
				else
					ERRc();	// Normally already handled before.
			else if ( RA )
				InsertUsingRecordAlias_( RecordAlias, Aliases.Records, Tables, XFlow, Record );
			else
				ReportErrorAndExit_( "Missing insertion parameters", XFlow );

			Continue = false;
			break;
		}
		case xml::tValue:
			ReportErrorAndExit_( "No value allowed here", XFlow );
			break;
		case xml::tError:
			ReportErrorAndExit_( XFlow );
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
	xpp::preprocessing_extended_text_iflow___ &XFlow,
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
					ProcessInsertion_( Browser, XFlow, Aliases, Tables, Record );	// '...<erpck:insert ...
				else																//                   ^
					ReportErrorAndExit_( "'" INSERTION_TAG "' is the only '" NAMESPACE "' tag allowed here", XFlow );
			} else {
				Level++;
				Record.Content.Append( Browser.DumpData() );
			}
			break;
		case xml::tAttribute:
			if ( BelongsToNamespace_( Browser.AttributeName() ) )
				ReportErrorAndExit_( "Unexpected attribute", XFlow );

			Record.Content.Append( Browser.DumpData() );
			break;
		case xml::tValue:
			if ( Level == 0 )
				ReportErrorAndExit_( "Missing tag", XFlow );

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
			ReportErrorAndExit_( XFlow );
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
	xpp::preprocessing_extended_text_iflow___ &XFlow,
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
					ReportErrorAndExit_( "Bad attribute value", XFlow );
			} else if ( Browser.AttributeName() == RECORD_LABEL_ATTRIBUTE ) {
					Assign_( Record.Label, "Record label", Browser, XFlow );
			} else if ( Browser.AttributeName() == RECORD_HANDLING_ATTRIBUTE ) {
				if ( Browser.Value() == RECORD_HANDLING_ATTRIBUTE_SKIP_VALUE )
					Record.Skip() = true;
				else if ( Browser.Value() == RECORD_HANDLING_ATTRIBUTE_IGNORE_VALUE )
					Ignore = true;
				else
					ReportErrorAndExit_( "Unknown attribute value", XFlow );
			} else if ( BelongsToNamespace_( Browser.AttributeName() ) ) {
				ReportErrorAndExit_( "Unknown attribute", XFlow );
			} else
				Record.Content.Append( Browser.DumpData() );
			break;
		case xml::tStartTagClosed:
			Record.Content.Append( " id=\"" );
			Record.Content.Append( bso::Convert( Records.Amount() + 1, Buffer ) );
			Record.Content.Append( '"' );
			Record.Content.Append( Browser.DumpData() );
			ProcessRecord_( Browser, XFlow, DefaultRecordLabelTag, Aliases, Tables, Record );	// '...<ercp:content ...>...<TAG ...>...' -> '...</TAG>...'
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
			ReportErrorAndExit_( XFlow );
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
	xpp::preprocessing_extended_text_iflow___ &XFlow,
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
				Assign_( DefaultRecordLabelTag, "Default record label", Browser, XFlow );
			} else
				ReportErrorAndExit_( "Unknown attribute", XFlow );
			break;
		case xml::tStartTagClosed:
			if ( Browser.TagName() == CONTENT_TAG ) {
				ProcessRecords_( Browser, XFlow, DefaultRecordLabelTag, Aliases, Tables, Table.Records );	// '<ercp:content ...><...' -> '</erpck:content>...'
				Continue = false;
			}  else														        			//                    ^                         ^
				ERRc();
			break;
		case xml::tEndTag:
			Continue = false;	// '</erpck:content>...'
			break;				//                  ^
		case xml::tError:
			ReportErrorAndExit_( XFlow );
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
	xpp::preprocessing_extended_text_iflow___ &XFlow,
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
			ReportErrorAndExit_( "No tag allowed here", XFlow );
			break;
		case xml::tAttribute:
			if ( Browser.AttributeName() == ALIAS_TABLE_ALIAS_ATTRIBUTE ) {
				if ( TableLabel.Amount() != 0 )
					ReportErrorAndExit_( "Both '" ALIAS_TABLE_LABEL_ATTRIBUTE "' and '" ALIAS_TABLE_ALIAS_ATTRIBUTE "' cannot be defined together", XFlow );

				Assign_( TableAliasLabel, "Table alias", Browser, XFlow );
			} else if ( Browser.AttributeName() == ALIAS_TABLE_LABEL_ATTRIBUTE ) {
				if ( TableAliasLabel.Amount() != 0 )
					ReportErrorAndExit_( "Both '" ALIAS_TABLE_LABEL_ATTRIBUTE "' and '" ALIAS_TABLE_ALIAS_ATTRIBUTE "' cannot be defined together", XFlow );

				Assign_( TableLabel, "Table alias", Browser, XFlow );
			} else if ( Browser.AttributeName() == ALIAS_RECORD_LABEL_ATTRIBUTE ) {
				Assign_( RecordLabel, "Record label", Browser, XFlow );
			} else if ( Browser.AttributeName() == ALIAS_LABEL_ATTRIBUTE ) {
				Assign_( AliasLabel, "Alias label", Browser, XFlow );
			} else
				ReportErrorAndExit_( "Unknown attribute", XFlow );
			break;
		case xml::tStartTagClosed:
			if ( TableLabel.Amount() != 0 ) {
				if ( ( TableRow = SearchTable_( TableLabel, Tables ) ) == NONE )
					ReportErrorAndExit_( "Unable to find table", XFlow );
			} else if ( TableAliasLabel.Amount() != 0 ) {
				if ( ( TableRow = SearchTable_( TableAliasLabel, TableAliases ) ) == NONE )
					ReportErrorAndExit_( "Unable to find table", XFlow );
			} else
				ReportErrorAndExit_( "Missing table reference", XFlow );

			if ( AliasLabel.Amount() == 0 )
				ReportErrorAndExit_( "Alias label missing", XFlow );

			if ( RecordLabel.Amount() ) {
				if ( ( RecordRow = SearchRecord_( RecordLabel, TableRow, Tables ) ) == NONE )
					ReportErrorAndExit_( "Unable to find record", XFlow );

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
			ReportErrorAndExit_( "No value allowed here", XFlow );
			break;
		case xml::tEndTag:
			switch ( AliasType ) {
			case atRecord:
			case atTable:
				Continue = false;
				break;
			case at_Undefined:
				ReportErrorAndExit_( "Incomplete alias definition", XFlow );
				break;
			default:
				ERRc();
				break;
			}
			break;
		case xml::tError:
			ReportErrorAndExit_( XFlow );
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
	xpp::preprocessing_extended_text_iflow___ &XFlow,
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

				switch ( ProcessAlias_( Browser, XFlow, Aliases.Tables, Tables, RecordAlias, TableAlias ) ) {	// '...<erpck:alias ...>...' -> '...</alias>...'
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
				ReportErrorAndExit_( "Unknow tag", XFlow );
			break;
		case xml::tEndTag:
			Continue = false;
			break;
		case xml::tError:
			ReportErrorAndExit_( XFlow );
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
	xpp::preprocessing_extended_text_iflow___ &XFlow,
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
				ProcessAliases_( Browser, XFlow, Tables, Aliases );	// '<erpck:table ...>...<erpck:aliases ...>...'
			else if ( Browser.TagName() == CONTENT_TAG )			//                                     ^
				ProcessContent_( Browser, XFlow, Table, Tables, Aliases );	// '<erpck:table ...>...<erpck:content ...>...' -> '...</erpc:content>...'
			else															//                                     ^                              ^
				ReportErrorAndExit_( "Unknown tag", XFlow );
			break;
		case xml::tAttribute:
			if ( Browser.TagName() != TABLE_TAG )
				ERRc();

			if ( Browser.AttributeName() == TABLE_LABEL_ATTRIBUTE ) {
				Assign_( Table.Label, "Table label", Browser, XFlow );
			} else
				ReportErrorAndExit_( "Unknown attribute", XFlow );
			break;
		case xml::tEndTag:
			Continue = false;
			break;
		case xml::tError:
			ReportErrorAndExit_( XFlow );
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
	xpp::preprocessing_extended_text_iflow___ &XFlow,
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
				ProcessTable_( Browser, XFlow, Table, Data );	// '...<erpck::table ...><erpck:content>...' -> '....</erpck:table>...'
				Data.Append( Table );							//                   ^                                             ^
			} else
				ReportErrorAndExit_( "Missing '" TABLE_TAG "'", XFlow );
			break;
		case xml::tEndTag:
			if ( !TableDetected ) {
				cerr << "No '" TABLE_TAG "' tag in data file !" << txf::nl;
				ERRExit( EXIT_FAILURE );
			} else
				Continue = false;
			break;
		case xml::tError:
			ReportErrorAndExit_( XFlow );
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
	xpp::preprocessing_extended_text_iflow___ XFlow;
	xml::browser___ Browser;
	bso::bool__ Continue = true;
	lcl::locales Locales;
	bso::bool__ DataDetected = false;
	FNM_BUFFER___ Buffer;
ERRBegin
	if ( FFlow.Init( DataFileName, err::hSkip ) != fil::sSuccess ) {
		cerr << "Unable to open data file '" << DataFileName << "' !" << txf::nl;
		ERRExit( EXIT_FAILURE );
	}

	FFlow.EOFD( XTF_EOXT );

	XFlow.Init( FFlow, str::string( fnm::GetLocation( DataFileName, Buffer ) ) );

	Browser.Init( XFlow );

	Data.Init();

	while ( Continue ) {
		switch ( Browser.Browse( xml::tfStartTagClosed |xml::tfAttribute ) ) {
		case xml::tStartTagClosed:
			if ( ( Browser.TagName() == DATA_TAG ) ) {
				ProcessData_( Browser, XFlow, Data );	// '...<erpck:data><erpck:table ...>' -> '...</erpck:table>...'
				DataDetected = true;					//                 ^                                       ^
			} else {
				ReportErrorAndExit_( "Unexpected tag", XFlow );
			} 
			break;
		case xml::tAttribute:
			ReportErrorAndExit_( "Unexpected attribute", XFlow );
			break;
		case xml::tProcessed:
			if ( !DataDetected ) {
				cerr << "No '" << NAME << "' data in '" << DataFileName << "' !" << txf::nl;
				ERRExit( EXIT_FAILURE );
			} else
				Continue = false;
			break;
		case xml::tError:
			ReportErrorAndExit_( XFlow );
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

static void Display_(
	id__ Id,
	const records_ &Records,
	xml::writer_ &Writer )
{
	rrow__ Row = NONE;
	bso::integer_buffer__ Buffer;

	Writer.PutAttribute( "TotalAmount", bso::Convert( Records.Amount(), Buffer ) );

	if ( Id == ALL ) {
		Writer.PutAttribute( "Amount", bso::Convert( Records.Amount(), Buffer ) );
		DisplayAll_( Records, Writer );
	} else {
		Writer.PutAttribute( "Amount", "1" );

		if ( Id == 0 ) {
			tol::InitializeRandomGenerator();
			Row = Records.First( rand() % Records.Amount() );
		} else {
			if ( Id > Records.Amount() ) {
				cerr << "No record of id '" << Id << "'! " << txf::nl;
				ERRExit( EXIT_FAILURE )
			} else
				Row = Id - 1;
		}

		Display_( Row, Records, Writer );
	}

}

static void Display_(
	id__ Id,
	const table_ &Table,
	const str::string_ &XSLFileName,
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

	Writer.Init( Output );
	Writer.PushTag( Table.Label );

	Display_( Id, Table.Records, Writer );	

	Writer.PopTag();

ERRErr
ERREnd
ERREpilog
}

static void Display_(
	id__ Id,
	const data_ &Data,
	const str::string_ &XSLFileName,
	txf::text_oflow__ &Output )
{
	ctn::E_CITEMt( table_, trow__ ) Table;

	Table.Init( Data );

	Display_( Id, Table( Data.Last() ), XSLFileName, Output );
}

static void Display_(
	id__ Id,
	const data_ &Data,
	const str::string_ &XSLFileName,
	const char *FileName )
{
ERRProlog
	bso::bool__ Backuped = false;
	flf::file_oflow___ FFlow;
	txf::text_oflow__ TFlow( FFlow );
ERRBegin
	fil::CreateBackupFile( FileName, fil::hbfRename );

	Backuped = true;

	if ( FFlow.Init( FileName ) != fil::sSuccess ) {
		cerr << "Unable to open '" << FileName << "' for output !" << txf::nl;
		ERRExit( EXIT_FAILURE );
	}

	Display_( Id, Data, XSLFileName, TFlow );
ERRErr
	FFlow.reset();
	if ( Backuped )
		fil::RecoverBackupFile( FileName );
ERREnd
ERREpilog
}

static void Display_(
						id__ Id,
	const data_ &Data,
	const str::string_ &XSLFileName,
	const str::string_ &OutputFileName )
{
ERRProlog
	STR_BUFFER___ Buffer;
ERRBegin
	if ( OutputFileName.Amount() == 0 )
		Display_( Id, Data, XSLFileName, cout );
	else
		Display_( Id, Data, XSLFileName, OutputFileName.Convert( Buffer ) );
ERRErr
ERREnd
ERREpilog
}

#define DATA_FILENAME_TAG	"Data"
#define OUTPUT_FILENAME_TAG	"Output"
#define XSL_FILENAME_TAG	"XSL"
#define COMMAND_TAG			"Command"

void LaunchCommand_(
	const str::string_ &Command,
	const str::string_ &OutputFileName )
{
ERRProlog
	str::string CompleteCommand;
	STR_BUFFER___ Buffer;
ERRBegin
	if ( ( Command.Amount() != 0 ) && ( OutputFileName.Amount() != 0 ) ) {
		CompleteCommand.Init( Command );
		str::ReplaceTag( CompleteCommand, 1, OutputFileName, '$' );
	cout << "Launching '" << CompleteCommand << "\"." << txf::nl;
		system( CompleteCommand.Convert( Buffer ) );
	}


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
	str::string OutputFileName;
	str::string XSLFileName;
	str::string Command;
ERRBegin
	DataFileName.Init();
	if ( !Registry.GetValue( str::string( DATA_FILENAME_TAG ), RegistryRoot, DataFileName ) ) {
		cerr << "No '" << DATA_FILENAME_TAG << "' tag found !" << txf::nl;
		ERRExit( EXIT_FAILURE );
	}

	OutputFileName.Init();
	Registry.GetValue( str::string( OUTPUT_FILENAME_TAG ), RegistryRoot, OutputFileName );

	XSLFileName.Init();
	Registry.GetValue( str::string( XSL_FILENAME_TAG ), RegistryRoot, XSLFileName );

	Data.Init();
	RetrieveData_( DataFileName.Convert( Buffer ), Data );

	Display_( Id, Data, XSLFileName, OutputFileName );

	Command.Init();
	Registry.GetValue( str::string( COMMAND_TAG ), RegistryRoot, Command );

	LaunchCommand_( Command, OutputFileName );
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
