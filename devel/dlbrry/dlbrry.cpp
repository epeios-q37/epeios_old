/*
	'dlbrry' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'dlbrry' header file ('dlbrry.h').
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

#define DLBRRY__COMPILATION

#include "dlbrry.h"

class dlbrrytutor
: public ttr_tutor
{
public:
	dlbrrytutor( void )
	: ttr_tutor( DLBRRY_NAME )
	{
#ifdef DLBRRY_DBG
		Version = DLBRRY_VERSION "\b\bD $";
#else
		Version = DLBRRY_VERSION;
#endif
		Owner = DLBRRY_OWNER;
		Date = "$Date$";
	}
	virtual ~dlbrrytutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace dlbrry;

#if defined( CPE__T_LINUX ) || defined( CPE__T_BEOS ) || defined( CPE__T_CYGWIN ) || defined( CPE__T_MAC )
#	define TARGET_POSIX
#elif defined( CPE__T_MS )
#	define TARGET_MS
#else
#	error "Unknown target !"
#endif

#ifdef TARGET_MS
#	include <windows.h>
#elif defined ( TARGET_POSIX )
#	include <dlfcn.h>
#else
#	error
#endif

bso::bool__ dlbrry::dynamic_library___::_LoadLibrary( const char *LibraryName )
{
	if ( _LibraryHandler != NULL )
		ERRc();

#ifdef TARGET_MS
	if ( ( _LibraryHandler = LoadLibrary( LibraryName ) ) == NULL )
		return false;
#elif defined( TARGET_POSIX )
	if ( ( _LibraryHandler = dlopen( LibraryName, RTLD_LAZY ) ) == NULL ) {
		const char *Error = dlerror();	// Facilite le débogage.
		return false;
	}
#else
#	error
#endif
	return true;
}
bso::bool__ dlbrry::dynamic_library___::_UnloadLibrary( void  )
{
	if ( _LibraryHandler == NULL )
		ERRc();

#ifdef TARGET_MS
	if ( !FreeLibrary( (HMODULE)_LibraryHandler) )
		return false;
#elif defined( TARGET_POSIX )
	if ( dlclose( _LibraryHandler ) == -1 ) {
		const char *Error = dlerror();	// Facilite le débogage.
		return false;
	}
#else
#	error
#endif
	_LibraryHandler = NULL;

	return true;
}

void * dlbrry::dynamic_library___::GetFunction( const char *FunctionName )
{
	void *Function = NULL;

	if ( !IsInitialized() )
		ERRc();

#ifdef TARGET_MS
	Function = GetProcAddress( (HMODULE)_LibraryHandler, FunctionName );
#elif defined( TARGET_POSIX )
	Function = dlsym( _LibraryHandler, FunctionName );

	if ( Function == NULL )
		const char *Error = dlerror();	// Facilite le débogage.
#else
#	error
#endif

	return Function;
}




/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class dlbrrypersonnalization
: public dlbrrytutor
{
public:
	dlbrrypersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~dlbrrypersonnalization( void )
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

static dlbrrypersonnalization Tutor;

ttr_tutor &DLBRRYTutor = Tutor;
