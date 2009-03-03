/*
	'ui_rcd_f.cpp' by Claude SIMON (http://zeusw.org/).

	 This file is part of 'emobda' software.

    'emobda' is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'emobda' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with 'emobda'.  If not, see <http://www.gnu.org/licenses/>.
*/

// $Id$

#include "ui_rcd_f.h"
#include "kernel.h"

using namespace ui_rcd_f;
using kernel::kernel___;
using nsxpcm::event__;

using namespace kernel;

/* UI Registrations */

void ui_rcd_f::Register(
	kernel___ &Kernel,
	record_form__ &UI,
	nsIDOMWindow *Window )
{
	UI.Set( Window );
}


