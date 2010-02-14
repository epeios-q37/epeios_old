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

E_ROW( row__ );

typedef str::string_	item_;
typedef str::string	item;

typedef dtr::E_DTREEt_( row__ ) tree_;
typedef ctn::E_XMCONTAINERt_( str::string_, row__ ) container_;

class structure_
: public tree_,
  public container_
{
private:
	row__ _Add( const item_ &Item )
	{
		row__ Row = container_::Append( Item );
		tree_::Allocate( container_::Amount() );

		return Row;
	}
public:
	struct s
	: public tree_::s,
	public container_::s
	{
		row__ Root;
	} &S_;
	structure_( s &S )
	: S_( S ),
	  tree_( S ),
	  container_( S )
	{
	}
	void reset( bso::bool__ P = true )
	{
		tree_::reset( P );
		container_::reset( P );

		S_.Root = NONE;
	}
	void plug( mmm::E_MULTIMEMORY_ &MM )
	{
		tree_::plug( MM );
		container_::plug( MM );
	}
	structure_ &operator =( const structure_ &S )
	{
		tree_::operator =( S );
		container_::operator =( S );

		S_.Root = S.S_.Root;

		return *this;
	}
	void Init( void )
	{
		reset();

		tree_::Init();
		container_::Init();
	}
	row__ Create( const item_ &Item )
	{
		if ( S_.Root != NONE )
			ERRu();

		S_.Root = _Add( Item );
	}
	row__ BecomeChild(
		const item_ &Item,
		row__ Parent )
	{
		row__ Row = NONE;

		Row = _Add( Item );

		if ( S_.Root == NONE )
			S_.Root = Row;
		else
			tree_::BecomeFirstChild( Row, Parent );

		return Row;
	}
	row__ BecomeSibling(
		const item_ &Item,
		row__ Sibling )
	{
		row__ Row = NONE;

		if ( S_.Root == NONE )
			ERRu();

		Row = _Add( Item );
		tree_::BecomeNextSibling( Row, Sibling );

		return Row;
	}
};

E_AUTO( structure )

class table_ {
public:
	struct s {
		structure_::s Structure;
	};
	structure_ Structure;
	table_( s &S )
	: Structure( S.Structure )
	{
	};
	void reset( bso::bool__ P = true )
	{
		Structure.reset( P );
	}
	void plug( mmm::E_MULTIMEMORY_ &MM )
	{
		Structure.plug( MM );
	}
	table_ &operator =(const table_ &T )
	{
		Structure = T.Structure;

		return *this;
	}
	void Init( void )
	{
		reset();

		Structure.Init();
	}
};

E_AUTO( table )

typedef ctn::E_XCONTAINER_( table_ ) tables_;
E_AUTO( tables );

typedef tables_	data_;
E_AUTO( data )


	

#define NAMESPACE		NAME ":"
#define DATA_TAG		NAMESPACE "data"
#define TABLE_TAG		NAMESPACE "table"
#define STRUCTURE_TAG	NAMESPACE "structure"
#define CONTENT_TAG		NAMESPACE "content"
#define TAG_TAG			NAMESPACE "tag"
#define ATTRIBUTE_TAG	NAMESPACE "attribute"
#define NAME_ATTRIBUTE	"name"

static void PrintPosition_(
	const xtf::coord__ &Coord,
	txf::text_oflow__ TFlow )
{
	TFlow << "at line " << Coord.Line << ", column " << Coord.Column;
}

static void PrintPosition_(
	const xml::browser___ &Browser,
	txf::text_oflow__ &TFlow )
{
	PrintPosition_( Browser.GetCurrentCoord(), TFlow );
}

