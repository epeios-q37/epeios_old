/*
  'aem' library by Claude L. Simon (simon@epeios.org)
  Requires the 'aem' header file ('aem.h').
  Copyright (C) 2000 Claude L. SIMON (simon@epeios.org).

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

#define AEM__COMPILATION

#include "aem.h"

class aemtutor
: public ttr_tutor
{
public:
	aemtutor( void )
	: ttr_tutor( AEM_NAME )
	{
#ifdef AEM_DBG
		Version = AEM_VERSION "\b\bD $";
#else
		Version = AEM_VERSION;
#endif
		Owner = AEM_OWNER;
		Date = "$Date$";
	}
	virtual ~aemtutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

namespace aem {
}

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class aempersonnalization
: public aemtutor
{
public:
	aempersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~aempersonnalization( void )
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

static aempersonnalization Tutor;

ttr_tutor &AEMTutor = Tutor;
