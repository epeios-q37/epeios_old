/*
	'mbdbkd.h' by Claude SIMON (http://zeusw.org/).

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

// BacKenD CORE

#ifndef BKDCORE__INC
#define BKDCORE__INC

#include "bkdmnger.h"

#include "csdscm.h"

#define BKDCORE_BACKEND_NAME	"emobdabkd"
#define BKDCORE_BACKEND_VERSION	"0.1.0"
#define BKDCORE_COPYRIGHT_YEARS	"2009"

namespace bkdcore {
	typedef bkdmng::backend	_backend;

	class backend
	: public _backend
	{
	private:
		BKDMNG_RAM_MODULE( bkdmnger::manager_ ) Manager;
	public:
		void Init( const lcl::locales_ &Locales )
		{
			_backend::Init( BKDCORE_BACKEND_NAME, BKDCORE_BACKEND_VERSION, Locales );

			bkdmnger::Inform( *this );

			Manager.Init();
			Add( Manager );
		}

	};

	typedef backend backend_;

	typedef csdscm::user_functions__ _backend_functions__;

	class backend_functions__
	: public _backend_functions__
	{
	protected:
		virtual void *CSDPreProcess( flw::ioflow__ &Flow );
		virtual csdscm::action__ CSDProcess(
			flw::ioflow__ &Flow,
			void *UP );
		virtual void CSDPostProcess( void *UP );
	public:
		void Init( void )
		{
			_backend_functions__::Init();
		}
	};
}


#endif