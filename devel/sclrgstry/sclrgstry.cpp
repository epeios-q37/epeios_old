/*
	'sclrgstry' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'sclrgstry' header file ('sclrgstry.h').
	Copyright (C) $COPYRIGHT_DATES$Claude SIMON.
$_RAW_$
	This file is part of the Epeios (http://zeusw.org/epeios/) project.

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.
 
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, go to http://www.fsf.org/
	or write to the:
  
         	         Free Software Foundation, Inc.,
           59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/



//	$Id$

#define SCLRGSTRY__COMPILATION

#include "sclrgstry.h"

class sclrgstrytutor
: public ttr_tutor
{
public:
	sclrgstrytutor( void )
	: ttr_tutor( SCLRGSTRY_NAME )
	{
#ifdef SCLRGSTRY_DBG
		Version = SCLRGSTRY_VERSION "\b\bD $";
#else
		Version = SCLRGSTRY_VERSION;
#endif
		Owner = SCLRGSTRY_OWNER;
		Date = "$Date$";
	}
	virtual ~sclrgstrytutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#include "cio.h"
#include "lcl.h"
#include "dir.h"
#include "fnm.h"

using namespace sclrgstry;

#define REGISTRY_FILE_EXTENSION ".xcfg"

static rgstry::registry Registry_;
static rgstry::row__ Root_ = NONE;
static STR_BUFFER___ Translation_;

const char *sclrgstry::paths::LocaleFileName = "LocaleFileName";
const char *sclrgstry::paths::Language = "Language";

const rgstry::registry_ &sclrgstry::GetRegistry( void )
{
	return Registry_;
}

rgstry::row__ sclrgstry::GetRegistryRoot( void )
{
	if ( Root_ == NONE )
		ERRc();

	return Root_;
}


bso::bool__ sclrgstry::IsRegistryReady( void )
{
	return Root_ != NONE;
}

static rgstry::status__ FillRegistry_(
	const char *FileName,
	const char *RootPath,
	rgstry::context___ &Context )
{
	return rgstry::FillRegistry( FileName, xpp::criterions___(), RootPath, Registry_, Root_, Context );
}

bso::bool__ sclrgstry::GetValue(
	const xpath___ &Path,
	str::string_ &Value )
{
	bso::bool__ Missing = true;
ERRProlog
	str::string TargetedPath;
ERRBegin
	TargetedPath.Init();

	Missing = Registry_.GetValue( Path.GetTargetedPath( TargetedPath ), ::Root_, Value );
ERRErr
ERREnd
ERREpilog
	return Missing;
}

bso::bool__ sclrgstry::GetValues(
	const xpath___ &Path,
		str::strings_ &Values )
{
	bso::bool__ Missing = true;
ERRProlog
	str::string TargetedPath;
ERRBegin
	TargetedPath.Init();

	Missing = Registry_.GetValues( Path.GetTargetedPath( TargetedPath ), ::Root_, Values );
ERRErr
ERREnd
ERREpilog
	return Missing;
}

static str::string_ &GetTranslation_(
	const char *Message,
	str::string_ &Translation )
{
	if ( LocaleRack == NULL )
		ERRc();

	LocaleRack->GetTranslation( Message, SCLRGSTRY_NAME "_", Translation );

	return Translation;
}

static inline void ReportBadOrNoValueForRegistryEntryError_( const xpath___ &Path )
{
ERRProlog
	str::string Translation;
	str::string TargetedPath;
ERRBegin
	Translation.Init();

	GetTranslation_( "BadOrNoValueForRegistryEntry", Translation );

	TargetedPath.Init();
	lcl::ReplaceTag( Translation, 1, Path.GetTargetedPath( TargetedPath ) );

	cio::CErr << Translation << " !" << txf::nl << txf::commit;
ERRErr
ERREnd
ERREpilog
}

static inline void ReportConfigurationFileParsingError_( const rgstry::context___ &Context )
{
ERRProlog
	str::string Translation;
	str::string EmbeddedMessage;
ERRBegin
	Translation.Init();

	GetTranslation_( "ConfigurationFileParsingError", Translation );

	EmbeddedMessage.Init();
	rgstry::GetTranslation( Context, *LocaleRack, EmbeddedMessage );

	lcl::ReplaceTag( Translation, 1, EmbeddedMessage );

	cio::CErr << Translation << " !" << txf::nl << txf::commit;
ERRErr
ERREnd
ERREpilog
}

void sclrgstry::LoadRegistry(
	const char *Affix,
	const char *RootPath )
{
ERRProlog
	rgstry::status__ Status = rgstry::s_Undefined;
	rgstry::context___ Context;
	DIR_BUFFER___ DIRBuffer;
	FNM_BUFFER___ FNMBuffer;
ERRBegin
	Context.Init();

	if ( ( Status = FillRegistry_( fnm::BuildFileName( "", Affix, REGISTRY_FILE_EXTENSION, FNMBuffer ), RootPath, Context ) ) != rgstry::sOK )
		if ( Status == rgstry::sUnableToOpenFile ) {
			Context.Init();
			if ( ( Status = FillRegistry_( fnm::BuildFileName( dir::GetSelfPath( DIRBuffer ), Affix, REGISTRY_FILE_EXTENSION, FNMBuffer ), RootPath, Context ) ) != rgstry::sOK ) {
				ReportConfigurationFileParsingError_( Context );
				ERRExit( EXIT_FAILURE );
			}
		} else {
			ReportConfigurationFileParsingError_( Context );
			ERRExit( EXIT_FAILURE );
		}
ERRErr
ERREnd
ERREpilog
}

const str::string_ &sclrgstry::GetOptionalRegistryValue(
	const xpath___ &Path,
	str::string_ &Value,
	bso::bool__ *Missing )
{
	if ( !GetValue( Path, Value ) )
		if ( Missing != NULL )
			*Missing = true;
	
	return Value;
}

const char *sclrgstry::GetOptionalRegistryValue(
	const xpath___ &Path,
	STR_BUFFER___ &Buffer,
	bso::bool__ *Missing )
{
ERRProlog
	str::string Value;
	bso::bool__ LocalMissing = false;
ERRBegin
	Value.Init();

	GetOptionalRegistryValue( Path, Value, &LocalMissing );

	if ( LocalMissing ) {
		if ( Missing != NULL )
			*Missing = true;
	} else
		Value.Convert( Buffer );
ERRErr
ERREnd
ERREpilog
	return Buffer;
}

const str::string_ &sclrgstry::GetMandatoryRegistryValue(
	const xpath___ &Path,
	str::string_ &Value )
{
	if ( !GetValue( Path, Value ) ) {
		ReportBadOrNoValueForRegistryEntryError_( Path );
		ERRExit( EXIT_FAILURE );
	}

	return Value;
}

const char *sclrgstry::GetMandatoryRegistryValue(
	const xpath___ &Path,
	STR_BUFFER___ &Buffer )
{
ERRProlog
	str::string Value;
ERRBegin
	Value.Init();

	GetMandatoryRegistryValue( Path, Value );

	Value.Convert( Buffer );
ERRErr
ERREnd
ERREpilog
	return Buffer;
}

template <typename t> static bso::bool__ GetRegistryUnsignedNumber_(
	const xpath___ &Path,
	t Limit,
	t &Value )
{
	bso::bool__ Present = false;
ERRProlog
	str::string RawValue;
	epeios::row__ Error = NONE;
ERRBegin
	RawValue.Init();

	if ( !( Present = GetValue( Path, RawValue ) ) )
		ERRReturn;

	RawValue.ToNumber( Limit, Value, &Error );

	if ( Error != NONE ) {
		ReportBadOrNoValueForRegistryEntryError_( Path );
		ERRExit( EXIT_FAILURE );
	}
ERRErr
ERREnd
ERREpilog
	return Present;
}

template <typename t> static bso::bool__ GetRegistrySignedNumber_(
	const xpath___ &Path,
	t LowerLimit,
	t UpperLimit,
	t &Value )
{
	bso::bool__ Present = false;
ERRProlog
	str::string RawValue;
	epeios::row__ Error = NONE;
ERRBegin
	RawValue.Init();

	if ( !( Present = GetValue( Path, RawValue ) ) )
		ERRReturn;

	RawValue.ToNumber( UpperLimit, LowerLimit, Value, &Error );

	if ( Error != NONE ) {
		ReportBadOrNoValueForRegistryEntryError_( Path );
		ERRExit( EXIT_FAILURE );
	}
ERRErr
ERREnd
ERREpilog
	return Present;
}

#define RUN( name, type )\
	type sclrgstry::GetMandatoryRegistry##name(\
		const xpath___ &Path,\
		type Limit  )\
	{\
		type Value;\
\
		if ( !GetRegistryUnsignedNumber_( Path, Limit, Value ) ) {\
			ReportBadOrNoValueForRegistryEntryError_( Path );\
			ERRExit( EXIT_FAILURE );\
		}\
		return Value;\
	}\
	type sclrgstry::GetRegistry##name(\
		const xpath___ &Path,\
		type DefaultValue,\
		type Limit )\
	{\
		type Value;\
\
		if ( !GetRegistryUnsignedNumber_( Path, Limit, Value ) )\
			Value = DefaultValue;\
\
		return Value;\
	}


#ifdef CPE__64_BITS_TYPES_ALLOWED
RUN( ULLong, bso::ullong__ )
#endif

RUN( ULong, bso::ulong__ )
RUN( UShort, bso::ushort__ )
RUN( UByte, bso::ubyte__ )

#define RSN( name, type )\
	type sclrgstry::GetMandatoryRegistry##name(\
		const xpath___ &Path,\
		type Min,\
		type Max)\
	{\
		type Value;\
\
		if ( !GetRegistrySignedNumber_( Path, Min, Max, Value ) ) {\
			ReportBadOrNoValueForRegistryEntryError_( Path );\
			ERRExit( EXIT_FAILURE );\
		}\
		return Value;\
	}\
	type sclrgstry::GetRegistry##name(\
		const xpath___ &Path,\
		type DefaultValue,\
		type Min,\
		type Max )\
	{\
		type Value;\
\
		if ( !GetRegistrySignedNumber_( Path, Min, Max, Value ) )\
			Value = DefaultValue;\
\
		return Value;\
	}

# ifdef CPE__64_BITS_TYPES_ALLOWED
	RSN( SLLong, bso::sllong__ )
#endif
	RSN( SLong, bso::slong__ )
	RSN( SShort, bso::sshort__ )
	RSN( SByte, bso::sbyte__ )

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class sclrgstrypersonnalization
: public sclrgstrytutor
{
public:
	sclrgstrypersonnalization( void )
	{
		Registry_.Init();
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~sclrgstrypersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the ending of the application  */
	}
};


/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

// 'static' by GNU C++.

static sclrgstrypersonnalization Tutor;

ttr_tutor &SCLRGSTRYTutor = Tutor;
