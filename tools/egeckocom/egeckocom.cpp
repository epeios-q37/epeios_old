/*
	'egeckocom.cpp' by Claude SIMON (http://zeusw.org/).

	 This file is part of 'eGeckoCOM' software.

    'eGeckoCOM' is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'eGeckoCOM' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with 'eGeckoCOM'.  If not, see <http://www.gnu.org/licenses/>.
*/

// $Id$

#include "egeckocom.h"

#include "nsxpcm.h"

#include "geckof.h"

#include "scllocale.h"
#include "sclrgstry.h"
#include "sclmisc.h"

#include "flx.h"


#define COMPONENT_VERSION	"alpha 1"

#define NAME "egeckocom"

#define EGECKOCOM_CONTRACTID "@zeusw.org/egeckocom;" COMPONENT_VERSION
#define EGECKOCOM_CLASSNAME "Generic Epeios component"
#define EGECKOCOM_CID  EIGECKOCOM_IID

static mtx::mutex_handler__ _Mutex = MTX_INVALID_HANDLER;	// To protect access to following object.
static geckoo::user_functions__ *_CurrentSteering = NULL;
static bso::bool__ IsInitialized_ = false;

static str::string COutString_;
static flx::E_STRING_OFLOW_DRIVER___ COutDriver_;

static str::string CErrString_;
static flx::E_STRING_OFLOW_DRIVER___ CErrDriver_;

#define MESSAGE_UNABLE_TO_REGISTER_ELEMENT		"UnableToRegisterElement"
#define MESSAGE_UNABLE_TO_OPEN_COMPONENT		"UnableToOpenComponent"
#define MESSAGE_BAD_LONE_REGISTRATION_CONTEXT	"BadLoneRegistrationContext"
#define MESSAGE_LONE_REGISTRATION_FAILURE		"LoneRegistrationFailure"

class egeckocom
: public EIGeckoCOM
{
private:
	geckof::gecko_wrapper___ _Wrapper;
	geckoo::shared_data__ _Data;			// Only used by the master window.
	~egeckocom();
public:
  NS_DECL_ISUPPORTS
  NS_DECL_EIGECKOCOM

  egeckocom();
private:
protected:
  /* additional members */
};


#define RP	\
		nsresult NSResult = NS_OK;\
	ERRProlog\
		str::string ErrorMessage;\
		STR_BUFFER___ ErrorMessageBuffer;\
		err::buffer__ ERRBuffer;

#define RB	ERRBegin

#define RR	\
	ERRErr
// BE CAREFUL : the exception can come from the library. In this case, he error data are NOT available. The library has its own 'ERR' library data.
#define RN\
		if ( CErrString_.Amount() != 0 )\
			ErrorMessage = CErrString_.Convert( ErrorMessageBuffer );\
		else if ( ( ERRMajor != err::itn ) || ( ERRMinor != err::iBeam ) )\
			ErrorMessage = err::Message( ERRBuffer );\
\
		if ( err::ERR.Error )\
	 		ERRRst();\
\
		nsxpcm::Transform( ErrorMessage, JSErrorMessage );\
\
	ERREnd

#define RE	\
	ERREpilog\
	return NSResult;

static const char *GetComponentPath_(
	const char *ComponentId,
	STR_BUFFER___ &Buffer )
{
ERRProlog
	str::string Path;
ERRBegin
	Path.Init( "Components/Component[@id=\"" );
	Path.Append( ComponentId );
	Path.Append( "\"]" );

	Path.Convert( Buffer );
ERRErr
ERREnd
ERREpilog
	return Buffer;
}

NS_IMETHODIMP egeckocom::Initialize(
	const char *ComponentId,
	const char *Language,
	char **JSErrorMessage )
{
RP
	str::string LibraryName;
	str::strings Tags;
	STR_BUFFER___ Buffer;
RB
	if ( !IsInitialized_ ) {
		sclmisc::Initialize( NAME );
		IsInitialized_ = true;
	}

	LibraryName.Init();
	sclrgstry::GetMandatoryRegistryValue( GetComponentPath_( ComponentId, Buffer ), LibraryName );

	mtx::Lock( _Mutex );

	if ( _CurrentSteering != NULL )
		ERRc();

	_Data.Init( Language );

	if ( !_Wrapper.Init( LibraryName.Convert( Buffer ), &_Data, err::hUserDefined ) ) {
		ErrorMessage.Init( scllocale::GetTranslation( MESSAGE_UNABLE_TO_OPEN_COMPONENT ) );
		Tags.Init();
		Tags.Append( str::string( " F: " __FILE__ "; L: " E_STRING( __LINE__ ) ) );
		Tags.Append( LibraryName );
		lcl::ReplaceTags( ErrorMessage, Tags );
		ErrorMessage.Append( " !" );
		ERRBeam();
	}

	_CurrentSteering = &_Wrapper.GetSteering();
RR
	_CurrentSteering = NULL;

	if ( mtx::IsLocked( _Mutex ) )
		mtx::Unlock( _Mutex );
RN
RE
}


