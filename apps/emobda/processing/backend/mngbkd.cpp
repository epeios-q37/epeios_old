/*
	'mngbkd.cpp' by Claude SIMON (http://zeusw.org/).

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

#include "mngbkd.h"
#include "dir.h"

using namespace mngbkd;
using namespace mbdmng;

const char *mngbkd::manager_::PREFIX = MNGBKD_MANAGER_PREFIX;
const char *mngbkd::manager_::NAME = MNGBKD_MANAGER_NAME;

enum message__ {
	mOK,
	mIncorrectLocation,

	mIncorrectDatabaseName,
	mUnableToCreateDatabase,
	mUnableToOpenDatabase,

	mNameAlreadyUsed,

	mIncorrectTableName,
	mUnableToCreateTable,
	mUnknownTable,

	mIncorrectFieldName,
	mUnknownField,
	mFieldNotOwnedByTable,

	mBadFieldRow,
	mSameFieldTwice,

	mUndefinedRecord,

	m_amount,
	m_Undefined
};

#define CASE( i )	\
	case m##i:\
		Message = "MANAGER_" #i;\
		break

static const char *GetRawMessage_( message__ MessageId )
{
	const char *Message = NULL;

	switch ( MessageId ) {
	CASE( OK );
	CASE( IncorrectLocation );
	CASE( IncorrectDatabaseName );
	CASE( UnableToCreateDatabase );
	CASE( UnableToOpenDatabase );
	CASE( IncorrectTableName );
	CASE( UnableToCreateTable );
	CASE( UnknownTable );
	CASE( IncorrectFieldName );
	CASE( NameAlreadyUsed );
	CASE( UnknownField );
	CASE( FieldNotOwnedByTable );
	CASE( BadFieldRow );
	CASE( SameFieldTwice );
	CASE( UndefinedRecord );
	break;
	default:
		ERRu();
		break;
	}

	return Message;
}

typedef msg::i18_messages_ _messages_;
typedef msg::i18_messages _messages;

static class messages
: public _messages
{
protected:
	const char *MSGGetRawMessage( int MessageId ) const
	{
		return GetRawMessage_( (message__)MessageId );
	}
} Messages_;

void mngbkd::manager_::RAW_MESSAGES( msg::messages_ &Messages )
{
	::Messages_.DumpRawMessages( Messages );
}

_messages_ &mngbkd::GetMessages( void )
{
	return ::Messages_;
}

static const char *GetMessage_(
	message__ MessageId,
	lgg::language__ Language,
	msg::buffer__ &Buffer )
{
	return ::Messages_.GetMessage( MessageId, Language, Buffer );
}

typedef message__ (* f_manager )(
	_manager_ &Manager,
	bkdmng::request_manager__ &Request );

#define DEC( name )\
static message__ name(\
	_manager_ &Manager,\
	bkdmng::request_manager__ &Request )

void mngbkd::manager_::HANDLE(
	bkdmng::backend &Backend,
	bkdmng::untyped_module &Module,
	bkdmng::index__ Index,
	bkdmng::command__ Command,
	bkdmng::request_manager__ &Request,
	void *UP )
{
ERRProlog
	message__ Message = m_Undefined;
	msg::buffer__ RawMessageBuffer, I18MessageBuffer;
ERRBegin
	Message = ((f_manager)Module.UPs( Command ))( *this, Request );

	if ( Message != mOK )
		Request.SendExplanationMessage(
			GetMessage_( Message, lgg::lRaw, RawMessageBuffer ),
			GetMessage_( Message, Backend.GetLanguage(), I18MessageBuffer ) );
ERRErr
ERREnd
ERREpilog
}

static bso::bool__ TestAndNormalize_( str::string_ &Name )
{
	Name.StripCharacter( ' ' );

	if ( Name.Amount() == 0 )
		return false;

	return true;
}

DEC( CreateDatabase )
{
	message__ Message = mOK;
ERRProlog
	str::string Location, Name, Comment;
	tol::E_FPOINTER___( bso::char__ ) Buffer;
ERRBegin
	Location.Init();
	Location = Request.StringIn();

	Name.Init();
	Name = Request.StringIn();

	Comment.Init();
	Comment = Request.StringIn();

	if ( !TestAndNormalize_( Location ) ) {
		Message = mIncorrectLocation;
		ERRReturn;
	}

	if ( !TestAndNormalize_( Name ) ) {
		Message = mIncorrectDatabaseName;
		ERRReturn;
	}

/*
	if ( dir::CreateDir( Buffer = Location.Convert() ) != dir::sOK ) {
		Message = mUnableToCreateDatabase;
		ERRReturn;
	}
*/

	if ( !Manager.Init( Location, dbstbl::mAdmin, true, mbdmng::tCreate ) ) {
		Message = mUnableToCreateDatabase;
		ERRReturn;
	}

	Manager.SetNameAndComment( Name, Comment );
