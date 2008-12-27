/*
	'mdbfld.cpp' by Claude SIMON (http://zeusw.org/).

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

#include "mbdfld.h"

using namespace mbdfld;

void mbdfld::Dump(
	const field_ &Field,
	xml::writer_ &Writer )
{
	Writer.PushTag( "Name" );
	Writer.PutValue( Field.Name );
	Writer.PopTag();
}

void mbdfld::Dump(
	const fields_ &Fields,
	xml::writer_ &Writer )
{
	frow__ Row = Fields.First();
	ctn::E_CITEMt( field_, frow__ ) Field;
	bso::integer_buffer__ Buffer;

	Field.Init( Fields );

	while ( Row != NULL ) {
		Writer.PushTag( "Field" );
		Writer.PutAttribute( "id", bso::Convert( *Row, Buffer ) );

		Dump( Field( Row ), Writer );

		Writer.PopTag();

		Row = Fields.Next( Row );
	}
}
