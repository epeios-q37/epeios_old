/*
  'bitmmr' library by Claude L. Simon (simon@epeios.org)
  Requires the 'bitmmr' header file ('bitmmr.h').
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

#define BITMMR__COMPILATION

#include "bitmmr.h"

class bitmmrtutor
: public ttr_tutor
{
public:
	bitmmrtutor( void )
	: ttr_tutor( BITMMR_NAME )
	{
#ifdef BITMMR_DBG
		Version = BITMMR_VERSION "\b\bD $";
#else
		Version = BITMMR_VERSION;
#endif
		Owner = BITMMR_OWNER;
		Date = "$Date$";
	}
	virtual ~bitmmrtutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

namespace bitmmr {
}

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class bitmmrpersonnalization
: public bitmmrtutor
{
public:
	bitmmrpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~bitmmrpersonnalization( void )
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

static bitmmrpersonnalization Tutor;

ttr_tutor &BITMMRTutor = Tutor;
