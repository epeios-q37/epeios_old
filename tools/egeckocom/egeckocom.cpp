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

#include "flx.h"
#include "fnm.h"
#include "mtx.h"


#define COMPONENT_VERSION	"3"

#define NAME "egeckocom"
#define VERSION	"0.1.0"

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

#define MESSAGE_UNABLE_TO_REGISTER_ELEMENT	"UnableToRegisterElement"
#define MESSAGE_UNABLE_TO_OPEN_COMPONENT	"UnableToOpenComponent"
#define MESSAGE_BAD_RETRIEVE_CONTEXT		"BadRetrieveContext"
#define MESSAGE_RETRIEVE_FAILURE		"RetrieveFailure"

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
		str::string ErrorMessage;\
		STR_BUFFER___ ErrorMessageBuffer;\
		err::buffer__ ERRBuffer;

#define RB	ERRBegin

#define RR	\
	ERRErr
// BE CAREFUL : the exception can come from the library. In this case, the error data are NOT available. The library has its own 'ERR' library data.
#define RN\
		if ( CErrString_.Amount() != 0 ) {\
			ErrorMessage = CErrString_.Convert( ErrorMessageBuffer );\
			CErrString_.Init();\
		} else if ( ( ERRMajor != err::itn ) || ( ERRMinor != err::iBeam ) )\
			ErrorMessage = err::Message( ERRBuffer );\
\
		if ( ERRError() )\
	 		ERRRst();\
\
		nsxpcm::Transform( ErrorMessage, JSErrorMessage );\
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

	registry::GetMandatoryRegistryValue( registry::TaggedComponent, Tags, Component );
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
	sclmisc::Initialize( NAME, GetConfigurationDirectory_( Buffer ) );
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
	str::string LibraryName;
	str::strings Tags;
	FNM_BUFFER___ LocationBuffer;
	str::string Translation;
	STR_BUFFER___ Buffer;
	str::string ConfigurationDirectory;
