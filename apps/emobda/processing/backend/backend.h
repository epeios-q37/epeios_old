/*
	'backend.h' by Claude SIMON (http://zeusw.org/).

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

// BACKEND

#ifndef BACKEND__INC
#define BACKEND__INC

#error "Obsolete due to name conflict ! Use 'mbdbkd' instead !".

#include "mngbkd.h"

#define BACKEND_NAME	"emobda"
#define BACKEND_VERSION	"0.1.0"

namespace backend {
	typedef bkdmng::backend	_backend;

	class backend
	: public _backend
	{
	private:
		BKDMNG_RAM_MODULE( mngbkd::manager ) Manager;
	public:
		void Init( void )
		{
			_backend::Init( BACKEND_NAME, BACKEND_VERSION );
		}

	};

	typedef backend backend_;
}


#endif
