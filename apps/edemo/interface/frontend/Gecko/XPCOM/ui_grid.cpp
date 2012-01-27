/*
	'ui_ssn_vew.cpp' by Claude SIMON (http://zeusw.org/).

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

#include "ui_grid.h"

#include "ui.h"
#include "trunk.h"

using namespace ui_grid;
using namespace ui_base;

using nsxpcm::event__;

using trunk::trunk___;

/* Registrations */

/* 'broadcaster's */

#define R( name ) Trunk, Broadcasters.name.Init( Trunk, Trunk.UI().SessionForm.Window(), #name );
static void Register_(
	trunk___ &Trunk,
	grid__::broadcasters__ &Broadcasters,
	nsIDOMWindow *Window )
{
}

#undef R

# define R( name )\
	EventHandlers.name.Init( Trunk );\
	nsxpcm::Attach( Trunk.UI().SessionForm.Document(), #name, EventHandlers.name );
static void Register_(
	trunk___ &Trunk,
	grid__::event_handlers__ &EventHandlers,
	nsIDOMWindow *Window )
{
}

#undef R

#define R( name ) Widgets.name.Init( Trunk, Trunk.UI().SessionView.Window(), #name );

static void Register_(
	trunk___ &Trunk,
	grid__::widgets__ &Widgets,
	nsIDOMWindow *Window )
{
	Widgets.treGrid.Init( Trunk, Window, "treGrid" );
}

void ui_grid::grid__::Init(
	nsIDOMWindow *Window,
	trunk::trunk___ &Trunk )
{
	_ui_core__::Init( Window, Trunk );
	_Trunk = &Trunk;
}

void ui_grid::RegisterGridUI(
	trunk___ &Trunk,
	nsIDOMWindow *Window )
{
	Trunk.UI().SessionView.Init( Window, Trunk );

	xulfsv::RegisterSessionViewUI( Trunk, Window );

	Register_( Trunk, Trunk.UI().SessionView.Grid.Broadcasters, Window );
	Register_( Trunk, Trunk.UI().SessionView.Grid.EventHandlers, Window );
	Register_( Trunk, Trunk.UI().SessionView.Grid.Widgets, Window );

	nsxpcm::PatchOverallBadCommandBehavior( Trunk.UI().SessionView.Document() );
}
