/*
	'ui_ssn_frm.h' by Claude SIMON (http://zeusw.org/).

	 This file is part of 'eSketch' software.

    'Ssketch' is free software: you can redistribute it and/or modify
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

#ifndef UI_SSN_FRM__INC
# define UI_SSN_FRM__INC

# include "xulfsf.h"
# include "ui_base.h"

namespace ui_ssn_frm {

	typedef xulfsf::session_form__ _session_form__;

	struct session_form__
	: public _session_form__
	{
	public:
		struct broadcasters__
		: public _session_form__::broadcasters__
		{} Broadcasters;
		struct event_handlers__
		: public _session_form__::event_handlers__
		{} EventHandlers;
		struct widgets__
		: public _session_form__::widgets__
		{} Widgets;
		session_form__( void )
		: _session_form__( Broadcasters, EventHandlers, Widgets )
		{}
		void Update( void )
		{
			_session_form__::Update();
		}
	};

	void RegisterSessionFormUI(
		trunk::trunk___ &Trunk,
		nsIDOMWindow *Window );
}

#endif