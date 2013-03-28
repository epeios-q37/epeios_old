/*
	Header for the 'csdsnb' library by Claude SIMON (csimon at zeusw dot org)
	Copyright (C) $COPYRIGHT_DATES$Claude SIMON.
$_RAW_$
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

#ifndef CSDSNB__INC
#define CSDSNB__INC

#define CSDSNB_NAME		"CSDSNB"

#define	CSDSNB_VERSION	"$Revision$"

#define CSDSNB_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &CSDSNBTutor;

#if defined( XXX_DBG ) && !defined( CSDSNB_NODBG )
#define CSDSNB_DBG
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
//D Client-Server Standard Network Base 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#	include "err.h"
#	include "flw.h"
#	include "dtfptb.h"

#	define CSDSNB_PING	BSO_U32_MAX
#	define CSDSNB_CLOSE	( CSDSNB_PING - 1 )
#	define CSDSNB_UNDEFINED	( CSDSNB_CLOSE - 1 )
#	define CSDSNB_RESERVED	CSDSNB_UNDEFINED

namespace csdsnb {
	typedef bso::u32__ id__;

	inline void PutId(
		id__ Id,
		flw::oflow__ &Flow )
	{
		dtfptb::FittedPutU32( Id, Flow );
	}

	inline id__ GetId( flw::iflow__ &Flow )
	{
		return dtfptb::FittedGetU32( Flow );
	}
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
