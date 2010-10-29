/*
	Header for the 'xulfrd' library by Claude SIMON (csimon at zeusw dot org)
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

#ifndef XULFRD__INC
#define XULFRD__INC

#define XULFRD_NAME		"XULFRD"

#define	XULFRD_VERSION	"$Revision$"

#define XULFRD_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &XULFRDTutor;

#if defined( XXX_DBG ) && !defined( XULFRD_NODBG )
#define XULFRD_DBG
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
//D XUL WiDGets 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "xulfbs.h"

namespace xulfrd {

	XULFBS_WINDOW( window__ );
	XULFBS_COMMAND( about_command__ );
	XULFBS_COMMAND( web_site_command__ );
	XULFBS_COMMAND( debug_command__ );
	XULFBS_COMMAND( new_project_command__ );
	XULFBS_COMMAND( open_project_command__ );
	XULFBS_COMMAND( close_project_command__ );
	XULFBS_COMMAND( exit_command__ );

	typedef nsxpcm::ui_core__ _ui_core__;

	struct main__
	: public _ui_core__
	{
	private:
		trunk___ *_Trunk;
	public:
		struct widgets__ {
			window__ Window;
			struct broadcasters__ {
			} Broadcasters;
			struct commands__ {
				new_project_command__ NewProject;
				open_project_command__ OpenProject;
				close_project_command__ CloseProject;
				about_command__ About;
				web_site_command__ WebSite;
				debug_command__ Debug;
				exit_command__ Exit;
			} Commands;
		} Widgets;
		void Init(
			nsIDOMWindow *Window,
			trunk___ &Trunk )
		{
			_ui_core__::Init( Window );
			_Trunk = &Trunk;
		}
		void UpdateUI( void );
	};

	void RegisterMainUI(
		trunk___ &Trunk,
		nsIDOMWindow *Window );

	E_ROW( trow__ );	// 'trunk row', see below.

	extern nsxpcm::repository< class trunk___, trow__> Repository;

	struct trunk_functions__
	{
	protected:
		virtual void UIExposeTrunk( void ) = 0;	// Trunk used by UI becomes current trunk to use for further UI part.
		virtual void UIRevokeTrunk( void ) = 0;	// Trunk used by this UI do no more be availabe for further UI part.
	public:
		void ExposeTrunk( void )
		{
			UIExposeTrunk();
		}
		void RevokeTrunk( void )
		{
			UIRevokeTrunk();
		}
	};

	class trunk___
	{
	};


}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
