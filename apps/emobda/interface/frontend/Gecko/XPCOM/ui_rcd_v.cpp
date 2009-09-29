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
		UI().Alert( frdkernl::mBadRecordNumber );
	else
		UI().GoToRecordNumber( Number );
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
	bridge_functions__ &Functions,
	broadcaster__ &Broadcaster,
	nsIDOMDocument *Document,
	const char *Id )
{
	ui_base::Register( Functions, Broadcaster, Document, Id, nsxpcm::efNone );
}


static void Register_(
	bridge_functions__ &Functions,
	record_view__::broadcasters__ &UI,
	nsIDOMDocument *Document )
{
	Register_( Functions, UI.NotFirstRecord, Document, "bcrNotFirstRecord" );
	Register_( Functions, UI.NotLastRecord, Document, "bcrNotLastRecord" );
}

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
	Register_( Functions, UI.FirstRecord, Document, "cmdFirstRecord" );
	Register_( Functions, UI.PreviousRecord, Document, "cmdPreviousRecord" );
	Register_( Functions, UI.NextRecord, Document, "cmdNextRecord" );
	Register_( Functions, UI.LastRecord, Document, "cmdLastRecord" );
}

static void Register_(
	bridge_functions__ &Functions,
	record_view__ &UI )
{
	Register_( Functions, UI.Broadcasters, UI.Document );
	Register_( Functions, UI.Commands, UI.Document );

	ui_base::Register( Functions, UI.RecordNumberTextbox, UI.Document, "txbRecordNumber", nsxpcm::efKeyPress | nsxpcm::efBlur );
	ui_base::Register( Functions, UI.RecordsAmountDescription, UI.Document, "dscRecordsAmount" );
	ui_base::Register( Functions, UI.RecordBox, UI.Document, "boxRecord", nsxpcm::efNone );

}

void ui_rcd_v::RegisterRecordViewUI(
	ui::ui___ &UI,
	nsIDOMWindow *Window )
{
	UI.RecordView.Init( UI, Window );

	Register_( UI, UI.RecordView );
}


