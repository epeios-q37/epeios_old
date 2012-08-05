/*
	Header for the 'xulfsf' library by Claude SIMON (csimon at zeusw dot org)
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

#ifndef XULFSF__INC
#define XULFSF__INC

#define XULFSF_NAME		"XULFSF"

#define	XULFSF_VERSION	"$Revision$"

#define XULFSF_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &XULFSFTutor;

#if defined( XXX_DBG ) && !defined( XULFSF_NODBG )
#define XULFSF_DBG
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
//D XUL Frontend Session Form 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# include "err.h"
# include "flw.h"

# include "xulfbs.h"
# include "frdkrn.h"

# define XULFSF_PAGE_ID	"pgeSessionForm"

namespace xulfsf {
	using namespace xulfbs;

	XULFBS_EH( backend_type_selection_eh__ );
	XULFBS_EH( emebedded_backend_selection_eh__ );
	XULFBS_EH( apply_eh__ );
	XULFBS_EH( cancel_eh__ );

	struct session_form__
	: public ui_core__<xulftk::trunk___>
	{
	private:
		frdkrn::backend_extended_type__ _BackendExtendedType;
	public:
		void reset( bso::bool__ = true )
		{
			_BackendExtendedType = frdkrn::bxt_Undefined;
		}
		struct broadcasters__ {
			broadcaster__
				bdcNoBackend,
				bdcPredefinedBackend,
				bdcDaemonBackend,
				bdcEmbeddedBackend,
				bdcEmbeddedBackendSelection,
				bdcAuthentication;
		} &Broadcasters;
		struct event_handlers__ {
			backend_type_selection_eh__ ehBackendTypeSelection;
			emebedded_backend_selection_eh__ ehEmbeddedBackendSelection;
			apply_eh__ ehApply;
			cancel_eh__ ehCancel;
		} &EventHandlers;
		struct widgets__ {
			menulist__ mnlBackendType;
			menuitem__
				mniNoBackend,
				mniPredefinedBackend,
				mniDaemonBackend,
				mniEmbeddedBackend;
			deck__ dckBackendType;
			menulist__ mnlPredefinedBackend;
			textbox__ txbDaemonBackend;
			textbox__ txbEmbeddedBackend;
			textbox__ txbLogin;
			textbox__ txbPassword;
		} &Widgets;
		session_form__(
			broadcasters__ &Broadcasters,
			event_handlers__ &EventHandlers,
			widgets__ &Widgets )
		: Broadcasters( Broadcasters ),
		  EventHandlers( EventHandlers ),
		  Widgets( Widgets )
		{
			reset( false );
		}
		void SetBackendType( frdkrn::backend_extended_type__ Type )
		{
			_BackendExtendedType = Type;
		}
		void Update( void );
	};

	void RegisterSessionFormUI(
		xulftk::trunk___ &Trunk,
		nsIDOMWindow *Window );
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
