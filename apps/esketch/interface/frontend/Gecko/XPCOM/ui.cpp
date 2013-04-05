/*
	'ui.cpp' by Claude SIMON (http://zeusw.org/).

	 This file is part of 'eSketch' software.

    'eSketch' is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'eSketch' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with 'eSketch'.  If not, see <http://www.gnu.org/licenses/>.
*/

// $Id$

#include "ui.h"
#include "trunk.h"

using namespace ui;

static void _ShowPanel(
	display__ Display,
	ui___ &UI )
{
	nsIDOMNode *MainPanel = NULL;

	switch ( Display ) {
	case dHome:
		MainPanel = UI.Main.Widgets.vewHome;
		break;
	case dSessionForm:
		MainPanel = UI.Main.Widgets.vewSessionForm;
		break;
	case dSessionView:
		MainPanel = UI.Main.Widgets.vewSessionView;
		break;
	default:
		ERRCcp();
		break;
	}

	UI.Main.Widgets.dckMain.SetSelectedItem( MainPanel );

}

void ui::ui___::_SetDisplay( display__ Display )
{
	_Display = Display;
}

void ui::ui___::_Update( void )
{
	Main.Update();

	switch( _Display ) {
	case dHome:
		break;
	case dSessionForm:
		SessionForm.Update();
		break;
	case dSessionView:
		SessionView.Update();
		break;
	default:
		ERRCcp();
		break;
	}

	_ShowPanel( _Display, *this );
}


