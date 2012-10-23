/*
	Header for the 'scllocale' library by Claude SIMON (csimon at zeusw dot org)
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

#ifndef SCLLOCALE__INC
#define SCLLOCALE__INC

#define SCLLOCALE_NAME		"SCLLOCALE"

#define	SCLLOCALE_VERSION	"$Revision$"

#define SCLLOCALE_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &SCLLOCALETutor;

#if defined( XXX_DBG ) && !defined( SCLLOCALE_NODBG )
#define SCLLOCALE_DBG
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
//D SoCLe LOCALE 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# include "err.h"
# include "flw.h"
# include "str.h"
# include "lcl.h"

namespace scllocale {

	const lcl::rack__ &GetRack( void );

	lcl::level__ Load(
		flw::iflow__ &Flow,
		const char *Directory,
		const char *RootPath );

	void SetLanguage( const str::string_ &Language );

	const str::string_ &GetTranslation(
		const char *Message,
		str::string_ &Translation );
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
