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
#include "ui.h"

#define VERSION __DATE__ " " __TIME__

#define EMOBDACOM_CONTRACTID "@zeusw.org/emobdacom;1"
#define EMOBDACOM_CLASSNAME "EMOBDA"
// {cfee5019-09e1-4d5c-af89-0ffadf903df5}
#define EMOBDACOM_CID  IEMOBDACOM_IID

using kernel::krow__;
using kernel::Repository;

class emobdacom
: public iemobdacom
{
private:
	void _BaseRegistration( nsIDOMWindow *Window );
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

	Repository.GetCurrentObject().PreInit( Repository.CreateNewObject() );
RR
RN
RE
}

void emobdacom::_BaseRegistration( nsIDOMWindow *Window )
{
ERRProlog
	str::string Id;
ERRBegin
	ui::ui___ &UI = Repository.GetCurrentObject();

#ifdef XXX_DBG
	if ( _KernelRow != NONE )
		ERRu();
#endif
	_KernelRow = Repository.GetCurrentRow();

	Id.Init();
	nsxpcm::GetAttribute( nsxpcm::GetWindowElement( Window ), "id", Id );
//	nsxpcm::Log( Id );

	if ( Id == "wdwMain" ) {
		ui_main::RegisterMainUI( UI, Window );
		if ( nsxpcm::MasterWindow == NULL )
			nsxpcm::MasterWindow = Window;
	} else if ( Id == "wdwBackendSelectionDialogBox" ) {
		ui_bksdb::RegisterBackendSelectionUI( UI, Window );
	} else if ( Id == "wdwDatabaseSelectionDialogBox" ) {
		ui_dbsdb::RegisterDatabaseSelectionUI( UI, Window );
		UI.FillDatabaseSelectionList();
	} else if ( Id == "wdwDatabaseForm" ) {
		ui_dbs_f::RegisterDatabaseFormUI( UI, Window );
		UI.DatabaseForm.Textboxes.Name.Select();
	} else if ( Id == "pgeStructure" )
		ui_struct::RegisterStructureUI( UI, Window );
	else if ( Id == "pgeListView" )
		ui_lst_v::RegisterListViewUI( UI, Window );
	else if ( Id == "pgeRecordForm" )
		ui_rcd_f::RegisterRecordFormUI( UI, Window );
	else if ( Id == "pgeRecordView" )
		ui_rcd_v::RegisterRecordViewUI( UI, Window );
	else
		ERRu();
ERRErr
ERREnd
ERREpilog
}

NS_IMETHODIMP emobdacom::Register( nsIDOMWindow *Window )
{
	// Don't know how to get a 'window' fro its 'document'.
RP
RBB
	_BaseRegistration( Window );
RR
RN
RE
}

NS_IMETHODIMP emobdacom::RegisterAlone( nsIDOMWindow *Window )
{
	// Don't know how to get a 'window' fro its 'document'.
RP
RBB
	_BaseRegistration( Window );

	Repository.DismissCurrentObject();
RR
RN
RE
}

NS_IMETHODIMP emobdacom::RegisteringEnd( const char *ConfigFile )
{
RP
RBB
	kernel::kernel___ &Kernel = Repository.GetCurrentObject();
	
	Repository.DismissCurrentObject();

	if ( ConfigFile != "" )
		Kernel.Init( ConfigFile );
	else
		Kernel.DefineSession();
RR
RN
RE
}

NS_IMETHODIMP emobdacom::Retire( void )
{
RP
RBB
	Repository.Delete( _KernelRow );
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
