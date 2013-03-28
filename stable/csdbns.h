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

# include "err.h"
# include "sck.h"
# include "cpe.h"

# include "csdsuf.h"

# ifdef CPE__WIN
#  ifndef CSD_NO_SERVICE_HANDLING
#   define CSD__HANDLE_SERVICE	// Handles the Windows service system.
#  endif
# endif

 #ifdef CSD__HANDLE_SERVICE
#  include "wintol.h"
	typedef	wintol::service__ _service__;
	using wintol::InstallService;
# endif

# define CSDNBS__DEFAULT_TIMEOUT	SCK__DEFAULT_TIMEOUT

namespace csdbns {
	using namespace sck;

	using namespace csdsuf;

	//t The type of a port.
	typedef bso::u16__	port__;

	//c User functions with socket.
	class socket_user_functions__ {
	protected:
		virtual void *CSDBNSPreProcess(
			sck::socket__ Socket,
			const char *IP ) = 0;
		virtual action__ CSDBNSProcess(
			sck::socket__ Socket,
			void *UP ) = 0;
		virtual void CSDBNSPostProcess( void *UP ) = 0;
		virtual void CSDBNSExit( void ) = 0;	// Appel� lorsque l'on quitte l'application, principalement dnas le cadre d'un service Windows.
	public:
		void *PreProcess(
			sck::socket__ Socket,
			const char *IP )
		{
			return CSDBNSPreProcess( Socket, IP );
		}
		action__ Process(
			sck::socket__ Socket,
			void *UP )
		{
			return CSDBNSProcess( Socket, UP );
		}
		void PostProcess( void *UP )
		{
			CSDBNSPostProcess( UP );
		}
		void Exit( void )
		{
			CSDBNSExit();
		}
	};

