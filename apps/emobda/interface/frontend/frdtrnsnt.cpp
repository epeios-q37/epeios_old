/*
	'frdtrnsnt.cpp' by Claude SIMON (http://zeusw.org/).

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

#include "frdtrnsnt.h"

using namespace frdtrnsnt;

#define CASE( context )	\
case c##context:\
	Label = #context;\
	break

const char *frdtrnsnt::GetContextLabel( context__ Context )
{
	const char *Label = NULL;

	switch( Context ){
	CASE( StructureManagement );
	CASE( DatabaseIdentification );
	CASE( DatabaseSelection );
	CASE( BackendSelection );
	CASE( RecordInput );
	default:
		ERRu();
	}

	return Label;
}


