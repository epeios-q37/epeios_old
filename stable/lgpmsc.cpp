/*
	'lgpmsc' library by Claude SIMON (csimon@epeios.org)
	Requires the 'lgpmsc' header file ('lgpmsc.h').
	Copyright (C) 2003  Claude SIMON (csimon@epeios.org).

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

#define LGPMSC__COMPILATION

#include "lgpmsc.h"

class lgpmsctutor
: public ttr_tutor
{
public:
	lgpmsctutor( void )
	: ttr_tutor( LGPMSC_NAME )
	{
#ifdef LGPMSC_DBG
		Version = LGPMSC_VERSION "\b\bD $";
#else
		Version = LGPMSC_VERSION;
#endif
		Owner = LGPMSC_OWNER;
		Date = "$Date$";
	}
	virtual ~lgpmsctutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#include "stf.h"

using namespace lgpmsc;
using namespace txf;

void lgpmsc::PrintLicense( void )
{
	fout << "LICENSE A COMPLETER" << nl;
}

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class lgpmscpersonnalization
: public lgpmsctutor
{
public:
	lgpmscpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~lgpmscpersonnalization( void )
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

static lgpmscpersonnalization Tutor;

ttr_tutor &LGPMSCTutor = Tutor;
