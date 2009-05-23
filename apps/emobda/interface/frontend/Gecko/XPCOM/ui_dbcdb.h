/*
	'ui_dbcdb.h' by Claude SIMON (http://zeusw.org/).

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

#ifndef UI_DBCDB__INC
#define UI_DBCDB__INC

#include "ui_base.h"

namespace ui_dbcdb {
	using namespace ui_base;

	UI_ETYPEDEF( ui_base::window__, window__ );

	UI_TYPEDEF( broadcaster__, database_naming_broadcaster__ );

	UI_ETYPEDEF( command__, apply_command__ );
	UI_ETYPEDEF( command__, cancel_command__ );

	UI_TYPEDEF( textbox__, name_textbox__ );
	UI_TYPEDEF( textbox__, path_textbox__ );
	UI_TYPEDEF( textbox__, comment_textbox__ );

	struct database_creation__
	: public _ui_core__,
	  public bridge__
	{
	public:
		window__ Window;
		struct broadcasters__ {
			database_naming_broadcaster__ DatabaseNaming;
		} Broadcasters;
		struct commands__ {
			apply_command__ Apply;
			cancel_command__ Cancel;
		} Commands;
		struct textboxes__ {
			name_textbox__ Name;
			path_textbox__ Path;
			comment_textbox__ Comment;
		} Textboxes;
		void Init( bridge_functions__ &Functions )
		{
			_ui_core__::Init();
			bridge__::Init( Functions );
		}
	};

	void Register(
		ui::ui___ &UI,
		database_creation__ &DatabaseCreationUI,
		nsIDOMWindow *Window );
}

#endif