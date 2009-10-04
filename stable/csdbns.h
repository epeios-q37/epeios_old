/*
	Header for the 'csdbns' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Copyright (C) 2004 Claude SIMON (http://zeusw.org/intl/contact.html).

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

#ifndef CSDBNS__INC
#define CSDBNS__INC

#define CSDBNS_NAME		"CSDBNS"

#define	CSDBNS_VERSION	"$Revision$"

#define CSDBNS_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"

#include "ttr.h"

extern class ttr_tutor &CSDBNSTutor;

#if defined( XXX_DBG ) && !defined( CSDBNS_NODBG )
#define CSDBNS_DBG
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
		socket_user_functions__ *_UserFunctions;
		void *_UP;
		bso::bool__ _UserFunctionsCalled( void ) const	// Retourne 'true', si une fonction utilisateurs a été appelée.
		{
			return _UserFunctions != NULL;
		}
		socket__ _Interroger(
			err::handle ErrHandle,
			sck::duration__ TimeOut );
	public:
		void reset( bool P = true )
		{
			if ( P )
			{
				if ( Socket_ != SCK_INVALID_SOCKET )
					Close( Socket_ );

				if ( _UserFunctionsCalled() )
					_UserFunctions->PostProcess( _UP );	// Même si 'UP' != NULL;
			}

			Socket_ = SCK_INVALID_SOCKET;
			_UserFunctions = NULL;
			_UP = NULL;
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
		//f Return the first available connection. BLOCKING FUNCTION if 'TimeOut == 'SCK_INFINITE'.
		socket__ GetConnection(
			err::handle ErrHandle = err::hUsual,
			sck::duration__ TimeOut = SCK_INFINITE )
		{
			return _Interroger( ErrHandle, TimeOut );
		}
		//f Initialize 'Socket' with the first connection available. BLOCKING FUNCTION if 'TimeOut' == 'SCK_INFINITE'.
		void GetConnection(
			socket__ &Socket,
			err::handle ErrHandle = err::hUsual,
			sck::duration__ TimeOut = 0 )
		{
			Socket = _Interroger( ErrHandle, TimeOut );
		}
		// If returned value = 'true', then exiting is because 'TimeOut' reached.
		// If returned value == 'false', then underlying user function retuns 'bStop'.
		bso::bool__ Process(
			socket_user_functions__ &Functions,
			err::handle ErrHandle = err::hUsual,
			sck::duration__ TimeOut = SCK_INFINITE );
	};

#ifdef CPE__T_MT

	struct _flow_data__ {
		sck::unsafe_socket_ioflow___ Flow;
		void *UP;
	};

	class _functions__
	: public socket_user_functions__
	{
	protected:
		virtual void *CSDNBSPreProcess( socket__ Socket )
		{
			_flow_data__ *Data = NULL;
		ERRProlog
		ERRBegin
			Data = new _flow_data__;

			if ( Data == NULL )
				ERRa();

			Data->Flow.Init( Socket );
			Data->UP = UserFunctions->PreProcess( Data->Flow );
		ERRErr
			if ( Data != NULL )
				delete Data;
			Data = NULL;
		ERREnd
		ERREpilog
			return Data;
		}
		virtual action__ CSDNBSProcess(
			socket__ Socket,
			void *UP )
		{
			_flow_data__ &Data = *(_flow_data__ *)UP;
#ifdef CSDNBS_DBG
			if ( Data.Flow.GetSocket() != Socket )
				ERRc();
#endif

			return UserFunctions->Process( Data.Flow, Data.UP );
		}
		virtual void CSDNBSPostProcess( void *UP )
		{
			if ( UP == NULL )
				ERRc();

			UserFunctions->PostProcess( ((_flow_data__ *)UP)->UP );

			delete (_flow_data__ *)UP;
		}
	public:
		user_functions__ *UserFunctions;
	};

	/*c Handling a server, with process duplication for each client. */
	class server___
	: public listener___
	{
	private:
		socket_user_functions__ *_SocketFunctions;
		_functions__ _Functions;
	public:
		void reset( bool P = true )
		{
			listener___::reset( P );
		}
		server___( void )
		{
			reset( false );
			_SocketFunctions = NULL;
		}
		~server___( void )
		{
			reset( true );
		}
		/*f Initialzation with 'Service' as port to listen.
		A maximum of 'Amount' are accepted in the waiting queue. */
		bso::bool__ Init(
			service__ Service,
			socket_user_functions__ &SocketFunctions,
			int Amount,
			err::handle ErrHandle = err::hUsual )
		{
			_SocketFunctions = &SocketFunctions;
			
			return listener___::Init( Service, Amount, ErrHandle );
		}
		/*f Initialzation with 'Service' as port to listen.
		A maximum of 'Amount' are accepted in the waiting queue. */
		bso::bool__ Init(
			service__ Service,
			socket_user_functions__ &SocketFunctions,
			err::handle ErrHandle = err::hUsual,
			int Amount = 5 )
		{
			_SocketFunctions = &SocketFunctions;

			return listener___::Init( Service, Amount, ErrHandle );
		}
		bso::bool__ Init(
			service__ Service,
			user_functions__ &UserFunctions,
			err::handle ErrHandle = err::hUsual,
			int Amount = 5 )
		{
			_Functions.UserFunctions = &UserFunctions;

			return Init( Service, this->_Functions, Amount, ErrHandle );
		}
		bso::bool__ Init(
			service__ Service,
			user_functions__ &UserFunctions,
			int Amount,
			err::handle ErrHandle = err::hUsual )
		{
			_Functions.UserFunctions = &UserFunctions;

			return Init( Service, this->_Functions, Amount, ErrHandle );
		}
		/*f Handle each new connection using 'Functions'. */
		void Process(
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