static void ProcessTag_(
	xml::browser___ &Browser,
	structure_ &Structure,
	row__ Parent,
	lcl::locales_ &Locales )
{
	bso::bool__ Continue = true;
	row__ Root = NONE;

	while ( Continue ) {
		switch ( Browser.Browse( xml::tfStartTag |xml::tfAttribute | xml::tfStartTagClosed | xml::tfEndTag ) ) {
		case xml::tStartTag:
			if ( Browser.TagName() == ATTRIBUTE_TAG ) {
				cerr << "Orphan attribute definition at ";
				PrintPosition_( Browser, cerr );
				cerr << '!' << txf::nl;
				ERRExit( EXIT_FAILURE );
			} else if ( Browser.TagName() != TAG_TAG ) {
				cerr << "Unknown tag at ";
				PrintPosition_( Browser, cerr );
				cerr << '!' << txf::nl;
				ERRExit( EXIT_FAILURE );
			}
			break;
		case xml::tAttribute:
			if ( Browser.AttributeName() == NAME_ATTRIBUTE ) {
				Root = Structure.BecomeChild( Browser.Value(), Parent );
			} else {
				cerr << "Unknown attribute at ";
				PrintPosition_( Browser, cerr );
				cerr << '!' << txf::nl;
				ERRExit( EXIT_FAILURE );
			} 
			break;
		case xml::tStartTagClosed:
			if ( Root != NONE )
				ProcessTag_( Browser, Structure, Root, Locales );
			else {
				cerr << "Unamed tag at ";
				PrintPosition_( Browser, cerr );
				cerr << '!' << txf::nl;
				ERRExit( EXIT_FAILURE );
			}
			break;
		case xml::tEndTag:
			Continue = false;
			break;
		case xml::tError:
			Locales.Init();
			ErrorMessage.Init();
			cerr << xml::GetTranslation( Browser.Status(), str::string(), Locales, ErrorMessage ) << txf::nl;
			break;
		default:
			ERRc();
			break;
		}
	}
}

