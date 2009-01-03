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

#include "nsxpcm.h"

// Predeclaration.
namespace kernel {
	class kernel___;
}

namespace ui {
	typedef nsxpcm::ui_core__ _ui_core__;

	class bridge_functions__
	{
	protected:
		virtual const kernel::kernel___ &K_( void ) const = 0;
		virtual kernel::kernel___ &K_( void ) = 0;
	public:
		const kernel::kernel___ &K( void ) const
		{
			return K_();
		}
		kernel::kernel___ &K( void )
		{
			return K_();
		}
	};

	class bridge__
	{
	private:
		bridge_functions__ *_Functions;
	public:
		bridge__( void )
		{
			_Functions = NULL;
		}
		void Init( bridge_functions__ &Functions )
		{
			_Functions = &Functions;
		}
		const kernel::kernel___ &K( void ) const
		{
			return _Functions->K();
		}
		kernel::kernel___ &K( void )
		{
			return _Functions->K();
		}
	};

	template <typename widget> class _generic__
	: public bridge__,
	  public widget
	{
	public:
		void Init( bridge_functions__ &Functions )
		{
			bridge__::Init( Functions );
			widget::Init();
		}
	};

	typedef _generic__<nsxpcm::textbox__> textbox__;
	typedef _generic__<nsxpcm::button__> button__;
//	typedef _generic__<nsxpcm::_element__<nsIListBoxObject>> listbox__;
	typedef _generic__<nsxpcm::listbox__> listbox__;

	// For widget with 'oncommand' event.
#define UI_C( widget, name )\
	class name\
	: public widget\
	{\
	protected:\
		virtual void NSXPCMOnClick( void ) {}\
		virtual void NSXPCMOnInput( void ) {}\
		virtual void NSXPCMOnFocus( void ) {}\
		virtual void NSXPCMOnBlur( void ) {}\
		virtual void NSXPCMOnSelect( void ) {}\
		virtual void NSXPCMOnCommand( void );\
	}

	// For widget with 'onselect' event.
#define UI_S( widget, name )\
	class name\
	: public widget\
	{\
	protected:\
		virtual void NSXPCMOnClick( void ) {}\
		virtual void NSXPCMOnInput( void ) {}\
		virtual void NSXPCMOnFocus( void ) {}\
		virtual void NSXPCMOnBlur( void ) {}\
		virtual void NSXPCMOnCommand( void ) {}\
		virtual void NSXPCMOnSelect( void );\
	}

	UI_C( textbox__, table_location_textbox__ );
	UI_C( textbox__, field_name_textbox__ );

	UI_C( button__, create_table_button__ );
	UI_C( button__, add_field_button__ );

	UI_S( listbox__, field_list_listbox__ );
	UI_C( button__, remove_field_button__ );

	struct main__
	: public _ui_core__
	{
		table_location_textbox__ TableLocationTextbox;
		create_table_button__ CreateTableButton;
		field_name_textbox__ FieldNameTextbox;
		add_field_button__ AddFieldButton;
		field_list_listbox__ FieldListListbox;
		remove_field_button__ RemoveFieldButton;

		void Init( void )
		{
			_ui_core__::Init();
		}
	};

	struct ui__ {
		main__ Main;
		void Init( void )
		{
			Main.Init();
		}
	};
}

#endif