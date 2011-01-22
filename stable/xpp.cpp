/*
	'xpp' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'xpp' header file ('xpp.h').
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

#define XPP__COMPILATION

#include "xpp.h"

class xpptutor
: public ttr_tutor
{
public:
	xpptutor( void )
	: ttr_tutor( XPP_NAME )
	{
#ifdef XPP_DBG
		Version = XPP_VERSION "\b\bD $";
#else
		Version = XPP_VERSION;
#endif
		Owner = XPP_OWNER;
		Date = "$Date$";
	}
	virtual ~xpptutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#include "fnm.h"
#include "lcl.h"

using namespace xpp;

using xml::browser___;
using xml::token__;

#define NAME_ATTRIBUTE		"name"
#define SELECT_ATTRIBUTE	"select"
#define HREF_ATTRIBUTE		"href"
#define VALUE_ATTRIBUTE		"value"

#define DEFINE_TAG			"define"
#define EXPAND_TAG			"expand"
#define IFEQ_TAG			"ifeq"
#define SET_TAG				"set"
#define BLOC_TAG			"bloc"
#define ATTRIBUTE_ATTRIBUTE	"attribute"

static inline status__ Convert_( xml::status__ Status )
{
	return (status__)Status;
}

#define CASE( m )\
	case s##m:\
	return #m;\
	break

const char *xpp::GetLabel( status__ Status )
{
	if ( Status < Convert_( xml::s_amount ) )
		return xml::GetLabel( (xml::status__)Status );

	switch( Status ) {
	CASE( OK );
	CASE( NoTagsAllowedHere );
//	CASE( UnexpectedTag );
	CASE( UnexpectedValue );
	CASE( UnknownDirective );
//	CASE( AttributeAlreadyDefined );
	CASE( UnexpectedAttribute );
	CASE( UnknownAttribute );
	CASE( MissingNameAttribute );
	CASE( MissingSelectOrHRefAttribute );
	CASE( MissingSelectAttribute );
	CASE( MissingValueAttribute );
	CASE( MissingNameAndValueAttributes );
	CASE( MissingSelectAndValueAttributes );
//	CASE( UnknownVariable );
	CASE( MustBeEmpty );
	CASE( TooManyTags );
	CASE( UnableToOpenFile );
//	CASE( NestingOverflow );
	CASE( UnknownMacro );
	CASE( BadAttributeDefinitionSyntax );
	default:
		ERRu();
		break;
	}

	return NULL;	// Pour éviter un 'warning'.
}

const str::string_ &xpp::GetTranslation(
	status__ Status,
	const str::string_ &Language,
	const lcl::locale_ &Locale,
	str::string_ &Translation )
{
ERRProlog
	str::string MessageLabel;
ERRBegin
	MessageLabel.Init( "EXML_" );

	MessageLabel.Append( GetLabel( Status ) );

	Locale.GetTranslation( MessageLabel, Language, Translation );
ERRErr
ERREnd
ERREpilog
	return Translation;
}

void xpp::_qualified_preprocessor_directives___::Init( const str::string_ &Namespace )
{
	NamespaceWithSeparator.Init( Namespace );
	NamespaceWithSeparator.Append( ':' );

	DefineTag.Init( NamespaceWithSeparator );
	DefineTag.Append( DEFINE_TAG );

	ExpandTag.Init( NamespaceWithSeparator );
	ExpandTag.Append( EXPAND_TAG );

	SetTag.Init( NamespaceWithSeparator );
	SetTag.Append( SET_TAG );

	IfeqTag.Init( NamespaceWithSeparator );
	IfeqTag.Append( IFEQ_TAG );

	BlocTag.Init( NamespaceWithSeparator );
	BlocTag.Append( BLOC_TAG );

	AttributeAttribute.Init( NamespaceWithSeparator );
	AttributeAttribute.Append( ATTRIBUTE_ATTRIBUTE );
}

static mdr::size__ Fill_(
	mdr::datum__ *&Buffer,
	mdr::size__ &Wanted,
	const str::string_ &Data,
	mdr::size__ &Position )
{
	mdr::size__ Available = Data.Amount() - Position;

	if ( Wanted < Available )
		Available = Wanted;

	if ( Available ) {
		Data.Recall( Position, Available, (bso::char__ *&)Buffer );
		Position += Available;
		Wanted -= Available;
		Buffer += Available;
	}

	return Available;
}

static inline bso::bool__ BelongsToNamespace_(
	const str::string_ &Name,
	const str::string_ &NamespaceWithSeparator )
{
	if ( Name.Amount() == 0 )
		ERRc();

	if ( Name.Amount() < NamespaceWithSeparator.Amount() )
		return false;

	return str::Compare( Name, NamespaceWithSeparator, Name.First(), NamespaceWithSeparator.First(), NamespaceWithSeparator.Amount() ) == 0;
}

enum directive__ {
	dDefine,
	dExpand,
	dIfeq,
	dBloc,
	dSet,
	dAttribute,
	t_amount,
	t_Undefined
};

static inline directive__ GetDirective_(
	const str::string_ &Name,
	const _qualified_preprocessor_directives___ &Directives )
{
	if ( Directives.DefineTag == Name )
		return dDefine;
	else if ( Directives.ExpandTag == Name )
		return dExpand;
	else if ( Directives.IfeqTag == Name )
		return dIfeq;
	else if ( Directives.BlocTag == Name )
		return dBloc;
	else if ( Directives.SetTag == Name )
		return dSet;
	else if ( Directives.AttributeAttribute == Name )
		return dAttribute;

	return ::t_Undefined;
}

static status__ AwaitingToken_(
	browser___ &Browser,
	token__ AwaitedToken,
	status__ StatusIfNotAwaitedToken )
{
	token__ Token = xml::t_Undefined;

	switch ( Token = Browser.Browse( xml::tfAll & ~xml::tfComment ) ) {
		case xml::tProcessed:
			ERRc();
			break;
		case xml::tError:
			return Convert_( Browser.Status() );
			break;
		default:
			if ( Token != AwaitedToken )
				return StatusIfNotAwaitedToken;
			break;
	}

	return sOK;
}

static status__ GetNameAttributeValue_(
	browser___ &Browser,
	str::string_ &Value )
{
	status__ Status = AwaitingToken_( Browser, xml::tAttribute, sMissingNameAttribute );

	if ( Status != sOK )
		return Status;

	if ( Browser.AttributeName() != NAME_ATTRIBUTE )
		return sUnexpectedAttribute;

	Value = Browser.Value();

	return sOK;
}

static status__ RetrieveTree_(
	browser___ &Browser,
	str::string_ &Tree )
{
	bso::ulong__ Nesting = 0;
	status__ Status = s_Undefined;

	while ( Status == s_Undefined ) {
		switch ( Browser.Browse( xml::tfStartTag | xml::tfEndTag | xml::tfValue ) ) {
		case xml::tStartTag:
			Tree.Append( Browser.DumpData() );
			if ( Nesting == BSO_ULONG_MAX )
				ERRc();
			Nesting++;
			break;
		case xml::tValue:
			if( Nesting == 0 )
				return sUnexpectedValue;
			else
				Tree.Append( Browser.DumpData() );
			break;
		case xml::tEndTag:
			Tree.Append( Browser.DumpData() );
			switch ( Nesting ) {
			case 0:
				ERRc();
				break;
			case 1:
				Status = AwaitingToken_( Browser, xml::tEndTag, sTooManyTags );
				break;
			default:
				Nesting--;
				break;
			}
			break;
		case xml::tError:
			return Convert_( Browser.Status() );
			break;
		case xml::tProcessed:
			ERRc();
			break;
		default:
			ERRc();
			break;
		}
	}

	return Status;
}

static status__ GetDefineNameAndContent_(
	browser___ &Browser,
	str::string_ &Name,
	str::string_ &Content )
{
	status__ Status = sOK;

	if ( ( Status = GetNameAttributeValue_( Browser, Name ) ) != sOK )
		return Status;

	if ( ( Status = AwaitingToken_( Browser, xml::tStartTagClosed, sUnexpectedAttribute ) ) != sOK )
		return Status;

	if ( ( Status = RetrieveTree_( Browser, Content ) ) != sOK )
		return Status;

/*	if ( ( Status = AwaitingToken_( Browser, xml::tEndTag, sTooManyTags ) ) != sOK )
		return Status;
*/
	return sOK;
}

