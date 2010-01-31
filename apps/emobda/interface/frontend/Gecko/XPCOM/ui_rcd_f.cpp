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
	ui___ &UI,
	command__<ui___> &Command,
	nsIDOMDocument *Document,
	const char *Id )
{
	ui_base::Register( UI, Command, Document, Id );
}

static void Register_(
	ui___ &UI,
	record_form__::commands__ &CUI,
	nsIDOMDocument *Document )
{
	Register_( UI, CUI.ApplyRecord, Document, "cmdApplyRecord" );
	Register_( UI, CUI.CancelRecord, Document, "cmdCancelRecord" );
}

static void Register_(
	ui___ &UI,
	record_form__ &RFUI )
{
	ui_base::Register( UI, RFUI.RecordBox, RFUI.Document, "boxRecord", nsxpcm::efNone );

	Register_( UI, RFUI.Commands, RFUI.Document );
}


void ui_rcd_f::RegisterRecordFormUI(
	ui___ &UI,
	nsIDOMWindow *Window )
{
	UI.RecordForm.Init( UI, Window );

	Register_( UI, UI.RecordForm );
}


