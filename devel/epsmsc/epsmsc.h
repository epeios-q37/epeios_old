/*
  Header for the 'epsmsc' library by Claude L. Simon (csimon@epeios.org)
  Copyright (C) 2000 Claude L. SIMON (csimon@epeios.org) 

  This file is part of the Epeios (http://www.epeios.org/) project.
  

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.
 
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, go to http://www.fsf.org or write to the
  
                        Free Software Foundation, Inc.,
           59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

//	$Id$

#ifndef EPSMSC__INC
#define EPSMSC__INC

#define EPSMSC_NAME		"EPSMSC"

#define	EPSMSC_VERSION	"$Revision$"	

#define EPSMSC_OWNER		"Claude SIMON (csimon@epeios.org)"

#include "ttr.h"

extern class ttr_tutor &EPSMSCTutor;

#if defined( XXX_DBG ) && !defined( EPSMSC_NODBG )
#define EPSMSC_DBG 
#endif

/* Begin of automatic documentation generation part. */

//V $Revision$
//C Claude L. SIMON (csimon@epeios.org)
//R $Date$

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#include "err.h"
#include "flw.h"

//d The epeios site URL.
#define EPSMSC_EPEIOS_URL	"http://epeios.org/"

//d The gnu URL.
#define EPSMSC_GNU_URL	"http://www.gnu.org/"

//d The hypertext link to the Epeios project.
#define EPSMSC_EPEIOS_LINK	"<A HREF=\"" EPSMSC_EPEIOS_URL "\">Epeios project</A>"

//m The hypertext link to the software named 'Name'.
#define EPSMSC_APP_LINK( Name )\
	"<A HREF=\"" EPSMSC_EPEIOS_URL "en/" Name ".html\" TARGET=\"_blank\">" Name "</A>"

//m The invitation to consult the help text for application named 'Name'.
#define EPSMSC_HELP_INVITATION( Name )	"Try '" Name " --help' for more informations."

//d The author.
#define EPSMSC_AUTHOR_NAME	"Claude SIMON"

//d Author e-mail
#define EPSMSC_AUTHOR_EMAIL	"csimon@epeios.org"

//d Author mail hypertext link.
#define EPSMSC_AUTHOR_LINK	"<A HREF=\"mailto://" EPSMSC_AUTHOR_EMAIL "\">" EPSMSC_AUTHOR_NAME "</A>"

//d GNU (http://www.gnu.org) Text.
#define EPSMSC_GNU_TEXT "\tThis program is part of the GNU project, released\n\tunder the aegis of GNU (" EPSMSC_GNU_URL ").\n\tIt uses Epeios (" EPSMSC_EPEIOS_URL ") technologies."

//d Epeios (http://epeios.org) Text.
#define EPSMSC_EPEIOS_TEXT "\tThis program is part of the Epeios project (" EPSMSC_EPEIOS_URL ")." 

							
namespace epsmsc {
	//f Print information about GNU GPL license.
	void PrintLicense( void );
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
