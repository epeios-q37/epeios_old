/*
	'csdlec' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'csdlec' header file ('csdlec.h').
	Copyright (C) $COPYRIGHT_DATES$Claude SIMON.
$_RAW_$
	This file is part of the Epeios (http://zeusw.org/epeios/) project.

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

#define CSDLEC__COMPILATION

#include "csdlec.h"

class csdlectutor
: public ttr_tutor
{
public:
	csdlectutor( void )
	: ttr_tutor( CSDLEC_NAME )
	{
#ifdef CSDLEC_DBG
		Version = CSDLEC_VERSION "\b\bD $";
#else
		Version = CSDLEC_VERSION;
#endif
		Owner = CSDLEC_OWNER;
		Date = "$Date$";
	}
	virtual ~csdlectutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace csdlec;

#if defined( CPE__T_LINUX ) || defined( CPE__T_BEOS ) || defined( CPE__T_CYGWIN ) || defined( CPE__T_MAC )
#	define CSDDLC__POSIX
#elif defined( CPE__T_MS )
#	define CSDDLC__MS
#else
#	error "Unknown target !"
#endif

#ifdef CSDDLC__MS
#	include <windows.h>
#elif defined ( CSDDLC__POSIX )
#	include <dlfcn.h>
#else
#	error
#endif

using namespace csdlec;

bso::bool__ csdlec::library_embedded_client_core__::_LoadLibrary( const char *LibraryName )
{
	if ( _LibraryHandler != NULL )
		ERRu();

#ifdef CSDDLC__MS
	if ( ( _LibraryHandler = LoadLibrary( LibraryName ) ) == NULL )
		return false;
#elif defined( CSDDLC__POSIX )
	if ( ( _LibraryHandler = dlopen( LibraryName, RTLD_LAZY ) ) == NULL ) {
		const char *Error = dlerror();	// Facilite le débogage.
		return false;
	}
#else
#	error
#endif
	return true;
}
bso::bool__ csdlec::library_embedded_client_core__::_UnloadLibrary( void  )
{
	if ( _LibraryHandler == NULL )
		ERRu();

#ifdef CSDDLC__MS
	if ( !FreeLibrary( (HMODULE)_LibraryHandler ) )
		return false;
#elif defined( CSDDLC__POSIX )
	if ( dlclose( _LibraryHandler ) == -1 ) {
		const char *Error = dlerror();	// Facilite le débogage.
		return false;
	}
#else
#	error
#endif
	return true;
}


template <typename function> function GetFunction_(
	const char *FunctionName,
	void *LibraryHandler )
{
	function Function;

	if ( LibraryHandler == NULL )
		ERRu();

#ifdef CSDDLC__MS
	if ( ( Function = (function)GetProcAddress( (HMODULE)LibraryHandler, FunctionName ) ) == NULL )
		ERRs();
#elif defined( CSDDLC__POSIX )
	Function = (function)dlsym( LibraryHandler, FunctionName );

	if ( Function == NULL )
		const char *Error = dlerror();	// Facilite le débogage.
#else
#	error
#endif

	return Function;
}

extern "C" typedef csdleo::retrieve_steering retrieve_steering;

bso::bool__ csdlec::library_embedded_client_core__::_RetrieveSteering( csdleo::shared_data__ *Data )
{
	retrieve_steering *RetrieveSteering = GetFunction_<retrieve_steering *>( E_STRING( CSDLEO_RETRIEVE_STEERING_FUNCTION_NAME ), _LibraryHandler );

	if ( RetrieveSteering == NULL )
		return false;

	if ( ( _Steering = RetrieveSteering( Data ) ) == NULL )
		return false;

	return true;
}

extern "C" typedef csdleo::release_steering release_steering;

bso::bool__ csdlec::library_embedded_client_core__::_ReleaseSteering( void )
{
	release_steering *ReleaseSteering = GetFunction_<release_steering *>( E_STRING( CSDLEO_RELEASE_STEERING_FUNCTION_NAME ), _LibraryHandler );

	if ( ReleaseSteering == NULL )
		return false;

	ReleaseSteering( _Steering );

	return true;
}

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class csdlecpersonnalization
: public csdlectutor
{
public:
	csdlecpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~csdlecpersonnalization( void )
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

static csdlecpersonnalization Tutor;

ttr_tutor &CSDLECTutor = Tutor;
