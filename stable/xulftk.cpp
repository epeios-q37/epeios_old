/*
	'xulftk' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'xulftk' header file ('xulftk.h').
	Copyright (C) 2004 Claude SIMON.

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

#define XULFTK__COMPILATION

#include "xulftk.h"

class xulftktutor
: public ttr_tutor
{
public:
	xulftktutor( void )
	: ttr_tutor( XULFTK_NAME )
	{
#ifdef XULFTK_DBG
		Version = XULFTK_VERSION "\b\bD $";
#else
		Version = XULFTK_VERSION;
#endif
		Owner = XULFTK_OWNER;
		Date = "$Date$";
	}
	virtual ~xulftktutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#include "xulfrg.h"

using namespace xulftk;

#define PREFIX	XULFKL_NAME "_" 

void xulftk::error_reporting_functions__::FBLFRDReportError(
	fblovl::reply__ Reply,
	const char *RawMessage )
{
ERRProlog
	str::string Message;
	STR_BUFFER___ Buffer;
ERRBegin
	if ( _UI == NULL )
		ERRc();

	Message.Init( _Rack.GetTranslation( "ErrorType", PREFIX, Buffer ) );

	Message.Append( " : " );

	Message.Append( fblovl::GetTranslation( Reply, _Rack, Buffer ) );

	Message.Append( '\n' );

	Message.Append( _Rack.GetTranslation( "ErrorMessage", PREFIX, Buffer ) );

	Message.Append( " : " );

	Message.Append( RawMessage );

	_UI->LogAndPrompt( Message );
ERRErr
ERREnd
ERREpilog
}

enum annex_type__
{
	atEmbedded,
	atFile,
	atVolatile,
	at_amount,
	at_Undefined
};


static annex_type__ GetAnnexType_( const rgstry::multi_level_registry_ &Registry )
{
	annex_type__ Type = at_Undefined;
ERRProlog
	str::string Value;
ERRBegin
	Value.Init();

	if ( xulfrg::GetRawAnnexType( Registry, Value ) ) {
		if ( Value == "Embedded" )
			Type = atEmbedded;
		else if ( Value == "File" )
			Type = atFile;
		else if ( Value == "Volatile " )
			Type = atVolatile;
	}
ERRErr
ERREnd
ERREpilog
	return Type;
}

static bso::bool__ IsValid_(
	const str::string_ &Value,
	bso::bool__ AlpahaNumericOnly )	// A 'true' si la valeur doit être alpha-numérique, comme un libellé d'attribut, par example.
{
	epeios::row__ Row = Value.First();

	if ( Value.Amount() == 0 )
		return false;

	if ( !AlpahaNumericOnly )
		return true;

	while ( Row != NONE ) {
		if ( !isalnum( Value( Row ) ) && ( Value( Row ) != '_' ) )
			return false;

		Row = Value.Next( Row );
	}

	return true;
}


static const char *GetAnnexTarget_( 
	xulfui::ui___ &UI,
	const rgstry::multi_level_registry_ &Registry,
	const lcl::rack__ &Rack,
	bso::bool__ AlphaNumericOnly,	
	STR_BUFFER___ &Buffer )
{
	const char *Result = NULL;
ERRProlog
	str::string Target, Translation;
ERRBegin
	Target.Init();

	xulfrg::GetAnnexTarget( Registry, Target );

	if ( !IsValid_( Target, AlphaNumericOnly ) ) {
		Translation.Init();
		Rack.GetTranslation( "MissingOrBadAnnexTargetDefinition", PREFIX, Translation );
		UI.LogAndPrompt( Translation );
		ERRReturn;
	}

	Result = Target.Convert( Buffer );
ERRErr
ERREnd
ERREpilog
	return Result;
}

template <typename bag, typename flow> static flow *GetEmbeddedFlow_(
	bag &Bag,
	xulfui::ui___ &UI,
	const rgstry::multi_level_registry_ &Registry,
	const lcl::rack__ &Rack )
{
	flow *Flow = NULL;
ERRProlog
	STR_BUFFER___ Buffer;
	const char *Target = NULL;
ERRBegin
	if ( ( Target = GetAnnexTarget_( UI, Registry, Rack, true, Buffer ) ) == NULL )
		ERRReturn;

	Bag.Embedded.Init();
	nsxpcm::GetAttribute( nsxpcm::GetElement( UI.Main().Window() ), Target, Bag.Embedded );
	Bag.EmbeddedFlow.Init( Bag.Embedded );
	Flow = &Bag.EmbeddedFlow;
ERRErr
ERREnd
ERREpilog
	return Flow;
}

template <typename bag, typename flow> static flow *GetFileFlow_(
	bag &Bag,
	xulfui::ui___ &UI,
	const rgstry::multi_level_registry_ &Registry,
	const lcl::rack__ &Rack )
{
	flow *Flow = NULL;
ERRProlog
	STR_BUFFER___ Buffer;
	const char *Target = NULL;
ERRBegin
	if ( ( Target = GetAnnexTarget_( UI, Registry, Rack, false, Buffer ) ) == NULL )
		ERRReturn;

	if ( Bag.FileFlow.Init( Target, err::hUserDefined ) != fil::sSuccess )
		Flow = &Bag.VoidFlow;
	else
		Flow = &Bag.FileFlow;
ERRErr
ERREnd
ERREpilog
	return Flow;
}

template <typename bag, typename flow> static flow  *GetVolatileFlow_( bag &Bag )
{
	return &Bag.VoidFlow;
}

template <typename bag, typename flow> static flow  *GetFlow_(
	bag &Bag,
	xulfui::ui___ &UI,
	const rgstry::multi_level_registry_ &Registry,
	const lcl::rack__ &Rack )
{
	flow *Flow = NULL;
ERRProlog
	str::string Translation;
ERRBegin
	switch ( GetAnnexType_( Registry )  ) {
	case atEmbedded:
		Flow = GetEmbeddedFlow_<bag, flow>( Bag, UI, Registry, Rack );
		break;
	case atFile:
		Flow = GetFileFlow_<bag, flow>( Bag, UI, Registry, Rack );
		break;
	case atVolatile:
		Flow = GetVolatileFlow_<bag, flow>( Bag );
		break;
	case at_Undefined:
			Translation.Init();
			Rack.GetTranslation( "MissingOrBadAnnexTypeDefinition", PREFIX, Translation );
			UI.LogAndPrompt( Translation );
			ERRReturn;
		break;
	default:
		ERRc();
		break;
	}
ERRErr
ERREnd
ERREpilog
	return Flow;
}

class settings_
{
public:
	struct s {
		str::string_::s
			Id,
			Content;
	};
	str::string_
		Id,
		Content;
	settings_( s &S )
	: Id( S.Id ),
	  Content( S.Content )
	{}
	void reset( bso::bool__ P = true )
	{
		Id.reset( P );
		Content.reset( P );
	}
	void plug( mmm::E_MULTIMEMORY_ &MM )
	{
		Id.plug( MM );
		Content.plug( MM );
	}
	settings_ &operator =( const settings_ &S )
	{
		Id = S.Id;
		Content = S.Content;

		return *this;
	}
	void Init( void )
	{
		Id.Init();
		Content.Init();
	}
};

E_AUTO( settings )

static void Write_(
	const settings_ &Settings,
	flw::oflow__ &Flow )
{
	Settings.Id.NewWriteToFlow( Flow, true );
	Settings.Content.NewWriteToFlow( Flow, true );
}

static void Read_(
	flw::iflow__ &Flow,
	settings_ &Settings )
{
	Settings.Id.NewReadFromFlow( Flow, 0 );
	Settings.Content.NewReadFromFlow( Flow, 0 );
}

typedef ctn::E_XCONTAINER_( settings_ ) settings_set_;
E_AUTO( settings_set );

static void Write_(
	const settings_set_ &Set,
	flw::oflow__ &Flow )
{
	epeios::row__ Row = Set.First();
	ctn::E_CITEM( settings_ ) Settings;

	dtfptb::NewPutSize( Set.Amount(), Flow );

	Settings.Init( Set );

	while ( Row != NONE ) {
		Write_( Settings( Row ), Flow );

		Row = Set.Next( Row );
	}
}

static void Read_(
	flw::iflow__ &Flow,
	settings_set_ &Set )
{
ERRProlog
	epeios::size__ Amount = 0;
	settings Settings;
ERRBegin
	Amount = dtfptb::NewGetSize( Flow );

	while ( Amount-- != 0 ) {
		Settings.Init();

		Read_( Flow, Settings );

		Set.Append(Settings );
	}
ERRErr
ERREnd
ERREpilog
}

static epeios::row__ Search_(
	const str::string_ &Id,
	const settings_set_ &Set )
{
	epeios::row__ Row = Set.First();
	ctn::E_CITEM( settings_ ) Settings;

	Settings.Init( Set );

	while ( ( Row != NONE ) && ( Settings( Row ).Id != Id ) )
		Row = Set.Next( Row );

	return Row;
}

static bso::bool__ Get_(
	const str::string_ &Id,
	const settings_set_ &Set,
	str::string_ &Content )
{
	epeios::row__ Row = Search_( Id, Set );
	ctn::E_CITEM( settings_ ) Settings;

	if ( Row != NONE ) {
		Settings.Init( Set );

		Content = Settings( Row ).Content;
	}

	return Row != NONE;
}

static void Set_(
	const str::string_ &Id,
	const str::string_ &Content,
	settings_set_ &Set )
{
ERRProlog
	epeios::row__ Row = NONE;
	settings Settings;
ERRBegin
	Row = Search_( Id, Set );

	Settings.Init();

	Settings.Id = Id;
	Settings.Content = Content;

	if ( Row != NONE )
		Set.Store( Settings, Row );
	else
		Set.Append( Settings );
ERRErr
ERREnd
ERREpilog
}

struct ibag___ {
	str::string Embedded;
	flf::file_iflow___ FileFlow;
	flx::E_STRING_IFLOW__  EmbeddedFlow;
	flw::iflow__ &VoidFlow;
	ibag___ ( void )
	: VoidFlow( flx::VoidIFlow )
	{}
	void Init( void )
	{
		// Standardisation.
	}
};

static bso::bool__ RetrieveSettingsSet_(
	xulfui::ui___ &UI,
	const rgstry::multi_level_registry_ &Registry,
	const lcl::rack__ &Rack,
	settings_set_ &Set )
{
	bso::bool__ Success = false;
ERRProlog
	flw::iflow__ *Flow = NULL;
	ibag___ Bag;
	STR_BUFFER___ Buffer;
ERRBegin
	Bag.Init();
	
	if ( ( Flow = GetFlow_<ibag___,flw::iflow__>( Bag, UI, Registry, Rack ) ) == NULL ) {
		UI.LogAndPrompt( Rack.GetTranslation( "UnableToRetrieveSettings", PREFIX, Buffer ) );
		ERRReturn;
	}

	Flow->EOFD( (const void *)"\0", 1 );	// Si vide, pour signaler une taille de 0 à la fonction ci-dessous.

	Read_( *Flow, Set );

	Success = true;
ERRErr
ERREnd
ERREpilog
	return Success;
}

void xulftk::trunk___::_DefineSession(
	const str::string_ &ProjectFileName,	// Si non vide, contient le nom du fichier projet avec lequel préremplir le 'SessionForm'.
	const xpp::criterions___ &Criterions,
	const rgstry::multi_level_registry_ &Registry )
{
ERRProlog
	frdkrn::backend_extended_type__ Type = frdkrn::bxt_Undefined;
	str::string Location;
ERRBegin

	if ( ProjectFileName.Amount() != 0 ) {
		Handle_( Kernel().LoadProject( ProjectFileName, _TargetName, Criterions ) );
		Location.Init();
		frdrgy::GetBackendLocation( Kernel().Registry(), Location );
	}

	switch ( Type = frdkrn::GetBackendExtendedType( Kernel().Registry() ) ) {
	case frdkrn::bxtPredefined:
		ERRl();
		break;
	case frdkrn::bxtDaemon:
		UI().SessionForm().Widgets.DaemonBackendLocationTextbox.SetValue( Location );
		break;
	case frdkrn::bxtEmbedded:
		UI().SessionForm().Widgets.EmbeddedBackendFileNameTextbox.SetValue( Location );
		break;
	case frdkrn::bxt_Undefined:
		break;
	default:
		ERRc();
		break;
	}

	UI().SessionForm().Update( Type );
ERRErr
ERREnd
ERREpilog
}


void xulftk::trunk___::_ApplySession( const frdkrn::compatibility_informations__ &CompatibilityInformations )
{
ERRProlog
	xtf::extended_text_iflow__ XFlow;
	settings_set Set;
	str::string Settings;
	flx::E_STRING_IFLOW__ Flow;
	str::string Value;
	frdkrn::backend_extended_type__ Type = frdkrn::bxt_Undefined;
ERRBegin
	Set.Init();

	RetrieveSettingsSet_( UI(), Registry(), Kernel().LocaleRack(), Set );

	Settings.Init();

	Get_( Kernel().GetId(), Set, Settings );

	if ( Settings.Amount() != 0 ) {
		Flow.Init( Settings );
		Flow.EOFD( XTF_EOXT );

		XFlow.Init( Flow );

		Handle_( Kernel().FillSettingsRegistry( XFlow, xpp::criterions___() ) );
	}

	Value.Init();
	UI().SessionForm().Widgets.BackendTypeSwitchMenulist.GetValue( Value );

#if 0	// Reportté.
	switch ( Type = frdkrn::GetBackendExtendedType( Value ) ) {
	case frdkrn::bxtPredefined:
		Value.Init();
		UI().SessionForm().Widgets.PredefinedBackendSwitchMenuitem.GetValue( Value );
		Registry().SetValue( xulfrg::paths::backend_selection::Value );

	frdkrn::SetBackendExtendedType( Registry(), UI().SessionForm().Widgets.PredefinedBackendSwitchMenuitem.

	Registry().SetValue( xulfrg::paths::backend_selection::Mode, 
#endif
ERRErr
ERREnd
ERREpilog
}

bso::bool__ xulftk::trunk___::_DefendSession( void )
{
	bso::bool__ Confirmed = false;
ERRProlog
	str::string Translation;
	STR_BUFFER___ Buffer;
ERRBegin
	Translation.Init( Kernel().LocaleRack().GetTranslation( "ProjectClosingAsking", PREFIX, Buffer ) );
	Translation.Append( ' ' );
	Translation.Append( Kernel().LocaleRack().GetTranslation( "ProjectClosingWarning", PREFIX, Buffer ) );

	Confirmed = UI().Confirm( Translation );
ERRErr
ERREnd
ERREpilog
	return Confirmed;
}

struct obag___ {
	str::string Embedded;
	flf::file_oflow___ FileFlow;
	flx::E_STRING_OFLOW___  EmbeddedFlow;
	flw::oflow__ &VoidFlow;
	obag___ ( void )
	: VoidFlow( flx::VoidOFlow )
	{}
	void Init( void )
	{
		// Standardisation.
	}
};

static bso::bool__ StoreSettingsSet_(
	const settings_set_ &Set,
	xulfui::ui___ &UI,
	const rgstry::multi_level_registry_ &Registry,
	const lcl::rack__ &Rack )
{
	bso::bool__ Success = false;
ERRProlog
	flw::oflow__ *Flow = NULL;
	obag___ Bag;
	STR_BUFFER___ Buffer;
ERRBegin
	Bag.Init();
	
	if ( ( Flow = GetFlow_<obag___,flw::oflow__>( Bag, UI, Registry, Rack ) ) == NULL ) {
		UI.LogAndPrompt( Rack.GetTranslation( "UnableToStoreSettings", PREFIX, Buffer ) );
		ERRReturn;
	}

	Write_( Set, *Flow );

	Success = true;
ERRErr
ERREnd
ERREpilog
	return Success;
}

static void GetSettings_(
	const xulfkl::kernel___ &Kernel,
	str::string_ &Settings )
{
ERRProlog
	flx::E_STRING_OFLOW___ Flow;
	txf::text_oflow__ TFlow;
	xml::writer Writer;
ERRBegin
	Flow.Init( Settings );
	TFlow.Init( Flow );
	Writer.Init( TFlow, xml::oCompact, xml::e_Default );

	Kernel.DumpSettingsRegistry( Writer );
ERRErr
ERREnd
ERREpilog
}


void xulftk::trunk___::_DropSession( void )
{
ERRProlog
	str::string Settings;
	settings_set Set;
ERRBegin

	Set.Init();

	if ( RetrieveSettingsSet_( UI(), Registry(), Kernel().LocaleRack(), Set ) ) {
		Settings.Init();
		GetSettings_( Kernel(), Settings );

		Set_( Kernel().GetId(), Settings, Set );

		StoreSettingsSet_( Set, UI(), Registry(), Kernel().LocaleRack() );
	}

	Handle_( Kernel().CloseProject() );
ERRErr
ERREnd
ERREpilog
}



/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class xulftkpersonnalization
: public xulftktutor
{
public:
	xulftkpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~xulftkpersonnalization( void )
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

static xulftkpersonnalization Tutor;

ttr_tutor &XULFTKTutor = Tutor;
