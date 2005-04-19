// $Id$

#include "etest.h"
#include "nsMemory.h"
#include "nsCOMPtr.h"
#include "nsXPCOM.h"
#include "nsIComponentManager.h"
#include "nsIServiceManager.h"
#include "nsIDOMDocument.h"
#include "nsIDOMElement.h"
#include "nsIDOMWindow.h"
#include "nsIWindowWatcher.h"
#include "nsIWebBrowser.h"
#include "nsIWebBrowserChrome.h"
#include "nsIDOMNamedNodeMap.h"
#include "nsEmbedString.h"

#include "epeios.h"
#include "str.h"
 
NS_IMPL_ISUPPORTS1(etest, ietest)
 
etest::etest()
{
  /* member initializers and constructor code */
}
 
etest::~etest()
{
  /* destructor code */
}
 
#include "cpe.h"

#if defined( CPE__CONSOLE )
#	error
#endif
// Template function.
/*
NS_IMETHODIMP etest::( *_retval )
{
	nsresult Result = NS_OK;
ERRProlog
ERRBegin
ERRErr
	ERRRst();
	Result = NS_ERROR_FAILURE;
ERREnd
ERREpilog
	return Result;
}
*/


static inline void Convert_(
	const char *CString,
	epeios::size__ Size,
	char **JString )
{
    if ( !JString )
        ERRu();
	else
	    *JString = (char*) nsMemory::Clone( CString, sizeof(char) * Size );
    
	if ( !JString )
		ERRa();
}

static inline void Convert_(
	const char *CString,
	char **JString )
{
	Convert_( CString, strlen( CString ) + 1, JString );
}

static inline void Convert_(
	const str::string_ &EString,
	char **JString )
{
ERRProlog
	tol::E_POINTER___( char ) Buffer;
ERRBegin
	Buffer = EString.Convert();

	Convert_( Buffer, EString.Amount() + 1, JString );
ERRErr
ERREnd
ERREpilog
}



#define RP	\
		nsresult NSResult = NS_OK;\
	ERRProlog
#define RB	ERRBegin
#define RR	\
	ERRErr	\
	if ( ( ERRMajor != err::itn ) || ( ERRMinor != err::iReturn ) )\
		NSResult = NS_ERROR_FAILURE;\
	ERRRst();
#define RN	ERREnd
#define RE	\
	ERREpilog\
	return NSResult;


NS_IMETHODIMP etest::Test(char **_retval)
{
RP
RB
	Convert_( "Document OK !", _retval );
RR
RN
RE
}

static inline void Convert_(
	const nsEmbedCString &ECString,
	nsEmbedString &EString )
{
	NS_CStringToUTF16(ECString, NS_CSTRING_ENCODING_ASCII, EString );
}

static inline void Convert_( 
	const char *String,
	nsEmbedString &EString )
{
	Convert_( nsEmbedCString( String ), EString );
}

static inline void Convert_(
	const str::string_ &String,
	nsEmbedString &EString )
{
ERRProlog
	tol::E_POINTER___( char ) Buffer;
ERRBegin
	Buffer = String.Convert();

	Convert_( Buffer, EString );
ERRErr
ERREnd
ERREpilog
}

static inline void Convert_(
	const nsEmbedString &EString,
	nsEmbedCString &ECString )
{
	NS_UTF16ToCString( EString, NS_CSTRING_ENCODING_ASCII, ECString );
}

static void Convert_( 
	const nsEmbedString &EString,
	str::string_ &String )
{
	nsEmbedCString ECString;

	Convert_( EString, ECString );

	String.Append( ECString.get() );
}

static inline void Convert_( 
	const nsEmbedString &EString,
	char **JString )
{
ERRProlog
	str::string String;
ERRBegin
	String.Init();

	Convert_( EString, String );
	Convert_( String, JString );
ERRErr
ERREnd
ERREpilog
}


