/* begin of 'entete.txt' template file V1.1 */
/* Best viewed with a tab size of 4 */
/*
	This file is part of the Epeios project. For all information
	concerning the Epeios project, this file and its conditions of use,
	consult the site: 

			http://www.epeios.org/
*/
/* end of 'entete.txt' template file */
/* begin of 'xxx.cpp' template file V2.21 */
/* 
	Pour plus de détails, consultez le fichier 'fam.h'.
	See the 'fam.h' file  for more details.
*/

#define FAM__COMPILATION

#include "fam.h"

class famtutor
: public ttr_tutor
{
public:
	famtutor( void )
	: ttr_tutor( FAM_NAME )
	{
		Version = FAM_VERSION;
		Owner = FAM_OWNER;
		Date = "27/05/2000";
	}
	virtual ~famtutor( void ){}
};

//  	Substitution is not applied to the rest.
/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

fam_fast_allocation_memory FAM;

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */
class fampersonnalization
: public famtutor
{
public:
	fampersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~fampersonnalization( void )
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
static fampersonnalization Tutor;

ttr_tutor &FAMTutor = Tutor;

/* end of 'xxx.cpp' template file */