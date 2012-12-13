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

#include "locale.h"
#include "registry.h"

#include "sclmisc.h"
#include "sclerror.h"

#include "atcplsrch.h"

#include "flx.h"
#include "fnm.h"
#include "mtx.h"


#define COMPONENT_VERSION	"5"

#define APP_NAME	"egeckocom"
#define VERSION	"0.1.4"

#define EGECKOCOM_CONTRACTID "@zeusw.org/egeckocom;" COMPONENT_VERSION
#define EGECKOCOM_CLASSNAME "Generic Epeios component"
#define EGECKOCOM_CID  EIGECKOCOM_IID

static mtx::mutex_handler__ _Mutex = MTX_INVALID_HANDLER;	// To protect access to following object.
static geckoo::steering_callback__ *_CurrentSteering = NULL;
static bso::bool__ IsInitialized_ = false;

static str::string COutString_;
static flx::E_STRING_OFLOW_DRIVER___ COutDriver_;

static str::string CErrString_;
static flx::E_STRING_OFLOW_DRIVER___ CErrDriver_;

#define MESSAGE_UNABLE_TO_REGISTER_ELEMENT	"UnableToRegisterElement"
#define MESSAGE_UNABLE_TO_OPEN_COMPONENT	"UnableToOpenComponent"
#define MESSAGE_BAD_RETRIEVE_CONTEXT		"BadRetrieveContext"
#define MESSAGE_RETRIEVE_FAILURE		"RetrieveFailure"
#define MESSAGE_UNABLE_TO_HANDLE_PSEUDO_EVENT	"UnableToHandlePseudoEvent"

class egeckocom___
: public EIGeckoCOM
{
private:
	geckof::gecko_wrapper___ _Wrapper;
	geckoo::shared_data__ _Data;			// Only used by the master window.
	tol::E_FPOINTER___( char ) _LanguageBuffer;		//				"
	~egeckocom___();
public:
  NS_DECL_ISUPPORTS
  NS_DECL_EIGECKOCOM

  egeckocom___();
private:
protected:
  /* additional members */
};


#define RP	\
		nsresult NSResult = NS_OK;\
	ERRProlog\
		lcl::meaning ErrorMeaning;\
		str::string ErrorTranslation;\
		err::buffer__ ERRBuffer;

#define RB	ERRBegin

#define RR	\
	ERRErr
// BE CAREFUL : the exception can come from the library. In this case, the error data are NOT available. The library has its own 'ERR' library data.
#define RN\
		cio::CErr << txf::commit;\
		ErrorTranslation.Init();\
		if ( CErrString_.Amount() != 0 ) {\
			ErrorTranslation = CErrString_;\
			CErrString_.Init();\
		} else if ( !ErrorMeaning.Levels.IsEmpty() ) {\
			ERRRst();\
			scllocale::GetLocale().GetTranslation( ErrorMeaning, _LanguageBuffer, ErrorTranslation );\
		} else if ( sclerror::IsErrorPending() ) {\
			ERRRst();\
			scllocale::GetLocale().GetTranslation( sclerror::GetMeaning(), _LanguageBuffer, ErrorTranslation );\
			sclerror::ResetPendingError();\
		} else if ( ERRType != err::t_Free )\
			ErrorTranslation = err::Message( ERRBuffer );\
\
		if ( ERRError() )\
	 		ERRRst();\
\
		nsxpcm::Transform( ErrorTranslation, JSErrorMessage );\
\
	ERREnd

#define RE	\
	ERREpilog\
	return NSResult;

static const str::string_ &GetComponent_(
	const char *ComponentId,
	str::string_ &Component )
{
ERRProlog
	registry::tags Tags;
ERRBegin
	Tags.Init();
	Tags.Append( str::string( ComponentId ) );

	registry::GetMandatoryRegistryValue( rgstry::tentry__( registry::TaggedComponent, Tags ), Component );
ERRErr
ERREnd
ERREpilog
	return Component;
}

const char *GetConfigurationDirectory_( STR_BUFFER___ &Buffer )
{
ERRProlog
	str::string Directory;
ERRBegin
	Directory.Init();

	nsxpcm::GetInstallationDirectory( Directory );

	Directory.Append( FNM_DIRECTORY_SEPARATOR_STRING "components" );

	Directory.Convert( Buffer );
ERRErr
ERREnd
ERREpilog
	return Buffer;
}

static void Initialize_( void )
{
ERRProlog
	STR_BUFFER___ Buffer;
ERRBegin
	sclmisc::Initialize( APP_NAME, GetConfigurationDirectory_( Buffer ) );
ERRErr
ERREnd
ERREpilog
}

