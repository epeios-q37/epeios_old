/*
	'sck' library by Claude SIMON (csimon@epeios.org)
	Requires the 'sck' header file ('sck.h').
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

#ifdef CPE__UNIX
#	include <signal.h>
#endif

using namespace sck;

#ifdef CPE__MS
bool sck::Ready_ = false;
#elif defined( SCK__UNIX_LIKE )
bool sck::Ready_ = true;
#else
#	error "Unknow compiler enviroment"
#endif

flw::bsize__ sck::Read(
	socket__ Socket,
	flw::bsize__ Amount,
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
				ERRd();
		}
		else if ( !Result && Amount )
			Result = SCK_DISCONNECTED;
	}
	else if ( Result == SCK_SOCKET_ERROR )
		ERRd();
	else if ( Result == 0 )
		ERRx();
	else
		ERRs();

	return Result;
}

flw::bsize__ sck::Write(
	socket__ Socket,
	const void *Buffer,
	flw::bsize__ Amount,
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
				ERRd();
		}
		else if ( !Result && Amount )	// I assume that, because the send is call immediatly after the select, this can not be happen.
			ERRc();
	}
	else if ( Result == SCK_SOCKET_ERROR )
		ERRd();
	else if ( Result == 0 )
		ERRx();
	else
		ERRs();

	return Result;
}


flw::bsize__ sck::socket_ioflow___::FLWRead(
	flw::bsize__ Minimum,
	flw::datum__ *Buffer,
	flw::bsize__ Wanted )
{
	flw::bsize__ Amount = 0;

	if ( Minimum != 0 )
	{
		flw::bsize__ Result;
		
		while( Minimum > Amount ) {
			if ( ( Result = sck::Read( _Socket, ( Wanted - Amount ), Buffer + Amount, _TimeOut ) ) == SCK_DISCONNECTED )
				if ( ( Result = iflow__::HandleEOFD( Buffer, ( Wanted - Amount ) ) ) == 0 ) {
					_Socket = SCK_INVALID_SOCKET;
					_Error = true;
					ERRd();
				}
			Amount += Result;
		}
	}
	else
		Amount = sck::Read( _Socket, Wanted, Buffer, 0 );

	if ( Amount == SCK_DISCONNECTED )
		if ( ( Amount = iflow__::HandleEOFD( Buffer, Wanted ) ) == 0 ) {
				_Socket = SCK_INVALID_SOCKET;
				_Error = true;
				ERRd();
			}

		return Amount;
}


flw::bsize__ sck::socket_ioflow___::FLWWrite(
	const flw::datum__ *Buffer,
	flw::bsize__ Wanted,
	flw::bsize__ Minimum,
	bool Synchronization )
{
	flw::bsize__ Amount = 0;

	if ( _Error )
		ERRd();

	if ( Minimum != 0 )
	{
		flw::bsize__ Result;
		
		while( Minimum > Amount ) {
			if ( ( Result = sck::Write( _Socket, Buffer + Amount, Wanted - Amount, _TimeOut ) ) == SCK_DISCONNECTED ) {
				_Socket = SCK_INVALID_SOCKET;
				_Error = true;
				ERRd();
			}
			Amount += Result;
		}
	}
	else
		Amount = sck::Write( _Socket, Buffer, Wanted, 0 );

	if ( Amount == SCK_DISCONNECTED ) {
		_Socket = SCK_INVALID_SOCKET;
		_Error = true;
		ERRd();
	}

	return Amount;
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
#ifdef CPE__UNIX
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
