/* begin of 'entete.txt' template file V1.1 */
/* Best viewed with a tab size of 4 */
/*
	This file is part of the Epeios project. For all information
	concerning the Epeios project, this file and its conditions of use,
	consult the site: 

			http://www.epeios.org/
*/
/* end of 'entete.txt' template file */
/* begin of 'xxx.cpp' template file V2.8 */
/* 
	See the 'lgg.h' file  for more details.
*/

#define LGG__COMPILATION

#include "lgg.h"

#ifdef LGG__LINE
#line 12
#endif

class lggtutor
: public ttr_tutor
{
public:
	lggtutor( void )
	: ttr_tutor( LGG_NAME )
	{
#ifdef LGG_DBG
		Version = LGG_VERSION " (DBG)";
#else
		Version = LGG_VERSION;
#endif
		Owner = LGG_OWNER;
		Date = "02/07/2000";
	}
	virtual ~lggtutor( void ){}
};

//  	Substitution is not applied to the rest.
/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

const char *LGGLanguageName[lgg::l_amount] = {
	"Default",
	"Français",
	"English",
	"Deutsch" };




/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */
class lggpersonnalization
: public lggtutor
{
public:
	lggpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~lggpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the ending of the application  */
	}
};


/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

// 'static' by GNU C++.

static lggpersonnalization Tutor;

ttr_tutor &LGGTutor = Tutor;

/* end of 'xxx.cpp' template file */