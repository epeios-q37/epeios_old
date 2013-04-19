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
	return XML_NAME "_" #m;\
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
		ERRPrm();
		break;
	}

	return NULL;	// Pour �viter un 'warning'.
}

#undef CASE

void xml::GetMeaning(
	status__ Status,
	const coord__ &Coord,
	lcl::meaning_ &Meaning )
{
ERRProlog
	lcl::meaning MeaningBuffer;
	bso::integer_buffer__ IBuffer;
ERRBegin
	Meaning.SetValue( XML_NAME "_ErrorAtLineColumn" );

	MeaningBuffer.Init();
	MeaningBuffer.SetValue( GetLabel( Status ) );

	Meaning.AddTag( MeaningBuffer );

	Meaning.AddTag( bso::Convert( Coord.Line, IBuffer ) );
	Meaning.AddTag( bso::Convert( Coord.Column, IBuffer ) );
ERRErr
ERREnd
ERREpilog
}

static status__ SkipSpaces_( _flow___ &Flow )
{
	while ( !Flow.EndOfFlow() && isspace( Flow.View() ) )
		Flow.Get();

	if ( Flow.EndOfFlow() )
		return sUnexpectedEOF;

	return sOK;
}

static status__ HandleProcessingInstruction_( _flow___ &Flow )	// G�re aussi le prologue '<?xml ... ?>'
{
	if ( Flow.Get() != '?' )
		ERRDta();

	while ( !Flow.EndOfFlow() && ( Flow.Get() != '>' ) );

	if ( Flow.EndOfFlow() )
		return sUnexpectedEOF;

	return sOK;
}

static status__ Test_(
	_flow___ &Flow,
	const char C )
{
	if ( Flow.EndOfFlow() )
		return sUnexpectedEOF;

	if ( Flow.Get() != C )
		return sUnexpectedCharacter;

	return sOK;
}

static status__ Test_(
	_flow___ &Flow,
	const char *S )
{
	status__ Status = sOK;

	while ( ( Status == sOK ) && ( *S != 0 ) ) {
		Status = Test_( Flow, *S );
		S++;
	}

	return Status;
}

static status__ GetComment_(
	_flow___ &Flow,
	str::string_ &Content )
{
	status__ Status = s_Undefined;
	bso::bool__ Continue = true;

	if ( Flow.Get() != '-' )
		ERRDta();

	if ( ( Status = Test_( Flow, '-' ) ) != sOK )
		return Status;

	if ( Flow.EndOfFlow() )
		return sUnexpectedEOF;

	while ( Continue ) {
		while ( !Flow.EndOfFlow() && ( Flow.View() != '-' ) )
			Content.Append( Flow.Get() );
		
		if ( Flow.EndOfFlow() )
			return sUnexpectedEOF;

		Flow.Get();	// Pour passer le '-' de d�but de fin de commentaire.

		if ( Flow.EndOfFlow() )
			return sUnexpectedEOF;

		if ( Flow.View() == '-' ) {
			Flow.Get();

			if ( Flow.EndOfFlow() )
				return sUnexpectedEOF;

			if ( Flow.View() == '>' ) {
				Flow.Get();

				Continue = false;
			} else
				Content.Append( "--" );
		}
	}

	return sOK;
}

static status__ GetCData_(
	_flow___ &Flow,
	str::string_ &Content )
{
	status__ Status = s_Undefined;
	bso::bool__ Continue = true;

	if ( Flow.Get() != '[' )
		ERRDta();

	if ( ( Status = Test_( Flow, "CDATA[" ) ) != sOK )
		return Status;

	if ( Flow.EndOfFlow() )
		return sUnexpectedEOF;

	if ( Flow.EndOfFlow() )
		return sUnexpectedEOF;

	while ( Continue ) {
		while ( !Flow.EndOfFlow() && ( Flow.View() != ']' ) )
			Content.Append( Flow.Get() );

		if ( Flow.EndOfFlow() )
			return sUnexpectedEOF;

		Flow.Get();	// Pour passer le ']' de d�but de fin de 'CDATA'.

		if ( Flow.EndOfFlow() )
			return sUnexpectedEOF;

		if ( Flow.View() == ']' ) {
			Flow.Get();

			if ( Flow.EndOfFlow() )
				return sUnexpectedEOF;

			if ( Flow.View() == '>' ) {
				Flow.Get();

				Continue = false;
			} else
				Content.Append( "]]" );
		}
	}

	return sOK;
}