NS_IMETHODIMP egeckocom___::Create(
	const char *ComponentId,
	const char *Language,
	char **JSErrorMessage )
{
RP
	str::string RawLibraryName, CorrectedLibraryName;
	FNM_BUFFER___ LocationBuffer;
	STR_BUFFER___ Buffer, IdentificationBuffer;
	str::string ConfigurationDirectory, Identification;
RB
	nsCOMPtr<nsxpcm::clh__>CLH;

	nsxpcm::GetService( CLH_CONTRACTID, CLH );

	if ( !IsInitialized_ ) {
		Initialize_();
		IsInitialized_ = true;
	}

	_LanguageBuffer.Init();

	if ( ( _LanguageBuffer =  malloc( strlen( Language ) + 1 ) ) == NULL )
		ERRa();

	strcpy( _LanguageBuffer, Language );

	RawLibraryName.Init();
	GetComponent_( ComponentId, RawLibraryName );

	mtx::Lock( _Mutex );

	if ( _CurrentSteering != NULL )
		ERRc();


	CorrectedLibraryName.Init();

	if ( fnm::Type( RawLibraryName.Convert( Buffer ) ) != fnm::tAbsolute )
		nsxpcm::GetInstallationDirectory( CorrectedLibraryName );

	CorrectedLibraryName.Append( RawLibraryName );

	Identification.Init();
	Identification.Append( APP_NAME " V" VERSION " Build " __DATE__ " " __TIME__ " (" );
	Identification.Append( cpe::GetDescription() );
	Identification.Append( ')' );

	_Data.Init( Identification.Convert( IdentificationBuffer ), _LanguageBuffer, fnm::GetLocation( CorrectedLibraryName.Convert( Buffer ), LocationBuffer ) );

	if ( !_Wrapper.Init( Buffer, &_Data, err::hUserDefined ) ) {
		if ( CErrString_.Amount() == 0 ) {
			ErrorMeaning.Init();
			ErrorMeaning.SetValue( MESSAGE_UNABLE_TO_OPEN_COMPONENT );
			ErrorMeaning.AddTag( " F: " __FILE__ "; L: " E_STRING( __LINE__ ) );
		}
		ERRFree();
	}

	_CurrentSteering = &_Wrapper.GetSteering();
RR
	_CurrentSteering = NULL;

	if ( mtx::IsLocked( _Mutex ) )
		mtx::Unlock( _Mutex );
RN
RE
}

NS_IMETHODIMP egeckocom___::Retrieve(
	const char *ComponentId,
	char **JSErrorMessage )
{
RP
	str::string LibraryName;
	STR_BUFFER___ Buffer;
RB
	LibraryName.Init();
	GetComponent_( ComponentId, LibraryName );

	mtx::Lock( _Mutex );

	if ( _CurrentSteering != NULL ) {
		ErrorMeaning.Init();
		ErrorMeaning.SetValue( MESSAGE_BAD_RETRIEVE_CONTEXT );
		ErrorMeaning.AddTag( " F: " __FILE__ "; L: " E_STRING( __LINE__ ) );
		ERRFree();
	}

	if ( ( _CurrentSteering = geckof::RetrieveSteering( LibraryName.Convert( Buffer ), err::hUserDefined ) ) == NULL ) {
		ErrorMeaning.Init();
		ErrorMeaning.SetValue( MESSAGE_RETRIEVE_FAILURE );
		ErrorMeaning.AddTag( " F: " __FILE__ "; L: " E_STRING( __LINE__ ) );
		ERRFree();
	}

RR
	_CurrentSteering = NULL;

	if ( mtx::IsLocked( _Mutex ) )
		mtx::Unlock( _Mutex );
RN
RE
}

NS_IMETHODIMP egeckocom___::Register(
	nsIDOMWindow *Window,
	char **JSErrorMessage )
{
	// Don't know how to get a 'window' from its 'document'.
RP
	str::string Id;
RB
	if ( !mtx::IsLocked( _Mutex ) )
		ERRu();

	if ( _CurrentSteering == NULL )
		ERRu();

	Id.Init();

	nsxpcm::GetId( nsxpcm::GetElement( Window ), Id );

	if ( !_CurrentSteering->Register( Window, Id ) ) {
		ErrorMeaning.Init();
		ErrorMeaning.SetValue( MESSAGE_UNABLE_TO_REGISTER_ELEMENT );
		ErrorMeaning.AddTag( " F: " __FILE__ "; L: " E_STRING( __LINE__ ) );
		ErrorMeaning.AddTag( Id );
		ERRFree();
	}
RR
RN
RE
}

