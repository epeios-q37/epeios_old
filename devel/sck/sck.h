/*
	Header for the 'sck' library by Claude SIMON (csimon@epeios.org)
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

#ifndef SCK__INC
#define SCK__INC

#define SCK_NAME		"SCK"

#define	SCK_VERSION	"$Revision$"

#define SCK_OWNER		"Claude SIMON (csimon@epeios.org)"

#include "ttr.h"

extern class ttr_tutor &SCKTutor;

#if defined( XXX_DBG ) && !defined( SCK_NODBG )
#define SCK_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision$
//C Claude SIMON (csimon@epeios.org)
//R $Date$

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D SoCKet 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "cpe.h"
#include "flw.h"

#if defined( CPE__UNIX ) || defined( CPE__BEOS )
#	define SCK__UNIX_LIKE
#endif

#ifdef CPE__MS
#	include <winsock.h>
//d Invalid value for a socket descriptor ( 'socket__' ).
#	define SCK_INVALID_SOCKET		INVALID_SOCKET
#	define SCK_SOCKET_ERROR			SOCKET_ERROR
#	define SCK_ECONNRESET			WSAECONNRESET
#	define SCK_EWOULDBLOCK			WSAEWOULDBLOCK
#	define SCK_EINTR				WSAEINTR
#	define SCK_ENOTSOCK				WSAENOTSOCK
#elif defined( SCK__UNIX_LIKE )
#	include <sys/time.h>
#	include <sys/types.h>
#	include <sys/socket.h>
#	include <unistd.h>
#	include <netinet/in.h>
#	include <sys/ioctl.h>
#	include <errno.h>
#	define SCK_INVALID_SOCKET		-1
#	define SCK_SOCKET_ERROR			-1
#	define SCK_ECONNRESET			ECONNRESET
#	define SCK_EWOULDBLOCK			EWOULDBLOCK
#	define SCK_EINTR				EINTR
#	define SCK_ENOTSOCK				ENOTSOCK
#else
#	error "Unknow compiler enviroment"
#endif

//d Value to give to the 'TimeOut' parameter to indicate waiting forever.
#define SCK_INFINITE	BSO_USHORT_MAX

//d Returned value to indicate that the connection no longer exists.
#define SCK_DISCONNECTED	-1

#ifndef SCK_SOCKET_FLOW_BUFFER_SIZE
//d The size of the buffers used by the flows.
#	define SCK_SOCKET_FLOW_BUFFER_SIZE	500
#endif

#ifdef SCK_DEFAULT_TIMEOUT
#	define SCK__DEFAULT_TIMEOUT	SCK_DEFAULT_TIMEOUT
#else
#	ifdef XXX_DBG
#		define SCK__DEFAULT_TIMEOUT	SCK_INFINITE
#	else
//d Default timeout, in seconds.
#		define SCK__DEFAULT_TIMEOUT	( 30 * 60 )	// half an hour.
#	endif
#endif

//d Max data amount of concurrent write and read.
#define SCK__DEFAULT_AMOUNT	( 1024 * 1024 )

namespace sck {
	using flw::datum__;
#ifdef CPE__MS
	typedef SOCKET socket__;
	typedef char *	cast__;
#elif defined( SCK__UNIX_LIKE )
	typedef int	socket__;
	typedef void * cast__;
#else
#	error "Unknow compiler enviroment"
#endif

	typedef int error__;

	typedef bso::ushort__	duration__;

	/* NOTA: Les deux déclarations ci-dessous ont été mise en place pour simplifier
	l'usage des sockets sous Windows. En effet, ce dernier, et lui seul, ne réalise
	pas automatiquement l'initialisation de la couche réseau. Cette librairie,
	cependant, propose une fonction permettant d'assurer un comportement identique
	sur un maximum de sytème. Cette fonction n'est pas utilisé par cette librairie,
	mais une utilisation pertinente de cette fonction, notamment par les librairies
	gèrant respectivement les fonctionnalités de serveur et de client, permettront
	à l'utilisateur de faire abstraction de la contrainte imposée par Windows. */

	// Vrai si l'initialisation de la couche réseau a été faite, faux sinon.
	extern bool Ready_;

	//f Initialize TCP/IP layer.
	inline void Initialize( void )
	{
		if ( !Ready_ )
		{
	#ifdef CPE__MS
			WORD wVersionRequested;
			WSADATA wsaData;

			wVersionRequested = MAKEWORD( 1, 1);

			if ( WSAStartup( wVersionRequested, &wsaData ) )
				ERRd();
	#elif defined( SCK__UNIX_LIKE )
			Ready_ = true;
	#else
	#	error "Unknow compiler enviroment"
	#endif

		}
	}

	//f Create a socket. Only used in some particular multitasking program.
	inline socket__ CreateSocket( err::handle ErrHandle = err::hUsual )
	{
	#ifdef CPE__BEOS
		socket__ Desc = socket( AF_INET, SOCK_STREAM, 0 );
	#else
		socket__ Desc = socket( PF_INET, SOCK_STREAM, 0 );
	#endif
		if ( ( Desc == SCK_INVALID_SOCKET ) && ( ErrHandle == err::hUsual ) )
			ERRs();

		return Desc;
	}

	//f 'Error' becomes the error value returned by 'SCKError()'.
	inline void Error( error__ Error )
	{
	#ifdef CPE__MS
		WSASetLastError( Error );
	#elif defined( SCK__UNIX_LIKE )
		errno = Error;
	#else
	#	error "Unknow compiler enviroment"
	#endif
	}

	//f Return the last error.
	inline error__ Error( void )
	{
	#ifdef CPE__MS
		return WSAGetLastError();
	#elif defined( SCK__UNIX_LIKE )
		return errno;
	#else
	#	error "Unknow compiler enviroment"
	#endif
	}

	#ifndef CPE__BEOS
	/*f The socket 'Socket' becomes blocking or not, depend on the value of 'Value'.
	Not currently available under Be OS. */
	inline void Blocking(
		socket__ Socket,
		bso::bool__ Value )
	{
		unsigned long *V;

		if ( Value )
			V = (unsigned long *)"\x0\x0\x0\x0";
		else
			V = (unsigned long *)"1111";

	#	ifdef CPE__MS
		ioctlsocket( Socket, FIONBIO, V );
	#	elif defined( SCK__UNIX_LIKE )
		ioctl( Socket, FIONBIO, V );
	#	else
	#		error "Unknow compiler enviroment"
	#	endif
	}
	#endif

	/*f Put in 'Buffer' up to 'Amount' bytes for the 'Socket' socket. Return
	the amount effectively red. If 0 is returned, it means that the 'Timeout'
	expired. If the connection no longer exists, then 'SCK_DISCONNECTED' is
	returned. */
	flw::size__ Read(
		socket__ Socket,
		flw::size__ Amount,
		void *Buffer,
		duration__ TimeOut = SCK_INFINITE );

	/*f Write up to 'Amount' bytes from 'Buffer' to the socket 'Socket'. Return
	the amount effectively written. If 0 is returned, it means 'TimeOut' expired.
	If connection no longer exists, then 'SCK_DISCONNECTED' is returned. */
	flw::size__ Write(
		socket__ Socket,
		const void *Buffer,
		flw::size__ Amount,
		duration__ TimeOut = SCK_INFINITE );

	//f Close the socket 'Socket'.
	inline void Close( socket__ Socket )
	{
	#ifdef CPE__MS
	//	shutdown( Socket, 2 );
		if ( closesocket( Socket ) == SCK_SOCKET_ERROR )
			ERRd();
	#elif defined( SCK__UNIX_LIKE )
	//	shutdown( Socket, 2 );
		if ( close( Socket ) == SCK_SOCKET_ERROR )
			ERRd();
	#else
	#	error "Unknow compiler enviroment"
	#endif
	}

	//c Socket as input/output flow driver.
	class socket_ioflow___
	: public flw::ioflow__
	{
	private:
		socket__ Socket_;
		duration__ TimeOut_;
		bso::bool__ Error_;
		flw::datum__ Cache_[2 * SCK_SOCKET_FLOW_BUFFER_SIZE];
	protected:
		virtual flw::size__ FLWRead(
			flw::size__ Minimum,
			flw::datum__ *Buffer,
			flw::size__ Wanted );
		virtual flw::size__ FLWWrite(
			const flw::datum__ *Buffer,
			flw::size__ Wanted,
			flw::size__ Minimum,
			bool Synchronization );
	public:
		void reset( bool P = true )
		{
			if ( P ) {
				if ( Socket_ != SCK_INVALID_SOCKET ) {
					ioflow__::Synchronize();
					Close( Socket_ );
				}
			}
								
			Socket_ = SCK_INVALID_SOCKET;
			TimeOut_ = SCK_INFINITE;
			Error_ = false;
		}
		socket_ioflow___( void )
		: ioflow__( Cache_, sizeof( Cache_ ), FLW_AMOUNT_MAX )
		{
			reset( false );
		}
		virtual ~socket_ioflow___( void )
		{
			reset( true );
		}
		//f Initialization with socket 'Socket' and 'TimeOut' as timeout.
		void Init(
			socket__ Socket,
			flw::amount__ AmountMax,
			duration__ TimeOut )
		{
			reset();
		
			ioflow__::SetAmountMax( AmountMax );

			Socket_ = Socket;
			TimeOut_ = TimeOut;
		}
		//f Initialization with socket 'Socket' and 'TimeOut' as timeout.
		void Init(
			socket__ Socket,
			duration__ TimeOut = SCK__DEFAULT_TIMEOUT )
		{
			Init( Socket, SCK__DEFAULT_AMOUNT, TimeOut );
		}
	};

}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
