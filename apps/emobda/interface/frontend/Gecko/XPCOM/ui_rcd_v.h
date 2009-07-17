/*
	'ui_rcd_v.h' by Claude SIMON (http://zeusw.org/).

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

#ifndef UI_RCD_V__INC
#define UI_RCD_V__INC

#include "ui_base.h"

namespace ui_rcd_v {
	using namespace ui_base;

	UI_TYPEDEF( box__, record_box__ );

	UI_TYPEDEF( broadcaster__, not_first_record_broadcaster__ );
	UI_TYPEDEF( broadcaster__, not_last_record_broadcaster__ );

	UI_ETYPEDEF( command__, first_record_command__ );
	UI_ETYPEDEF( command__, previous_record_command__ );
	UI_ETYPEDEF( command__, next_record_command__ );
	UI_ETYPEDEF( command__, last_record_command__ );

	UI_ETYPEDEF( textbox__, record_number_textbox__ );

	UI_TYPEDEF( description__, records_amount_description__ );


	struct record_view__
	: public _ui_core__,
	  public bridge__
	{
	public:
		record_box__ RecordBox;
		struct broadcasters__ {
			not_first_record_broadcaster__ NotFirstRecord;
			not_last_record_broadcaster__ NotLastRecord;
		} Broadcasters;
		struct commands__ {
			first_record_command__ FirstRecord;
			previous_record_command__ PreviousRecord;
			next_record_command__ NextRecord;
			last_record_command__ LastRecord;
		} Commands;
		record_number_textbox__ RecordNumberTextbox;
		records_amount_description__ RecordsAmountDescription;
		void Init(
			bridge_functions__ &Functions,
			nsIDOMWindow *Window)
		{
			_ui_core__::Init( Window );
			bridge__::Init( Functions );
		}
		void UpdateInterface(
			bso::size__ RecordPosition,
			bso::size__ RecordsAmount )
		{
			bso::integer_buffer__ Buffer;
			const char *BufferResult = NULL;
			size_t RecordsAmountSize = 0;


			if ( RecordPosition > 1 )
				Broadcasters.NotFirstRecord.Enable();
			else
				Broadcasters.NotFirstRecord.Disable();

			if ( RecordPosition < RecordsAmount )
				Broadcasters.NotLastRecord.Enable();
			else
				Broadcasters.NotLastRecord.Disable();

			RecordNumberTextbox.SetValue( str::string( bso::Convert( RecordPosition, Buffer ) ) );
			RecordsAmountDescription.SetValue( str::string( BufferResult = bso::Convert( RecordsAmount, Buffer ) ) );

			RecordsAmountSize = strlen( BufferResult );

			RecordNumberTextbox.SetSize( RecordsAmountSize );
			RecordNumberTextbox.SetMaxLength( RecordsAmountSize );
		}
		void HandleTextboxContent( void );
	};

	void RegisterRecordViewUI(
		ui::ui___ &UI,
		nsIDOMWindow *Window );
}

#endif