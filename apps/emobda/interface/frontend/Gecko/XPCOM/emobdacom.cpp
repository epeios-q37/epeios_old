/*
	'emobdacom.cpp' by Claude SIMON (http://zeusw.org/).

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

#include "iemobda.h"

#include "nsxpcm.h"

#define VERSION __DATE__ " " __TIME__

#define EMOBDACOM_CONTRACTID "@zeusw.org/emobdacom;1"
#define EMOBDACOM_CLASSNAME "EMOBDA"
// {cfee5019-09e1-4d5c-af89-0ffadf903df5}
#define EMOBDACOM_CID  IEMOBDACOM_IID

class emobdacom
: public iemobdacom
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_IEMOBDACOM

  emobdacom();
//  krow__ _KernelRow;
private:
  ~emobdacom();
protected:
  /* additional members */
};

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



NS_IMETHODIMP emobdacom::RegisteringStart( void )
{
RP
	str::string Version;
	tol::buffer__ Buffer;
RBB
	Version.Init( VERSION );

	Version.Append( " (" );
	Version.Append( tol::DateAndTime( Buffer ) );
	Version.Append( ')' );

	nsxpcm::Log( Version );

//	Global.CreateNewObject();
RR
RN
RE
}

template <typename element, typename ui, typename id > static void _Register(
	element &Core,
	ui &UI,
	const id &Id )
{
	Core.Init( Global.GetCurrentRow() );

	nsxpcm::Register( Core, UI.Document, Id );
}

/*
static void _RegisterSpecific( ui__::main &UI )
{
	_Register( UI.Input, UI, "input" );
	_Register( UI.Shared, UI, "shared" );
	_Register( UI.Output, UI, "output" );
	_Register( UI.JSConsole, UI, "jsconsole" );
	_Register( UI.XSLT, UI, "xslt" );
	_Register( UI.Endianess, UI, "endianess" );
}

static void _RegisterSpecific( ui__::page &UI )
{
	_Register( UI.Error, UI, str::string( "error" ) );	// Version 'const str::string_', pour test.
	_Register( UI.Link, UI, "link" );
}
*/

template <typename ui> static void _RegisterCommon(
	nsIDOMWindow *Window,
	ui &UI )
{
	UI.Window = Window;

	Window->GetDocument( &UI.Document );
}

/*
template <typename ui> static void _Register(
	nsIDOMWindow *Window,
	ui &UI )
{
	_RegisterCommon( Window, UI );
	_RegisterSpecific( UI );
}
*/

NS_IMETHODIMP emobdacom::Register(
	nsIDOMWindow *Window,
	const char *UIDesignation )
{
	// Ne sait pas récupèrer une 'window' à partir de son document.
RP
RBB
/*	ui__ &UI = Global.GetCurrentObject().UI;

#ifdef XXX_DBG
	if ( _KernelRow != NONE )
		ERRu();
#endif
	_KernelRow = Global.GetCurrentRow();

	if ( !strcmp( UIDesignation, "main" ) ) {
		_Register( Window, UI.Main );
		Global.GetCurrentObject().DisplayEndianess();
	} else if ( !strcmp( UIDesignation, "page" ) ) {
		_Register( Window, UI.Page );
	} else
		ERRu();
*/
RR
RN
RE
}

NS_IMETHODIMP emobdacom::RegisteringEnd( void )
{
RP
RBB
//	Global.DismissCurrentObject();
RR
RN
RE
}

NS_IMETHODIMP emobdacom::Unregister( void )
{
RP
RBB
//	Global.Delete( _KernelRow );
RR
RN
RE
}

emobdacom::emobdacom( void )
{
}

emobdacom::~emobdacom( void )
{
}



/* Gecko required part. */

#include "nsIGenericFactory.h"

NS_IMPL_ISUPPORTS1(emobdacom, iemobdacom)
NS_GENERIC_FACTORY_CONSTRUCTOR(emobdacom)

static nsModuleComponentInfo components[] =
{
    {
       EMOBDACOM_CLASSNAME, 
       EMOBDACOM_CID,
       EMOBDACOM_CONTRACTID,
       emobdacomConstructor,
    },
	NSXPCM_COMPONENTS,
};

NS_IMPL_NSGETMODULE("EMOBDAModule", components) 
