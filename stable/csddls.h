/*
	Header for the 'csddls' library by Claude SIMON (csimon@epeios.org)
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

#ifndef CSDDLS__INC
#define CSDDLS__INC

#define CSDDLS_NAME		"CSDDLS"

#define	CSDDLS_VERSION	"$Revision$"

#define CSDDLS_OWNER		"Claude SIMON (csimon@epeios.org)"

#include "ttr.h"

extern class ttr_tutor &CSDDLSTutor;

#if defined( XXX_DBG ) && !defined( CSDDLS_NODBG )
#define CSDDLS_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision$
//C Claude SIMON (csimon@epeios.org)
//R $Date$

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D Client-Server Devices Dynamic Library Server 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "csdscm.h"
#include "cpe.h"

#ifdef CPE__MS
#	define CSDDSL__FUNCTION_SPEC	__declspec(dllexport)
#elif defined( CPE__UNIX )
#	define CSDDSL__FUNCTION_SPEC
#else
#	error "OS not supported yet."
#endif

extern "C" CSDDSL__FUNCTION_SPEC csdscm::user_functions__ *CSDDLEntry( void *UP );

namespace csddls {
	csdscm::user_functions__ *CSDDLSCallback( void *UP );	// A surcharger.
}		

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
