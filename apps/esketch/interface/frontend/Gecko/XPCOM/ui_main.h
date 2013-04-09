/*
	'ui_main.h' by Claude SIMON (http://zeusw.org/).

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

#ifndef UI_MAIN__INC
# define UI_MAIN__INC

# include "xulfmn.h"
# include "ui_base.h"

namespace ui_main {

	typedef xulfmn::broadcasters__ _broadcasters__;

	class broadcasters__
	: public _broadcasters__
	{
	public:
		void reset( bso::bool__ P = true )
		{
			_broadcasters__::reset( P );
		}
		E_CDTOR( broadcasters__ );
		void Init( trunk::trunk___ &Trunk );
	};

	
	typedef xulfmn::event_handlers__ _event_handlers__;

	class event_handlers__
	: public _event_handlers__
	{
	public:
		void reset( bso::bool__ P = true )
		{
			_event_handlers__::reset( P );
		}
		E_CDTOR( event_handlers__ );
		void Init( trunk::trunk___ &Trunk );
	};


	typedef xulfmn::widgets__ _widgets__;

	class widgets__
	: public _widgets__
	{
	public:
		void reset( bso::bool__ P = true )
		{
			_widgets__::reset( P );
		}
		E_CDTOR( widgets__ );
		void Init( trunk::trunk___ &Trunk );
	};

	
	typedef xulfmn::main__ _main__;

	class main__
	: public _main__
	{
	private:
		trunk::trunk___ *_Trunk;
	public:
		broadcasters__ Broadcasters;
		event_handlers__ EventHandlers;
		widgets__ Widgets;
		void reset( bso::bool__ P = true )
		{
			_main__::reset( P );
			_Trunk = NULL;
		}
		main__( void )
		: _main__( Broadcasters, EventHandlers, Widgets )
		{
			reset( false );
		}
		E_DTOR( main__ )
		void Init(
			trunk::trunk___ &Trunk,
			nsIDOMWindow *Window );
		void Update( void );
		trunk::trunk___ &Trunk( void )
		{
			if ( _Trunk == NULL )
				ERRFwk();

			return *_Trunk;
		}
	};
/*
	void RegisterMainUI(
		trunk::trunk___ &Trunk,
		nsIDOMWindow *Window );
*/
}

#endif