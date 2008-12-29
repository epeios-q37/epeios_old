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

void mbdeng::table_::Init(
	const str::string_ &Location,
	dbstbl::mode__ Mode,
	bso::bool__ Erase,
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
	mbdbsc::BuildLocatedRecordFieldIndexRootFileName( Location, LocatedRootFileName );
	RecordIdFieldRowIndex.Init( LocatedRootFileName, S_.UniversalContent, FileMode, Erase, Partial, S_.FilesgroupID );

	LocatedRootFileName.Init();
	mbdbsc::BuildLocatedFieldDatumIndexRootFileName( Location, LocatedRootFileName );
	FieldRowDatumIndex.Init( LocatedRootFileName, S_.UniversalContent, FileMode, Erase, Partial, S_.FilesgroupID );

	S_.RecordIdFieldRowIndexRow = AddIndex( RecordIdFieldRowIndex );
	S_.FieldRowDatumIndexRow = AddIndex( FieldRowDatumIndex );

ERRErr
ERREnd
ERREpilog
}
