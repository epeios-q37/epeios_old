/*
	'csdbns' library by Claude SIMON (csimon@epeios.org)
	Requires the 'csdbns' header file ('csdbns.h').
	Copyright (C) 2004 Claude SIMON (csimon@epeios.org).

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

#define CSDBNS__COMPILATION

#include "csdbns.h"

class csdbnstutor
: public ttr_tutor
{
public:
	csdbnstutor( void )
	: ttr_tutor( CSDBNS_NAME )
	{
#ifdef CSDBNS_DBG
		Version = CSDBNS_VERSION "\b\bD $";
#else
		Version = CSDBNS_VERSION;
#endif
		Owner = CSDBNS_OWNER;
		Date = "$Date$";
	}
	virtual ~csdbnstutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace csdbns;

#ifdef CPE__MT
#include "mtx.h"
#include "mtk.h"
#endif

bso::bool__ csdbns::listener___::Init(
	service__ Service,
	int Amount,
	err::handle ErrHandle)
{
	sockaddr_in nom;

	sck::Initialize();

	reset();

	Socket_ = CreateSocket();

	memset((char *)&nom,0,sizeof(nom));
	nom.sin_port=htons( Service );
	nom.sin_addr.s_addr=INADDR_ANY;
	nom.sin_family=AF_INET;

#ifdef CPE__UNIX
	int Val = ~0;

	if ( setsockopt( Socket_, SOL_SOCKET, SO_REUSEADDR, &Val, sizeof( Val ) ) != 
0 )
		ERRs();
#endif

	if( bind( Socket_, (struct sockaddr*)(&nom), sizeof(sockaddr_in) ) )
		if ( ErrHandle == err::hUsual )
			ERRs();
		else
			return false;

	if ( listen( Socket_, Amount ) )
		ERRs();
		
	return true;
}

socket__ csdbns::listener___::Interroger_( err::handle ErrHandle )
{
	fd_set fds;
	int Reponse;
	bso::bool__ Boucler = true;
	socket__ Socket;

	while( Boucler )
	{
ERRProlog
		Socket = SCK_INVALID_SOCKET;
ERRBegin
		Boucler = false;
		FD_ZERO( &fds );
		FD_SET( Socket_, &fds );

		Reponse = select( (int)( Socket_ + 1 ), &fds, 0, 0, NULL );

		if ( Reponse == SCK_SOCKET_ERROR )
			ERRs();
		else if ( Reponse > 0 )
		{
			if ( ( Socket = accept( Socket_, NULL, NULL ) ) == SCK_INVALID_SOCKET )
				if ( Error() != SCK_EWOULDBLOCK )
					ERRs();
		}
ERRErr
	if ( ErrHandle == err::hSkip )
	{
		ERRRst();
		Boucler = true;
	}
	else if ( ErrHandle != err::hUsual )
		ERRu();
ERREnd
ERREpilog
	}

	return Socket;
}

void csdbns::listener___::Process(
	socket_user_functions__ &Functions,
	err::handle ErrHandle )
{
ERRProlog
	void *UP = NULL;
	sck::socket__ Socket = SCK_INVALID_SOCKET;
	action__ Action = a_Undefined;
ERRBegin
	Socket = Interroger_( ErrHandle );

	UP = Functions.PreProcess( Socket );

	while ( ( Action = Functions.Process( Socket, UP ) ) == aContinue );

	switch( Action ) {
	case aContinue:
		ERRc();
		break;
	case aStop:
		break;
	default:
		ERRu();
		break;
	}

ERRErr
ERREnd
	if ( UP != NULL )
		Functions.PostProcess( UP );
ERREpilog
}

#ifdef CPE__MT
struct socket_data__
{
	socket_user_functions__ *Functions;
	sck::socket__ Socket;
	mtx::mutex_handler__ Mutex;
};

static void Traiter_( void *PU )
{
	::socket_data__ &Data = *(::socket_data__ *)PU;
ERRFProlog
	bso::bool__ Close = true;
	socket_user_functions__ &Functions = *Data.Functions;
	socket__ Socket = Data.Socket;
	void *UP = NULL;
	action__ Action = a_Undefined;
ERRFBegin
	mtx::Unlock( Data.Mutex );

	ERRProlog
	ERRBegin
		UP = Functions.PreProcess( Socket );

		while ( ( Action = Functions.Process( Socket, UP ) ) == aContinue );
	ERRErr
	ERREnd
		if ( UP != NULL )
			Functions.PostProcess( UP );
	ERREpilog
ERRFErr
ERRFEnd
ERRFEpilog
}

void server___::Process(
	socket_user_functions__ &Functions,
	err::handle ErrHandle )
{
ERRProlog
	sck::socket__ Socket = SCK_INVALID_SOCKET;
	::socket_data__ Data = {NULL, SCK_INVALID_SOCKET, MTX_INVALID_HANDLER};
ERRBegin

	Socket = listener___::GetConnection( ErrHandle );

	Data.Functions = &Functions;
	Data.Mutex = mtx::Create( mtx::mFree );

	mtx::Lock( Data.Mutex );	// Unlocked by the 'Traiter_()' function.

	Data.Socket = Socket;


	for(;;)
	{
		mtk::Launch( Traiter_, &Data );

//		SCKClose( Socket );	// Only needed when using processes.

		Socket = SCK_INVALID_SOCKET;

		Socket = listener___::GetConnection( ErrHandle );

		mtx::Lock( Data.Mutex );	// Unlocked by the 'Traiter_()' function.

		Data.Socket = Socket;
	}
ERRErr
ERREnd
	if ( Socket != SCK_INVALID_SOCKET )
		sck::Close( Socket );

	if ( Data.Mutex != MTX_INVALID_HANDLER )
		mtx::Delete( Data.Mutex );
ERREpilog
}


namespace {
	struct flow_data__ {
		sck::socket_ioflow___ Flow;
		void *UP;
	};

	class internal_functions__
	: public socket_user_functions__
	{
	protected:
		virtual void *CSDNBSPreProcess( socket__ Socket )
		{
			flow_data__ *Data = NULL;
		ERRProlog
		ERRBegin
			Data = new flow_data__;

			if ( Data == NULL )
				ERRa();

			Data->Flow.Init( Socket );
			Data->UP = Functions->PreProcess( Data->Flow );
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
			flow_data__ &Data = *(flow_data__ *)UP;
#ifdef CSDNBS_DBG
			if ( Data.Flow.GetSocket() != Socket )
				ERRc();
#endif

			return Functions->Process( Data.Flow, Data.UP );
		}
		virtual void CSDNBSPostProcess( void *UP )
		{
			if ( UP == NULL )
				ERRc();

			delete (flow_data__ *)UP;
		}
	public:
		user_functions__ *Functions;
		sck::duration__ TimeOut;
	};
}

void server___::Process(
	user_functions__ &Functions,
	sck::duration__ TimeOut,
	err::handle ErrHandle )
{
	internal_functions__ F;

	F.Functions = &Functions;
	F.TimeOut = TimeOut;

	Process( F, ErrHandle );
}


#endif

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class csdbnspersonnalization
: public csdbnstutor
{
public:
	csdbnspersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~csdbnspersonnalization( void )
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

static csdbnspersonnalization Tutor;

ttr_tutor &CSDBNSTutor = Tutor;
