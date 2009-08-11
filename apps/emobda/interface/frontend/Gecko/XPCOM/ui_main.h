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

	UI_TYPEDEF( broadcaster__, project_opened_broadcaster__ );
	UI_TYPEDEF( broadcaster__, database_opened_broadcaster__ );
	UI_TYPEDEF( broadcaster__, table_with_field_selected_broadcaster__ );
	UI_TYPEDEF( broadcaster__, record_selected_broadcaster__ );

	UI_TYPEDEF( menu__, table_menu__ );

	UI_ETYPEDEF( menu_item__, table_menu_item__ );

	UI_ETYPEDEF( command__, jsconsole_command__ );
	UI_ETYPEDEF( command__, dom_inspector_command__ );

	UI_ETYPEDEF( command__, open_project_command__ );
	UI_ETYPEDEF( command__, close_project_command__ );

	UI_ETYPEDEF( command__, create_database_command__ );
	UI_ETYPEDEF( command__, open_database_command__ );
	UI_ETYPEDEF( command__, define_database_command__ );
	UI_ETYPEDEF( command__, close_database_command__ );

	UI_ETYPEDEF( command__, create_record_command__ );
	UI_ETYPEDEF( command__, modify_record_command__ );
	UI_ETYPEDEF( command__, duplicate_record_command__ );
	UI_ETYPEDEF( command__, browse_record_command__ );
	UI_ETYPEDEF( command__, delete_record_command__ );

	UI_TYPEDEF( deck__, main_deck__ );

	struct main__
	: public _ui_core__
	{
		struct broadcasters__ {
			project_opened_broadcaster__ ProjectOpened;
			database_opened_broadcaster__ DatabaseOpened;
			table_with_field_selected_broadcaster__ TableWithFieldSelected;
			record_selected_broadcaster__ RecordSelected;
		} Broadcasters;
		struct commands__ {
			jsconsole_command__ JSConsole;
			dom_inspector_command__ DOMInspector;
			struct backend__ {
				open_project_command__ OpenProject;
				close_project_command__ CloseProject;
			} Backend;
			struct database__ {
				create_database_command__ Create;
				open_database_command__ Open;
				define_database_command__ Define;
				close_database_command__ Close;
			} Database;
			struct record__ {
				create_record_command__ Create;
				modify_record_command__ Modify;
				duplicate_record_command__ Duplicate;
				browse_record_command__ Browse;
				delete_record_command__ Delete;
			} Record;
		} Commands;
		table_menu__ TableMenu;
		main_deck__ MainDeck;
		struct panels__ {
			panel__
				Home,
				StructureFormAndView,
				ListView,
				RecordView,
				RecordForm;
		} Panels;
		void Init( nsIDOMWindow *Window )
		{
			_ui_core__::Init( Window );
		}
	};

	void RegisterMainUI(
		ui::ui___ &UI,
		nsIDOMWindow *Window );
}

#endif