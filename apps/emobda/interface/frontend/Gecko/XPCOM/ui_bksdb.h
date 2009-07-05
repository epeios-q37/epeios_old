/*
	'ui_bksdb.h' by Claude SIMON (http://zeusw.org/).

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

#ifndef UI_BKSDB__INC
#define UI_BKSDB__INC

#include "ui_base.h"

namespace ui_bksdb {
	using namespace ui_base;

	UI_ETYPEDEF( ui_base::window__, window__ );

	UI_TYPEDEF( broadcaster__, backend_definition_broadcaster__ );
	UI_TYPEDEF( broadcaster__, remote_broadcaster__ );
	UI_TYPEDEF( broadcaster__, local_broadcaster__ );

	UI_ETYPEDEF( textbox__, host_service_textbox__ );

	UI_ETYPEDEF( command__, apply_command__ );
	UI_ETYPEDEF( command__, cancel_command__ );

	struct backend_selection__
	: public _ui_core__,
	  public bridge__
	{
	public:
		window__ Window;
		host_service_textbox__ HostServiceTextbox;
		struct broadcasters__ {
			backend_definition_broadcaster__ BackendDefinition;
			remote_broadcaster__ Remote;
			local_broadcaster__ Local;
		} Broadcasters;
		struct commands__ {
			apply_command__ Apply;
			cancel_command__ Cancel;
		} Commands;
		void Init(
			bridge_functions__ &Functions,
			nsIDOMWindow *Window)
		{
			_ui_core__::Init( Window );
			bridge__::Init( Functions );
		}
		void ExtractSelectedDatabase( void );
	};

	void RegisterDatabaseSelectionUI(
		ui::ui___ &UI,
		nsIDOMWindow *Window );
}

#endif