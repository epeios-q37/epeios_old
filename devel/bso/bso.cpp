/*
	'bso' library by Claude SIMON (csimon@epeios.org)
	Requires the 'bso' header file ('bso.h').
	Copyright (C) $COPYRIGHT_DATES$Claude SIMON (csimon@epeios.org).
$_RAW_$
	This file is part of the Epeios (http://epeios.org/) project.

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.
 
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, go to http://www.fsf.org/
	or write to the:
  
         	         Free Software Foundation, Inc.,
           59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/



//	$Id$

#define BSO__COMPILATION

#include "bso.h"

class bsotutor
: public ttr_tutor
{
public:
	bsotutor( void )
	: ttr_tutor( BSO_NAME )
	{
#ifdef BSO_DBG
		Version = BSO_VERSION "\b\bD $";
#else
		Version = BSO_VERSION;
#endif
		Owner = BSO_OWNER;
		Date = "$Date$";
	}
	virtual ~bsotutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */
class bsopersonnalization
: public bsotutor
{
public:
	bsopersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */

		if ( sizeof( bso::size__ ) != 4 )
			ERRc(); //If this error occurs, then the definition of 'p_size__' must be modified.

		if ( sizeof( bso::size__ ) != sizeof( int ) )
			ERRc();
	}
	~bsopersonnalization( void )
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

static bsopersonnalization Tutor;

ttr_tutor &BSOTutor = Tutor;
