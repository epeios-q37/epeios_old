/*
	Header for the 'xului' library by Claude SIMON (csimon at zeusw dot org)
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

#ifndef XULUI__INC
#define XULUI__INC

#define XULUI_NAME		"XULUI"

#define	XULUI_VERSION	"$Revision$"

#define XULUI_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &XULUITutor;

#if defined( XXX_DBG ) && !defined( XULUI_NODBG )
#define XULUI_DBG
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
//D XUL User Interface. 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "nsxpcm.h"

namespace xului {

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
			bridge__::Init( Target );
			widget::Init();
		}
	};

#	define XULUI__WN( widget, name, events )\
	template <typename target> E_TTYPEDEF__( E_COVER2( _generic__<target, nsxpcm::widget##__> ), name##__ );\
	template <typename target> inline void Register(\
		target &Target,\
		name##__<target> &Widget,\
		nsIDOMWindow *Element )\
	{\
		Register( Target, Widget, Element, events );\
	}\
	template <typename target> inline void Register(\
		target &Target,\
		name##__<target> &Widget,\
		nsIDOMWindow *Window,\
		const char *Id )\
	{\
		Register( Target, Widget, Window, Id, events );\
	}

#	define XULUI__W( widget, events )	XULUI__WN( widget, widget, events )

	XULUI__W( textbox, nsxpcm::ef_None );
	XULUI__W( radio, nsxpcm::efCommand );
	XULUI__W( button, nsxpcm::efCommand );
	XULUI__W( listbox, nsxpcm::efCommand );
	XULUI__W( tree, nsxpcm::efSelect | nsxpcm::efDblClick );
	XULUI__W( deck, nsxpcm::ef_None );
	XULUI__W( broadcast, nsxpcm::ef_None );
	XULUI__W( command, nsxpcm::efCommand );
	XULUI__W( menu, nsxpcm::ef_None );
	XULUI__W( menu_item, nsxpcm::efCommand );
	XULUI__W( panel, nsxpcm::ef_None );
	XULUI__WN( element, box, nsxpcm::ef_None );
	XULUI__W( window, nsxpcm::efClose );
	XULUI__W( description, nsxpcm::ef_None );

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
		int Events )
	{
		Widget.Init( Target );
		nsxpcm::Register( Widget, Supports, Events );
	}
/*
	template <typename target> inline void Register(
		ui &UI,
		window__<ui> &Window,
		nsIDOMWindow *Element )
	{
		Register( UI, Window, Element, nsxpcm::efClose );
	}

	template <typename target> inline void Register(
		ui &UI,
		broadcast__<ui> &Broadcast,
		nsIDOMWindow *Window,
		const char *Id )
	{
		Register( UI, Broadcast, Window, Id, nsxpcm::ef_None );
	}

	template <typename target> inline void Register(
		ui &UI,
		command__<ui> &Command,
		nsIDOMWindow *Window,
		const char *Id )
	{
		Register( UI, Command, Window, Id, nsxpcm::efCommand );
	}

	template <typename target> inline void Register(
		ui &UI,
		tree__<ui> &Tree,
		nsIDOMWindow *Window,
		const char *Id )
	{
		Register( UI, Tree, Window, Id, nsxpcm::efSelect | nsxpcm::efDblClick );
	}

	template <typename target> inline void Register(
		ui &UI,
		deck__<ui> &Deck,
		nsIDOMWindow *Window,
		const char *Id )
	{
		Register( UI, Deck, Document, Id, nsxpcm::ef_None );
	}

	template <typename ui> inline void Register(
		ui &UI,
		textbox__<ui> &Textbox,
		nsIDOMWindow *Window,
		const char *Id )
	{
		Register( UI, Textbox, Window, Id, nsxpcm::ef_None );
	}

	template <typename ui> inline void Register(
		ui &UI,
		button__<ui> &Button,
		nsIDOMWindow *Window,
		const char *Id )
	{
		Register( UI, Button, Window, Id, nsxpcm::efCommand );
	}

	template <typename ui> inline void Register(
		ui &UI,
		description__<ui> &Description,
		nsIDOMWindow *Window,
		const char *Id )
	{
		Register( UI, Description, Window, Id, nsxpcm::ef_None );
	}
	*/
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
