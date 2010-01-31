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

#include "xui.h"

namespace ui_base {
	using namespace xui;
}

// Predeclaration
namespace ui {
	class ui___;
};

// Sensitive widget (reacts to events).
#define UI_SWIDGET( widget, name )\
	class name\
	: public widget<ui::ui___>\
	{\
	protected:\
		virtual void NSXPCMOnEvent( nsxpcm::event__ Event );\
	}

// Unsensitive widget (reacts to no event).
#define UI_UWIDGET( widget, name )\
	class name\
	: public widget<ui::ui___>\
	{}

namespace ui_base {
	using ui::ui___;

	typedef nsxpcm::ui_core__ _ui_core__;
	typedef bare_bridge__<ui___> bridge__;
};

#endif