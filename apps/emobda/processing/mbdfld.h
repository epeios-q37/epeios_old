/*
	'mdbfld.h' by Claude SIMON (http://zeusw.org/).

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

// eMoBDa FieLDs

#ifndef MDBFLD__INC
#define MDBFLD__INC

#include "mbdbsc.h"

#include "str.h"
#include "lstctn.h"
#include "xml.h"

namespace mbdfld {

	using mbdbsc::field_row__;

	class field_ {
	public:
		struct s {
			str::string_::s Name;
		};
		str::string_ Name;
		field_( s &S )
		: Name( S.Name )
		{}
		void reset( bso::bool__ P = true )
		{
			Name.reset( P );
		}
		void plug( mmm::E_MULTIMEMORY_ &MM )
		{
			Name.plug( MM );
		}
		field_ &operator =( const field_ &F )
		{
			Name = F.Name;

			return *this;
		}
		void Init( void )
		{
			reset();

			Name.Init();
		}
	};

	E_AUTO( field );

	typedef lstctn::E_LXCONTAINERt_( field_, field_row__ ) fields_;
	E_AUTO( fields );

	void Dump(
		const field_ &Field,
		xml::writer_ &Writer );

	void Dump(
		const fields_ &Fields,
		xml::writer_ &Writer );

	typedef str::string_ field_description_;
	typedef str::string	field_description;

	typedef ctn::E_XMCONTAINER_( field_description_ ) field_descriptions_;
	E_AUTO( field_descriptions );

}

#endif