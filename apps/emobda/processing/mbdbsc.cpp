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

using namespace mbdbsc;


void mbdbsc::Extract(
	const raw_datum_ &RawDatum,
	record_ &Record )
{
ERRProlog
	record_static_part__ RecordStaticPart;
	datum Datum;
ERRBegin
	ExtractRecordStaticPart( RawDatum, RecordStaticPart );

	Datum.Init();

	Datum.Append( RawDatum, RawDatum.First( sizeof( record_static_part__ ) ) );
ERRErr
ERREnd
ERREpilog
}

static inline bso::char__ ToUpper_( bso::char__ C )
{
	if ( ( C > 0 ) && islower( C ) )
		return toupper( C );
	else
		return C;
}

static inline bso::sign__ Compare_(
	bso::char__ C1,
	bso::char__ C2 )
{
	return bso::Compare( ToUpper_( C1 ), ToUpper_( C2 ) );
}

bso::sign__ mbdbsc::Compare(
	const raw_datum_ &D1,
	epeios::row__ O1,
	const raw_datum_ &D2,
	epeios::row__ &O2 )
{
	bso::sign__ Result = 0;

	while ( ( O1 != NONE ) && ( O2 != NONE ) && ( ( Result == Compare_( D1( O1 ), D2( O2 ) ) == 0 ) ) ) {
		O1 = D1.Next( 01 );
		O2 = D2.Next( O2 );
	}

	if ( Result == 0 )
		if ( O1 == NONE )
			if ( O2 == NONE )
				Result = 0;
			else
				Result = 1;
		else
			if ( O2 == NONE )
				Result = -1;
			else
				ERRc();

	return Result;

}

