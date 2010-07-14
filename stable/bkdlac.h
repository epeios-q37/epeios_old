/*
	Header for the 'bkdlac' library by Claude SIMON (csimon at zeusw dot org)
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

#ifndef BKDLAC__INC
#define BKDLAC__INC

#define BKDLAC_NAME		"BKDLAC"

#define	BKDLAC_VERSION	"$Revision$"

#define BKDLAC_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &BKDLACTutor;

#if defined( XXX_DBG ) && !defined( BKDLAC_NODBG )
#define BKDLAC_DBG
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
//D BacKenD Local ACcess 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "bkdacc.h"

namespace bkdlac {

	class backend_local_access_base__
	{
	public:
		void In(
			bkdcst::cast Cast,
			const void *Pointer,
			flw::ioflow__ &Flow )
		{
			Flow.Put( Cast );

			flw::Put( Pointer, Flow );
		}
		void Out(
			flw::ioflow__ &Flow,
			bkdcst::cast Cast,
			void *Pointer )
		{
			Flow.Put( Cast );

			flw::Put( Pointer, Flow );
		}
		bso::bool__ Handle(
			bso::bool__ Success,
			flw::ioflow__ &Flow )
		{
			return Success;
		}
		void Init( void )
		{
			// Standardization.
		}
	};

	class backend_local_access___
	: public bkdacc::backend_access___
	{
	private:
		backend_local_access_base__ _Base;
	protected:
		virtual void BKDACCIn(
			bkdcst::cast Cast,
			const void *Pointer,
			flw::ioflow__ &Flow )
		{
			_Base.In( Cast, Pointer, Flow );
		}
		virtual void BKDACCOut(
			flw::ioflow__ &Flow,
			bkdcst::cast Cast,
			void *Pointer )
		{
			_Base.Out( Flow, Cast, Pointer );
		}
		virtual bso::bool__ BKDACCHandle(
			bso::bool__ Success,
			flw::ioflow__ &Flow )
		{
			return _Base.Handle( Success, Flow );
		}
		void Init( flw::ioflow__ &Flow )
		{
			_Base.Init();
			backend_access___::Init( Flow );
		}
	};


}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
