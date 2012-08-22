/*
	Header for the 'llio' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Copyright (C) 2004 Claude SIMON (http://zeusw.org/intl/contact.html).

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

#ifndef LLIO__INC
#define LLIO__INC

#define LLIO_NAME		"LLIO"

#define	LLIO_VERSION	"$Revision$"

#define LLIO_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"

#include "ttr.h"

extern class ttr_tutor &LLIOTutor;

#if defined( XXX_DBG ) && !defined( LLIO_NODBG )
#define LLIO_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision$
//C Claude SIMON (http://zeusw.org/intl/contact.html)
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

#ifdef CPE__VC
#	include "wllio.h"
#elif defined( CPE__LINUX )
#	include "pllio.h"
#elif defined( CPE__CYGWIN )
#	include "pllio.h"
#elif defined( CPE__MAC )
//#	include "mllio.h"
#	include "pllio.h"	// Mac suit la norme POSIX.
#else
#	error "Undefiend platform !"
#endif

namespace llio {
#ifdef CPE__VC
	using namespace wllio;
#	define LLIO_UNDEFINED_DESCRIPTOR	WLLIO_UNDEFINED_DESCRIPTOR
#elif defined( CPE__LINUX ) || defined( CPE__CYGWIN ) || defined( CPE__MAC )
	using namespace pllio;
#	define LLIO_UNDEFINED_DESCRIPTOR	PLLIO_UNDEFINED_DESCRIPTOR
#else
#	error
#endif
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
