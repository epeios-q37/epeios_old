/*
	Header for the 'lgpmsc' library by Claude SIMON (csimon@epeios.org)
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

#ifndef LGPMSC__INC
#define LGPMSC__INC

#define LGPMSC_NAME		"LGPMSC"

#define	LGPMSC_VERSION	"$Revision$"

#define LGPMSC_OWNER		"Claude SIMON (csimon@epeios.org)"

#include "ttr.h"

extern class ttr_tutor &LGPMSCTutor;

#if defined( XXX_DBG ) && !defined( LGPMSC_NODBG )
#define LGPMSC_DBG
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
//D LoGiPlus (Logi+) MiSC 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"

//d The logiplus site URL.
#define LGPMSC_LOGIPLUS_URL	"http://logiplus.fr/"

//d The hypertext link to the logiplus web site..
#define LGPMSC_LOGIPLUS_LINK	"<A HREF=\"" LGPMSC_LOGIPLUS_URL "\">Logi+</A>"

//m The hypertext link to the software named 'Name'.
#if 0
#define LGPMSC_APP_LINK( Name )\
	"<A HREF=\"" LGPMSC_EPEIOS_URL "en/" Name ".html\" TARGET=\"_blank\">" Name "</A>"
#endif

//m The invitation to consult the help text for application named 'Name'.
#define LGPMSC_HELP_INVITATION( Name )	"Try '" Name " --help' for more informations."

//d The author.
#define LGPMSC_AUTHOR_NAME	"Claude SIMON"

//d Author e-mail
#define LGPMSC_AUTHOR_EMAIL	"develop@logiplus.fr"

//d Author mail hypertext link.
#define LGPMSC_AUTHOR_LINK	"<A HREF=\"mailto://" LGPMSC_AUTHOR_EMAIL "\">" LGPMSC_AUTHOR_NAME "</A>"

//d Logiplus (http://epeios.org) Text.
#define LGPMSC_LOGIPLUS_TEXT "\tThis program was developed for Logi+ (" LGPMSC_LOGIPLUS_URL ")." 

//m Copyright text with yers 'Years' (string) for Logi+.
#define LGPMSC_COPYRIGHT( years )	"Copyright (c) " years " Logi+ (" LGPMSC_LOGIPLUS_URL ")."


namespace lgpmsc {
	//f Print information about GNU GPL license.
	void PrintLicense(
#ifdef CPE__MT
		txf::text_oflow__ &Flow );
#else
	txf::text_oflow__ &Flow = cio::cout );
#endif
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
