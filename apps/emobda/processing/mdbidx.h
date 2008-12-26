/*
	'mdbidx.h' by Claude SIMON (http://zeusw.org/).

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

#ifndef MDBIDX__INC
#define MDBIDX__INC

#include "dbsidx.h"
#include "mbdbsc.h"

namespace mbdidx {
	typedef dbsidx::sort_function__ _sort_function__;

	class record_row_field_row_sort_function__
	: public _sort_function__
	{
	protected:
		virtual bso::sign__ DBSIDXCompare(
			const dbsidx::datum_ &RawDatum1,
			const dbsidx:: &RawDatum2 )
		{
			return mbdbsc::RecordRowFieldRowCompare( Datum1, Datum2 );
		}
	};
}
	

#endif