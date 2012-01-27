/*
	'ui_grid.h' by Claude SIMON (http://zeusw.org/).

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

#ifndef UI_GRID__INC
# define UI_GRID__INC

# include "ui_base.h"

# define UI_GRID_PAGE_ID	"pgeGrid"

namespace ui_grid {
	typedef nsxpcm::tree_view_functions__ _functions__;

	class tree_view_functions__
	: public _functions__
	{
	protected:
		virtual bso::ulong__ NSXPCMGetRowCount( void )
		{
			return 100;
		}
		virtual void NSXPCMGetCellText(
			bso::ulong__ Row,
			bso::ulong__ Column,
			str::string_ &Text )
		{
			bso::integer_buffer__ IBuffer;
			tol::buffer__ TBuffer;

			Text.Append( bso::Convert( Row, IBuffer ) );
			Text.Append( ", " );
			Text.Append( bso::Convert( Column, IBuffer ) );
			Text.Append( " : " );
			Text.Append( tol::Time( TBuffer ) );
		}
	};

	typedef ui_base::tree__ _tree__;

	class tree__
	: public _tree__
	{
	private:
		tree_view_functions__ _Functions;
	public:
		void reset( bso::bool__ P = true )
		{
			_tree__::reset( P );
			_Functions.reset( P );
		}
		E_CVDTOR( tree__ );
		void Init(
			trunk::trunk___ &Trunk,
			nsIDOMWindow *Window,
			const char *Id )
		{
			_Functions.Init();
			_tree__::Init( _Functions, Trunk, Window, Id );
		}
	};

	typedef xulfbs::ui_core__<xulftk::trunk___> _ui_core__;

	struct grid__
	: public _ui_core__
	{
	private:
		trunk::trunk___ *_Trunk;
	public:
		struct broadcasters__
		{} Broadcasters;
		struct event_handlers__
		{} EventHandlers;
		struct widgets__
		{
			tree__ treGrid;
		} Widgets;
		void Init(
			nsIDOMWindow *Window,
			trunk::trunk___ &Trunk );
		void Update( void )
		{}
	};

	void RegisterGridUI(
		trunk::trunk___ &Trunk,
		nsIDOMWindow *Window );
}

#endif