/*

NS_IMETHODIMP etest::XPCOM( char **_retval )
{
RP
	nsCOMPtr<nsIComponentManager> compMan; 
	nsCOMPtr<ietest> etest; 
	str::string String;
RB
	if ( NS_FAILED( NS_GetComponentManager(getter_AddRefs(compMan)) ) )
		ERRx();

	if ( NS_FAILED( compMan->CreateInstanceByContractID("@epeios.org/etest;1", nsnull, NS_GET_IID(ietest), getter_AddRefs(etest)) ) )
		ERRx();


	nsCOMPtr<nsIServiceManager> servMan; 
	nsresult rv = NS_GetServiceManager(getter_AddRefs(servMan)); 
	if (NS_FAILED(rv)) 
		ERRx();

	nsCOMPtr<nsIWindowWatcher> windowWatcher; 
	rv = servMan->GetServiceByContractID("@mozilla.org/embedcomp/window-watcher;1", NS_GET_IID(nsIWindowWatcher), getter_AddRefs(windowWatcher)); 

	if (NS_FAILED(rv)) 
		ERRx();

	nsCOMPtr<nsIDOMWindow> Wind;
	windowWatcher->GetActiveWindow(getter_AddRefs( Wind) ); 

	if (!Wind )
		ERRx();

	nsCOMPtr<nsIWebBrowserChrome> webBrowserChrome; 
	rv = windowWatcher->GetChromeForWindow(Wind, getter_AddRefs( webBrowserChrome ) );  

	NSResult = rv;
	ERRReturn;


	if (!webBrowserChrome )
		ERRx();


	nsCOMPtr<nsIWebBrowser> webBrowser; 
	webBrowserChrome->GetWebBrowser( getter_AddRefs( webBrowser ) );

	if (!webBrowser )
		ERRx();

	nsCOMPtr<nsIDOMWindow> wind;
	webBrowser->GetContentDOMWindow(getter_AddRefs( wind) ); 

	if ( !wind )
		ERRx();

	nsCOMPtr<nsIDOMDocument> doc;

	wind->GetDocument(getter_AddRefs(doc));

	if ( !doc )
		ERRx();

	nsCOMPtr<nsIDOMNode> Element1, Element2, Element3;

	doc->GetLastChild( getter_AddRefs( Element1 ) );

	if ( !Element1 )
		ERRx();

	nsEmbedString Text;

	Element1->GetNodeName( Text );

	Element1->GetLastChild( getter_AddRefs( Element2 ) );

	if ( !Element2 )
		ERRx();
	
	String.Init();

	Element2->GetLastChild( getter_AddRefs( Element3 ) );

	if ( !Element3 )
		ERRx();

	Element3->GetNodeName( Text );

	Convert_( Text, String );

	Convert_( String, _retval );

	ERRReturn;

	Element2->SetNodeValue( nsEmbedString( L"Texte XPCOM !" ) );

//	Convert_( "No XPCOM text yet !", _retval );
	etest->Test( _retval );


RR
RN
RE
}
*/


// Se connecte au composant etest (c'sst--à-dire lui-même) à travers XPCOM !
static void TestSelf_( char **_retval )
{
	nsCOMPtr<nsIComponentManager> compMan; 
	nsCOMPtr<ietest> Etest; 

	if ( NS_FAILED( NS_GetComponentManager(getter_AddRefs(compMan)) ) )
		ERRx();

	if ( NS_FAILED( compMan->CreateInstanceByContractID("@epeios.org/etest;1", nsnull, NS_GET_IID(ietest), getter_AddRefs(Etest)) ) )
		ERRx();

	Etest->Test( _retval );
}

static void GetServiceManager_( nsCOMPtr<nsIServiceManager> &serviceManager )
{
	if ( NS_FAILED( NS_GetServiceManager( getter_AddRefs( serviceManager ) ) ) )
		ERRx();
}

static void GetWindowWatcher_( nsCOMPtr<nsIWindowWatcher> &windowWatcher )
{
	nsCOMPtr<nsIServiceManager> serviceManager;

	GetServiceManager_( serviceManager );

	if ( NS_FAILED( serviceManager->GetServiceByContractID("@mozilla.org/embedcomp/window-watcher;1", NS_GET_IID(nsIWindowWatcher), getter_AddRefs(windowWatcher)) ) )
		ERRx();

}

static nsresult CreateWindow_(
	nsIWindowWatcher &windowWatcher,
	nsCOMPtr<nsIDOMWindow> &window )
{
	return windowWatcher.OpenWindow( nsnull, "chrome://gesbib//gesbib.xul", nsnull, nsnull, nsnull, getter_AddRefs( window ) );
}

static nsresult GetActiveWindow_(
	nsIWindowWatcher &windowWatcher,
	nsCOMPtr<nsIDOMWindow> &window )
{
	return windowWatcher.GetActiveWindow( getter_AddRefs( window ) );
}

static void GetWindowUsingWindowWatcher_( nsCOMPtr<nsIDOMWindow> &window )
{
	nsCOMPtr<nsIWindowWatcher> windowWatcher; 

	GetWindowWatcher_( windowWatcher );

//	if ( NS_FAILED( CreateWindow_( *windowWatcher, window ) ) )
	if ( NS_FAILED( GetActiveWindow_( *windowWatcher, window ) ) )
		ERRx();

	if ( !window )
		ERRx();
}

