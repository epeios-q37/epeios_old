/*
	'msg' library by Claude SIMON (csimon@epeios.org)
	Requires the 'msg' header file ('msg.h').
	Copyright (C) $COPYRIGHT_DATES$Claude SIMON (csimon@epeios.org).
$_RAW_$
	This file is part of the Epeios (http://epeios.org/) project.

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.
 
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, go to http://www.fsf.org/
	or write to the:
  
         	         Free Software Foundation, Inc.,
           59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/



//	$Id$

#define MSG__COMPILATION

#include "msg.h"

using namespace msg;

enum state__ {
	sUnknow,
	sEnd,
	sBase,
	sTranslation
};

static state__ GetFirstNonEmptyLine_(
	xtf::extended_text_iflow__ &Flow,
	str::string_ &Line )
{
	state__ State = sUnknow;

	while ( State == sUnknow ) {

		if ( Flow.EOX() )
			State = sEnd;
		else {
			Line.Init();

			Flow.GetLine( Line );

			if ( ( Line.Amount() != 0 ) && ( Line( 0 ) != '#' ) ) {
				if ( Line( 0 ) == '\t' )
					State = sTranslation;
				else
					State = sBase;
			}
		}
	}

	return State;
}

xtf::location__ msg::i18_messages_::LoadMessages_(
	xtf::extended_text_iflow__ &Flow,
	const messages_ &Base )
{
	xtf::location__ Location = 0;
ERRProlog
	str::string Line;
	state__ State = sUnknow;
	int Language = 1;
	epeios::row__ Row;
	ctn::E_CMITEM( message_ ) BaseMessage;
ERRBegin
	Row = Base.First();
	BaseMessage.Init( Base );

	Line.Init();

	do {
		Line.Init();

		switch ( State = GetFirstNonEmptyLine_( Flow, Line ) ) {
		case sEnd:
			break;
		case sBase:
			if ( ( Row == NONE ) || ( BaseMessage( Row ) != Line ) ) {
				State = sEnd;
				Location = Flow.Line();
			} else {
				English.New();
				French.New();
				German.New();
				Language = 1;
				Row = Base.Next( Row );
			}
			break;
		case sTranslation:
			if ( Line.Amount() > MSG_MESSAGE_LENGTH_MAX )
				ERRl();
			switch ( Language ) {
			case lgg::lEnglish:
				English( English.Last() ) = Line;
				English.Flush();
				break;
			case lgg::lFrench:
				French( French.Last() ) = Line;
				French.Flush();
				break;
			case lgg::lGerman:
				German( German.Last() ) = Line;
				German.Flush();
				break;
			default:
				State = sEnd;
				Location = Flow.Line();
				break;
			}
			Language++;
		}
	} while ( State != sEnd );

	if ( Row != NONE )
		Location = Flow.Line();
ERRErr
ERREnd
ERREpilog
	return Location;
}

xtf::location__ msg::i18_messages_::LoadMessages(
	xtf::extended_text_iflow__ &Flow,
	int Amount )
{
	xtf::location__ Location = 0;
ERRProlog
	messages Base;
ERRBegin
	Base.Init();

	for ( int i = 0; i < Amount ; i++ )
		Base.Append( str::string( _GetBaseMessage( i ) ) );

	Location = LoadMessages_( Flow, Base );
ERRErr
ERREnd
ERREpilog
	return Location;
}

const messages_ &msg::i18_messages_::_GetMessages( lgg::language__ Language ) const
{
	switch ( Language ) {
	case lgg::lEnglish:
		return English;
		break;
	case lgg::lFrench:
		return French;
		break;
	case lgg::lGerman:
		return German;
		break;
	default:
		ERRc();
		return *(const messages_ *)NULL;	// Only to avoid a warning.
		break;
	}
}

const char *msg::i18_messages_::_GetMessage(
	int MessageId,
	lgg::language__ Language,
	buffer__ &Buffer ) const
{
	ctn::E_CMITEM( message_ ) Message;

	Buffer.Data[0] = 0;

	if ( _GetMessages( Language ).Amount() > (epeios::size__)MessageId ) {
		Message.Init( _GetMessages( Language ) );

#ifdef MSG_DBG
		if ( Message( MessageId ).Amount() >= MSG_MESSAGE_LENGTH_MAX )
			ERRl();
#endif

		Message().Recall( 0, Message().Amount(), Buffer.Data );

		Buffer.Data[Message().Amount()]=0;
	}

	return Buffer.Data;
}

const char *msg::i18_messages_::GetMessage(
	int MessageId,
	lgg::language__ Language,
	buffer__ &Buffer ) const
{
	const char *Message = NULL;

	if ( Language == lgg::lTest )
		Message = _GetBaseMessage( MessageId );
	else {
		Message = _GetMessage( MessageId, Language, Buffer );

		if ( Message[0] == 0 )
			Message = _GetMessage( MessageId, lgg::lTest, Buffer );

		if ( Message[0] == 0 )
			Message = _GetBaseMessage( MessageId );
	}

	return Message;
}

void msg::i18_messages_::DumpBaseMessages(
	txf::text_oflow__ &Flow,
	int Amount ) const
{
	for ( int i = 0; i < Amount ; i++ )
		Flow << _GetBaseMessage( i ) << txf::nl;
}



class msgtutor
: public ttr_tutor
{
public:
	msgtutor( void )
	: ttr_tutor( MSG_NAME )
	{
#ifdef MSG_DBG
		Version = MSG_VERSION "\b\bD $";
#else
		Version = MSG_VERSION;
#endif
		Owner = MSG_OWNER;
		Date = "$Date$";
	}
	virtual ~msgtutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace msg;

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class msgpersonnalization
: public msgtutor
{
public:
	msgpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~msgpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the ending of the application  */
	}
};


/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

// 'static' by GNU C++.

static msgpersonnalization Tutor;

ttr_tutor &MSGTutor = Tutor;
