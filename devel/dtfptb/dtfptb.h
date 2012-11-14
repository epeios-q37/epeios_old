/*
	Header for the 'dtfptb' library by Claude SIMON (csimon at zeusw dot org)
	Copyright (C) $COPYRIGHT_DATES$Claude SIMON.
$_RAW_$
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

	template <typename short__> inline void _FixedPutShort(
		short__ Short,
		flw::oflow__ &Flow )
	{
		dtfbsc::PutUByte( Short & 0xff, Flow );
		dtfbsc::PutUByte( Short >> 8, Flow );
	}

	template <typename short__> inline short__ _FixedGetShort( flw::iflow__ &Flow )
	{
		flw::datum__ Object[sizeof( short__ )];

		Flow.Read( sizeof( Object ), Object );

		return Object[0] | ( Object[1] << 8 );
	}

	//f Put 'SShort' into 'Flow'.
	inline void FixedPutSShort(
		sshort__ SShort,
		flw::oflow__ &Flow )
	{
		_FixedPutShort( SShort, Flow );
	}

	//f Return the signed short stored in 'Flow'.
	inline sshort__ FixedGetSShort( flw::iflow__ &Flow )
	{
		return _FixedGetShort<sshort__>( Flow );
	}

	//f Put 'UShort' into 'Flow'.
	inline void FixedPutUShort(
		ushort__ UShort,
		flw::oflow__ &Flow )
	{
		_FixedPutShort( UShort, Flow );
	}

	//f Return the unsigned short stored in 'Flow'.
	inline ushort__ FixedGetUShort( flw::iflow__ &Flow )
	{
		return _FixedGetShort<ushort__>( Flow );
	}


	template <typename long__> inline void _FixedPutLong(
		long__ Long,
		flw::oflow__ &Flow )
	{
		dtfbsc::PutUByte( (dtfbsc::ubyte__)( Long & 0xff ), Flow );
		dtfbsc::PutUByte( (dtfbsc::ubyte__)( ( Long >> 8 ) & 0xff ), Flow );
		dtfbsc::PutUByte( (dtfbsc::ubyte__)( ( Long >> 16 ) & 0xff ), Flow );
		dtfbsc::PutUByte( (dtfbsc::ubyte__)( Long >> 24 ), Flow );
	}

	template <typename long__> long__ _FixedGetLong( flw::iflow__ &Flow )
	{
		flw::datum__ Object[sizeof( long__ )];

		Flow.Read( sizeof( Object ), Object );

		return (long__)( Object[0] | ( Object[1] << 8 ) | ( Object[2] << 16 ) | ( Object[3] << 24 ) );
	}

	//f Put 'SLong' into 'Flow'.
	inline void FixedPutSLong(
		slong__ SLong,
		flw::oflow__ &Flow )
	{
		_FixedPutLong( SLong, Flow );
	}

	//f Return the signed long stored in 'Flow'.
	inline slong__ FixedGetSLong( flw::iflow__ &Flow )
	{
		return _FixedGetLong<slong__>( Flow );
	}

	//f Put 'ULong' into 'Flow'.
	inline void FixedPutULong(
		ulong__ ULong,
		flw::oflow__ &Flow )
	{
		_FixedPutLong( ULong, Flow );
	}

	//f Return the unsigned long stored in 'Flow'.
	inline ulong__ FixedGetULong( flw::iflow__ &Flow )
	{
		return _FixedGetLong<ulong__>( Flow );
	}

	void FittedPutULong(
		bso::ulong__ ULong,
		flw::oflow__ &Flow );

	bso::ulong__ FittedGetULong( flw::iflow__ &Flow );

	inline void FittedPutSLong(
		bso::slong__ SLong,
		flw::oflow__ &Flow )
	{
		FittedPutULong( (bso::ulong__ )SLong, Flow );
	}

	inline bso::slong__ FittedGetSLong( flw::iflow__ &Flow )
	{
		return (bso::slong__)FittedGetULong( Flow );
	}

	inline void FittedPutUShort(
		bso::ushort__ &UShort,
		flw::oflow__ &Flow )
	{
		FittedPutULong( UShort, Flow );
	}

	inline bso::ushort__ FittedGetUShort( flw::iflow__ &Flow )
	{
		bso::ulong__ ULong = FittedGetULong( Flow );

		if ( ULong > BSO_USHORT_MAX )
			ERRc();

		return (bso::ushort__)ULong;
	}

	inline void FittedPutSHort(
		bso::sshort__ SShort,
		flw::oflow__ &Flow )
	{
		FittedPutULong( (bso::ushort__ )SShort, Flow );
	}

	inline bso::sshort__ FittedGetShort( flw::iflow__ &Flow )
	{
		return (bso::sshort__)FittedGetUShort( Flow );
	}

	typedef bso::raw__ size_buffer__[10];

	#define DTFPTB_L1	255
	#define DTFPTB_L2	65535
	#define DTFPTB_L3	16777215

	inline bso::ubyte__ OldGetSizeLength( bso::ulong__ Size )
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
			bso::ulong__ Size,
			bso::ubyte__ Level )
		{
			return ( Size < ( 1UL << ( Level * 7 ) ) );
		}
	};

#	define DTFPTB__TEST( l )	if ( Test_( Size, l ) ) return l;

	inline bso::ubyte__ NewGetSizeLength( bso::ulong__ Size )
	{
		bso::ubyte__ Counter = 1;

		while ( !Fits_( Size, Counter ) ) {
#	ifdef DTFPTB_DBG
			if ( Counter >= BSO_UBYTE_MAX )
				ERRc();
#endif
			Counter++;
		}

		return Counter;
	}

	void OldPutSize(
		bso::ulong__ Size,
		size_buffer__ &Buffer );
/*
	inline void NewPutSize(
		bso::ulong__ Size,
		size_buffer__ &Buffer );
*/
	void OldPutSize(
		bso::ulong__ Size,
		flw::oflow__ &Flow );
	
	inline void NewPutSize(
		bso::ulong__ Size,
		flw::oflow__ &Flow )
	{
		FittedPutULong( Size, Flow );
	}

	
	bso::ulong__ OldGetSize( flw::iflow__ &Flow );

	inline bso::ulong__ NewGetSize( flw::iflow__ &Flow )
	{
		return FittedGetULong( Flow );
	}

	bso::ulong__ OldGetSize( const size_buffer__ &Buffer );
//	bso::ulong__ NewGetSize( const size_buffer__ &Buffer );

}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
