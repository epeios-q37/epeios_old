/*
	'frdkernl.cpp' by Claude SIMON (http://zeusw.org/).

	 This file is part of 'emobda' software.

    'emobda' is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'emobda' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with 'emobda'.  If not, see <http://www.gnu.org/licenses/>.
*/

// $Id$

#include "frdkernl.h"
#include "flf.h"
#include "flx.h"
#include "lcl.h"

static lcl::locales _Locales;

using namespace frdkernl;
using xml::writer_;

#define CASE( message )	\
	case m##message:\
		Message = #message;\
		break

static const char *GetRawMessage_( frdkernl::message__ MessageId )
{
	const char *Message = NULL;

	switch ( MessageId ) {
	CASE( SelectProjectFile );
	CASE( UnableToOpenConfigFile );
	CASE( MissingConfigurationTree );
	CASE( MissingConfigurationId );
	CASE( MissingDatabaseName );
	CASE( MissingDatabasePath );
	CASE( NoDatabaseSelected );
	CASE( BadDatabasePath );
	CASE( DropStructureItemConfirmation );
	CASE( DeleteTableConfirmation );
	CASE( DeleteFieldConfirmation );
	CASE( DeleteRecordConfirmation );
	CASE( DropRecordConfirmation );
	CASE( UnableToReachRecord );
	CASE( NotImplementedYet );

	default:
		ERRu();
		break;
	}

	return Message;
}

void frdkernl::kernel___::Init(
	const str::string_ &Language,
	const char *LocalesFileName )
{
ERRProlog
	flf::file_iflow___ FIFlow;
	xtf::extended_text_iflow__ XTFlow;
ERRBegin
	reset();

	_Language.Init( Language );

	if ( FIFlow.Init( LocalesFileName, err::hSkip ) == fil::sSuccess ) {
		FIFlow.EOFD( XTF_EOXT );

		XTFlow.Init( FIFlow );

		_Locales.Init( XTFlow, "Locale[target=\"emobdafrd\"]" );
	}

	// Other initialisations happend in 'OpenProject'.
ERRErr
ERREnd
ERREpilog
	// Other initialisations happend in 'OpenProject'.
}


message__ frdkernl::kernel___::OpenProject(
	const char *ConfigFile,
	const char *RootPath,
	str::string_ &ConfigurationId )
{
	message__ Message = m_Undefined;
ERRProlog
	flf::file_iflow___ FIFlow;
	xtf::extended_text_iflow__ XFlow;
ERRBegin
	if ( FIFlow.Init( ConfigFile, err::hSkip ) != fil::sSuccess ) {
		Message = mUnableToOpenConfigFile;
		ERRReturn;
	}

	FIFlow.EOFD( XTF_EOXT );

	XFlow.Init( FIFlow );

	Message = OpenProject( XFlow, RootPath, ConfigurationId  );
ERRErr
ERREnd
ERREpilog
	return Message;
}

void frdkernl::kernel___::_Connect(
	const char *RemoteHostServiceOrLocalLibraryPath,
	csducl::type__ Type )
{
	_ClientCore.Init( RemoteHostServiceOrLocalLibraryPath, NULL, _LogFunctions, Type );
	_backend___::Init( _ClientCore );
}


void frdkernl::kernel___::Connect(
	const char *RemoteHostServiceOrLocalLibraryPath,
	csducl::type__ Type )
{
	Close();

	_Connect( RemoteHostServiceOrLocalLibraryPath, Type );
}


void frdkernl::kernel___::_Connect(
	const str::string_ &RemoteHostServiceOrLocalLibraryPath,
	csducl::type__ Type )
{
ERRProlog
	STR_BUFFER___ RemoteHostServiceOrLocalLibraryPathBuffer;
ERRBegin
	_Connect( RemoteHostServiceOrLocalLibraryPath.Convert( RemoteHostServiceOrLocalLibraryPathBuffer ), Type );
ERRErr
ERREnd
ERREpilog
}

void frdkernl::kernel___::Connect(
	const str::string_ &RemoteHostServiceOrLocalLibraryPath,
	csducl::type__ Type )
{
ERRProlog
	STR_BUFFER___ RemoteHostServiceOrLocalLibraryPathBuffer;
ERRBegin
	Connect( RemoteHostServiceOrLocalLibraryPath.Convert( RemoteHostServiceOrLocalLibraryPathBuffer ), Type );
ERRErr
ERREnd
ERREpilog
}

