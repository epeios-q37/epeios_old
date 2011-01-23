/*
	'cio' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Requires the 'cio' header file ('cio.h').
	Copyright (C) $COPYRIGHT_DATES$Claude SIMON (http://zeusw.org/intl/contact.html).
$_RAW_$
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

static bso::bool__ Initialized_ = false;

#if defined( CPE__P_MS )
#	include <io.h>
#	include <fcntl.h>
#endif

using namespace cio;

#ifdef IOP__USE_LOWLEVEL_IO
#	if defined( CPE__P_MS ) || defined( CPE__P_LINUX ) || defined( CPE__P_CYGWIN ) || defined( CPE__P_MAC )
iop::descriptor__ cio::cind = 0, cio::coutd = 1, cio::cerrd = 2;
#else
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
	coutf___ cio::coutf;
	cerrf___ cio::cerrf;
	cinf___ cio::cinf;

	cout___ cio::cout;
	cerr___ cio::cerr;
	cin___ cio::cin;
#endif

void cio::Initialize( void )
{
#if defined( CPE__P_MS )
		if ( _setmode( _fileno( stdin ), _O_BINARY ) == -1 )
			ERRd();

		if ( _setmode( _fileno( stdout ), _O_BINARY ) == -1 )
			ERRd();

		if ( _setmode( _fileno( stderr ), _O_BINARY ) == -1 )
			ERRd();
#endif
		cio::_coutf.Init( coutd, fwf::ts_Default );
		cio::_cinf.Init( cind, fwf::ts_Default );
		cio::_cerrf.Init( cerrd, fwf::ts_Default );

#ifndef CPE__T_MT
		cio::cout.Init();
		cio::cerr.Init();
		cio::cin.Init();
#endif


		::Initialized_ = true;
}

bso::bool__ cio::IsInitialized( void )
{
	return ::Initialized_;
}

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class ciopersonnalization
: public ciotutor
{
public:
	ciopersonnalization( void )
	{
#ifdef CIO__AUTOMATIC_INITIALIZATION
		cio::Initialize();
#endif
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
