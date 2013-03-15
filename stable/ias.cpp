/*
	'ias' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'ias' header file ('ias.h').
	Copyright (C) 2004 Claude SIMON.

	This file is part of the Epeios (http://zeusw.org/epeios/) project.

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

#define IAS__COMPILATION

#include "ias.h"

class iastutor
: public ttr_tutor
{
public:
	iastutor( void )
	: ttr_tutor( IAS_NAME )
	{
#ifdef IAS_DBG
		Version = IAS_VERSION "\b\bD $";
#else
		Version = IAS_VERSION;
#endif
		Owner = IAS_OWNER;
		Date = "$Date$";
	}
	virtual ~iastutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace ias;

void indexed_aggregated_storage_::AllouerMoins_(
	sdr::size__ CapaciteCourante,
	sdr::size__ CapaciteDemandee,
	aem::mode__ Mode )
{
	while ( CapaciteCourante-- > CapaciteDemandee )
		AStorage.Free( Descriptors.Get( CapaciteCourante ) );

	Descriptors.Allocate( CapaciteDemandee, Mode );
}

void indexed_aggregated_storage_::RemoveWithoutReallocating(
	sdr::row__ Position,
	sdr::size__ ActualCapacity,
	sdr::size__ Amount )
{
	sdr::size__ Counter = Amount;

	while( Counter-- )
		AStorage.Free( Descriptors.Get( *Position + Counter ) );

	Descriptors.Store( Descriptors, ActualCapacity - *Position - Amount, *Position, *Position + Amount );

	Initialize_( ActualCapacity - Amount, ActualCapacity );
}


/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class iaspersonnalization
: public iastutor
{
public:
	iaspersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~iaspersonnalization( void )
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

static iaspersonnalization Tutor;

ttr_tutor &IASTutor = Tutor;
