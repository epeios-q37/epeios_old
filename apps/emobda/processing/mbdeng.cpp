/*
	'mbdeng.cpp' by Claude SIMON (http://zeusw.org/).

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

#include "mbdeng.h"

using namespace mbdeng;

using namespace mbdbsc;

void mbdeng::engine_::Init(
	const str::string_ &Location,
	dbstbl::mode__ Mode,
	bso::bool__ EraseIndexes,
	bso::bool__ Partial )
{
ERRProlog
	mdr::mode__ FileMode = mdr::m_Undefined;
	str::string	LocatedRootFileName;
ERRBegin
	reset();

	FileMode = dbstbl::Convert( Mode );

	S_.FilesgroupID = flm::GetId();

	S_.UniversalContent.Init( Content );
	_table_::Init( S_.UniversalContent, Mode );

	LocatedRootFileName.Init();
	mbdbsc::BuildLocatedContentRootFileName( Location, LocatedRootFileName );
	Content.Init( LocatedRootFileName, FileMode ,Partial, S_.FilesgroupID );

	LocatedRootFileName.Init();
	mbdbsc::BuildLocatedTableRecordFieldIndexRootFileName( Location, LocatedRootFileName );
	TableRecordFieldIndex.Init( LocatedRootFileName, S_.UniversalContent, FileMode, EraseIndexes, Partial, S_.FilesgroupID );

	LocatedRootFileName.Init();
	mbdbsc::BuildLocatedTableFieldDatumIndexRootFileName( Location, LocatedRootFileName );
	TableFieldDatumIndex.Init( LocatedRootFileName, S_.UniversalContent, FileMode, EraseIndexes, Partial, S_.FilesgroupID );

	S_.TableRecordFieldIndexRow = AddIndex( TableRecordFieldIndex );
	S_.TableFieldDatumIndexRow = AddIndex( TableFieldDatumIndex );

ERRErr
ERREnd
ERREpilog
}

epeios::size__ mbdeng::engine_::GetTableRecordsAmount( table_id__ TableId ) const
{
	epeios::size__ Amount = 0;
ERRProlog
	record_row__ Row = NONE;
	field_id__ FieldId = MBDBSC_UNDEFINED_FIELD_ID;
	raw_datum RawDatum;
	record_static_part__ RecordStaticPart;
	record Record;
ERRBegin
	Record.Init( TableId, MBDBSC_UNDEFINED_FIELD_ID, MBDBSC_UNDEFINED_RECORD_ID, datum() );

	RawDatum.Init();
	mbdbsc::Convert( Record, RawDatum );

	Row = TableRecordFieldIndex.Seek( dbsidx::bLesser, RawDatum, 2 );

	while ( Row != NONE ) {
		RawDatum.Init();
		S_.UniversalContent.Retrieve( Row, RawDatum );

		mbdbsc::ExtractRecordStaticPart( RawDatum, RecordStaticPart );

		if ( RecordStaticPart.FieldId != FieldId ) {
			Amount++;

			FieldId = RecordStaticPart.FieldId;
		}

		Row = TableRecordFieldIndex.Next( Row );
	}
ERRErr
ERREnd
ERREpilog
	return Amount;
}

