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

# define XULFSF_PAGE_ID	"pgeSessionForm"

namespace xulfsf {
	using namespace xulfbs;

	XULFBS_COMMAND( select_project_command__ );
	XULFBS_UWIDGET( xulwdg::textbox__, project_filename_textbox__ );
	XULFBS_BROADCASTER( embedded_broadcaster__ );
	XULFBS_BROADCASTER( daemon_broadcaster__ );
	XULFBS_BROADCASTER( predefined_broadcaster__ );
	XULFBS_SWIDGET( xulwdg::radiogroup__, backend_location_radiogroup__ );	// '<radiogroup observes=...>' et '<radiogroup command=...>' ne fonctionne pas.

	struct session_form__
	: public _ui_core__
	{
	private:
		xulftk::trunk___ *_Trunk;
	public:
		struct widgets__ {
			struct broadcasters__ {
				embedded_broadcaster__ Embedded;
				daemon_broadcaster__ Daemon;
				predefined_broadcaster__ Predefined;
			} Broadcasters;
			struct commands__ {
				select_project_command__ SelectProject;
			} Commands;
			project_filename_textbox__ ProjectFileNameTextbox;
			backend_location_radiogroup__ BackendLocationRadiogroup;
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
