/* begin of 'entete.txt' template file V1.1 */
/* Best viewed with a tab size of 4 */
/*
	This file is part of the Epeios project. For all information
	concerning the Epeios project, this file and its conditions of use,
	consult the site: 

			http://www.epeios.org/
*/
/* end of 'entete.txt' template file */
/* begin of 'xxx.cpp' template file V2.3 */
/* 
	Pour plus de détails, consultez le fichier 'idxque.h'.
	See the 'idxque.h' file  for more details.
*/

#define IDXQUE__COMPILATION

#include "idxque.h"

#ifdef IDXQUE__LINE
#line 13
#endif

class idxquetutor
: public ttr_tutor
{
public:
	idxquetutor( void )
	: ttr_tutor( IDXQUE_NAME )
	{
		Version = IDXQUE_VERSION;
		Owner = IDXQUE_OWNER;
		Date = "26/04/2000";
	}
	virtual ~idxquetutor( void ){}
};

//  	Substitution is not applied to the rest.
/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/


/* Remplit la pile 'Pile' avec la file à partir de 'Debut' dans le
sens 'Sens'. */
void idxque_queue_index_::Dump(
	stk_stack_<POSITION__> &Pile,
	POSITION__ Debut,
	idxque::direction Direction )
{
	if ( Direction == idxque::dAscending )
		do
		{
			Pile.Push( Debut );
			Debut = Next( Debut );
		}
		while ( Debut != NONE );
	else
		do
		{
			Pile.Push( Debut );
			Debut = Previous( Debut );
		}
		while ( Debut != NONE );
}


/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */
class idxquepersonnalization
: public idxquetutor
{
public:
	idxquepersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~idxquepersonnalization( void )
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
static idxquepersonnalization Tutor;

ttr_tutor &IDXQUETutor = Tutor;

/* end of 'xxx.cpp' template file */