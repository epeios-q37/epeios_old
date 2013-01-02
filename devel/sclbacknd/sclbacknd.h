/*
	Header for the 'sclbacknd' library by Claude SIMON (csimon at zeusw dot org)
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

#ifndef SCLBACKND__INC
#define SCLBACKND__INC

#define SCLBACKND_NAME		"SCLBACKND"

#define	SCLBACKND_VERSION	"$Revision$"

#define SCLBACKND_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &SCLBACKNDTutor;

#if defined( XXX_DBG ) && !defined( SCLBACKND_NODBG )
#define SCLBACKND_DBG
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
//D SoCLe BACKEND 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"

#include "csdleo.h"

#include "fblbur.h"

namespace sclbacknd {

	const char *GetLanguage( void );

	// A définir par l'utilisateur.
	extern const char *TargetName;

	// A définir par l'utilisateur.
	csdleo::user_functions__ *RetrieveSteering(
		fblbur::mode__ Mode,
		void *UP,
		const lcl::locale_ &Locale );	// To overload !

	// A définir par l'utilisateur.
	void ReleaseSteering( csdleo::user_functions__ *Steering );	// To overload.

	void DisplayModuleClosingMessage( void );

	void DisplayModuleClosedMessage( void );
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