static status__ GetCommentOrCData_(
	_flow___ &Flow,
	str::string_ &Content,
	bso::bool__ &IsCData )
{
	bso::bool__ Continue = true;

	if ( Flow.Get() != '!' )
		ERRDta();

	if ( Flow.EndOfFlow() )
		return sUnexpectedEOF;

	if ( Flow.View() == '-' ) {
		IsCData = false;
		return GetComment_( Flow, Content );
	} else if ( Flow.View() == '[' ) {
		IsCData = true;
		return GetCData_( Flow, Content );
	}	else
		return sUnexpectedCharacter;
}

static sdr::size__ GetId_(
	_flow___ &Flow,
	str::string_ &Id )
{
	bso::size__ Size = 0;

	while ( !Flow.EndOfFlow() && ( isalnum( Flow.View() ) || Flow.View() == ':' || Flow.View() == '_' || Flow.View() == '-' ) ) {
		Id.Append( Flow.Get() );
		Size++;
	}

	return Size;
}

static inline sdr::size__ GetName_( 
	_flow___ &Flow,
	str::string_ &Name )
{
	return GetId_( Flow, Name );
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

	while ( !Flow.EndOfFlow() ) {

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
	entities_handling__ EntitiesHandling,
	str::string_ &Value,
	bso::bool__ &OnlySpaces )
{
	unsigned char C;
	OnlySpaces = true;

	while ( !Flow.EndOfFlow() && ( Flow.View() != Delimiter ) ) {

		if ( !isspace( C = Flow.Get() ) )
			OnlySpaces = false;
		else if ( !OnlySpaces && ErrorIfSpaceInValue && ( C != ' ' ) )
			return sUnexpectedCharacter;

		if ( ( C == '&' ) && ( EntitiesHandling == ehReplace ) ) {
			C = HandleEntity_( Flow );

			if ( C == ENTITY_ERROR_VALUE )
				return sUnknownEntity;
		}

		Value.Append( C );
	}

	if ( Flow.EndOfFlow() )
		return sUnexpectedEOF;

	return sOK;
}

inline status__ GetAttributeValue_(
	_flow___ &Flow,
	char Delimiter,
	entities_handling__ EntitiesHandling,
	str::string_ &Value )
{	
	bso::bool__ Dummy;

	return GetValue_( Flow, Delimiter, true, EntitiesHandling, Value, Dummy );
}

#define HANDLE( F )\
	if ( ( Status = ( F ) ) != sOK )\
		return ( Status );\
	else\
		Status = s_Undefined;

static status__ GetAttribute_(
	_flow___ &Flow,
	entities_handling__ EntitiesHandling,
	str::string_ &Name,
	str::string_ &Value )
{
	char Delimiter;
	status__ Status = s_Undefined;

	if ( GetName_( Flow, Name ) == 0 ) {
		Flow.Get();	// Pour ajuster les coordonn�es de l'erreur.
		return sUnexpectedCharacter;
	}

	HANDLE( SkipSpaces_( Flow ) );

	if ( Flow.Get() != '=' )
		return sMissingEqualSign;

	HANDLE( SkipSpaces_( Flow ) );

	Delimiter = Flow.Get();

	if ( ( Delimiter != '"' ) && ( Delimiter != '\'' ) )
		return sBadAttributeValueDelimiter;

	HANDLE( GetAttributeValue_( Flow, Delimiter, EntitiesHandling, Value ) );

	Flow.Get();	// To skip the '"' or '''.
/*
	if ( Flow.EndOfFlow() )
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
	entities_handling__ EntitiesHandling,
	bso::bool__ &OnlySpaces )
{
	return GetValue_( Flow, '<', false, EntitiesHandling, Value, OnlySpaces );
}

#define XMLNS	"xmlns"

inline static bso::bool__ IsSpecialAttribute_( const str::string_ &Attribute )
{
	bso::size__ Length = strlen( XMLNS );

	if ( Attribute.Amount() < Length )
		return false;

	if ( str::Compare( str::string( XMLNS ), Attribute, 0, 0, Length ) )
		return false;

	if ( ( Attribute.Amount() > Length ) && ( Attribute( Length ) != ':' ) )
		return false;

	return true;
}

#undef HANDLE

#define HANDLE( F )\
	if ( ( _Status = ( F ) ) != sOK )\
	{\
		_Token = t_Error;\
		ERRReturn;\
	}

#define RETURN( V )\
	{\
		_Status = V;\
		_Token = t_Error;\
		ERRReturn;\
	}

token__ xml::parser___::Parse( int TokenToReport )
{
ERRProlog
	bso::bool__ OnlySpaces = false, Continue = true, TEOX = true;	// 'TEOX' : Test EOX.
ERRBegin

	_Flow.Purge();

	while ( Continue ) {
		 TEOX = true;

		switch ( _Token ) {
		case tStartTagClosed:
			TEOX = false;
			break;
		case tEndTag:
			if ( _Tags.Amount() <= 1 )
				TEOX = false;
			break;
		default:
			break;
		}

		if ( TEOX && _Flow.EndOfFlow() )
			if ( _Token != tEndTag )
				RETURN( sUnexpectedEOF );

		switch ( _Context ) {
		case cHeaderExpected:

			HANDLE( SkipSpaces_( _Flow ) );

			switch ( _Token ) {
			case t_Undefined:
				flw::datum__ Datum[2];
				if ( _Flow.View( 2, Datum ) != 2 )	// 'EOF'.
					Continue = false;	// Fichier vide ou presque.
				else if ( Datum[0] != '<' )
					RETURN( sUnexpectedCharacter )
				else {
					if ( Datum[1] == '?' ) {
						_Flow.Get();	// Pour enlever le '<'.
						HANDLE( HandleProcessingInstruction_( _Flow ) );

						_Token = tProcessingInstruction;

						if ( ( 1 << _Token ) & TokenToReport )
							Continue = false;

					} else {
						_Context = cTagExpected;
					}
				}
				break;
			case tProcessingInstruction:
				_Token = t_Undefined;

				HANDLE( SkipSpaces_( _Flow ) );

				_Context = cTagExpected;
				break;
			default:
				ERRFwk();
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
			case tComment:
			case tCData:
			case t_Undefined:
				if ( _Flow.View() == '!' ) {
					bso::bool__ IsCData = false;

					_Value.Init();

					HANDLE( GetCommentOrCData_( _Flow, _Value, IsCData ) );
					_Context = cTagExpected;

					if ( IsCData )
						_Token = tCData;
					else
						_Token = tComment;

					if ( ( 1 << _Token ) & TokenToReport )
						Continue = false;

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
						ERRFwk();

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
					_Token = t_Processed;
					Continue = false;
				} else
					_Context = cValueExpected;
				break;
			default:
				ERRFwk();
			}
			break;
		case cAttribute:
			switch ( _Token ) {
			case t_Undefined:
				_AttributeName.Init();
				_Value.Init();

				HANDLE( GetAttribute_( _Flow, _EntitiesHandling, _AttributeName, _Value ) );

				if ( _Tags.IsEmpty() )
					ERRFwk();

				_TagName.Init();

				_Tags.Top( _TagName );

				if ( IsSpecialAttribute_( _AttributeName ) )
					_Token = tSpecialAttribute;
				else
					_Token = tAttribute;

				if ( ( 1 << _Token ) & TokenToReport )
					Continue = false;

				break;
			case tAttribute:
			case tSpecialAttribute:
				_Token = t_Undefined;

				HANDLE( SkipSpaces_( _Flow ) );

				if ( _Flow.View() == '/' ) {

					_Flow.Get();

					HANDLE( SkipSpaces_( _Flow ) );

					if ( _Flow.View() == '>' ) {

						_Flow.Get();

						if ( _Tags.IsEmpty() )
							ERRFwk();

						_TagName.Init();

						_Tags.Top( _TagName );

						_Token = tStartTagClosed;

						if ( ( 1 << _Token ) & TokenToReport )
							Continue = false;

						_EmptyTag = true;


					} else {
						_Flow.Get();	// Pour la mise � jour des coordonn�es.
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
				}/* else {
					_Flow.Get();	// Pour la mise � jour des coordonn�es.
					RETURN( sUnexpectedCharacter );
				}
*/				break;
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
					_Token = t_Processed;
					Continue = false;
				} else
					_Context = cValueExpected;
				break;
			default:
				ERRFwk();
				break;
			}
			break;
		case cClosingTag:
			switch ( _Token ) {
			case t_Undefined:
			case tValue:
			case tComment:
			case tCData:
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

				if ( ( _Token == tCData )
					 && !( ( 1 << tCData ) & TokenToReport ) )
					_Token = tValue;	// si on ne demande pas le 'CDATA' en tant que tel, on le reporte en tant que 'tValue'.

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
				ERRFwk();
				break;
			}
			break;
		case cValueExpected:
			switch( _Token ) {
			case t_Undefined:
				if ( _Flow.View() != '<' ) {
					_Value.Init();
					
					HANDLE( GetTagValue_( _Flow, _Value, _EntitiesHandling, OnlySpaces ) );

					if ( !OnlySpaces ) {

						_TagName.Init();

						if ( _Tags.IsEmpty() )
							ERRFwk();
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
				ERRFwk();
				break;
			}
			break;
		default:
			ERRFwk();
			break;
		}
	}

	if ( _Tags.IsEmpty() )
		if ( _Token == t_Undefined )
			_Token = t_Processed;

	_Status = sOK;