status__ xpp::_extended_browser___::_HandleDefineDirective( _extended_browser___ *&Browser )	// 'Browser' est mis à 'NULL', ce qui est normal. 
{
	status__ Status = s_Undefined;
ERRProlog
	str::string Name, Content;
	xtf::coord__ Coord;
ERRBegin
	Browser = NULL;

	Coord = _Browser.GetCurrentCoord();

	Name.Init();
	Content.Init();

	if ( ( Status = GetDefineNameAndContent_( _Browser, Name, Content ) ) != sOK )
		ERRReturn;

	_Repository.Store( Name, Coord, _LocalizedFileName, Content );
ERRErr
ERREnd
ERREpilog
	return Status;
}

enum expand_type__ {
	etMacro,
	etFile,
	et_amount,
	et_Undefined
};

static inline expand_type__ GetExpandTypeAndValue_(
	browser___ &Browser,
	str::string_ &Value,
	status__ &Status )	// Siginfiant seulement si valeur retournée == 'et_Undefined'.
{
	expand_type__ Type = et_Undefined;
ERRProlog
	str::string AttributeName;
ERRBegin
	AttributeName.Init();

	if ( ( Status = AwaitingToken_( Browser, xml::tAttribute, sMissingSelectOrHRefAttribute ) ) != sOK )
		ERRReturn;

	if ( Browser.AttributeName() == HREF_ATTRIBUTE )
		Type = etFile;
	else if ( Browser.AttributeName() == SELECT_ATTRIBUTE )
		Type = etMacro;
	else {
		Status = sUnknownAttribute;
		ERRReturn;
	}

	Value = Browser.Value();
ERRErr
ERREnd
ERREpilog
	return Type;
}

