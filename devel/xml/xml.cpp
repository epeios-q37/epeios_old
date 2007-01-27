/*
	'xml' library by Claude SIMON (csimon@epeios.org)
	Requires the 'xml' header file ('xml.h').
	Copyright (C) $COPYRIGHT_DATES$Claude SIMON (csimon@epeios.org).
$_RAW_$
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

using namespace xml;

class flow {
private:
	xtf::extended_text_iflow__ *_Flow;
public:
	str::string Dump;
	unsigned char Get( void )
	{
		unsigned char C = _Flow->Get();

		Dump.Append( C );

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

		Dump.Truncate();
	}
	void Init( xtf::extended_text_iflow__ &Flow )
	{
		_Flow = &Flow;

		Dump.Init();
	}
};

typedef flow flow_;

static void SkipSpaces_(
	flow_ &Flow,
	bso::bool__ ErrorIfEOF = true )
{
	while ( !Flow.EOX() && isspace( Flow.View() ) )
		Flow.Get();

	if ( Flow.EOX() && ErrorIfEOF )
		ERRI( iBeam );
}

// Entitée reconnue : '&lt;', '&gt;', '&amp;', '&apos;', '&quot;'.

enum entity_state__ {
	sStart,
	sL,
	sLT,
	sG,
	sGT,
	sA,
	sAM,
	sAMP,
	sAP,
	sAPO,
	sAPOS,
	sQ,
	sQU,
	sQUO,
	sQUOT,
	s_amount,
	s_Undefined
};

static unsigned char HandleEntity_( flow_ &Flow )
{
	entity_state__ State = sStart;

	while ( !Flow.EOX() ) {

		switch ( State ) {
		case sStart:
			switch( Flow.Get() ) {
			case 'l':
				State = sL;
				break;
			case 'g':
				State = sG;
				break;
			case 'a':
				State = sA;
				break;
			case 'q':
				State = sQ;
				break;
			default:
				ERRI( iBeam );
				break;
			}
			break;
		case sL:
			if ( Flow.Get() != 't' )
				ERRI( iBeam );

			State = sLT;
			break;
		case sLT:
			if ( Flow.Get() != ';' )
				ERRI( iBeam );

			return '<';
			break;
		case sG:
			if ( Flow.Get() != 't' )
				ERRI( iBeam );

			State = sGT;
			break;
		case sGT:
			if ( Flow.Get() != ';' )
				ERRI( iBeam );

			return '>';
			break;
		case sA:
			switch ( Flow.Get() ) {
			case 'm':
				State = sAM;
				break;
			case 'p':
				State = sAP;
				break;
			default:
				ERRI( iBeam );
				break;
			}
			break;
		case sAM:
			if ( Flow.Get() != 'p' )
				ERRI( iBeam );

			State = sAMP;
			break;
		case sAMP:
			if ( Flow.Get() != ';' )
				ERRI( iBeam );

			return '&';
			break;
		case sAP:
			if ( Flow.Get() != 'o' )
				ERRI( iBeam );

			State = sAPO;
			break;
		case sAPO:
			if ( Flow.Get() != 's' )
				ERRI( iBeam );

			State = sAPOS;
			break;
		case sAPOS:
			if ( Flow.Get() != ';' )
				ERRI( iBeam );

			return '\'';
			break;
		case sQ:
			if ( Flow.Get() != 'u' )
				ERRI( iBeam );

			State = sQU;
			break;
		case sQU:
			if ( Flow.Get() != 'o' )
				ERRI( iBeam );

			State = sQUO;
		case sQUO:
			if ( Flow.Get() != 't' )
				ERRI( iBeam );

			State = sQUOT;
		case sQUOT:
			if ( Flow.Get() != ';' )
				ERRI( iBeam );

			return '"';
			break;
		default:
			ERRI( iBeam );
			break;
		}
	}

	ERRI( iBeam );

	return 0;	// Juste pour éviter un 'Warning'.
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

static bso::bool__ GetValue_(
	flow_ &Flow,
	bso::char__ Delimiter,
	str::string_ &Value )
{
	unsigned char C;
	bso::bool__ IsSpace = true;

	while ( !Flow.EOX() && ( Flow.View() != Delimiter ) ) {

		if ( !isspace( C = Flow.Get() ) )
			IsSpace = false;

		if ( C == '&' )
			Value.Append( HandleEntity_( Flow ) );
		else
			Value.Append( C );
	}

	return !IsSpace;
}

inline static bso::bool__ GetTagValue_(
	flow_ &Flow,
	str::string_ &Value )
{
	return GetValue_( Flow, '<', Value );
}

inline static void GetAttributeValue_(
	flow_ &Flow,
	str::string_ &Value )
{
	GetValue_( Flow, '"', Value );
}

static void GetAttribute_(
	flow_ &Flow,
	str::string_ &Name,
	str::string_ &Value )
{
	GetName_( Flow, Name );

	SkipSpaces_( Flow );

	if ( Flow.Get() != '=' )
		ERRI( iBeam );

	SkipSpaces_( Flow );

	if ( Flow.Get() != '"' )
		ERRI( iBeam );

	GetAttributeValue_( Flow, Value );

	Flow.Get();	// To skip the '"'.

	if ( Flow.EOX() )
		ERRI( iBeam );

	if ( !isspace( Flow.View() ) && ( Flow.View() != '/' ) && ( Flow.View() != '>' ) )
		ERRI( iBeam );

	SkipSpaces_( Flow );
}

static void SkipComment_( flow_ &Flow )
{
	bso::bool__ Continue = true;

	if ( Flow.Get() != '!' )
		ERRI( iBeam );

	if ( Flow.EOX() )
		ERRI( iBeam );

	if ( Flow.Get() != '-' )
		ERRI( iBeam );

	if ( Flow.EOX() )
		ERRI( iBeam );

	if ( Flow.Get() != '-' )
		ERRI( iBeam );

	if ( Flow.EOX() )
		ERRI( iBeam );

	while ( Continue ) {
		while ( !Flow.EOX() && ( Flow.Get() != '-' ) );

		if ( Flow.EOX() )
			ERRI( iBeam );

		if ( Flow.View() == '-' ) {
			Flow.Get();

			if ( Flow.EOX() )
				ERRI( iBeam );

			if ( Flow.View() == '>' ) {
				Flow.Get();

				Continue = false;
			}
		}
	}
}

static void SkipHeader_( flow_ &Flow )
{
	if ( Flow.Get() != '?' )
		ERRI( iBeam );

	while ( !Flow.EOX() && ( Flow.Get() != '>' ) );

	if ( Flow.EOX() )
		ERRI( iBeam );
}


enum state__ {
	HeaderExpected,
	TagExpected,
	OpeningTag,
	ClosingTag,
	Attribute,
	ValueExpected,
};

using xtf::location__;

E_ROW( srow__ );

bso::bool__ xml::Parse(
	xtf::extended_text_iflow__ &UserFlow,
	callback__ &Callback )
{
	bso::bool__ Success = true;
ERRProlog
	state__ State = HeaderExpected;
	str::string Name, Value, Tag;
	stk::E_XMCSTACKt( str::string_, srow__ ) Tags;
	bso::ulong__ Level = BSO_ULONG_MAX;
	flow Flow;
ERRBegin
	Flow.Init( UserFlow );

	Tags.Init();

	SkipSpaces_( Flow );

	while ( Level ) {
		if ( Flow.EOX() )
			ERRI( iBeam );

		switch ( State ) {
		case HeaderExpected:
			if ( Flow.View() != '<' )
				ERRI( iBeam );
			else {
				Flow.Get();
				if ( Flow.View() == '?' ) {
					SkipHeader_( Flow );
					SkipSpaces_( Flow );
				} else
					Flow.Unget( '<' );
			}
		case TagExpected:
			if ( Flow.Get() != '<' )
				ERRI( iBeam );

			SkipSpaces_( Flow );

			if ( Flow.View() == '/' ) {
				State = ClosingTag;
				Flow.Get();
			} else
				State = OpeningTag;
			break;
		case OpeningTag:
			if ( Flow.View() == '!' ) {
				SkipComment_( Flow );
				State = TagExpected;
				SkipSpaces_( Flow );
				break;
			}

			Name.Init();

			GetName_( Flow, Name );

			Tags.Push( Name );

			if ( !Callback.XMLStartTag( Name, Flow.Dump ) )
				ERRI( iBeam );

			Flow.Dump.Init();

			if ( Level == BSO_ULONG_MAX )
				Level = 1;
			else {
				Level++;

				if ( Level == BSO_ULONG_MAX )
					ERRl();
			}


			SkipSpaces_( Flow );

			switch( Flow.View() ) {
			case '/':
				Flow.Get();
				SkipSpaces_( Flow );

				if ( Flow.Get() != '>' )
					ERRI( iBeam );

				if ( Tags.IsEmpty() )
					ERRI( iBeam );

				Tag.Init();

				Tags.Pop( Tag );

				if ( !Callback.XMLEndTag( Name, Flow.Dump ) )
					ERRI( iBeam );

				Flow.Dump.Init();

				Level--;

				State = ValueExpected;
				break;
			case '>':
				Flow.Get();
				if ( !Callback.XMLStartTagClosed( Name, Flow.Dump ) )
					ERRI( iBeam );

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

			GetAttribute_( Flow, Name, Value );

			if ( Tags.IsEmpty() )
				ERRI( iBeam );

			Tag.Init();

			Tags.Top( Tag );

			if ( !Callback.XMLAttribute( Tag, Name, Value, Flow.Dump ) )
					ERRI( iBeam );

			Flow.Dump.Init();

			SkipSpaces_( Flow );

			if ( Flow.View() == '/' ) {

				Flow.Get();

				SkipSpaces_( Flow );

				if ( Flow.View() == '>' ) {

					Flow.Get();

					if ( Tags.IsEmpty() )
						ERRI( iBeam );

					Tag.Init();

					Tags.Pop( Tag );

					if ( !Callback.XMLStartTagClosed( Tag, Flow.Dump ) )
						ERRI( iBeam );

					Flow.Dump.Init();

					if ( !Callback.XMLEndTag( Tag, Flow.Dump ) )
						ERRI( iBeam );

					Flow.Dump.Init();

					Level--;

					State = TagExpected;

					if ( Level )
						SkipSpaces_( Flow );
				} else
					ERRI( iBeam );
			} else if ( Flow.View() == '>' ) {
				Flow.Get();

				if ( !Callback.XMLStartTagClosed( Tag, Flow.Dump ) )
					ERRI( iBeam );

				Flow.Dump.Init();

				State = ValueExpected;
			}

			break;
		case ClosingTag:
			SkipSpaces_( Flow );

			Name.Init();

			GetName_( Flow, Name );

			SkipSpaces_( Flow );

			if ( Flow.Get() != '>' )
				ERRI( iBeam );

			if ( Tags.IsEmpty() )
				ERRI( iBeam );

			Tag.Init();

			Tags.Pop( Tag );

			if ( Tag != Name )
				ERRI( iBeam );

			if ( !Callback.XMLEndTag( Tag, Flow.Dump ) )
				ERRI( iBeam );

			Flow.Dump.Init();

			Level--;

			State = ValueExpected;
			break;
		case ValueExpected:
			if ( Flow.View() != '<' ) {
				Value.Init();
				if ( GetTagValue_( Flow, Value ) ) {

					Tag.Init();

					if ( Tags.IsEmpty() )
							ERRI( iBeam );
					else
						Tags.Top( Tag );

					if ( !Callback.XMLValue( Tag, Value, Flow.Dump ) )
						ERRI( iBeam );

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
		ERRI( iBeam );
ERRErr
	if ( ( ERRMajor == err::itn ) && ( ERRMinor == err::iBeam ) ) {
		Success = false;
		ERRRst();
	}
ERREnd
ERREpilog
	return Success;
}

struct position__
{
	xtf::location__ Line, Column;
	position__(
		xtf::location__ Line,
		xtf::location__ Column )
	{
		this->Line = Line;
		this->Column = Column;
	}
};

E_ROW( rrow__ );	// Repository row.

class repository
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
	ctn::E_XMCONTAINERt( str::string_, rrow__ ) Names;
	bch::E_BUNCHt( position__, rrow__ ) Positions;
	ctn::E_XMCONTAINERt( str::string_, rrow__ ) Strings;
	void Init( void )
	{
		Names.Init();
		Positions.Init();
		Strings.Init();
	}
	bso::bool__ Store(
		const str::string_ &Name,
		xtf::location__ Line,
		xtf::location__ Column,
		const str::string_ &String )
	{
		if ( _Locate( Name ) != NONE )
			return false;

		rrow__ Row = Names.Append( Name );

		if ( Row != Positions.Append( position__( Line, Column ) ) )
			ERRc();

		if ( Row != Strings.Append( String ) )
			ERRc();

		return true;
	}
	bso::bool__ Get(
		const str::string_ &Name,
		xtf::location__ &Line,
		xtf::location__ &Column,
		str::string_ &String ) const
	{
		rrow__ Row = _Locate( Name );

		if ( Row == NONE )
			return false;

		position__ P = Positions.Get( Row );

		Strings.Recall( Row, String );

		Line = P.Line;
		Column = P.Column;

		return true;
	}
};

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
	virtual bso::bool__ XMLStartTag(
		const str::string_ &Name,
		const str::string_ &Dump )
	{
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
		const str::string_ &Dump )
	{
		return true;
	}
	virtual bso::bool__ XMLValue(
		const str::string_ &TagName,
		const str::string_ &Value,
		const str::string_ &Dump )
	{
		return true;
	}
	virtual bso::bool__ XMLEndTag(
		const str::string_ &Name,
		const str::string_ &Dump )
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
	void _Store( const str::string_ &Dump )
	{
		epeios::row__ Row = Dump.First();

		while ( Row != NONE ) {
			_Flow->Put( Dump( Row ) );

			Row = Dump.Next( Row );
		}
	}
protected:
	virtual bso::bool__ XMLStartTag(
		const str::string_ &Name,
		const str::string_ &Dump )
	{
		_Store( Dump );

		return true;
	}
	virtual bso::bool__ XMLStartTagClosed(
		const str::string_ &Name,
		const str::string_ &Dump )
	{
		_Store( Dump );

		return true;
	}
	virtual bso::bool__ XMLAttribute(
		const str::string_ &TagName,
		const str::string_ &Name,
		const str::string_ &Value,
		const str::string_ &Dump )
	{
		_Store( Dump );

		return true;
	}
	virtual bso::bool__ XMLValue(
		const str::string_ &TagName,
		const str::string_ &Value,
		const str::string_ &Dump )
	{
		_Store( Dump );

		return true;
	}
	virtual bso::bool__ XMLEndTag(
		const str::string_ &Name,
		const str::string_ &Dump )
	{
		_Store( Dump );

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
#define VALUE_ATTRIBUTE		"value"

#define DEFINE_TAG	"define"
#define EXPAND_TAG	"expand"
#define IFEQ_TAG	"ifeq"
#define SET_TAG		"set"
#define BLOC_TAG	"bloc"


class extended_callback
: public callback__
{
private:
	callback__ *_UserCallback;
	xtf::extended_text_iflow__ *_Flow;
	variables _Variables;
	repository _Repository;
	str::string _Namespace;
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
	str::string _BlocPendingDump;
	// Contient la valeur de l'attribut 'name' ou 'select'.
	str::string _NameSelectAttribute;
	// Contient la valeur de l'attribut 'value'.
	str::string _ValueAttribute;
	bso::bool__ _IsDefining;
	bso::bool__ _BelongsToNamespace( const str::string_ &TagName ) const
	{
		return str::Compare( TagName, _Namespace, TagName.First(), _Namespace.First(), _Namespace.Amount() ) == 0;
	}
	bso::bool__ _Ignore( xtf::extended_text_iflow__ &Flow )
	{
		donothing_callback__ Callback;

		Callback.Init();

		return xml::Parse( Flow, Callback );
	}
	bso::bool__ _HandleDefine( 
		xtf::extended_text_iflow__ &Flow,
		const str::string_ &Name )
	{
		bso::bool__ Success = false;
	ERRProlog
		xtf::location__ Line, Column;
		store_callback Callback;
		str::string String;
		flx::E_STRING_OFLOW___ SFlow;
	ERRBegin
		Line = Flow.Line();
		Column = Flow.Column();

		String.Init();

		SFlow.Init( String );

		Callback.Init( SFlow );

		Success = Parse( Flow, Callback );

		if ( !Success )
			ERRReturn;

		SFlow.Synchronize();

		_Repository.Store(Name, Line, Column, String );
	ERRErr
	ERREnd
	ERREpilog
		return Success;
	}
	bso::bool__ _HandleExpand( const str::string_ &Name )
	{
		bso::bool__ Success = false;
	ERRProlog
		str::string String;
		flx::E_STRING_IFLOW__ SFlow;
		xtf::extended_text_iflow__ XFlow;
		xtf::location__ Line, Column;
		xtf::extended_text_iflow__ *PreviousFlow = _Flow;
	ERRBegin
		String.Init();

		if ( !_Repository.Get( Name, Line, Column, String ) )
			ERRReturn;

		SFlow.Init( String );
		SFlow.EOFD( XTF_EOXT );	// Normalement inutile (la conformité au format XML à déjà été traité), mais aide au déboguage.


		XFlow.Init( SFlow, Line, Column );

		_Flow = &XFlow;

		Success = Parse( XFlow, *this );

		_Flow = PreviousFlow;

		if ( !Success )
			_Flow->Set( XFlow.Line(), XFlow.Column() );	// Pour que l'utilisateur puisse récupèrer la position de l'erreur.
	ERRErr
	ERREnd
	ERREpilog
		return Success;
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
	virtual bso::bool__ XMLStartTag(
		const str::string_ &Name,
		const str::string_ &Dump )
	{
		if ( _IsDefining )
			return false;

		if ( !_BelongsToNamespace( Name ) )
			_BlocPendingTag = Name;

		switch ( _GetTag( Name ) ) {
		case tUser:
			return _UserCallback->XMLStartTag( Name, Dump );
			break;
		case tDefine:
			_NameSelectAttribute.Init();
			break;
		case tExpand:
			_NameSelectAttribute.Init();
			break;
		case tIfeq:
			_NameSelectAttribute.Init();
			_ValueAttribute.Init();
			break;
		case tBloc:
			break;
		case tSet:
			_NameSelectAttribute.Init();
			_ValueAttribute.Init();
			break;
		case t_Undefined:
			return false;
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
		const str::string_ &Dump )
	{
		switch ( _GetTag( TagName ) ) {
		case tUser:
			return _UserCallback->XMLAttribute( TagName, Name, Value, Dump );
			break;
		case tDefine:
			if ( Name == NAME_ATTRIBUTE ) {
				if ( _NameSelectAttribute.Amount() != 0 )
					return false;

				_NameSelectAttribute = Value;
			} else
				return false;
			break;
		case tExpand:
			if ( Name == SELECT_ATTRIBUTE ) {
				if ( _NameSelectAttribute.Amount() != 0 )
					return false;

				_NameSelectAttribute = Value;
			} else
				return false;
			break;
		case tIfeq:
			if ( Name == SELECT_ATTRIBUTE ) {
				if ( _NameSelectAttribute.Amount() != 0 )
					return false;

				_NameSelectAttribute = Value;
			} else
			if ( Name == VALUE_ATTRIBUTE ) {
				if ( _ValueAttribute.Amount() != 0 )
					return false;

				_ValueAttribute = Value;
			} else
				return false;
			break;
		case tBloc:
			return false;
			break;
		case tSet:
			if ( Name == NAME_ATTRIBUTE ) {
				if ( _NameSelectAttribute.Amount() != 0 )
					return false;

				_NameSelectAttribute = Value;
			} else
			if ( Name == VALUE_ATTRIBUTE ) {
				if ( _ValueAttribute.Amount() != 0 )
					return false;

				_ValueAttribute = Value;
			} else
				return false;
			break;
		case t_Undefined:
			return false;
			break;
		default:
			ERRc();
			break;
		}

		return true;
	}
	virtual bso::bool__ XMLStartTagClosed(
		const str::string_ &Name,
		const str::string_ &Dump )
	{
		switch ( _GetTag( Name ) ) {
		case tUser:
			return _UserCallback->XMLStartTagClosed( Name, Dump );
			break;
		case tDefine:
			if ( _NameSelectAttribute.Amount() == 0 )
				return false;

			if ( _Variables.Exists( _NameSelectAttribute ) )
				return false;

			if ( !_HandleDefine( *_Flow, _NameSelectAttribute ) )
				return false;

			_IsDefining = true;

			break;
		case tExpand:
			break;
		case tIfeq:
			if ( _NameSelectAttribute.Amount() == 0 )
				return false;
			if ( !_Variables.Exists( _NameSelectAttribute ) )
				return false;
			if ( !_Variables.IsEqual( _NameSelectAttribute, _ValueAttribute ) )
				return _Ignore( *_Flow );
			break;
		case tBloc:
			break;
		case tSet:
			if ( _NameSelectAttribute.Amount() == 0 )
				return false;
			break;
		case t_Undefined:
			return false;
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
		const str::string_ &Dump )
	{
		switch ( _GetTag( TagName ) ) {
		case tUser:
			return _UserCallback->XMLValue( TagName, Value, Dump );
			break;
		case tDefine:
			return false;
			break;
		case tExpand:
			return false;
			break;
		case tIfeq:
			return false;
			break;
		case tBloc:
			_BlocPendingValue.Append( Value );
			_BlocPendingDump.Append( Dump );
			break;
		case tSet:
			return false;
			break;
		case t_Undefined:
			return false;
			break;
		default:
			ERRc();
			break;
		}

		return true;
	}
	virtual bso::bool__ XMLEndTag(
		const str::string_ &Name,
		const str::string_ &Dump )
	{
		if ( !_BelongsToNamespace( Name ) && ( _BlocPendingValue.Amount() != 0 ) ) {
			_UserCallback->XMLValue( _BlocPendingTag, _BlocPendingValue, _BlocPendingDump );

			_BlocPendingTag.Init();
			_BlocPendingValue.Init();
			_BlocPendingDump.Init();
		}

		switch ( _GetTag( Name ) ) {
		case tUser:
		return _UserCallback->XMLEndTag( Name, Dump );
			break;
		case tDefine:
			_NameSelectAttribute.Init();

			_IsDefining = false;
			break;
		case tExpand:
			if ( _NameSelectAttribute.Amount() == 0 )
				return false;

			if ( !_HandleExpand( _NameSelectAttribute ) )
				return false;

			_NameSelectAttribute.Init();
			break;
		case tIfeq:
			_NameSelectAttribute.Init();
			_ValueAttribute.Init();
			break;
		case tBloc:
			break;
		case tSet:
			if ( _NameSelectAttribute.Amount() == 0 )
				return false;

			if ( _Variables.Exists( _NameSelectAttribute ) )
				return false;

			_Variables.Set( _NameSelectAttribute, _ValueAttribute );

			_NameSelectAttribute.Init();
			_ValueAttribute.Init();
			break;
		case t_Undefined:
			return false;
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
			xtf::extended_text_iflow__ &Flow,
			callback__ &UserCallback )
		{
			_UserCallback = &UserCallback;
			_Flow = &Flow;
			_Variables.Init();
			_Repository.Init();

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

			_NameSelectAttribute.Init();
			_ValueAttribute.Init();

			_BlocPendingTag.Init();
			_BlocPendingValue.Init();
			_BlocPendingDump.Init();

			_IsDefining = false;
		}
};

bso::bool__ xml::ExtendedParse(
	xtf::extended_text_iflow__ &Flow,
	const str::string_ &Namespace,
	callback__ &Callback )
{
	bso::bool__ Success = false;
ERRProlog
	extended_callback XCallback;
ERRBegin
	XCallback.Init( Namespace, Flow, Callback );

	Success = Parse( Flow, XCallback );
ERRErr
ERREnd
ERREpilog
	return Success;
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
