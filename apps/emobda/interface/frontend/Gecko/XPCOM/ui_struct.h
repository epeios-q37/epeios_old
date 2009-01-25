/*
	'ui_struct.h' by Claude SIMON (http://zeusw.org/).

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

#ifndef UI_STRUCT__INC
#define UI_STRUCT__INC

#include "ui_base.h"

namespace ui_struct {
	using namespace ui_base;

	UI_NONE( broadcaster__, item_edition_broadcaster__ );
	UI_B( broadcaster__, structure_browsing_broadcaster__ );
	UI_NONE( broadcaster__, database_selection_broadcaster__ );
	UI_NONE( broadcaster__, table_selection_broadcaster__ );
	UI_NONE( broadcaster__, field_selection_broadcaster__ );

	UI_S( tree__, browse_tree__ );

	UI_NONE( deck__, buttons_deck__ );

	UI_C( command__, rename_database_command__ );

	UI_C( command__, create_table_command__ );
	UI_C( command__, rename_table_command__ );
	UI_C( command__, delete_table_command__ );

	UI_C( command__, create_field_command__ );
	UI_C( command__, modify_field_command__ );
	UI_C( command__, delete_field_command__ );

	UI_NONE( deck__, form_deck__ );

	UI_NONE( button__, apply_form_button__ );
	UI_NONE( button__, cancel_form_button__ );

	struct structure__
	: public _ui_core__,
	  public bridge__
	{
	public:
		nsIDOMElement *Items;
		browse_tree__ BrowseTree;
		struct broadcasters__ {
			item_edition_broadcaster__ ItemEdition;
			structure_browsing_broadcaster__ StructureBrowsing;
			database_selection_broadcaster__ DatabaseSelection;
			table_selection_broadcaster__ TableSelection;
			field_selection_broadcaster__ FieldSelection;
		} Broadcasters;
		struct commands__ {
			struct database__ {
				rename_database_command__ Rename;
			} Database;
			struct table__ {
				create_table_command__ Create;
				rename_table_command__ Rename;
				delete_table_command__ Delete;
			} Table;
			struct field__ {
				create_field_command__ Create;
				modify_field_command__ Modify;
				delete_field_command__ Delete;
			} Field;
		} Commands;
		form_deck__ FormDeck;
		nsIDOMElement *DatabaseFormPanel, *TableFormPanel, *FieldFormPanel;
		apply_form_button__ ApplyFormButton;
		cancel_form_button__ CancelFormButton;
		void Init( bridge_functions__ &Functions )
		{
			_ui_core__::Init();
			bridge__::Init( Functions );
			Items = NULL;
			DatabaseFormPanel = TableFormPanel = FieldFormPanel = NULL;
		}
		void UpdateDecks( void );
	};

	void Register(
		kernel::kernel___ &Kernel,
		structure__ &UI,
		nsIDOMWindow *Window );
}

#endif