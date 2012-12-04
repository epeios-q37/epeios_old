/*
	'binding.cpp' by Claude SIMON (http://zeusw.org/).

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

#include "binding.h"
#include "txf.h"

using namespace binding;

bso::bool__ binding::binding_callback__::XULFUIRegister(
	nsIDOMWindow *Window,
	const str::string_ &Id )
{
	if ( Id == XULFMN_WINDOW_ID ) {
		_Trunk.UI().Main.Init( _Trunk, Window );
		_Trunk.FinalizeLaunching();
	} else if ( Id == XULFSF_PAGE_ID )
		ui_ssn_frm::RegisterSessionFormUI( _Trunk, Window );
	else if ( Id == XULFSV_PAGE_ID )
		ui_ssn_vew::RegisterSessionViewUI( _Trunk, Window );
	else if ( Id == XULFDG_DIALOG_ID )
		xulfdg::Register( _Trunk, Window );
	else
		return false;

	return true;
}
