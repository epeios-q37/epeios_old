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
	See the 'idxbtq.h' file  for more details.
*/

#define IDXBTQ__COMPILATION

#include "idxbtq.h"

#ifdef IDXBTQ__LINE
#line 12
#endif

class idxbtqtutor
: public ttr_tutor
{
public:
	idxbtqtutor( void )
	: ttr_tutor( IDXBTQ_NAME )
	{
#ifdef IDXBTQ_DBG
		Version = IDXBTQ_VERSION " (DBG)";
#else
		Version = IDXBTQ_VERSION;
#endif
		Owner = IDXBTQ_OWNER;
		Date = "24/05/2000";
	}
	virtual ~idxbtqtutor( void ){}
};

//  	Substitution is not applied to the rest.
/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */
class idxbtqpersonnalization
: public idxbtqtutor
{
public:
	idxbtqpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~idxbtqpersonnalization( void )
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

static idxbtqpersonnalization Tutor;

ttr_tutor &IDXBTQTutor = Tutor;

/* end of 'xxx.cpp' template file */