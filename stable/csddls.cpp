/*
	'csddls' library by Claude SIMON (csimon@epeios.org)
	Requires the 'csddls' header file ('csddls.h').
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

#define CSDDLS__COMPILATION

#include "csddls.h"

class csddlstutor
: public ttr_tutor
{
public:
	csddlstutor( void )
	: ttr_tutor( CSDDLS_NAME )
	{
#ifdef CSDDLS_DBG
		Version = CSDDLS_VERSION "\b\bD $";
#else
		Version = CSDDLS_VERSION;
#endif
		Owner = CSDDLS_OWNER;
		Date = "$Date$";
	}
	virtual ~csddlstutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace csddls;

#ifdef CPE__MS

#	include <windows.h>

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			break;
		case DLL_THREAD_ATTACH:
			break;
		case DLL_THREAD_DETACH:
			break;
		case DLL_PROCESS_DETACH:
			break;
		default:
			break;
    }
    return TRUE;
}
#endif

csdscm::user_functions__ *CSDDLEntry( void *UP )
{
	return &csddls::CSDDLSCallback( UP );
}

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class csddlspersonnalization
: public csddlstutor
{
public:
	csddlspersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~csddlspersonnalization( void )
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

static csddlspersonnalization Tutor;

ttr_tutor &CSDDLSTutor = Tutor;
