/*
	Header for the 'iodef' library by Claude SIMON (csimon@epeios.org)
	Copyright (C) 2004 Claude SIMON (csimon@epeios.org).

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

#ifndef IODEF__INC
#define IODEF__INC

#define IODEF_NAME		"IODEF"

#define	IODEF_VERSION	"$Revision$"

#define IODEF_OWNER		"Claude SIMON (csimon@epeios.org)"

#include "ttr.h"

extern class ttr_tutor &IODEFTutor;

#if defined( XXX_DBG ) && !defined( IODEF_NODBG )
#define IODEF_DBG
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
//D Input/Output DEFinitions. 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#error "Obsolete"

#include "cpe.h"

namespace iodef {
	//e Open mode.
	enum mode__ {
		//i Unknow.
		mUnknow,
		//i Ecrase le contenu du fichier.
		mRemove,
		//i Ajoute à la fin du fichier.
		mAppend,
		//i Ouvre le fichier ne lecture/écriture.
		mReadWrite,
		//i Ouvre le fichier en lecture seule.
		mReadOnly,
		//i Amount of mode.
		m_amount
	};
 	//e Status.
	enum status__ {
		//i Failure.
		sFailure = 0,
		//i Success.
		sSuccess,
		//i Unknow,
		s_Unknow,
		//i Amount of status.
		s_amount
	};
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
