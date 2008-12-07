/* $Id$ */

#include "egeckocom.h"
#include "nsxpcm.h"
#include "nsIDOMEventListener.h"
#include "nsIDOMEventTarget.h"

class element_repository
: public nsxpcm::element_cores
{
public:
	element_repository( void )
	{
		Init();
	}
};


element_repository ElementRepository;

void Append( nsxpcm::element_core__ &Core )
{
	nsIDOMEventTarget *EventTarget = NULL;
	nsCOMPtr<eevent_listener> EventListener;

	ElementRepository.Append( &Core );

	EventTarget = nsxpcm::QueryInterface<nsIDOMEventTarget>( Core.GetElement() );

	nsxpcm::CreateInstance( EEVENT_LISTENER_CONTRACTID, EventListener );

	if ( EventTarget->AddEventListener( NS_LITERAL_STRING( "command" ), EventListener, true ) != NS_OK )
		ERRc();

	if ( EventTarget->AddEventListener( NS_LITERAL_STRING( "input" ), EventListener, true ) != NS_OK )
		ERRc();

	if ( EventTarget->AddEventListener( NS_LITERAL_STRING( "click" ), EventListener, true ) != NS_OK )
		ERRc();

	if ( EventTarget->AddEventListener( NS_LITERAL_STRING( "focus" ), EventListener, true ) != NS_OK )
		ERRc();

	if ( EventTarget->AddEventListener( NS_LITERAL_STRING( "blur" ), EventListener, true ) != NS_OK )
		ERRc();
}


inline void Append(
	nsxpcm::element_core__ &Core,
	nsIDOMDocument *Document,
	const str::string_ &Id )
{
	Core.Init( nsxpcm::GetElementById( Document, Id ), NULL );

	Append( Core );
}

#define RBB	ERRBegin

#define RP	\
		nsresult NSResult = NS_OK;\
	ERRProlog
#define RB	RBB { if ( !Connected ) ERRu(); kernel::SpecialDebug( "--> " __FILE__ " ", __LINE__ ); }
#define RR	\
	ERRErr	\
	if ( ERRMajor != err::itn ) {\
		NSResult = NS_ERROR_FAILURE;\
	} else if ( ERRMinor != err::iReturn ) {\
		NSResult = NS_ERROR_FAILURE;\
	}\
	ERRRst();
#define RN	ERREnd
#define RE	\
	ERREpilog\
	return NSResult;

NS_IMPL_ISUPPORTS1(eshared, ieshared)

NS_IMETHODIMP eshared::Test( void )
{
RP
RBB
	nsxpcm::Log( "Hello the world !" );
RR
RN
RE
}

NS_IMETHODIMP eshared::Set(const char *Value)
{
RP
RBB
	this->Value.Init( Value );
RR
RN
RE
}

/* string Get (); */
NS_IMETHODIMP eshared::Get(char **_retval)
{
RP
RBB
	nsxpcm::Transform( Value, _retval );
RR
RN
RE
}



eshared::eshared()
{
	Value.Init();
  /* member initializers and constructor code */
}

eshared::~eshared()
{
  /* destructor code */
}


NS_IMPL_ISUPPORTS1(eprivate, ieprivate)

eprivate::eprivate()
{
  /* member initializers and constructor code */
	_Kernel.Init( _UI.Text );
	_UI.Init( _Kernel );
}

eprivate::~eprivate()
{
  /* destructor code */
}

NS_IMPL_ISUPPORTS1(eevent_listener, ieevent_listener)

eevent_listener::eevent_listener()
{
  /* member initializers and constructor code */
}

eevent_listener::~eevent_listener()
{
  /* destructor code */
}

NS_IMETHODIMP eevent_listener::HandleEvent(nsIDOMEvent *Event)
{
	nsresult NSResult = NS_OK;
ERRProlog
ERRBegin
	nsxpcm::Handle( Event, ElementRepository );
ERRErr
	NSResult = NS_ERROR_FAILURE;

/*	if ( _Kernel != NULL ) {
		if ( ( ERRMajor != err::itn ) || ( ERRMinor != err::iBeam ) )
			_Kernel->Alert( err::Message( Buffer ) );	
		else
			_Kernel->Alert( _Kernel->Backend.GetMessage() );
	}
*/
	ERRRst();
ERREnd
ERREpilog
    return NSResult;
}



NS_IMETHODIMP eprivate::SetUI(nsIDOMDocument *Document)
{
RP
RBB
	_UI.Document = Document;

	Append( _UI.Set, Document, str::string( "XPCOMSet" ) );
	Append( _UI.Get, Document, str::string( "XPCOMGet" ) );
	Append( _UI.Text, Document, str::string( "XPCOMText" ) );
RR
RN
RE
}


/* Gecko required part. */

#include "nsIGenericFactory.h"

NS_GENERIC_FACTORY_CONSTRUCTOR(eshared)
NS_GENERIC_FACTORY_CONSTRUCTOR(eprivate)
NS_GENERIC_FACTORY_CONSTRUCTOR(eevent_listener)

static nsModuleComponentInfo components[] =
{
    {
       ESHARED_CLASSNAME, 
       ESHARED_CID,
       ESHARED_CONTRACTID,
       esharedConstructor,
    },
    {
       EPRIVATE_CLASSNAME, 
       EPRIVATE_CID,
       EPRIVATE_CONTRACTID,
       eprivateConstructor,
    },
    {
       EEVENT_LISTENER_CLASSNAME, 
       EEVENT_LISTENER_CID,
       EEVENT_LISTENER_CONTRACTID,
       eevent_listenerConstructor,
    },
};

NS_IMPL_NSGETMODULE("EpeiosModule", components) 
