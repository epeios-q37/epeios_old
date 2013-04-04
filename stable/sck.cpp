/*
	'sck' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Requires the 'sck' header file ('sck.h').
	Copyright (C) 2000-2004 Claude SIMON (http://zeusw.org/intl/contact.html).

	This file is part of the Epeios (http://zeusw.org/epeios/) project.

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 3
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

#define SCK__COMPILATION

#include "sck.h"

class scktutor
: public ttr_tutor
{
public:
	scktutor( void )
	: ttr_tutor( SCK_NAME )
	{
#ifdef SCK_DBG
		Version = SCK_VERSION "\b\bD $";
#else
		Version = SCK_VERSION;
#endif
		Owner = SCK_OWNER;
		Date = "$Date$";
	}
	virtual ~scktutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#ifdef CPE_LINUX
#	define SCK__IGNORE_SIGPIPE
#endif


#ifdef SCK__IGNORE_SIGPIPE
#	include <signal.h>
#endif

using namespace sck;

#ifdef CPE_WIN
bool sck::Ready_ = false;
#else
bool sck::Ready_ = true;
#endif

flw::size__ sck::Read(
	socket__ Socket,
	flw::size__ Amount,
	void *Buffer,
	duration__ TimeOut )
{
	fd_set fds;
	int Result;

	FD_ZERO( &fds );
	FD_SET( Socket, &fds );

	if ( TimeOut == SCK_INFINITE )
		Result = select( (int)( Socket + 1 ), &fds, NULL, NULL, NULL );
	else
	{
		timeval TV;

		TV.tv_sec = TimeOut;
		TV.tv_usec = 0;

		Result = select( (int)( Socket + 1 ), &fds, NULL, NULL, &TV );
	}

	if ( Result == 1 )
	{
		Result = recv( Socket, (cast__)Buffer, (int)Amount, 0 );

		if ( Result == SCK_SOCKET_ERROR )
		{
			Result = SCK_DISCONNECTED;

			if ( Error() != SCK_ECONNRESET )
				ERRDvc();
		}
		else if ( !Result && Amount )
			Result = SCK_DISCONNECTED;
	}
	else if ( Result == SCK_SOCKET_ERROR )
		ERRDvc();
	else if ( Result == 0 )
		ERRExt();
	else
		ERRSys();

	return Result;
}

flw::size__ sck::Write(
	socket__ Socket,
	const void *Buffer,
	flw::size__ Amount,
	duration__ TimeOut )
{
	fd_set fds;
	int Result;

	FD_ZERO( &fds );
	FD_SET( Socket, &fds );

	if ( TimeOut == SCK_INFINITE )
		Result = select( (int)( Socket + 1 ), NULL, &fds, NULL, NULL );
	else
	{
		timeval TV;

		TV.tv_sec = TimeOut;
		TV.tv_usec = 0;

		Result = select( (int)( Socket + 1 ), NULL, &fds, NULL, &TV );
	}

	if ( Result == 1 )
	{
		Result = send( Socket, (const cast__)Buffer, (int)Amount, 0 );

		if ( Result == SCK_SOCKET_ERROR )
		{
			Result = SCK_DISCONNECTED;

			if ( Error() != SCK_ECONNRESET )
				ERRDvc();
		}
		else if ( !Result && Amount )	// I assume that, because the send is call immediatly after the select, this can not be happen.
			ERRCcp();
	}
	else if ( Result == SCK_SOCKET_ERROR )
		ERRDvc();
	else if ( Result == 0 )
		ERRExt();
	else
		ERRSys();

	return Result;
}


/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */
class sckpersonnalization
: public scktutor
{
public:
	sckpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
#ifdef SCK__IGNORE_SIGPIPE
	signal( SIGPIPE, SIG_IGN );
#endif
	}
	~sckpersonnalization( void )
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

static sckpersonnalization Tutor;

ttr_tutor &SCKTutor = Tutor;
