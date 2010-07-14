/*
	'bkdlac' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'bkdlac' header file ('bkdlac.h').
	Copyright (C) 2004 Claude SIMON.

	This file is part of the Epeios (http://zeusw.org/epeios/) project.

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

#define BKDLAC__COMPILATION

#include "bkdlac.h"

class bkdlactutor
: public ttr_tutor
{
public:
	bkdlactutor( void )
	: ttr_tutor( BKDLAC_NAME )
	{
#ifdef BKDLAC_DBG
		Version = BKDLAC_VERSION "\b\bD $";
#else
		Version = BKDLAC_VERSION;
#endif
		Owner = BKDLAC_OWNER;
		Date = "$Date$";
	}
	virtual ~bkdlactutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace bkdlac;

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class bkdlacpersonnalization
: public bkdlactutor
{
public:
	bkdlacpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~bkdlacpersonnalization( void )
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

static bkdlacpersonnalization Tutor;

ttr_tutor &BKDLACTutor = Tutor;
