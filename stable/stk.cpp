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
	See the 'stk.h' file  for more details.
*/

#define STK__COMPILATION

#include "stk.h"

#ifdef STK__LINE
#line 12
#endif

class stktutor
: public ttr_tutor
{
public:
	stktutor( void )
	: ttr_tutor( STK_NAME )
	{
#ifdef STK_DBG
		Version = STK_VERSION " (DBG)";
#else
		Version = STK_VERSION;
#endif
		Owner = STK_OWNER;
		Date = "24/05/2000";
	}
	virtual ~stktutor( void ){}
};

//  	Substitution is not applied to the rest.
/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */
class stkpersonnalization
: public stktutor
{
public:
	stkpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~stkpersonnalization( void )
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

static stkpersonnalization Tutor;

ttr_tutor &STKTutor = Tutor;

/* end of 'xxx.cpp' template file */