/*
	'mmg' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Requires the 'mmg' header file ('mmg.h').
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

#define MMG__COMPILATION

#include "mmg.h"

class mmgtutor
: public ttr_tutor
{
public:
	mmgtutor( void )
	: ttr_tutor( MMG_NAME )
	{
#ifdef MMG_DBG
		Version = MMG_VERSION "\b\bD $";
#else
		Version = MMG_VERSION;
#endif
		Owner = MMG_OWNER;
		Date = "$Date$";
	}
	virtual ~mmgtutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */
class mmgpersonnalization
: public mmgtutor
{
public:
	mmgpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~mmgpersonnalization( void )
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

static mmgpersonnalization Tutor;

ttr_tutor &MMGTutor = Tutor;