status__ xpp::_extended_browser___::_HandleMacroExpand(
	const str::string_ &MacroName,
	_extended_browser___ *&Browser )
{
	status__ Status = s_Undefined;
ERRProlog
	str::string FileName;
	str::string Content;
	xtf::coord__ Coord;
ERRBegin
	FileName.Init();
	Content.Init();

	if ( !_Repository.Get( MacroName, Coord, FileName, Content ) ) {
		Status = sUnknownMacro;
		ERRReturn;
	}

	Browser = NewBrowser( _Repository, _Variables, _Directives );

	Status = Browser->InitWithContent( Content, FileName, Coord, _Directory );
ERRErr
	if ( Browser != NULL ) {
		delete Browser;
		Browser = NULL;
	}
ERREnd
	if ( Status != sOK ) {
		if ( Browser != NULL ) {
			delete Browser;
			Browser = NULL;
		}
	}
ERREpilog
	return Status;
}

status__ xpp::_extended_browser___::_HandleFileExpand(
	const str::string_ &FileName,
	_extended_browser___ *&Browser )
{
	status__ Status = s_Undefined;
ERRProlog
ERRBegin
	Browser = NewBrowser( _Repository, _Variables, _Directives );

	Status = Browser->InitWithFile( FileName, _Directory );
ERRErr
	if ( Browser != NULL ) {
		delete Browser;
		Browser = NULL;
	}
ERREnd
	if ( Status != sOK ) {
		if ( Browser != NULL ) {
			delete Browser;
			Browser = NULL;
		}
	}
ERREpilog
	return Status;
}

