/*
	'xml' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Requires the 'xml' header file ('xml.h').
	Copyright (C) 2004 Claude SIMON (http://zeusw.org/intl/contact.html).

	This file is part of the Epeios (http://zeusw.org/epeios/) project.

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 3
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

#define XML__COMPILATION

#include "xml.h"
#include "flx.h"

#ifdef XML__MT
#	include "mtk.h"
#endif

class xmltutor
: public ttr_tutor
{
public:
	xmltutor( void )
	: ttr_tutor( XML_NAME )
	{
#ifdef XML_DBG
		Version = XML_VERSION "\b\bD $";
#else
		Version = XML_VERSION;
#endif
		Owner = XML_OWNER;
		Date = "$Date$";
	}
	virtual ~xmltutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#include "lstctn.h"
#include "flx.h"
#include "flf.h"
#include "fnm.h"
#include "lcl.h"

using namespace xml;

#define CASE( m )\
	case s##m:\
	return #m;\
	break

const char *xml::GetLabel( status__ Status )
{
	switch( Status ) {
	CASE( OK );
	CASE( UnexpectedEOF );
	CASE( UnknownEntity );
	CASE( MissingEqualSign );
	CASE( BadAttributeValueDelimiter );
	CASE( UnexpectedCharacter );
	CASE( EmptyTagName );
	CASE( MismatchedTag );
	default:
		ERRu();
		break;
	}

	return NULL;	// Pour éviter un 'warning'.
}

#undef CASE

const str::string_ &xml::GetTranslation(
	status__ Status,
	const str::string_ &Language,
	const lcl::locales_ &Locales,
	str::string_ &Translation )
{
ERRProlog
	str::string MessageLabel;
ERRBegin
	MessageLabel.Init( "EXML_" );

	MessageLabel.Append( GetLabel( Status ) );

	Locales.GetTranslation( MessageLabel, Language, Translation );
ERRErr
ERREnd
ERREpilog
	return Translation;
}

static status__ SkipSpaces_( _flow___ &Flow )
{
	while ( !Flow.EOX() && isspace( Flow.View() ) )
		Flow.Get();

	if ( Flow.EOX() )
		return sUnexpectedEOF;

	return sOK;
}

static status__ HandleProcessingInstruction_( _flow___ &Flow )	// Gère aussi le prologue '<?xml ... ?>'
{
	if ( Flow.Get() != '?' )
		ERRc();

	while ( !Flow.EOX() && ( Flow.Get() != '>' ) );

	if ( Flow.EOX() )
		return sUnexpectedEOF;

	return sOK;
}

static status__ SkipComment_( _flow___ &Flow )
{
	bso::bool__ Continue = true;

	if ( Flow.Get() != '!' )
		ERRc();

	if ( Flow.EOX() )
		return sUnexpectedEOF;

	if ( Flow.Get() != '-' )
		return sUnexpectedCharacter;

	if ( Flow.EOX() )
		return sUnexpectedEOF;

	if ( Flow.Get() != '-' )
		return sUnexpectedCharacter;

	if ( Flow.EOX() )
		return sUnexpectedEOF;

	while ( Continue ) {
		while ( !Flow.EOX() && ( Flow.Get() != '-' ) );

		if ( Flow.EOX() )
			return sUnexpectedEOF;

		if ( Flow.View() == '-' ) {
			Flow.Get();

			if ( Flow.EOX() )
				return sUnexpectedEOF;

			if ( Flow.View() == '>' ) {
				Flow.Get();

				Continue = false;
			}
		}
	}

	return sOK;
}

static void GetId_(
	_flow___ &Flow,
	str::string_ &Id )
{
	while ( !Flow.EOX() && ( isalnum( Flow.View() ) || Flow.View() == ':' || Flow.View() == '_' ) )
		Id.Append( Flow.Get() );
}

static inline void GetName_( 
	_flow___ &Flow,
	str::string_ &Name )
{
	GetId_( Flow, Name );
}

enum entity_handling_state__ {
	ehsStart,
	ehsL,
	ehsLT,
	ehsG,
	ehsGT,
	ehsA,
	ehsAM,
	ehsAMP,
	ehsAP,
	ehsAPO,
	ehsAPOS,
	ehsQ,
	ehsQU,
	ehsQUO,
	ehsQUOT,
	ehs_amount,
	ehs_Undefined
};

#define ENTITY_ERROR_VALUE	0

static unsigned char HandleEntity_( _flow___ &Flow )
{
	entity_handling_state__ State = ehsStart;

	while ( !Flow.EOX() ) {

		switch ( State ) {
		case ehsStart:
			switch( Flow.Get() ) {
			case 'l':
				State = ehsL;
				break;
			case 'g':
				State = ehsG;
				break;
			case 'a':
				State = ehsA;
				break;
			case 'q':
				State = ehsQ;
				break;
			default:
				return ENTITY_ERROR_VALUE;
				break;
			}
			break;
		case ehsL:
			if ( Flow.Get() != 't' )
				return ENTITY_ERROR_VALUE;

			State = ehsLT;
			break;
		case ehsLT:
			if ( Flow.Get() != ';' )
				return ENTITY_ERROR_VALUE;

			return '<';
			break;
		case ehsG:
			if ( Flow.Get() != 't' )
				return ENTITY_ERROR_VALUE;

			State = ehsGT;
			break;
		case ehsGT:
			if ( Flow.Get() != ';' )
				return ENTITY_ERROR_VALUE;

			return '>';
			break;
		case ehsA:
			switch ( Flow.Get() ) {
			case 'm':
				State = ehsAM;
				break;
			case 'p':
				State = ehsAP;
				break;
			default:
				return ENTITY_ERROR_VALUE;
				break;
			}
			break;
		case ehsAM:
			if ( Flow.Get() != 'p' )
				return ENTITY_ERROR_VALUE;

			State = ehsAMP;
			break;
		case ehsAMP:
			if ( Flow.Get() != ';' )
				return ENTITY_ERROR_VALUE;

			return '&';
			break;
		case ehsAP:
			if ( Flow.Get() != 'o' )
				return ENTITY_ERROR_VALUE;

			State = ehsAPO;
			break;
		case ehsAPO:
			if ( Flow.Get() != 's' )
				return ENTITY_ERROR_VALUE;

			State = ehsAPOS;
			break;
		case ehsAPOS:
			if ( Flow.Get() != ';' )
				return ENTITY_ERROR_VALUE;

			return '\'';
			break;
		case ehsQ:
			if ( Flow.Get() != 'u' )
				return ENTITY_ERROR_VALUE;

			State = ehsQU;
			break;
		case ehsQU:
			if ( Flow.Get() != 'o' )
				return ENTITY_ERROR_VALUE;

			State = ehsQUO;
		case ehsQUO:
			if ( Flow.Get() != 't' )
				return ENTITY_ERROR_VALUE;

			State = ehsQUOT;
		case ehsQUOT:
			if ( Flow.Get() != ';' )
				return ENTITY_ERROR_VALUE;

			return '"';
			break;
		default:
				return ENTITY_ERROR_VALUE;
			break;
		}
	}

	return ENTITY_ERROR_VALUE;
}

static status__ GetValue_(
	_flow___ &Flow,
	bso::char__ Delimiter,
	bso::bool__ ErrorIfSpaceInValue,
	str::string_ &Value,
	bso::bool__ &OnlySpaces )
{
	unsigned char C;
	OnlySpaces = true;

	while ( !Flow.EOX() && ( Flow.View() != Delimiter ) ) {

		if ( !isspace( C = Flow.Get() ) )
			OnlySpaces = false;
		else if ( !OnlySpaces && ErrorIfSpaceInValue && ( C != ' ' ) )
			return sUnexpectedCharacter;

		if ( C == '&' ) {
			C = HandleEntity_( Flow );

			if ( C == ENTITY_ERROR_VALUE )
				return sUnknownEntity;
		}

		Value.Append( C );
	}

	if ( Flow.EOX() )
		return sUnexpectedEOF;

	return sOK;
}

inline status__ GetAttributeValue_(
	_flow___ &Flow,
	char Delimiter,
	str::string_ &Value )
{	
	bso::bool__ Dummy;

	return GetValue_( Flow, Delimiter, true, Value, Dummy );
}

#define HANDLE( F )\
	if ( ( Status = ( F ) ) != sOK )\
		return ( Status );\
	else\
		Status = s_Undefined;

static status__ GetAttribute_(
	_flow___ &Flow,
	str::string_ &Name,
	str::string_ &Value )
{
	char Delimiter;
	status__ Status = s_Undefined;

	GetName_( Flow, Name );

	HANDLE( SkipSpaces_( Flow ) );

	if ( Flow.Get() != '=' )
		return sMissingEqualSign;

	HANDLE( SkipSpaces_( Flow ) );

	Delimiter = Flow.Get();

	if ( ( Delimiter != '"' ) && ( Delimiter != '\'' ) )
		return sBadAttributeValueDelimiter;

	HANDLE( GetAttributeValue_( Flow, Delimiter, Value ) );

	Flow.Get();	// To skip the '"' or '''.
/*
	if ( Flow.EOX() )
		return sUnexpectedEOF;

	if ( !isspace( Flow.View() ) && ( Flow.View() != '/' ) && ( Flow.View() != '>' ) )
		return sUnexpectedCharacter;

	HANDLE( SkipSpaces_( Flow ) );
*/
	return sOK;
}