static void GetWebBrowser_( nsCOMPtr<nsIWebBrowser> &webBrowser )
{
	nsCOMPtr<nsIServiceManager> serviceManager;

	GetServiceManager_( serviceManager );

	if ( NS_FAILED( serviceManager->GetServiceByContractID("@mozilla.org/embedding/browser/nsWebBrowser;1", NS_GET_IID(nsIWebBrowser), getter_AddRefs(webBrowser)) ) )
		ERRx();

	if ( !webBrowser )
		ERRx();
}

static void GetWindowUsingWebBrowser_( nsCOMPtr<nsIDOMWindow> &window )
{
	nsCOMPtr<nsIWebBrowser> webBrowser;

	GetWebBrowser_( webBrowser );

	if ( NS_FAILED( webBrowser->GetContentDOMWindow( getter_AddRefs( window ) ) ) )
		ERRx();

	if ( !window )
		ERRx();
}

static void GetWindow_( nsCOMPtr<nsIDOMWindow> &window )
{
//	GetWindowUsingWindowWatcher_( window );
	GetWindowUsingWebBrowser_( window );
}

static void GetDocument_( nsCOMPtr<nsIDOMDocument> &document )
{
	nsCOMPtr<nsIDOMWindow> window;

	GetWindow_( window );

	if ( NS_FAILED( window->GetDocument(getter_AddRefs(document ) ) ) )
		ERRx();

	if ( !document )
		ERRx();
}

static void GetRootElement_( nsCOMPtr<nsIDOMElement> &element )
{
	nsCOMPtr<nsIDOMDocument> document;

	GetDocument_( document );

	if ( NS_FAILED( document->GetDocumentElement( getter_AddRefs( element ) ) ) )
		ERRx();

	if ( !element )
		ERRx();
}

static void GetName_(
	nsIDOMNode &Node,
	str::string_ &Name )
{
	nsEmbedString Text;

	Node.GetNodeName( Text );

	Convert_( Text, Name );
}

static void GetValue_(
	nsIDOMNode &Node,
	str::string_ &Name )
{
	nsEmbedString Text;

	Node.GetNodeValue( Text );

	Convert_( Text, Name );
}

static void GetAttributeValue_(
	nsIDOMNode &Node,
	const char *Label,
	str::string_ &Value )
{
	nsCOMPtr<nsIDOMNamedNodeMap> namedNodeMap;
	nsCOMPtr<nsIDOMNode> INode;
	nsEmbedString EString;

	if ( NS_FAILED( Node.GetAttributes( getter_AddRefs( namedNodeMap ) ) ) )
		ERRx();

	if ( !namedNodeMap )
		ERRx();

	Convert_( Label, EString );

	if ( NS_FAILED( namedNodeMap->GetNamedItem( EString, getter_AddRefs( INode ) ) ) )
		ERRx();

	if ( !INode )
		ERRx();

	GetValue_( *INode, Value );
}


NS_IMETHODIMP etest::XPCOM( char **_retval )
{
RP
	str::string String;
RB
	nsCOMPtr<nsIDOMElement> Element;

	GetRootElement_( Element );

	nsCOMPtr<nsIDOMNode> Node1, Node2;

	Element->GetFirstChild( getter_AddRefs( Node1 ) );

	if ( !Node1 )
		ERRx();
/*
	Node1->GetFirstChild( getter_AddRefs( Node2 ) );

	if ( !Node2 )
		ERRx();
*/
/*
	Node1->GetLastChild( getter_AddRefs( Node2 ) );

	if ( !Node2 )
		ERRx();
*/	
	String.Init();

//	GetName_( *Element, String );
	GetAttributeValue_( *Element, "id", String );

	Convert_( String, _retval );

	ERRReturn;


/*
	if ( !Element )
		ERRx();

	Element2->SetNodeValue( nsEmbedString( L"Texte XPCOM !" ) );
*/


RR
RN
RE
}

NS_IMETHODIMP etest::GetErrorMessage( char **_retval )
{
	err::buffer__ Buffer;

	const char *Message = NULL;

	if ( ERRMajor != err::ok )
		Message = err::Message( Buffer );

	if ( Message != NULL )
		Convert_( Message, _retval );
	else
		Convert_( "No error !", _retval );
	return NS_OK;
}
