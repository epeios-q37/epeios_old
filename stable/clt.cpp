/*
  'clt' library by Claude L. Simon (epeios@epeios.org)
  Requires the 'clt' header file ('clt.h').
  Copyright (C) 2000 Claude L. SIMON (epeios@epeios.org).

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
  along with this program; if not, go to http://www.fsf.org or write to the:
  
                        Free Software Foundation, Inc.,
           59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

//	$Id$

#define CLT__COMPILATION

#include "clt.h"

class clttutor
: public ttr_tutor
{
public:
	clttutor( void )
	: ttr_tutor( CLT_NAME )
	{
#ifdef CLT_DBG
		Version = CLT_VERSION " (DBG)";
#else
		Version = CLT_VERSION;
#endif
		Owner = CLT_OWNER;
		Date = "$Date$";
	}
	virtual ~clttutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace clt;

const char *clt::Host( const char *HostService )
{
	static char Adresse[50];
	char *P;

	if ( strlen( HostService ) >= sizeof( Adresse ) )
		ERRl();

	strcpy( Adresse, HostService );

	P = strchr( Adresse, ':' );

	if ( P )
		*P = 0;

	return Adresse;
}

#ifdef CPE__BEOS
#	ifdef INADDR_NONE
#		error "Be OS include file have changed !"
#	else
#		define INADDR_NONE     (unsigned long)0xffffffff
#	endif
#endif

static inline bso__bool GetHost_(
	const char *Host,
	struct sockaddr_in &Nom )
{
	struct hostent *Hostent;
	bso__bool Return = true;

	if ( ( Hostent =(struct hostent *)gethostbyname( Host ) ) != NULL )
		memcpy((void *)&Nom.sin_addr,(void *)Hostent->h_addr,Hostent->h_length);
	else if ( ( Nom.sin_addr.s_addr = inet_addr( Host ) ) == INADDR_NONE )
		Return = false;

	return Return;
}

static inline bso__bool GetService_(
	const char *Service,
	struct sockaddr_in &Nom )
{
	struct servent *Servent;

	if ( ( Servent = getservbyname( Service, NULL ) ) != NULL )
		Nom.sin_port = Servent->s_port;
	else
		Nom.sin_port=htons( atoi( Service ) );

	return true;
}

socket__ clt::Connect(
	const char *Host,
	const char *Service,
	socket__ Desc,
	err::handle ErrHandle )
{
	struct sockaddr_in Nom;

	sck::Initialize();

	Nom.sin_family = AF_INET;

	if ( GetHost_( Host, Nom ) && GetService_( Service, Nom ) ) {
		if ( Desc == SCK_INVALID_SOCKET )
			Desc = CreateSocket( ErrHandle );

		if ( Desc != SCK_INVALID_SOCKET )
			if ( connect( Desc, (sockaddr *)&Nom, sizeof( Nom ) ) == SCK_SOCKET_ERROR )
				Desc = SCK_INVALID_SOCKET;

		if ( ( Desc == SCK_INVALID_SOCKET ) && ( ErrHandle == err::hUsual ) )
			ERRs();
	} else {
		Desc = SCK_INVALID_SOCKET;

		if ( ErrHandle == err::hUsual )
			ERRu();
	}

	return Desc;
}


/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */
class cltpersonnalization
: public clttutor
{
public:
	cltpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~cltpersonnalization( void )
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

static cltpersonnalization Tutor;

ttr_tutor &CLTTutor = Tutor;
