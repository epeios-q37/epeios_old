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
	const str::string_ &BaseFileName,
	ndbtbl::mode__ Mode,
	bso::bool__ EraseIndexes,
	bso::bool__ Partial )
{
ERRProlog
	fil::mode__ FileMode = fil::m_Undefined;
	str::string	LocatedBaseFileName;
ERRBegin
	reset();

	FileMode = ndbtbl::Convert( Mode );

	S_.FilesgroupID = flm::GetId();

	_table_::InitDynamic( Mode, *((ndbtbl::content_post_initialization_function__ *)NULL) );

	LocatedBaseFileName.Init();
	mbdbsc::BuildLocatedContentBaseFileName( Location, BaseFileName, LocatedBaseFileName );
	S_.SpreadedFileManager.Table.Init( *this, LocatedBaseFileName, Mode, S_.FilesgroupID );
//	Content.Init( LocatedBaseFileName, FileMode ,Partial, S_.FilesgroupID );

	LocatedBaseFileName.Init();
	mbdbsc::BuildLocatedTableRecordFieldIndexBaseFileName( Location, BaseFileName, LocatedBaseFileName );
//	TableRecordFieldIndex.Init( LocatedBaseFileName, S_.UniversalContent, FileMode, EraseIndexes, Partial, S_.FilesgroupID );
	TableRecordFieldIndex.Init( _table_::Content(), FileMode );
	S_.SpreadedFileManager.TableRecordFieldIndex.Init( TableRecordFieldIndex, LocatedBaseFileName, EraseIndexes, FileMode, S_.FilesgroupID );

	LocatedBaseFileName.Init();
	mbdbsc::BuildLocatedTableFieldDatumIndexBaseFileName( Location, BaseFileName, LocatedBaseFileName );
	TableFieldDatumIndex.Init( _table_::Content(), FileMode );
	S_.SpreadedFileManager.TableFieldDatumIndex.Init( TableFieldDatumIndex, LocatedBaseFileName, EraseIndexes, FileMode, S_.FilesgroupID );

	_table_::AddIndex( TableRecordFieldIndex );
	_table_::AddIndex( TableFieldDatumIndex );

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

	Row = TableRecordFieldIndex.StrictSeek( RawDatum, ndbidx::bLesser, 2 );

	while ( Row != NONE ) {
		RawDatum.Init();
		Retrieve( Row, RawDatum );

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

