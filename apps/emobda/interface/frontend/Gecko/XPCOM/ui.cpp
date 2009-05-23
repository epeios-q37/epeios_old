/*
	'ui.cpp' by Claude SIMON (http://zeusw.org/).

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

#include "ui.h"

using namespace ui;

void ui::ui___::_SwitchTo( context__ Context )
{
	switch ( Context ) {
	case cSessionForm:
		K().FillTableMenu();
		UI.Main.Broadcasters.DatabaseOpened.Disable();
		UI.Main.MainDeck.SetSelectedPanel( UI.Main.Panels.Home );
		break;
	case cStructureView:
		FillStructureView();
		K().FillTableMenu();
		UI.Main.Broadcasters.DatabaseOpened.Enable();
		UI.Structure.Broadcasters.ItemBrowsing.Enable();
		UI.Structure.Broadcasters.ItemEdition.Disable();
		UI.Main.MainDeck.SetSelectedPanel( UI.Main.Panels.StructureFormAndView );
		break;
	case cStructureItemView:
		UpdateDecks();
		UI.Structure.Broadcasters.ItemBrowsing.Enable();
		UI.Structure.Broadcasters.ItemEdition.Disable();
		UI.Main.MainDeck.SetSelectedPanel( UI.Main.Panels.StructureFormAndView );
		break;
	case cStructureItemDefinition:
	{
		ui_struct::structure__ &UI = this->UI.Structure;

		UI.Broadcasters.ItemBrowsing.Disable();
		UI.Broadcasters.ItemEdition.Enable();

		UI.NameTextbox.Select();
		break;
	}
	case cListView:
		K().FillListView();
		UI.Main.Broadcasters.RecordSelected.Disable();
		UI.Main.MainDeck.SetSelectedPanel( UI.Main.Panels.ListView );

		UI.ListView.ContentTree.ClearSelection();
		break;
	case cRecordForm:
		K().FillRecordForm();
		UI.Main.MainDeck.SetSelectedPanel( UI.Main.Panels.RecordForm );
		break;
	case cRecordView:
		K().FillRecordView();
		UI.Main.MainDeck.SetSelectedPanel( UI.Main.Panels.RecordView );
		break;
	default:
		ERRc();
		break;
	}

	UI.Main.Broadcasters.RecordSelected.Enable( _Target.Record != UNDEFINED_RECORD );
	UI.Main.Broadcasters.TableWithFieldSelected.Enable( _Target.Table != UNDEFINED_TABLE );
}

