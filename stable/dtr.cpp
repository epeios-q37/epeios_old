/*
  'dtr' library by Claude L. Simon (simon@epeios.org)
  Requires the 'dtr' header file ('dtr.h').
  Copyright (C) 2000,2001 Claude L. SIMON (simon@epeios.org).

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
  along with this program; if not, go to http://www.fsf.org/
  or write to the:
  
                        Free Software Foundation, Inc.,
           59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

//	$Id$

#define DTR__COMPILATION

#include "dtr.h"

class dtrtutor
: public ttr_tutor
{
public:
	dtrtutor( void )
	: ttr_tutor( DTR_NAME )
	{
#ifdef DTR_DBG
		Version = DTR_VERSION "\b\bD $";
#else
		Version = DTR_VERSION;
#endif
		Owner = DTR_OWNER;
		Date = "$Date$";
	}
	virtual ~dtrtutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class dtrpersonnalization
: public dtrtutor
{
public:
	dtrpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~dtrpersonnalization( void )
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

static dtrpersonnalization Tutor;

ttr_tutor &DTRTutor = Tutor;
