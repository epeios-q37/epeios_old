/*
	'ui_rcd_f.h' by Claude SIMON (http://zeusw.org/).

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

#ifndef UI_RCD_F__INC
#define UI_RCD_F__INC

#include "ui_base.h"

namespace ui_rcd_f {
	using namespace ui_base;

	UI_UWIDGET( box__, record_box__ );

	UI_SWIDGET( command__, apply_record_command__ );
	UI_SWIDGET( command__, cancel_record_command__ );

	struct record_form__
	: public _ui_core__,
	  public bridge__
	{
	public:
		record_box__ RecordBox;
		struct commands__ {
			apply_record_command__ ApplyRecord;
			cancel_record_command__ CancelRecord;
		} Commands;
		void Init(
			ui___ &UI,
			nsIDOMWindow *Window)
		{
			_ui_core__::Init( Window );
			bridge__::Init( UI );
		}
	};

	void RegisterRecordFormUI(
		ui::ui___ &UI,
		nsIDOMWindow *Window );
}

#endif