/*
	'ui_main.h' by Claude SIMON (http://zeusw.org/).

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

#ifndef UI_MAIN__INC
# define UI_MAIN__INC

# include "xulfmn.h"
# include "ui_base.h"

namespace ui_main {

	typedef xulfmn::main__ _main__;
	
	struct main__
	: public _main__
	{
	private:
		trunk::trunk___ *_Trunk;
	public:
		struct broadcasters__
		: public _main__::broadcasters__
		{} Broadcasters;
		struct event_handlers__
		: public _main__::event_handlers__
		{} EventHandlers;
		struct widgets__
		: public _main__::widgets__
		{
		} Widgets;
		main__( void )
		: _main__( Broadcasters, EventHandlers, Widgets )
		{
			_Trunk = NULL;
		}
		void Init(
			nsIDOMWindow *Window,
			trunk::trunk___ &Trunk );
		void Update( void )
		{
			_main__::Update();
		}
		trunk::trunk___ &Trunk( void )
		{
			if ( _Trunk == NULL )
				ERRc();

			return *_Trunk;
		}
	};

	void RegisterMainUI(
		trunk::trunk___ &Trunk,
		nsIDOMWindow *Window );
}

#endif