/*
	Header for the 'llio' library by Claude SIMON (csimon@epeios.org)
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

#ifndef LLIO__INC
#define LLIO__INC

#define LLIO_NAME		"LLIO"

#define	LLIO_VERSION	"$Revision$"

#define LLIO_OWNER		"Claude SIMON (csimon@epeios.org)"

#include "ttr.h"

extern class ttr_tutor &LLIOTutor;

#if defined( XXX_DBG ) && !defined( LLIO_NODBG )
#define LLIO_DBG
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
//D Low-Level Input/Output 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "cpe.h"

#ifdef CPE__MS
#	include "wllio.h"
#elif defined( CPE__UNIX )
#	include "pllio.h"
#elif defined( CPE__MAC )
#	include "mllio.h"
#else
#	error "Not implemented yet."
#endif

namespace llio {
#ifdef CPE__MS
	using namespace wllio;
#	define LLIO_UNDEFINED_DESCRIPTOR	WLLIO_UNDEFINED_DESCRIPTOR
#elif defined( CPE__UNIX )
	using namespace pllio;
#	define LLIO_UNDEFINED_DESCRIPTOR	PLLIO_UNDEFINED_DESCRIPTOR
#elif defined( CPE__MAC )
	using namespace mllio;
#endif
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