inline static status__ GetTagValue_(
	_flow___ &Flow,
	str::string_ &Value,
	bso::bool__ &OnlySpaces )
{
	return GetValue_( Flow, '<', false, Value, OnlySpaces );
}

#undef HANDLE

#define HANDLE( F )\
	if ( ( _Status = ( F ) ) != sOK )\
	{\
		_Token = tError;\
		ERRReturn;\
	}

#define RETURN( V )\
	{\
		_Status = V;\
		_Token = tError;\
		ERRReturn;\
	}

token__ xml::browser___::Browse( int TokenToReport )
{
ERRProlog
	bso::bool__ OnlySpaces = false, Continue = true, EOT = false;	// 'EOT' : End Of Tree.
ERRBegin

	_Flow.Purge();

	while ( Continue ) {
		EOT = false;

		switch ( _Token ) {
		case tStartTagClosed:
		case tEndTag:
			if ( _Tags.Amount() <= 1 )
				EOT = true;
			break;
		default:
			break;
		}

		if ( !EOT && _Flow.EOX() )
			if ( _Token != tEndTag )
				RETURN( sUnexpectedEOF );

		switch ( _Context ) {
		case cHeaderExpected:

			HANDLE( SkipSpaces_( _Flow ) );

			switch ( _Token ) {
			case t_Undefined:
				if ( _Flow.View() != '<' )
					RETURN( sUnexpectedCharacter )
				else {
					_Flow.Get();
					if ( _Flow.View() == '?' ) {
						HANDLE( HandleProcessingInstruction_( _Flow ) );

						_Token = tProcessingInstruction;

						if ( ( 1 << _Token ) & TokenToReport )
							Continue = false;

					} else {
						_Context = cTagExpected;
						_Flow.Unget( '<' );
					}
				}
				break;
			case tProcessingInstruction:
				_Token = t_Undefined;

				HANDLE( SkipSpaces_( _Flow ) );

				_Context = cTagExpected;
				break;
			default:
				ERRc();
				break;
			}
			break;
		case cTagExpected:
			if ( _Flow.Get() != '<' )
				RETURN( sUnexpectedCharacter )

			HANDLE( SkipSpaces_( _Flow ) );

			if ( _Flow.View() == '/' ) {
				_Context = cClosingTag;
				_Flow.Get();
			} else
				_Context = cOpeningTag;
			break;
		case cOpeningTag:
			switch ( _Token ) {
			case t_Undefined:
				if ( _Flow.View() == '!' ) {
					HANDLE( SkipComment_( _Flow ) );
					_Context = cTagExpected;

					HANDLE( SkipSpaces_( _Flow ) );
				} else {
					_TagName.Init();

					GetName_( _Flow, _TagName );

					if ( _TagName.Amount() == 0 )
						RETURN( sEmptyTagName );

					_Tags.Push( _TagName );

					_Token = tStartTag;

					// Pour faciliter la localisation d'une erreur.
					if ( isspace( _Flow.View() ) )
						_Flow.Get();

					if ( ( 1 << _Token ) & TokenToReport )
						Continue = false;
				}
				break;
			case tStartTag:
				_Token = t_Undefined;

				HANDLE( SkipSpaces_( _Flow ) );

				switch( _Flow.View() ) {
				case '/':
					_Flow.Get();

					HANDLE( SkipSpaces_( _Flow ) );

					if ( _Flow.Get() != '>' )
						RETURN( sUnexpectedCharacter );

					if ( _Tags.IsEmpty() )
						ERRc();

					_TagName.Init();

					_Tags.Top( _TagName );

					_Token = tStartTagClosed;

					if ( ( 1 << _Token ) & TokenToReport )
						Continue = false;

					_EmptyTag = true;

				break;
				case '>':
					_Flow.Get();

					_TagName.Init();

					_Tags.Top( _TagName );

					_Token = tStartTagClosed;

					_EmptyTag = false;

					if ( ( 1 << _Token ) & TokenToReport )
						Continue = false;

					break;
				default:
					_Context = cAttribute;
					break;
				}
				break;
			case tStartTagClosed:
				if ( _EmptyTag ) {
					_TagName.Init();

					_Tags.Top( _TagName );

					_Token = tEndTag;
	
					if ( ( 1 << _Token ) & TokenToReport )
						Continue = false;
				} else {
					_Token = t_Undefined;

					_Context = cValueExpected;
				}
				break;
			case tEndTag:
				_Token = t_Undefined;

				_TagName.Init();

				_Tags.Pop( _TagName );

				if ( _Tags.Amount() == 0 ) {
					_Token = tProcessed;
					Continue = false;
				} else
					_Context = cValueExpected;
				break;
			default:
				ERRc();
			}
			break;
		case cAttribute:
			switch ( _Token ) {
			case t_Undefined:
				_AttributeName.Init();
				_Value.Init();

				HANDLE( GetAttribute_( _Flow, _AttributeName, _Value ) );

				if ( _Tags.IsEmpty() )
					ERRc();

				_TagName.Init();

				_Tags.Top( _TagName );

				_Token = tAttribute;

				if ( ( 1 << _Token ) & TokenToReport )
					Continue = false;

				break;
			case tAttribute:
				_Token = t_Undefined;

				HANDLE( SkipSpaces_( _Flow ) );

				if ( _Flow.View() == '/' ) {

					_Flow.Get();

					HANDLE( SkipSpaces_( _Flow ) );

					if ( _Flow.View() == '>' ) {

						_Flow.Get();

						if ( _Tags.IsEmpty() )
							ERRc();

						_TagName.Init();

						_Tags.Top( _TagName );

						_Token = tStartTagClosed;

						if ( ( 1 << _Token ) & TokenToReport )
							Continue = false;

						_EmptyTag = true;


					} else {
						_Flow.Get();	// Pour la mise à jour des coordonnées.
						RETURN( sUnexpectedCharacter );
					}
				} else if ( _Flow.View() == '>' ) {
					_Flow.Get();

					_TagName.Init();

					_Tags.Top( _TagName );

					_Token = tStartTagClosed;

					if ( ( 1 << _Token ) & TokenToReport )
						Continue = false;

					_EmptyTag = false;
				} else {
					_Flow.Get();	// Pour la mise à jour des coordonnées.
					RETURN( sUnexpectedCharacter );
				}
				break;
			case tStartTagClosed:
				if ( _EmptyTag ) {
					_TagName.Init();

					_Tags.Top( _TagName );

/*					if ( !EOT )
						HANDLE( SkipSpaces_( _Flow ) );
*/
					_Token = tEndTag;

					if ( ( 1 << _Token ) & TokenToReport )
						Continue = false;
				} else {
					_Token = t_Undefined;

					_Context = cValueExpected;
				}
				break;
			case tEndTag:
				_Token = t_Undefined;

				_TagName.Init();

				_Tags.Pop( _TagName );

				if ( _Tags.Amount() == 0 ) {
					_Token = tProcessed;
					Continue = false;
				} else
					_Context = cValueExpected;
				break;
			default:
				ERRc();
				break;
			}
			break;
		case cClosingTag:
			switch ( _Token ) {
			case t_Undefined:
			case tValue:
				HANDLE( SkipSpaces_( _Flow ) );

				_TagName.Init();

				GetName_( _Flow, _TagName );

				HANDLE( SkipSpaces_( _Flow ) );

				if ( _Flow.Get() != '>' )
					RETURN( sUnexpectedCharacter );

				if ( _Tags.IsEmpty() )
					RETURN( sMismatchedTag );

				{
					str::string_ &Buffer = _Value;

					Buffer.Init();

					_Tags.Top( Buffer );

					if ( Buffer != _TagName )
						RETURN( sMismatchedTag );

					Buffer.Init();
				}

				_Token = tEndTag;

				if ( ( 1 << _Token ) & TokenToReport )
					Continue = false;

				break;
			case tEndTag:
				_Token = t_Undefined;

				_TagName.Init();

				_Tags.Pop( _TagName );

				_Context = cValueExpected;

				if ( _Tags.IsEmpty() )
					Continue = false;
				break;
			default:
				ERRc();
				break;
			}
			break;
		case cValueExpected:
			switch( _Token ) {
			case t_Undefined:
				if ( _Flow.View() != '<' ) {
					_Value.Init();
					
					HANDLE( GetTagValue_( _Flow, _Value, OnlySpaces ) );

					if ( !OnlySpaces ) {

						_TagName.Init();

						if ( _Tags.IsEmpty() )
							ERRc();
						else
							_Tags.Top( _TagName );

						_Token = tValue;

						if ( ( 1 << _Token ) & TokenToReport )
							Continue = false;
					}
				} else
					_Context = cTagExpected;
				break;
			case tValue:
				_Token  = t_Undefined;

				_Context = cTagExpected;
			break;
			default:
				ERRc();
				break;
			}
			break;
		default:
			ERRc();
			break;
		}
	}

	if ( _Tags.IsEmpty() )
		if ( _Token == t_Undefined )
			_Token = tProcessed;

	_Status = sOK;
ERRErr
ERREnd
ERREpilog
	return _Token;
}

