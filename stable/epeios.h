/*
	Header for the 'epeios' library by Claude SIMON (csimon@epeios.org)
	Copyright (C) 2001, 2004 Claude SIMON (csimon@epeios.org).

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

#ifndef EPEIOS__INC
#define EPEIOS__INC

#define EPEIOS_NAME		"EPEIOS"

#define	EPEIOS_VERSION	"$Revision$"

#define EPEIOS_OWNER		"Claude SIMON (csimon@epeios.org)"

#include "ttr.h"

extern class ttr_tutor &EPEIOSTutor;

#if defined( XXX_DBG ) && !defined( EPEIOS_NODBG )
#define EPEIOS_DBG
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
//D EPEIOS often used features. 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

/* Addendum to the automatic documentation generation part. */
//D EPEIOS current features.
/* End addendum to automatic documentation generation part. */


#include "err.h"
#include "flw.h"
#include "tol.h"
#include "mdr.h"

namespace epeios {
	typedef mdr::row_t__	row_t__;
	E_TYPEDEF( row_t__, row__ );

	typedef mdr::p_row_t__	p_row_t__;
	E_TYPEDEF( p_row_t__, p_row__ );

	#define EPEIOS_ROW_MAX		MDR_ROW_T_MAX

	using mdr::size__;
	using mdr::p_size__;

	#define EPEIOS_SIZE_MAX		MDR_SIZE_MAX

#ifndef NONE
#	define NONE ((mdr::row_t__)-1)
#endif

	using mdr::datum__;
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
