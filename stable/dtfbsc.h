/*
	Header for the 'dtfbsc' library by Claude SIMON (csimon@epeios.org)
	Copyright (C) 2000-2001, 2003 Claude SIMON (csimon@epeios.org).

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

#ifndef DTFBSC__INC
#define DTFBSC__INC

#define DTFBSC_NAME		"DTFBSC"

#define	DTFBSC_VERSION	"$Revision$"

#define DTFBSC_OWNER		"Claude SIMON (csimon@epeios.org)"

#include "ttr.h"

extern class ttr_tutor &DTFBSCTutor;

#if defined( XXX_DBG ) && !defined( DTFBSC_NODBG )
#define DTFBSC_DBG
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
//D Data transFert BaSiC 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

/* Addendum to the automatic documentation generation part. */
//D Data transFer BaSiC.
/* End addendum to automatic documentation generation part. */


#include "err.h"
#include "flw.h"
#include "bso.h"

namespace dtfbsc {
	//t A byte.
	typedef bso::ubyte__	byte__;

	//t A short.
	typedef bso::ushort__ short__;

	//t A long.
	typedef bso::ulong__ long__;

	// Internal use.	
	template <typename generic> inline generic GenericGet_( flw::iflow___ &Flow )
	{
		return (generic)Flow.Get();
	}
	
	// Internal use.	
	template <typename generic> inline void GenericPut_(
		generic Generic,
		flw::oflow___ &Flow )
	{
		byte__ B = Generic;
	
		FLWPut( B, Flow );
	}

	//f Put 'Byte' in 'Flow'.
	inline void PutByte(
		byte__ Byte,
		flw::oflow___ &Flow )
	{
		flw::Put( Byte, Flow );
	}

	//f Return byte in 'Flow'.
	inline byte__ GetByte( flw::iflow___ &Flow )
	{
		return (byte__)Flow.Get();
	}
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
