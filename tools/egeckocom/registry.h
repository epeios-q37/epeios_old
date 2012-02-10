/*
	'registry.h' by Claude SIMON (http://zeusw.org/).

	 This file is part of 'eGeckoCOM' software.

    'eGeckoCOM' is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'eGeckoCOM' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with 'eGeckoCOM'.  If not, see <http://www.gnu.org/licenses/>.
*/

// $Id$

#ifndef REGISTRY__INC
# define REGISTRY__INC

# include "sclrgstry.h"

namespace registry {

	using namespace sclrgstry;

	extern rgstry::entry__ Components;
	extern rgstry::entry__ TaggedComponent;
}

#endif
