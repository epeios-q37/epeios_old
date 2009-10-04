/*
  'sph' library by Claude L. Simon (epeios@epeios.org)
  Requires the 'sph' header file ('sph.h').
  Copyright (C) 2000 Claude L. Simon.

  This file is part of the Epeios project (http://www.epeios.org/).
  

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 3
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

#define SPH__COMPILATION

#include "sph.h"

class sphtutor
: public ttr_tutor
{
public:
	sphtutor( void )
	: ttr_tutor( SPH_NAME )
	{
#ifdef SPH_DBG
		Version = SPH_VERSION " (DBG)";
#else
		Version = SPH_VERSION;
#endif
		Owner = SPH_OWNER;
		Date = "$Date$";
	}
	virtual ~sphtutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#ifdef SPH__CUSTOM
// Boucle jusqu'� ce que le s�maphore indique la disponibilit�.
void sph__semaphore_::AttendreDisponibilite_( void )
{
	do
	{
		Sleep( 0 );
	}
	while ( !AllouerSemaphore_() );
}
#endif



/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */
class sphpersonnalization
: public sphtutor
{
public:
	sphpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~sphpersonnalization( void )
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

static sphpersonnalization Tutor;

ttr_tutor &SPHTutor = Tutor;
