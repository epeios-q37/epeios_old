/*
	Header for the 'dtfptb' library by Claude SIMON (csimon@epeios.org)
	Copyright (C) 2004 Claude SIMON (csimon@epeios.org).

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

#ifndef DTFPTB__INC
#define DTFPTB__INC

#define DTFPTB_NAME		"DTFPTB"

#define	DTFPTB_VERSION	"$Revision$"

#define DTFPTB_OWNER		"Claude SIMON (csimon@epeios.org)"

#include "ttr.h"

extern class ttr_tutor &DTFPTBTutor;

#if defined( XXX_DBG ) && !defined( DTFPTB_NODBG )
#define DTFPTB_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision$
//C Claude SIMON (csimon@epeios.org)
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

	template <typename short__> inline void _PutShort(
		short__ Short,
		flw::oflow__ &Flow )
	{
		dtfbsc::PutUByte( Short & 0xff, Flow );
		dtfbsc::PutUByte( Short >> 8, Flow );
	}

	template <typename short__> short__ _GetShort( flw::iflow__ &Flow )
	{
		flw::datum__ Object[sizeof( short__ )];

		Flow.Get( sizeof( Object ), Object );

		return Object[0] | ( Object[1] << 8 );
	}

	//f Put 'SShort' into 'Flow'.
	inline void PutSShort(
		sshort__ SShort,
		flw::oflow__ &Flow )
	{
		_PutShort( SShort, Flow );
	}

	//f Return the signed short stored in 'Flow'.
	inline sshort__ GetSShort( flw::iflow__ &Flow )
	{
		return _GetShort<sshort__>( Flow );
	}

	//f Put 'UShort' into 'Flow'.
	inline void PutUShort(
		ushort__ UShort,
		flw::oflow__ &Flow )
	{
		_PutShort( UShort, Flow );
	}

	//f Return the unsigned short stored in 'Flow'.
	inline ushort__ GetUShort( flw::iflow__ &Flow )
	{
		return _GetShort<ushort__>( Flow );
	}


	template <typename long__> inline void _PutLong(
		long__ Long,
		flw::oflow__ &Flow )
	{
		dtfbsc::PutUByte( (dtfbsc::ubyte__)( Long & 0xff ), Flow );
		dtfbsc::PutUByte( (dtfbsc::ubyte__)( ( Long >> 8 ) & 0xff ), Flow );
		dtfbsc::PutUByte( (dtfbsc::ubyte__)( ( Long >> 16 ) & 0xff ), Flow );
		dtfbsc::PutUByte( (dtfbsc::ubyte__)( Long >> 24 ), Flow );
	}

	template <typename long__> long__ _GetLong( flw::iflow__ &Flow )
	{
		flw::datum__ Object[sizeof( long__ )];

		Flow.Get( sizeof( Object ), Object );

		return (long__)( Object[0] | ( Object[1] << 8 ) | ( Object[2] << 16 ) | ( Object[3] << 24 ) );
	}

	//f Put 'SLong' into 'Flow'.
	inline void PutSShort(
		slong__ SLong,
		flw::oflow__ &Flow )
	{
		_PutLong( SLong, Flow );
	}

	//f Return the signed long stored in 'Flow'.
	inline slong__ GetSLong( flw::iflow__ &Flow )
	{
		return _GetLong<slong__>( Flow );
	}

	//f Put 'ULong' into 'Flow'.
	inline void PutULong(
		ulong__ ULong,
		flw::oflow__ &Flow )
	{
		_PutLong( ULong, Flow );
	}

	//f Return the unsigned long stored in 'Flow'.
	inline ulong__ GetULong( flw::iflow__ &Flow )
	{
		return _GetLong<ulong__>( Flow );
	}

	//f Write 'Size' into 'Flow'.
	void PutSize(
		bso::ulong__ Size,
		flw::oflow__ &Flow );
	
	//f Return size stored in 'Flow'.
	bso::ulong__ GetSize( flw::iflow__ &Flow );
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
