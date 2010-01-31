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

void ui_rcd_v::record_view__::HandleTextboxContent( void )
{
ERRProlog
	str::string Value;
	bso::ulong__ Number;
	epeios::row__ Error = NONE;
ERRBegin
	Value.Init();

	RecordNumberTextbox.GetValue( Value );

	Number = Value.ToUL( &Error );

	if ( Error != NONE )
		UI().Alert( frdkernl::mBadRecordPosition );
	else
		UI().GoToRecordPosition( Number );
ERRErr
ERREnd
ERREpilog
}

void ui_rcd_v::record_number_textbox__::NSXPCMOnEvent( event__ Event )
{
	switch ( Event ) {
	case nsxpcm::eKeyPress:
		PRUint32 KeyCode;

		if ( KeyEvent().GetKeyCode( &KeyCode ) != NS_OK )
			ERRs();

		if ( KeyCode == KeyEvent().DOM_VK_RETURN )
			UI().RecordView.HandleTextboxContent();

		break;
	case nsxpcm::eBlur:
		if ( !IsEventImbricated() )
			UI()._SwitchTo( ui::cRecordView );
		break;
	default:
		ERRc();
		break;
	}
}

void ui_rcd_v::first_record_command__::NSXPCMOnEvent( event__ )
{
	UI().GoToFirstRecord();
}

void ui_rcd_v::previous_record_command__::NSXPCMOnEvent( event__ )
{
	UI().GoToPreviousRecord();
}

void ui_rcd_v::next_record_command__::NSXPCMOnEvent( event__ )
{
	UI().GoToNextRecord();
}

void ui_rcd_v::last_record_command__::NSXPCMOnEvent( event__ )
{
	UI().GoToLastRecord();
}

/* UI Registrations */

static void Register_(
	ui___ &UI,
	broadcaster__<ui___> &Broadcaster,
	nsIDOMDocument *Document,
	const char *Id )
{
	ui_base::Register( UI, Broadcaster, Document, Id, nsxpcm::efNone );
}


static void Register_(
	ui___ &UI,
	record_view__::broadcasters__ &BUI,
	nsIDOMDocument *Document )
{
	Register_( UI, BUI.NotFirstRecord, Document, "bcrNotFirstRecord" );
	Register_( UI, BUI.NotLastRecord, Document, "bcrNotLastRecord" );
}

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
	record_view__::commands__ &CUI,
	nsIDOMDocument *Document )
{
	Register_( UI, CUI.FirstRecord, Document, "cmdFirstRecord" );
	Register_( UI, CUI.PreviousRecord, Document, "cmdPreviousRecord" );
	Register_( UI, CUI.NextRecord, Document, "cmdNextRecord" );
	Register_( UI, CUI.LastRecord, Document, "cmdLastRecord" );
}

static void Register_(
	ui___ &UI,
	record_view__ &RVUI )
{
	Register_( UI, RVUI.Broadcasters, RVUI.Document );
	Register_( UI, RVUI.Commands, RVUI.Document );

	ui_base::Register( UI, RVUI.RecordNumberTextbox, RVUI.Document, "txbRecordNumber", nsxpcm::efKeyPress | nsxpcm::efBlur );
	ui_base::Register( UI, RVUI.RecordsAmountDescription, RVUI.Document, "dscRecordsAmount" );
	ui_base::Register( UI, RVUI.RecordBox, RVUI.Document, "boxRecord", nsxpcm::efNone );

}

void ui_rcd_v::RegisterRecordViewUI(
	ui___ &UI,
	nsIDOMWindow *Window )
{
	UI.RecordView.Init( UI, Window );

	Register_( UI, UI.RecordView );
}


