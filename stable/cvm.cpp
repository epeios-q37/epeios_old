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
	See the 'cvm.h' file  for more details.
*/

#define CVM__COMPILATION

#include "cvm.h"

#ifdef CVM__LINE
#line 12
#endif

class cvmtutor
: public ttr_tutor
{
public:
	cvmtutor( void )
	: ttr_tutor( CVM_NAME )
	{
#ifdef CVM_DBG
		Version = CVM_VERSION " (DBG)";
#else
		Version = CVM_VERSION;
#endif
		Owner = CVM_OWNER;
		Date = "16/03/2000";
	}
	virtual ~cvmtutor( void ){}
};

//  	Substitution is not applied to the rest.
/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */
class cvmpersonnalization
: public cvmtutor
{
public:
	cvmpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~cvmpersonnalization( void )
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

static cvmpersonnalization Tutor;

ttr_tutor &CVMTutor = Tutor;

/* end of 'xxx.cpp' template file */