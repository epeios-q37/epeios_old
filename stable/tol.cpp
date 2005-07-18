/*
	'tol' library by Claude SIMON (csimon@epeios.org)
	Requires the 'tol' header file ('tol.h').
	Copyright (C) 2000-2004 Claude SIMON (csimon@epeios.org).

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

#define TOL__COMPILATION

#include "tol.h"

class toltutor
: public ttr_tutor
{
public:
	toltutor( void )
	: ttr_tutor( TOL_NAME )
	{
#ifdef TOL_DBG
		Version = TOL_VERSION "\b\bD $";
#else
		Version = TOL_VERSION;
#endif
		Owner = TOL_OWNER;
		Date = "$Date$";
	}
	virtual ~toltutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

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

#ifdef CPE__MT
#include "mtk.h"
#endif

#include <fstream>

bool tol::FileExists( const char *Nom )
{
	std::ifstream Stream( Nom, std::ios::binary );

#ifdef CPE__VC
	return Stream != NULL;
#else
	return Stream;
#endif
}

const char *tol::Date( buffer__ &Buffer )
{
   struct tm *time_now;
   time_t secs_now;

   time(&secs_now);
   time_now = localtime(&secs_now);

   if ( !strftime( Buffer, sizeof( Buffer ), "%d/%m/%Y", time_now) )
	ERRl();

   return Buffer;
}

const char *tol::Time( buffer__ &Buffer )
{
   struct tm *time_now;
   time_t secs_now;

   time(&secs_now);
   time_now = localtime(&secs_now);

   if ( !strftime( Buffer, sizeof( Buffer ), "%H:%M:%S", time_now) )
	ERRl();

   return Buffer;
}

const char *tol::DateAndTime( buffer__ &Buffer )
{
   struct tm *time_now;
   time_t secs_now;

   time(&secs_now);
   time_now = localtime(&secs_now);

   if ( !strftime( Buffer, sizeof( Buffer ), "%d/%m/%Y %H:%M:%S", time_now) )
	ERRl();

   return Buffer;
}

void tol::Suspend( unsigned long Delay )
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

#if _MSC_VER != 1400
typedef unsigned long	intptr_t;
#endif


#ifdef CPE__MT
namespace {
	void WaitAndExit( void *UP )
	{
		tol::Wait( (unsigned long)(intptr_t)UP );
		exit( EXIT_SUCCESS );
	}
}

void tol::Defer( void )
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

void tol::ForceExit( unsigned long Seconds )
{
	mtk::Launch( WaitAndExit, (void *)(intptr_t)Seconds );
}
#endif

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */
class tolpersonnalization
: public toltutor
{
public:
	tolpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~tolpersonnalization( void )
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

static tolpersonnalization Tutor;

ttr_tutor &TOLTutor = Tutor;
