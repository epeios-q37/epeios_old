/*
	'lstbch' library by Claude SIMON (csimon@epeios.org)
	Requires the 'lstbch' header file ('lstbch.h').
	Copyright (C) 2004 Claude SIMON (csimon@epeios.org).

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

#define LSTBCH__COMPILATION

#include "lstbch.h"

class lstbchtutor
: public ttr_tutor
{
public:
	lstbchtutor( void )
	: ttr_tutor( LSTBCH_NAME )
	{
#ifdef LSTBCH_DBG
		Version = LSTBCH_VERSION "\b\bD $";
#else
		Version = LSTBCH_VERSION;
#endif
		Owner = LSTBCH_OWNER;
		Date = "$Date$";
	}
	virtual ~lstbchtutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace lstbch;

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class lstbchpersonnalization
: public lstbchtutor
{
public:
	lstbchpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~lstbchpersonnalization( void )
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

static lstbchpersonnalization Tutor;

ttr_tutor &LSTBCHTutor = Tutor;