status__ xpp::_extended_browser___::_HandleExpandDirective( _extended_browser___ *&Browser )
{
	status__ Status = s_Undefined;
ERRProlog
	expand_type__ Type = et_Undefined;
	str::string Value;
ERRBegin
	Value.Init();

	switch ( GetExpandTypeAndValue_( _Browser, Value, Status ) ) {
	case etMacro:
		Status = _HandleMacroExpand( Value, Browser );
		break;
	case etFile:
		Status = _HandleFileExpand( Value, Browser );
		break;
	case et_Undefined:
		// 'Status' initialisé par 'etExpandTypeAndValue_(...)'.
		break;
	default:
		ERRc();
		break;
	}

	if ( Status == sOK )
		Status = AwaitingToken_( _Browser, xml::tStartTagClosed, sUnexpectedAttribute );

	if ( Status == sOK )
		Status = AwaitingToken_( _Browser, xml::tEndTag, sMustBeEmpty );

	_Browser.PurgeDumpData();

ERRErr
ERREnd
ERREpilog
	return Status;
}

static status__ GetSetNameAndValue_(
	browser___ &Browser,
	str::string_ &Name,
	str::string_ &Value )
{
	status__ Status = sOK;

	if ( ( Status = AwaitingToken_( Browser, xml::tAttribute, sMissingNameAndValueAttributes ) ) == sOK ) {
		if ( Browser.AttributeName() == NAME_ATTRIBUTE ) {
			Name.Append( Browser.Value() );

			if ( ( Status = AwaitingToken_( Browser, xml::tAttribute, sMissingValueAttribute ) ) == sOK ) {
				if ( Browser.AttributeName() == VALUE_ATTRIBUTE )
					Value.Append( Browser.Value() );
				else
					Status = sUnknownAttribute;
			}
		} else if ( Browser.AttributeName() == VALUE_ATTRIBUTE ) {
			Value.Append( Browser.Value() );

			if ( ( Status = AwaitingToken_( Browser, xml::tAttribute, sMissingNameAttribute ) ) == sOK ) {
				if ( Browser.AttributeName() == NAME_ATTRIBUTE )
					Name.Append( Browser.Value() );
				else
					Status = sUnknownAttribute;
			}
		}
	}

	return Status;
}

status__ xpp::_extended_browser___::_HandleSetDirective( _extended_browser___ *&Browser )	// 'Browser' est mis à 'NULL', ce qui est normal. 
{
	status__ Status = s_Undefined;
ERRProlog
	str::string Name, Value;
ERRBegin
	Browser = NULL;

	Name.Init();
	Value.Init();

	if ( ( Status = GetSetNameAndValue_( _Browser, Name, Value ) ) != sOK )
		ERRReturn;

	_Variables.Set( Name, Value );

	if ( Status == sOK )
		Status = AwaitingToken_( _Browser, xml::tStartTagClosed, sUnexpectedAttribute );

	if ( Status == sOK )
		Status = AwaitingToken_( _Browser, xml::tEndTag, sMustBeEmpty );

	_Browser.PurgeDumpData();

ERRErr
ERREnd
ERREpilog
	return Status;
}

static status__ GetIfeqSelectAndValue_(
	browser___ &Browser,
	str::string_ &Name,
	str::string_ &Value )
{
	status__ Status = sOK;

	if ( ( Status = AwaitingToken_( Browser, xml::tAttribute, sMissingSelectAndValueAttributes ) ) == sOK ) {
		if ( Browser.AttributeName() == SELECT_ATTRIBUTE ) {
			Name.Append( Browser.Value() );

			if ( ( Status = AwaitingToken_( Browser, xml::tAttribute, sMissingValueAttribute ) ) == sOK ) {
				if ( Browser.AttributeName() == VALUE_ATTRIBUTE )
					Value.Append( Browser.Value() );
				else
					Status = sUnknownAttribute;
			}
		} else if ( Browser.AttributeName() == VALUE_ATTRIBUTE ) {
			Value.Append( Browser.Value() );

			if ( ( Status = AwaitingToken_( Browser, xml::tAttribute, sMissingSelectAttribute ) ) == sOK ) {
				if ( Browser.AttributeName() == SELECT_ATTRIBUTE )
					Name.Append( Browser.Value() );
				else
					Status = sUnknownAttribute;
			}
		}
	}

	return Status;
}