NS_IMETHODIMP egeckocom___::Stop(
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

NS_IMETHODIMP egeckocom___::Terminate( char **JSErrorMessage )
{
RP
RB
	_Wrapper.reset();
RR
RN
RE
}

NS_IMETHODIMP egeckocom___::HandlePseudoEvent(
	nsIDOMElement *Element,
	const char *Parameter,
	char **JSErrorMessage )
{
RP
RB
	((geckoo::pseudo_event_callback__ *)str::string( Parameter ).ToPointer() )->Handle( Element );
RR
RN
RE
}

NS_IMETHODIMP egeckocom___::Test( char **JSErrorMessage )
{
RP
	int i = 0;
RB
	i = i;
RR
RN
RE
}

egeckocom___::egeckocom___( void )
{
}

egeckocom___::~egeckocom___( void )
{
}

/* Gecko required part. */

using namespace nsxpcm;

NS_IMPL_ISUPPORTS1(egeckocom___, EIGeckoCOM)
NS_GENERIC_FACTORY_CONSTRUCTOR(egeckocom___)

#ifdef NSXPCM__GECKO_V1

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


#elif defined( NSXPCM__GECKO_V2 ) || defined ( NSXPCM__GECKO_V8 )
# include "mozilla/ModuleUtils.h"


//NS_GENERIC_FACTORY_CONSTRUCTOR(event_listener)

// The following line defines a kNS_SAMPLE_CID CID variable.
NS_DEFINE_NAMED_CID(EGECKOCOM_CID);
NS_DEFINE_NAMED_CID(NSXPCM_EVENT_LISTENER_CID);
NS_DEFINE_NAMED_CID(NSXPCM_TREE_VIEW_CID);
NS_DEFINE_NAMED_CID(ATCPLSRCH_AUTOCOMPLETE_SEARCH_CID);
NS_DEFINE_NAMED_CID(NSXPCM_AUTOCOMPLETE_RESULT_CID);
NS_DEFINE_NAMED_CID(CLH_CID);

// Build a table of ClassIDs (CIDs) which are implemented by this module. CIDs
// should be completely unique UUIDs.
// each entry has the form { CID, service, factoryproc, constructorproc }
// where factoryproc is usually NULL.
static const mozilla::Module::CIDEntry kEGeckoCOMCIDs[] = {
    { &kEGECKOCOM_CID, false, NULL, egeckocom___Constructor },
	{ &kNSXPCM_EVENT_LISTENER_CID, false, NULL, nsxpcm::event_listener__Constructor },
	{ &kNSXPCM_TREE_VIEW_CID, false, NULL, nsxpcm::tree_view__Constructor },
	{ &kATCPLSRCH_AUTOCOMPLETE_SEARCH_CID, false, NULL, atcplsrch::autocomplete_search__Constructor },
	{ &kNSXPCM_AUTOCOMPLETE_RESULT_CID, false, NULL, nsxpcm::autocomplete_result___Constructor },
	{ &kCLH_CID, false, NULL, nsxpcm::clh__Constructor },
    { NULL }
};

// Build a table which maps contract IDs to CIDs.
// A contract is a string which identifies a particular set of functionality. In some
// cases an extension component may override the contract ID of a builtin gecko component
// to modify or extend functionality.
static const mozilla::Module::ContractIDEntry kEGeckoCOMContracts[] = {
    { EGECKOCOM_CONTRACTID, &kEGECKOCOM_CID },
    { NSXPCM_EVENT_LISTENER_CONTRACTID, &kNSXPCM_EVENT_LISTENER_CID },
    { NSXPCM_TREE_VIEW_CONTRACTID, &kNSXPCM_TREE_VIEW_CID },
    { ATCPLSRCH_AUTOCOMPLETE_SEARCH_CONTRACTID, &kATCPLSRCH_AUTOCOMPLETE_SEARCH_CID },
    { NSXPCM_AUTOCOMPLETE_RESULT_CONTRACTID, &kNSXPCM_AUTOCOMPLETE_RESULT_CID },
    { CLH_CONTRACTID, &kCLH_CID },
    { NULL }
};

// Category entries are category/key/value triples which can be used
// to register contract ID as content handlers or to observe certain
// notifications. Most modules do not need to register any category
// entries: this is just a sample of how you'd do it.
// @see nsICategoryManager for information on retrieving category data.
static const mozilla::Module::CategoryEntry kEGeckoCOMCategories[] = {
	{ "command-line-handler", "m-geckocom", CLH_CONTRACTID },
    { NULL }
};

static const mozilla::Module EGeckoCOMModule = {
    mozilla::Module::kVersion,
    kEGeckoCOMCIDs,
    kEGeckoCOMContracts,
    kEGeckoCOMCategories
};

static const mozilla::Module EEventListenerModule = {
    mozilla::Module::kVersion,
    kEGeckoCOMCIDs,
    kEGeckoCOMContracts,
    kEGeckoCOMCategories
};

// The following line implements the one-and-only "NSModule" symbol exported from this
// shared library.
NSMODULE_DEFN(EGeckoCOMModule) = &EGeckoCOMModule;
#else
# error
#endif


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