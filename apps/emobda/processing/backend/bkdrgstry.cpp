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
#include "lcl.h"

using namespace bkdrgstry;

static rgstry::registry Registry_;
static rgstry::row__ Root_ = NONE;

bso::bool__ bkdrgstry::FillRegistry(
	const char *FileName,
	const char *RootPath,
	txf::text_oflow__ &Flow )
{
	bso::bool__ Success = false;
ERRProlog
	epeios::row__ PathErrorRow = NONE;
	rgstry::error_details ErrorDetails;
	xml::extended_status__ Status = xml::xs_Undefined;
	rgstry::error__ Error = rgstry::e_Undefined;
	str::string Message;
	lcl::locales DummyLocales;
ERRBegin
	Registry_.Init();

	ErrorDetails.Init();

	switch ( ( Error = rgstry::FillRegistry( FileName, RootPath, Registry_, Root_, ErrorDetails ) ) ) {
	case rgstry::eOK:
		break;
	default:
		// 'Registry_' should contain the 'locales' definition, so, if failed to read it, then we have no way to display translated messages.
		DummyLocales.Init();
		Message.Init();
		Flow << rgstry::GetTranslation( Error, ErrorDetails, str::string( "" ), DummyLocales, Message ) << txf::nl;
		ERRReturn;
		break;
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
	return ::Registry_.GetValue( str::string( Path ), ::Root_, Value );
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


