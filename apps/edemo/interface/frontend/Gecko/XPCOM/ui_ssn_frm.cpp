/*
	'ui_ssn_frm.cpp' by Claude SIMON (http://zeusw.org/).

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

#include "ui_ssn_frm.h"

#include "ui.h"
#include "dmoinf.h"
#include "trunk.h"

using namespace ui_ssn_frm;
using namespace ui_base;
using trunk::trunk___;

using nsxpcm::event__;

/* Registrations */

/* 'broadcaster's */

static void Register_(
	trunk___ &Trunk,
	session_form__::broadcasters__ &Broadcasters,
	nsIDOMWindow *Window )
{
}

static void Register_(
	trunk___ &Trunk,
	session_form__::event_handlers__ &EventHandlers,
	nsIDOMWindow *Window )
{
}

static void Register_(
	trunk___ &Trunk,
	session_form__::widgets__ &Widgets,
	nsIDOMWindow *Window )
{
}

void ui_ssn_frm::RegisterSessionFormUI(
	trunk___ &Trunk,
	nsIDOMWindow *Window )
{
	Trunk.UI().SessionForm.Init( Window, Trunk );

	xulfsf::RegisterSessionFormUI( Trunk, Window );

	Register_( Trunk, Trunk.UI().SessionForm.Broadcasters, Window );
	Register_( Trunk, Trunk.UI().SessionForm.EventHandlers, Window );
	Register_( Trunk, Trunk.UI().SessionForm.Widgets, Window );

	nsxpcm::PatchOverallBadCommandBehavior( Trunk.UI().SessionForm.Document() );
}
