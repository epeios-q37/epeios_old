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
	mIncorrectTableName,
	mUnableToCreateTable,
	mNoSuchTable,
	mIncorrectFieldName,
	mNameAlreadyUsed,
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
	CASE( IncorrectTableName );
	CASE( UnableToCreateTable );
	CASE( NoSuchTable );
	CASE( IncorrectFieldName );
	CASE( NameAlreadyUsed );
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

	if ( !Manager.Init( Location, Name, Comment, dbstbl::mAdmin, true, true ) ) {
		Message = mUnableToCreateDatabase;
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
ERRErr
ERREnd
ERREpilog
	return Message;
}

static bso::bool__ TestAndNormalizeTableName_( str::string_ &Name )
{
	return TestAndNormalize_( Name );
}

DEC( CreateTable )
{
	message__ Message = mOK;
ERRProlog
	bkdmng::string Name, Comment;
	table_row__ TableRow = NONE;
	mbdtbl::table_description Description;
ERRBegin
	Name.Init();
	Name = Request.StringIn();

	Comment.Init();
	Comment = Request.StringIn();

	if ( !TestAndNormalizeTableName_( Name ) ) {
		Message = mIncorrectTableName;
		ERRReturn;
	}

	if ( Manager.SearchTable( Name ) != NONE ) {
		Message = mNameAlreadyUsed;
		ERRReturn;
	}

	Description.Init( Name, Comment );

	if ( ( TableRow = Manager.AddTable( Description ) ) == NONE ) {
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
	bkdmng::ids32_ &Rows,
	bkdmng::strings_ &Names,
	bkdmng::strings_ &Comments,
	bkdmng::ids16_ &Ids )
{
	ctn::E_CITEMt( mbdtbl::table_, mbdtbl::table_row__ ) Table;
	mbdtbl::table_row__ Row = Tables.First();

	Table.Init( Tables );

	while ( Row != NONE ) {
		Rows.Append( *Row );
		Names.Append( Table( Row ).Name );
		Comments.Append( Table( Row ).Comment );
		Ids.Append( *Table( Row ).TableId() );


		Row = Tables.Next( Row );
	}
}

DEC( GetTables )
{
	message__ Message = mOK;
ERRProlog
ERRBegin
	bkdmng::ids32_ &Rows = Request.Ids32Out();
	bkdmng::strings_ &Names = Request.StringsOut();
	bkdmng::strings_ &Comments = Request.StringsOut();
	bkdmng::ids16_ &Ids = Request.Ids16Out();

	GetTables_( Manager.Structure.Tables, Rows, Names, Comments, Ids );
ERRErr
ERREnd
ERREpilog
	return Message;
}

static bso::bool__ TestAndNormalizeFieldName_( str::string_ &Name )
{
	return TestAndNormalize_( Name );
}

DEC( AddField )
{
	message__ Message = mOK;
ERRProlog
	bkdmng::string Name, Comment;
	table_row__ TableRow = NONE;
	field_row__ FieldRow = NONE;
	field_description Field;
ERRBegin

	TableRow = *Request.Id32In();

	Name.Init();
	Name = Request.StringIn();

	Comment.Init();
	Comment = Request.StringIn();

	Name.StripCharacter( ' ' );

	if ( !Manager.TableExists( TableRow ) ) {
		Message = mNoSuchTable;
		ERRReturn;
	}

	if ( !TestAndNormalizeFieldName_( Name ) ) {
		Message = mIncorrectFieldName;
		ERRReturn;
	}

	if ( Manager.SearchField( TableRow, Name ) != NONE ) {
		Message = mNameAlreadyUsed;
		ERRReturn;
	}

	Field.Init( Name, Comment );

	FieldRow = Manager.AddField( TableRow, Field );

	Request.Id32Out() = *FieldRow;
ERRErr
ERREnd
ERREpilog
	return Message;
}

static const bkdmng::items32_ &Convert_(
	const fields_ &Fields,
	bkdmng::items32_ &Items )
{
ERRProlog
	field_row__ FieldRow = NONE;
	ctn::E_CITEMt( field_, field_row__ ) Field;
	bkdmng::item32 Item;
ERRBegin
	Field.Init( Fields );

	FieldRow = Fields.First();

	while ( FieldRow != NONE ) {
		Item.Init( *FieldRow, Field( FieldRow ).Name );

		Items.Append( Item );

		FieldRow = Fields.Next( FieldRow );
	}
ERRErr
ERREnd
ERREpilog
	return Items;
}

DEC( GetFields )
{
	message__ Message = mOK;
ERRProlog
	bkdmng::items32 Items;
ERRBegin
	Items.Init();

	Request.Items32Out() = Convert_( Manager.Structure.Fields, Items );
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
	Module.Add( D( GetDatabaseInfos ),
		bkdmng::cEnd,
			bkdmng::cString,	// Nom de la base de donnée.
		bkdmng::cEnd );
	Module.Add( D( CreateTable ),
			bkdmng::cString,	// Table name.
			bkdmng::cString,	// Table comment.
		bkdmng::cEnd,
			bkdmng::cId32,		// Table row.
		bkdmng::cEnd );
	Module.Add( D( GetTables ),
		bkdmng::cEnd,
			bkdmng::cIds32,		// 'row's.
			bkdmng::cStrings,	// Noms.
			bkdmng::cStrings,	// Commentaires.
			bkdmng::cIds16,		// 'id's.
		bkdmng::cEnd );
	Module.Add( D( AddField ),
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
	{}
} Starter_;