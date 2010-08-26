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

	template <typename ui> class bare_bridge__
	{
	private:
		ui *_UI;
	public:
		bare_bridge__( void )
		{
			_UI = NULL;
		}
		void Init( ui &UI )
		{
			_UI = &UI;
		}
		const ui &UI( void ) const
		{
			return *_UI;
		}
		ui &UI( void )
		{
			return *_UI;
		}
	};

	template <typename ui, typename widget> class _generic__
	: public bare_bridge__<ui>,
	  public widget
	{
	protected:
		void NSXPCMOnEvent( nsxpcm::event__ )
		{
			ERRu();
		}
	public:
		void Init( ui &UI )
		{
			bridge__::Init( UI );
			widget::Init();
		}
	};

#	define XULUI__WN( widget, name )\
	template <typename ui> E_TTYPEDEF__( E_COVER2( _generic__<ui, nsxpcm::widget##__> ), name##__ )

#	define XULUI__W( widget )	XULUI__WN( widget, widget )

	XULUI__W( textbox );
	XULUI__W( radio );
	XULUI__W( button );
	XULUI__W( listbox );
	XULUI__W( tree );
	XULUI__W( deck );
	XULUI__W( broadcast );
	XULUI__W( command );
	XULUI__W( menu );
	XULUI__W( menu_item );
	XULUI__W( panel );
	XULUI__WN( element, box );
	XULUI__W( window );
	XULUI__W( description );

	template <typename ui, typename widget> void Register(
		ui &UI,
		widget &Widget,
		nsIDOMDocument *Document,
		const char *Id,
		int Events )
	{
		Widget.Init( UI );
		nsxpcm::Register( Widget, Document, Id, Events );
	}

	template <typename ui, typename widget> void Register(
		ui &UI,
		widget &Widget,
		nsISupports *Supports,
		int Events )
	{
		Widget.Init( UI );
		nsxpcm::Register( Widget, Supports, Events );
	}

	template <typename ui> inline void Register(
		ui &UI,
		window__<ui> &Window,
		nsIDOMWindow *Element )
	{
		Register( UI, Window, Element, nsxpcm::efClose );
	}

	template <typename ui> inline void Register(
		ui &UI,
		broadcast__<ui> &Broadcast,
		nsIDOMDocument *Document,
		const char *Id )
	{
		Register( UI, Broadcast, Document, Id, nsxpcm::ef_None );
	}

	template <typename ui> inline void Register(
		ui &UI,
		command__<ui> &Command,
		nsIDOMDocument *Document,
		const char *Id )
	{
		Register( UI, Command, Document, Id, nsxpcm::efCommand );
	}

	template <typename ui> inline void Register(
		ui &UI,
		tree__<ui> &Tree,
		nsIDOMDocument *Document,
		const char *Id )
	{
		Register( UI, Tree, Document, Id, nsxpcm::efSelect | nsxpcm::efDblClick );
	}

	template <typename ui> inline void Register(
		ui &UI,
		deck__<ui> &Deck,
		nsIDOMDocument *Document,
		const char *Id )
	{
		Register( UI, Deck, Document, Id, nsxpcm::ef_None );
	}

	template <typename ui> inline void Register(
		ui &UI,
		textbox__<ui> &Textbox,
		nsIDOMDocument *Document,
		const char *Id )
	{
		Register( UI, Textbox, Document, Id, nsxpcm::ef_None );
	}

	template <typename ui> inline void Register(
		ui &UI,
		button__<ui> &Button,
		nsIDOMDocument *Document,
		const char *Id )
	{
		Register( UI, Button, Document, Id, nsxpcm::efCommand );
	}

	template <typename ui> inline void Register(
		ui &UI,
		description__<ui> &Description,
		nsIDOMDocument *Document,
		const char *Id )
	{
		Register( UI, Description, Document, Id, nsxpcm::ef_None );
	}
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
