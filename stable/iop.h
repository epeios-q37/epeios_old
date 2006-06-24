/*
	Header for the 'iop' library by Claude SIMON (csimon@epeios.org)
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

#ifndef IOP__INC
#define IOP__INC

#define IOP_NAME		"IOP"

#define	IOP_VERSION	"$Revision$"

#define IOP_OWNER		"Claude SIMON (csimon@epeios.org)"

#include "ttr.h"

extern class ttr_tutor &IOPTutor;

#if defined( XXX_DBG ) && !defined( IOP_NODBG )
#define IOP_DBG
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
//D Input/Output Picker 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"

#ifdef IOP_BUFFER_SIZE
#	define IOP__BUFFER_SIZE	IOP_BUFFER_SIZE
#else
#	define IOP__BUFFER_SIZE	1024
#endif

#ifdef IOP_USE_STANDARD_IO
#	define IOP__USE_STANDARD_IO
#elif defined( CPE__MS )
#	define IOP__USE_LOWLEVEL_IO
#elif defined( CPE__UNIX )
#	define IOP__USE_LOWLEVEL_IO
#elif defined( CPE__MAC )
//#	define IOP__USE_LOWLEVEL_IO	// Because not implemented yet for Mac.
#	define	IOP__USE_STANDARD_IO
#else
#	define	IOP__USE_STANDARD_IO
#endif

#ifdef IOP__USE_STANDARD_IO
#	include "cslio.h"
#elif defined( IOP__USE_LOWLEVEL_IO )
#	include "llio.h"
#else
#	error "Undefined I/O enviroment !"
#endif

namespace iop {
#ifdef IOP__USE_LOWLEVEL_IO
	typedef llio::lowlevel_input__		input__;
	typedef llio::lowlevel_output__		output__;
	typedef llio::lowlevel_io__			io__;
	using llio::descriptor__;
	using llio::amount__;
#	define IOP_UNDEFINED_DESCRIPTOR	LLIO_UNDEFINED_DESCRIPTOR
#elif defined( IOP__USE_STANDARD_IO )
	typedef cslio::standard_input__		input__;
	typedef cslio::standard_output__		output__;
	typedef cslio::standard_io__			io__;
	using cslio::descriptor__;
	using cslio::amount__;
#	define IOP_UNDEFINED_DESCRIPTOR	CSLIO_UNDEFINED_DESCRIPTOR
#else
#	error "Undefined I/O enviroment !"
#endif


}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
