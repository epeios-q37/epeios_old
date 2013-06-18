/*
	'ui_main.cpp' by Claude SIMON (http://zeusw.org/).

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

#include "ui_main.h"

#include "ui.h"
#include "sktinf.h"
#include "trunk.h"

using namespace ui_main;
using namespace ui_base;
using trunk::trunk___;

using nsxpcm::event__;

void ui_main::main__::Update( void )
{
	_main__::Update();
}

#define I( name )\
	name.Init( Trunk, nsxpcm::supports__( Trunk.UI().Main().Window(), "bdc" #name ) )

void ui_main::broadcasters__::Init( trunk::trunk___ &Trunk )
{
	_broadcasters__::Init( Trunk );
}

#undef I


static void InitAndRegister_(
	trunk___ &Trunk,
	ui_base::event_handler__ &EventHandler,
	const char *Id )
{
	EventHandler.Init( Trunk );
	nsxpcm::AttachEventHandler( Trunk.UI().Main.Document(), Id, EventHandler );
}

#define IR( name )\
	InitAndRegister_( Trunk, name, "eh" #name );	

void ui_main::event_handlers__::Init( trunk::trunk___ &Trunk )
{
	_event_handlers__::Init( Trunk );
}

#undef IR


#define I( name )\
	name.Init( Trunk, nsxpcm::supports__( Trunk.UI().Main().Window(), #name ) );

void ui_main::widgets__::Init( trunk::trunk___ &Trunk )
{
	_widgets__::Init( Trunk );
}

#undef I


void ui_main::main__::Init(
	trunk::trunk___ &Trunk,
	nsIDOMWindow *Window )
{
	_main__::Init( Trunk, Window );

	Broadcasters.Init( Trunk );
	Widgets.Init( Trunk );
	EventHandlers.Init( Trunk );

	nsxpcm::PatchOverallBadCommandBehavior( Trunk.UI().Main.Document() );

	_Trunk = &Trunk;
}
