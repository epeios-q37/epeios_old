/*
	'ui.cpp' by Claude SIMON (http://zeusw.org/).

	 This file is part of 'eDemo' software.

    'eDemo' is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'eDemo' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with 'eDemo'.  If not, see <http://www.gnu.org/licenses/>.
*/

// $Id$

#include "ui.h"
#include "flx.h"
#include "trunk.h"

using namespace ui;

void ui::ui___::_SwitchTo( panel__ Panel )
{
	nsIDOMNode *MainPanel = NULL;

	switch ( Panel ) {
	case pHome:
		MainPanel = Main.Widgets.vewHome.GetWidget();
		break;
	case pSessionForm:
		MainPanel = Main.Widgets.vewSessionForm.GetWidget();
		break;
	case pSessionView:
		MainPanel = Main.Widgets.vewSessionView.GetWidget();
		break;
	default:
		ERRc();
		break;
	}

	Main.Widgets.dckMain.SetSelectedItem( MainPanel );

}