status__ xml::Parse(
	xtf::extended_text_iflow__ &UserFlow,
	callback__ &Callback )
{
	status__ Status = s_Undefined;
ERRProlog
	browser___ Browser;
	str::string TagName, AttributeName, Value;	
	bso::bool__ Stop = false;
	xml::dump Dump;
ERRBegin
	Browser.Init( UserFlow );

	while ( !Stop ) {
		TagName.Init();
		AttributeName.Init();
		Value.Init();
		Dump.PurgeData();

		switch ( Browser.Browse( TagName, AttributeName, Value, Dump, Status ) ) {
		case tProcessingInstruction:
			Stop = !Callback.XMLProcessingInstruction( Dump );
			break;
		case tStartTag:
			Stop = !Callback.XMLStartTag( TagName, Dump );
			break;
		case tStartTagClosed:
			Stop = !Callback.XMLStartTagClosed( TagName, Dump );
			break;
		case tAttribute:
			Stop = !Callback.XMLAttribute( TagName, AttributeName, Value, Dump );
			break;
		case tValue:
			Stop = !Callback.XMLValue( TagName, Value, Dump );
			break;
		case tEndTag:
			Stop = !Callback.XMLEndTag( TagName, Dump );
			break;
		case tProcessed:
			Stop = true;
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

void xml::Transform( str::string_ &Target )
{
ERRProlog
	epeios::row__ Position = Target.First();
	bso::char__ C;
	str::string Buffer;
ERRBegin
	while( Position != NONE ) {
		switch ( C = Target( Position ) ) {
		case '\'':
			Buffer.Init( "&apos;" );
			Target.Remove( Position );
			Target.Insert( Buffer, Position );
			break;
		case '"':
			Buffer.Init( "&quot;" );
			Target.Remove( Position );
			Target.Insert( Buffer, Position );
			break;
		case '<':
			Buffer.Init( "&lt;" );
			Target.Remove( Position );
			Target.Insert( Buffer, Position );
			break;
		case '>':
			Buffer.Init( "&gt;" );
			Target.Remove( Position );
			Target.Insert( Buffer, Position );
			break;
		case '&':
			Buffer.Init( "&amp;" );
			Target.Remove( Position );
			Target.Insert( Buffer, Position );
			break;
		default:
			break;
		}

		Position = Target.Next( Position );	// Could be dangerous, but actually works.
	}
ERRErr
ERREnd
ERREpilog
}

void xml::writer_::_CloseAllTags( void )
{
	while ( Tags.Amount() != 0 )
		PopTag();
}

void xml::writer_::_WriteTabs( bso::ulong__ Amount ) const
{
	while ( Amount-- )
		*S_.Flow << txf::tab;
}

void xml::writer_::PutValue( const value_ &Value )
{
ERRProlog
	value TransformedValue;
ERRBegin
	TransformedValue.Init();

	Convert( Value, TransformedValue );


	if ( S_.TagNameInProgress ) {
		*S_.Flow << '>';
		S_.TagNameInProgress = false;
	}

	*S_.Flow << TransformedValue;

	S_.TagValueInProgress = true;
ERRErr
ERREnd
ERREpilog
}

void xml::writer_::PutAttribute(
	const name_ &Name,
	const value_ &Value )
{
ERRProlog
	value TransformedValue;
ERRBegin
	TransformedValue.Init();

	Convert( Value, TransformedValue );

	if ( !S_.TagNameInProgress )
		ERRu();

	*S_.Flow << ' ' << Name << "=\"" << TransformedValue << '"';
ERRErr
ERREnd
ERREpilog
}


void xml::writer_::PopTag( void )
{
ERRProlog
	name Name;
ERRBegin
	if ( Tags.IsEmpty() )
		ERRu();

	Name.Init();

	Tags.Pop( Name );

	if ( S_.TagNameInProgress )
		*S_.Flow << "/>";
	else {
		if ( !S_.TagValueInProgress && S_.Indent )
			_WriteTabs( Tags.Amount() );
		*S_.Flow << "</" << Name << ">";
	}

	if ( S_.Indent )
		*S_.Flow << txf::nl;

	S_.TagNameInProgress = false;
	S_.TagValueInProgress = false;
ERRErr
ERREnd
ERREpilog
}



/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class xmlpersonnalization
: public xmltutor
{
public:
	xmlpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~xmlpersonnalization( void )
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

static xmlpersonnalization Tutor;

ttr_tutor &XMLTutor = Tutor;
