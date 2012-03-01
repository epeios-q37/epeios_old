/*
	'ui_ssn_frm.h' by Claude SIMON (http://zeusw.org/).

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
    along with 'emobda'.  If not, see <http://www.gnu.org/licenses/>.
*/

// $Id$

#ifndef UI_SSN_VEW__INC
# define UI_SSN_VEW__INC

# include "xulfsv.h"
# include "ui_base.h"
# include "ui_grid.h"

namespace ui_ssn_vew {
	typedef xulfsv::session_view__ _session_view__;

	UI_BASE_EH( get_directory_eh__ );

	struct session_view__
	: public _session_view__
	{
	private:
		trunk::trunk___ *_Trunk;
	public:
		ui_grid::grid__ Grid;
		struct broadcasters__
		: public _session_view__::broadcasters__
		{} Broadcasters;
		struct event_handlers__
		: public _session_view__::event_handlers__
		{
			get_directory_eh__ ehGetDirectory;
		} EventHandlers;
		struct widgets__
		: public _session_view__::widgets__
		{
			ui_base::textbox__
				txbDirectorySymbolicName,
				txbDirectory,
				txbAutocomplete;
		} Widgets;
		session_view__( void )
		: _session_view__( Broadcasters, EventHandlers, Widgets )
		{}
		void Init(
			nsIDOMWindow *Window,
			trunk::trunk___ &Trunk );
		void Update( void )
		{
			_session_view__::Update();
			Grid.Update();
		}
	};

	void RegisterSessionViewUI(
		trunk::trunk___ &Trunk,
		nsIDOMWindow *Window );
}

#endif