/*
	'dtfptb' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'dtfptb' header file ('dtfptb.h').
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

#define DTFPTB__COMPILATION

#include "dtfptb.h"

class dtfptbtutor
: public ttr_tutor
{
public:
	dtfptbtutor( void )
	: ttr_tutor( DTFPTB_NAME )
	{
#ifdef DTFPTB_DBG
		Version = DTFPTB_VERSION "\b\bD $";
#else
		Version = DTFPTB_VERSION;
#endif
		Owner = DTFPTB_OWNER;
		Date = "$Date$";
	}
	virtual ~dtfptbtutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace dtfptb;

#define M1	DTFPTB_L1
#define M2	DTFPTB_L2
#define M3	DTFPTB_L3


void dtfptb::PutSize(
	bso::ulong__ Size,
	size_buffer__ &Buffer )
{
	bso::raw__ *Pointer = Buffer;

	if ( Size >= M1 )
	{
		*(Pointer++) = M1;
		Size -= M1;

		if ( Size >= M2 )
		{
			*(Pointer++) = M1;
			*(Pointer++) = M1;
			Size -= M2;

			if ( Size >= M3 )
			{
				*(Pointer++) = M1;
				*(Pointer++) = M1;
				*(Pointer++) = M1;

				Size -= M3;

				*(Pointer++) = (bso::raw__)( ( Size & ( 255 << 24 ) ) >> 24 );
			}

			*(Pointer++) = (bso::raw__)( ( Size & ( 255 << 16 ) ) >> 16 );
		}

		*(Pointer++) = (bso::raw__)( ( Size & ( 255 << 8 ) ) >> 8 );
	}

	*(Pointer++) = (bso::raw__)( Size & 255 );
}


#if 0	// Version originale.
void dtfptb::PutSize(
	bso::ulong__ Size,
	flw::oflow__ &Flow )
{
	if ( Size >= M1 )
	{
		Flow.Put( (flw::datum__)M1 );
		Size -= M1;

		if ( Size >= M2 )
		{
			Flow.Put( (flw::datum__)M1 );
			Flow.Put( (flw::datum__)M1 );
			Size -= M2;

			if ( Size >= M3 )
			{
				Flow.Put( (flw::datum__)M1 );
				Flow.Put( (flw::datum__)M1 );
				Flow.Put( (flw::datum__)M1 );

				Size -= M3;

				Flow.Put( (flw::datum__)( ( Size & ( 255 << 24 ) ) >> 24 ) );
			}
			Flow.Put( (flw::datum__)( ( Size & ( 255 << 16 ) ) >> 16 ) );
		}
		Flow.Put( (flw::datum__)( ( Size & ( 255 << 8 ) ) >> 8 ) );
	}
	Flow.Put( (flw::datum__)( Size & 255 ) );
}
#else
void dtfptb::PutSize(
	bso::ulong__ Size,
	flw::oflow__ &Flow )
{
	size_buffer__ Buffer;

	PutSize( Size, Buffer );

	Flow.Write( Buffer, GetSizeLength( Size ) );
}
#endif

bso::ulong__ dtfptb::GetSize( flw::iflow__ &IFlow )
{
	bso::ulong__ Size = (bso::ubyte__)IFlow.Get();

	if ( Size == M1 )
	{
		flw::datum__ Data[4];

		IFlow.Read( 2, Data );

		Size += ( Data[0] << 8 ) | Data[1];

		if ( Size == ( M1 + M2 ) )
		{
			IFlow.Read( 3, Data );

			Size += ( Data[0] << 16 ) | ( Data[1] << 8 ) | Data[2];

			if ( Size == ( M1 + M2 + M3 ) ) {

				IFlow.Read( 4, Data );

				Size += ( Data[0] << 24 ) | ( Data[1] << 16 ) | ( Data[2] << 8 ) | Data[3];
			}
		}
	}

	return Size;
}

bso::ulong__ dtfptb::GetSize( const size_buffer__ &Buffer )
{
	bso::ulong__ Size = Buffer[0];

	if ( Size == M1 )
	{
		Size += ( Buffer[1] << 8 ) | Buffer[2];

		if ( Size == ( M1 + M2 ) )
		{
			Size += ( Buffer[3] << 16 ) | ( Buffer[4] << 8 ) | Buffer[5];

			if ( Size == ( M1 + M2 + M3 ) ) {

				Size += ( Buffer[6] << 24 ) | ( Buffer[7] << 16 ) | ( Buffer[8] << 8 ) | Buffer[9];
			}
		}
	}

	return Size;
}

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class dtfptbpersonnalization
: public dtfptbtutor
{
public:
	dtfptbpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~dtfptbpersonnalization( void )
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

static dtfptbpersonnalization Tutor;

ttr_tutor &DTFPTBTutor = Tutor;
