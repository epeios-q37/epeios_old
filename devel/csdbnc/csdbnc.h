/*
	Header for the 'csdbnc' library by Claude SIMON (csimon@epeios.org)
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

#ifndef CSDBNC__INC
#define CSDBNC__INC

#define CSDBNC_NAME		"CSDBNC"

#define	CSDBNC_VERSION	"$Revision$"

#define CSDBNC_OWNER		"Claude SIMON (csimon@epeios.org)"

#include "ttr.h"

extern class ttr_tutor &CSDBNCTutor;

#if defined( XXX_DBG ) && !defined( CSDBNC_NODBG )
#define CSDBNC_DBG
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
//D Client-Server Base Network Client 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "sck.h"

#if defined( CPE__UNIX ) || defined( CPE__BEOS )
#	define CSDBNC__UNIX_LIKE
#endif

#if defined( CSDBNC__UNIX_LIKE )
#	include <sys/socket.h>
#	include <netinet/in.h>
#	include <arpa/inet.h>
#	include <netdb.h>
#elif !defined( CPE__MS )
#	error "Unknow compiler enviroment"
#endif

#define CSDBNC_ADDRESS_SIZE_MAX	100

namespace csdbnc {
	using namespace sck;

	typedef bso::char__	buffer__[CSDBNC_ADDRESS_SIZE_MAX+1];

	//f Return the host name contained in 'HostService'.
	const char *Host(
		const char *HostService,
		buffer__ Buffer );

#ifndef CPE__MT
	inline const char *Host( const char *HostService )
	{
		static buffer__ Buffer;

		return Host( HostService, Buffer );
	}
#endif

	//f Return the Service contained in 'HostService'.
	inline const char *Service( const char *HostService )
	{
		const char *P;

		P = strchr( HostService, ':' );

		if ( P == NULL )
			ERRu();

		return P + 1;
	}

	/*f Return a descriptor to a socket connected to 'Host' at 'Service'.
	'Host' can be in '212.95.72.3' format, or in 'www.epeios.org' format,
	and 'Service' can be in '80' format of in 'http' format.
	If 'Desc' != 'SCK_INVALID', the socket descriptor 'Desc' is used.
	If connection failed, and 'ErrHandler' == 'err::hSkip', return 'SCK_INVALID'. */
	socket__ Connect(
		const char *Host,
		const char *Service,
		socket__ Desc = SCK_INVALID_SOCKET,
		err::handle ErrHandle = err::hUsual );

	/*f Return a descriptor to a socket connected to 'Host' at 'Service'.
	'Host' can be in '212.95.72.3' format, or in 'www.epeios.org' format,
	and 'Service' can be in '80' format of in 'http' format.
	If connection failed, and 'ErrHandler' == 'err::hSkip', return 'SCK_INVALID'. */
	inline socket__ Connect(
		const char *Host,
		const char *Service,
		err::handle ErrHandle )
	{
		return Connect( Host, Service, SCK_INVALID_SOCKET, ErrHandle );
	}

	/*f Return a descriptor to a socket connected to 'HostService'.
	'HostService' can be in '212.95.72.3:80' format, or in 'www.ensd.net:http' format.
	If 'Desc' != 'SCK_INVALID', the socket descriptor 'Desc' is used.
	If connection failed, and 'ErrHandler' == 'err::hSkip', return 'SCK_INVALID'. */
	inline socket__ Connect(
		const char *HostService,
		socket__ Desc = SCK_INVALID_SOCKET,
		err::handle ErrHandle = err::hUsual )
	{
		buffer__ Buffer;

		return Connect( Host( HostService, Buffer ), Service( HostService ), Desc, ErrHandle );
	} 

	/*f Return a descriptot to a socket connected to 'HostService'.
	'HostService' can be in '212.95.72.3:80' format, or in 'www.ensd.net:http' format.
	If connection failed, and 'ErrHandler' == 'err::hSkip', return 'SCK_INVALID'. */
	inline socket__ Connect(
		const char *HostService,
		err::handle ErrHandle )
	{
		return Connect( HostService, SCK_INVALID_SOCKET, ErrHandle );
	} 
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
