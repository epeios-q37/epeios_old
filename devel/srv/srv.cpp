/*
  'srv' library by Claude L. Simon (simon@epeios.org)
  Requires the 'srv' header file ('srv.h').
  Copyright (C) 2000,2001 Claude L. SIMON (simon@epeios.org).

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

#define SRV__COMPILATION

#include "srv.h"

class srvtutor
: public ttr_tutor
{
public:
	srvtutor( void )
	: ttr_tutor( SRV_NAME )
	{
#ifdef SRV_DBG
		Version = SRV_VERSION "\b\bD $";
#else
		Version = SRV_VERSION;
#endif
		Owner = SRV_OWNER;
		Date = "$Date$";
	}
	virtual ~srvtutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace srv;

#ifdef CPE__MT
#include "mtx.h"
#include "mtk.h"
#endif

void srv::listener___::Init(
	service__ Service,
	int Nombre )
{
	sockaddr_in nom;

	sck::Initialize();

	reset();

	Socket_ = CreateSocket();

	memset((char *)&nom,0,sizeof(nom));
	nom.sin_port=htons( Service );
	nom.sin_addr.s_addr=INADDR_ANY;
	nom.sin_family=AF_INET;

	if( bind( Socket_, (struct sockaddr*)(&nom), sizeof(sockaddr_in) ) )
		ERRs();

	if ( listen( Socket_, Nombre ) )
		ERRs();
}

socket__ srv::listener___::Interroger_( err::handle ErrHandle )
{
	fd_set fds;
	int Reponse;
	bso__bool Boucler = true;
	socket__ Socket;

	while( Boucler )
	{
ERRProlog
		Socket = SCK_INVALID_SOCKET;
ERRBegin
		Boucler = false;
		FD_ZERO( &fds );
		FD_SET( Socket_, &fds );

		Reponse = select( Socket_ + 1, &fds, 0, 0, NULL );

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

#ifdef CPE__MT
struct data__
{
	socket_functions__ *Functions;
	sck::socket__ Socket;
	mtx::mutex_handler__ Mutex;
};

static void Traiter_( void *PU )
{
	::data__ &Data = *(::data__ *)PU;
ERRFProlog
	bso__bool Close = true;
	socket_functions__ &Functions = *Data.Functions;
	socket__ Socket = Data.Socket;
ERRFBegin
	mtx::Unlock( Data.Mutex );

	ERRProlog
	ERRBegin
		Functions.Process( Socket );
	ERRErr
	ERREnd
	ERREpilog
ERRFErr
ERRFEnd
ERRFEpilog
}

void server___::Process(
	socket_functions__ &Functions,
	err::handle ErrHandle )
{
ERRProlog
	sck::socket__ Socket = SCK_INVALID_SOCKET;
	::data__ Data = {NULL, SCK_INVALID_SOCKET, MTX_INVALID_HANDLER};
ERRBegin

	Socket = listener___::GetConnection( ErrHandle );

	Data.Functions = &Functions;
	Data.Mutex = mtx::Create();

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
	class internal_functions__
	: public socket_functions__
	{
	protected:
		virtual void SRVProcess( socket__ Socket )
		{
		ERRProlog
			sck::socket_ioflow___ SocketFlow;
		ERRBegin
			SocketFlow.Init( Socket );
			Functions->Process( SocketFlow );
		ERRErr
		ERREnd
		ERREpilog
		}
	public:
		flow_functions__ *Functions;
	};
}

void server___::Process(
	flow_functions__ &Functions,
	err::handle ErrHandle )
{
	internal_functions__ F;

	F.Functions = &Functions;

	Process( F, ErrHandle );
}


#endif

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */
class srvpersonnalization
: public srvtutor
{
public:
	srvpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~srvpersonnalization( void )
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

static srvpersonnalization Tutor;

ttr_tutor &SRVTutor = Tutor;