	//c To listen at a port..
	class listener___
	{
	private:
		socket__ Socket_;
		/* Retourne une socket sur une connection. FONCTION BLOQUANTE.
		Lorsque 'IgnorerErreur' � vrai, toute les erreurs sont ignor�es, ceci pour garantir
		la pr�sence du processus d'�coute. */
		socket_user_functions__ *_UserFunctions;
		void *_UP;
		bso::bool__ _UserFunctionsCalled( void ) const	// Retourne 'true', si une fonction utilisateurs a �t� appel�e.
		{
			return _UserFunctions != NULL;
		}
		socket__ _Interroger(
			err::handling__ ErrorHandling,
			sck::duration__ TimeOut,
			const char *&IP );
	public:
		void reset( bool P = true )
		{
			if ( P )
			{
				if ( Socket_ != SCK_INVALID_SOCKET )
					Close( Socket_ );

				if ( _UserFunctionsCalled() )
					_UserFunctions->PostProcess( _UP );	// M�me si 'UP' != NULL;
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
		/*f Initialzation with 'Port' as port to listen.
		A maximum of 'Amount' are accepted in the waiting queue. */
		bso::bool__ Init(
			port__ Port,
			int Amount,
			err::handling__ ErrorHandling = err::h_Default );
		/*f Initialzation with 'Port' as port to listen.
		A maximum of 'Amount' are accepted in the waiting queue. */
		bso::bool__ Init(
			port__ Port,
			err::handling__ ErrorHandling = err::h_Default,
			int Amount = 5 )
		{
			return Init( Port, Amount, ErrorHandling );
		}
		//f Return the first available connection. BLOCKING FUNCTION if 'TimeOut == 'SCK_INFINITE'.
		socket__ GetConnection(
			const char *&IP,
			err::handling__ ErrorHandling = err::h_Default,
			sck::duration__ TimeOut = SCK_INFINITE )
		{
			return _Interroger( ErrorHandling, TimeOut, IP );
		}
		//f Initialize 'Socket' with the first connection available. BLOCKING FUNCTION if 'TimeOut' == 'SCK_INFINITE'.
		void GetConnection(
			socket__ &Socket,
			const char *&IP,
			err::handling__ ErrorHandling = err::h_Default,
			sck::duration__ TimeOut = 0 )
		{
			Socket = _Interroger( ErrorHandling, TimeOut, IP );
		}
		// If returned value = 'true', then exiting is because 'TimeOut' reached.
		// If returned value == 'false', then underlying user function retuns 'bStop'.
		bso::bool__ Process(
			socket_user_functions__ &Functions,
			err::handling__ ErrorHandling = err::h_Default,
			sck::duration__ TimeOut = SCK_INFINITE );
	};

#ifdef CPE__MT

	struct _flow_data__ {
		sck::socket_ioflow___ Flow;
		void *UP;
	};

	class _functions__
	: public socket_user_functions__
	{
	protected:
		virtual void *CSDBNSPreProcess(
			socket__ Socket,
			const char *IP )
		{
			_flow_data__ *Data = NULL;
		ERRProlog
		ERRBegin
			Data = new _flow_data__;

			if ( Data == NULL )
				ERRa();

			Data->Flow.Init( Socket );
			Data->UP = UserFunctions->PreProcess( IP );
		ERRErr
			if ( Data != NULL )
				delete Data;
			Data = NULL;
		ERREnd
		ERREpilog
			return Data;
		}
		virtual action__ CSDBNSProcess(
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
		virtual void CSDBNSPostProcess( void *UP )
		{
			if ( UP == NULL )
				ERRc();

			UserFunctions->PostProcess( ((_flow_data__ *)UP)->UP );

			delete (_flow_data__ *)UP;
		}
		virtual void CSDBNSExit( void )
		{
			UserFunctions->Exit();
		}
	public:
		user_functions__ *UserFunctions;
	};

	/*c Handling a server, with process duplication for each client. */
	/* Sous Windows, pour utiliser comme service :
		- Pour installer le service : 
			- 'InitService(...)' (Pas d'autres 'Init(...)',
			- 'InstallService(...)'.
		- Pour lancer le service (lorsque l'application lanc�e par le gestionnaire de service de 'Windows', auqual cas 'argc' == '0').
			- 'Init(...)',
			- 'LaunchService()'.
	*/
	class server___
	: public listener___
#ifdef CSD__HANDLE_SERVICE
	, protected _service__
#endif
	{
#ifdef CSD__HANDLE_SERVICE
	protected:
		virtual void WINTOLProcess( void )
		{
			while ( !TestTermination() )
				Process( 2500 );
		}
		virtual void WINTOLShutdown( void )
		{
			_Functions.Exit();
		}
#endif
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
		/*f Initialzation with 'Port' as port to listen.
		A maximum of 'Amount' are accepted in the waiting queue. */
		bso::bool__ Init(
			port__ Port,
			socket_user_functions__ &SocketFunctions,
			int Amount,
			err::handling__ ErrorHandling = err::h_Default )
		{
#ifdef CSD__HANDLE_SERVICE
			if ( !_service__::Init() )
				ERRc();
#endif
			_SocketFunctions = &SocketFunctions;
			
			return listener___::Init( Port, Amount, ErrorHandling );
		}
		/*f Initialzation with 'Port' as port to listen.
		A maximum of 'Amount' are accepted in the waiting queue. */
		bso::bool__ Init(
			port__ Port,
			socket_user_functions__ &SocketFunctions,
			err::handling__ ErrorHandling = err::h_Default,
			int Amount = 5 )
		{
#ifdef CSD__HANDLE_SERVICE
			if ( !_service__::Init() )
				ERRc();
#endif
			_SocketFunctions = &SocketFunctions;

			return listener___::Init( Port, Amount, ErrorHandling );
		}
		bso::bool__ Init(
			port__ Port,
			user_functions__ &UserFunctions,
			err::handling__ ErrorHandling = err::h_Default,
			int Amount = 5 )
		{
#ifdef CSD__HANDLE_SERVICE
			if ( !_service__::Init() )
				ERRc();
#endif
			_Functions.UserFunctions = &UserFunctions;

			return Init( Port, this->_Functions, Amount, ErrorHandling );
		}
		bso::bool__ Init(
			port__ Port,
			user_functions__ &UserFunctions,
			int Amount,
			err::handling__ ErrorHandling = err::h_Default )
		{
#ifdef CSD__HANDLE_SERVICE
			if ( !_service__::Init() )
				ERRc();
#endif
			_Functions.UserFunctions = &UserFunctions;

			return Init( Port, this->_Functions, Amount, ErrorHandling );
		}
		bso::bool__ LaunchService( const char *ServiceName )
		{
#ifdef CSD__HANDLE_SERVICE
			return _service__::Launch( ServiceName );
#else
			return false;
#endif
		}
		void Process(
			sck::duration__ TimeOut = CSDNBS__DEFAULT_TIMEOUT,
			err::handling__ ErrorHandling = err::h_Default );
	};

#endif
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