message__ frdkernl::kernel___::OpenProject(
	xtf::extended_text_iflow__ &Config,
	const char *RootPath,
	str::string_ &ConfigurationId )
{
	message__ Message = m_Undefined;
ERRProlog
	str::string RemoteHostServiceOrLocalLibraryPath;
	rgstry::nrow__ BaseRoot = NONE;
	epeios::row__ PathErrorRow = NONE;
	rgstry::erow__ AttributeEntryRow = NONE;
ERRBegin
	Close();

	_GlobalRegistry.Init();

	BaseRoot = rgstry::Parse( Config, str::string( "." ), _GlobalRegistry, NONE );
//	UserRoot = rgstry::Parse( UserConfig, str::string( "." ), _GlobalRegistry, NONE );

	if ( BaseRoot == NONE ) {
		Message = mMissingConfigurationTree;
		ERRReturn;
	}
	
	if ( ( BaseRoot = _GlobalRegistry.SearchPath( rgstry::term( RootPath ), BaseRoot, AttributeEntryRow, PathErrorRow ) ) == NONE ) {
		if ( PathErrorRow != NONE )
			ERRc();

		if ( AttributeEntryRow != NONE )
			ERRc();

		Message = mMissingConfigurationTree;

		ERRReturn;
	}

	_Registry.Init( _GlobalRegistry, BaseRoot );

	if ( !GetRegistryValue( "@Id", ConfigurationId ) ) {
		Message = mMissingConfigurationId;
		ERRReturn;
	}

	RemoteHostServiceOrLocalLibraryPath.Init();

	if ( GetRegistryValue( frdrgstry::paths::Parameters.Backend.Location, RemoteHostServiceOrLocalLibraryPath ) )
		_Connect( RemoteHostServiceOrLocalLibraryPath, csducl::tShared );

	_Records.Init();

	_ProjectIsOpen = true;
ERRErr
ERREnd
ERREpilog
	return Message;
}

void frdkernl::kernel___::SetLocalRegistry(
	xtf::extended_text_iflow__ &Config,
	const str::string_ &Path )
{
ERRProlog
	rgstry::nrow__ BaseRoot = NONE;
	epeios::row__ PathErrorRow = NONE;
	rgstry::erow__ AttributeEntryRow = NONE;
ERRBegin
	if ( _LocalRegistryRoot != NONE )
		ERRu();

	_LocalRegistryRoot = rgstry::Parse( Config, str::string( "." ), _GlobalRegistry, NONE );

	BaseRoot = _GlobalRegistry.SearchPath( Path, _LocalRegistryRoot, AttributeEntryRow, PathErrorRow );

	if ( PathErrorRow != NONE )
		ERRc();

	if ( AttributeEntryRow != NONE )
		ERRc();

	if ( BaseRoot == NONE ) {
		BaseRoot = _GlobalRegistry.CreatePath( Path, _LocalRegistryRoot, PathErrorRow );

		if ( PathErrorRow != NONE )
			ERRc();
	}

	_Registry.SetLocal( _GlobalRegistry, BaseRoot );
ERRErr
ERREnd
ERREpilog
}

static const char *GetMessage_(
	frdkernl::message__ Message,
	const str::string_ &Language,
	STR_BUFFER___ &Buffer )
{
ERRProlog
	str::string Translation;
ERRBegin
	Translation.Init();

	_Locales.GetTranslation( str::string( GetRawMessage_( Message ) ), Language, Translation );

	Translation.Convert( Buffer );
ERRErr
ERREnd
ERREpilog
	return Buffer();
}

const char *frdkernl::kernel___::GetMessage(
	frdkernl::message__ MessageId,
	STR_BUFFER___ &Buffer  )
{
	return GetMessage_( MessageId, _Language, Buffer );
}

static inline void PutInt_(
	const char *Name,
	bso::ulong__ Value,
	writer_ &Writer )
{
	bso::integer_buffer__ Buffer;

	Writer.PutAttribute( Name, bso::Convert( Value, Buffer ) );
}

template <typename id> static inline void PutId_(
	const char *Name,
	id Id,
	writer_ &Writer )
{
	PutInt_( Name, **Id, Writer );
}

void frdkernl::kernel___::ApplyQuery( void )
{
	_Records.Init();

	GetRecords( Target().Table, _Records );
}

void Dump_(
	const str::string_ &Database,
	writer_ &Writer )
{
	Writer.PushTag( "Database" );
	Writer.PushTag( "Path" );

	Writer.PutValue( Database );

	Writer.PopTag();
	Writer.PopTag();
}

void Dump_(
	const bkdacc::strings_ &Databases,
	writer_ &Writer )
{
	ctn::E_CMITEM( str::string_ ) Database;
	epeios::row__ Row = Databases.First();

	Database.Init( Databases );

	Writer.PushTag( "Databases" );

	while ( Row != NONE ) {
		Dump_( Database( Row ), Writer );

		Row = Databases.Next( Row );
	}

	Writer.PopTag();
}


