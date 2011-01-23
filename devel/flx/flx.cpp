/*
	'flx' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Requires the 'flx' header file ('flx.h').
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

#define FLX__COMPILATION

#include "flx.h"

class flxtutor
: public ttr_tutor
{
public:
	flxtutor( void )
	: ttr_tutor( FLX_NAME )
	{
#ifdef FLX_DBG
		Version = FLX_VERSION "\b\bD $";
#else
		Version = FLX_VERSION;
#endif
		Owner = FLX_OWNER;
		Date = "$Date$";
	}
	virtual ~flxtutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

flx::dump_oflow_functions___ flx::dump;


/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */
class flxpersonnalization
: public flxtutor
{
public:
	flxpersonnalization( void )
	{
		flx::dump.Init( fwf::ts_Default );
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~flxpersonnalization( void )
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

static flxpersonnalization Tutor;

ttr_tutor &FLXTutor = Tutor;
