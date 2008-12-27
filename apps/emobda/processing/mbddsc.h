/*
	'mdbdsc.h' by Claude SIMON (http://zeusw.org/).

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

// eMoBDa DeSCription

#ifndef MDBDSC__INC
#define MDBDSC__INC

#include "mbdfld.h"
#include "xml.h"

namespace mbddsc {

	class description_
	{
	public:
		struct s
		{
			str::string_::s Name;
			mbdfld::fields_::s Fields;
		};
		str::string_ Name;
		mbdfld::fields_ Fields;
		description_( s &S )
		: Name( S.Name ),
		  Fields( S.Fields )
		{}
		void reset( bso::bool__ P = true )
		{
			Name.reset( P );
			Fields.reset( P );
		}
		void plug( mmm::E_MULTIMEMORY_ &MM )
		{
			Name.plug( MM );
			Fields.plug( MM );
		}
		description_ &operator =( const description_ &D )
		{
			Name = D.Name;
			Fields = D.Fields;

			return *this;
		}
		void Init( void )
		{
			reset();

			Name.Init();
			Fields.Init();
		}
	};

	E_AUTO( description );

	void Dump(
		const description_ &Description,
		xml::writer_ &Writer );
}

#endif