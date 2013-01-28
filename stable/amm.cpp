/*
	'amm' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'amm' header file ('amm.h').
	Copyright (C) 20132004 Claude SIMON.

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

#define AMM__COMPILATION

#include "amm.h"

class ammtutor
: public ttr_tutor
{
public:
	ammtutor( void )
	: ttr_tutor( AMM_NAME )
	{
#ifdef AMM_DBG
		Version = AMM_VERSION "\b\bD $";
#else
		Version = AMM_VERSION;
#endif
		Owner = AMM_OWNER;
		Date = "$Date$";
	}
	virtual ~ammtutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace amm;

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

xsize__ amm::Convert( mdr::size__ Size )
{
	xsize__ XSize;
	_length__ Position = AMM__DSIZE_SIZE_MAX - 1;

	XSize._Size[Position] = Size & 0x7f;
	Size >>= 7;

	while ( Size != 0 ) {
		if ( Position-- == 0 )
			ERRc();

		XSize._Size[Position] = ( Size & 0x7f ) | 0x80; 
		Size >>= 7;
	}

	XSize.Length = AMM__DSIZE_SIZE_MAX - Position;

	return XSize;
}

#define LIMIT ( MDR_SIZE_MAX >> 7 )

mdr::size__ amm::Convert( const mdr::datum__ *DSize )
{
	_length__ Position = 0;
	mdr::size__ Size = 0;

	{
		if ( Size > LIMIT )
			ERRc();

		Size = Size << ( 7 + DSize[Position] & 0x7f );
	} while ( DSize[Position++] & 0x7f );

	return Size;
}

class ammpersonnalization
: public ammtutor
{
public:
	ammpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~ammpersonnalization( void )
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

static ammpersonnalization Tutor;

ttr_tutor &AMMTutor = Tutor;
