/*
	Header for the 'gui' library by Claude SIMON (csimon@epeios.org)
	Copyright (C) 2004 Claude SIMON (csimon@epeios.org).

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

#ifndef GUI__INC
#define GUI__INC

#define GUI_NAME		"GUI"

#define	GUI_VERSION	"$Revision$"

#define GUI_OWNER		"Claude SIMON (csimon@epeios.org)"

#include "ttr.h"

extern class ttr_tutor &GUITutor;

#if defined( XXX_DBG ) && !defined( GUI_NODBG )
#define GUI_DBG
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
//D Graphical User Interface 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "ids.h"

#include "wx/wx.h" 
#include "wx/treectrl.h"

namespace gui {

	class eTreeCtrl;

	typedef wxMenu			eMenu;
	typedef wxMenuBar		eMenuBar;
	typedef wxApp			eApp;
	typedef wxTreeEvent		eTreeEvent;
	typedef wxWindow		eWindow;
	typedef wxEvent			eEvent;
	typedef wxEventType		eEventType;
	typedef wxTreeItemData	eTreeItemData;
	typedef void (eTreeCtrl::*eTreeCtrlFunction)(eEvent&);
	typedef wxTreeItemId	eTreeItemId;
	typedef wxString			eString;
	typedef wxPoint			ePoint;
	typedef wxSize			eSize;
	typedef wxCommandEvent	eCommandEvent;

	ePoint &eDefaultPosition = *(ePoint *)&wxDefaultPosition;
	eSize &eDefaultSize = *(eSize *)&wxDefaultSize;

#define	eMessageBox	wxMessageBox;
#define eEVT_COMMAND_TREE_ITEM_COLLAPSING	wxEVT_COMMAND_TREE_ITEM_COLLAPSING
#define eEVT_COMMAND_TREE_ITEM_EXPANDING	wxEVT_COMMAND_TREE_ITEM_EXPANDING
	class eFrame
	: public wxFrame
	{
	private:
		ids::E_IDS_STORE( int ) Ids;
	public:
		eFrame( const eString& title,
               const ePoint& pos = eDefaultPosition,
               const eSize& size = eDefaultSize )
			   : wxFrame( NULL, wxID_ANY, title, pos, size )
		{}
	};

	class eTreeCtrl
	: public wxTreeCtrl
	{
	public:
		eTreeCtrl( eWindow* parent )
		: wxTreeCtrl( parent )
		{}
		void Connect( eEventType eventType,
					  eTreeCtrlFunction func )
		{
			wxTreeCtrl::Connect( wxID_ANY, eventType, (wxObjectEventFunction)func );
		}
	    bool Disconnect( eEventType eventType,
					  eTreeCtrlFunction func )
		{
			return wxTreeCtrl::Disconnect( wxID_ANY, eventType, (wxObjectEventFunction)func );
		}

	};
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