ERRErr
ERREnd
ERREpilog
	return Message;
}

DEC( OpenDatabase )
{
	message__ Message = mOK;
ERRProlog
	str::string Location;
	tol::E_FPOINTER___( bso::char__ ) Buffer;
ERRBegin
	Location.Init();
	Location = Request.StringIn();

	if ( !TestAndNormalize_( Location ) ) {
		Message = mIncorrectLocation;
		ERRReturn;
	}

/*
	if ( dir::CreateDir( Buffer = Location.Convert() ) != dir::sOK ) {
		Message = mUnableToCreateDatabase;
		ERRReturn;
	}
*/

	if ( !Manager.Init( Location, dbstbl::mAdmin, false, mbdmng::tRetrieve ) ) {
		Message = mUnableToOpenDatabase;
		ERRReturn;
	}
ERRErr
ERREnd
ERREpilog
	return Message;
}

DEC( GetDatabaseInfos )
{
	message__ Message = mOK;
ERRProlog
ERRBegin
	Request.StringOut() = Manager.Structure.Name;
	Request.StringOut() = Manager.Structure.Comment;
ERRErr
ERREnd
ERREpilog
	return Message;
}

static bso::bool__ TestAndNormalizeTableName_( str::string_ &Name )
{
	return TestAndNormalize_( Name );
}

DEC( CreateOrModifyTable )
{
	message__ Message = mOK;
ERRProlog
	bkdmng::string Name, Comment;
	table_row__ TableRow = NONE;
	mbdtbl::table_description Description;
ERRBegin
	TableRow = *Request.Id32In();	// Si == 'NONE' cr�ation, sinon modification.

	if ( TableRow != NONE )
		if ( !Manager.TableExists( TableRow ) ) {
			Message = mUnknownTable;
			ERRReturn;
		}


	Name.Init();
	Name = Request.StringIn();

	Comment.Init();
	Comment = Request.StringIn();

	if ( !TestAndNormalizeTableName_( Name ) ) {
		Message = mIncorrectTableName;
		ERRReturn;
	}

	if ( ( Manager.SearchTable( Name ) != NONE )
	     && ( Manager.SearchTable( Name ) != TableRow ) )
	{
		Message = mNameAlreadyUsed;
		ERRReturn;
	}

	Description.Init( Name, Comment );

	if ( TableRow != NONE )
		Manager.ModifyTable( TableRow, Description );
	else if ( ( TableRow = Manager.AddTable( Description ) ) == NONE ) {
		Message = mUnableToCreateTable;
		ERRReturn;
	}

	Request.Id32Out() = *TableRow;
ERRErr
ERREnd
ERREpilog
	return Message;
}

static void GetTables_(
	const mbdtbl::tables_ &Tables,
	bkdmng::ids32_ &Rows )
{
	ctn::E_CITEMt( mbdtbl::table_, mbdtbl::table_row__ ) Table;
	mbdtbl::table_row__ Row = Tables.First();

	Table.Init( Tables );

	while ( Row != NONE ) {
		Rows.Append( *Row );

		Row = Tables.Next( Row );
	}
}

DEC( GetTables )
{
	message__ Message = mOK;
ERRProlog
ERRBegin
	bkdmng::ids32_ &Rows = Request.Ids32Out();

	GetTables_( Manager.Structure.Tables, Rows );
ERRErr
ERREnd
ERREpilog
	return Message;
}

static message__ GetTablesInfos_(
	const bkdmng::ids32_ &TableRows,
	const mbdtbl::tables_ &Tables,
	bkdmng::strings_ &Names,
	bkdmng::strings_ &Comments,
	bkdmng::ids8_ &Ids )
{
	ctn::E_CITEMt( mbdtbl::table_, mbdtbl::table_row__ ) Table;
	epeios::row__ Row = TableRows.First();
	table_row__ TableRow = NONE;

	Table.Init( Tables );

	while ( Row != NONE ) {
		TableRow = *TableRows( Row );

		if ( !Tables.Exists( TableRow ) )
			return mUnknownTable;

		Names.Append( Table( TableRow ).Name );
		Comments.Append( Table( TableRow ).Comment );
		Ids.Append( *Table( TableRow ).Id() );


		Row = TableRows.Next( Row );
	}

	return mOK;
}