status__ xpp::_extended_browser___::_HandleIfeqDirective( _extended_browser___ *&Browser )
{
	status__ Status = s_Undefined;
ERRProlog
	str::string Name, ExpectedValue, TrueValue, Content;
	xtf::coord__ Coord;
ERRBegin
	Browser = NULL;

	Name.Init();
	ExpectedValue.Init();

	if ( ( Status = GetIfeqSelectAndValue_( _Browser, Name, ExpectedValue ) ) != sOK )
		ERRReturn;

	if ( ( Status = AwaitingToken_( _Browser, xml::tStartTagClosed, sUnexpectedAttribute ) ) != sOK )
		ERRReturn;


	Content.Init();

	if ( ( Status = RetrieveTree_( _Browser, Content ) ) != sOK)
		ERRReturn;


	TrueValue.Init();

	if ( ( _Variables.Get( Name, TrueValue ) ) && ( ExpectedValue == TrueValue ) ) {
		Browser = NewBrowser( _Repository, _Variables, _Directives );

		Status = Browser->InitWithContent( Content, _LocalizedFileName, Coord, _Directory );
	}
ERRErr
	if ( Browser != NULL ) {
		delete Browser;
		Browser = NULL;
	}
ERREnd
	if ( Status != sOK ) {
		if ( Browser != NULL ) {
			delete Browser;
			Browser = NULL;
		}
	}
ERREpilog
	return Status;
}

status__ xpp::_extended_browser___::_HandlePreprocessorDirective(
	const str::string_ &DirectiveName,
	_extended_browser___ *&Browser )
{
	Browser = NULL;

	switch ( GetDirective_( DirectiveName, _Directives ) ) {
	case dDefine:
		return _HandleDefineDirective( Browser );
		break;
	case dBloc:
		ERRc();	// Traité en amont.
		break;
	case dExpand:
		return _HandleExpandDirective( Browser );
		break;
	case dSet:
		return _HandleSetDirective( Browser );
		break;
	case dIfeq:
		return _HandleIfeqDirective( Browser );
		break;
	default:
		return sUnknownDirective;
		break;
	}

	ERRc();

	return s_Undefined;	// Pour éviter un 'warning'.

}

static epeios::row__ ExtractAttributeName_(
	const str::string_ &Source,
	str::string_ &Value )
{
	epeios::row__ Row = Source.First();

	while ( ( Row != NONE )
		    && ( isalnum( Source( Row ) ) || ( Source( Row ) == ':' ) || ( Source( Row ) == '_' ) ) ) {
		Value.Append( Source( Row ) );
		Row = Source.Next( Row );
	}

	return Row;
}

status__ xpp::_extended_browser___::_HandleAttributeDirective(
	const str::string_ &Parameters,
	_extended_browser___ *&Browser,
	str::string_ &Data )
{
	status__ Status = s_Undefined;
ERRProlog
	str::string AttributeName, MacroName;
	epeios::row__ Row = NONE;
ERRBegin
	AttributeName.Init();

	Row = ExtractAttributeName_( Parameters, AttributeName );

	if ( ( Row == NONE )
		  || ( AttributeName.Amount() == 0 )
		  || ( Parameters( Row ) != ',' ) ) {
		Status = sBadAttributeDefinitionSyntax;
		ERRReturn;
	}

	MacroName.Init( Parameters );
	MacroName.Remove( MacroName.First(), *Row + 1 );

	Data.Append( AttributeName );
	Data.Append( "=\"" );
	Status = this->_HandleMacroExpand( MacroName, Browser );

	_AttributeDefinitionInProgress = true;
ERRErr
ERREnd
ERREpilog
	return Status;
}

