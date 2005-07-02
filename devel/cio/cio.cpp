/*
	'cio' library by Claude SIMON (csimon@epeios.org)
	Requires the 'cio' header file ('cio.h').
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

#define CIO__COMPILATION

#include "cio.h"

class ciotutor
: public ttr_tutor
{
public:
	ciotutor( void )
	: ttr_tutor( CIO_NAME )
	{
#ifdef CIO_DBG
		Version = CIO_VERSION "\b\bD $";
#else
		Version = CIO_VERSION;
#endif
		Owner = CIO_OWNER;
		Date = "$Date$";
	}
	virtual ~ciotutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#include "flx.h"

#if defined( CPE__MS )
#	include <io.h>
#	include <fcntl.h>
#endif

using namespace cio;

#ifdef IOF__USE_LOWLEVEL_IO
#	if defined( CPE__MS ) || defined( CPE__UNIX )
iof::descriptor__ cio::cind = 0, cio::coutd = 1, cio::cerrd = 2;
#	elif defined( CPE__MAC )
#		error "Not implemented yet ! "
#	else
#		error "Unknow compilation enviroment !"
#	endif
#elif defined( IOF__USE_STANDARD_IO )
iof::descriptor__ cio::cind = stdin, cio::coutd = stdout, cio::cerrd = stderr;
#else
#	error "Unkonw I/O enviroment !"
#endif

_oflow__ cio::unsafe_coutf( coutd, FLW_NO_MUTEX ), cio::unsafe_cerrf( cerrd, FLW_NO_MUTEX );
_iflow__ cio::unsafe_cinf( cind, FLW_NO_MUTEX );

txf::text_oflow__ cio::unsafe_cout( cio::unsafe_coutf ), cio::unsafe_cerr( unsafe_cerrf );
txf::text_iflow__ cio::unsafe_cin( cio::unsafe_cinf );

static flx::dump_oflow__ nul( FLW_NO_MUTEX );
txf::text_oflow__ cio::unsafe_nul( ::nul );

#ifdef CPE__MT
flw::mutex__
	cio::cinm = FLW_NO_MUTEX,
	cio::coutm = FLW_NO_MUTEX,
	cio::cerrm = FLW_NO_MUTEX;
#endif


/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class ciopersonnalization
: public ciotutor
{
public:
	ciopersonnalization( void )
	{
#if defined( CPE__MS )
		if ( _setmode( _fileno( stdin ), _O_BINARY ) == -1 )
			ERRd();

		if ( _setmode( _fileno( stdout ), _O_BINARY ) == -1 )
			ERRd();

		if ( _setmode( _fileno( stderr ), _O_BINARY ) == -1 )
			ERRd();
#endif
#ifdef CPE__MT
		cinm = mtx::Create();
		coutm = mtx::Create();
		cerrm = mtx::Create();
#endif
	/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~ciopersonnalization( void )
	{
#ifdef CPE__MT
		mtx::Delete( cinm );
		mtx::Delete( coutm );
		mtx::Delete( cerrm );
#endif
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

static ciopersonnalization Tutor;

ttr_tutor &CIOTutor = Tutor;