DEC( GetTablesInfos )
{
	message__ Message = mOK;
ERRProlog
ERRBegin
	const bkdmng::ids32_ &Rows = Request.Ids32In();
	bkdmng::strings_ &Names = Request.StringsOut();
	bkdmng::strings_ &Comments = Request.StringsOut();
	bkdmng::ids8_ &Ids = Request.Ids8Out();

	GetTablesInfos_( Rows, Manager.Structure.Tables, Names, Comments, Ids );
ERRErr
ERREnd
ERREpilog
	return Message;
}

static bso::bool__ TestAndNormalizeFieldName_( str::string_ &Name )
{
	return TestAndNormalize_( Name );
}

DEC( AddOrModifyField )
{
	message__ Message = mOK;
ERRProlog
	bkdmng::string Name, Comment;
	table_row__ TableRow = NONE;
	field_row__ FieldRow = NONE;
	field_description Field;
ERRBegin
	FieldRow = *Request.Id32In();	// Si = 'NONE', cr�ation, sinon modification.

	TableRow = *Request.Id32In();

	Name.Init();
	Name = Request.StringIn();

	Comment.Init();
	Comment = Request.StringIn();

	Name.StripCharacter( ' ' );

	if ( !Manager.TableExists( TableRow ) ) {
		Message = mUnknownTable;
		ERRReturn;
	}

	if ( FieldRow != NONE ) {
		if ( !Manager.FieldExists( FieldRow ) ) {
			Message = mUnknownField;
			ERRReturn;
		} else if ( !Manager.IsFieldOwnedByTable( FieldRow, TableRow ) ) {
			Message = mFieldNotOwnedByTable;
			ERRReturn;
		}
	}

	if ( !TestAndNormalizeFieldName_( Name ) ) {
		Message = mIncorrectFieldName;
		ERRReturn;
	}

	if ( ( Manager.SearchField( TableRow, Name ) != NONE ) 
		  && ( Manager.SearchField( TableRow, Name ) != FieldRow ) )
	{
		Message = mNameAlreadyUsed;
		ERRReturn;
	}

	Field.Init( Name, Comment );

	if ( FieldRow != NONE )
		Manager.ModifyField( FieldRow, Field );
	else
		FieldRow = Manager.AddField( TableRow, Field );

	Request.Id32Out() = *FieldRow;
ERRErr
ERREnd
ERREpilog
	return Message;
}

static void Convert_(
	const field_rows_ &Rows,
	bkdmng::ids32_ &Ids )
{
	epeios::row__ Row = Rows.First();

	while ( Row != NONE ) {
		Ids.Append( *Rows( Row ) );

		Row = Rows.Next( Row );
	}
}

static void SetWith_(
	const fields_ &Fields,
	bkdmng::ids32_ &Ids )
{
	field_row__ Row = Fields.First();

	while ( Row != NONE ) {
		Ids.Append( *Row );

		Row = Fields.Next( Row );
	}
}
	

DEC( GetFields )
{
	message__ Message = mOK;
ERRProlog
	table_row__ TableRow = NONE;
	ctn::E_CITEMt( table_, table_row__ ) Table;
ERRBegin
	TableRow = *Request.Id32In();
	bkdmng::ids32_ &Fields = Request.Ids32Out();

	if ( TableRow != NONE ) {
		if ( !Manager.TableExists( TableRow ) ) {
			Message = mUnknownTable;
			ERRReturn;	
		}

		Table.Init( Manager.Structure.Tables );

		Convert_( Table( TableRow ).Fields, Fields );
	} else
		SetWith_( Manager.Structure.Fields, Fields );
ERRErr
ERREnd
ERREpilog
	return Message;
}

static message__ GetFieldsInfos_(
	const bkdmng::ids32_ &FieldRows,
	const mbdtbl::fields_ &Fields,
	bkdmng::strings_ &Names,
	bkdmng::strings_ &Comments,
	bkdmng::ids8_ &Ids,
	bkdmng::ids32_ &Tables )
{
	ctn::E_CITEMt( mbdtbl::field_, mbdtbl::field_row__ ) Field;
	epeios::row__ Row = FieldRows.First();
	field_row__ FieldRow = NONE;

	Field.Init( Fields );

	while ( Row != NONE ) {
		FieldRow = *FieldRows( Row );

		if ( !Fields.Exists( FieldRow ) )
			return mUnknownField;

		Names.Append( Field( FieldRow ).Name );
		Comments.Append( Field( FieldRow ).Comment );
		Ids.Append( *Field( FieldRow ).Id() );
		Tables.Append( *Field( FieldRow ).TableRow() );


		Row = FieldRows.Next( Row );
	}

	return mOK;
}

