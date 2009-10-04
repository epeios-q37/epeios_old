/*
  Header for the 'broker1' library by Claude SIMON (http://zeusw.org/intl/contact.html)
  Copyright (C) 2000 Claude SIMON (http://zeusw.org/intl/contact.html) 

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
  along with this program; if not, go to http://www.fsf.org or write to the
  
                        Free Software Foundation, Inc.,
           59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

//	$Id$

#ifndef BROKER1__INC
#define BROKER1__INC

#define BROKER1_NAME		"BROKER1"

#define	BROKER1_VERSION	"$Revision$"	

#define BROKER1_OWNER		"the Epeios project (http://zeusw.org/epeios/)"

#include "ttr.h"

extern class ttr_tutor &BROKER1Tutor;

#if defined( XXX_DBG ) && !defined( BROKER1_NODBG )
#define BROKER1_DBG 
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
/*$BEGIN$*/

#error Obsolete. Use library 'BRKCMD' instead.

#include "err.h"
#include "flw.h"
#include "broker0.h"

namespace broker1 {

	using namespace broker0;

	enum command {
		cSetErrorBreakingStatus,
		cGetErrorBreakingStatus,
		cGetNewObject,
		cGetType,
		cGetCommand,
		cGetCommands,
		cGetTypeAndCommands,
		cRemoveObject,
		cGetLanguagesIDAndName,
		cGetVersion,
		cDisconnect,
		cGetTypesIDAndName,
		cGetCommandsIDAndName,
		cGetParameters,
		c_amount
	};

	//o Commands names.
	extern const char *CommandsNames[broker1::c_amount];

	//o Commands parameters.
	extern const cast *CommandsParameters[broker1::c_amount];
}
		
/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