status__ xpp::_extended_browser___::InitWithFile(
	const str::string_ &FileName,
	const str::string_ &Directory )
{
	status__ Status = s_Undefined;
ERRProlog
	STR_BUFFER___ DirectoryBuffer;
	STR_BUFFER___ MacroNameBuffer;
	FNM_BUFFER___ LocalizedFileNameBuffer;
	FNM_BUFFER___ LocationBuffer;
	const char *LocalizedFileName = NULL;
	const char *Location = NULL;
ERRBegin
	LocalizedFileName = fnm::BuildFileName( Directory.Convert( DirectoryBuffer ), FileName.Convert( MacroNameBuffer ), "", LocalizedFileNameBuffer );

	if ( _FFlow.Init( fnm::CorrectLocation( LocalizedFileNameBuffer, LocationBuffer ), fil::mReadOnly, err::hUserDefined ) != fil::sSuccess ) {
		Status = sUnableToOpenFile;
		ERRReturn;
	}

	_FFlow.EOFD( XTF_EOXT );

	_XFlow.Init( _FFlow );

	Location = fnm::GetLocation( LocalizedFileNameBuffer, LocationBuffer );

	Init( _XFlow, str::string( LocalizedFileName ), str::string( Location ) );

	_IgnorePreprocessingInstruction = true;

	Status = sOK;
ERRErr
ERREnd
ERREpilog
	return Status;
}

status__ xpp::_extended_browser___::InitWithContent(
	const str::string_ &Content,
	const str::string_ &NameOfTheCurrentFile,
	const xtf::coord__ &Coord,
	const str::string_ &Directory )
{
	status__ Status = s_Undefined;
ERRProlog
ERRBegin
	_MacroContent.Init( Content );
	_SFlow.Init( _MacroContent );
//	_SFlow.EOFD( XTF_EOXT );

	_XFlow.Init( _SFlow, Coord );

	Init( _XFlow, NameOfTheCurrentFile, _Directory );

	_IgnorePreprocessingInstruction = false;

	Status = sOK;
ERRErr
ERREnd
ERREpilog
	return Status;
}

static bso::bool__ StripHeadingSpaces_(
	xml::token__ PreviousToken,
	const xml::browser___ &Browser,
	const str::string_ &NamespaceWithSeparator )
{
	return ( PreviousToken == xml::tValue )
		   || ( ( PreviousToken == xml::tEndTag )
		      && ( BelongsToNamespace_( Browser.TagName(), NamespaceWithSeparator )
			  || ( Browser.Token() == xml::tEndTag ) ) );
}

static void StripHeadingSpaces_( str::string_ &Data )
{
	while ( ( Data.First() != NONE ) && ( isspace( Data( Data.First() ) ) ) )
		Data.Remove( Data.First() );
}