DEC( GetFieldsInfos )
{
	message__ Message = mOK;
ERRProlog
ERRBegin
	const bkdmng::ids32_ &Rows = Request.Ids32In();
	bkdmng::strings_ &Names = Request.StringsOut();
	bkdmng::strings_ &Comments = Request.StringsOut();
	bkdmng::ids8_ &Ids = Request.Ids8Out();
	bkdmng::ids32_ &Tables = Request.Ids32Out();

	GetFieldsInfos_( Rows, Manager.Structure.Fields, Names, Comments, Ids, Tables );
ERRErr
ERREnd
ERREpilog
	return Message;
}

static message__ ConvertAndTest_(
	const bkdmng::items32_ &Items,
	data_ &Data,
	field_rows_ &FieldRows )
{
	ctn::E_CMITEM( bkdmng::item32_ ) Item;
	epeios::row__ Row = Items.First();
	field_row__ FieldRow = NONE;

	Item.Init( Items );

	while ( Row != NONE ) {
		FieldRow = *Item( Row ).ID();

		if ( FieldRow == NONE )
			return mBadFieldRow;

		if ( FieldRows.Search( FieldRow ) != NONE )
			return mSameFieldTwice;

		FieldRows.Append( FieldRow );

		Data.Append( Item( Row ).Value );

		Row = Items.Next( Row );
	}

	return mOK;
}

DEC( InsertRecord )
{
	message__ Message = mOK;
ERRProlog
	data Data;
	field_rows FieldRows;
	table_row__ TableRow = NONE;
ERRBegin
	Data.Init();
	FieldRows.Init();

	TableRow = *Request.Id32In();

	if ( ( Message = ConvertAndTest_( Request.Items32In(), Data, FieldRows ) ) != mOK )
		ERRReturn;

	if ( !Manager.FieldsExist( FieldRows ) ) {
		Message = mUnknownField;
		ERRReturn;
	}

	if ( !Manager.TableExists( TableRow ) ) {
		Message = mUnknownTable;
		ERRReturn;
	}

	if ( !Manager.AreFieldsOwnedByTable( FieldRows, TableRow ) ) {
		Message = mFieldNotOwnedByTable;
		ERRReturn;
	}

	Request.Id16Out() = *Manager.AddRecord( Data, TableRow, FieldRows );
ERRErr
ERREnd
ERREpilog
	return Message;
}

static message__ Convert_(
	const bkdmng::ids32_ &Ids,
	field_rows_ &FieldRows )
{
	epeios::row__ Row = Ids.First();

	while ( Row != NONE ) {
		FieldRows.Append( *Ids( Row ) );

		Row = Ids.Next( Row );
	}

	return mOK;
}

static message__ Convert_(
	const bkdmng::ids16_ &Ids,
	record_ids_ &RecordIds )
{
	epeios::row__ Row = Ids.First();

	while ( Row != NONE ) {
		RecordIds.Append( *Ids( Row ) );

		Row = Ids.Next( Row );
	}

	return mOK;
}

DEC( GetRecords )
{
	message__ Message = mOK;
ERRProlog
	table_row__ TableRow = NONE;
ERRBegin
	TableRow = *Request.Id32In();

	if ( !Manager.TableExists( TableRow ) ) {
		Message = mUnknownTable;
		ERRReturn;
	}

	Manager.GetRecords( TableRow, *(record_ids_ *)&Request.Ids16Out() );

ERRErr
ERREnd
ERREpilog
	return Message;
}

DEC( GetRecordsData )
{
	message__ Message = mOK;
ERRProlog
	field_rows FieldRows;
	record_ids RecordIds;
	table_row__ TableRow = NONE;
ERRBegin
	TableRow = *Request.Id32In();

	FieldRows.Init();

	if ( ( Message = Convert_( Request.Ids32In(), FieldRows ) ) != mOK )
		ERRReturn;

	RecordIds.Init();

	if ( ( Message = Convert_( Request.Ids16In(), RecordIds ) ) != mOK )
		ERRReturn;

	if ( !Manager.TableExists( TableRow ) ) {
		Message = mUnknownTable;
		ERRReturn;
	}

	if ( !Manager.Exist( FieldRows ) ) {
		Message = mUnknownField;
		ERRReturn;
	}

	if ( !Manager.AreFieldsOwnedByTable( FieldRows, TableRow ) ) {
		Message = mFieldNotOwnedByTable;
		ERRReturn;
	}

	Manager.GetRecords( TableRow, FieldRows, RecordIds, Request.XStringsOut() );
ERRErr
ERREnd
ERREpilog
	return Message;
}


