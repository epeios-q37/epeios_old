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

#include "kernel.h"

#define VERSION __DATE__ " " __TIME__

#define EMOBDACOM_CONTRACTID "@zeusw.org/emobdacom;1"
#define EMOBDACOM_CLASSNAME "EMOBDA"
// {cfee5019-09e1-4d5c-af89-0ffadf903df5}
#define EMOBDACOM_CID  IEMOBDACOM_IID

E_ROW( krow__ );	// Core row; see below.

class emobdacom
: public iemobdacom
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_IEMOBDACOM

  emobdacom();
private:
  krow__ _KernelRow;
  ~emobdacom();
protected:
  /* additional members */
};


static nsxpcm::repository< kernel::kernel___, krow__> Repository_;

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

	Repository_.CreateNewObject();
RR
RN
RE
}

template <typename widget> static void Register_(
	widget &Widget,
	nsIDOMDocument *Document,
	const char *Id )
{
	Widget.Init( Repository_.GetCurrentObject() );
	nsxpcm::Register( Widget, Document, Id );
}

static void Register_(
	ui::main__ &UI,
	nsIDOMWindow *Window )
{
	UI.Set( Window );

	Register_( UI.TableLocationTextbox, UI.Document, "tableLocation" );
	Register_( UI.CreateTableButton, UI.Document, "createTable" );

	Register_( UI.FieldNameTextbox, UI.Document, "fieldName" );
	Register_( UI.AddFieldButton, UI.Document, "addField" );

	Register_( UI.FieldListListbox, UI.Document, "fieldList" );
	Register_( UI.RemoveFieldButton, UI.Document, "removeField" );
}

static void Register_(
	ui::structure__ &UI,
	nsIDOMWindow *Window )
{
	UI.Set( Window );

	Register_( UI.FormBroadcaster, UI.Document, "formBroadcaster" );

	UI.Items = nsxpcm::GetElementById( UI.Document, "items" );

	UI.DatabaseButtonsPanel = nsxpcm::GetElementById( UI.Document, "databaseButtonsPanel" );
	UI.TableButtonsPanel = nsxpcm::GetElementById( UI.Document, "tableButtonsPanel" );
	UI.FieldButtonsPanel = nsxpcm::GetElementById( UI.Document, "fieldButtonsPanel" );


	Register_( UI.BrowseTree, UI.Document, "browseTree" );

	Register_( UI.ButtonsDeck, UI.Document, "buttonsDeck" );

	Register_( UI.CreateTableButton, UI.Document, "createTableButton" );
	Register_( UI.ModifyTableButton, UI.Document, "modifyTableButton" );

	Register_( UI.CreateFieldButton, UI.Document, "createFieldButton" );
	Register_( UI.ModifyFieldButton, UI.Document, "modifyFieldButton" );

	Register_( UI.FormDeck, UI.Document, "formDeck" );

	UI.DatabaseFormPanel = nsxpcm::GetElementById( UI.Document, "databaseFormPanel" );
	UI.TableFormPanel = nsxpcm::GetElementById( UI.Document, "tableFormPanel" );
	UI.FieldFormPanel = nsxpcm::GetElementById( UI.Document, "fieldFormPanel" );

	Register_( UI.ApplyFormButton, UI.Document, "applyFormButton" );
	Register_( UI.CancelFormButton, UI.Document, "cancelFormButton" );
}

NS_IMETHODIMP emobdacom::Register(
	nsIDOMWindow *Window,
	const char *UIDesignation )
{
	// Ne sait pas récupèrer une 'window' à partir de son document.
RP
RBB
	ui::ui__ &UI = Repository_.GetCurrentObject().UI;

#ifdef XXX_DBG
	if ( _KernelRow != NONE )
		ERRu();
#endif
	_KernelRow = Repository_.GetCurrentRow();

	if ( !strcmp( UIDesignation, "main" ) )
		Register_( UI.Main, Window );
	else if ( !strcmp( UIDesignation, "structure" ) )
		Register_( UI.Structure, Window );
	else
		ERRu();
RR
RN
RE
}

NS_IMETHODIMP emobdacom::RegisteringEnd( void )
{
RP
	nsIDOMWindowInternal *Console = NULL;
RBB
	nsxpcm::GetJSConsole( Repository_.GetCurrentObject().UI.Main.Window, &Console );

	Repository_.GetCurrentObject().RefreshStructureView();

	Repository_.DismissCurrentObject();
RR
RN
RE
}

NS_IMETHODIMP emobdacom::Retire( void )
{
RP
RBB
	Repository_.Delete( _KernelRow );
RR
RN
RE
}

emobdacom::emobdacom( void )
{
	_KernelRow = NONE;
}

emobdacom::~emobdacom( void )
{
}

static class starter
{
public:
	starter( void )
	{
		::Repository_.Init();
	}
	~starter( void )
	{}
} Stater_;

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
