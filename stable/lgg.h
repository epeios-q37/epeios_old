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
	1.0.2 02/07/2000 11:51:31

Instigators:
	Claude SIMON

Authors:
	Claude SIMON

Contributors:
	/

Coordinator:
	Claude SIMON

Description:
	LanGuaGe
*/

/* Begin of automatic documentation generation part. */

//V 1.0.2
//C Claude SIMON
//D LanGuaGe
//R 02/07/2000 11:51:31

/* End of automatic documentation generation part. */

/* end of 'entete.h' template file */
/* begin of 'xxx.h' template file V2.6 */

#ifndef LGG__INC
#define LGG__INC

#define LGG_NAME		"LGG"

#define	LGG_VERSION		"1.0.2"	
#define LGG_OWNER		"The Epeios Project (www.epeios.org)"	

//  	Substitution is not applied to the rest when emancipated

#ifdef LGG__LINE
#line 14
#endif

#include "ttr.h"

extern class ttr_tutor &LGGTutor;

#if defined( XXX_DBG ) && !defined( LGG_NODBG )
#define LGG_DBG 
#endif

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#include "err.h"

struct lgg {
	//e Languages.
	enum language {
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
};

extern const char *LGGLanguageName[lgg::l_amount];

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
/* end of 'xxx.h' template file */