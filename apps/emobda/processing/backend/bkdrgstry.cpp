/*
	'mbdrgy.cpp' by Claude SIMON (http://zeusw.org/).

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

#include "bkdrgstry.h"
#include "flf.h"

using namespace bkdrgstry;

static rgstry::registry Registry_;
static rgstry::nrow__ Root_ = NONE;

bso::bool__ bkdrgstry::FillRegistry(
	const char *ConfigurationFileName,
	const char *PathRoot,
	txf::text_oflow__ &ErrFlow )
{
	bso::bool__ Success = false;
ERRProlog
	flf::file_iflow___ FIFlow;
	xtf::extended_text_iflow__ XFlow;
	rgstry::nrow__ BaseRoot = NONE;
	rgstry::erow__  AttributeEntryRow = NONE;
	epeios::row__ PathErrorRow = NONE;
	xml::extended_status__ ExtendedStatus = xml::xs_Undefined;
	rgstry::xcoord ErrorCoord;
ERRBegin
	if ( ::Root_ != NONE )
		ERRc();

	if ( FIFlow.Init( ConfigurationFileName, err::hSkip ) != fil::sSuccess ) {
		ErrFlow << "Unable to open file '" << ConfigurationFileName << "' !" << txf::nl;
		ERRReturn;
	}

	FIFlow.EOFD( XTF_EOXT );

	XFlow.Init( FIFlow );

	::Registry_.Init();

	ErrorCoord.Init();

	BaseRoot = rgstry::Parse( XFlow, str::string( "" ), ::Registry_, NONE, ExtendedStatus, ErrorCoord );

	if ( BaseRoot == NONE ) {
		ErrFlow << xml::GetLabel( ExtendedStatus ) << " at line " << ErrorCoord.Coord().Line << ", column " << ErrorCoord.Coord().Column;

		if ( ErrorCoord.File.Amount() )
			ErrFlow << " in file '" << ErrorCoord.File;

		ErrFlow << " !" << txf::nl;

		ERRReturn;
	}

	::Root_ = ::Registry_.SearchPath( str::string( PathRoot ), BaseRoot, AttributeEntryRow, PathErrorRow );

	if ((  AttributeEntryRow != NONE ) || ( PathErrorRow != NONE ) )
		ERRc();

	if ( ::Root_ == NONE ) {
		ErrFlow << "Unable to find '" << PathRoot << "' in configurationfile !" << txf::nl;
		ERRReturn;
	}

	Success = true;
ERRErr
ERREnd
ERREpilog
	return Success;
}


inline bso::bool__ bkdrgstry::GetPathValue(
	const char *Path,
	str::string_ &Value )
{
	return ::Registry_.GetPathValue( rgstry::term( Path ), ::Root_, Value );
}

#define PATH_SEPARATOR MBDRGY_PATH_SEPARATOR

const char *bkdrgstry::paths::Repository = "Repository";

template <typename t> static inline bso::bool__ Convert_(
	const str::string_ &Value,
	t &Number )
{
	epeios::row__ Error = NONE;

	Value.ToNumber( Number, &Error );

	return Error == NONE;
}

template <typename t> static inline bso::bool__ Convert_(
	const char *Path,
	t &Number )
{
	bso::bool__ Exists = false;
ERRProlog
	str::string Value;
	epeios::row__ Error = NONE;
ERRBegin
	Value.Init();

	if ( Exists = GetPathValue( Path, Value ) )
		Exists = Convert_( Value, Number );
ERRErr
ERREnd
ERREpilog
	return Exists;
}

bso::bool__ bkdrgstry::GetNumber(
	const char *Path,
	bso::ulong__ &ULong )
{
	return Convert_( Path, ULong );
}

bso::bool__ bkdrgstry::GetNumber(
	const char *Path,
	bso::ushort__ &UShort )
{
	return Convert_( Path, UShort );
}

bso::bool__ bkdrgstry::GetNumber(
	const char *Path,
	bso::ubyte__ &UByte )
{
	return Convert_( Path, UByte );
}


