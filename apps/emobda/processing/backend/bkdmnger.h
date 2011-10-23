/*
	'bkdmnger.h' by Claude SIMON (http://zeusw.org/).

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

// BaCKend MaNGER

#ifndef BKDMNGER__INC
#define BKDMNGER__INC

#include "mbdmng.h"

#include "fblbkd.h"

#define BKDMNGER_MANAGER_PREFIX	"mbd"
#define BKDMNGER_MANAGER_NAME	"manager"

namespace bkdmnger {
	extern const char *UserDefinedBackendVersion;	// To define by the user.

	void Inform( fblbkd::backend_ &Backend );

	typedef mbdmng::manager_	_manager_;

	class manager_
	: public _manager_
	{
	public:
		struct s
		: public _manager_::s
		{};
		manager_( s &S )
		: _manager_( S )
		{}
		void HANDLE(
			fblbkd::backend &Backend,
			fblbkd::untyped_module &Module,
			fblbkd::index__ Index,
			fblbkd::command__ Command,
			fblbkd::request__ &Request,
			void *UP );
		static void NOTIFY( fblbkd::untyped_module &Module );
		static const char *PREFIX;
		static const char *NAME;
		static void RAW_MESSAGES( lcl::strings_ &Messages );
		_manager_ *OBJECT( void )
		{
			return this;
		}
		void Init( void );
	};

	E_AUTO(	manager );

//	lcl::strings_ &GetMessages( void );
}


#endif
