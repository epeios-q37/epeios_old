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

static rgstry::multi_level_registry Registry_;
static rgstry::level__ RootLevel_ = RGSTRY_UNDEFINED_LEVEL;
static STR_BUFFER___ Translation_;

rgstry::entry sclrgstry::Parameters( "Parameters" );

rgstry::entry sclrgstry::LocaleFileName( "LocaleFileName", Parameters );
rgstry::entry sclrgstry::Language( "Language", Parameters );

bso::bool__ sclrgstry::IsRegistryReady( void )
{
	return RootLevel_ != RGSTRY_UNDEFINED_LEVEL;
}

const rgstry::multi_level_registry_ &sclrgstry::GetRegistry( void )
{
	return Registry_;
}

static rgstry::status__ FillRegistry_(
	const char *FileName,
	const char *RootPath,
	rgstry::context___ &Context )
{
	rgstry::status__ Status = rgstry::s_Undefined;
	rgstry::row__ Root = Registry_.GetRoot( RootLevel_) ;

	if ( ( Status = rgstry::FillRegistry( FileName, xpp::criterions___(), RootPath, Registry_.GetRegistry( RootLevel_), Root, Context ) ) == rgstry::sOK )
		Registry_.SetRoot( RootLevel_, Root );

	return Status;
}

bso::bool__ sclrgstry::GetValue(
	const rgstry::entry_ &Entry,
	const rgstry::tags_ &Tags,
	str::string_ &Value )
{
	bso::bool__ Missing = true;
ERRProlog
	str::string TargetedPath;
ERRBegin
	TargetedPath.Init();

	Missing = Entry.GetValue( Registry_, Tags, Value );
ERRErr
ERREnd
ERREpilog
	return Missing;
}

