/* begin of 'entete.txt' template file V1.1 */
/* Best viewed with a tab size of 4 */
/*
	This file is part of the Epeios project. For all information
	concerning the Epeios project, this file and its conditions of use,
	consult the site: 

			http://www.epeios.org/
*/
/* end of 'entete.txt' template file */
/* begin of 'entete.h' template file V1.3 */
/*
Version:
	1.0.3 06/02/2000 17:19:01

Instigators:
	Claude SIMON

Authors:
	Claude SIMON

Contributors:
	/

Coordinator:
	Claude SIMON

Description:
	EPeioS MiSC
*/

/* Begin of automatic documentation generation part. */

//V 1.0.3
//C Claude SIMON
//D EPeioS MiSC
//R 06/02/2000 17:19:01

/* End of automatic documentation generation part. */

/* end of 'entete.h' template file */
/* begin of 'xxx.h' template file V2.5 */

#ifndef EPSMSC__INC
#define EPSMSC__INC

#define EPSMSC_NAME		"EPSMSC"

#define	EPSMSC_VERSION		"1.0.3"	
#define EPSMSC_OWNER		"The Epeios Project (www.epeios.org)"	

//  	Substitution is not applied to the rest when emancipated

#ifdef EPSMSC__LINE
#line 14
#endif

#include "ttr.h"

extern class ttr_tutor &EPSMSCTutor;

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#include "err.h"
#include "flw.h"

//d The epeios site URL.
#define EPSMSC_EPEIOS_URL	"http://www.epeios.org/"

//d The hypertext link to the Epeios project.
#define EPSMSC_EPEIOS_LINK	"<A HREF=\"" EPSMSC_EPEIOS_URL "\">Epeios project</A>"

//m The hypertext link to the software named 'Name'.
#define EPSMSC_APP_LINK( Name )\
	"<A HREF=\"" EPSMSC_EPEIOS_URL "en/" Name ".html\" TARGET=\"_blank\">" Name "</A>"

//m The invitation to consult the help text for application named 'Nam'.
#define EPSMSC_HELP_INVITATION( Name )	"Try '" Name " --version' for more informations."

//d The author.
#define EPSMSC_AUTHOR	"Claude L. SIMON"

//d Author e-mail
#define EPSMSC_AUTHOR_EMAIL	"simon@epeios.org"

//d Author mail hypertext link.
#define EPSMSC_AUTHOR_LINK	"<A HREF=\"mailto://" EPSMSC_AUTHOR_EMAIL "\">" EPSMSC_AUTHOR "</A>"

//d GNU Text.
#define EPSMSC_GNU_TEXT "\tThis program is part of the GNU project, released\n\tunder the aegis of GNU (http://www.gnu.org/)."

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
/* end of 'xxx.h' template file */