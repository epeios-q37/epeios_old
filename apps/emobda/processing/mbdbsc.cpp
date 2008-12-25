/*
	'mdbbsc.cpp' by Claude SIMON (http://zeusw.org/).

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

#include "mdbbsc.h"

using namespace mdbbsc;

void Extract(
	const raw_datum_ &RawDatum,
	record_ &Record )
{
ERRProlog
	record_static_part__ RecordStaticPart;
	datum Datum;
ERRBegin
	if ( RawDatum.Amount() < sizeof( RecordStaticPart ) )
		ERRu();

	RawDatum.Get( RawDatum.First(), sizeof( record_static_part__ ), (dbsbsc::atom__ *)&RecordStaticPart );

	Datum.Init();

	Datum.Append( RawDatum, RawDatum.First( sizeof( record_static_part__ ) ) );
ERRErr
ERREnd
ERREpilog

}