RB
	nsCOMPtr<nsxpcm::clh__>CLH;

	nsxpcm::GetService( CLH_CONTRACTID, CLH );

	if ( !IsInitialized_ ) {
		Initialize_();
		IsInitialized_ = true;
	}

	LibraryName.Init();
	GetComponent_( ComponentId, LibraryName );

	mtx::Lock( _Mutex );

	if ( _CurrentSteering != NULL )
		ERRc();

	_LanguageBuffer.Init();

	if ( ( _LanguageBuffer =  malloc( strlen( Language ) + 1 ) ) == NULL )
		ERRa();

	strcpy( _LanguageBuffer, Language );

	_Data.Init( NAME " " VERSION, _LanguageBuffer, fnm::GetLocation( LibraryName.Convert( Buffer ), LocationBuffer ) );

	if ( !_Wrapper.Init( Buffer, &_Data, err::hUserDefined ) ) {
		if ( CErrString_.Amount() == 0 ) {
			Translation.Init();
			ErrorMessage.Init( scllocale::GetTranslation( MESSAGE_UNABLE_TO_OPEN_COMPONENT, Translation ) );
			Tags.Init();
			Tags.Append( str::string( " F: " __FILE__ "; L: " E_STRING( __LINE__ ) ) );
			Tags.Append( LibraryName );
			lcl::ReplaceTags( ErrorMessage, Tags );
			ErrorMessage.Append( " !" );
		}
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

NS_IMETHODIMP egeckocom___::Retrieve(
	const char *ComponentId,
	char **JSErrorMessage )
{
RP
	str::string LibraryName;
	str::strings Tags;
	STR_BUFFER___ Buffer;
	str::string Translation;
RB
	LibraryName.Init();
	GetComponent_( ComponentId, LibraryName );

	mtx::Lock( _Mutex );

	if ( _CurrentSteering != NULL ) {
		Translation.Init();
		ErrorMessage.Init( scllocale::GetTranslation( MESSAGE_BAD_RETRIEVE_CONTEXT, Translation ) );
		lcl::ReplaceTag( ErrorMessage, 1, str::string( " F: " __FILE__ "; L: " E_STRING( __LINE__ ) ) );
		ErrorMessage.Append( " !" );
		ERRBeam();
	}

	if ( ( _CurrentSteering = geckof::RetrieveSteering( LibraryName.Convert( Buffer ), err::hUserDefined ) ) == NULL ) {
		Translation.Init();
		ErrorMessage.Init( scllocale::GetTranslation( MESSAGE_RETRIEVE_FAILURE, Translation ) );
		lcl::ReplaceTag( ErrorMessage, 1, str::string( " F: " __FILE__ "; L: " E_STRING( __LINE__ ) ) );
		ErrorMessage.Append( " !" );
		ERRBeam();
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
	str::strings Tags;
	str::string Translation;
RB
	if ( !mtx::IsLocked( _Mutex ) )
		ERRu();

	if ( _CurrentSteering == NULL )
		ERRu();

	Id.Init();

	nsxpcm::GetId( nsxpcm::GetElement( Window ), Id );

	if ( !_CurrentSteering->Register( Window, Id ) ) {
		Translation.Init();
		ErrorMessage.Init( scllocale::GetTranslation( MESSAGE_UNABLE_TO_REGISTER_ELEMENT, Translation ) );
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

egeckocom___::egeckocom___( void )
{
}

egeckocom___::~egeckocom___( void )
{
}

/* Gecko required part. */

/*************************/
/* Debut 'auto-complete' */
/*************************/

/**************************************************/
/* Debut partie concernant 'auto complete observer' */

#include "nsIAutoCompleteResult.h"

# define ACR_IID_STR "d333cd20-c453-11dd-ad8b-0800200c9a68"

# define ACR_LISTENER_IID \
  {0xd333cd20, 0xc453, 0x11dd, \
    { 0xad, 0x8b, 0x08, 0x00, 0x20, 0x0c, 0x9a, 0x68 }}

namespace nsxpcm {
	class acr__
	: public nsIAutoCompleteResult
	{
	private:
		event_handler__ *_EventHandler;
	public:
		NS_DECLARE_STATIC_IID_ACCESSOR(ACR_LISTENER_IID)
		NS_DECL_ISUPPORTS
		NS_DECL_NSIAUTOCOMPLETERESULT
		  void reset( bso::bool__ = true )
		  {
			  _EventHandler = NULL;
		  }
		E_CVDTOR( acr__ );
	public:
		void Init( event_handler__ &EventHandler )
		{
#ifdef NSXPCM_DBG
			if ( _EventHandler != NULL )
				ERRu();
#endif
			reset();

			_EventHandler = &EventHandler;
		}
	};

	NS_GENERIC_FACTORY_CONSTRUCTOR(acr__)
}

NS_IMETHODIMP nsxpcm::acr__::GetSearchString(nsAString & aSearchString)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP nsxpcm::acr__::GetSearchResult(PRUint16 *aSearchResult)
{
	*aSearchResult = nsIAutoCompleteResult::RESULT_SUCCESS_ONGOING;

    return NS_OK;
}

NS_IMETHODIMP nsxpcm::acr__::GetDefaultIndex(PRInt32 *aDefaultIndex)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP nsxpcm::acr__::GetErrorDescription(nsAString & aErrorDescription)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP nsxpcm::acr__::GetMatchCount(PRUint32 *aMatchCount)
{
	*aMatchCount = 100;

    return NS_OK;
}

NS_IMETHODIMP nsxpcm::acr__::GetValueAt(PRInt32 index, nsAString & _retval NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

#include "dte.h"

NS_IMETHODIMP nsxpcm::acr__::GetLabelAt(PRInt32 index, nsAString & _retval NS_OUTPARAM)
{
	tol::buffer__ Buffer;

	nsxpcm::Transform( tol::Time( Buffer ) , _retval );

    return NS_OK;
}

NS_IMETHODIMP nsxpcm::acr__::GetCommentAt(PRInt32 index, nsAString & _retval NS_OUTPARAM)
{
	tol::buffer__ Buffer;

	nsxpcm::Transform( tol::Date( Buffer ) , _retval );

    return NS_OK;
}

NS_IMETHODIMP nsxpcm::acr__::GetStyleAt(PRInt32 index, nsAString & _retval NS_OUTPARAM)
{
    return NS_OK;
}

NS_IMETHODIMP nsxpcm::acr__::GetImageAt(PRInt32 index, nsAString & _retval NS_OUTPARAM)
{
    return NS_OK;
}

NS_IMETHODIMP nsxpcm::acr__::RemoveValueAt(PRInt32 rowIndex, NSXPCM__BOOL removeFromDb)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}


NS_DEFINE_STATIC_IID_ACCESSOR(nsxpcm::acr__, ACR_LISTENER_IID)

# define ACR_CONTRACTID "@zeusw.org/acr;1"
# define ACR_CLASSNAME "NSXPCM_ACR"
# define ACR_CID  ACR_LISTENER_IID

NS_IMPL_ISUPPORTS2(nsxpcm::acr__, nsxpcm::acr__, nsIAutoCompleteResult );

/* Fin partie concernant 'auto complete observer' */
/************************************************/



/**************************************************/
/* Debut partie concernant 'auto complete search' */

#include "nsIAutoCompleteSearch.h"

# define ACS_IID_STR "d333cd20-c453-11dd-ad8b-0800200c9a67"

# define ACS_LISTENER_IID \
  {0xd333cd20, 0xc453, 0x11dd, \
    { 0xad, 0x8b, 0x08, 0x00, 0x20, 0x0c, 0x9a, 0x67 }}

namespace nsxpcm {
	class acs__
	: public nsIAutoCompleteSearch
	{
	private:
		nsCOMPtr<nsIAutoCompleteResult> _Result;
	public:
		NS_DECLARE_STATIC_IID_ACCESSOR(ACS_LISTENER_IID)
		NS_DECL_ISUPPORTS
		NS_DECL_NSIAUTOCOMPLETESEARCH
		  void reset( bso::bool__ = true )
		  {
		  }
		E_CVDTOR( acs__ );
	public:
		void Init( event_handler__ &EventHandler )
		{
		}
	};

	NS_GENERIC_FACTORY_CONSTRUCTOR(acs__)
}

NS_IMETHODIMP nsxpcm::acs__::StartSearch(const nsAString & searchString, const nsAString & searchParam, nsIAutoCompleteResult *previousResult, nsIAutoCompleteObserver *listener)
{
	nsxpcm::CreateInstance( ACR_CONTRACTID, _Result );

	listener->OnSearchResult( this, _Result );

    return NS_OK;
}

/* void stopSearch (); */
NS_IMETHODIMP nsxpcm::acs__::StopSearch()
{
    return NS_OK;
}

NS_DEFINE_STATIC_IID_ACCESSOR(nsxpcm::acs__, ACS_LISTENER_IID)

# define ACS_CONTRACTID "@mozilla.org/autocomplete/search;1?name=nsxpcm_ac"
# define ACS_CLASSNAME "NSXPCMSearchResult"
# define ACS_CID  ACS_LISTENER_IID

NS_IMPL_ISUPPORTS2(nsxpcm::acs__, nsxpcm::acs__, nsIAutoCompleteSearch );

/* Fin partie concernant 'auto complete search' */
/************************************************/


/*************************/
/* Debut 'auto-complete' */
/*************************/

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
NS_DEFINE_NAMED_CID(ACS_CID);
NS_DEFINE_NAMED_CID(ACR_CID);
NS_DEFINE_NAMED_CID(CLH_CID);

// Build a table of ClassIDs (CIDs) which are implemented by this module. CIDs
// should be completely unique UUIDs.
// each entry has the form { CID, service, factoryproc, constructorproc }
// where factoryproc is usually NULL.
static const mozilla::Module::CIDEntry kEGeckoCOMCIDs[] = {
    { &kEGECKOCOM_CID, false, NULL, egeckocom___Constructor },
	{ &kNSXPCM_EVENT_LISTENER_CID, false, NULL, nsxpcm::event_listener__Constructor },
	{ &kNSXPCM_TREE_VIEW_CID, false, NULL, nsxpcm::tree_view__Constructor },
	{ &kACS_CID, false, NULL, nsxpcm::acs__Constructor },
	{ &kACR_CID, false, NULL, nsxpcm::acr__Constructor },
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
    { ACS_CONTRACTID, &kACS_CID },
    { ACR_CONTRACTID, &kACR_CID },
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