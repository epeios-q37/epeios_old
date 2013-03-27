/*
	Header for the 'dtfptb' library by Claude SIMON (csimon at zeusw dot org)
	Copyright (C) 2004 Claude SIMON.

	This file is part of the Epeios (http://zeusw.org/epeios/) project.

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

#ifndef DTFPTB__INC
#define DTFPTB__INC

#define DTFPTB_NAME		"DTFPTB"

#define	DTFPTB_VERSION	"$Revision$"

#define DTFPTB_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &DTFPTBTutor;

#if defined( XXX_DBG ) && !defined( DTFPTB_NODBG )
#define DTFPTB_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision$
//C Claude SIMON (csimon at zeusw dot org)
//R $Date$

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D Data TransFert PorTaBle 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "dtfbsc.h"

namespace dtfptb {
	using namespace dtfbsc;

	template <typename i16__> inline void _FixedPutI16(
		i16__ I16,
		flw::oflow__ &Flow )
	{
		dtfbsc::PutU8( I16 & 0xff, Flow );
		dtfbsc::PutU8( I16 >> 8, Flow );
	}

	template <typename i16__> inline i16__ _FixedGetI16( flw::iflow__ &Flow )
	{
		flw::datum__ Object[sizeof( i16__ )];

		Flow.Read( sizeof( Object ), Object );

		return Object[0] | ( Object[1] << 8 );
	}

	//f Put 'SShort' into 'Flow'.
	inline void FixedPutS16(
		s16__ S16,
		flw::oflow__ &Flow )
	{
		_FixedPutI16( S16, Flow );
	}

	//f Return the signed short stored in 'Flow'.
	inline s16__ FixedGetS16( flw::iflow__ &Flow )
	{
		return _FixedGetI16<s16__>( Flow );
	}

	//f Put 'UShort' into 'Flow'.
	inline void FixedPutU16(
		u16__ U16,
		flw::oflow__ &Flow )
	{
		_FixedPutI16( U16, Flow );
	}

	//f Return the unsigned short stored in 'Flow'.
	inline u16__ FixedGetU16( flw::iflow__ &Flow )
	{
		return _FixedGetI16<u16__>( Flow );
	}


	template <typename i32__> inline void _FixedPutI32(
		i32__ I32,
		flw::oflow__ &Flow )
	{
		dtfbsc::PutU8( (dtfbsc::u8__)( I32 & 0xff ), Flow );
		dtfbsc::PutU8( (dtfbsc::u8__)( ( I32 >> 8 ) & 0xff ), Flow );
		dtfbsc::PutU8( (dtfbsc::u8__)( ( I32 >> 16 ) & 0xff ), Flow );
		dtfbsc::PutU8( (dtfbsc::u8__)( I32 >> 24 ), Flow );
	}

	template <typename i32__> i32__ _FixedGetI32( flw::iflow__ &Flow )
	{
		flw::datum__ Object[sizeof( i32__ )];

		Flow.Read( sizeof( Object ), Object );

		return (i32__)( Object[0] | ( Object[1] << 8 ) | ( Object[2] << 16 ) | ( Object[3] << 24 ) );
	}

	//f Put 'SLong' into 'Flow'.
	inline void FixedPutS32(
		s32__ S32,
		flw::oflow__ &Flow )
	{
		_FixedPutI32( S32, Flow );
	}

	//f Return the signed long stored in 'Flow'.
	inline s32__ FixedGetS32( flw::iflow__ &Flow )
	{
		return _FixedGetI32<s32__>( Flow );
	}

	//f Put 'ULong' into 'Flow'.
	inline void FixedPutU32(
		u32__ U32,
		flw::oflow__ &Flow )
	{
		_FixedPutI32( U32, Flow );
	}

	//f Return the unsigned long stored in 'Flow'.
	inline u32__ FixedGetU32( flw::iflow__ &Flow )
	{
		return _FixedGetI32<u32__>( Flow );
	}

	void FittedPutU32(
		bso::u32__ U32,
		flw::oflow__ &Flow );

	bso::u32__ FittedGetU32( flw::iflow__ &Flow );

	inline void FittedPutS32(
		bso::s32__ S32,
		flw::oflow__ &Flow )
	{
		FittedPutU32( (bso::u32__ )S32, Flow );
	}

	inline bso::s32__ FittedGetS32( flw::iflow__ &Flow )
	{
		return (bso::s32__)FittedGetU32( Flow );
	}

	inline void FittedPutU16(
		bso::u16__ &U16,
		flw::oflow__ &Flow )
	{
		FittedPutU32( U16, Flow );
	}

	inline bso::u16__ FittedGetU16( flw::iflow__ &Flow )
	{
		bso::u32__ U32 = FittedGetU32( Flow );

		if ( U32 > BSO_U16_MAX )
			ERRc();

		return (bso::u16__)U32;
	}

	inline void FittedPutSHort(
		bso::s16__ S16,
		flw::oflow__ &Flow )
	{
		FittedPutU32( (bso::u16__ )S16, Flow );
	}

	inline bso::s16__ FittedGet16( flw::iflow__ &Flow )
	{
		return (bso::s16__)FittedGetU16( Flow );
	}

	typedef bso::raw__ size_buffer__[10];

	#define DTFPTB_L1	255
	#define DTFPTB_L2	65535
	#define DTFPTB_L3	16777215

	inline bso::u8__ OldGetSizeLength( bso::u32__ Size )
	{
		if ( Size >= ( DTFPTB_L3 + DTFPTB_L2 + DTFPTB_L1 ) )
			return 10;
		else if ( Size >= ( DTFPTB_L2 + DTFPTB_L1 ) )
			return 6;
		else if ( Size >= DTFPTB_L1 )
			return 3;
		else
			return 1;
	}

	namespace {
		inline bso::bool__ Fits_(
			bso::u32__ Size,
			bso::u8__ Level )
		{
			return ( Size < ( 1UL << ( Level * 7 ) ) );
		}
	};

#	define DTFPTB__TEST( l )	if ( Test_( Size, l ) ) return l;

	inline bso::u8__ NewGetSizeLength( bso::u32__ Size )
	{
		bso::u8__ Counter = 1;

		while ( !Fits_( Size, Counter ) ) {
#	ifdef DTFPTB_DBG
			if ( Counter >= BSO_U8_MAX )
				ERRc();
#endif
			Counter++;
		}

		return Counter;
	}

	void OldPutSize(
		bso::u32__ Size,
		size_buffer__ &Buffer );
/*
	inline void NewPutSize(
		bso::u32__ Size,
		size_buffer__ &Buffer );
*/
	void OldPutSize(
		bso::u32__ Size,
		flw::oflow__ &Flow );
	
	inline void NewPutSize(
		bso::u32__ Size,
		flw::oflow__ &Flow )
	{
		FittedPutU32( Size, Flow );
	}

	
	bso::u32__ OldGetSize( flw::iflow__ &Flow );

	inline bso::u32__ NewGetSize( flw::iflow__ &Flow )
	{
		return FittedGetU32( Flow );
	}

	bso::u32__ OldGetSize( const size_buffer__ &Buffer );
//	bso::u32__ NewGetSize( const size_buffer__ &Buffer );

}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
