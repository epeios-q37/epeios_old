/*
	Header for the 'csdscm' library by Claude SIMON (csimon@epeios.org)
	Copyright (C) $COPYRIGHT_DATES$Claude SIMON (csimon@epeios.org).
$_RAW_$
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

#ifndef CSDSCM__INC
#define CSDSCM__INC

#define CSDSCM_NAME		"CSDSCM"

#define	CSDSCM_VERSION	"$Revision$"

#define CSDSCM_OWNER		"Claude SIMON (csimon@epeios.org)"

#include "ttr.h"

extern class ttr_tutor &CSDSCMTutor;

#if defined( XXX_DBG ) && !defined( CSDSCM_NODBG )
#define CSDSCM_DBG
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
//D Client-Server Devices Server CoMmon 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"

namespace csdscm {
	enum action__ {
		aContinue,
		aStop,
		a_amount,
		a_Undefined
	};

	class user_functions__ {
	protected:
		virtual void *CSDPreProcess( flw::ioflow__ &Flow ) = 0;
		virtual action__ CSDProcess(
			flw::ioflow__ &Flow,
			void *UP ) = 0;
		virtual void CSDPostProcess( void *UP ) = 0;
	public:
		void *PreProcess( flw::ioflow__ &Flow )
		{
			return CSDPreProcess( Flow );
		}
		action__ Process(
			flw::ioflow__ &Flow,
			void *UP )
		{
			return CSDProcess( Flow, UP );
		}
		void PostProcess( void *UP )
		{
			CSDPostProcess( UP );
		}
		void Init( void )
		{
			// A des fins de standardisation.
		}
	};

}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
