/*
	'ui_rcd_v.cpp' by Claude SIMON (http://zeusw.org/).

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

#include "ui_rcd_v.h"
#include "kernel.h"

using namespace ui_rcd_v;
using kernel::kernel___;
using nsxpcm::event__;

using namespace kernel;

/* UI Registrations */

static void Register_(
	bridge_functions__ &Functions,
	command__ &Command,
	nsIDOMDocument *Document,
	const char *Id )
{
	ui_base::Register( Functions, Command, Document, Id );
}

static void Register_(
	bridge_functions__ &Functions,
	record_view__::commands__ &UI,
	nsIDOMDocument *Document )
{
//	Register_( Kernel, UI.ApplyRecord, Document, "cmdApplyRecord" );
}

static void Register_(
	bridge_functions__ &Functions,
	record_view__ &UI )
{
	ui_base::Register( Functions, UI.RecordBox, UI.Document, "boxRecord", nsxpcm::efNone );

	Register_( Functions, UI.Commands, UI.Document );
}

void ui_rcd_v::RegisterRecordViewUI(
	ui::ui___ &UI,
	nsIDOMWindow *Window )
{
	UI.RecordView.Set( Window );

	Register_( UI, UI.RecordView );
}


