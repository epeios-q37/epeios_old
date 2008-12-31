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

using namespace mngbkd;
using namespace mbdmng;

enum message__ {
	mOK,
	mIncorrectFieldName,
	m_amount,
	m_Undefined
};

#define CASE( i )	\
	case m##i:\
		Message = "MANAGER_" #i;\
		break


const char *mngbkd::manager::PREFIX = MNGBKD_MANAGER_PREFIX;
const char *mngbkd::manager::NAME = MNGBKD_MANAGER_NAME;


static const char *GetRawMessage_( message__ MessageId )
{
	const char *Message = NULL;

	switch ( MessageId ) {
	CASE( OK );
	CASE( IncorrectFieldName );
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

void mngbkd::manager::RAW_MESSAGES( msg::messages_ &Messages )
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

void mngbkd::manager::HANDLE(
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

static bso::bool__ TestAndNormalizeFieldName_( str::string_ &Name )
{
	Name.StripCharacter( ' ' );

	if ( Name.Amount() == 0 )
		return false;

	return true;
}


DEC( AddField )
{
	message__ Message = mOK;
ERRProlog
	bkdmng::string Name;
	field_row__ FieldRow = NONE;
ERRBegin
	Name.Init();
	Name = Request.StringIn();

	Name.StripCharacter( ' ' );

	if ( !TestAndNormalizeFieldName_( Name ) ) {
		Message = mIncorrectFieldName;
		ERRReturn;
	}

	FieldRow = Manager.AddField( Name, FieldRow );

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

void mngbkd::manager::NOTIFY( bkdmng::untyped_module &Module )
{
	Module.Add( D( AddField ),
			bkdmng::cString,	// Field name.
		bkdmng::cEnd,
			bkdmng::cId32,		// Field row.
		bkdmng::cEnd );
	Module.Add( D( GetFields ),
		bkdmng::cEnd,
			bkdmng::cItems32,
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