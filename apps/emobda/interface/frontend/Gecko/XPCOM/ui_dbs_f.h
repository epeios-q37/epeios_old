/*
	'ui_dbs_f.h' by Claude SIMON (http://zeusw.org/).

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

#ifndef UI_DBS_F__INC
#define UI_DBS_F__INC

#include "ui_base.h"

namespace ui_dbs_f {
	using namespace ui_base;

	UI_SWIDGET( ui_base::window__, window__ );

	UI_UWIDGET( broadcaster__, database_naming_broadcaster__ );

	UI_SWIDGET( command__, apply_command__ );
	UI_SWIDGET( command__, cancel_command__ );

	UI_UWIDGET( textbox__, name_textbox__ );
	UI_UWIDGET( textbox__, path_textbox__ );
	UI_UWIDGET( textbox__, comment_textbox__ );

	struct database_form__
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
		void Init(
			ui___ &UI,
			nsIDOMWindow *Window)
		{
			_ui_core__::Init( Window );
			bridge__::Init( UI );
		}
	};

	void RegisterDatabaseFormUI(
		ui::ui___ &UI,
		nsIDOMWindow *Window );
}

#endif