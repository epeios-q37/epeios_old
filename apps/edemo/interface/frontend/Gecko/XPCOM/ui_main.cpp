/*
	'ui_main.cpp' by Claude SIMON (http://zeusw.org/).

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

#include "ui_main.h"

#include "ui.h"
#include "dmoinf.h"
#include "trunk.h"

using namespace ui_main;
using namespace ui_base;
using trunk::trunk___;

using nsxpcm::event__;

void ui_main::main__::Init(
	nsIDOMWindow *Window,
	trunk::trunk___ &Trunk )
{
	_main__::Init( Window, Trunk );
	_Trunk = &Trunk;
}

/* Registrations */

/* 'broadcaster's */

static void Register_(
	trunk___ &Trunk,
	broadcaster__ &Broadcaster,
	const char *Id )
{
	Broadcaster.Init( Trunk, Trunk.UI().Main.Window(), Id );
}

#define R( name ) Register_( Trunk, Broadcasters.name, #name );

static void Register_(
	trunk___ &Trunk,
	main__::broadcasters__ &Broadcasters,
	nsIDOMWindow *Window )
{
}

static void Register_(
	trunk___ &Trunk,
	main__::event_handlers__ &EventHandlers,
	nsIDOMWindow *Window )
{
}

static void Register_(
	trunk___ &Trunk,
	main__::widgets__ &Widgets,
	nsIDOMWindow *Window )
{
}

void ui_main::RegisterMainUI(
	trunk___ &Trunk,
	nsIDOMWindow *Window )
{

	nsCOMPtr<nsxpcm::clh__>CLH;

	nsxpcm::CreateInstance( CLH_CONTRACTID, CLH );

	Trunk.UI().Main.Init( Window, Trunk );

	xulfmn::RegisterMainUI( Trunk, Window );

	Register_( Trunk, Trunk.UI().Main.Broadcasters, Window );
	Register_( Trunk, Trunk.UI().Main.EventHandlers, Window );
	Register_( Trunk, Trunk.UI().Main.Widgets, Window );

	nsxpcm::PatchOverallBadCommandBehavior( Trunk.UI().Main.Document() );

	Trunk.UpdateUI();
}
