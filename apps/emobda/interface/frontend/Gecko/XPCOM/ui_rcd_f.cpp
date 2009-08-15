/*
	'ui_rcd_f.cpp' by Claude SIMON (http://zeusw.org/).

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

#include "ui_rcd_f.h"
#include "kernel.h"

using namespace ui_rcd_f;
using kernel::kernel___;
using nsxpcm::event__;

using namespace kernel;

void ui_rcd_f::apply_record_command__::NSXPCMOnEvent( event__ )
{
	UI().ApplyRecord();
}

void ui_rcd_f::cancel_record_command__::NSXPCMOnEvent( event__ )
{
	if ( UI().Confirm( frdkernl::mDropRecordConfirmation ) )
		UI().DropRecord();
}

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
	record_form__::commands__ &UI,
	nsIDOMDocument *Document )
{
	Register_( Functions, UI.ApplyRecord, Document, "cmdApplyRecord" );
	Register_( Functions, UI.CancelRecord, Document, "cmdCancelRecord" );
}

static void Register_(
	bridge_functions__ &Functions,
	record_form__ &UI )
{
	ui_base::Register( Functions, UI.RecordBox, UI.Document, "boxRecord", nsxpcm::efNone );

	Register_( Functions, UI.Commands, UI.Document );
}


void ui_rcd_f::RegisterRecordFormUI(
	ui::ui___ &UI,
	nsIDOMWindow *Window )
{
	UI.RecordForm.Init( UI, Window );

	Register_( UI, UI.RecordForm );
}