status__ xpp::_extended_browser___::Handle(
	_extended_browser___ *&Browser,
	str::string_ &Data )
{
	status__ Status = s_Undefined;
	bso::bool__ Continue = true;
	xml::token__ PreviousToken = xml::t_Undefined;
	bso::bool__ StripHeadingSpaces = false;

	Browser = NULL;

	if ( _AttributeDefinitionInProgress ) {
		Data.Append( '"' );
		_AttributeDefinitionInProgress = false;
	}

	while ( Continue ) {
		Continue = false;
		PreviousToken = _Browser.Token();
		switch ( _Browser.Browse() ) {
		case  xml::tProcessingInstruction:
			if ( _IgnorePreprocessingInstruction )
					_Browser.PurgeDumpData();
			Continue = true;
			break;
		case xml::tStartTag:
			if ( BelongsToNamespace_( _Browser.TagName(), _Directives.NamespaceWithSeparator ) )
				if ( GetDirective_( _Browser.TagName(), _Directives ) != dBloc ) {
					Status = _HandlePreprocessorDirective( _Browser.TagName(), Browser );
					
					if ( Browser == NULL )
						Continue = true;
				} else
					Continue = true;
			else
				Status = sOK;
			break;
		case xml::tAttribute:
			if ( BelongsToNamespace_( _Browser.TagName(), _Directives.NamespaceWithSeparator ) ) {
				if ( GetDirective_( _Browser.TagName(), _Directives ) != dBloc )
					ERRc();
				else {
//					Status = sUnexpectedAttribute;
//					Tous les attributs sont acceptés (et ignorés), notamment les 'xmlns:...'. Ils ne sont cependant pas reportés sur le flux de sortie.
					Status = sOK;
					_Browser.PurgeDumpData();
				}
			} else if ( BelongsToNamespace_( _Browser.AttributeName(), _Directives.NamespaceWithSeparator ) ) {
				if ( GetDirective_( _Browser.AttributeName(), _Directives ) == dAttribute ) {
					_Browser.PurgeDumpData();
					Status = _HandleAttributeDirective( _Browser.Value(), Browser, Data );
				} else
					Status = sUnknownDirective;
			} else 
				Status = sOK;
			break;
		case xml::tStartTagClosed:
			if ( BelongsToNamespace_( _Browser.TagName(), _Directives.NamespaceWithSeparator ) )
				if ( GetDirective_( _Browser.TagName(), _Directives ) != dBloc )
					ERRc();
				else
					Continue = true;
			else
				Status = sOK;
			break;
		case xml::tEndTag:
			if ( BelongsToNamespace_( _Browser.TagName(), _Directives.NamespaceWithSeparator ) )
				switch ( GetDirective_( _Browser.TagName(), _Directives ) ) {
				case dBloc:
					StripHeadingSpaces = StripHeadingSpaces_( PreviousToken, _Browser, _Directives.NamespaceWithSeparator );
					Continue = true;
					break;
				default:
					ERRc();
					break;
				}
			else {
				StripHeadingSpaces = StripHeadingSpaces_( PreviousToken, _Browser, _Directives.NamespaceWithSeparator );
				Status = sOK;
			}
			break;
		case xml::tValue:
			Status = sOK;
			break;
		case xml::tProcessed:
//			StripHeadingSpaces = StripHeadingSpaces_( PreviousToken, _Browser, _Directives.NamespaceWithSeparator );
			Status = s_Pending;
			break;
		case xml::tError:
			Status = Convert_( _Browser.Status() );
			break;
		case xml::tComment:
			_Browser.PurgeDumpData();
			Continue = true;
			break;
		default:
			ERRc();
			break;
		}

		if ( ( Status != sOK ) && ( Status != s_Undefined ) )
			Continue = false;
	}

	if ( Browser == NULL ) {
		Data.Append( _Browser.DumpData() );

		if ( StripHeadingSpaces )
			StripHeadingSpaces_( Data );
	}

	return Status;
}

void xpp::_preprocessing_iflow_functions___::_DeleteBrowsers( void )
{
	if ( _CurrentBrowser != NULL )
		delete _CurrentBrowser;

	while ( _Browsers.Amount() )
		delete _Browsers.Pop();
}

mdr::size__ xpp::_preprocessing_iflow_functions___::FWFRead(
	mdr::size__ Maximum,
	mdr::datum__ *Buffer )
{
	mdr::size__ CumulativeRed = 0;
	_extended_browser___ *Browser = NULL;
	mdr::size__ PonctualRed = Fill_( Buffer, Maximum, _Data, _Position );


	while ( ( PonctualRed != 0 ) && ( Maximum > CumulativeRed ) && ( _CurrentBrowser != NULL ) ) {
		_Data.Init();
		_Position = 0;
		CumulativeRed += PonctualRed;

		Browser = NULL;

		_Status = _Browser().Handle( Browser, _Data );

		while ( _Status == s_Pending ) {
#ifdef XPP_DBG
			if ( Browser != NULL )
				ERRc();
#endif
			delete _CurrentBrowser;
			_CurrentBrowser = NULL;

			if ( _Browsers.Amount() )
				_CurrentBrowser = _Browsers.Pop();

			_Status = _Browser().Handle( Browser, _Data );

		} 
		
		if ( _Status != sOK ) {
			*Buffer = XTF_EOXC;	// Pour provoquer une erreur.
			CumulativeRed++;
			break;
		}

		if ( Browser != NULL ) {
			_Browsers.Push( _CurrentBrowser );
			_CurrentBrowser = Browser;
		}

		PonctualRed = Fill_( Buffer, Maximum, _Data, _Position );
	}

	return CumulativeRed;
}

