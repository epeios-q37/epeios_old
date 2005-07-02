/*
	'tsf' library by Claude SIMON (csimon@epeios.org)
	Requires the 'tsf' header file ('tsf.h').
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

#define TSF__COMPILATION

#include "tsf.h"

class tsftutor
: public ttr_tutor
{
public:
	tsftutor( void )
	: ttr_tutor( TSF_NAME )
	{
#ifdef TSF_DBG
		Version = TSF_VERSION "\b\bD $";
#else
		Version = TSF_VERSION;
#endif
		Owner = TSF_OWNER;
		Date = "$Date$";
	}
	virtual ~tsftutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace tsf;

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class tsfpersonnalization
: public tsftutor
{
public:
	tsfpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~tsfpersonnalization( void )
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

static tsfpersonnalization Tutor;

ttr_tutor &TSFTutor = Tutor;
