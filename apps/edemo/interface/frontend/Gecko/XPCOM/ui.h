/*
	'ui.h' by Claude SIMON (http://zeusw.org/).

	 This file is part of 'eDemo' software.

    'eDemo' is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'eDemo' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with 'eDemo'.  If not, see <http://www.gnu.org/licenses/>.
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

	enum panel__ {
		pHome,
		pSessionForm,
		pSessionView,
		p_amount,
		p_Undefined
	};

	class ui___
	: public _ui___
	{
	private:
		trunk::trunk___ *_Trunk;
		void _SwitchTo( panel__ Panel );
	protected:
		void XULFUIUpdate( void )
		{
			Main.Update();
			SessionForm.Update();
			SessionView.Update();
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
			_ui___::reset( P );
		}
		void Init( trunk::trunk___ &Trunk )
		{
			_Trunk = &Trunk;
			// The initialisation of the different sub-element occurs through the registration.
			_ui___::Init( Main, SessionForm, SessionView );
		}
		void Show( panel__ Panel )
		{
			_SwitchTo( Panel );
		}
	};
}

#endif