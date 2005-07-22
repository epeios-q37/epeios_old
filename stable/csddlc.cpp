/*
	'csddlc' library by Claude SIMON (csimon@epeios.org)
	Requires the 'csddlc' header file ('csddlc.h').
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

#define CSDDLC__COMPILATION

#include "csddlc.h"

class csddlctutor
: public ttr_tutor
{
public:
	csddlctutor( void )
	: ttr_tutor( CSDDLC_NAME )
	{
#ifdef CSDDLC_DBG
		Version = CSDDLC_VERSION "\b\bD $";
#else
		Version = CSDDLC_VERSION;
#endif
		Owner = CSDDLC_OWNER;
		Date = "$Date$";
	}
	virtual ~csddlctutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#include "windows.h"

using namespace csddlc;

typedef csdscm::user_functions__ *(*f)( void *);

void csddlc::dynamic_library_client_core::reset( bso::bool__ P )
{
	if ( P ) {
		if ( _DLLHandler != NULL )
			FreeLibrary( (HMODULE)_DLLHandler );
	}

	_UserFunctions = NULL;
	_DLLHandler = NULL;
}


void csddlc::dynamic_library_client_core::Init(
	const char *LibraryName,
	void *UP )
{
ERRProlog
	f CSDDLGet;
ERRBegin
	reset();

#if 1
	if ( ( _DLLHandler = LoadLibrary( LibraryName ) ) == NULL )
		ERRu();
#else
	if ( ( _DLLHandler = GetModuleHandle( LibraryName ) ) == NULL )
		ERRu();
#endif

	if ( ( CSDDLGet = (f)GetProcAddress( (HMODULE)_DLLHandler, "CSDDLEntry" ) ) == NULL )
		ERRu();

	_UserFunctions = CSDDLGet( UP );
ERRErr
	reset();
ERREnd
ERREpilog
}


/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class csddlcpersonnalization
: public csddlctutor
{
public:
	csddlcpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~csddlcpersonnalization( void )
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

static csddlcpersonnalization Tutor;

ttr_tutor &CSDDLCTutor = Tutor;
