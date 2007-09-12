/*
	Header for the 'wintol' library by Claude SIMON (csimon at zeusw dot org)
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

#ifndef WINTOL__INC
#define WINTOL__INC

#define WINTOL_NAME		"WINTOL"

#define	WINTOL_VERSION	"$Revision$"

#define WINTOL_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &WINTOLTutor;

#if defined( XXX_DBG ) && !defined( WINTOL_NODBG )
#define WINTOL_DBG
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
//D WINdows TOols 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"

namespace wintol {

	class service__
	{
	private:
		const char *_Name;
	protected:
		virtual void WINTOLCallback( void ) = 0;
	public:
		void reset( bso::bool__ P = true )
		{
			_Name = NULL;
		}
		void Init( const char *Name )	// WARNING : 'Name' is not duplicated, so you should not change the content.
		{
			_Name = Name;
		}
		void Install( void );
		void Remove( void );
		void Launch( void );
		void Callback( void )
		{
			WINTOLCallback();
		}
	};
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
