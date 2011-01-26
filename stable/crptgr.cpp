/*
	'crptgr' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'crptgr' header file ('crptgr.h').
	Copyright (C) 20112004 Claude SIMON.

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

#define CRPTGR__COMPILATION

#include "crptgr.h"

class crptgrtutor
: public ttr_tutor
{
public:
	crptgrtutor( void )
	: ttr_tutor( CRPTGR_NAME )
	{
#ifdef CRPTGR_DBG
		Version = CRPTGR_VERSION "\b\bD $";
#else
		Version = CRPTGR_VERSION;
#endif
		Owner = CRPTGR_OWNER;
		Date = "$Date$";
	}
	virtual ~crptgrtutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace crptgr;

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class crptgrpersonnalization
: public crptgrtutor
{
public:
	crptgrpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~crptgrpersonnalization( void )
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

static crptgrpersonnalization Tutor;

ttr_tutor &CRPTGRTutor = Tutor;
