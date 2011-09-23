/*
	Header for the 'csdleo' library by Claude SIMON (csimon at zeusw dot org)
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

#ifndef CSDLEO__INC
#define CSDLEO__INC

#define CSDLEO_NAME		"CSDLEO"

#define	CSDLEO_VERSION	"$Revision$"

#define CSDLEO_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &CSDLEOTutor;

#if defined( XXX_DBG ) && !defined( CSDLEO_NODBG )
#define CSDLEO_DBG
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
//D Client-Server Devices Library Embedded Overlapping 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# include "err.h"
# include "flw.h"

namespace csdleo {
	enum action__ {
		aContinue,
		aStop,
		a_amount,
		a_Undefined
	};

	class user_functions__ {
	protected:
		virtual void *CSDLEOPreProcess( const char *Origin ) = 0;
		virtual action__ CSDLEOProcess(
			flw::ioflow__ &Flow,
			void *UP ) = 0;
		virtual void CSDLEOPostProcess( void *UP ) = 0;
		virtual void CSDLEOExit( void ) = 0;	// Appelé lorsque l'on quitte l'application
												// (facilite la mise en oeuvre en tant que service Windows).
	public:
		void reset( bso::bool__ = true )
		{
			// Standardisation.
		}
		user_functions__( void )
		{
			reset( false );
		}
		~user_functions__( void )
		{
			reset();
		}
		void *PreProcess( const char *Origin )
		{
			return CSDLEOPreProcess( Origin );
		}
		action__ Process(
			flw::ioflow__ &Flow,
			void *UP )
		{
			return CSDLEOProcess( Flow, UP );
		}
		void PostProcess( void *UP )
		{
			CSDLEOPostProcess( UP );
		}
		void Exit( void )
		{
			CSDLEOExit();
		}
		void Init( void )
		{
			// Standadisation.
		}
	};

}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
