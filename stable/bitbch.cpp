/*
	'bitbch' library by Claude SIMON (csimon@epeios.org)
	Requires the 'bitbch' header file ('bitbch.h').
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

#include "txf.h"

using namespace bitbch;

#ifdef BITBCH_BUFFER_SIZE
#	define BUFFER_SIZE	BITBCH_BUFFER_SIZE
#else
#	define BUFFER_SIZE	5000
#endif

#if 0
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

	epeios::row_t__ Row = 0;
	epeios::size__ Limit = ( O1.Amount() == 0 ? 0 : ( O1.Amount() - 1 ) / BITBCH__RECEPTACLE_SIZE_IN_BITS + 1 );

	while ( Row < Limit ) {
		R.Table.Store( O1.Table( Row ) & O2.Table( Row ), Row );

		Row++;
	}
}
#else

static inline void And_(
	const receptacle__ *O1,
	const receptacle__ *O2,
	epeios::size__ Amount,
	receptacle__ *R )
{
	while ( Amount-- )
		R[Amount] = O1[Amount] & O2[Amount];
}

void bitbch::And(
	const E_BIT_BUNCH_ &O1,
	const E_BIT_BUNCH_ &O2,
	E_BIT_BUNCH_ &R )
{
#ifdef BITBCH_DBG
	if ( O1.Amount() != O2.Amount() )
		ERRu();
#endif

	receptacle__ Buffer[3 * BUFFER_SIZE];
	epeios::size__ Size = ( O1.Amount() == 0 ? 0 : ( O1.Amount() - 1 ) / BITBCH__RECEPTACLE_SIZE_IN_BITS + 1 );
	epeios::size__ Current = 0;
	epeios::size__ Amount = BUFFER_SIZE;

	if ( R.Amount() < O1.Amount() )
		R.Allocate( O1.Amount() );

	while ( Current < Size ) {
		if ( ( Size - Current ) < Amount )
			Amount = Size - Current;

		O1.Table.Recall( Current, Amount, Buffer );
		O2.Table.Recall( Current, Amount, Buffer + BUFFER_SIZE);

		And_( Buffer, Buffer + BUFFER_SIZE, Amount, Buffer + 2 * BUFFER_SIZE );

		R.Table.Store( Buffer + 2 * BUFFER_SIZE, Amount, Current );

		Current += Amount;
	}
}

#if 0
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

	epeios::row_t__ Row = 0;
	epeios::size__ Limit = ( O1.Amount() == 0 ? 0 : ( O1.Amount() - 1 ) / BITBCH_NB_BITS_RECEPTACLE + 1 );

	while ( Row < Limit ) {
		R.Table.Store( O1.Table( Row ) | O2.Table( Row ), Row );

		Row++;
	}
}
#else

static inline void Or_(
	const receptacle__ *O1,
	const receptacle__ *O2,
	epeios::size__ Amount,
	receptacle__ *R )
{
	while ( Amount-- )
		R[Amount] = O1[Amount] | O2[Amount];
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

	receptacle__ Buffer[3 * BUFFER_SIZE];
	epeios::size__ Size = ( O1.Amount() == 0 ? 0 : ( O1.Amount() - 1 ) / BITBCH__RECEPTACLE_SIZE_IN_BITS + 1 );
	epeios::size__ Current = 0;
	epeios::size__ Amount = BUFFER_SIZE;

	if ( R.Amount() < O1.Amount() )
		R.Allocate( O1.Amount() );

	while ( Current < Size ) {
		if ( ( Size - Current ) < Amount )
			Amount = Size - Current;

		O1.Table.Recall( Current, Amount, Buffer );
		O2.Table.Recall( Current, Amount, Buffer + BUFFER_SIZE);

		Or_( Buffer, Buffer + BUFFER_SIZE, Amount, Buffer + 2 * BUFFER_SIZE );

		R.Table.Store( Buffer + 2 * BUFFER_SIZE, Amount, Current );

		Current += Amount;
	}
}
#endif

#if 0
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

	epeios::row_t__ Row = 0;
	epeios::size__ Limit = ( O1.Amount() == 0 ? 0 : ( O1.Amount() - 1 ) / BITBCH_NB_BITS_RECEPTACLE + 1 );

	while ( Row < Limit ) {
		R.Table.Store( O1.Table( Row ) ^ O2.Table( Row ), Row );

		Row++;
	}
}
#else

static inline void XOr_(
	const receptacle__ *O1,
	const receptacle__ *O2,
	epeios::size__ Amount,
	receptacle__ *R )
{
	while ( Amount-- )
		R[Amount] = O1[Amount] ^ O2[Amount];
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

	receptacle__ Buffer[3 * BUFFER_SIZE];
	epeios::size__ Size = ( O1.Amount() == 0 ? 0 : ( O1.Amount() - 1 ) / BITBCH__RECEPTACLE_SIZE_IN_BITS + 1 );
	epeios::size__ Current = 0;
	epeios::size__ Amount = BUFFER_SIZE;

	if ( R.Amount() < O1.Amount() )
		R.Allocate( O1.Amount() );

	while ( Current < Size ) {
		if ( ( Size - Current ) < Amount )
			Amount = Size - Current;

		O1.Table.Recall( Current, Amount, Buffer );
		O2.Table.Recall( Current, Amount, Buffer + BUFFER_SIZE);

		XOr_( Buffer, Buffer + BUFFER_SIZE, Amount, Buffer + 2 * BUFFER_SIZE );

		R.Table.Store( Buffer + 2 * BUFFER_SIZE, Amount, Current );

		Current += Amount;
	}
}
#endif

#if 0
void bitbch::Not(
	const E_BIT_BUNCH_ &O,
	E_BIT_BUNCH_ &R )
{
	if ( R.Amount() < O.Amount() )
		R.Allocate( O.Amount() );

	epeios::row_t__ Row = 0;
	epeios::size__ Limit = ( O.Amount() == 0 ? 0 : ( O.Amount() - 1 ) / BITBCH_NB_BITS_RECEPTACLE + 1 );

	if ( R.Amount() < O.Amount() )
		R.Allocate( O.Amount() );

	while ( Row != NONE ) {
		R.Table.Store( ~O.Table( Row ), Row );

		Row++;
	}
}
#else

static inline void Not_(
	const receptacle__ *O,
	epeios::size__ Amount,
	receptacle__ *R )
{
	while ( Amount-- )
		R[Amount] = ~O[Amount];
}

void bitbch::Not(
	const E_BIT_BUNCH_ &O,
	E_BIT_BUNCH_ &R )
{
	receptacle__ Buffer[2 * BUFFER_SIZE];
	epeios::size__ Size = ( O.Amount() == 0 ? 0 : ( O.Amount() - 1 ) / BITBCH__RECEPTACLE_SIZE_IN_BITS + 1 );
	epeios::size__ Current = 0;
	epeios::size__ Amount = BUFFER_SIZE;

	while ( Current < Size ) {
		if ( ( Size - Current ) < Amount )
			Amount = Size - Current;

		O.Table.Recall( Current, Amount, Buffer );

		Not_( Buffer, Amount, Buffer + BUFFER_SIZE );

		R.Table.Store( Buffer + BUFFER_SIZE, Amount, Current );

		Current += Amount;
	}
}
#endif
#endif

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
