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
	See the 'mdr.h' file  for more details.
*/

#define MDR__COMPILATION

#include "mdr.h"

#ifdef MDR__LINE
#line 12
#endif

class mdrtutor
: public ttr_tutor
{
public:
	mdrtutor( void )
	: ttr_tutor( MDR_NAME )
	{
#ifdef MDR_DBG
		Version = MDR_VERSION " (DBG)";
#else
		Version = MDR_VERSION;
#endif
		Owner = MDR_OWNER;
		Date = "16/03/2000";
	}
	virtual ~mdrtutor( void ){}
};

//  	Substitution is not applied to the rest.
/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */
class mdrpersonnalization
: public mdrtutor
{
public:
	mdrpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~mdrpersonnalization( void )
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

static mdrpersonnalization Tutor;

ttr_tutor &MDRTutor = Tutor;

/* end of 'xxx.cpp' template file */