/*
  Header for the 'brkcst' library by Claude L. Simon (epeios@epeios.org)
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

#ifndef BRKCST__INC
#define BRKCST__INC

#define BRKCST_NAME		"BRKCST"

#define	BRKCST_VERSION	"$Revision$"	

#define BRKCST_OWNER		"the Epeios project (http://www.epeios.org/)"

#include "ttr.h"

extern class ttr_tutor &BRKCSTTutor;

#if defined( XXX_DBG ) && !defined( BRKCST_NODBG )
#define BRKCST_DBG 
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

/* Addendum to the automatic documentation generation part. */
//D BRoKer CaSTs.
/* End addendum to automatic documentation generation part. */


#include "err.h"
#include "flw.h"

namespace brkcst {
	//e casts.
	enum cast {
		//i End of request or end of parameters list. Value MUST be 0.
		cEnd = 0,
		//i Object ('rqm::object__')'.
		cObject,
		//i Type ('rqm::type__').
		cType,
		//i Set of casts ('SET_( rqm::cast__ )').
		cCasts,
		//i Command ('rqm::command__').
		cCommand,
		//i Boolean ('bso::bool__')
		cBoolean,
		//i Short identifier (8 bits, 'bso::ubyte__').
		cId8,
		//i Medium identifier (16 bits, 'bso::ushort__').
		cId16,
		//i Long identifier (32 bits, 'bso::ulong__').
		cId32,
		//i Character ('bso::char__' ).
		cChar,
		//i String ('str::string_').
		cString,
		/*i A byte, as data common data cluster for all plateform.
		Has no intrensec signification ('bso::raw__'). */
		cByte,
		//i Binary 'SET_( bso::raw__ )'.
		cBinary,
		//i Pointer ('void *').
		cPointer,
		//i An array of casts ending with 'brkcst::cEnd'.
		cArray,
		//i Amount of casts.
		c_amount,
		//i Not a cast, but signalize an invalid cast.
		cInvalid,
	};	// A modification must be reflected by 'CastsNames'.

	//o The casts names.
	extern const char *CastsNames[c_amount];

};

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
