/*
  Header for the 'broker0' library by Claude L. Simon (epeios@epeios.org)
  Copyright (C) 2000 Claude L. SIMON (epeios@epeios.org) 

  This file is part of the Epeios (http://www.epeios.org/) project.
  

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.
 
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, go to http://www.fsf.org or write to the
  
                        Free Software Foundation, Inc.,
           59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

//	$Id$

#ifndef BROKER0__INC
#define BROKER0__INC

#define BROKER0_NAME		"BROKER0"

#define	BROKER0_VERSION	"$Revision$"	

#define BROKER0_OWNER		"the Epeios project (http://www.epeios.org/)"

#include "ttr.h"

extern class ttr_tutor &BROKER0Tutor;

#if defined( XXX_DBG ) && !defined( BROKER0_NODBG )
#define BROKER0_DBG 
#endif

/* Begin of automatic documentation generation part. */

//V $Revision$
//C Claude L. SIMON (epeios@epeios.org)
//R $Date$

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#error Obsolete. Use library 'BRKCST' instead.

#include "err.h"
#include "flw.h"

namespace broker0 {
	//e casts.
	enum cast {
		//i End of request or end of parameters list. Value MUST be 0.
		cEnd = 0,
		//i Object ('rqm__object')'.
		cObject,
		//i Type ('rqm__type').
		cType,
		//i Set of casts ('SET_( rqm__cast )').
		cCasts,
		//i Command ('rqm__command').
		cCommand,
		//i Boolean ('bso__bool')
		cBoolean,
		//i Short identifier (8 bits, 'bso__ubyte').
		cId8,
		//i Medium identifier (16 bits, 'bso__ushort').
		cId16,
		//i Long identifier (32 bits, 'bso__ulong').
		cId32,
		//i Character ('bso__char' ).
		cChar,
		//i String ('str_string_').
		cString,
		/*i A byte, as data common data cluster for all plateform.
		Has no intrensec signification ('bso__raw'). */
		cByte,
		//i Binary 'SET_( bso__raw )'.
		cBinary,
		//i Pointer ('void *').
		cPointer,
		//i An array of casts ending with 'broker0::cEnd'.
		cArray,
		//i Amount of casts.
		c_amount,
		//i Not a cast, but signalize an invalid cast.
		cInvalid,
	};	// A modification must be reflected by 'BROKER0CastsNames'.

	//o The casts names.
	extern const char *CastsNames[c_amount];
	// MUST reflect 'broker0::cast'
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