static void ProcessStructure_(
	xml::browser___ &Browser,
	structure_ &Structure,
	lcl::locales_ &Locales )
{
ERRProlog
	bso::bool__ Continue = true;
	str::string ErrorMessage;
ERRBegin
	while ( Continue ) {
		switch ( Browser.Browse( xml::tfStartTag |xml::tfAttribute | xml::tfStartTagClosed | xml::tfEndTag ) ) {
		case xml::tStartTag:
			if ( RootTagDetected ) {
				cerr << "No other tag can be defined at this position ";
				PrintPosition_( Browser, cerr );
				cerr << '!' << txf::nl;
				ERRExit( EXIT_FAILURE );
			if ( Browser.TagName() == ATTRIBUTE_TAG ) {
				cerr << "Orphan attribute definition at ";
				PrintPosition_( Browser, cerr );
				cerr << '!' << txf::nl;
				ERRExit( EXIT_FAILURE );
			} else if ( Browser.TagName() != TAG_TAG ) {
				cerr << "Unknown tag at ";
				PrintPosition_( Browser, cerr );
				cerr << '!' << txf::nl;
				ERRExit( EXIT_FAILURE );
			} else
				RootTagDetected = true;

			break;
		case xml::tAttribute:
			if ( Browser.AttributeName() == NAME_ATTRIBUTE ) {
				ProcessTag_( Browser, Structure, Structure.BecomeChild( Browser.Value(), NONE ), Locales );
			} else {
				cerr << "Unknown attribute at ";
				PrintPosition_( Browser, cerr );
				cerr << '!' << txf::nl;
				ERRExit( EXIT_FAILURE );
			} 
			break;
		case xml::tEndTag:
			Continue = false;
			break;
		case xml::tError:
			Locales.Init();
			ErrorMessage.Init();
			cerr << xml::GetTranslation( Browser.Status(), str::string(), Locales, ErrorMessage ) << txf::nl;
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

static void ProcessTable_(
	xml::browser___ &Browser,
	table_ &Table,
	lcl::locales_ &Locales )
{
ERRProlog
	bso::bool__ Continue = true;
	bso::bool__ StructureDetected = false;
	str::string ErrorMessage;
ERRBegin
	while ( Continue ) {
		switch ( Browser.Browse( xml::tfStartTag | xml::tfStartTagClosed | xml::tfEndTag ) ) {
		case xml::tStartTag:
			if ( Browser.TagName() == STRUCTURE_TAG ) {
				if ( StructureDetected ) {
					cerr << "Duplicate '" STRUCTURE_TAG " ";
					PrintPosition_( Browser, cerr );
					cerr << " !" << txf::nl;
				}
				StructureDetected = true;
				ProcessStructure_( Browser, Table.Structure, Locales );
			} else {
				cerr << "No '" TABLE_TAG "' in data file !" << txf::nl;
				ERRExit( EXIT_FAILURE );
			} 
			break;
		case xml::tEndTag:
			Continue = false;
			break;
		case xml::tError:
			Locales.Init();
			ErrorMessage.Init();
			cerr << xml::GetTranslation( Browser.Status(), str::string(), Locales, ErrorMessage ) << txf::nl;
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

static void ProcessData_(
	xml::browser___ &Browser,
	data_ &Data,
	lcl::locales_ &Locales )
{
ERRProlog
	bso::bool__ Continue = true;
	bso::bool__ TableDetected = false;
	table Table;
	str::string ErrorMessage;
ERRBegin
	while ( Continue ) {
		switch ( Browser.Browse( xml::tfStartTag | xml::tfStartTagClosed | xml::tfEndTag ) ) {
		case xml::tStartTag:
			if ( Browser.TagName() == TABLE_TAG ) {
				TableDetected = true;
				Table.Init();
				ProcessTable_( Browser, Table, Locales );
				Data.Append( Table );
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
			Locales.Init();
			ErrorMessage.Init();
			cerr << xml::GetTranslation( Browser.Status(), str::string(), Locales, ErrorMessage ) << txf::nl;
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

static void ProcessDataFile_( const char *DataFileName )
{
ERRProlog
	flf::file_iflow___ FFlow;
	xtf::extended_text_iflow__ XFlow;
	xml::browser___ Browser;
	bso::bool__ Continue = true;
	lcl::locales Locales;
	str::string ErrorMessage;
	bso::bool__ DataDetected = false;
	data Data;
ERRBegin
	if ( FFlow.Init( DataFileName, err::hSkip ) != fil::sSuccess ) {
		cerr << "Unable to open data file'" << DataFileName << "' !" << txf::nl;
		ERRExit( EXIT_FAILURE );
	}

	FFlow.EOFD( XTF_EOXT );

	XFlow.Init( FFlow );

	Browser.Init( XFlow );

	while ( Continue ) {
		switch ( Browser.Browse( xml::tfStartTag | xml::tfStartTagClosed | xml::tfEndTag ) ) {
		case xml::tStartTag:
			if ( ( Browser.TagName() == DATA_TAG ) ) {
				ProcessData_( Browser, Data, Locales );
				DataDetected = true;
			} else {
				cerr << '\'' << DataFileName << "' is not a '" NAME "' data file !" << txf::nl;
				ERRExit( EXIT_FAILURE );
			} 
			break;
		case xml::tEndTag:
			if ( !DataDetected ) {
				cerr << "No '" << NAME << "' data in '" << DataFileName << "' !" << txf::nl;
				ERRExit( EXIT_FAILURE );
			} else
				Continue = false;
			break;
		case xml::tError:
			Locales.Init();
			ErrorMessage.Init();
			cerr << xml::GetTranslation( Browser.Status(), str::string(), Locales, ErrorMessage ) << txf::nl;
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

#define DATA_FILENAME_TAG	"Data"

void Process_(
	const rgstry::registry_ &Registry,
	rgstry::row__ RegistryRoot )
{
ERRProlog
	str::string DataFileName;
	STR_BUFFER___ Buffer;
ERRBegin
	DataFileName.Init();

	if ( !Registry.GetValue( str::string( DATA_FILENAME_TAG ), RegistryRoot, DataFileName ) ) {
		cerr << "No '" << DATA_FILENAME_TAG << "' tag found !" << txf::nl;
		ERRExit( EXIT_FAILURE );
	}

	ProcessDataFile_( DataFileName.Convert( Buffer ) );
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

	if ( ( Error = rgstry::FillRegistry( Project, "Projects/Project[@Target=\"" NAME "\']", Registry, RegistryRoot, ErrorDetails ) ) != rgstry::eOK ) {
		Locales.Init();
		ErrorMessage.Init();
		cerr << rgstry::GetTranslation( Error, ErrorDetails,  str::string(), Locales, ErrorMessage ) << txf::nl;
		ERRExit( EXIT_FAILURE );
	}
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
