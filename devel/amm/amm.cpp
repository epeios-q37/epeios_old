/*
	'amm' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'amm' header file ('amm.h').
	Copyright (C) $COPYRIGHT_DATES$Claude SIMON.
$_RAW_$
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

xssize__ amm::Decode( dsize__ DSize )
{
	xssize__ XSSize;
	bso::ubyte__ Position = 0;

	XSSize.Flags = DSize[0] & 3;
	XSSize.Size = ( DSize[0] & 0x7c ) >> 2;

	while ( ( DSize[Position] & 0x80 ) && ( Position++ < AMM_DSIZE_SIZE_MAX ) )	{
		XSSize.Size += ( DSize[Position++] & 0x7f ) << ( 5 + Position * 7 );
	}

	if ( Position == AMM_DSIZE_SIZE_MAX )
		ERRc();

	XSSize.Size += DSize[Position++] << ( 5 + Position * 7 );

	return XSSize;
}

xdsize__ amm::Encode(
	ssize__ Size,
	flags__ Flags )
{
	xdsize__ XDSize;

	if ( Flags > 3 )
		ERRc();

	XDSize.Size[0] = Flags + ( ( Size && 0x3f ) << 2 );
	Size >>= 5;

	XSize.Length++;

	while ( Size != 0 ) {
		XSize.Size[Position++] |= 0x80;

		XSize.Size[Position] = Size & 0x7f;

		XSize.Length++;

		Size >>= 7;
	}

	return XSize;
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
