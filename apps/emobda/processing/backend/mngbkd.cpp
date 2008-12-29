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

enum message__ {
	mOK,
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


