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

#ifndef UI_MAIN__INC
#define UI_MAIN__INC

#include "ui_base.h"

namespace ui_main {
	using namespace ui_base;

	UI_TYPEDEF( broadcaster__, database_opened_broadcaster__ );

	UI_ETYPEDEF( command__, create_database_command__ );
	UI_ETYPEDEF( command__, open_database_command__ );
	UI_ETYPEDEF( command__, close_database_command__ );

	struct main__
	: public _ui_core__
	{
		struct commands__ {
			struct database__ {
				create_database_command__ Create;
				open_database_command__ Open;
				close_database_command__ Close;
			} Database;
		} Commands;

		struct broadcasters__ {
			database_opened_broadcaster__ DatabaseOpened;
		} Broadcasters;

		void Init( void )
		{
			_ui_core__::Init();
		}
	};

	void Register(
		kernel::kernel___ &Kernel,
		main__ &UI,
		nsIDOMWindow *Window );
}

#endif