bso::bool__ sclrgstry::GetValues(
	const rgstry::entry_ &Entry,
	const rgstry::tags_ &Tags,
	str::strings_ &Values )
{
	bso::bool__ Missing = true;
ERRProlog
	str::string TargetedPath;
ERRBegin
	TargetedPath.Init();

	Missing = Entry.GetValues( Registry_, Tags, Values );
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

static inline void ReportBadOrNoValueForRegistryEntryError_(
	const rgstry::entry_ &Entry,
	const rgstry::tags_ &Tags )
{
ERRProlog
	str::string Translation;
	str::string Path;
ERRBegin
	Translation.Init();

	GetTranslation_( "BadOrNoValueForRegistryEntry", Translation );

	Path.Init();
	lcl::ReplaceTag( Translation, 1, Entry.GetPath( Tags, Path ) );

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

static bso::bool__ LoadRegistry_(
	const char *Affix,
	const char *RegistryRootPath,
	const char *FilePath,
	bso::bool__ ReportUnableToFindFile )
{
	bso::bool__ Success = false;
ERRProlog
	rgstry::context___ Context;
	FNM_BUFFER___ FNMBuffer;
ERRBegin
	Context.Init();

	switch ( FillRegistry_( fnm::BuildFileName( FilePath, Affix, REGISTRY_FILE_EXTENSION, FNMBuffer ), RegistryRootPath, Context ) ) {
	case rgstry::sOK:
		Success = true;
		break;
	case rgstry::sUnableToOpenFile:
		if ( !ReportUnableToFindFile )
			break;
	default:
		ReportConfigurationFileParsingError_( Context );
		ERRExit( EXIT_FAILURE );
		break;
	}

ERRErr
ERREnd
ERREpilog
	return Success;
}

void sclrgstry::LoadRegistry(
	const char *Affix,
	const char *RegistryRootPath,
	const char *FileSuggestedPath )
{
ERRProlog
	DIR_BUFFER___ DIRBuffer;
ERRBegin

	if ( LoadRegistry_( Affix, RegistryRootPath, "", false ) )
		ERRReturn;

	if ( LoadRegistry_( Affix, RegistryRootPath, FileSuggestedPath, false ) )
		ERRReturn;

	if ( !LoadRegistry_( Affix, RegistryRootPath, dir::GetSelfPath( DIRBuffer ), true ) )
		ERRc();	// Si échec fonction ci-dessus, aurait dû sortir du programme avec signalement du message d'erreur à cause du 'true'.
ERRErr
ERREnd
ERREpilog
}

const str::string_ &sclrgstry::GetOptionalRegistryValue(
	const rgstry::entry_ &Entry,
	const rgstry::tags_ &Tags,
	str::string_ &Value,
	bso::bool__ *Missing )
{
	if ( !GetValue( Entry, Tags, Value ) )
		if ( Missing != NULL )
			*Missing = true;
	
	return Value;
}

const char *sclrgstry::GetOptionalRegistryValue(
	const rgstry::entry_ &Entry,
	const rgstry::tags_ &Tags,
	STR_BUFFER___ &Buffer,
	bso::bool__ *Missing )
{
ERRProlog
	str::string Value;
	bso::bool__ LocalMissing = false;
ERRBegin
	Value.Init();

	GetOptionalRegistryValue( Entry, Tags, Value, &LocalMissing );

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
	const rgstry::entry_ &Entry,
	const rgstry::tags_ &Tags,
	str::string_ &Value )
{
	if ( !GetValue( Entry, Tags, Value ) ) {
		ReportBadOrNoValueForRegistryEntryError_( Entry, Tags );
		ERRExit( EXIT_FAILURE );
	}

	return Value;
}

const char *sclrgstry::GetMandatoryRegistryValue(
	const rgstry::entry_ &Entry,
	const rgstry::tags_ &Tags,
	STR_BUFFER___ &Buffer )
{
ERRProlog
	str::string Value;
ERRBegin
	Value.Init();

	GetMandatoryRegistryValue( Entry, Tags, Value );

	Value.Convert( Buffer );
ERRErr
ERREnd
ERREpilog
	return Buffer;
}

template <typename t> static bso::bool__ GetRegistryUnsignedNumber_(
	const rgstry::entry_ &Entry,
	const rgstry::tags_ &Tags,
	t Limit,
	t &Value )
{
	bso::bool__ Present = false;
ERRProlog
	str::string RawValue;
	epeios::row__ Error = NONE;
ERRBegin
	RawValue.Init();

	if ( !( Present = GetValue( Entry, Tags, RawValue ) ) )
		ERRReturn;

	RawValue.ToNumber( Limit, Value, &Error );

	if ( Error != NONE ) {
		ReportBadOrNoValueForRegistryEntryError_( Entry, Tags );
		ERRExit( EXIT_FAILURE );
	}
ERRErr
ERREnd
ERREpilog
	return Present;
}

template <typename t> static bso::bool__ GetRegistrySignedNumber_(
	const rgstry::entry_ &Entry,
	const rgstry::tags_ &Tags,
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

	if ( !( Present = GetValue( Entry, Tags, RawValue ) ) )
		ERRReturn;

	RawValue.ToNumber( UpperLimit, LowerLimit, Value, &Error );

	if ( Error != NONE ) {
		ReportBadOrNoValueForRegistryEntryError_( Entry, Tags );
		ERRExit( EXIT_FAILURE );
	}
ERRErr
ERREnd
ERREpilog
	return Present;
}

#define RUN( name, type )\
	type sclrgstry::GetMandatoryRegistry##name(\
		const rgstry::entry_ &Entry,\
		const rgstry::tags_ &Tags,\
		type Limit  )\
	{\
		type Value;\
\
		if ( !GetRegistryUnsignedNumber_( Entry, Tags, Limit, Value ) ) {\
			ReportBadOrNoValueForRegistryEntryError_( Entry, Tags );\
			ERRExit( EXIT_FAILURE );\
		}\
\
		return Value;\
	}\
	type sclrgstry::GetRegistry##name(\
		const rgstry::entry_ &Entry,\
		const rgstry::tags_ &Tags,\
		type DefaultValue,\
		type Limit )\
	{\
		type Value;\
\
		if ( !GetRegistryUnsignedNumber_( Entry, Tags, Limit, Value ) )\
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
		const rgstry::entry_ &Entry,\
		const rgstry::tags_ &Tags,\
		type Min,\
		type Max)\
	{\
		type Value;\
\
		if ( !GetRegistrySignedNumber_( Entry, Tags, Min, Max, Value ) ) {\
			ReportBadOrNoValueForRegistryEntryError_( Entry, Tags );\
			ERRExit( EXIT_FAILURE );\
		}\
		return Value;\
	}\
	type sclrgstry::GetRegistry##name(\
		const rgstry::entry_ &Entry,\
		const rgstry::tags_ &Tags,\
		type DefaultValue,\
		type Min,\
		type Max )\
	{\
		type Value;\
\
		if ( !GetRegistrySignedNumber_( Entry, Tags, Min, Max, Value ) )\
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

		RootLevel_ = Registry_.AddEmbeddedLevel();

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
