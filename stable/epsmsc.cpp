/* begin of 'entete.txt' template file V1.1 */
/* Best viewed with a tab size of 4 */
/*
	This file is part of the Epeios project. For all information
	concerning the Epeios project, this file and its conditions of use,
	consult the site: 

			http://www.epeios.org/
*/
/* end of 'entete.txt' template file */
/* begin of 'xxx.cpp' template file V2.5 */
/* 
	Pour plus de détails, consultez le fichier 'epsmsc.h'.
	See the 'epsmsc.h' file  for more details.
*/

#define EPSMSC__COMPILATION

#include "epsmsc.h"

#ifdef EPSMSC__LINE
#line 12
#endif

class epsmsctutor
: public ttr_tutor
{
public:
	epsmsctutor( void )
	: ttr_tutor( EPSMSC_NAME )
	{
		Version = EPSMSC_VERSION;
		Owner = EPSMSC_OWNER;
		Date = "06/02/2000 17:19:01";
	}
	virtual ~epsmsctutor( void ){}
};

//  	Substitution is not applied to the rest.
/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class epsmscpersonnalization
: public epsmsctutor
{
public:
	epsmscpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~epsmscpersonnalization( void )
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

static epsmscpersonnalization Tutor;

ttr_tutor &EPSMSCTutor = Tutor;

/* end of 'xxx.cpp' template file */