ERRErr
ERREnd
ERREpilog
	return _Token;
}

status__ xml::Parse(
	xtf::extended_text_iflow__ &UserFlow,
	entities_handling__ EntitiesHandling,
	callback__ &Callback )
{
	status__ Status = s_Undefined;
ERRProlog
	parser___ Parser;
	str::string TagName, AttributeName, Value;	
	bso::bool__ Stop = false;
	xml::dump Dump;
ERRBegin
	Parser.Init( UserFlow, EntitiesHandling );

	while ( !Stop ) {
		TagName.Init();
		AttributeName.Init();
		Value.Init();
		Dump.PurgeData();

		switch ( Parser.Parse( TagName, AttributeName, Value, Dump, Status ) ) {
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
		case tSpecialAttribute:
			Stop = !Callback.XMLSpecialAttribute( TagName, AttributeName, Value, Dump );
			break;
		case tValue:
		case tCData:
			Stop = !Callback.XMLValue( TagName, Value, Dump );
			break;
		case tEndTag:
			Stop = !Callback.XMLEndTag( TagName, Dump );
			break;
		case tComment:
			Stop = !Callback.XMLComment( Value, Dump );
			break;
		case t_Error:
		case t_Processed:
			Stop = true;
			break;
		default:
			ERRFwk();
			break;
		}
	}
ERRErr
ERREnd
ERREpilog
	return Status;
}

