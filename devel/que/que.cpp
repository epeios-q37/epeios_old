/*
	'que' library by Claude SIMON (csimon@epeios.org)
	Requires the 'que' header file ('que.h').
	Copyright (C) $COPYRIGHT_DATES$Claude SIMON (csimon@epeios.org).
$_RAW_$
	This file is part of the Epeios (http://epeios.org/) project.

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

using namespace que;

void que::links_::Initialize(
	epeios::row_t__ Begin,
	epeios::row_t__ End )
{
	que::link__ L;

	do {
		Store( L, Begin );
	} while( Begin++ < End );
}


void que::Dump_(
		const E_QUEUE_ &Queue,
		E_QSTACK_( epeios::row__ ) &Stack,
		epeios::row__ Begin,
		direction Direction )
{
	if ( Direction == que::dAscending )
		do
		{
			Stack.Push( Begin );
			Begin = Queue.Next( Begin );
		}
		while ( Begin != NONE );
	else
		do
		{
			Stack.Push( Begin );
			Begin = Queue.Previous( Begin );
		}
		while ( Begin != NONE );
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
