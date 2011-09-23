/*
	'csdlec' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'csdlec' header file ('csdlec.h').
	Copyright (C) 2004 Claude SIMON.

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

#define FUNCTION_NAME	"CSDDLSEntry"

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

typedef csdleo::user_functions__ *(*f)( void *);

void csdlec::library_embedded_client_core__::reset( bso::bool__ P )
{
	if ( P ) {
		if ( _LibraryHandler != NULL )
#ifdef CSDDLC__MS
			if ( !FreeLibrary( (HMODULE)_LibraryHandler ) )
				ERRs();
#elif defined( CSDDLC__POSIX )
			if ( dlclose( _LibraryHandler ) == -1 )
				ERRs();
#else
#	error
#endif
	}

	_UserFunctions = NULL;
	_LibraryHandler = NULL;
}


bso::bool__ csdlec::library_embedded_client_core__::Init(
	const char *LibraryName,
	void *UP )
{
	bso::bool__ Success = false;
ERRProlog
	f CSDDLGet;
ERRBegin
	reset();

#ifdef CSDDLC__MS
	if ( ( _LibraryHandler = LoadLibrary( LibraryName ) ) == NULL )
		ERRu();

	if ( ( CSDDLGet = (f)GetProcAddress( (HMODULE)_LibraryHandler, FUNCTION_NAME ) ) == NULL )
		ERRs();
#elif defined( CSDDLC__POSIX )
	if ( ( _LibraryHandler = dlopen( LibraryName, RTLD_LAZY ) ) == NULL )
		ERRu();

	CSDDLGet = (f)dlsym( _LibraryHandler, FUNCTION_NAME );

	if ( dlerror() != NULL )
		ERRs();
#else
#	error
#endif

	Success = ( _UserFunctions = CSDDLGet( UP ) ) != NULL;
ERRErr
ERREnd
	if ( !Success )
		reset();
ERREpilog
	return Success;
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
