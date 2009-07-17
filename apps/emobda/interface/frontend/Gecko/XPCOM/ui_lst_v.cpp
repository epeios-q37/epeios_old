/*
	'ui_lst_v.cpp' by Claude SIMON (http://zeusw.org/).

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

#include "ui_lst_v.h"
#include "kernel.h"

using namespace ui_lst_v;

using kernel::kernel___;
using nsxpcm::event__;

using namespace kernel;

void ui_lst_v::content_tree__::NSXPCMOnEvent( event__ Event )
{
	switch ( Event ) {
	case nsxpcm::eSelect:
		UI().SelectRecord();
		break;
	case nsxpcm::eDblClick:
		UI().SelectRecord();	// In most case, the 'Select' event is called before the 'DblClick' one, but not always.
		UI().BrowseRecord();
		break;
	default:
		ERRc();
		break;
	}
}

/* UI Registrations */

static void Register_(
	bridge_functions__ &Functions,
	list_view__ &UI )
{
	ui_base::Register( Functions, UI.ContentTree, UI.Document, "treContent" );
}

void ui_lst_v::RegisterListViewUI(
	ui::ui___ &UI,
	nsIDOMWindow *Window )
{
	UI.ListView.Init( UI, Window );

	Register_( UI, UI.ListView );

}

