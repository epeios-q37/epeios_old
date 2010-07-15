/*
	Header for the 'mscmdf' library by Claude SIMON (csimon at zeusw dot org)
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

#ifndef MSCMDF__INC
#define MSCMDF__INC

#define MSCMDF_NAME		"MSCMDF"

#define	MSCMDF_VERSION	"$Revision$"

#define MSCMDF_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &MSCMDFTutor;

#if defined( XXX_DBG ) && !defined( MSCMDF_NODBG )
#define MSCMDF_DBG
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
//D MuSiC MiDi File 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "bso.h"

namespace mscmdf {

	//t The type of a a SMF type.
	typedef bso::ushort__ smf_type__;

	//t Type of the delta time ticks..
	typedef bso::ushort__ delta_time_ticks__;

	//t Type of the track-chunk amount.
	typedef bso::ushort__ track_chunk_amount__;

#define MSCMDF_TRACK_CHUNK_AMOUNT_MAX	BSO_USHORT_MAX

	//c A header chunk.
	struct header_chunk__ {
		smf_type__ SMFType;
		track_chunk_amount__ TrackChunkAmount;
		delta_time_ticks__ DeltaTimeTicks;
	};

	//f Put in 'HeaderChunk' the content of the header chunk.
	bso::bool__ GetHeaderChunk(
		flw::iflow__ &Flow,
		header_chunk__ &HeaderChunk,
		err::handle ErrHandling = err::hUsual );

	void PutHeaderChunk(
		smf_type__ SMFType,
		track_chunk_amount__ TrackChunkAmount,
		delta_time_ticks__ DeltaTimeTicks,
		flw::oflow__ &OFlow );

	inline void PutHeaderChunk(
		const header_chunk__ &HeaderChunk,
		flw::oflow__ &OFlow )
	{
		PutHeaderChunk( HeaderChunk.SMFType, HeaderChunk.TrackChunkAmount, HeaderChunk.DeltaTimeTicks, OFlow );
	}

	//t The type of the size of a track chunk.
	typedef bso::ulong__ track_chunk_size__;

	//f Return the size of the track chunk
	track_chunk_size__  GetTrackChunkSize(
		flw::iflow__ &Flow,
		err::handle ErrHandling = err::hUsual );

	void PutTrackChunkHeader(
		track_chunk_size__ Size,
		flw::oflow__ &OFlow );
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
