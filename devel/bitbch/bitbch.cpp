/*
	'bitbch' library by Claude SIMON (csimon@epeios.org)
	Requires the 'bitbch' header file ('bitbch.h').
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

#define BITBCH__COMPILATION

#include "bitbch.h"

class bitbchtutor
: public ttr_tutor
{
public:
	bitbchtutor( void )
	: ttr_tutor( BITBCH_NAME )
	{
#ifdef BITBCH_DBG
		Version = BITBCH_VERSION "\b\bD $";
#else
		Version = BITBCH_VERSION;
#endif
		Owner = BITBCH_OWNER;
		Date = "$Date$";
	}
	virtual ~bitbchtutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace bitbch;

void bitbch::And(
	const E_BIT_BUNCH_ &O1,
	const E_BIT_BUNCH_ &O2,
	E_BIT_BUNCH_ &R )
{
#ifdef BITBCH_DBG
	if ( O1.Amount() != O2.Amount() )
		ERRu();
#endif

	if ( R.Amount() < O1.Amount() )
		R.Allocate( O1.Amount() );

	epeios::row__ Row = O1.First();

	while ( Row != NONE ) {
		R.Store( O1( Row ) && O2( Row ), Row );

		Row = O1.Next( Row );
	}
}

void bitbch::Or(
	const E_BIT_BUNCH_ &O1,
	const E_BIT_BUNCH_ &O2,
	E_BIT_BUNCH_ &R )
{
#ifdef BITBCH_DBG
	if ( O1.Amount() != O2.Amount() )
		ERRu();
#endif

	if ( R.Amount() < O1.Amount() )
		R.Allocate( O1.Amount() );

	epeios::row__ Row = O1.First();

	while ( Row != NONE ) {
		R.Store( O1( Row ) || O2( Row ), Row );

		Row = O1.Next( Row );
	}
}

void bitbch::XOr(
	const E_BIT_BUNCH_ &O1,
	const E_BIT_BUNCH_ &O2,
	E_BIT_BUNCH_ &R )
{
#ifdef BITBCH_DBG
	if ( O1.Amount() != O2.Amount() )
		ERRu();
#endif

	if ( R.Amount() < O1.Amount() )
		R.Allocate( O1.Amount() );

	epeios::row__ Row = O1.First();

	while ( Row != NONE ) {
		R.Store( O1( Row ) != O2( Row ), Row );

		Row = O1.Next( Row );
	}
}

void bitbch::Not(
	const E_BIT_BUNCH_ &O,
	E_BIT_BUNCH_ &R )
{
	if ( R.Amount() < O.Amount() )
		R.Allocate( O.Amount() );

	epeios::row__ Row = O.First();

	while ( Row != NONE ) {
		R.Store( !O( Row ), Row );

		Row = O.Next( Row );
	}
}

txf::text_oflow__ &operator <<(
	txf::text_oflow__ &OStream,
	const E_BIT_BUNCH_ &Bunch )
{
	epeios::row__ Row = Bunch.First();

	while ( Row != NONE ) {
		OStream << (unsigned long)Bunch( Row );

		Row = Bunch.Next( Row );
	}

	return OStream;
}



/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class bitbchpersonnalization
: public bitbchtutor
{
public:
	bitbchpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~bitbchpersonnalization( void )
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

static bitbchpersonnalization Tutor;

ttr_tutor &BITBCHTutor = Tutor;
