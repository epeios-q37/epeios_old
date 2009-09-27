/*
	'ui_base.h' by Claude SIMON (http://zeusw.org/).

	 This file is part of 'emobda' software.

    'emobda' is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'emobda' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with 'emobda'.  If not, see <http://www.gnu.org/licenses/>.
*/

// $Id$

#ifndef UI_BASE__INC
#define UI_BASE__INC

#include "nsxpcm.h"

// Predeclaration.
namespace ui {
	class ui___;
}

namespace ui_base {
	typedef nsxpcm::ui_core__ _ui_core__;

	class bridge_functions__
	{
	protected:
		virtual const ui::ui___ &__UI( void ) const = 0;
		virtual ui::ui___ &__UI( void ) = 0;
	public:
		const ui::ui___ &UI( void ) const
		{
			return __UI();
		}
		ui::ui___ &UI( void )
		{
			return __UI();
		}
	};

	class bridge__
	{
	private:
		bridge_functions__ *_Functions;
	public:
		bridge__( void )
		{
			_Functions = NULL;
		}
		void Init( bridge_functions__ &Functions )
		{
			_Functions = &Functions;
		}
		const ui::ui___ &UI( void ) const
		{
			return _Functions->UI();
		}
		ui::ui___ &UI( void )
		{
			return _Functions->UI();
		}
	};

	template <typename widget> class _generic__
	: public bridge__,
	  public widget
	{
	protected:
		void NSXPCMOnEvent( nsxpcm::event__ )
		{
			ERRu();
		}
	public:
		void Init( bridge_functions__ &Functions )
		{
			bridge__::Init( Functions );
			widget::Init();
		}
	};

	typedef _generic__<nsxpcm::textbox__> textbox__;
	typedef _generic__<nsxpcm::radio__> radio__;
	typedef _generic__<nsxpcm::button__> button__;
	typedef _generic__<nsxpcm::listbox__> listbox__;
	typedef _generic__<nsxpcm::tree__> tree__;
	typedef _generic__<nsxpcm::deck__> deck__;
	typedef _generic__<nsxpcm::broadcaster__> broadcaster__;
	typedef _generic__<nsxpcm::command__> command__;
	typedef _generic__<nsxpcm::menu__> menu__;
	typedef _generic__<nsxpcm::menu_item__> menu_item__;
	typedef _generic__<nsxpcm::panel__> panel__;
	typedef _generic__<nsxpcm::element__> box__;
	typedef _generic__<nsxpcm::window__> window__;
	typedef _generic__<nsxpcm::description__> description__;

	template <typename widget> void Register(
		bridge_functions__ &Functions,
		widget &Widget,
		nsIDOMDocument *Document,
		const char *Id,
		int Events )
	{
		Widget.Init( Functions );
		nsxpcm::Register( Widget, Document, Id, Events );
	}

	template <typename widget> void Register(
		bridge_functions__ &Functions,
		widget &Widget,
		nsISupports *Supports,
		int Events )
	{
		Widget.Init( Functions );
		nsxpcm::Register( Widget, Supports, Events );
	}

	inline void Register(
		bridge_functions__ &Functions,
		window__ &Window,
		nsIDOMWindow *Element )
	{
		Register( Functions, Window, Element, nsxpcm::efClose );
	}

	inline void Register(
		bridge_functions__ &Functions,
		broadcaster__ &Broadcaster,
		nsIDOMDocument *Document,
		const char *Id )
	{
		Register( Functions, Broadcaster, Document, Id, nsxpcm::efNone );
	}

	inline void Register(
		bridge_functions__ &Functions,
		command__ &Command,
		nsIDOMDocument *Document,
		const char *Id )
	{
		Register( Functions, Command, Document, Id, nsxpcm::efCommand );
	}

	inline void Register(
		bridge_functions__ &Functions,
		tree__ &Tree,
		nsIDOMDocument *Document,
		const char *Id )
	{
		Register( Functions, Tree, Document, Id, nsxpcm::efSelect | nsxpcm::efDblClick );
	}

	inline void Register(
		bridge_functions__ &Functions,
		deck__ &Deck,
		nsIDOMDocument *Document,
		const char *Id )
	{
		Register( Functions, Deck, Document, Id, nsxpcm::efNone );
	}

	inline void Register(
		bridge_functions__ &Functions,
		textbox__ &Textbox,
		nsIDOMDocument *Document,
		const char *Id )
	{
		Register( Functions, Textbox, Document, Id, nsxpcm::efNone );
	}

	inline void Register(
		bridge_functions__ &Functions,
		button__ &Button,
		nsIDOMDocument *Document,
		const char *Id )
	{
		Register( Functions, Button, Document, Id, nsxpcm::efCommand );
	}

	inline void Register(
		bridge_functions__ &Functions,
		description__ &Description,
		nsIDOMDocument *Document,
		const char *Id )
	{
		Register( Functions, Description, Document, Id, nsxpcm::efNone );
	}
}

#define UI_ETYPEDEF( widget, name )\
	class name\
	: public widget\
	{\
	protected:\
		virtual void NSXPCMOnEvent( nsxpcm::event__ Event );\
	}

#define UI_TYPEDEF( widget, name )\
	class name\
	: public widget\
	{}

#endif