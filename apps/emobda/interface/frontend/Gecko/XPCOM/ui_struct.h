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

namespace mbdkernl {
	struct target__;
}	// Predeclaration.

namespace ui_struct {
	using namespace ui_base;

	UI_TYPEDEF( broadcaster__, item_edition_broadcaster__ );
	UI_TYPEDEF( broadcaster__, item_browsing_broadcaster__ );
	UI_TYPEDEF( broadcaster__, item_action_broadcaster__ );

	UI_ETYPEDEF( tree__, browse_tree__ );

	UI_TYPEDEF( deck__, item_action_deck__ );

	UI_ETYPEDEF( command__, modify_database_command__ );
	UI_ETYPEDEF( command__, delete_database_command__ );

	UI_ETYPEDEF( command__, create_table_command__ );
	UI_ETYPEDEF( command__, modify_table_command__ );
	UI_ETYPEDEF( command__, delete_table_command__ );

	UI_ETYPEDEF( command__, create_field_command__ );
	UI_ETYPEDEF( command__, modify_field_command__ );
	UI_ETYPEDEF( command__, delete_field_command__ );

	UI_TYPEDEF( textbox__, name_textbox__ );
	UI_TYPEDEF( textbox__, comment_textbox__ );

	UI_ETYPEDEF( command__, apply_item_command__ );
	UI_ETYPEDEF( command__, cancel_item_command__ );

	UI_TYPEDEF( deck__, form_deck__ );

	struct structure__
	: public _ui_core__,
	  public bridge__
	{
	public:
		nsIDOMElement *Items;
		browse_tree__ BrowseTree;
		struct broadcasters__ {
			item_edition_broadcaster__ ItemEdition;
			item_browsing_broadcaster__ ItemBrowsing;
			struct database__ {
				item_action_broadcaster__
					Modification,
					Deletion;
			} Database;
			struct table__ {
				item_action_broadcaster__
					Creation,
					Modification,
					Deletion;
			} Table;
			struct field__ {
				item_action_broadcaster__
					Creation,
					Modification,
					Deletion;
			} Field;
		} Broadcasters;
		struct commands__ {
			struct database__ {
				modify_database_command__ Modify;
				delete_database_command__ Delete;
			} Database;
			struct table__ {
				create_table_command__ Create;
				modify_table_command__ Modify;
				delete_table_command__ Delete;
			} Table;
			struct field__ {
				create_field_command__ Create;
				modify_field_command__ Modify;
				delete_field_command__ Delete;
			} Field;
			struct item__ {
				apply_item_command__ Apply;
				cancel_item_command__ Cancel;
			} Item;
		} Commands;
		name_textbox__ NameTextbox;
		comment_textbox__ CommentTextbox;
		form_deck__ FormDeck;
		item_action_deck__ ActionDeck;
		nsIDOMElement *DatabaseFormPanel, *TableFormPanel, *FieldFormPanel;
		nsIDOMElement *DatabaseSelectionPanel, *TableSelectionPanel, *FieldSelectionPanel;
		void Init(
			bridge_functions__ &Functions,
			nsIDOMWindow *Window)
		{
			_ui_core__::Init( Window );
			bridge__::Init( Functions );
			Items = NULL;
			DatabaseFormPanel = TableFormPanel = FieldFormPanel = NULL;
			DatabaseSelectionPanel = TableSelectionPanel = FieldSelectionPanel = NULL;
		}
		bso::bool__ GetSelectedItem( mbdkernl::target__ &Target );	// Return 'false' if no item selected, 'true' otherwise.
		void UpdateDecks( void );
		void FillView( void );
	};

	void RegisterStructureUI(
		ui::ui___ &UI,
		nsIDOMWindow *Window );
}

#endif