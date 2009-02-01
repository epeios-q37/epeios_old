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

	UI_TYPEDEF( broadcaster__, item_edition_broadcaster__ );
	UI_TYPEDEF( broadcaster__, structure_item_browsing_broadcaster__ );
	UI_TYPEDEF( broadcaster__, structure_item_creation_broadcaster__ );
	UI_TYPEDEF( broadcaster__, structure_item_modification_broadcaster__ );
	UI_TYPEDEF( broadcaster__, structure_item_deletion_broadcaster__ );

	UI_ETYPEDEF( tree__, browse_tree__ );

	UI_TYPEDEF( deck__, buttons_deck__ );

	UI_ETYPEDEF( command__, create_structure_item_command__ );
	UI_ETYPEDEF( command__, modify_structure_item_command__ );
	UI_ETYPEDEF( command__, delete_structure_item_command__ );

	UI_TYPEDEF( deck__, form_deck__ );

	UI_TYPEDEF( button__, apply_form_button__ );
	UI_TYPEDEF( button__, cancel_form_button__ );

	struct structure__
	: public _ui_core__,
	  public bridge__
	{
	public:
		nsIDOMElement *Items;
		browse_tree__ BrowseTree;
		struct broadcasters__ {
			item_edition_broadcaster__ StructureItemEdition;
			structure_item_creation_broadcaster__ StructureItemBrowsing;
			structure_item_creation_broadcaster__ StructureItemCreation;
			structure_item_modification_broadcaster__ StructureItemModification;
			structure_item_deletion_broadcaster__ StructureItemDeletion;
		} Broadcasters;
		struct commands__ {
			create_structure_item_command__ CreateStructureItem;
			modify_structure_item_command__ ModifyStructureItem;
			delete_structure_item_command__ DeleteStructureItem;
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