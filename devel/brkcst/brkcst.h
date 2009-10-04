/*
  Header for the 'brkcst' library by Claude L. Simon (simon@epeios.org)
  Copyright (C) 2000 Claude L. SIMON (simon@epeios.org) 

  This file is part of the Epeios (http://www.epeios.org/) project.
  

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
//C Claude L. SIMON (simon@epeios.org)
//R $Date$

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

/* Addendum to the automatic documentation generation part. */
//D Obsolete. Use 'BKDCST' instead.
/* End addendum to automatic documentation generation part. */

#error Obsolete. Use 'BKDCST' instead.

//d Version casts version.
#define BRKCST_CASTS_VERSION	"1"

#include "err.h"
#include "flw.h"

namespace brkcst {
	//e casts.
	enum cast {
		//i End of request or end of parameters list.
		cEnd = 0,
		//i Object ('bso::ushort__')'.
		cObject,
		//i Boolean ('bso::bool__')
		cBoolean,
		//i 8 bits identifier ( 'bso::ubyte__' ).
		cId8,
		//i Array of 8 bits identifiers ( 'SET_( bso::ubyte__ )' )
		cIds8,
		//i 16 bits identifier ( 'bso::ushort__' ).
		cId16,
		//i Array of 16 bits identifiers ( 'SET_( bso::ushort__ )' )
		cIds16,
		//i 32 bits identifier ('bso::ulong__').
		cId32,
		//i Array of 32 bits identifiers ( 'SET_( bso::ulong__ )' )
		cIds32,
		//i Character ('bso::char__' ).
		cChar,
		//i String ('str::string_').
		cString,
		//i Array of strings ( 'XMCONTAINER_( str::string_ )' )
		cStrings,
		/*i A byte, as data common data cluster for all plateform.
		Has no intrensec signification ('bso::raw__'). */
		cByte,
		//i Binary 'SET_( bso::raw__ )'.
		cBinary,
		//i Array of binaries ( 'XMCONTAINER_( SET_( bso::raw__ ) )' ).
		cBinaries,
		//i An array of 8 bits items ( 'XMCONTAINER_( { bso::ubyte__, str::string_ } )' ).
		cItems8,
		//i An array of 16 bits items ( 'XMCONTAINER_( { bso::ushort__, str::string_ } )' ).
		cItems16,
		//i An array of 32 bits items ( 'XMCONTAINER_( { bso::ulong__, str::string_ } )' ).
		cItems32,
		// The following casts are used for some special commands.
		//i Array of command descriptions ( 'XCONTAINER_( { str::string_, SET_( bso::ubyte__ ) } )' ).
		cCommandsDetails,
		//i Array of object types and identifiers. ( 'SET_( { bso::ushort__, bso::ushort__ } )' ).
		cObjectsReferences,
		//i Amount of casts.
		c_amount,
		//i Not a cast, but signalize an invalid cast.
		cInvalid,
	};	// A modification must be reflected by 'CastsNames'.

	//o The casts names.
	extern const char *CastsNames[c_amount];

	//f Get id from cast named 'CastName'.
	cast GetID( const char *CastName );

};

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
