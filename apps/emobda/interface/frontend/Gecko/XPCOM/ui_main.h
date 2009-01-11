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

	UI_C( textbox__, table_location_textbox__ );
	UI_C( textbox__, field_name_textbox__ );

	UI_C( button__, create_table_button__ );
	UI_C( button__, add_field_button__ );

	UI_S( listbox__, field_list_listbox__ );
	UI_C( button__, remove_field_button__ );

	struct main__
	: public _ui_core__
	{
		table_location_textbox__ TableLocationTextbox;
		create_table_button__ CreateTableButton;
		field_name_textbox__ FieldNameTextbox;
		add_field_button__ AddFieldButton;
		field_list_listbox__ FieldListListbox;
		remove_field_button__ RemoveFieldButton;

		void Init( void )
		{
			_ui_core__::Init();
		}
	};
}

#endif