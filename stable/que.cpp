/*
  'que' library by Claude L. Simon (simon@epeios.org)
  Requires the 'que' header file ('que.h').
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

#define QUE__COMPILATION

#include "que.h"

class quetutor
: public ttr_tutor
{
public:
	quetutor( void )
	: ttr_tutor( QUE_NAME )
	{
#ifdef QUE_DBG
		Version = QUE_VERSION "\b\bD $";
#else
		Version = QUE_VERSION;
#endif
		Owner = QUE_OWNER;
		Date = "$Date$";
	}
	virtual ~quetutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/


void que::queue_::Initialize_(
	tym::row__ Begin,
	tym::row__ End )
{
	que::link__ L;

	do {
		Links.Write( L, Begin );
	} while( Begin++ < End );
}


/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */
class quepersonnalization
: public quetutor
{
public:
	quepersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~quepersonnalization( void )
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

static quepersonnalization Tutor;

ttr_tutor &QUETutor = Tutor;
