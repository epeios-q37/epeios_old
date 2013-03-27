/*
	Header for the 'fblber' library by Claude SIMON (csimon at zeusw dot org)
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

#ifndef FBLBER__INC
#define FBLBER__INC

#define FBLBER_NAME		"FBLBER"

#define	FBLBER_VERSION	"$Revision$"

#define FBLBER_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &FBLBERTutor;

#if defined( XXX_DBG ) && !defined( FBLBER_NODBG )
#define FBLBER_DBG
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
//D Frontend/Backend Layout Backend Embed Request 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# include "err.h"
# include "flw.h"

# include "fblbrq.h"

namespace fblber {
	using namespace fblbrq;

	class embed_request_functions___
	: public functions__
	{
	protected:
		virtual const void *FBLBRQGet(
			sdr::row__ Row,
			cast__ Cast )
		{
			return Repository_( Row );
		}
		virtual void *FBLBRQPut(
			sdr::row__ Row,
			cast__ Cast )
		{
			return Repository_( Row );
		}
		virtual void FBLBRQPop(
			flw::iflow__ &Flow,
			const casts_ &Casts )
		{
			Repository_.Init();
			_GetAll( Flow, Casts );
		}
		virtual void FBLBRQPush(
			flw::oflow__ &Flow,
			const casts_ &Casts )
		{
			// Rien � faire ...
		}
	private:
		bch::E_BUNCH( void *)Repository_;
		void _GetAll(
			flw::iflow__ &Flow,
			const casts_ &Casts );
	public:
		void reset( bso::bool__ P = true )
		{
			functions__::reset( P );
			Repository_.reset( P );
		}
		void Init( void )
		{
			Repository_.Init();
			functions__::Init();
		}
	};
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
