/*
	Header for the 'fblovl' library by Claude SIMON (csimon at zeusw dot org)
	Copyright (C) 2004 Claude SIMON.

	This file is part of the Epeios (http://zeusw.org/epeios/) project.

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

#ifndef FBLOVL__INC
#define FBLOVL__INC

#define FBLOVL_NAME		"FBLOVL"

#define	FBLOVL_VERSION	"$Revision$"

#define FBLOVL_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &FBLOVLTutor;

#if defined( XXX_DBG ) && !defined( FBLOVL_NODBG )
#define FBLOVL_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision$
//C Claude SIMON (csimon at zeusw dot org)
//R $Date$

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D Frontend/Backend Layout OVerLapping 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "lcl.h"

#define FBLOVL_PROTOCOL_VERSION	"4"

namespace fblovl {

	enum mode__ {
		mEmbedded,
		mRemote,
		m_Amount,
		m_Undefined
	};

	enum reply__ {
		rOK,
		rSoftwareError,	// Lorsque survient un 'ERR[x|X](...)'.
		rRequestError,	// Lorsque survient un problème lors de l'exécution de la requête.
		r_amount,
		r_Undefined
	};

	const char *Label( reply__ Reply );

	inline const char *GetTranslation(
		reply__ Reply,
		const lcl::rack__ &Rack,
		STR_BUFFER___ &Buffer )
	{
		return Rack.GetTranslation( Label( Reply ),  FBLOVL_NAME "_", Buffer );
	}
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
