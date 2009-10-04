/*
	'llio' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Requires the 'llio' header file ('llio.h').
	Copyright (C) $COPYRIGHT_DATES$Claude SIMON (http://zeusw.org/intl/contact.html).
$_RAW_$
	This file is part of the Epeios (http://zeusw.org/epeios/) project.

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 3
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

#define LLIO__COMPILATION

#include "llio.h"

class lliotutor
: public ttr_tutor
{
public:
	lliotutor( void )
	: ttr_tutor( LLIO_NAME )
	{
#ifdef LLIO_DBG
		Version = LLIO_VERSION "\b\bD $";
#else
		Version = LLIO_VERSION;
#endif
		Owner = LLIO_OWNER;
		Date = "$Date$";
	}
	virtual ~lliotutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace llio;

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class lliopersonnalization
: public lliotutor
{
public:
	lliopersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~lliopersonnalization( void )
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

static lliopersonnalization Tutor;

ttr_tutor &LLIOTutor = Tutor;
