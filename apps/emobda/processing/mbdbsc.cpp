/*
	'mbdbsc.cpp' by Claude SIMON (http://zeusw.org/).

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

#include "mbdbsc.h"

#include "fnm.h"

using namespace mbdbsc;

#define CASE( label )\
	case ft##label:\
	Label = #label;\
	break;


const char *mbdbsc::GetFieldTypeLabel( field_type__ FieldType )
{
	const char *Label = NULL;

	if ( FieldType == ftRelational )	// Field type not yet implemented.
		ERRl();

	switch ( FieldType ) {
		CASE( Alphanumeric );
		CASE( Numeric );
		CASE( Relational );
		default:
			ERRu();
			break;
	}

	return Label;
}

const str::string_ &mbdbsc::BuildBaseDirectoryName(
	const str::string_ &Location,
	const str::string_ &BaseName,
	str::string_ &BaseDirectoryName )
{
ERRProlog
	STR_BUFFER___ LocationBuffer;
	STR_BUFFER___ BaseNameBuffer;
	FNM_BUFFER___ FullDirectoryBuffer;
ERRBegin
	BaseDirectoryName.Append( fnm::BuildFileName( Location.Convert( LocationBuffer ), BaseName.Convert( BaseNameBuffer ), NULL, FullDirectoryBuffer ) );
ERRErr
ERREnd
ERREpilog
	return BaseDirectoryName;
}

static const str::string_ &BuildLocatedRootFileName_(
	const str::string_ &Location,
	const str::string_ &BaseName,
	const char *RootFileName,
	str::string_ &LocatedRootFileName )
{
ERRProlog
	STR_BUFFER___ BaseDirectoryNameBuffer;
	FNM_BUFFER___ FileNameBuffer;
	str::string BaseDirectoryName;
ERRBegin
	BaseDirectoryName.Init();

	LocatedRootFileName.Append( fnm::BuildFileName( BuildBaseDirectoryName( Location, BaseName, BaseDirectoryName ).Convert( BaseDirectoryNameBuffer ), RootFileName, NULL, FileNameBuffer ) );
ERRErr
ERREnd
ERREpilog
	return LocatedRootFileName;
}

const str::string_ &mbdbsc::BuildLocatedContentBaseFileName(
	const str::string_ &Location,
	const str::string_ &BaseName,
	str::string_ &LocalizedRootFileName )
{
	return BuildLocatedRootFileName_( Location, BaseName, MBDBSC_CONTENT_ROOT_FILE_NAME, LocalizedRootFileName );
}

const str::string_ &mbdbsc::BuildLocatedTableRecordFieldIndexBaseFileName(
	const str::string_ &Location,
	const str::string_ &BaseName,
	str::string_ &LocalizedRootFileName )
{
	return BuildLocatedRootFileName_( Location, BaseName, MBDBSC_TABLE_RECORD_FIELD_INDEX_ROOT_FILE_NAME, LocalizedRootFileName );
}

const str::string_ &mbdbsc::BuildLocatedTableFieldDatumIndexBaseFileName(
	const str::string_ &Location,
	const str::string_ &BaseName,
	str::string_ &LocalizedRootFileName )
{
	return BuildLocatedRootFileName_( Location, BaseName, MBDBSC_TABLE_FIELD_DATUM_INDEX_ROOT_FILE_NAME, LocalizedRootFileName );
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
	epeios::row__ O2 )
{
	bso::sign__ Result = 0;

	while ( ( O1 != NONE ) && ( O2 != NONE ) && ( ( Result = Compare_( D1( O1 ), D2( O2 ) ) ) == 0 ) ) {
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

