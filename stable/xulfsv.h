/*
	Header for the 'xulfsv' library by Claude SIMON (csimon at zeusw dot org)
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

#ifndef XULFSV__INC
#define XULFSV__INC

#define XULFSV_NAME		"XULFSV"

#define	XULFSV_VERSION	"$Revision$"

#define XULFSV_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &XULFSVTutor;

#if defined( XXX_DBG ) && !defined( XULFSV_NODBG )
#define XULFSV_DBG
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
//D XUL Frontend Session View 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# include "xulfbs.h"
# include "frdkrn.h"

# define XULFSV_PAGE_ID	"pgeSessionView"

namespace xulfsv {
	using namespace xulfbs;

	struct session_view__
	: public ui_core__<xulftk::trunk___>
	{
	public:
		struct broadcasters__
		{} &Broadcasters;
		struct event_handlers__
		{} &EventHandlers;
		struct widgets__
		{} &Widgets;
		session_view__(
			broadcasters__ &Broadcasters,
			event_handlers__ &EventHandlers,
			widgets__ &Widgets )
		: Broadcasters( Broadcasters ),
		  EventHandlers( EventHandlers ),
		  Widgets( Widgets )
		{}
		void Update( frdkrn::backend_extended_type__ Type = frdkrn::bxt_Undefined );
	};

	void RegisterSessionViewUI(
		xulftk::trunk___ &Trunk,
		nsIDOMWindow *Window );
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
