/*
	'ui_struct.cpp' by Claude SIMON (http://zeusw.org/).

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

#include "ui_struct.h"
#include "kernel.h"

using namespace ui_struct;

void ui_struct::structure__::UpdateDecks( void )
{
ERRProlog
	str::string Type;
ERRBegin
	structure__ &UI = K().UI.Structure;

	if ( UI.BrowseTree.GetCurrentIndex() != -1 ) {
		Type.Init();
		UI.BrowseTree.GetCurrentItemAttribute( "Type", Type );

		if ( Type == "Database" ) {
			UI.ButtonsDeck.SetSelectedPanel( UI.DatabaseButtonsPanel );
			UI.FormDeck.SetSelectedPanel( UI.DatabaseFormPanel );
		} else if ( Type == "Table" ) {
			UI.ButtonsDeck.SetSelectedPanel( UI.TableButtonsPanel );
			UI.FormDeck.SetSelectedPanel( UI.TableFormPanel );
		}else if ( Type == "Field" ) {
			UI.ButtonsDeck.SetSelectedPanel( UI.FieldButtonsPanel );
			UI.FormDeck.SetSelectedPanel( UI.FieldFormPanel );
		} else
			ERRu();
	} else {
		UI.ButtonsDeck.SetSelectedIndex( -1 );
		UI.FormDeck.SetSelectedIndex( -1 );
	}

ERRErr
ERREnd
ERREpilog
}

void ui_struct::structure__::EnableForm( bso::bool__ Value )
{
	structure__ &UI = K().UI.Structure;
/*
	UI.ApplyFormButton.Enable( Value );
	UI.CancelFormButton.Enable( Value );
*/
	UI.FormBroadcaster.Disable();
}




void ui_struct::create_table_button__::NSXPCMOnCommand( void )
{
	nsxpcm::Alert( K().UI.Structure.Window, "Create Table !" );
}

void ui_struct::modify_table_button__::NSXPCMOnCommand( void )
{
	nsxpcm::Alert( K().UI.Structure.Window, "Modify Table !" );
}

void ui_struct::create_field_button__::NSXPCMOnCommand( void )
{
	nsxpcm::Alert( K().UI.Structure.Window, "Create Field !" );
}

void ui_struct::modify_field_button__::NSXPCMOnCommand( void )
{
	nsxpcm::Alert( K().UI.Structure.Window, "Modify Field !" );
}

void ui_struct::browse_tree__::NSXPCMOnSelect( void )
{
	K().UI.Structure.UpdateDecks();
}



