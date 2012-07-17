/*
	'ui_base.h' by Claude SIMON (http://zeusw.org/).

	 This file is part of 'eDemo' software.

    'eDemo' is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'eDemo' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with 'eDemo'.  If not, see <http://www.gnu.org/licenses/>.
*/

// $Id$

#ifndef UI_BASE__INC
#define UI_BASE__INC

#include "xulfbs.h"

// Predeclarations
namespace trunk {
	class trunk___;
};

#define UI_SWIDGET( widget, name )	XULWDG_SWIDGET( ui_base::widget, name )
#define UI_UWIDGET( widget, name )	XULWDG_UWIDGET( ui_base::widget, name )

#define UI_WINDOW( name )		UI_SWIDGET( window__, name )
#define UI_COMMAND( name )		UI_SWIDGET( command__, name )
#define UI_BROADCASTER( name )	UI_UWIDGET( broadcaster__, name )

namespace ui_base {

	enum panel__ {
		pHome,
		pSessionForm,
		pSessionView,
		p_amount,
		p_Undefined
	};


	typedef xulfbs::ui_core__<trunk::trunk___> ui_core__;

	typedef xulwdg::event_handler__<trunk::trunk___> _event_handler__;

	class event_handler__
	: public _event_handler__
	{
	protected:
		virtual void NSXPCMOnErr( const char *Message );
	};

	XULWDG_ALMOST_ALL_WIDGETS( trunk::trunk___ )

# define UI_BASE_EH( name )\
	class name\
	: public ui_base::event_handler__\
	{\
	protected:\
		virtual void NSXPCMOnEvent( nsxpcm::event__ Event );\
	};
};

#endif