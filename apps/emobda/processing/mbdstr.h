/*
	'mdbstr.h' by Claude SIMON (http://zeusw.org/).

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

// eMoBDa STRucture

#ifndef MDBSTR__INC
#define MDBSTR__INC

#include "mbdfld.h"

#define MBDSTR_STRUCTURE_VERSION	"0.1.0"	// Doit être modifié dés que la structure interne (le format des fichiers) est modifié.

namespace mbdstr {
	using namespace mbdfld;

	class structure_
	{
	public:
		struct s
		{
			str::string_::s Name;
			fields_::s Fields;
		};
		str::string_ Name;
		fields_ Fields;
		structure_( s &S )
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
		structure_ &operator =( const structure_ &D )
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
		field_row__ AddField(
			const field_description_ &Description,
			field_row__ FieldRow )	// Peut être = à 'NONE'.
		{
			FieldRow = Fields.New( FieldRow );

			Fields( FieldRow ).Init();

			Fields( FieldRow ).Name =  Description ;

			return FieldRow;
		}
	};

	E_AUTO( structure );
}

#endif