void frdkernl::kernel___::_DumpAvailableDatabases( writer_ &Writer )
{
ERRProlog
	bkdacc::strings Databases;
ERRBegin
	Databases.Init();

	if ( IsConnected() )
		GetAvailableDatabases( Databases );

	Writer.PushTag( "Available" );

	Dump_( Databases, Writer );

	Writer.PopTag();
ERRErr
ERREnd
ERREpilog
}

static void DumpField_(
	table__ Table,
	field__ Field,
	const bkdacc::string_ &Name,
	const bkdacc::string_ &Comment,
	field_id__ Id,
	writer_ &Writer )
{
	Writer.PushTag( "Field" );

	PutId_( "Row", Field, Writer );
	PutId_( "TableRow", Table, Writer );
	PutId_( "Id", Id, Writer );

	Writer.PushTag( "Name" );
	Writer.PutValue( Name );
	Writer.PopTag();

	Writer.PushTag( "Comment" );
	Writer.PutValue( Comment );
	Writer.PopTag();

	Writer.PopTag();
}


static void DumpFields_(
	const fields_ &Fields,
	backend___ &Backend,
	writer_ &Writer )
{
ERRProlog
	bkdacc::strings Names, Comments;
	field_ids Ids;
	tables Tables;
	epeios::row__ Row = NONE;
ERRBegin
	Writer.PushTag( "Fields" );
	PutInt_( "Amount", Fields.Amount(), Writer );

	Names.Init();
	Comments.Init();
	Ids.Init();
	Tables.Init();

	if ( Backend.IsConnected() )
		Backend.GetFieldsInfos( Fields, Names, Comments, Ids, Tables );

	Row = Fields.First();

	while ( Row != NONE ) {
		DumpField_( Tables( Row ), Fields( Row ), Names( Row ), Comments( Row ), Ids( Row ), Writer );

		Row = Fields.Next( Row );
	}

	Writer.PopTag();

ERRErr
ERREnd
ERREpilog
}


static void DumpFields_(
	backend___ &Backend,
	writer_ &Writer )
{
ERRProlog
	fields Fields;
ERRBegin	
	Fields.Init();

	if ( Backend.IsConnected() )
		Backend.GetFields( UNDEFINED_TABLE, Fields );

	DumpFields_( Fields, Backend, Writer );
ERRErr
ERREnd
ERREpilog
}


static void DumpTable_(
	const bkdacc::string_ &Name,
	const bkdacc::string_ &Comment,
	table__ Table,
	table_id__ Id,
	writer_ &Writer )
{
	PutId_( "Id", Id, Writer );
	PutId_( "Row", Table, Writer );

	Writer.PushTag( "Name" );
	Writer.PutValue( Name );
	Writer.PopTag();

	Writer.PushTag( "Comment" );
	Writer.PutValue( Comment );
	Writer.PopTag();
}

static void DumpTables_(
	const bkdacc::strings_ &Names,
	const bkdacc::strings_ &Comments,
	const tables_ &Tables,
	const table_ids_ &Ids,
	writer_ &Writer )
{
	ctn::E_CMITEM( bkdacc::string_ ) Name, Comment;
	epeios::row__ Row = Names.First();

	if ( Names.Amount() != Comments.Amount() )
		ERRc();

	if ( Names.Amount() != Ids.Amount() )
		ERRc();

	Name.Init( Names );
	Comment.Init( Comments );

	while ( Row != NONE ) {
		Writer.PushTag( "Table" );

		DumpTable_( Name( Row ), Comment( Row ), Tables( Row ), Ids( Row ), Writer );

		Writer.PopTag();

		Row = Names.Next( Row );
	}
}

static void DumpTables_(
	backend___ &Backend,
	writer_ &Writer )
{
ERRProlog
	tables Tables;
	bkdacc::strings Names, Comments;
	table_ids Ids;
ERRBegin
	Tables.Init();

	if ( Backend.IsConnected() )
		Backend.GetTables( Tables );

	Names.Init();
	Comments.Init();
	Ids.Init();

	if ( Backend.IsConnected() )
		Backend.GetTablesInfos( Tables, Names, Comments, Ids );

	if ( Tables.Amount() != Names.Amount() )
		ERRc();

	if ( Tables.Amount() != Ids.Amount() )
		ERRc();

	Writer.PushTag( "Tables" );

	DumpTables_( Names, Comments, Tables, Ids, Writer );

	Writer.PopTag();

ERRErr
ERREnd
ERREpilog
}

