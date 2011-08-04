/*
	Header for the 'xulwdg' library by Claude SIMON (csimon at zeusw dot org)
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

#ifndef XULWDG__INC
#define XULWDG__INC

#define XULWDG_NAME		"XULWDG"

#define	XULWDG_VERSION	"$Revision$"

#define XULWDG_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &XULWDGTutor;

#if defined( XXX_DBG ) && !defined( XULWDG_NODBG )
#define XULWDG_DBG
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
#include "nsxpcm.h"

// Sensitive widget (reacts to events).
#define XULWDG_SWIDGET( widget, name )\
	class name\
	: public widget\
	{\
	protected:\
		virtual void NSXPCMOnEvent( nsxpcm::event__ Event );\
	}

// Unsensitive widget (reacts to no event).
#define XULWDG_UWIDGET( widget, name )\
	class name\
	: public widget\
	{}

#define XULWDG_ALL_WIDGETS( target )\
	typedef xulwdg::broadcast__<target> broadcast__;\
	typedef xulwdg::command__<target> command__;\
	typedef xulwdg::button__<target> button__;\
	typedef xulwdg::textbox__<target> textbox__;\
	typedef xulwdg::listbox__<target> listbox__;\
	typedef xulwdg::tree__<target> tree__;\
	typedef xulwdg::window__<target> window__;\


namespace xulwdg {
	template <typename target> class bare_bridge__
	{
	private:
		target *_Target;
	public:
		bare_bridge__( void )
		{
			_Target = NULL;
		}
		void Init( target &Target )
		{
			_Target = &Target;
		}
		const target &Target( void ) const
		{
			return *_Target;
		}
		target &Target( void )
		{
			return *_Target;
		}
	};

	template <typename target, typename widget> class _generic__
	: public bare_bridge__<target>,
	  public widget
	{
	protected:
		void NSXPCMOnEvent( nsxpcm::event__ )
		{
			ERRu();
		}
	public:
		void Init( target &Target )
		{
			bare_bridge__<target>::Init( Target );
			widget::Init();
		}
	};

#	define XULWDG__WN( widget, name, events )\
	template <typename target> E_TMIMIC__( E_COVER2( _generic__<target, nsxpcm::widget##__> ), name##__ );\
	template <typename target> inline void Register(\
		target &Target,\
		name##__<target> &Widget,\
		nsISupports *Element,\
		nsIDOMWindow *Window )\
	{\
		Register( Target, Widget, Element, Window, events );\
	}\
	template <typename target> inline void Register(\
		target &Target,\
		name##__<target> &Widget,\
		nsIDOMWindow *Window,\
		const char *Id )\
	{\
		Register( Target, Widget, Window, Id, events );\
	}

#	define XULWDG__W( widget, events )	XULWDG__WN( widget, widget, events )

	XULWDG__W( textbox, nsxpcm::ef_None );
	XULWDG__W( radio, nsxpcm::efCommand );
	XULWDG__W( button, nsxpcm::efCommand );
	XULWDG__W( listbox, nsxpcm::efCommand );
	XULWDG__W( tree, nsxpcm::efSelect | nsxpcm::efDblClick );
	XULWDG__W( deck, nsxpcm::ef_None );
	XULWDG__W( broadcast, nsxpcm::ef_None );
	XULWDG__W( command, nsxpcm::efCommand );
	XULWDG__W( menu, nsxpcm::ef_None );
	XULWDG__W( menu_item, nsxpcm::efCommand );
	XULWDG__W( panel, nsxpcm::ef_None );
	XULWDG__WN( widget, box, nsxpcm::ef_None );
//	XULWDG__W( document, nsxpcm::efClose );
	XULWDG__W( window, nsxpcm::efClose );
	XULWDG__W( description, nsxpcm::ef_None );

	template <typename target, typename widget> void Register(
		target &Target,
		widget &Widget,
		nsIDOMWindow *Window,
		const char *Id,
		int Events )
	{
		Widget.Init( Target );
		nsxpcm::Register( Widget, Window, Id, Events );
	}

	template <typename target, typename widget> void Register(
		target &Target,
		widget &Widget,
		nsISupports *Supports,
		nsIDOMWindow *Window,
		int Events )
	{
		Widget.Init( Target );
		nsxpcm::Register( Widget, Supports, Window, Events );
	}

}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