#define D( name )	#name, (void *)::##name

void mngbkd::manager_::NOTIFY( bkdmng::untyped_module &Module )
{
	Module.Add( D( CreateDatabase ),
			bkdmng::cString,	// Database location.
			bkdmng::cString,	// Database name.
			bkdmng::cString,	// Database comment.
		bkdmng::cEnd,
		bkdmng::cEnd );
	Module.Add( D( OpenDatabase ),
			bkdmng::cString,	// Database location.
		bkdmng::cEnd,
		bkdmng::cEnd );
	Module.Add( D( GetDatabaseInfos ),
		bkdmng::cEnd,
			bkdmng::cString,	// Nom de la base de donn�e.
			bkdmng::cString,	// Commentaire de la base de donn�e.
		bkdmng::cEnd );
	Module.Add( D( CreateOrModifyTable ),
			bkdmng::cId32,		// Table row ('NONE' for creation).
			bkdmng::cString,	// Table name.
			bkdmng::cString,	// Table comment.
		bkdmng::cEnd,
			bkdmng::cId32,		// Table row.
		bkdmng::cEnd );
	Module.Add( D( GetTables ),
		bkdmng::cEnd,
			bkdmng::cIds32,		// 'row's.
		bkdmng::cEnd );
	Module.Add( D( GetTablesInfos ),
			bkdmng::cIds32,		// Table 'rows'.
		bkdmng::cEnd,
			bkdmng::cStrings,	// Noms.
			bkdmng::cStrings,	// Commentaires.
			bkdmng::cIds8,		// 'id's.
		bkdmng::cEnd );
	Module.Add( D( GetFields ),
			bkdmng::cId32,		// Table row. Si == 'NONE', tous les champs sont retourn�s.
		bkdmng::cEnd,
			bkdmng::cIds32,		// 'row's.
		bkdmng::cEnd );
	Module.Add( D( GetFieldsInfos ),
			bkdmng::cIds32,		// Field 'rows'.
		bkdmng::cEnd,
			bkdmng::cStrings,	// Noms.
			bkdmng::cStrings,	// Commentaires.
			bkdmng::cIds8,		// 'id's.
			bkdmng::cIds32,		// 'Table's.
		bkdmng::cEnd );
	Module.Add( D( AddOrModifyField ),
		bkdmng::cId32,			// Field row ('NONE' for creation).
		bkdmng::cId32,			// Table row.
			bkdmng::cString,	// Field name.
			bkdmng::cString,	// Field comment.
		bkdmng::cEnd,
			bkdmng::cId32,		// Field row.
		bkdmng::cEnd );
	Module.Add( D( GetFields ),
		bkdmng::cEnd,
			bkdmng::cItems32,
		bkdmng::cEnd );
	Module.Add( D( InsertRecord ),
			bkdmng::cId32,		// Table row.
			bkdmng::cItems32,	// Field row and corresponding datum.
		bkdmng::cEnd,
			bkdmng::cId16,			// Record id.
		bkdmng::cEnd );
	Module.Add( D( GetRecords ),
			bkdmng::cId32,		// Table row.
		bkdmng::cEnd,
			bkdmng::cIds16,		// Record ids,
		bkdmng::cEnd );
	Module.Add( D( GetRecordsData ),
			bkdmng::cId32,		// Table row.
			bkdmng::cIds32,		// Field rows.
			bkdmng::cIds16,		// Record ids,
		bkdmng::cEnd,
			bkdmng::cXStrings,	// Data.
		bkdmng::cEnd );
}

#undef DEC
#undef D

#define DEC( name )\
	static inline void name(\
		bkdmng::backend_ &Backend,\
		bkdmng::untyped_module &,\
		bkdmng::index__,\
		bkdmng::command__,\
		bkdmng::request_manager__ &Request,\
		bso::bool__ &,\
		void * )

DEC( Test )
{
}



#define D( name )	"MBD" #name, ::name

void mngbkd::Inform( bkdmng::backend_ &Backend )
{
	Backend.Add( D( Test ),
		bkdmng::cEnd,
		bkdmng::cEnd );
}



static class starter 
{
public:
	starter( void )
	{
		::Messages_.Init( m_amount );
	}
	~starter( void )
	{
	}
} Starter_;