NS_IMETHODIMP egeckocom::Register(
	nsIDOMWindow *Window,
	char **JSErrorMessage )
{
	// Don't know how to get a 'window' from its 'document'.
RP
	str::string Id;
	str::strings Tags;
RB
	if ( !mtx::IsLocked( _Mutex ) )
		ERRu();

	if ( _CurrentSteering == NULL )
		ERRu();

	Id.Init();

	nsxpcm::GetId( nsxpcm::GetElement( Window ), Id );

	if ( !_CurrentSteering->Register( Window, Id ) ) {
		ErrorMessage.Init( scllocale::GetTranslation( MESSAGE_UNABLE_TO_REGISTER_ELEMENT ) );
		Tags.Init();
		Tags.Append( str::string( " F: " __FILE__ "; L: " E_STRING( __LINE__ ) ) );
		Tags.Append( Id );
		lcl::ReplaceTags( ErrorMessage, Tags );
		ErrorMessage.Append( " !" );
		ERRBeam();
	}
RR
RN
RE
}

NS_IMETHODIMP egeckocom::Start(
	nsICommandLine *CommandLine,
	char **JSErrorMessage )
{
RP
RB
	if ( !mtx::IsLocked( _Mutex ) )
		ERRu();

	if ( _CurrentSteering == NULL )
		ERRu();

	_CurrentSteering = NULL;

	mtx::Unlock( _Mutex );
RR
RN
RE
}

NS_IMETHODIMP egeckocom::LoneRegister(
	nsIDOMWindow *Window,
	const char *ComponentId,
	char **JSErrorMessage )
{
	// Don't know how to get a 'window' fro its 'document'.
RP
	geckoo::user_functions__ *Steering = NULL;
	str::string Id, LibraryName;
	str::strings Tags;
	STR_BUFFER___ Buffer;
RB
	LibraryName.Init();
	sclrgstry::GetMandatoryRegistryValue( GetComponentPath_( ComponentId, Buffer ), LibraryName );

	if ( _CurrentSteering != NULL ) {
		ErrorMessage.Init( scllocale::GetTranslation( MESSAGE_BAD_LONE_REGISTRATION_CONTEXT ) );
		lcl::ReplaceTag( ErrorMessage, 1, str::string( " F: " __FILE__ "; L: " E_STRING( __LINE__ ) ) );
		ErrorMessage.Append( " !" );
		ERRBeam();
	}

	if ( ( Steering = geckof::RetrieveLoneSteering( LibraryName.Convert( Buffer ), err::hUserDefined ) ) == NULL ) {
		ErrorMessage.Init( scllocale::GetTranslation( MESSAGE_LONE_REGISTRATION_FAILURE ) );
		lcl::ReplaceTag( ErrorMessage, 1, str::string( " F: " __FILE__ "; L: " E_STRING( __LINE__ ) ) );
		ErrorMessage.Append( " !" );
		ERRBeam();
	}

	Id.Init();

	nsxpcm::GetId( nsxpcm::GetElement( Window ), Id );

	if ( !Steering->Register( Window, Id ) ) {
		ErrorMessage.Init( scllocale::GetTranslation( MESSAGE_UNABLE_TO_REGISTER_ELEMENT ) );
		Tags.Init();
		Tags.Append( str::string( " F: " __FILE__ "; L: " E_STRING( __LINE__ ) ) );
		Tags.Append( Id );
		lcl::ReplaceTags( ErrorMessage, Tags );
		ErrorMessage.Append( " !" );
		ERRBeam();
	}
RR
	if ( Window != NULL )
		nsxpcm::Close( Window );
RN
RE
}

NS_IMETHODIMP egeckocom::Terminate( char **JSErrorMessage )
{
RP
RB
	_Wrapper.reset();
RR
RN
RE
}

egeckocom::egeckocom( void )
{
}

egeckocom::~egeckocom( void )
{
}

/* Gecko required part. */

#include "nsIGenericFactory.h"

NS_IMPL_ISUPPORTS1(egeckocom, EIGeckoCOM)
NS_GENERIC_FACTORY_CONSTRUCTOR(egeckocom)

static nsModuleComponentInfo components[] =
{
    {
       EGECKOCOM_CLASSNAME, 
       EGECKOCOM_CID,
       EGECKOCOM_CONTRACTID,
       egeckocomConstructor,
    },
	NSXPCM_COMPONENTS,
};

NS_IMPL_NSGETMODULE("EGeckoCOMModule", components) 


static class cdtor__ {
public:
	cdtor__( void )
	{
		_Mutex = mtx::Create( mtx::mOwned );

		COutString_.Init();
		COutDriver_.Init( COutString_, fdr::tsDisabled );
		cio::COutDriver.Init( COutDriver_, fdr::ts_Default );

		CErrString_.Init();
		CErrDriver_.Init( CErrString_, fdr::tsDisabled );
		cio::CErrDriver.Init( CErrDriver_, fdr::ts_Default );

		cio::CInDriver.Init( flx::VoidIFlowDriver, fdr::ts_Default );

		cio::Initialize( cio::tUser );
	}
	~cdtor__( void )
	{
		if ( _Mutex != MTX_INVALID_HANDLER )
			mtx::Delete( _Mutex );

		_Mutex = MTX_INVALID_HANDLER;

		if ( IsInitialized_ )
			sclmisc::Terminate();
	}
} _CDTor;