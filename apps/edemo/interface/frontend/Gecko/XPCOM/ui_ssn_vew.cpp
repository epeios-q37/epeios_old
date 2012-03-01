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

#include "ui_ssn_vew.h"

#include "ui.h"
#include "dmoinf.h"
#include "trunk.h"

using namespace ui_ssn_vew;
using namespace ui_base;

using nsxpcm::event__;

using trunk::trunk___;

void ui_ssn_vew::get_directory_eh__::NSXPCMOnEvent( nsxpcm::event__ Event )
{
	Trunk().GetDirectoryName();
}

/* Registrations */

/* 'broadcaster's */

#define R( name ) Trunk, Broadcasters.name.Init( Trunk, Trunk.UI().SessionView.Window(), #name );
static void Register_(
	trunk___ &Trunk,
	session_view__::broadcasters__ &Broadcasters,
	nsIDOMWindow *Window )
{
}

#undef R

# define R( name )\
	EventHandlers.name.Init( Trunk );\
	nsxpcm::Attach( Trunk.UI().SessionView.Document(), #name, EventHandlers.name );
static void Register_(
	trunk___ &Trunk,
	session_view__::event_handlers__ &EventHandlers,
	nsIDOMWindow *Window )
{
	R( ehGetDirectory );
}

#undef R

#define R( name ) Widgets.name.Init( Trunk, Trunk.UI().SessionView.Window(), #name );

static void Register_(
	trunk___ &Trunk,
	session_view__::widgets__ &Widgets,
	nsIDOMWindow *Window )
{
	R( txbDirectorySymbolicName );
	R( txbDirectory );
	R( txbAutocomplete );
}

void ui_ssn_vew::session_view__::Init(
	nsIDOMWindow *Window,
	trunk::trunk___ &Trunk )
{
	_session_view__::Init( Window, Trunk );
	_Trunk = &Trunk;
}

void ui_ssn_vew::RegisterSessionViewUI(
	trunk___ &Trunk,
	nsIDOMWindow *Window )
{
	Trunk.UI().SessionView.Init( Window, Trunk );

	xulfsv::RegisterSessionViewUI( Trunk, Window );

	Register_( Trunk, Trunk.UI().SessionView.Broadcasters, Window );
	Register_( Trunk, Trunk.UI().SessionView.EventHandlers, Window );
	Register_( Trunk, Trunk.UI().SessionView.Widgets, Window );

	nsxpcm::PatchOverallBadCommandBehavior( Trunk.UI().SessionView.Document() );
}
