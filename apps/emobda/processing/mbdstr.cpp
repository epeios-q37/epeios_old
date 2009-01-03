/*
	'mdbstr.cpp' by Claude SIMON (http://zeusw.org/).

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

#include "mbdstr.h"

using namespace mbdstr;

field_row__ mbdstr::structure_::SearchField( const str::string_ &Name ) const
{
	ctn::E_CITEMt( field_, field_row__ ) Field;
	field_row__ Row = Fields.First();

	Field.Init( Fields );

	while ( ( Row != NONE ) && ( Field( Row ).Name != Name ) )
		Row = Fields.Next( Row );

	return Row;

}


