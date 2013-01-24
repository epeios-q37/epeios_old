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

# define AMM_DSIZE_SIZE_MAX ((8*sizeof( mdr::size__))/7+1)

namespace amm {
	typedef mdr::row_t__ descriptor__;

	// 'Dynamic size' : taile de taille variable.
	typedef mdr::datum__ dsize__[AMM_DSIZE_SIZE_MAX];

	// 'static size' : taille de taille fixe.
	typedef mdr::size__ ssize__;

	typedef bso::ubyte__ flags__;

	struct xssize__
	{
		ssize__ Size;
		flags__ Flags;
		xssize__( void )
		{
			Size  = 0;
			Flags = 0;
		}
	};


	// 'extended synamix size' : les deux premiers bits sont des drapeax.
	struct xdsize__
	{
		dsize__ Size;
		bso::ubyte__ Length;
		xdsize__( void )
		{
			memset( Size, 0, AMM_DSIZE_SIZE_MAX );
			Length = 0;
		}
	} xsize__;

	xssize__ Decode( dsize__ XDSize );

	xdsize__ Encode(
		dsize__ Size,
		flags__ Flags );

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
