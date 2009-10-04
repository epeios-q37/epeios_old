/*
	Header for the 'dtfbsc' library by Claude SIMON (csimon@epeios.org)
	Copyright (C) 2000-2001, 2004 Claude SIMON (csimon@epeios.org).

	This file is part of the Epeios (http://epeios.org/) project.

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
	using bso::ubyte__;
	using bso::sbyte__;
	using bso::ushort__;
	using bso::sshort__;
	using bso::ulong__;
	using bso::slong__;

	//f Put 'UByte' in 'Flow'.
	inline void PutUByte(
		ubyte__ UByte,
		flw::oflow__ &Flow )
	{
		Flow.Put( *(flw::datum__ *)&UByte );
	}

	//f Return unsigned byte in 'Flow'.
	inline ubyte__ GetUByte( flw::iflow__ &Flow )
	{
		return (unsigned char)Flow.Get();
	}

	//f Put 'SByte' in 'Flow'.
	inline void PutSByte(
		sbyte__ SByte,
		flw::oflow__ &Flow )
	{
		Flow.Put( *(flw::datum__ *)&SByte );
	}

	//f Return unsigned byte in 'Flow'.
	inline sbyte__ GetSByte( flw::iflow__ &Flow )
	{
		return (signed char)Flow.Get();
	}

}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
