/*
	'sck' library by Claude SIMON (csimon@epeios.org)
	Requires the 'sck' header file ('sck.h').
	Copyright (C) 2000-2004 Claude SIMON (csimon@epeios.org).

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

amount__ sck::Read(
	socket__ Socket,
	amount__ Amount,
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
	else if ( Result != 0 )
		ERRs();

	return (amount__)Result;
}

amount__ sck::Write(
	socket__ Socket,
	const void *Buffer,
	amount__ Amount,
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
	else if ( Result != 0 )
		ERRs();

	return (amount__)Result;
}


flw::amount__ sck::socket_ioflow___::FLWGet(
	flw::amount__ Minimum,
	flw::datum__ *Buffer,
	flw::amount__ Wanted )
{
	flw::amount__ Amount = 0;

	if ( Minimum != 0 )
	{
		amount__ Result;
		
		while( Minimum > Amount ) {
			if ( ( Result = Read( Socket_, (amount__)( Wanted - Amount ), Buffer + Amount, TimeOut_ ) ) == SCK_DISCONNECTED )
				if ( ( Result = (amount__)iflow__::HandleEOFD( Buffer, (flw::size__)( Wanted - Amount ) ) ) == 0 ) {
					Socket_ = SCK_INVALID_SOCKET;
					Error_ = true;
					ERRd();
				}
			Amount += Result;
		}
	}
	else
		Amount = Read( Socket_, Wanted, Buffer, 0 );

	if ( Amount == SCK_DISCONNECTED )
		if ( ( Amount = iflow__::HandleEOFD( Buffer, Wanted ) ) == 0 ) {
				Socket_ = SCK_INVALID_SOCKET;
				Error_ = true;
				ERRd();
			}

		return Amount;
}


flw::amount__ sck::socket_ioflow___::FLWPut(
	const flw::datum__ *Buffer,
	flw::amount__ Wanted,
	flw::amount__ Minimum,
	bool Synchronization )
{
	flw::amount__ Amount = 0;

	if ( Error_ ) {
#ifdef SCK_DBG
		if ( !Synchronization )
			ERRu();
#endif
		return Minimum;
	}

	if ( Minimum != 0 )
	{
		amount__ Result;
		
		while( Minimum > Amount ) {
			if ( ( Result = Write( Socket_, Buffer + Amount, Wanted - Amount, TimeOut_ ) ) == SCK_DISCONNECTED ) {
				Socket_ = SCK_INVALID_SOCKET;
				Error_ = true;
				ERRd();
			}
			Amount += Result;
		}
	}
	else
		Amount = Write( Socket_, Buffer, Wanted, 0 );

	if ( Amount == SCK_DISCONNECTED ) {
		Socket_ = SCK_INVALID_SOCKET;
		Error_ = true;
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