status__ xpp::Process(
	const str::string_ &Namespace,
	flw::iflow__ &IFlow,
	const str::string_ &Directory,
	xml::writer_ &Writer,
	xtf::coord__ &Coord,
	str::string_ &GuiltyFileName )
{
	status__ Status = sOK;
ERRProlog
	preprocessing_iflow___ PFlow;
	xtf::extended_text_iflow__ XFlow;
	xml::token__ Token = xml::t_Undefined;
	bso::bool__ Continue = true;
	xml::browser___ Browser;
ERRBegin
	PFlow.Init( IFlow, Directory, Namespace );
	XFlow.Init( PFlow );

	Browser.Init( XFlow, xml::ehKeep );

	while ( Continue ) {
		switch( Browser.Browse( xml::tfAll & ~xml::tfStartTagClosed ) ) {
		case xml::tProcessingInstruction:
			Writer.GetFlow() << Browser.DumpData();
			
			if ( Writer.GetOutfit() == xml::oIndent )
				Writer.GetFlow() << txf::nl;

			break;
		case xml::tStartTag:
			Writer.PushTag( Browser.TagName() );
			break;
		case xml::tAttribute:
			Writer.PutAttribute( Browser.AttributeName(), Browser.Value() );
			break;
		case xml::tValue:
			Writer.PutValue( Browser.Value() );
			break;
		case xml::tEndTag:
			Writer.PopTag();
			break;
		case xml::tProcessed:
			Continue = false;
			break;
		case xml::tError:
			Status = PFlow.Status();
			Coord = PFlow.Coord();
			GuiltyFileName = PFlow.LocalizedFileName();
			Continue = false;
			break;
		default:
			ERRc();
			break;
		}
	}
ERRErr
ERREnd
ERREpilog
	return Status;
}

status__ xpp::Process(
	const str::string_ &Namespace,
	flw::iflow__ &IFlow,
	const str::string_ &Directory,
	xml::outfit__ Outfit,
	txf::text_oflow__ &OFlow,
	xtf::coord__ &Coord,
	str::string_ &GuiltyFileName )
{
	status__ Status = sOK;
ERRProlog
	xml::writer Writer;
ERRBegin
	Writer.Init( OFlow, Outfit, xml::schKeep );

	Status = Process( Namespace, IFlow, Directory, Writer, Coord, GuiltyFileName );
ERRErr
ERREnd
ERREpilog
	return Status;
}

status__ xpp::Process(
	const str::string_ &Namespace,
	flw::iflow__ &IFlow,
	const str::string_ &Directory,
	xml::writer_ &Writer )
{
	status__ Status = s_Undefined;
ERRProlog
	xtf::coord__ DummyCoord;
	str::string DummyString;
ERRBegin
	DummyString.Init();

	Status = Process( Namespace, IFlow, Directory, Writer, DummyCoord, DummyString );
ERRErr
ERREnd
ERREpilog
	return Status;
}

status__ xpp::Process(
	const str::string_ &Namespace,
	flw::iflow__ &IFlow,
	const str::string_ &Directory,
	xml::outfit__ Outfit,
	txf::text_oflow__ &OFlow )
{
	status__ Status = s_Undefined;
ERRProlog
	xtf::coord__ DummyCoord;
	str::string DummyString;
ERRBegin
	DummyString.Init();

	Status = Process( Namespace, IFlow, Directory, Outfit, OFlow, DummyCoord, DummyString );
ERRErr
ERREnd
ERREpilog
	return Status;
}

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class xpppersonnalization
: public xpptutor
{
public:
	xpppersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~xpppersonnalization( void )
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

static xpppersonnalization Tutor;

ttr_tutor &XPPTutor = Tutor;
