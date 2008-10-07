/*
	'xml' library by Claude SIMON (csimon@epeios.org)
	Requires the 'xml' header file ('xml.h').
	Copyright (C) 2004 Claude SIMON (csimon@epeios.org).

	This file is part of the Epeios (http://epeios.org/) project.

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

#define XML__COMPILATION

#include "xml.h"
#include "stk.h"

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

using namespace xml;


const char *xml::GetLabel( status__ Status )
{
	switch( Status ) {
	case sOK:
		ERRu();
		break;
	case sUnexpectedEOF:
		return "Unexpected EOF";
		break;
	case sUnknownEntity:
		return "Unknown entity";
		break;
	case sMissingEqualSign:
		return "Missing equal sign";
		break;
	case sBadAttributeValueDelimiter:
		return "Bad attribute value delimiter";
		break;
	case sUnexpectedCharacter:
		return "Unexpected character";
		break;
	case sEmptyTagName:
		return "Empty name";
		break;
	case sMismatchedTag:
		return "Mismatched tag";
		break;
	case sUserError:
		return "User error";
		break;
	default:
		ERRu();
		break;
	}

	return NULL;	// Pour éviter un 'warning'.
}

static inline extended_status__ Convert_( status__ Status )
{
	return (extended_status__)Status;
}

const char *xml::GetLabel( extended_status__ Status )
{
	if ( Status < Convert_( s_amount ) )
		return GetLabel( (status__)Status );

	switch( Status ) {
	case xsOK:
		ERRu();
		break;
	case xsNoTagsAllowedHere:
		return "No tags allowed here";
		break;
	case xsUnexpectedTag:
		return "Unexpected tag";
		break;
	case xsUnknownTag:
		return "Unknown tag";
		break;
	case xsAttributeAlreadyDefined:
		return "Attribute already defined";
		break;
	case xsUnexpectedAttribute:
		return "Unexpected attribute";
		break;
	case xsUnknownAttribute:
		return "Unknown attribute";
		break;
	case xsMissingNameAttribute:
		return "Missing 'name' attribute";
		break;
	case xsMissingSelectAttribute:
		return "Missing 'select' attribute";
		break;
	case xsMissingValueAttribute:
		return "Missing 'value' attribute";
		break;
	case xsUnknownVariable:
		return "Unknown variable";
		break;
	case xsNoValueAllowedHere:
		return "No value allowed here";
		break;
	case xsTooManyTags:
		return "Too many tags";
		break;
	case xsUnableToOpenFile:
		return "Unable to open file";
		break;
	case xsNestingOverflow:
		return "Nesting overflow";
		break;
	default:
		ERRu();
		break;
	}

	return NULL;	// Pour éviter un 'warning'.
}

#define	EXPAND_MAX_NESTING_LEVEL	100
#define	IFEQ_MAX_NESTING_LEVEL		100

class flow {
private:
	xtf::extended_text_iflow__ *_Flow;
public:
	dump Dump;
	unsigned char Get( void )
	{
		unsigned char C = _Flow->Get();

		if ( Dump.RawData.Amount() == 0 )
			Dump.Set( _Flow->Coord() );

		Dump.RawData.Append( C );

		return C;
	}
	unsigned char View( void )
	{
		return _Flow->View();
	}
	bso::bool__ EOX( void )
	{
		return _Flow->EOX();
	}
	void Unget( unsigned char C )
	{
		_Flow->Unget( C );

		Dump.RawData.Truncate();
	}
	void Reset( void )
	{
		Dump.RawData.Init();
	}
	void Init( xtf::extended_text_iflow__ &Flow )
	{
		_Flow = &Flow;

		Dump.Init();
	}
};

typedef flow flow_;

static status__ SkipSpaces_( flow_ &Flow )
{
	while ( !Flow.EOX() && isspace( Flow.View() ) )
		Flow.Get();

	if ( Flow.EOX() )
		return sUnexpectedEOF;

	return sOK;
}

// Entitée reconnue : '&lt;', '&gt;', '&amp;', '&apos;', '&quot;'.

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

static unsigned char HandleEntity_( flow_ &Flow )
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

static void GetId_(
	flow_ &Flow,
	str::string_ &Id )
{
	while ( !Flow.EOX() && ( isalnum( Flow.View() ) || Flow.View() == ':' || Flow.View() == '_' ) )
		Id.Append( Flow.Get() );
}

static inline void GetName_( 
	flow_ &Flow,
	str::string_ &Name )
{
	GetId_( Flow, Name );
}

static status__ GetValue_(
	flow_ &Flow,
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

	return sOK;
}

inline static status__ GetTagValue_(
	flow_ &Flow,
	str::string_ &Value,
	bso::bool__ &OnlySpaces )
{
	return GetValue_( Flow, '<', false, Value, OnlySpaces );
}

inline status__ GetAttributeValue_(
	flow_ &Flow,
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
	flow_ &Flow,
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

	if ( Flow.EOX() )
		return sUnexpectedEOF;

	Flow.Get();	// To skip the '"' or '''.

	if ( Flow.EOX() )
		return sUnexpectedEOF;

	if ( !isspace( Flow.View() ) && ( Flow.View() != '/' ) && ( Flow.View() != '>' ) )
		return sUnexpectedCharacter;

	HANDLE( SkipSpaces_( Flow ) );

	return sOK;
}

static status__ SkipComment_( flow_ &Flow )
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

static status__ HandleProcessingInstruction_( flow_ &Flow )	// Gère aussi le prologue '<?xml ... ?>'
{
	if ( Flow.Get() != '?' )
		ERRc();

	while ( !Flow.EOX() && ( Flow.Get() != '>' ) );

	if ( Flow.EOX() )
		return sUnexpectedEOF;

	return sOK;
}


enum state__ {
	HeaderExpected,
	TagExpected,
	OpeningTag,
	ClosingTag,
	Attribute,
	ValueExpected,
};

using xtf::coord__;

E_ROW( srow__ );

#undef HANDLE

#define HANDLE( F )\
	if ( ( Status = ( F ) ) != sOK )\
		ERRReturn;

#define RETURN( V )\
	{\
		Status = V;\
		ERRReturn;\
	}


status__ xml::Parse(
	xtf::extended_text_iflow__ &UserFlow,
	callback__ &Callback )
{
	status__ Status = s_Undefined;
ERRProlog
	state__ State = HeaderExpected;
	str::string Name, Value, Tag;
	stk::E_XMCSTACKt( str::string_, srow__ ) Tags;
	bso::ulong__ Level = BSO_ULONG_MAX;
	flow Flow;
	bso::bool__ OnlySpaces;
ERRBegin
	Flow.Init( UserFlow );

	Tags.Init();

	HANDLE( SkipSpaces_( Flow ) );

	while ( Level ) {
		if ( Flow.EOX() )
			RETURN( sUnexpectedEOF );

		switch ( State ) {
		case HeaderExpected:
			if ( Flow.View() != '<' )
				RETURN( sUnexpectedCharacter )
			else {
				Flow.Get();
				if ( Flow.View() == '?' ) {
					HANDLE( HandleProcessingInstruction_( Flow ) );

					if ( !Callback.XMLProcessingInstruction( Flow.Dump ) )
						RETURN( sUserError );

					HANDLE( SkipSpaces_( Flow ) );

					Flow.Dump.Init();
				} else
					Flow.Unget( '<' );
			}
		case TagExpected:
			if ( Flow.Get() != '<' )
				RETURN( sUnexpectedCharacter )

			HANDLE( SkipSpaces_( Flow ) );

			if ( Flow.View() == '/' ) {
				State = ClosingTag;
				Flow.Get();
			} else
				State = OpeningTag;
			break;
		case OpeningTag:
			if ( Flow.View() == '!' ) {
				HANDLE( SkipComment_( Flow ) );
				State = TagExpected;

				HANDLE( SkipSpaces_( Flow ) );

				break;
			}

			Name.Init();

			GetName_( Flow, Name );

			Tags.Push( Name );

			if ( Name.Amount() == 0 )
				RETURN( sEmptyTagName );

			if ( !Callback.XMLStartTag( Name, Flow.Dump ) )
				RETURN( sUserError );

			Flow.Dump.Init();

			if ( Level == BSO_ULONG_MAX )
				Level = 1;
			else {
				Level++;

				if ( Level == BSO_ULONG_MAX )
					ERRl();
			}

			HANDLE( SkipSpaces_( Flow ) );

			switch( Flow.View() ) {
			case '/':
				Flow.Get();

				HANDLE( SkipSpaces_( Flow ) );

				if ( Flow.Get() != '>' )
					RETURN( sUserError );

				if ( Tags.IsEmpty() )
					ERRc();

				Tag.Init();

				Tags.Pop( Tag );

				if ( !Callback.XMLStartTagClosed( Name, Flow.Dump ) )
					RETURN( sUserError );

				if ( !Callback.XMLEndTag( Name, Flow.Dump ) )
					RETURN( sUserError );

				Flow.Dump.Init();

				Level--;

				State = ValueExpected;
				break;
			case '>':
				Flow.Get();

				if ( !Callback.XMLStartTagClosed( Name, Flow.Dump ) )
					RETURN( sUserError );

				Flow.Dump.Init();

				State = ValueExpected;
				break;
			default:
				State = Attribute;
				break;
			}
			break;
		case Attribute:
			Name.Init();
			Value.Init();

			HANDLE( GetAttribute_( Flow, Name, Value ) );

			if ( Tags.IsEmpty() )
				ERRc();

			Tag.Init();

			Tags.Top( Tag );

			if ( !Callback.XMLAttribute( Tag, Name, Value, Flow.Dump ) )
				RETURN( sUserError );

			Flow.Dump.Init();

			HANDLE( SkipSpaces_( Flow ) );

			if ( Flow.View() == '/' ) {

				Flow.Get();

				HANDLE( SkipSpaces_( Flow ) );

				if ( Flow.View() == '>' ) {

					Flow.Get();

					if ( Tags.IsEmpty() )
						ERRc();

					Tag.Init();

					Tags.Pop( Tag );

					if ( !Callback.XMLStartTagClosed( Tag, Flow.Dump ) )
						RETURN( sUserError );

					Flow.Dump.Init();

					if ( !Callback.XMLEndTag( Tag, Flow.Dump ) )
						RETURN( sUserError );

					Flow.Dump.Init();

					Level--;

					State = TagExpected;

					if ( Level ) {
						HANDLE( SkipSpaces_( Flow ) );
					}
				} else
					RETURN( sUnexpectedCharacter );
			} else if ( Flow.View() == '>' ) {
				Flow.Get();

				if ( !Callback.XMLStartTagClosed( Tag, Flow.Dump ) )
					RETURN( sUserError );

				Flow.Dump.Init();

				State = ValueExpected;
			}

			break;
		case ClosingTag:
			HANDLE( SkipSpaces_( Flow ) );

			Name.Init();

			GetName_( Flow, Name );

			HANDLE( SkipSpaces_( Flow ) );

			if ( Flow.Get() != '>' )
				RETURN( sUnexpectedCharacter );

			if ( Tags.IsEmpty() )
				RETURN( sMismatchedTag );

			Tag.Init();

			Tags.Pop( Tag );

			if ( Tag != Name )
				RETURN( sMismatchedTag );

			if ( !Callback.XMLEndTag( Tag, Flow.Dump ) )
				RETURN( sUserError );

			Flow.Dump.Init();

			Level--;

			State = ValueExpected;
			break;
		case ValueExpected:
			if ( Flow.View() != '<' ) {
				Value.Init();
				
				HANDLE( GetTagValue_( Flow, Value, OnlySpaces ) );

				if ( !OnlySpaces ) {

					Tag.Init();

					if ( Tags.IsEmpty() )
						ERRc();
					else
						Tags.Top( Tag );

					if ( !Callback.XMLValue( Tag, Value, Flow.Dump ) )
						RETURN( sUserError );

					Flow.Dump.Init();
				}
			}
			State = TagExpected;
			break;
		default:
			ERRc();
			break;
		}
	}

	if ( Tags.Amount() != 0 )
		ERRc();

	Status = sOK;
ERRErr
ERREnd
ERREpilog
	return Status;
}

E_ROW( rrow__ );	// Repository row.

class repository_
{
private:
	rrow__ _Locate( const str::string_ &Name ) const
	{
		ctn::E_CMITEMt( str::string_, rrow__ ) NamesItem;
		rrow__ Row = Names.First();

		NamesItem.Init( Names );

		while ( ( Row!= NONE ) && ( NamesItem( Row ) != Name ) )
			Row = Names.Next( Row );

		return Row;
	}
public:
	struct s {
		ctn::E_XMCONTAINERt( str::string_, rrow__ )::s Names;
		bch::E_BUNCHt( coord__, rrow__ )::s Coords;
		ctn::E_XMCONTAINERt( str::string_, rrow__ )::s Strings;
	};
	ctn::E_XMCONTAINERt_( str::string_, rrow__ ) Names;
	bch::E_BUNCHt_( coord__, rrow__ ) Coords;
	ctn::E_XMCONTAINERt_( str::string_, rrow__ ) Strings;
	repository_( s &S )
	: Names( S.Names ),
	  Coords( S.Coords ),
      Strings( S.Strings )
	{}
	void reset( bso::bool__ P = true )
	{
		Names.reset( P );
		Coords.reset( P );
		Strings.reset( P );
	}
	void plug( mmm::E_MULTIMEMORY_ &MM )
	{
		Names.plug( MM );
		Coords.plug( MM );
		Strings.plug( MM );
	}
	repository_ &operator =( const repository_ &R )
	{
		Names = R.Names;
		Coords = R.Coords;
		Strings = R.Strings;

		return *this;
	}
	void Init( void )
	{
		Names.Init();
		Coords.Init();
		Strings.Init();
	}
	bso::bool__ Store(
		const str::string_ &Name,
		coord__ Coord,
		const str::string_ &String )
	{
		if ( _Locate( Name ) != NONE )
			return false;

		rrow__ Row = Names.Append( Name );

		if ( Row != Coords.Append( Coord ) )
			ERRc();

		if ( Row != Strings.Append( String ) )
			ERRc();

		return true;
	}
	bso::bool__ Get(
		const str::string_ &Name,
		coord__ &Coord,
		str::string_ &String ) const
	{
		rrow__ Row = _Locate( Name );

		if ( Row == NONE )
			return false;

		Coord = Coords.Get( Row );

		Strings.Recall( Row, String );

		return true;
	}
};

E_AUTO( repository )

typedef stk::E_XCSTACK_( repository_ )	repository_stack_;
E_AUTO( repository_stack )

E_ROW( vrow__ );	// Variable row.

class variables
{
	vrow__ _Locate( const str::string_ &Name ) const
	{
		ctn::E_CMITEMt( str::string_, vrow__ ) NamesItem;
		vrow__ Row = Names.First();

		NamesItem.Init( Names );

		while ( ( Row != NONE ) && ( NamesItem( Row ) != Name ) )
			Row = Names.Next( Row );

		return Row;
	}
public:
	ctn::E_XMCONTAINERt( str::string_, vrow__ ) Names, Values;
	void Init( void )
	{
		Names.Init();
		Values.Init();
	}
	void Set(
		const str::string_ &Name,
		const str::string_ &Value )
	{
		vrow__ Row = _Locate( Name );

		if ( Row == NONE ) {
			Row = Names.Append( Name );
			if ( Row != Values.Append( Value ) )
				ERRc();
		} else {
			Values( Row ) = Value;
			Values.Flush();
		}
	}
	bso::bool__ Get(
		const str::string_ &Name,
		str::string_ &Value ) const
	{
		vrow__ Row = _Locate( Name );

		if ( Row != NONE ) {
			ctn::E_CMITEMt( str::string_, vrow__ ) ValuesItem;
			ValuesItem.Init( Values );

			Value = ValuesItem( Row );
		}

		return Row != NONE;
	}
	bso::bool__ Exists( const str::string_ &Name ) const
	{
		return _Locate( Name ) != NONE;
	}
	bso::bool__ IsEqual(
		const str::string_ &Name,
		const str::string_ &Value ) const
	{
		ctn::E_CMITEMt( str::string_, vrow__ ) ValuesItem;
		vrow__ Row = _Locate( Name );

		if ( Row == NONE )
			ERRu();

		ValuesItem.Init( Values );

		return ValuesItem( Row ) == Value;
	}
};

enum tag__ {
	// Balise n'appartenant pas au 'namespace'.
	tUser,
	tDefine,
	tExpand,
	tIfeq,
	tBloc,
	tSet,
	t_amount,
	// Balise appartenant au namesapce mais non reconnue !
	t_Undefined
};

class donothing_callback__
: public callback__
{
protected:
	virtual bso::bool__ XMLProcessingInstruction( const dump_ &Dump )
	{
		return true;
	}
	virtual bso::bool__ XMLStartTag(
		const str::string_ &Name,
		const dump_ &Dump )
	{
		return true;
	}
	virtual bso::bool__ XMLStartTagClosed(
		const str::string_ &Name,
		const dump_ &Dump )
	{
		return true;
	}
	virtual bso::bool__ XMLAttribute(
		const str::string_ &TagName,
		const str::string_ &Name,
		const str::string_ &Value,
		const dump_ &Dump )
	{
		return true;
	}
	virtual bso::bool__ XMLValue(
		const str::string_ &TagName,
		const str::string_ &Value,
		const dump_ &Dump )
	{
		return true;
	}
	virtual bso::bool__ XMLEndTag(
		const str::string_ &Name,
		const dump_ &Dump )
	{
		return true;
	}
public:
	void Init( void )
	{
		callback__::Init();
	}
};

class store_callback
: public callback__
{
private:
	flw::oflow__ *_Flow;
	void _Store( const str::string_ &RawData )
	{
		epeios::row__ Row = RawData.First();

		while ( Row != NONE ) {
			_Flow->Put( RawData( Row ) );

			Row = RawData.Next( Row );
		}
	}
protected:
	virtual bso::bool__ XMLProcessingInstruction( const dump_ &Dump )
	{
		return true;
	}
	virtual bso::bool__ XMLStartTag(
		const str::string_ &Name,
		const dump_ &Dump )
	{
		_Store( Dump.RawData );

		return true;
	}
	virtual bso::bool__ XMLStartTagClosed(
		const str::string_ &Name,
		const dump_ &Dump )
	{
		_Store( Dump.RawData );

		return true;
	}
	virtual bso::bool__ XMLAttribute(
		const str::string_ &TagName,
		const str::string_ &Name,
		const str::string_ &Value,
		const dump_ &Dump )
	{
		_Store( Dump.RawData );

		return true;
	}
	virtual bso::bool__ XMLValue(
		const str::string_ &TagName,
		const str::string_ &Value,
		const dump_ &Dump )
	{
		_Store( Dump.RawData );

		return true;
	}
	virtual bso::bool__ XMLEndTag(
		const str::string_ &Name,
		const dump_ &Dump )
	{
		_Store( Dump.RawData );

		return true;
	}
public:
	void Init( flw::oflow__ &Flow )
	{
		callback__::Init();
		_Flow = &Flow;
	}
};

#define NAME_ATTRIBUTE		"name"
#define SELECT_ATTRIBUTE	"select"
#define HREF_ATTRIBUTE		"href"
#define VALUE_ATTRIBUTE		"value"

#define DEFINE_TAG	"define"
#define EXPAND_TAG	"expand"
#define IFEQ_TAG	"ifeq"
#define SET_TAG		"set"
#define BLOC_TAG	"bloc"

#undef RETURN

#define RETURN( V )\
	{\
		_RelayedStatus = V;\
		return false;\
	}

class extended_callback
: public callback__
{
private:
	extended_status__ _RelayedStatus;
	callback__ *_UserCallback;
	xtf::extended_text_iflow__ *_Flow;
	variables _Variables;
	repository _GlobalRepository;
	repository_stack _RepositoryStack;
	repository _CurrentRepository;
	str::string _Namespace;
	str::string _Directory;
	str::string _DefineTag;
	str::string _ExpandTag;
	str::string _IfeqTag;
	str::string _BlocTag;
	str::string _SetTag;
	// Balise parent de 'xxx:bloc' successifs et/ou imbriqués.
	str::string _BlocPendingTag;
	// Cumul des éventuels valeur de 'xxx::bloc' sucessifs et/ou imbriqués.
	str::string _BlocPendingValue;
	// Cumul des éventuels 'dump' de 'xxx::bloc' sucessifs et/ou imbriqués.
	dump _BlocPendingDump;
	// Contient la valeur de l'attribut 'name' (balises 'set' ou 'define'.
	str::string _NameAttribute;
	// Contient la valeur de l'attribut 'name' (balises 'ifeq' ou 'expand'.
	str::string _SelectAttribute;
	// Contient la valeur de l'attribut 'value'.
	str::string _ValueAttribute;
	bso::bool__ _IsDefining;
	bso::ubyte__ _ExpandNestingLevel;	// Niveau d'imbrication de 'xxx:expand'.
	bso::ubyte__ _IfeqIgnoring;	// On est dans un 'ifeq' dont le test est négatif.
	bso::bool__ _ExpandPending;
	bso::bool__ _ExpandIsHRef;
	str::string _GuiltyFileName;	// Lors d'une erreur, fichier contenant l'erreur, si significatif.
	bso::bool__ _BelongsToNamespace( const str::string_ &TagName ) const
	{
		if ( TagName.Amount() == 0 )
			ERRc();

		return str::Compare( TagName, _Namespace, TagName.First(), _Namespace.First(), _Namespace.Amount() ) == 0;
	}
	extended_status__ _Ignore( xtf::extended_text_iflow__ &Flow )
	{
		donothing_callback__ Callback;

		Callback.Init();

		return Convert_( xml::Parse( Flow, Callback ) );
	}
	extended_status__ _HandleDefine( 
		xtf::extended_text_iflow__ &Flow,
		const str::string_ &Name )
	{
		extended_status__ Status = xs_Undefined;
	ERRProlog
		xtf::coord__ Coord;
		store_callback Callback;
		str::string String;
		flx::E_STRING_OFLOW___ SFlow;
	ERRBegin
		Coord = Flow.Coord();

		String.Init();

		SFlow.Init( String );

		Callback.Init( SFlow );

		if ( ( Status = Convert_( Parse( Flow, Callback ) ) ) != xsOK )
			ERRReturn;

		SFlow.Synchronize();

		if ( _ExpandPending )
			_CurrentRepository.Store(Name, Coord, String );
		else
			_GlobalRepository.Store(Name, Coord, String );
	ERRErr
	ERREnd
	ERREpilog
		return Status;
	}
	extended_status__ _HandleMacroExpand( void )
	{
		extended_status__ Status = xs_Undefined;
	ERRProlog
		str::string String;
		flx::E_STRING_IFLOW__ SFlow;
		xtf::extended_text_iflow__ XFlow;
		coord__ Coord;
		xtf::extended_text_iflow__ *PreviousFlow = _Flow;
		repository Repository;
		stk::row__ Row = NONE;
		bso::bool__ Found = false;
	ERRBegin
		String.Init();

		Repository.Init();

		Repository = _CurrentRepository;

		Row = _RepositoryStack.Last();

		while ( !( Found = Repository.Get( _SelectAttribute, Coord, String ) ) && ( Row != NONE ) ) {
			_RepositoryStack.Recall( Row, Repository );

			Row = _RepositoryStack.Previous( Row );
		}

		if ( !Found )
			if ( !_GlobalRepository.Get( _SelectAttribute, Coord, String ) )
				ERRReturn;

		SFlow.Init( String );
		SFlow.EOFD( XTF_EOXT );	// Normalement inutile (la conformité au format XML à déjà été traité), mais aide au déboguage.


		XFlow.Init( SFlow, Coord );

		_Flow = &XFlow;

		if ( _ExpandNestingLevel++ == EXPAND_MAX_NESTING_LEVEL )
			ERRReturn;

		Status = Convert_( Parse( XFlow, *this ) );

		if ( _ExpandNestingLevel-- == 0 )
			ERRc();

		_RepositoryStack.Pop( _CurrentRepository );

		_Flow = PreviousFlow;

		if ( Status != xsOK ) {
			_Flow->Set( XFlow.Coord() );	// Pour que l'utilisateur puisse récupèrer la position de l'erreur.
			if ( Status == Convert_( sUserError ) )
				Status = _RelayedStatus;
		}
	ERRErr
	ERREnd
	ERREpilog
		return Status;
	}
	extended_status__ _HandleFileExpand( void )
	{
		extended_status__ Status = xs_Undefined;
	ERRProlog
		str::string String;
		flf::file_iflow___ Flow;
		xtf::extended_text_iflow__ XFlow;
		xtf::extended_text_iflow__ *PreviousFlow = _Flow;
		tol::E_FPOINTER___( char ) DirectoryBuffer;
		tol::E_FPOINTER___( char ) AttributeBuffer;
		FNM_BUFFER___ FileNameBuffer;
		FNM_BUFFER___ LocationBuffer;
		const char *FileName = NULL;
		const char *Location = NULL;
	ERRBegin
		_ExpandIsHRef = false;

		FileName =  fnm::BuildFileName( DirectoryBuffer = _Directory.Convert(), AttributeBuffer = _SelectAttribute.Convert(), "", FileNameBuffer );

		if ( Flow.Init( FileName, fil::mReadOnly, err::hSkip ) != fil::sSuccess ) {
			Status = xsUnableToOpenFile;
			ERRReturn;
		}

		Flow.EOFD( XTF_EOXT );

		XFlow.Init( Flow );

		_Flow = &XFlow;

		if ( _ExpandNestingLevel++ == EXPAND_MAX_NESTING_LEVEL ) {
			Status = xsNestingOverflow;
			ERRReturn;
		}

		Location = fnm::GetLocation( FileName, LocationBuffer );

		_Directory = Location;

		Status = Convert_( Parse( XFlow, *this ) );

		if ( _ExpandNestingLevel-- == 0 )
			ERRc();

		_Flow = PreviousFlow;

		if ( Status != xsOK ) {
			_Flow->Set( XFlow.Coord() );	// Pour que l'utilisateur puisse récupèrer la position de l'erreur.
			if ( Status == Convert_( sUserError ) )
				Status = _RelayedStatus;
		}

		_Directory = DirectoryBuffer;
	ERRErr
	ERREnd
		if ( Status != xsOK )
			if ( _GuiltyFileName.Amount() == 0 )
				if ( ( Status != xsUnableToOpenFile ) || ( _ExpandIsHRef ) )
					_GuiltyFileName = FileName;
				else
					_ExpandIsHRef = true;
	ERREpilog
		return Status;
	}
	extended_status__ _HandleExpand( void )
	{
		if ( _ExpandIsHRef )
			return _HandleFileExpand();
		else
			return _HandleMacroExpand();
	}
protected:
	tag__ _GetTag( const str::string_ &Name )
	{
		if ( !_BelongsToNamespace( Name ) )
			return tUser;
		else if ( _DefineTag == Name )
			return tDefine;
		else if ( _ExpandTag == Name )
			return tExpand;
		else if ( _IfeqTag == Name )
			return tIfeq;
		else if ( _BlocTag == Name )
			return tBloc;
		else if ( _SetTag == Name )
			return tSet;

		return t_Undefined;
	}
	virtual bso::bool__ XMLProcessingInstruction( const dump_ &Dump )
	{
		if ( _ExpandNestingLevel )
			return true;
		else
			return _UserCallback->XMLProcessingInstruction( Dump );
	}
	virtual bso::bool__ XMLStartTag(
		const str::string_ &Name,
		const dump_ &Dump )
	{
		if ( _IsDefining )
			RETURN( xsTooManyTags );

		if ( _IfeqIgnoring )
			RETURN( xsTooManyTags );

		if ( _ExpandPending ) {
			if ( _ExpandIsHRef ) {
				_ExpandIsHRef = false;
				RETURN( xsNoTagsAllowedHere )
			} else if ( _GetTag( Name ) == t_Undefined )
				RETURN( xsUnknownTag )
			else if ( _GetTag( Name ) != tDefine )
				RETURN( xsUnexpectedTag )
		}


		if ( !_BelongsToNamespace( Name ) )
			_BlocPendingTag = Name;

		switch ( _GetTag( Name ) ) {
		case tUser:
			return _UserCallback->XMLStartTag( Name, Dump );
			break;
		case tDefine:
			_NameAttribute.Init();
			break;
		case tExpand:
			_SelectAttribute.Init();
			_ExpandIsHRef = false;
			break;
		case tIfeq:
			_SelectAttribute.Init();
			_ValueAttribute.Init();
			break;
		case tBloc:
			break;
		case tSet:
			_NameAttribute.Init();
			_ValueAttribute.Init();
			break;
		case t_Undefined:
			RETURN( xsUnknownTag )
			break;
		default:
			ERRc();
			break;
		}

		return true;
	}
	virtual bso::bool__ XMLAttribute(
		const str::string_ &TagName,
		const str::string_ &Name,
		const str::string_ &Value,
		const dump_ &Dump )
	{
		switch ( _GetTag( TagName ) ) {
		case tUser:
			return _UserCallback->XMLAttribute( TagName, Name, Value, Dump );
			break;
		case tDefine:
			if ( Name == NAME_ATTRIBUTE ) {
				if ( _NameAttribute.Amount() != 0 )
					RETURN( xsAttributeAlreadyDefined )

				_NameAttribute = Value;
			} else
				RETURN( xsUnknownAttribute )
			break;
		case tExpand:
			if ( Name == SELECT_ATTRIBUTE ) {
				if ( _SelectAttribute.Amount() != 0 )
					if ( _ExpandIsHRef )
						RETURN( xsUnexpectedAttribute )
					else
						RETURN( xsAttributeAlreadyDefined )

				_ExpandIsHRef = false;

			} else if ( Name == HREF_ATTRIBUTE ) {
				if ( _SelectAttribute.Amount() != 0 )
					if ( _ExpandIsHRef )
						RETURN( xsAttributeAlreadyDefined )
					else
						RETURN( xsUnexpectedAttribute )

				_ExpandIsHRef = true;

			} else
				RETURN( xsUnknownAttribute )

			_SelectAttribute = Value;

			break;
		case tIfeq:
			if ( Name == SELECT_ATTRIBUTE ) {
				if ( _SelectAttribute.Amount() != 0 )
					RETURN( xsAttributeAlreadyDefined )

				_SelectAttribute = Value;
			} else if ( Name == VALUE_ATTRIBUTE ) {
				if ( _ValueAttribute.Amount() != 0 )
					RETURN( xsAttributeAlreadyDefined )

				_ValueAttribute = Value;
			} else
				RETURN( xsUnknownAttribute )
			break;
		case tBloc:
			//RETURN( xsUnexpectedAttribute )
			// Tous les attributs sont acceptés (et ignorés), notamment les 'xmlns:...'.
			break;
		case tSet:
			if ( Name == NAME_ATTRIBUTE ) {
				if ( _NameAttribute.Amount() != 0 )
					RETURN( xsAttributeAlreadyDefined )

				_NameAttribute = Value;
			} else
			if ( Name == VALUE_ATTRIBUTE ) {
				if ( _ValueAttribute.Amount() != 0 )
					RETURN( xsAttributeAlreadyDefined )

				_ValueAttribute = Value;
			} else
				RETURN( xsUnknownAttribute )
			break;
		case t_Undefined:
			RETURN( xsUnknownTag )
			break;
		default:
			ERRc();
			break;
		}

		return true;
	}
	virtual bso::bool__ XMLStartTagClosed(
		const str::string_ &Name,
		const dump_ &Dump )
	{
		switch ( _GetTag( Name ) ) {
		case tUser:
			return _UserCallback->XMLStartTagClosed( Name, Dump );
			break;
		case tDefine:
		{
			extended_status__ Status = xs_Undefined;

			if ( _NameAttribute.Amount() == 0 )
				RETURN( xsMissingNameAttribute )

			if ( ( Status = _HandleDefine( *_Flow, _NameAttribute ) ) != xsOK )
				RETURN( Status )

			_IsDefining = true;

			break;
		}
		case tExpand:
			if ( _SelectAttribute.Amount() == 0 )
				RETURN( xsMissingSelectAttribute )

			_RepositoryStack.Push( _CurrentRepository );
			_CurrentRepository.Init();
			_ExpandPending = true;
			break;
		case tIfeq:
			if ( _SelectAttribute.Amount() == 0 )
				RETURN( xsMissingSelectAttribute )

			if ( _ValueAttribute.Amount() == 0 )
				RETURN( xsMissingValueAttribute )

			if ( !_Variables.Exists( _SelectAttribute ) )
				RETURN( xsUnknownVariable );

			if ( !_Variables.IsEqual( _SelectAttribute, _ValueAttribute ) ) {
				extended_status__ Status = xs_Undefined;
				_IfeqIgnoring = true;

				if ( ( Status = _Ignore( *_Flow ) ) != xsOK )
					RETURN( Status )
			}

			break;
		case tBloc:
			break;
		case tSet:
			if ( _NameAttribute.Amount() == 0 )
				RETURN( xsMissingNameAttribute );

			break;
		case t_Undefined:
			RETURN( xsUnexpectedTag );
			break;
		default:
			ERRc();
			break;
		}

		return true;
	}
	virtual bso::bool__ XMLValue(
		const str::string_ &TagName,
		const str::string_ &Value,
		const dump_ &Dump )
	{
		switch ( _GetTag( TagName ) ) {
		case tUser:
			return _UserCallback->XMLValue( TagName, Value, Dump );
			break;
		case tDefine:
			RETURN( xsNoValueAllowedHere )
			break;
		case tExpand:
			RETURN( xsNoValueAllowedHere )
			break;
		case tIfeq:
			RETURN( xsNoValueAllowedHere )
			break;
		case tBloc:
			_BlocPendingValue.Append( Value );
			_BlocPendingDump.Append( Dump );
			break;
		case tSet:
			RETURN( xsNoValueAllowedHere )
			break;
		case t_Undefined:
			ERRc();
			break;
		default:
			ERRc();
			break;
		}

		return true;
	}
	virtual bso::bool__ XMLEndTag(
		const str::string_ &Name,
		const dump_ &Dump )
	{
		if ( !_BelongsToNamespace( Name ) && ( _BlocPendingValue.Amount() != 0 ) ) {
			_UserCallback->XMLValue( _BlocPendingTag, _BlocPendingValue, _BlocPendingDump );

			_BlocPendingTag.Init();
			_BlocPendingValue.Init();
			_BlocPendingDump.Reset();
		}

		switch ( _GetTag( Name ) ) {
		case tUser:
			return _UserCallback->XMLEndTag( Name, Dump );
			break;
		case tDefine:
			_NameAttribute.Init();

			_IsDefining = false;
			break;
		case tExpand:
		{
			extended_status__ Status = xs_Undefined;

			if ( _SelectAttribute.Amount() == 0 )
				ERRc();

			_ExpandPending = false;

			if ( ( Status = _HandleExpand() ) != xsOK )
				RETURN( Status )

			_SelectAttribute.Init();
			_ExpandIsHRef = false;
			break;
		}
		case tIfeq:
			_IfeqIgnoring = false;
			_SelectAttribute.Init();
			_ValueAttribute.Init();
			break;
		case tBloc:
			break;
		case tSet:
			if ( _NameAttribute.Amount() == 0 )
				ERRc();

			if ( _Variables.Exists( _NameAttribute ) )
				RETURN( xsUnknownVariable )

			_Variables.Set( _NameAttribute, _ValueAttribute );

			_NameAttribute.Init();
			_ValueAttribute.Init();
			break;
		case t_Undefined:
			ERRc();
			break;
		default:
			ERRc();
			break;
		}

		return true;
	}
	public:
		void Init(
			const str::string_ &Namespace,
			const str::string_ &Directory,
			xtf::extended_text_iflow__ &Flow,
			callback__ &UserCallback )
		{
			_RelayedStatus = xs_Undefined;
			_UserCallback = &UserCallback;
			_Flow = &Flow;
			_Variables.Init();
			_GlobalRepository.Init();
			_RepositoryStack.Init();
			_CurrentRepository.Init();
			_Directory.Init( Directory );

			_Namespace.Init( Namespace );
			_Namespace.Append( ":" );

			_DefineTag.Init( _Namespace );
			_DefineTag.Append( DEFINE_TAG );

			_ExpandTag.Init( _Namespace );
			_ExpandTag.Append( EXPAND_TAG );

			_IfeqTag.Init( _Namespace );
			_IfeqTag.Append( IFEQ_TAG );

			_SetTag.Init( _Namespace );
			_SetTag.Append( SET_TAG );

			_BlocTag.Init( _Namespace );
			_BlocTag.Append( BLOC_TAG );

			_SelectAttribute.Init();
			_NameAttribute.Init();
			_ValueAttribute.Init();

			_BlocPendingTag.Init();
			_BlocPendingValue.Init();
			_BlocPendingDump.Init();

			_IsDefining = false;
			_ExpandNestingLevel = 0;
			_IfeqIgnoring = false;
			_ExpandPending = false;
			_ExpandIsHRef = false;

			_GuiltyFileName.Init();
		}
		E_RODISCLOSE__( extended_status__, RelayedStatus );
		E_RODISCLOSE__( str::string_, GuiltyFileName );

};

extended_status__ xml::ExtendedParse(
	xtf::extended_text_iflow__ &Flow,
	const str::string_ &Namespace,
	callback__ &Callback,
	const str::string_ &Directory,
	str::string_ &FileName )
{
	extended_status__ Status = xs_Undefined;
ERRProlog
	extended_callback XCallback;
ERRBegin
	XCallback.Init( Namespace, Directory, Flow, Callback );

	Status = Convert_( Parse( Flow, XCallback ) );

	if ( Status != xsOK ) {
		FileName = XCallback.GuiltyFileName();

		if ( Status == Convert_( sUserError ) )
			Status = XCallback.RelayedStatus();
	}
ERRErr
ERREnd
ERREpilog
	return Status;
}

class neutral_callback
: public callback__
{
private:
	xml::writer _Writer;
protected:
	virtual bso::bool__ XMLProcessingInstruction( const dump_ &Dump )
	{
		_Writer.GetFlow() << Dump.RawData;
		
		if ( _Writer.Indent() )
			_Writer.GetFlow() << txf::nl;

		return true;
	}
	virtual bso::bool__ XMLStartTag(
		const str::string_ &Name,
		const dump_ &Dump )
	{
		_Writer.PushTag( Name );

		return true;
	}
	virtual bso::bool__ XMLStartTagClosed(
		const str::string_ &Name,
		const str::string_ &Dump )
	{
		return true;
	}
	virtual bso::bool__ XMLAttribute(
		const str::string_ &TagName,
		const str::string_ &Name,
		const str::string_ &Value,
		const dump_ &Dump )
	{
		_Writer.PutAttribute( Name, Value );

		return true;
	}
	virtual bso::bool__ XMLValue(
		const str::string_ &TagName,
		const str::string_ &Value,
		const dump_ &Dump )
	{
		_Writer.PutValue( Value );

		return true;
	}
	virtual bso::bool__ XMLEndTag(
		const str::string_ &Name,
		const dump_ &Dump )
	{
		_Writer.PopTag();

		return true;
	}
public:
	void Init(
		txf::text_oflow__ &Flow,
		bso::bool__ Indent )
	{
		_Writer.Init( Flow, Indent );
	}
};


extended_status__ xml::Normalize(
	xtf::extended_text_iflow__ &IFlow,
	const str::string_ &Namespace,
	bso::bool__ Indent,
	const str::string_ &Directory,
	txf::text_oflow__ &OFlow,
	str::string_ &GuiltyFileName )
{
	extended_status__ Status = xs_Undefined;
ERRProlog
	neutral_callback NCallback;
ERRBegin
	NCallback.Init( OFlow, Indent );

	Status = ExtendedParse( IFlow, Namespace, NCallback, Directory, GuiltyFileName );
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
