/*
  Header for the 'srv' library by Claude L. Simon (simon@epeios.org)
  Copyright (C) 2000,2001 Claude L. SIMON (simon@epeios.org) 

  This file is part of the Epeios (http://www.epeios.org/) project.
  

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

#ifndef SRV__INC
#define SRV__INC

#define SRV_NAME		"SRV"

#define	SRV_VERSION	"$Revision$"	

#define SRV_OWNER		"the Epeios project (http://www.epeios.org/)"

#include "ttr.h"

extern class ttr_tutor &SRVTutor;

#if defined( XXX_DBG ) && !defined( SRV_NODBG )
#define SRV_DBG 
#endif

/* Begin of automatic documentation generation part. */

//V $Revision$
//C Claude L. SIMON (simon@epeios.org)
//R $Date$

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#include "err.h"
#include "sck.h"

namespace srv {
	using namespace sck;

	//t The type of a service.
	typedef bso__ushort	service__;

	//c User functions with socket.
	class socket_functions__ {
	protected:
		virtual void SRVProcess( sck::socket__ Socket ) = 0;
	public:
		void Process( sck::socket__ Socket )
		{
			SRVProcess( Socket );
		}
	};

	//c User functions with flow handler.
	class flow_functions__ {
	protected:
		virtual void SRVProcess( flw::ioflow___ &Flow ) = 0;
	public:
		void Process( flw::ioflow___ &Flow )
		{
			SRVProcess( Flow );
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
		void Init(
			service__ Service,
			int Nombre = 5 );
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
			socket_functions__ &Functions,
			err::handle ErrHandle = err::hUsual )
		{
			Functions.Process( Interroger_( ErrHandle ) );
		}
	};

#ifdef CPE__MT
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
		void Init(
			service__ Service,
			int Nombre = 5 )
		{
			listener___::Init( Service, Nombre );
		}
		/*f Handle each new connection using 'Functions'. */
		void Process(
			socket_functions__ &Functions,
			err::handle ErrHandle = err::hUsual );
		/*f Handle each new connection using 'Functions'. */
		void Process(
			flow_functions__ &Functions,
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
