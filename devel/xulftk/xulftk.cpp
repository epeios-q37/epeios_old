/*
	'xulftk' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'xulftk' header file ('xulftk.h').
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

enum storage__
{
	sEmbedded,
	sFile,
	sVolatile,
	s_amount,
	s_Undefined
};


static storage__ GetStorage_( const rgstry::multi_level_registry_ &Registry )
{
	storage__ Storage = s_Undefined;
ERRProlog
	str::string Value;
ERRBegin
	Value.Init();

	if ( xulfrg::GetRawParametersStorage( Registry, Value ) ) {
		if ( Value == "Embedded" )
			Storage = sEmbedded;
		else if ( Value == "File" )
			Storage = sFile;
		else if ( Value == "Volatile " )
			Storage = sVolatile;
	}
ERRErr
ERREnd
ERREpilog
	return Storage;
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


static const char *GetTarget_( 
	nsIDOMWindow *Window,
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

	xulfrg::GetParametersTarget( Registry, Target );

	if ( !IsValid_( Target, AlphaNumericOnly ) ) {
		Translation.Init();
		Rack.GetTranslation( "MissingOrBadParametersTargetDefinition", PREFIX, Translation );
		nsxpcm::Alert( Window, Translation );
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
	nsIDOMWindow *Window,
	const rgstry::multi_level_registry_ &Registry,
	const lcl::rack__ &Rack )
{
	flow *Flow = NULL;
ERRProlog
	STR_BUFFER___ Buffer;
	const char *Target = NULL;
ERRBegin
	if ( ( Target = GetTarget_( Window, Registry, Rack, true, Buffer ) ) == NULL )
		ERRReturn;

	Bag.Embedded.Init();
	nsxpcm::GetAttribute( nsxpcm::GetElement( Window ), Target, Bag.Embedded );
	Bag.EmbeddedFlow.Init( Bag.Embedded );
	Flow = &Bag.EmbeddedFlow;
ERRErr
ERREnd
ERREpilog
	return Flow;
}

template <typename bag, typename flow> static flow *GetFileFlow_(
	bag &Bag,
	nsIDOMWindow *Window,
	const rgstry::multi_level_registry_ &Registry,
	const lcl::rack__ &Rack )
{
	flow *Flow = NULL;
ERRProlog
	STR_BUFFER___ Buffer;
	const char *Target = NULL;
ERRBegin
	if ( ( Target = GetTarget_( Window, Registry, Rack, false, Buffer ) ) == NULL )
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
	nsIDOMWindow *Window,
	const rgstry::multi_level_registry_ &Registry,
	const lcl::rack__ &Rack )
{
	flow *Flow = NULL;
ERRProlog
	str::string Translation;
ERRBegin
	switch ( GetStorage_( Registry )  ) {
	case sEmbedded:
		Flow = GetEmbeddedFlow_<bag, flow>( Bag, Window, Registry, Rack );
		break;
	case sFile:
		Flow = GetFileFlow_<bag, flow>( Bag, Window, Registry, Rack );
		break;
	case sVolatile:
		Flow = GetVolatileFlow_<bag, flow>( Bag );
		break;
	case s_Undefined:
			Translation.Init();
			Rack.GetTranslation( "MissingOrBadParametersStorageDefinition", PREFIX, Translation );
			nsxpcm::Alert( Window, Translation );
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

class parameters_
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
	parameters_( s &S )
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
	parameters_ &operator =( const parameters_ &P )
	{
		Id = P.Id;
		Content = P.Content;

		return *this;
	}
	void Init( void )
	{
		Id.Init();
		Content.Init();
	}
};

E_AUTO( parameters )

static void Write_(
	const parameters_ &Parameters,
	flw::oflow__ &Flow )
{
	Parameters.Id.NewWriteToFlow( Flow, true );
	Parameters.Content.NewWriteToFlow( Flow, true );
}

static void Read_(
	flw::iflow__ &Flow,
	parameters_ &Parameters )
{
	Parameters.Id.NewReadFromFlow( Flow, 0 );
	Parameters.Content.NewReadFromFlow( Flow, 0 );
}

typedef ctn::E_XCONTAINER_( parameters_ ) parameters_set_;
E_AUTO( parameters_set );

static void Write_(
	const parameters_set_ &Set,
	flw::oflow__ &Flow )
{
	epeios::row__ Row = Set.First();
	ctn::E_CITEM( parameters_ ) Parameters;

	dtfptb::NewPutSize( Set.Amount(), Flow );

	Parameters.Init( Set );

	while ( Row != NONE ) {
		Write_( Parameters( Row ), Flow );

		Row = Set.Next( Row );
	}
}

static void Read_(
	flw::iflow__ &Flow,
	parameters_set_ &Set )
{
ERRProlog
	epeios::size__ Amount = 0;
	parameters Parameters;
ERRBegin
	Amount = dtfptb::NewGetSize( Flow );

	while ( Amount-- != 0 ) {
		Parameters.Init();

		Read_( Flow, Parameters );

		Set.Append( Parameters );
	}
ERRErr
ERREnd
ERREpilog
}

static epeios::row__ Search_(
	const str::string_ &Id,
	const parameters_set_ &Set )
{
	epeios::row__ Row = Set.First();
	ctn::E_CITEM( parameters_ ) Parameters;

	Parameters.Init( Set );

	while ( ( Row != NONE ) && ( Parameters( Row ).Id != Id ) )
		Row = Set.Next( Row );

	return Row;
}

static bso::bool__ Get_(
	const str::string_ &Id,
	const parameters_set_ &Set,
	str::string_ &Content )
{
	epeios::row__ Row = Search_( Id, Set );
	ctn::E_CITEM( parameters_ ) Parameters;

	if ( Row != NONE ) {
		Parameters.Init( Set );

		Content = Parameters( Row ).Content;
	}

	return Row != NONE;
}

static void Set_(
	const str::string_ &Id,
	const str::string_ &Content,
	parameters_set_ &Set )
{
ERRProlog
	epeios::row__ Row = NONE;
	parameters Parameters;
ERRBegin
	Row = Search_( Id, Set );

	Parameters.Init();

	Parameters.Id = Id;
	Parameters.Content = Content;

	if ( Row != NONE )
		Set.Store( Parameters, Row );
	else
		Set.Append( Parameters );
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

static bso::bool__ RetrieveParametersSet_(
	xulfui::ui___ &UI,
	const rgstry::multi_level_registry_ &Registry,
	const lcl::rack__ &Rack,
	parameters_set_ &Set )
{
	bso::bool__ Success = false;
ERRProlog
	flw::iflow__ *Flow = NULL;
	ibag___ Bag;
	STR_BUFFER___ Buffer;
ERRBegin
	Bag.Init();
	
	if ( ( Flow = GetFlow_<ibag___,flw::iflow__>( Bag, UI.Main().Window(), Registry, Rack ) ) == NULL ) {
		UI.LogAndPrompt( Rack.GetTranslation( "UnableToRetrieveParameters", PREFIX, Buffer ) );
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

void xulftk::trunk___::_ApplySession(
	const str::string_ &FileName,
	const xpp::criterions___ &Criterions,
	const frdkrn::compatibility_informations__ &CompatibilityInformations,
	const rgstry::multi_level_registry_ &Registry )
{
ERRProlog
	xtf::extended_text_iflow__ XFlow;
	parameters_set Set;
	str::string Parameters;
	flx::E_STRING_IFLOW__ Flow;
ERRBegin
	Handle_( Kernel().LoadProject( FileName, _TargetName, Criterions, CompatibilityInformations ) );

	Set.Init();

	RetrieveParametersSet_( UI(), Registry, Kernel().LocaleRack(), Set );

	Parameters.Init();

	Get_( Kernel().GetId(), Set, Parameters );

	Flow.Init( Parameters );
	Flow.EOFD( XTF_EOXT );

	XFlow.Init( Flow );

	Handle_( Kernel().FillParametersRegistry( XFlow, xpp::criterions___() ) );
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

static bso::bool__ StoreParametersSet_(
	const parameters_set_ &Set,
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
	
	if ( ( Flow = GetFlow_<obag___,flw::oflow__>( Bag, UI.Main().Window(), Registry, Rack ) ) == NULL ) {
		UI.LogAndPrompt( Rack.GetTranslation( "UnableToStoreParameters", PREFIX, Buffer ) );
		ERRReturn;
	}

	Write_( Set, *Flow );

	Success = true;
ERRErr
ERREnd
ERREpilog
	return Success;
}

static void GetParameters_(
	const xulfkl::kernel___ &Kernel,
	str::string_ &Parameters )
{
ERRProlog
	flx::E_STRING_OFLOW___ Flow;
	txf::text_oflow__ TFlow;
	xml::writer Writer;
ERRBegin
	Flow.Init( Parameters );
	TFlow.Init( Flow );
	Writer.Init( TFlow, xml::oCompact, xml::e_Default );

	Kernel.DumpParametersRegistry( Writer );
ERRErr
ERREnd
ERREpilog
}


void xulftk::trunk___::_DropSession( void )
{
ERRProlog
	str::string Parameters;
	parameters_set Set;
ERRBegin

	Set.Init();

	if ( RetrieveParametersSet_( UI(), Registry(), Kernel().LocaleRack(), Set ) ) {
		Parameters.Init();
		GetParameters_( Kernel(), Parameters );

		Set_( Kernel().GetId(), Parameters, Set );

		StoreParametersSet_( Set, UI(), Registry(), Kernel().LocaleRack() );
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
