/*
	'sdr' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'sdr' header file ('sdr.h').
	Copyright (C) 2013-2004 Claude SIMON.

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

#define SDR__COMPILATION

#include "sdr.h"

class sdrtutor
: public ttr_tutor
{
public:
	sdrtutor( void )
	: ttr_tutor( SDR_NAME )
	{
#ifdef SDR_DBG
		Version = SDR_VERSION "\b\bD $";
#else
		Version = SDR_VERSION;
#endif
		Owner = SDR_OWNER;
		Date = "$Date$";
	}
	virtual ~sdrtutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace sdr;

xsize__ sdr::Convert( size__ Size )
{
	xsize__ XSize;
	length__ Position = SDR__DSIZE_SIZE_MAX - 1;

	XSize._Size[Position] = Size & 0x7f;
	Size >>= 7;

	while ( Size != 0 ) {
		if ( Position-- == 0 )
			ERRc();

		XSize._Size[Position] = ( Size & 0x7f ) | 0x80; 
		Size >>= 7;
	}

	XSize._Length = SDR__DSIZE_SIZE_MAX - Position;

	return XSize;
}

#define LIMIT ( SDR_SIZE_MAX >> 7 )

size__ sdr::Convert(
	const sdr::datum__ *DSize,
	sdr::size__ &Length )
{
	length__ Position = 0;
	size__ Size = 0;

	do {
		if ( Size > LIMIT )
			ERRc();

		Size = ( Size << 7 ) + ( DSize[Position] & 0x7f );
	} while ( DSize[Position++] & 0x80 );

	if ( &Length != NULL )
		Length = Position;

	return Size;
}


/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class sdrpersonnalization
: public sdrtutor
{
public:
	sdrpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~sdrpersonnalization( void )
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

static sdrpersonnalization Tutor;

ttr_tutor &SDRTutor = Tutor;
