/*
	'bch' library by Claude SIMON (csimon@epeios.org)
	Requires the 'bch' header file ('bch.h').
	Copyright (C) 2001-2004 Claude SIMON (csimon@epeios.org).

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

#define BCH__COMPILATION

#include "bch.h"

class bchtutor
: public ttr_tutor
{
public:
	bchtutor( void )
	: ttr_tutor( BCH_NAME )
	{
#ifdef BCH_DBG
		Version = BCH_VERSION "\b\bD $";
#else
		Version = BCH_VERSION;
#endif
		Owner = BCH_OWNER;
		Date = "$Date$";
	}
	virtual ~bchtutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace bch;

void bch::_GetRelations(
	const uym::untyped_memory_ &Sorted,
	const uym::untyped_memory_ &Unsorted,
	epeios::size__ Size,
	epeios::row_t__ Limit,
	epeios::datum__ *Buffer,
	E_BUNCH_( epeios::row__ ) &Relations )
{
	epeios::row_t__ Row = 0;

	while ( Row < Limit ) {
		Sorted.Recall( Row, Size, Buffer );
		Relations.Append( Unsorted.Search( Buffer, Size, 0, Limit ) / Size );

		Row += Size;
	}
}


/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class bchpersonnalization
: public bchtutor
{
public:
	bchpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~bchpersonnalization( void )
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

static bchpersonnalization Tutor;

ttr_tutor &BCHTutor = Tutor;
