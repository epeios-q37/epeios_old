/*
	'tht' library by Claude SIMON (csimon@epeios.org)
	Requires the 'tht' header file ('tht.h').
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

#define THT__COMPILATION

#include "tht.h"

class thttutor
: public ttr_tutor
{
public:
	thttutor( void )
	: ttr_tutor( THT_NAME )
	{
#ifdef THT_DBG
		Version = THT_VERSION "\b\bD $";
#else
		Version = THT_VERSION;
#endif
		Owner = THT_OWNER;
		Date = "$Date$";
	}
	virtual ~thttutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace tht;

#ifdef CPE__MS
#	include <stdlib.h>
#	include "windows.h"
#elif defined( CPE__UNIX ) || defined( CPE__BEOS )
#	include <unistd.h>
#	include <sched.h>
#	include <errno.h>
#elif !defined( CPE__MAC )
#	error "Unknown compilation enviroment"
#endif



void tht::Suspend( unsigned long Delay )
{
#ifdef CPE__MS
	Sleep( Delay );
#elif defined( CPE__UNIX ) || defined( CPE__BEOS )
	struct timespec T;

	T.tv_sec = Delay / 1000;
	T.tv_nsec = ( Delay % 1000 ) * 1000000;

	while ( nanosleep( &T, &T ) )
		if ( errno != EINTR )
			ERRs();
#elif defined( CPE__MAC )
	ERRl();
#else
#	error "Unknown compilation enviroment"
#endif
}


void tht::Defer( void )
{
#ifdef CPE__MS
	Sleep( 0 );	// PAs 0, sinon l'usage CPU monte à 100%
#elif defined( CPE__UNIX )
	if( sched_yield() != 0 )
		ERRs();
#else
	#error "Unknown compilation enviroment"
#endif
}

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class thtpersonnalization
: public thttutor
{
public:
	thtpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~thtpersonnalization( void )
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

static thtpersonnalization Tutor;

ttr_tutor &THTTutor = Tutor;
