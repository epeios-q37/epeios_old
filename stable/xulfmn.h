/*
	Header for the 'xulfmn' library by Claude SIMON (csimon at zeusw dot org)
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

#ifndef XULFMN__INC
#define XULFMN__INC

#define XULFMN_NAME		"XULFMN"

#define	XULFMN_VERSION	"$Revision$"

#define XULFMN_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &XULFMNTutor;

#if defined( XXX_DBG ) && !defined( XULFMN_NODBG )
#define XULFMN_DBG
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
//D XUL Frontend MaiN 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# include "err.h"
# include "flw.h"

# include "xulfbs.h"

# define XULFMN_WINDOW_ID	"wdwMain"

namespace xulfmn {
	using namespace xulfbs;

	XULFBS_EH( exit_eh__ );
	XULFBS_EH( about_eh__ );
	XULFBS_EH( web_site_eh__ );
	XULFBS_EH( debug_eh__ );
	XULFBS_EH( new_project_eh__ );
	XULFBS_EH( open_project_eh__ );
	XULFBS_EH( close_project_eh__ );

	struct main__
	: public ui_core__<xulftk::trunk___>
	{
	public:
		struct broadcasters__ {
			broadcaster__ bdcCloseProject;
		} &Broadcasters;
		struct event_handlers__ {
			new_project_eh__ ehNewProject;
			open_project_eh__ ehOpenProject;
			close_project_eh__ ehCloseProject;
			about_eh__ ehAbout;
			web_site_eh__ ehWebSite;
			debug_eh__ ehDebug;
			exit_eh__ ehExit;
		} &EventHandlers;
		struct widgets__ {
			window__ wdwMain;
			deck__ dckMain;
			widget__
				boxHome,
				ifrSessionForm,
				ifrSessionView;
		} &Widgets;
		main__(
			broadcasters__ &Broadcasters,
			event_handlers__ &EventHandlers,
			widgets__ &Widgets )
		: Broadcasters( Broadcasters ),
		  EventHandlers( EventHandlers ),
		  Widgets( Widgets )
		{}
		void Update( void );
	};

	void RegisterMainUI(
		xulftk::trunk___ &Trunk,
		nsIDOMWindow *Window );


}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
