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

static bso::bool__ IsValid_( const str::string_ &Value )
{
	epeios::row__ Row = Value.First();

	if ( Value.Amount() == 0 )
		return false;

	while ( Row != NONE ) {
		if ( !isalnum( Value( Row ) || ( Value( Row ) != '_' ) ) )
			return false;

		Row = Value.Next( Row );
	}

	return true;
}


static const char *GetTarget_( 
	nsIDOMWindow *Window,
	const rgstry::multi_level_registry_ &Registry,
	const lcl::rack__ &Rack,
	STR_BUFFER___ &Buffer )
{
	const char *Result = NULL;
ERRProlog
	str::string Target, Translation;
ERRBegin
	Target.Init();

	xulfrg::GetParametersTarget( Registry, Target );

	if ( !IsValid_( Target ) ) {
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
	if ( ( Target = GetTarget_( Window, Registry, Rack, Buffer ) ) == NULL )
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
	if ( ( Target = GetTarget_( Window, Registry, Rack, Buffer ) ) == NULL )
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

void xulftk::trunk___::_ApplySession(
	const str::string_ &FileName,
	const xpp::criterions___ &Criterions,
	const frdkrn::compatibility_informations__ &CompatibilityInformations,
	const rgstry::multi_level_registry_ &Registry )
{
ERRProlog
	flw::iflow__ *Flow = NULL;
	ibag___ Bag;
ERRBegin
	Bag.Init();
	
	if ( ( Flow = GetFlow_<ibag___,flw::iflow__>( Bag, UI().Main().Window(), Registry, Kernel().LocaleRack() ) ) == NULL )
		ERRReturn;

	if ( Kernel().LoadProject( FileName, _TargetName, xtf::extended_text_iflow__( *Flow ), Criterions, CompatibilityInformations ) != frdkrn::sOK )
		UI().Alert( Kernel().Message() );
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

void xulftk::trunk___::_DropSession( void )
{
ERRProlog
	flw::oflow__ *Flow = NULL;
	obag___ Bag;
ERRBegin
	Bag.Init();
	
	if ( ( Flow = GetFlow_<obag___,flw::oflow__>( Bag, UI().Main().Window(), Registry(), Kernel().LocaleRack() ) ) == NULL )
		ERRReturn;

	Kernel().CloseProject( txf::text_oflow__( *Flow ) );
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
