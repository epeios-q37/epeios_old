/*
	'binding.h' by Claude SIMON (http://zeusw.org/).

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

#ifndef BINDING__INC
#define BINDING__INC

#include "dmoinf.h"

#include "trunk.h"

#include "xulfui.h"

namespace binding {
	typedef xulfui::steering_callback__ _steering_callback__;

	 class binding_callback__
	 : public _steering_callback__
	{
	private:
		trunk::trunk___ _Trunk;
		trunk::user_callback__ _UserCallback;
	protected:
		bso::bool__ XULFUIRegister(
			nsIDOMWindow *Window,
			const str::string_ &Id );
	public:
		void reset( bso::bool__ P = true )
		{
			_steering_callback__::reset( P );
			_UserCallback.reset( P );
			_Trunk.reset( P );
		}
		E_CDTOR( binding_callback__ )
		xulftk::status__ Init(
			const char *LauncherIdentification,
			const rgstry::multi_level_registry_ &ConfigurationRegistry,
			const lcl::locale_ &Locale,
			const char *Language )
		{
			_steering_callback__::Init( _Trunk );
			_UserCallback.Init( LauncherIdentification,  _Trunk );

			return _Trunk.Init( _UserCallback, ConfigurationRegistry, Locale, Language, *this );
		}
	};
}

#endif
