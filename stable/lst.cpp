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
	Pour plus de détails, consultez le fichier 'lst.h'.
	See the 'lst.h' file  for more details.
*/

#define LST__COMPILATION

#include "lst.h"

#ifdef LST__LINE
#line 13
#endif

class lsttutor
: public ttr_tutor
{
public:
	lsttutor( void )
	: ttr_tutor( LST_NAME )
	{
		Version = LST_VERSION;
		Owner = LST_OWNER;
		Date = "24/05/2000";
	}
	virtual ~lsttutor( void ){}
};

//  	Substitution is not applied to the rest.
/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

// Retourne l'élément succédant à 'Element', ou LST_INEXISTANT si inexistant.
POSITION__ lst_list_::Successeur_( POSITION__ Element ) const
{
	SIZE__ Nombre = Capacite_();

	while( ( ++Element < Nombre ) && !Existe_( Element ) );

	if ( Element >= Nombre )
		return NONE;
	else
		return Element;
}

// Retourne l'élément précédent 'Element', ou LST_INEXISTANT si inexistant.
POSITION__ lst_list_::Predecesseur_( POSITION__ Element ) const
{
	bso__bool Trouve = false;

	while( ( Element > 0 ) && !( Trouve = Existe_( --Element ) ) );

	if ( Trouve )
		return Element;
	else
		return NONE;
}

// Retourne vrai si 'Element' existe dans la liste.
bso__bool lst_list_::Existe_( POSITION__ Position ) const
{
	POSITION__ Nombre = Libres.Amount();
	bso__bool Trouve = false;

	while ( Nombre-- && !( Trouve = ( Libres( Nombre ) == Position ) ) );

	return !Trouve;
}




/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */
class lstpersonnalization
: public lsttutor
{
public:
	lstpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~lstpersonnalization( void )
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
static lstpersonnalization Tutor;

ttr_tutor &LSTTutor = Tutor;

/* end of 'xxx.cpp' template file */