/*
  'epsmsc' library by Claude L. Simon (epeios@epeios.org)
  Requires the 'epsmsc' header file ('epsmsc.h').
  Copyright (C) 2000 Claude L. SIMON (epeios@epeios.org).

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
  along with this program; if not, go to http://www.fsf.org or write to the:
  
                        Free Software Foundation, Inc.,
           59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

//	$Id$

#define EPSMSC__COMPILATION

#include "epsmsc.h"

class epsmsctutor
: public ttr_tutor
{
public:
	epsmsctutor( void )
	: ttr_tutor( EPSMSC_NAME )
	{
#ifdef EPSMSC_DBG
		Version = EPSMSC_VERSION " (DBG)";
#else
		Version = EPSMSC_VERSION;
#endif
		Owner = EPSMSC_OWNER;
		Date = "$Date$";
	}
	virtual ~epsmsctutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class epsmscpersonnalization
: public epsmsctutor
{
public:
	epsmscpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~epsmscpersonnalization( void )
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

static epsmscpersonnalization Tutor;

ttr_tutor &EPSMSCTutor = Tutor;
