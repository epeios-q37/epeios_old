/*
  'lst' library by Claude L. Simon (epeios@epeios.org)
  Requires the 'lst' header file ('lst.h').
  Copyright (C) 2000 Claude L. SIMON (epeios@epeios.org).

  This file is part of the Epeios (http://www.epeios.org/) project.
  

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.
 
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, go to http://www.fsf.org or write to the:
  
                        Free Software Foundation, Inc.,
           59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

//	$Id$

#define LST__COMPILATION

#include "lst.h"

class lsttutor
: public ttr_tutor
{
public:
	lsttutor( void )
	: ttr_tutor( LST_NAME )
	{
#ifdef LST_DBG
		Version = LST_VERSION " (DBG)";
#else
		Version = LST_VERSION;
#endif
		Owner = LST_OWNER;
		Date = "$Date$";
	}
	virtual ~lsttutor( void ){}
};

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
