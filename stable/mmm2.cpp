/*
	'mmm2' library by Claude SIMON (csimon@epeios.org)
	Requires the 'mmm2' header file ('mmm2.h').
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

#define MMM2__COMPILATION

#include "mmm2.h"

class mmm2tutor
: public ttr_tutor
{
public:
	mmm2tutor( void )
	: ttr_tutor( MMM2_NAME )
	{
#ifdef MMM2_DBG
		Version = MMM2_VERSION "\b\bD $";
#else
		Version = MMM2_VERSION;
#endif
		Owner = MMM2_OWNER;
		Date = "$Date$";
	}
	virtual ~mmm2tutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace mmm;

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class mmm2personnalization
: public mmm2tutor
{
public:
	mmm2personnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~mmm2personnalization( void )
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

static mmm2personnalization Tutor;

ttr_tutor &MMM2Tutor = Tutor;
