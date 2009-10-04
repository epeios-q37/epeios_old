/*
	Header for the 'csddlc' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Copyright (C) 2004 Claude SIMON (http://zeusw.org/intl/contact.html).

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

#ifndef CSDDLC__INC
#define CSDDLC__INC

#define CSDDLC_NAME		"CSDDLC"

#define	CSDDLC_VERSION	"$Revision$"

#define CSDDLC_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"

#include "ttr.h"

extern class ttr_tutor &CSDDLCTutor;

#if defined( XXX_DBG ) && !defined( CSDDLC_NODBG )
#define CSDDLC_DBG
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

/* Addendum to the automatic documentation generation part. */
//D Client-Server Devices Dynamic Library Client 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "csdscm.h"
#include "csdebd.h"

namespace csddlc {

	class dynamic_library_client_core
	{
	private:
		csdscm::user_functions__ *_UserFunctions;
		void *_LibraryHandler;
	public:
		void reset( bso::bool__ P = true );
		dynamic_library_client_core( void )
		{
			reset( false );
		}
		~dynamic_library_client_core( void )
		{
			reset();
		}
		bso::bool__ Init(
			const char *LibraryName,
			void *UP );
		friend class dynamic_library_client;
	};

	
	class dynamic_library_client
	: public csdebd::embed_client_server
	{
	public:
		void Init( dynamic_library_client_core &Core )
		{
			if ( Core._UserFunctions == NULL )
				ERRu();

			embed_client_server::Init( *Core._UserFunctions );
		}
	};
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
