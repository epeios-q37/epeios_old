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
	See the 'dtr.h' file  for more details.
*/

#define DTR__COMPILATION

#include "dtr.h"

#ifdef DTR__LINE
#line 12
#endif

class dtrtutor
: public ttr_tutor
{
public:
	dtrtutor( void )
	: ttr_tutor( DTR_NAME )
	{
#ifdef DTR_DBG
		Version = DTR_VERSION " (DBG)";
#else
		Version = DTR_VERSION;
#endif
		Owner = DTR_OWNER;
		Date = "09/07/2000";
	}
	virtual ~dtrtutor( void ){}
};

//  	Substitution is not applied to the rest.
/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class dtrpersonnalization
: public dtrtutor
{
public:
	dtrpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~dtrpersonnalization( void )
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

static dtrpersonnalization Tutor;

ttr_tutor &DTRTutor = Tutor;

/* end of 'xxx.cpp' template file */