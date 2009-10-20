/*
	'frdrgstry.cpp' by Claude SIMON (http://zeusw.org/).

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

#include "frdrgstry.h"

#include "flf.h"
#include "fnm.h"

using namespace frdrgstry;

#define S		FRDRGSTRY_PATH_SEPARATOR 
#define T		FRDRGSTRY_PATH_TAG_STRING 
#define CTAG	FRDRGSTRY_PATH_TAG_CHAR

#define PARAMETERS	FRDRGSTRY_PARAMETERS_PATH

#define BACKEND		PARAMETERS "Backend" S
const char *frdrgstry::paths::parameters::backend::Location					= BACKEND "Location";
const char *frdrgstry::paths::parameters::backend::Type						= BACKEND "@Type";
const char *frdrgstry::paths::parameters::backend::AccessMode				= BACKEND "@AccessMode";
const char *frdrgstry::paths::parameters::backend::Configuration			= BACKEND "Configuration";

#define DATABASE	PARAMETERS "Database" S
const char *frdrgstry::paths::parameters::database::Path					= DATABASE "Path";
const char *frdrgstry::paths::parameters::database::AccessMode				= DATABASE "@AccessMode";


#define PROFILES	FRDRGSTRY_PROFILES_PATH
#define PROFILE		PROFILES "Profile[name=\"" T "\"]" S

const char *frdrgstry::paths::profiles::CurrentTable						= PROFILE "Table";
const char *frdrgstry::paths::profiles::FallbackProfile						= PROFILES "Fallback";
const char *frdrgstry::paths::profiles::DefaultProfile						= PROFILES "Default";
const char *frdrgstry::paths::profiles::UserProfile							= PROFILES "User";

bso::bool__ frdrgstry::FillRegistry(
	const char *FileName,
	const char *RootPath,
	rgstry::registry_ &Registry,
	rgstry::row__ &RegistryRoot,
	txf::text_oflow__ &Flow )
{
	bso::bool__ Success = false;
ERRProlog
	flf::file_iflow___ FFlow;
	xtf::extended_text_iflow__ XFlow;
	rgstry::row__ Root = NONE;
	epeios::row__ PathErrorRow = NONE;
	rgstry::xcoord ErrorCoord;;
	const char *Directory = NULL;
	FNM_BUFFER___ DirectoryBuffer;
	xml::extended_status__ Status = xml::xs_Undefined;
ERRBegin
	if ( FFlow.Init( FileName, err::hSkip ) != fil::sSuccess ) {
		Flow << txf::nl << "Error opening project file '" << FileName << "' !" << txf::nl;
		ERRReturn;
	}

	FFlow.EOFD( XTF_EOXT );

	XFlow.Init( FFlow );

	Registry.Init();

	Root = Registry.CreateNewRegistry( str::string( "BaseRegistry" ) );

	Directory = fnm::GetLocation( FileName, DirectoryBuffer );

	ErrorCoord.Init();

	if ( rgstry::Parse( XFlow, str::string( Directory ), Registry, Root, Status, ErrorCoord ) == NONE ) {
		Flow << "Error in file '";
		
		if ( ErrorCoord.File.Amount() != 0 )
			Flow << ErrorCoord.File;
		else
			Flow << FileName;
		
		Flow << "' line " << ErrorCoord.Coord().Line << ", column " << ErrorCoord.Coord().Column << " : " << xml::GetLabel( Status ) << " ! " << txf::nl;
		ERRReturn;
	}

	if ( ( RegistryRoot = Registry.Search( str::string( RootPath ), Root, &PathErrorRow ) ) == NONE ) {
		if ( PathErrorRow != NONE )
			ERRc();

		if ( Registry.GetNature( RegistryRoot ) == rgstry::nAttribute )
			ERRc();

		Flow << "Unable to find '" << RootPath << "'." << txf::nl;
		ERRReturn;
	}

	Success = true;
ERRErr
ERREnd
ERREpilog
	return Success;
}

static inline bso::bool__ GetFallbackProfileName_(
	const registry___ &Registry,
	str::string_ &Name )
{
	return GetValue( paths::Profiles.FallbackProfile, Registry, Name );
}

static inline bso::bool__ GetDefaultProfileName_(
	const registry___ &Registry,
	str::string_ &Name )
{
	return GetValue( paths::Profiles.DefaultProfile, Registry, Name );
}

static inline bso::bool__ GetUserProfileName_(
	const registry___ &Registry,
	str::string_ &Name )
{
	return GetValue( paths::Profiles.UserProfile, Registry, Name );
}

static bso::bool__ GetProfileValue_(
	const char *Path,
	const str::string_ &ProfileName,
	const registry___ &Registry,
	str::string_ &Value )
{
	bso::bool__ Success = false;
ERRProlog
str::string WorkPath;
ERRBegin
	if ( ProfileName.Amount() == 0 )
		ERRReturn;

	WorkPath.Init( Path );

	WorkPath.Replace( CTAG, 1, ProfileName );

	Success = Registry.GetValue( WorkPath, Value );
ERRErr
ERREnd
ERREpilog
	return Success;
}

bso::bool__ frdrgstry::GetProfileValue(
	const char *Path,
	const registry___ &Registry,
	str::string_ &Value )
{
	bso::bool__ Success = false;
ERRProlog
	str::string ProfileName;
ERRBegin
	ProfileName.Init();

	if ( GetUserProfileName_( Registry, ProfileName ) )
		if ( Success = GetProfileValue_( Path, ProfileName, Registry, Value ) )
			ERRReturn;

	ProfileName.Init();

	if ( GetDefaultProfileName_( Registry, ProfileName ) )
		if ( Success = GetProfileValue_( Path, ProfileName, Registry, Value ) )
			ERRReturn;

	ProfileName.Init();

	if ( GetFallbackProfileName_( Registry, ProfileName ) )
		Success = GetProfileValue_( Path, ProfileName, Registry, Value );
ERRErr
ERREnd
ERREpilog
	return Success;
}

bso::bool__ frdrgstry::GetProfileIntegerValue(
	const char *Path,
	const registry___ &Registry,
	bso::ulong__ &Id )
{
	bso::bool__ Success = false;
ERRProlog
	str::string Value;
	epeios::row__ Error = NONE;
ERRBegin
	Value.Init();

	if ( GetProfileValue( Path, Registry, Value ) ) {
		Id = Value.ToUL( &Error );

		Success = Error == NONE;
	}
ERRErr
ERREnd
ERREpilog
	return Success;
}

static void SetProfileValue_(
	const char *Path,
	const str::string_ &ProfileName,
	registry___ &Registry,
	const str::string_ &Value )
{
ERRProlog
str::string WorkPath;
ERRBegin
	if ( ProfileName.Amount() == 0 )
		ERRReturn;

	WorkPath.Init( Path );

	WorkPath.Replace( CTAG, 1, ProfileName );

	Registry.SetValue( WorkPath, Value );
ERRErr
ERREnd
ERREpilog
}

void frdrgstry::SetProfileValue(
	const char *Path,
	registry___ &Registry,
	const str::string_ &Value )
{
	bso::bool__ Success = false;
ERRProlog
	str::string ProfileName;
ERRBegin
	ProfileName.Init();

	if ( !GetUserProfileName_( Registry, ProfileName ) )
		if ( !GetDefaultProfileName_( Registry, ProfileName ) )
			if ( !GetFallbackProfileName_( Registry, ProfileName ) )
				ERRReturn;

	SetProfileValue_( Path, ProfileName, Registry, Value );
ERRErr
ERREnd
ERREpilog
}