static void DumpDatabases_(
	backend___ &Backend,
	writer_ &Writer )
{
ERRProlog
	bkdacc::string Name, Comment;
ERRBegin
	Name.Init();
	Comment.Init();

	if ( Backend.IsConnected() )
		Backend.GetDatabaseInfos( Name, Comment );

	Writer.PushTag( "Databases" );

	if ( Name.Amount() != 0 ) {

		Writer.PushTag( "Database" );

		Writer.PushTag( "Name" );
		Writer.PutValue( Name );
		Writer.PopTag();

		Writer.PushTag( "Comment" );
		Writer.PutValue( Comment );
		Writer.PopTag();

		Writer.PopTag();
	}

	Writer.PopTag();
ERRErr
ERREnd
ERREpilog
}

void frdkernl::kernel___::_DumpStructure( writer_ &Writer )
{
	Writer.PushTag( "Structure" );

	DumpDatabases_( *this, Writer );
	DumpTables_( *this, Writer );
	DumpFields_( *this, Writer );

	Writer.PopTag();
}

static void Dump_(
	const target__ &Target,
	writer_ &Writer )
{
	if ( Target.Table != UNDEFINED_TABLE )
		PutId_( "Table", Target.Table, Writer );

	if ( Target.Field != UNDEFINED_FIELD )
		PutId_( "Field", Target.Field, Writer );

	if ( Target.Record != UNDEFINED_RECORD )
		PutId_( "Record", Target.Record, Writer );
}

void frdkernl::kernel___::_DumpCurrent(
	writer_ &Writer,
	bso::bool__ ContextIsStandard )
{
	Writer.PushTag( "Current" );

	Writer.PutAttribute( "Context", ContextIsStandard ? "Standard" : "Special" );

	Dump_( _Target, Writer );

	Writer.PopTag();
}

static void inline Dump_(
	field__ Field,
	const bkdacc::string_ &Datum,
	writer_ &Writer )
{
	Writer.PushTag( "Datum" );

	PutId_( "Field", Field, Writer );

	Writer.PutValue( Datum );

	Writer.PopTag();
}


static void Dump_(
	record__ Record,
	const fields_ &Fields,
	const bkdacc::strings_ &Data,
	writer_ &Writer )
{
	ctn::E_CMITEM( bkdacc::string_ ) Datum;
	epeios::row__ Row = Data.First();

	if ( Fields.Amount() != Data.Amount() )
		ERRc();

	Datum.Init( Data );

	Writer.PushTag( "Record" );

	PutId_( "Id", Record, Writer );

	Writer.PushTag( "Data" );

	while ( Row != NONE ) {
		Dump_( Fields( Row ), Datum( Row ), Writer );

		Row = Fields.Next( Row );
	}

	Writer.PopTag();

	Writer.PopTag();
}

static void Dump_(
	const records_ &Records,
	const fields_ &Fields,
	const bkdacc::xstrings_ &DataCluster,
	writer_ &Writer )
{
	ctn::E_CITEM( bkdacc::strings_ ) Data;
	epeios::row__ Row = Records.First();

	Data.Init( DataCluster );

	if ( Records.Amount() != DataCluster.Amount() )
		ERRc();

	Writer.PushTag( "Records" );

	PutInt_( "Amount", Records.Amount(), Writer );

	while ( Row != NONE ) {
		Dump_( Records( Row ), Fields, Data( Row ), Writer );

		Row = Records.Next( Row );
	}

	Writer.PopTag();
}


void frdkernl::kernel___::_DumpContent( writer_ &Writer )
{
ERRProlog
	fields Fields;
	bkdacc::xstrings DataCluster;
	records Records;
	table__ Table = UNDEFINED_TABLE;
ERRBegin
	Writer.PushTag( "Content" );

	if ( ( Table = Target().Table ) != UNDEFINED_TABLE ) {

		Fields.Init();
		this->GetFields( Table, Fields );

		Records.Init();

		if ( Target().Record == UNDEFINED_RECORD )
			Records = _Records;
		else
			Records.Append( Target().Record );

		DataCluster.Init();
		this->GetRecordsData( Table, Fields, Records, DataCluster );

		Dump_( Records, Fields, DataCluster, Writer );
	}

	Writer.PopTag();
ERRErr
ERREnd
ERREpilog
}


void frdkernl::kernel___::DumpAsXML(
	str::string_ &XML,
	bso::bool__ ContextIsStandard )
{
ERRProlog
	flx::E_STRING_OFLOW___ Flow;
	txf::text_oflow__ TFlow( Flow );
	xml::writer Writer;
ERRBegin
	Flow.Init( XML );
	Writer.Init( TFlow, true );

	Writer.PushTag( "emobda" );
	_DumpCurrent( Writer, ContextIsStandard );

	_DumpAvailableDatabases( Writer );

	_DumpStructure( Writer );

	_DumpContent( Writer );

	Writer.PopTag();

	Writer.reset();

	Flow.reset();
ERRErr
ERREnd
ERREpilog
}