void xml::TransformUsingEntities(
	str::string_ &Target,
	bso::bool__ DelimiterOnly )
{
ERRProlog
	sdr::row__ Position = Target.First();
	bso::char__ C;
	str::string Buffer;
ERRBegin
	while( Position != E_NIL ) {
		switch ( C = Target( Position ) ) {
		case '\'':
			if ( !DelimiterOnly ) {
				Buffer.Init( "&apos;" );
				Target.Remove( Position );
				Target.Insert( Buffer, Position );
			}
			break;
		case '"':
			Buffer.Init( "&quot;" );
			Target.Remove( Position );
			Target.Insert( Buffer, Position );
			break;
		case '<':
			if ( !DelimiterOnly ) {
				Buffer.Init( "&lt;" );
				Target.Remove( Position );
				Target.Insert( Buffer, Position );
			}
			break;
		case '>':
			if ( !DelimiterOnly ) {
				Buffer.Init( "&gt;" );
				Target.Remove( Position );
				Target.Insert( Buffer, Position );
			}
			break;
		case '&':
			if ( !DelimiterOnly ) {
				Buffer.Init( "&amp;" );
				Target.Remove( Position );
				Target.Insert( Buffer, Position );
			}
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

void xml::writer_::_WriteTabs( bso::size__ Amount ) const
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

	switch ( S_.SpecialCharHandling ) {
	case schReplace:
		TransformUsingEntities( Value, false, TransformedValue );
		break;
	case schKeep:
		TransformedValue = Value;
		break;
	default:
		ERRFwk();
		break;
	}
	
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

	TransformUsingEntities( Value, S_.SpecialCharHandling == schKeep, TransformedValue );

	if ( !S_.TagNameInProgress )
		ERRFwk();

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
		ERRFwk();

	Name.Init();

	Tags.Pop( Name );

	if ( S_.TagNameInProgress )
		*S_.Flow << "/>";
	else {
		if ( !S_.TagValueInProgress && ( S_.Outfit == oIndent ) )
			_WriteTabs( Tags.Amount() );
		*S_.Flow << "</" << Name << ">";
	}

	if ( S_.Outfit == oIndent )
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
