/*
	'bts' library by Claude SIMON (csimon@epeios.org)
	Requires the 'bts' header file ('bts.h').
	Copyright (C) $COPYRIGHT_DATES$Claude SIMON (csimon@epeios.org).
$_RAW_$
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

#define BTS__COMPILATION

#include "bts.h"

class btstutor
: public ttr_tutor
{
public:
	btstutor( void )
	: ttr_tutor( BTS_NAME )
	{
#ifdef BTS_DBG
		Version = BTS_VERSION "\b\bD $";
#else
		Version = BTS_VERSION;
#endif
		Owner = BTS_OWNER;
		Date = "$Date$";
	}
	virtual ~btstutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace bts;

#if 0
_row__ _binary_tree_seeker__::NodeWithoutLeft_( _row__ Node )
{
	while( Tree_->HasLeft( Node ) )
		Node = *Tree_->Left( Node );

	return Node;
}

_row__ _binary_tree_seeker__::First_( _row__ Node )
{
	Node = NodeWithoutLeft_( Node );

	while( Tree_->HasRight( Node ) )
		Node = NodeWithoutLeft_( *Tree_->Right( Node ) );

	return Node;
}
#endif

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class btspersonnalization
: public btstutor
{
public:
	btspersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~btspersonnalization( void )
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

static btspersonnalization Tutor;

ttr_tutor &BTSTutor = Tutor;
