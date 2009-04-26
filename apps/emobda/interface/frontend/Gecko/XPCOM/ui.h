/*
	'ui.h' by Claude SIMON (http://zeusw.org/).

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

#ifndef UI__INC
#define UI__INC

#include "ui_main.h"
#include "ui_dbcdb.h"
#include "ui_dbsdb.h"
#include "ui_struct.h"
#include "ui_lst_v.h"
#include "ui_rcd_f.h"
#include "ui_rcd_v.h"

namespace ui {
	using ui_base::bridge_functions__;

	struct ui__ {
		ui_main::main__ Main;
		ui_dbcdb::database_creation__ DatabaseCreation;
		ui_dbsdb::database_selection__ DatabaseSelection;
		ui_struct::structure__ Structure;
		ui_lst_v::list_view__ ListView;
		ui_rcd_f::record_form__ RecordForm;
		ui_rcd_v::record_view__ RecordView;
		void Init( bridge_functions__ &Functions )
		{
			Main.Init();
			Structure.Init( Functions );
		}
	};
}

#endif