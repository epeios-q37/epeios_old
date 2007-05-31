/*
	Header for the 'mmm' library by Claude SIMON (csimon@epeios.org)
	Copyright (C) 2000-2001, 2004 Claude SIMON (csimon@epeios.org).

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

#ifndef MMM__INC
#define MMM__INC

#define MMM_NAME		"MMM"

#define	MMM_VERSION	"$Revision$"

#define MMM_OWNER		"Claude SIMON (csimon@epeios.org)"

#include "ttr.h"

extern class ttr_tutor &MMMTutor;

#if defined( XXX_DBG ) && !defined( MMM_NODBG )
#define MMM_DBG
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
//D MultiMeMory 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "bso.h"
#include "uym.h"
#include "tol.h"
#include "txf.h"
#include "mmm0.h"

#ifdef MMM__USE_V1
#include "mmm1.h"
#elif defined( MMM__USE_V2 )
#	include "mmm2.h"
#endif

namespace mmm {
	E_AUTO( multimemory )

	inline void multimemory_driver__::Liberer_( void )
	{
		if ( _Descriptor != MMM_UNDEFINED_DESCRIPTOR )
			Multimemoire_->Free( _Descriptor );
	}

	inline void multimemory_driver__::MDRRecall(
		mdr::row_t__ Position,
		mdr::size__ Amount,
		mdr::datum__ *Buffer )
	{
		Multimemoire_->Read( _Descriptor, Position, Amount, Buffer, _Addendum );
	}
	// lit à partir de 'Position' et place dans 'Tampon' 'Nombre' octets;
	inline void multimemory_driver__::MDRStore(
		const mdr::datum__ *Buffer,
		mdr::size__ Amount,
		mdr::row_t__ Position )
	{
		Multimemoire_->Write( Buffer, Amount, _Descriptor, Position, _Addendum );
	}
	// écrit 'Nombre' octets à la position 'Position'
	inline void multimemory_driver__::MDRAllocate( mdr::size__ Size )
	{
		_Descriptor = Multimemoire_->Reallocate( _Descriptor, Size, _Addendum );
	}
	// alloue 'Capacite' octets

	inline void multimemory_driver__::MDRFlush( void )
	{
		if ( Multimemoire_ )
			Multimemoire_->Flush();
	}
}

//d A multimemory.
#define E_MULTIMEMORY_	multimemory_
#define E_MULTIMEMORY	multimemory

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
