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

#include "sclerror.h"

using namespace sclrgstry;

#define REGISTRY_FILE_EXTENSION ".xcfg"

static rgstry::registry Registry_;
static rgstry::row__ Root_ = E_NIL;
static STR_BUFFER___ Translation_;

rgstry::entry___ sclrgstry::Parameters( "Parameters" );

rgstry::entry___ sclrgstry::Language( "Language", Parameters );

rgstry::entry___ sclrgstry::Locale( "Locale", Parameters );

bso::bool__ sclrgstry::IsReady( void )
{
	return Root_ != E_NIL;
}

const rgstry::registry_ &sclrgstry::GetRegistry( void )
{
	return Registry_;
}

rgstry::row__ sclrgstry::GetRoot( void )
{
	return Root_;
}

static rgstry::status__ FillRegistry_(
	flw::iflow__ &Flow,
	const char *Directory,
	const char *RootPath,
	rgstry::context___ &Context )
{
	rgstry::status__ Status = rgstry::s_Undefined;
ERRProlog
	xtf::extended_text_iflow__ XFlow;
ERRBegin
	if ( Root_ != E_NIL )
		ERRFwk();

	XFlow.Init( Flow );

	Status = rgstry::FillRegistry( XFlow, xpp::criterions___( str::string( Directory ) ), RootPath, Registry_, Root_, Context );
ERRErr
ERREnd
ERREpilog
	return Status;
}

bso::bool__ sclrgstry::GetValue(
	const rgstry::tentry__ &Entry,
	str::string_ &Value )
{
	bso::bool__ Missing = false;

	Registry_.GetValue( Entry, Root_, &Missing, Value );

	return !Missing;
}

bso::bool__ sclrgstry::GetValues(
	const rgstry::tentry__ &Entry,
	str::strings_ &Values )
{
	return Registry_.GetValues( Entry, Root_, Values );
}

void sclrgstry::ReportBadOrNoValueForEntryError( const rgstry::tentry__ &Entry )
{
ERRProlog
	lcl::meaning Meaning;
	str::string Path;
ERRBegin
	Meaning.Init();
	Meaning.SetValue( SCLRGSTRY_NAME "_BadOrNoValueForEntry" );

	Path.Init();
	Meaning.AddTag( Entry.GetPath( Path ) );

	sclerror::SetMeaning( Meaning );
ERRErr
ERREnd
ERREpilog
}

static inline void ReportConfigurationFileParsingError_( const rgstry::context___ &Context )
{
ERRProlog
	lcl::meaning Meaning;
	lcl::meaning MeaningBuffer;
ERRBegin
	Meaning.Init();
	Meaning.SetValue( SCLRGSTRY_NAME "_ConfigurationFileParsingError" );

	MeaningBuffer.Init();
	rgstry::GetMeaning( Context, MeaningBuffer );

	Meaning.AddTag( MeaningBuffer );

	sclerror::SetMeaning( Meaning );
ERRErr
ERREnd
ERREpilog
}

void sclrgstry::Load(
	flw::iflow__ &Flow,
	const char *Directory,
	const char *RootPath )
{
ERRProlog
	rgstry::context___ Context;
ERRBegin
	Context.Init();

	if ( FillRegistry_( Flow, Directory, RootPath, Context ) != rgstry::sOK ) {
		ReportConfigurationFileParsingError_( Context );
		ERRExit( EXIT_FAILURE );
	}
ERRErr
ERREnd
ERREpilog
}

const str::string_ &sclrgstry::GetOptionalValue(
	const rgstry::tentry__ &Entry,
	str::string_ &Value,
	bso::bool__ *Missing )
{
	if ( !GetValue( Entry, Value ) )
		if ( Missing != NULL )
			*Missing = true;
	
	return Value;
}

