/*
	Header for the 'xulfdg' library by Claude SIMON (csimon at zeusw dot org)
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

#ifndef XULFDG__INC
#define XULFDG__INC

#define XULFDG_NAME		"XULFDG"

#define	XULFDG_VERSION	"$Revision$"

#define XULFDG_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &XULFDGTutor;

#if defined( XXX_DBG ) && !defined( XULFDG_NODBG )
#define XULFDG_DBG
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
//D XUL Frontend DebuG 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# include "err.h"
# include "flw.h"

# include "xulfbs.h"

# define XULFDG_DIALOG_ID	"dlgDebug_alpha1"

namespace xulfdg {
	using namespace xulfbs;

	XULFBS_REH( jsconsole_eh__ );
	XULFBS_REH( dominspector_eh__ );
	XULFBS_REH( frontend_error_eh__ );
	XULFBS_REH( backend_error_eh__ );

	struct debug_dialog__
	: public _ui_core__
	{
	private:
		xulftk::trunk___ *_Trunk;
	public:
		struct broadcasters__ {
			broadcaster__
				JSConsole,
				DOMInspector,
				FrontendError,
				BackendError;
		} Broadcasters;
		struct event_handlers__ {
			jsconsole_eh__ JSConsole;
			dominspector_eh__ DOMInspector;
			frontend_error_eh__ FrontendError;
			backend_error_eh__ BackendError;
		} EventHandlers;
		void reset( bso::bool__ P = true )
		{
			_Trunk = NULL;
		}
		debug_dialog__( void )
		{
			reset( false );
		}
		~debug_dialog__( void )
		{
			reset();
		}
		void Init(
			xulftk::trunk___ &Trunk,
			nsIDOMWindow *Window)
		{
			reset();
			_ui_core__::Init( Window );
			_Trunk = &Trunk;
		}
		void Update( void );
		void ExtractSelectedDatabase( void );
		void Close( void )
		{
			nsxpcm::Close( Window() );
//			Widgets.reset();
		}
	};

	void Register(
		xulftk::trunk___ &Trunk,
		nsIDOMWindow *Window );


}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
