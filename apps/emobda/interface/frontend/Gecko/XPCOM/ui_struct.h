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

	UI_NONE( broadcaster__, form_broadcaster__ );

	UI_S( tree__, browse_tree__ );

	UI_NONE( deck__, buttons_deck__ );

	UI_C( button__, create_table_button__ );
	UI_C( button__, modify_table_button__ );

	UI_C( button__, create_field_button__ );
	UI_C( button__, modify_field_button__ );

	UI_NONE( deck__, form_deck__ );

	UI_NONE( button__, apply_form_button__ );
	UI_NONE( button__, cancel_form_button__ );

	struct structure__
	: public _ui_core__,
	  public bridge__
	{
	public:
		nsIDOMElement *Items;
		form_broadcaster__ FormBroadcaster;
		browse_tree__ BrowseTree;
		buttons_deck__ ButtonsDeck;
		nsIDOMElement *DatabaseButtonsPanel, *TableButtonsPanel, *FieldButtonsPanel;
		create_table_button__ CreateTableButton;
		modify_table_button__ ModifyTableButton;
		create_table_button__ CreateFieldButton;
		modify_table_button__ ModifyFieldButton;
		form_deck__ FormDeck;
		nsIDOMElement *DatabaseFormPanel, *TableFormPanel, *FieldFormPanel;
		apply_form_button__ ApplyFormButton;
		cancel_form_button__ CancelFormButton;
		void Init( bridge_functions__ &Functions )
		{
			_ui_core__::Init();
			bridge__::Init( Functions );
			Items = NULL;
			DatabaseButtonsPanel = TableButtonsPanel = FieldButtonsPanel = NULL;
			DatabaseFormPanel = TableFormPanel = FieldFormPanel = NULL;
		}
		void UpdateDecks( void );
		void EnableForm( bso::bool__ Value = true );
		void DisableForm( void )
		{
			EnableForm( false );
		}
	};
}

#endif