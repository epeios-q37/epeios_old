/*
	Header for the 'sck' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Copyright (C) $COPYRIGHT_DATES$Claude SIMON (http://zeusw.org/intl/contact.html).
$_RAW_$
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

#ifndef SCK__INC
#define SCK__INC

#define SCK_NAME		"SCK"

#define	SCK_VERSION	"$Revision$"

#define SCK_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"

#include "ttr.h"

extern class ttr_tutor &SCKTutor;

#if defined( XXX_DBG ) && !defined( SCK_NODBG )
#define SCK_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision$
//C Claude SIMON (http://zeusw.org/intl/contact.html)
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
#include "fwf.h"
#include "flw.h"
#include "tol.h"

#if defined( CPE__T_LINUX ) || defined( CPE__P_CYGWIN ) || defined( CPE__T_MAC )
#	define SCK__POSIX
#elif defined( CPE__T_MS )
#	define SCK__MS
#else
#	error "Uknown target !"
#endif

#ifdef SCK__MS
#	include <winsock.h>
#	define SCK_INVALID_SOCKET		INVALID_SOCKET
#	define SCK_SOCKET_ERROR			SOCKET_ERROR
#	define SCK_ECONNRESET			WSAECONNRESET
#	define SCK_EWOULDBLOCK			WSAEWOULDBLOCK
#	define SCK_EAGAIN				WSAEWOULDBLOCK	// Pas de 'WSAEAGAIN', mais mettre ça simplifie certaines lignes.
#	define SCK_EINTR				WSAEINTR
#	define SCK_ENOTSOCK				WSAENOTSOCK
#elif defined( SCK__POSIX )
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
#	define SCK_EAGAIN				EAGAIN
#	define SCK_EINTR				EINTR
#	define SCK_ENOTSOCK				ENOTSOCK
#else
#	error
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
#	define SCK__DEFAULT_TIMEOUT	SCK_INFINITE
#endif

//d Max data amount of concurrent write and read.
#define SCK__DEFAULT_AMOUNT	( 1024 * 1024 )

namespace sck {
	using flw::datum__;
#ifdef SCK__MS
	typedef SOCKET socket__;
	typedef char *	cast__;
#elif defined( SCK__POSIX )
	typedef int	socket__;
	typedef void * cast__;
#else
#	error
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
#ifdef SCK__MS
			WORD wVersionRequested;
			WSADATA wsaData;

			wVersionRequested = MAKEWORD( 1, 1);

			if ( WSAStartup( wVersionRequested, &wsaData ) )
				ERRd();
#elif defined( SCK__POSIX )
			Ready_ = true;
#else
#	error
#endif

		}
	}

	//f Create a socket. Only used in some particular multitasking program.
	inline socket__ CreateSocket( err::handling__ ErrorHandling = err::h_Default )
	{
	#ifdef CPE__T_BEOS
		socket__ Desc = socket( AF_INET, SOCK_STREAM, 0 );
	#else
		socket__ Desc = socket( PF_INET, SOCK_STREAM, 0 );
	#endif
		if ( ( Desc == SCK_INVALID_SOCKET ) && ( ErrorHandling == err::hThrowException ) )
			ERRs();

		return Desc;
	}

	//f 'Error' becomes the error value returned by 'SCKError()'.
	inline void Error( error__ Error )
	{
#ifdef SCK__MS
		WSASetLastError( Error );
#elif defined( SCK__POSIX )
		errno = Error;
#else
#	error
#endif
	}

	//f Return the last error.
	inline error__ Error( void )
	{
#ifdef SCK__MS
		return WSAGetLastError();
#elif defined( SCK__POSIX )
		return errno;
#else
#	error
#endif
	}

	inline const char *ErrorDescription( error__ Error )
	{
#ifdef SCK__MS
		return ("Not implemented" );
#elif defined( SCK__POSIX )
		return strerror( Error );
#else
#	error
#endif
	}


#ifndef CPE__T_BEOS
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

#	ifdef SCK__MS
		ioctlsocket( Socket, FIONBIO, V );
#	elif defined( SCK__POSIX )
		ioctl( Socket, FIONBIO, V );
#	else
#		error
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
		duration__ TimeOut = SCK_INFINITE );	// En secondes.

	/*f Write up to 'Amount' bytes from 'Buffer' to the socket 'Socket'. Return
	the amount effectively written. If 0 is returned, it means 'TimeOut' expired.
	If connection no longer exists, then 'SCK_DISCONNECTED' is returned. */
	flw::size__ Write(
		socket__ Socket,
		const void *Buffer,
		flw::size__ Amount,
		duration__ TimeOut = SCK_INFINITE );	// En secondes.

	//f Close the socket 'Socket'.
	inline void Close( socket__ Socket )
	{
#ifdef SCK__MS
	//	shutdown( Socket, 2 );
		if ( closesocket( Socket ) == SCK_SOCKET_ERROR )
			ERRd();
#elif defined( SCK__POSIX )
	//	shutdown( Socket, 2 );
		if ( close( Socket ) == SCK_SOCKET_ERROR )
			ERRd();
#else
#	error
#endif
	}

	//c Socket as input/output flow driver.
	class socket_ioflow_functions___
	: public fwf::ioflow_functions___
	{
	private:
		socket__ _Socket;
		duration__ _TimeOut;	// En secondes.
		bso::bool__ _Error;
	protected:
		virtual fwf::size__ FWFRead(
			fwf::size__ Minimum,
			fwf::datum__ *Buffer,
			fwf::size__ Wanted );
		virtual fwf::size__ FWFWrite(
			const fwf::datum__ *Buffer,
			fwf::size__ Wanted,
			fwf::size__ Minimum );
		virtual void FWFDismiss( void )
		{}
		virtual void FWFSynchronize( void )
		{}
	public:
		void reset( bool P = true )
		{
			if ( P ) {
				if ( _Socket != SCK_INVALID_SOCKET ) {
					ioflow_functions___::Synchronize();
					Close( _Socket );
				}
			}

			ioflow_functions___::reset( P );
								
			_Socket = SCK_INVALID_SOCKET;
			_TimeOut = SCK_INFINITE;
			_Error = false;
		}
		socket_ioflow_functions___( void )
		{
			reset( false );
		}
		virtual ~socket_ioflow_functions___( void )
		{
			reset();
		}
		//f Initialization with socket 'Socket' and 'TimeOut' as timeout.
		void Init(
			socket__ Socket,
			duration__ TimeOut = SCK__DEFAULT_TIMEOUT )	// En secondes.
		{
			reset();
		
			ioflow_functions___::Init();

			_Socket = Socket;
			_TimeOut = TimeOut;
		}
		E_RODISCLOSE__( socket__, Socket )
	};

	//c Socket as input/output flow driver.
	class unsafe_socket_ioflow___
	: public flw::ioflow__
	{
	private:
		socket_ioflow_functions___ _Functions;
		flw::datum__ _Cache[2 * SCK_SOCKET_FLOW_BUFFER_SIZE];
	public:
		void reset( bool P = true )
		{
			ioflow__::reset( P );
			_Functions.reset( P );
		}
		unsafe_socket_ioflow___( flw::size__ AmountMax = SCK__DEFAULT_AMOUNT )
		: ioflow__( _Functions, _Cache, sizeof( _Cache ), AmountMax )
		{
			reset( false );
		}
		virtual ~unsafe_socket_ioflow___( void )
		{
			reset();
		}
		//f Initialization with socket 'Socket' and 'TimeOut' as timeout.
		void Init(
			socket__ Socket,
			duration__ TimeOut = SCK__DEFAULT_TIMEOUT ) // En secondes.
		{
			reset();

			_Functions.Init( Socket, TimeOut );
		}
		socket__ Socket( void ) const
		{
			return _Functions.Socket();
		}
	};

}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
