/*
	'ui.h' by Claude SIMON (http://zeusw.org/).

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

#ifndef UI__INC
#define UI__INC

#include "ui_main.h"
#include "ui_struct.h"

namespace ui {
	using ui_base::bridge_functions__;

	using ui_main::main__;
	using ui_struct::structure__;

	struct ui__ {
		main__ Main;
		structure__ Structure;
		void Init( bridge_functions__ &Functions )
		{
			Main.Init();
			Structure.Init( Functions );
		}
	};

	inline void Initialize( ui__ &UI )
	{
		UI.Main.Broadcasters.DatabaseOpened.Disable();
	}
}

#endif