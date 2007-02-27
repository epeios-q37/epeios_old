/*
	'mmm0' library by Claude SIMON (csimon@epeios.org)
	Requires the 'mmm0' header file ('mmm0.h').
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

#define MMM0__COMPILATION

#include "mmm0.h"

class mmm0tutor
: public ttr_tutor
{
public:
	mmm0tutor( void )
	: ttr_tutor( MMM0_NAME )
	{
#ifdef MMM0_DBG
		Version = MMM0_VERSION "\b\bD $";
#else
		Version = MMM0_VERSION;
#endif
		Owner = MMM0_OWNER;
		Date = "$Date$";
	}
	virtual ~mmm0tutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace mmm;

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class mmm0personnalization
: public mmm0tutor
{
public:
	mmm0personnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~mmm0personnalization( void )
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

static mmm0personnalization Tutor;

ttr_tutor &MMM0Tutor = Tutor;
