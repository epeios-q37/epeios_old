/*
	Header for the 'lgg' library by Claude SIMON (csimon@epeios.org)
	Copyright (C) 2000-2002  Claude SIMON (csimon@epeios.org).

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

#ifndef LGG__INC
#define LGG__INC

#define LGG_NAME		"LGG"

#define	LGG_VERSION	"$Revision$"

#define LGG_OWNER		"Claude SIMON (csimon@epeios.org)"

#include "ttr.h"

extern class ttr_tutor &LGGTutor;

#if defined( XXX_DBG ) && !defined( LGG_NODBG )
#define LGG_DBG
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
//D LanGuaGes 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"

namespace lgg {
	//e Languages.
	enum language__ {
		//i Unknow language,
		lUnknow,
		//i Default language.
		lDefault,
		//i French.
		lFrench,
		//i English.
		lEnglish,
		//i German
		lGerman,
		//i Amount of languages.
		l_amount
	};

	extern const char *LanguageNames[lgg::l_amount];

	//f Return the ID of language named 'Name'.
	language__ GetID( const char *Name );

}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
