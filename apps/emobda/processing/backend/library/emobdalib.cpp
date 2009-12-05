/*
	'emobdalib.cpp' by Claude SIMON (http://zeusw.org/).

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

#include "bkdcore.h"
#include "registry.h"

#include "csddls.h"

#define VERSION			BKDCORE_BACKEND_VERSION
#define COPYRIGHT_YEARS	BKDCORE_COPYRIGHT_YEARS

#define NAME			"emobdalib"

// Backend/frontend common data BEGIN

#define DEFAULT_CONFIGURATION_FILENAME	NAME ".xcf"
#define CONFIGURATION_ROOT_PATH	"Configuration[target=\"" NAME "\"]"

#define LOCALES_PATH_ROOT	"Locale[target=\"" NAME "\"]"

const char *bkdmnger::UserDefinedBackendVersion = NAME " V" VERSION" (" __DATE__ " "  __TIME__ ")";

#define INTERAPP_DATA_ID_VERSION	"0.1.0"
#define INTERAPP_DATA_ID_BASE		"EMobDa frontend-backend common data V" INTERAPP_DATA_ID_VERSION

#ifdef XXX_DBG
#	define INTERAPP_DATA_ID	INTERAPP_DATA_ID_BASE "D"
#else
#	define INTERAPP_DATA_ID	INTERAPP_DATA_ID_BASE
#endif

// If modified, don't forget to modify 'INTERAPP_DATA_ID_VERSION' above.
struct data__ {
	const char *Id;
	const char *ConfigurationFileName;
	lcl::locales_ *Locales;
	txf::text_oflow__ *ErrorFlow;
};

// Backend/frontend common data END

static bkdcore::backend_functions__ Functions_;

// For the 'getbkdapi' software.
static csdscm::user_functions__ *LimitedUse_( void )
{
	bkdcore::backend_functions__ *Functions = NULL;
ERRProlog
	lcl::locales Dummy;
ERRBegin
	Dummy.Init();

	Functions_.Init( Dummy );

	Functions = &Functions_;
ERRErr
	ERRRst();	// Error catched to avoid that it goez further.
				// Error reported by the fact that the retruned value is 'NULL'.
ERREnd
ERREpilog
	return Functions;
}

static csdscm::user_functions__ *NormalUse_( data__ &Data )
{
	bkdcore::backend_functions__ *Functions = NULL;
ERRProlog
	fwf::oflow_functions___ *RequestLogFlow = NULL;
	rgstry::error__ Error = rgstry::e_Undefined;
	rgstry::error_details ErrorDetails;
	const char *ConfigurationFileName = NULL;
	txf::text_oflow__ *CErr = NULL;
	str::string LocalesFileName;
	STR_BUFFER___ LocalesFileNameBuffer;
	str::string Message;
ERRBegin
	if ( strcmp( Data.Id, INTERAPP_DATA_ID ) )
		ERRReturn;	// Erreur.

	ConfigurationFileName = Data.ConfigurationFileName;
	CErr = Data.ErrorFlow;

	if ( ConfigurationFileName == NULL )
		ConfigurationFileName = DEFAULT_CONFIGURATION_FILENAME;

	if ( !registry::FillRegistry( ConfigurationFileName, CONFIGURATION_ROOT_PATH, *CErr ) )
		ERRReturn;

	LocalesFileName.Init();

	if ( !registry::GetLocalesFileName( LocalesFileName ) ) {
		*CErr << "No locales file loaded : unable to find '" << registry::paths::Locales << "' entry in configuration file !" << txf::nl;
	}

	ErrorDetails.Init();

	switch( Error = Data.Locales->Init( LocalesFileName.Convert( LocalesFileNameBuffer ), LOCALES_PATH_ROOT, ErrorDetails ) ) {
	case rgstry::eOK:
		break;
	default:
		Message.Init();
		// Weird 'language' parameter, because, if we are here, it's because the 'locales' file could not be correctly open
		// so no message translations are available ...
		*CErr << "Failed to load '" << LocalesFileName <<  "' locales file : " << rgstry::GetTranslation( Error, ErrorDetails, str::string( "" ), *Data.Locales, Message ) << txf::nl;
		break;
	}

	Functions_.Init( *Data.Locales );

	Functions = &Functions_;
ERRErr
	ERRRst();	// Error catched to avoid that it goez further.
				// Error reported by the fact that the retruned value is 'NULL'.
				// Error description is in 'Data.ErrorFlow'.
ERREnd
ERREpilog
	return Functions;
}


csdscm::user_functions__ *csddls::CSDDLSCallback( void *UP )
{
	if ( UP == NULL )
		return LimitedUse_();
	else
		return NormalUse_( *(data__ *)UP );
}



