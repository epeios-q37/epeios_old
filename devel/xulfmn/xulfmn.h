/*
	Header for the 'xulfmn' library by Claude SIMON (csimon at zeusw dot org)
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

	XULFBS_EH( window_eh__, nsxpcm::efClose );
	XULFBS_REH( about_eh__ );
	XULFBS_REH( web_site_eh__ );
	XULFBS_REH( debug_eh__ );
	XULFBS_REH( new_project_eh__ );
	XULFBS_REH( open_project_eh__ );
	XULFBS_REH( close_project_eh__ );
	XULFBS_REH( exit_eh__ );

	XULFBS_REH( test_eh__ );

	XULFBS_W( deck__, main_deck__ );
	XULFBS_W( window__, window__ );

	struct main__
	: public _ui_core__
	{
	private:
		xulftk::trunk___ *_Trunk;
	public:
		struct event_handlers__ {
			window_eh__ Window;
			new_project_eh__ NewProject;
			open_project_eh__ OpenProject;
			close_project_eh__ CloseProject;
			about_eh__ About;
			web_site_eh__ WebSite;
			debug_eh__ Debug;
			exit_eh__ Exit;
			test_eh__ Test;
		} EventHandlers;
		struct broadcasters__ {
			broadcaster__ CloseProject;
		} Broadcasters;
		struct widgets__ {
			window__ Window;
			main_deck__ MainDeck;
		} Widgets;
		void Init(
			nsIDOMWindow *Window,
			xulftk::trunk___ &Trunk )
		{
			_ui_core__::Init( Window );
			_Trunk = &Trunk;
		}
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
