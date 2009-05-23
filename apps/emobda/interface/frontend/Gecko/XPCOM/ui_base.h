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
	public:
		void Init( bridge_functions__ &Functions )
		{
			bridge__::Init( Functions );
			widget::Init();
		}
	};

	typedef _generic__<nsxpcm::textbox__> textbox__;
	typedef _generic__<nsxpcm::button__> button__;
	typedef _generic__<nsxpcm::listbox__> listbox__;
	typedef _generic__<nsxpcm::tree__> tree__;
	typedef _generic__<nsxpcm::deck__> deck__;
	typedef _generic__<nsxpcm::broadcaster__> broadcaster__;
	typedef _generic__<nsxpcm::command__> command__;
	typedef _generic__<nsxpcm::element__> menu__;
	typedef _generic__<nsxpcm::menu_item__> menu_item__;
	typedef _generic__<nsxpcm::panel__> panel__;
	typedef _generic__<nsxpcm::element__> box__;
	typedef _generic__<nsxpcm::window__> window__;
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