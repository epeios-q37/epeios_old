/*
	'ui_dbsdb.h' by Claude SIMON (http://zeusw.org/).

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

#ifndef UI_DBSDB__INC
#define UI_DBSDB__INC

#include "ui_base.h"

namespace ui_dbsdb {
	using namespace ui_base;

	UI_ETYPEDEF( ui_base::window__, window__ );

	UI_TYPEDEF( broadcaster__, database_selection_broadcaster__ );

	UI_ETYPEDEF( tree__, database_tree__ );

	UI_ETYPEDEF( command__, apply_command__ );
	UI_ETYPEDEF( command__, cancel_command__ );

	struct database_selection__
	: public _ui_core__,
	  public bridge__
	{
	public:
		window__ Window;
		database_tree__ DatabaseTree;
		struct broadcasters__ {
			database_selection_broadcaster__ DatabaseSelection;
		} Broadcasters;
		struct commands__ {
			apply_command__ Apply;
			cancel_command__ Cancel;
		} Commands;
		void Init( bridge_functions__ &Functions )
		{
			_ui_core__::Init();
			bridge__::Init( Functions );
		}
	};

	void Register(
		ui::ui___ &UI,
		database_selection__ &DatabaseSelectionUI,
		nsIDOMWindow *Window );
}

#endif