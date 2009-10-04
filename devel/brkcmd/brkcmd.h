/*
  Header for the 'brkcmd' library by Claude L. Simon (csimon@webmails.com)
  Copyright (C) 2000,2001 Claude L. SIMON (csimon@webmails.com) 

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

#ifndef BRKCMD__INC
#define BRKCMD__INC

#define BRKCMD_NAME		"BRKCMD"

#define	BRKCMD_VERSION	"$Revision$"	

#define BRKCMD_OWNER		"the Epeios project (http://zeusw.org/epeios/)"

#include "ttr.h"

extern class ttr_tutor &BRKCMDTutor;

#if defined( XXX_DBG ) && !defined( BRKCMD_NODBG )
#define BRKCMD_DBG 
#endif

/* Begin of automatic documentation generation part. */

//V $Revision$
//C Claude L. SIMON (csimon@webmails.com)
//R $Date$

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

/* Addendum to the automatic documentation generation part. */
//D Obsolete. Use 'BKDCMD' instead.
/* End addendum to automatic documentation generation part. */

#error Obolete. Use 'BKDCMD' instead.

#include "err.h"
#include "flw.h"
#include "brkcst.h"

//d Version of the commands.
#define BRKCMD_COMMANDS_VERSION	"1"

namespace brkcmd {
	using namespace brkcst;

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
	extern const char *CommandsNames[brkcmd::c_amount];

	//o Commands parameters.
	extern const cast *CommandsParameters[brkcmd::c_amount];

};

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
