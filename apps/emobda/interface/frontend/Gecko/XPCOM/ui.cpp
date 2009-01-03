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
#include "kernel.h"

using namespace ui;

void ui::table_location_textbox__::NSXPCMOnCommand( void )
{
	K().CreateTable();
}

void ui::create_table_button__::NSXPCMOnCommand( void )
{
	K().CreateTable();
}

void ui::field_name_textbox__::NSXPCMOnCommand( void )
{
	K().AddField();
}

void ui::add_field_button__::NSXPCMOnCommand( void )
{
	K().AddField();
}

void ui::field_list_listbox__::NSXPCMOnSelect( void )
{
ERRProlog
	str::string Value;
ERRBegin
	Value.Init();
	nsxpcm::Alert( K().UI.Main.Window, K().UI.Main.FieldListListbox.GetCurrentItemAttribute( "FieldId", Value ) );
ERRErr
ERREnd
ERREpilog
}

void ui::remove_field_button__::NSXPCMOnCommand( void )
{
ERRProlog
	str::string Value;
ERRBegin
	if ( K().UI.Main.FieldListListbox.GetSelectedCount() == 0 )
		nsxpcm::Alert( K().UI.Main.Window, "No item selected" );
	else {
		Value.Init();
		nsxpcm::Alert( K().UI.Main.Window, K().UI.Main.FieldListListbox.GetCurrentItemAttribute( "FieldId", Value ) );
	}
ERRErr
ERREnd
ERREpilog
}

