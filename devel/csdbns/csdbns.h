/*
	Header for the 'csdbns' library by Claude SIMON (csimon@epeios.org)
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

#ifndef CSDBNS__INC
#define CSDBNS__INC

#define CSDBNS_NAME		"CSDBNS"

#define	CSDBNS_VERSION	"$Revision$"

#define CSDBNS_OWNER		"Claude SIMON (csimon@epeios.org)"

#include "ttr.h"

extern class ttr_tutor &CSDBNSTutor;

#if defined( XXX_DBG ) && !defined( CSDBNS_NODBG )
#define CSDBNS_DBG
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
//D Client-Server Base Network Server 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "sck.h"
#include "csdscm.h"

#define CSDNBS__DEFAULT_TIMEOUT	SCK__DEFAULT_TIMEOUT

namespace csdbns {
	using namespace sck;
	using namespace csdscm;

	//t The type of a service.
	typedef bso::ushort__	service__;

	//c User functions with socket.
	class socket_user_functions__ {
	protected:
		virtual void *CSDNBSPreProcess( sck::socket__ Socket ) = 0;
		virtual action__ CSDNBSProcess(
			sck::socket__ Socket,
			void *UP ) = 0;
		virtual void CSDNBSPostProcess( void *UP ) = 0;
	public:
		void *PreProcess( sck::socket__ Socket )
		{
			return CSDNBSPreProcess( Socket );
		}
		action__ Process(
			sck::socket__ Socket,
			void *UP )
		{
			return CSDNBSProcess( Socket, UP );
		}
		void PostProcess( void *UP )
		{
			CSDNBSPostProcess( UP );
		}
	};

	//c To listen at a port..
	class listener___
	{
	private:
		socket__ Socket_;
		/* Retourne une socket sur une connection. FONCTION BLOQUANTE.
		Lorsque 'IgnorerErreur' à vrai, toute les erreurs sont ignorées, ceci pour garantir
		la présence du processus d'écoute. */
		socket__ Interroger_( err::handle ErrHandle );
	public:
		void reset( bool P = true )
		{
			if ( P )
			{
				if ( Socket_ != SCK_INVALID_SOCKET )
					Close( Socket_ );
			}

			Socket_ = SCK_INVALID_SOCKET;
		}
		listener___( void )
		{
			reset( false );
		}
		~listener___( void )
		{
			reset( true );
		}
		/*f Initialzation with 'Service' as port to listen.
		A maximum of 'Amount' are accepted in the waiting queue. */
		bso::bool__ Init(
			service__ Service,
			int Amount,
			err::handle ErrHandle = err::hUsual );
		/*f Initialzation with 'Service' as port to listen.
		A maximum of 'Amount' are accepted in the waiting queue. */
		bso::bool__ Init(
			service__ Service,
			err::handle ErrHandle = err::hUsual,
			int Amount = 5 )
		{
			return Init( Service, Amount, ErrHandle );
		}
		//f Return the first available connection. BLOCKING FUNCTION.
		socket__ GetConnection( err::handle ErrHandle = err::hUsual )
		{
			return Interroger_( ErrHandle );
		}
		//f Initialize 'Socket' with the first connection available. BLOCKING FUNCTION.
		void GetConnection(
			socket__ &Socket,
			err::handle ErrHandle = err::hUsual )
		{
			Socket = Interroger_( ErrHandle );
		}
		//f Handle new connection using 'Functions'. BLOCKING FUNCTION.
		void Process(
			socket_user_functions__ &Functions,
			err::handle ErrHandle = err::hUsual );
	};

#ifdef CPE__T_MT
	/*c Handling a server, with process duplication for each client. */
	class server___
	: public listener___
	{
	public:
		void reset( bool P = true )
		{
			listener___::reset( P );
		}
		server___( void )
		{
			reset( false );
		}
		~server___( void )
		{
			reset( true );
		}
		/*f Initialzation with 'Service' as port to listen.
		A maximum of 'Amount' are accepted in the waiting queue. */
		bso::bool__ Init(
			service__ Service,
			int Amount,
			err::handle ErrHandle = err::hUsual )
		{
			return listener___::Init( Service, Amount, ErrHandle );
		}
		/*f Initialzation with 'Service' as port to listen.
		A maximum of 'Amount' are accepted in the waiting queue. */
		bso::bool__ Init(
			service__ Service,
			err::handle ErrHandle = err::hUsual,
			int Amount = 5 )
		{
			return listener___::Init( Service, Amount, ErrHandle );
		}
		/*f Handle each new connection using 'Functions'. */
		void Process(
			socket_user_functions__ &Functions,
			err::handle ErrHandle = err::hUsual );
		/*f Handle each new connection using 'Functions'. */
		void Process(
			user_functions__ &Functions,
			sck::duration__ TimeOut = CSDNBS__DEFAULT_TIMEOUT,
			err::handle ErrHandle = err::hUsual );
	};

#endif
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
