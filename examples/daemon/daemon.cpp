/*
	'daemon' library by Claude SIMON (csimon@epeios.org)
	Requires the 'daemon' header file ('daemon.h').
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

#define DAEMON__COMPILATION

#include "daemon.h"

class daemontutor
: public ttr_tutor
{
public:
	daemontutor( void )
	: ttr_tutor( DAEMON_NAME )
	{
#ifdef DAEMON_DBG
		Version = DAEMON_VERSION "\b\bD $";
#else
		Version = DAEMON_VERSION;
#endif
		Owner = DAEMON_OWNER;
		Date = "$Date$";
	}
	virtual ~daemontutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace deamon;

bso::bool__ deamon::user_function__::_Start( txf::text_oflow__ &Flow )
{
	bso::bool__ Odd = false;
ERRProlog
	lck::exclusive_access___<shared__> Shared;
ERRBegin
	Shared.Init( Control_ );
	Odd = ( Shared().Id % 2 ) == 1;

	Flow << "Vous êtes le client numéro " << Shared().Id++ << '.' << txf::nl << txf::sync;

	Shared().Counter++;

	if ( Odd ) {
		mtx::Lock( Mutexes.Even );
		mtx::Unlock( Mutexes.Odd );
	} else {
		mtx::Lock( Mutexes.Odd );
		mtx::Unlock( Mutexes.Even );
	}

ERRErr
ERREnd
ERREpilog
	return Odd;
}

void deamon::user_function__::_Other( txf::text_oflow__ &Flow )
{
ERRProlog
	lck::shared_access___<shared__> Shared;
ERRBegin
	Shared.Init( Control_ );

	Flow << "Il y a maintenant un total de " << Shared().Counter << " client(s)." << txf::nl << txf::sync;

ERRErr
ERREnd
ERREpilog
}

void deamon::user_function__::SRVProcess( flw::ioflow__ &Flow )
{
ERRProlog
	txf::text_oflow__ TFlow;
	bso::bool__ Odd = false;
ERRBegin
	TFlow.Init( Flow );

	Odd = _Start( TFlow );

	while ( 1 ) {
		if ( Odd ) {
			mtx::Lock( Mutexes.Even );
		} else {
			mtx::Lock( Mutexes.Odd );
		}

		_Other( TFlow );

		if ( Odd ) {
			mtx::Unlock( Mutexes.Even );
		} else {
			mtx::Unlock( Mutexes.Odd );
		}

		Odd = !Odd;

	}


ERRErr
ERREnd
ERREpilog
}

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class daemonpersonnalization
: public daemontutor
{
public:
	daemonpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~daemonpersonnalization( void )
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

static daemonpersonnalization Tutor;

ttr_tutor &DAEMONTutor = Tutor;