const char *sclrgstry::GetOptionalValue(
	const rgstry::tentry__ &Entry,
	STR_BUFFER___ &Buffer,
	bso::bool__ *Missing )
{
ERRProlog
	str::string Value;
	bso::bool__ LocalMissing = false;
ERRBegin
	Value.Init();

	GetOptionalValue( Entry, Value, &LocalMissing );

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

const str::string_ &sclrgstry::GetMandatoryValue(
	const rgstry::tentry__ &Entry,
	str::string_ &Value )
{
	if ( !GetValue( Entry, Value ) ) {
		ReportBadOrNoValueForEntryError( Entry );
		ERRExit( EXIT_FAILURE );
	}

	return Value;
}

const char *sclrgstry::GetMandatoryValue(
	const rgstry::tentry__ &Entry,
	STR_BUFFER___ &Buffer )
{
ERRProlog
	str::string Value;
ERRBegin
	Value.Init();

	GetMandatoryValue( Entry, Value );

	Value.Convert( Buffer );
ERRErr
ERREnd
ERREpilog
	return Buffer;
}

template <typename t> static bso::bool__ GetUnsignedNumber_(
	const rgstry::tentry__ &Entry,
	t Limit,
	t &Value )
{
	bso::bool__ Present = false;
ERRProlog
	str::string RawValue;
	sdr::row__ Error = E_NIL;
ERRBegin
	RawValue.Init();

	if ( !( Present = GetValue( Entry, RawValue ) ) )
		ERRReturn;

	RawValue.ToNumber( Limit, Value, &Error );

	if ( Error != E_NIL ) {
		ReportBadOrNoValueForEntryError( Entry );
		ERRExit( EXIT_FAILURE );
	}
ERRErr
ERREnd
ERREpilog
	return Present;
}

template <typename t> static bso::bool__ GetSignedNumber_(
	const rgstry::tentry__ &Entry,
	t LowerLimit,
	t UpperLimit,
	t &Value )
{
	bso::bool__ Present = false;
ERRProlog
	str::string RawValue;
	sdr::row__ Error = E_NIL;
ERRBegin
	RawValue.Init();

	if ( !( Present = GetValue( Entry, RawValue ) ) )
		ERRReturn;

	RawValue.ToNumber( UpperLimit, LowerLimit, Value, &Error );

	if ( Error != E_NIL ) {
		ReportBadOrNoValueForEntryError( Entry );
		ERRExit( EXIT_FAILURE );
	}
ERRErr
ERREnd
ERREpilog
	return Present;
}

#define UN( name, type )\
	type sclrgstry::GetMandatory##name(\
		const rgstry::tentry__ &Entry,\
		type Limit  )\
	{\
		type Value;\
\
		if ( !GetUnsignedNumber_( Entry, Limit, Value ) ) {\
			ReportBadOrNoValueForEntryError( Entry );\
			ERRExit( EXIT_FAILURE );\
		}\
\
		return Value;\
	}\
	type sclrgstry::Get##name(\
		const rgstry::tentry__ &Entry,\
		type DefaultValue,\
		type Limit )\
	{\
		type Value;\
\
		if ( !GetUnsignedNumber_( Entry, Limit, Value ) )\
			Value = DefaultValue;\
\
		return Value;\
	}


#ifdef BSO__64BITS_ENABLED
UN( U64, bso::u64__ )
#endif

UN( U32, bso::u32__ )
UN( U16, bso::u16__ )
UN( U8, bso::u8__ )

#define SN( name, type )\
	type sclrgstry::GetMandatory##name(\
		const rgstry::tentry__ &Entry,\
		type Min,\
		type Max)\
	{\
		type Value;\
\
		if ( !GetSignedNumber_( Entry, Min, Max, Value ) ) {\
			ReportBadOrNoValueForEntryError( Entry );\
			ERRExit( EXIT_FAILURE );\
		}\
		return Value;\
	}\
	type sclrgstry::Get##name(\
		const rgstry::tentry__ &Entry,\
		type DefaultValue,\
		type Min,\
		type Max )\
	{\
		type Value;\
\
		if ( !GetSignedNumber_( Entry, Min, Max, Value ) )\
			Value = DefaultValue;\
\
		return Value;\
	}

#ifdef BSO__64BITS_ENABLED
	SN( S64, bso::s64__ )
#endif
	SN( S32, bso::s32__ )
	SN( S16, bso::s16__ )
	SN( S8, bso::s8__ )

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
