/*
	'cio' library by Claude SIMON (csimon@epeios.org)
	Requires the 'cio' header file ('cio.h').
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

#ifdef IOP__USE_LOWLEVEL_IO
#	if defined( CPE__MS ) || defined( CPE__UNIX )
iop::descriptor__ cio::cind = 0, cio::coutd = 1, cio::cerrd = 2;
#	elif defined( CPE__MAC )
#		error "Not implemented yet ! "
#	else
#		error "Unknow compilation enviroment !"
#	endif
#elif defined( IOP__USE_STANDARD_IO )
iop::descriptor__ cio::cind = stdin, cio::coutd = stdout, cio::cerrd = stderr;
#else
#	error "Unkonw I/O enviroment !"
#endif

iof::io_oflow_functions___ cio::_coutf;
iof::io_oflow_functions___ cio::_cerrf;
iof::io_iflow_functions___ cio::_cinf;

#ifndef CPE__T_MT
	aware_coutf___ cio::coutf;
	aware_cerrf___ cio::cerrf;
	aware_cinf___ cio::cinf;

	aware_cout___ cio::cout;
	aware_cerr___ cio::cerr;
	aware_cin___ cio::cin;
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
		cio::_coutf.Init( coutd );
		cio::_cinf.Init( cind );
		cio::_cerrf.Init( cerrd );

	/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~ciopersonnalization( void )
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

static ciopersonnalization Tutor;

ttr_tutor &CIOTutor = Tutor;
