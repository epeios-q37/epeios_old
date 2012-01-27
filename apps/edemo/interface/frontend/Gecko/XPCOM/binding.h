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
	typedef xulfui::user_functions__ _user_functions__;

	 class binding_functions__
	 : public _user_functions__
	{
	private:
		trunk::trunk___ _Trunk;
		trunk::user_functions__ _UserFunctions;
	protected:
		bso::bool__ XULFUIRegister(
			nsIDOMWindow *Window,
			const str::string_ &Id );
	public:
		void reset( bso::bool__ P = true )
		{
			_user_functions__::reset( P );
			_UserFunctions.reset( P );
			_Trunk.reset( P );
		}
		binding_functions__( void )
		{
			reset( false );
		}
		~binding_functions__( void )
		{
			reset();
		}
		xulftk::status__ Init(
			const char *LauncherIdentification,
			const rgstry::multi_level_registry_ &ConfigurationRegistry,
			const lcl::locale_ &Locale,
			const char *Language )
		{
			_user_functions__::Init( _Trunk );
			_UserFunctions.Init( LauncherIdentification,  _Trunk );

			return _Trunk.Init( _UserFunctions, ConfigurationRegistry, Locale, Language, *this );
		}
	};
}

#endif
