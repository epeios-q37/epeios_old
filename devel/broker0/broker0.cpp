/*
  'broker0' library by Claude L. Simon (epeios@epeios.org)
  Requires the 'broker0' header file ('broker0.h').
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

#define BROKER0__COMPILATION

#include "broker0.h"

class broker0tutor
: public ttr_tutor
{
public:
	broker0tutor( void )
	: ttr_tutor( BROKER0_NAME )
	{
#ifdef BROKER0_DBG
		Version = BROKER0_VERSION " (DBG)";
#else
		Version = BROKER0_VERSION;
#endif
		Owner = BROKER0_OWNER;
		Date = "$Date$";
	}
	virtual ~broker0tutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace broker0;

// MUST reflect 'broker0::cast'
const char *broker0::CastsNames[c_amount] = {
	"_end_", "Object", "Type", "Casts", "Command", "Boolean", "Id.8", "Id.16", "Id.32", 
	"Char.", "String", "Byte", "Binary", "Pointer", "Multi." };

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class broker0personnalization
: public broker0tutor
{
public:
	broker0personnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~broker0personnalization( void )
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

static broker0personnalization Tutor;

ttr_tutor &BROKER0Tutor = Tutor;
