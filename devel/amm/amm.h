/*
	Header for the 'amm' library by Claude SIMON (csimon at zeusw dot org)
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

#ifndef AMM__INC
#define AMM__INC

#define AMM_NAME		"AMM"

#define	AMM_VERSION	"$Revision$"

#define AMM_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &AMMTutor;

#if defined( XXX_DBG ) && !defined( AMM_NODBG )
#define AMM_DBG
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
//D Aggregated MeMory 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# include "err.h"
# include "flw.h"

# include "uym.h"

namespace amm {

	enum flag_position__ {
		// Statut du fragment ; 0 : libre (free), 1 : occupé (used).
		fpFragmentStatus,
		// Type d'un fragment libre ; 0 : normal (regular), 1: orphelin (orphan).
		fpFreeFragmentType,
		// Type du predecesseur d'un fragment occupé ; 0 : libre (free) ; 1 : occupé (used).
		fpUsedFragmentPredecessorStatus = fpFreeFragmentType,
		// Signification de la taille embarquée ; 0 : Taille du fragment (fragment size) ; 1 : Longueur de la taille du fragment (fragemnt size length).
		fpEmbeddedSizeMeaning
	};

	enum flag_mask__ {
		fmFragmentStatus = 1 << fpFragmentStatus,
		fmFreeFragmentType = 1 << fpFreeFragmentType,
		fmUsedFragmentPredecessorStatus = 1 << fpUsedFragmentPredecessorStatus,
		fmEmbeddedSizeMeaning = 1 << fpEmbeddedSizeMeaning
	};

	using mdr::size__;

	typedef mdr::datum__ header__;

	inline bso::bool__ IsFragmentUsed( header__ Header )
	{
		return Header & fmFragmentStatus;
	}

	inline bso::bool__ IsFragmentFree( header__ Header )
	{
		return !IsFragmentUsed( Header );
	}

	inline bso::bool__ IsFreeFragmentOrphan( header__ Header )
	{
		if ( !IsFragmentFree( Header ) )
			ERRc();

		return ( Header & fmFreeFragmentType ) != 0;
	}

	inline bso::bool__ IsFreeFragmentRegular( header__ Header )
	{
		return !IsFreeFragmentRegular( Header );
	}

	inline bso::bool__ MeansEmbeddedSizeFragmentSizeLength( header__ Header )
	{
		return ( Header & fmEmbeddedSizeMeaning ) != 0;
	}

	inline bso::bool__ MeansEmbeddedSizeFragmentSize( header__ Header )
	{
		return !MeansEmbeddedSizeFragmentSize( Header );
	}

	inline bso::bool__ MeansUsedFragmentEmbeddedSizeFragmentSize( header__ Header )
	{
		if ( !IsFragmentUsed( Header ) )
			ERRc();

		return MeansEmbeddedSizeFragmentSizeLength( Header );
	}

	inline bso::bool__ MeansUsedFragmentEmbeddedSizeFragmentSizeLength( header__ Header )
	{
		return !MeansUsedFragmentEmbeddedSizeFragmentSize( Header );
	}

	inline bso::bool__ MeansFreeFragmentEmbeddedSizeFragmentSize( header__ Header )
	{
		if ( !IsFragmentFree( Header ) )
			ERRc();

		return MeansEmbeddedSizeFragmentSizeLength( Header );
	}

	inline bso::bool__ MeansFreeFragmentEmbeddedSizeFragmentSizeLength( header__ Header )
	{
		return !MeansFreeFragmentEmbeddedSizeFragmentSize( Header );
	}

	inline size__ GetFragmentSize( header__ Header )
	{
		if ( !amm::MeansEmbeddedSizeFragmentSize( Header ) )
			ERRc();

		return ( Header >> 3 ) + 1;
	}

	inline size__ GetUsedFragmentSize( header__ Header )
	{
		if ( !IsFragmentUsed( Header ) )
			ERRc();

		return GetFragmentSize( Header );
	}

	inline size__ GetFreeFragmentSize(header__ Header )
	{
		if ( !IsFragmentFree( Header ) )
			ERRc();

		return GetFragmentSize( Header );
	}

	typedef bso::ulong__ flength__;	// Longueur de la taille d'une fragment (32 bits) (fragemnt size length).

	inline flength__ GetFragmentSizeLength( header__ Header )
	{
		if ( !amm::MeansEmbeddedSizeFragmentSizeLength( Header ) )
			ERRc();

		return 1 << ( Header >> 3 );
	}

	inline flength__ GetUsedFragmentSizeLength(header__ Header )
	{
		if ( !IsFragmentUsed( Header ) )
			ERRc();

		return GetFragmentSizeLength( Header );
	}

	inline flength__ GetFreeFragmentSizeLength(header__ Header )
	{
		if ( !IsFragmentFree( Header ) )
			ERRc();

		return GetFragmentSizeLength( Header );
	}

	inline void MarkAsFreeFragment( header__ &Header )
	{
		Header &= ~fmFragmentStatus;
	}

	inline void MarkAsUsedFragment( header__ &Header )
	{
		Header |= fmFragmentStatus;
	}

	inline void MarkAsRegularFreeFragment( header__ &Header )
	{
		if (!IsFragmentFree( Header ) )
			ERRc();

		Header &= ~fmFreeFragmentType;
	}

	inline void MarkAsOrphanFreeFragment( header__ &Header )
	{
		if (!IsFragmentFree( Header ) )
			ERRc();

		Header |= fmFreeFragmentType;
	}

	typedef mdr::row_t__ descriptor__;

	class aggregate_memory_
	{
	private:
		mdr::size__ _Size( void )
		{
			return Memory.Size();
		}
		void _AllocateNew( void )
		{
		}
	public:
		uym::untyped_memory_ Memory;
		struct s {
			uym::untyped_memory_::s Memory;
			mdr::row__ FreeFragment;
		} &S_;
		aggregate_memory_( s &S )
		: S_( S ),
		  Memory( S.Memory )
		{}
		void reset( bso::bool__  P = true )
		{
			Memory.reset( P );
			S_.FreeFragment = NONE;
		}
		aggregate_memory_ operator =( const aggregate_memory_ &AM )
		{
			Memory = AM.Memory;
			S_.FreeFragment = NONE;

			return *this;
		}
		void plug( mdr::E_MEMORY_DRIVER__ &MD )
		{
			Memory.plug( MD );
		}
		void Init( void )
		{
			Memory.Init();
			S_.FreeFragment = NONE;
		}
		descriptor__ Allocate( mdr::size__ Size )
		{

		}
	};
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
