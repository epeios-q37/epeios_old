/*
	'ui.h' by Claude SIMON (http://zeusw.org/).

	 This file is part of 'eSketch' software.

    'eSketch' is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'eSketch' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with 'eSketch'.  If not, see <http://www.gnu.org/licenses/>.
*/

// $Id$

#ifndef UI__INC
# define UI__INC

# include "xulfui.h"

# include "ui_base.h"
# include "ui_main.h"
# include "ui_ssn_frm.h"
# include "ui_ssn_vew.h"


#define UI_USERDATA_ATTRIBUTE_NAME	"UserData"

namespace ui {
	using namespace ui_base;

	typedef xulfui::ui___ _ui___;

	class ui___
	: public _ui___
	{
	private:
		trunk::trunk___ *_Trunk;
		display__ _Display;
		void _SetDisplay( display__ Display );
		void _Update( void );
	protected:
		void XULFUIUpdate( void )
		{
			_Update();
		}
	public:
		ui_main::main__ Main;
		ui_ssn_frm::session_form__ SessionForm;
		ui_ssn_vew::session_view__ SessionView;
		void reset( bso::bool__ P = true )
		{
			Main.reset( P );
			SessionForm.reset( P );
			SessionView.reset( P );

			_Trunk = NULL;
			_Display = d_Undefined;
			_ui___::reset( P );
		}
		void Init( trunk::trunk___ &Trunk )
		{
			_Trunk = &Trunk;
			// The initialisation of the different sub-element occurs through the registration.
			_ui___::Init( Main, SessionForm, SessionView );
		}
		void Show( display__ Display )
		{
			_SetDisplay( Display );
			_Update();
		}
	};
}

#endif