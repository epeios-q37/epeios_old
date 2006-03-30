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

using namespace xml;

static void SkipSpaces_(
	xtf::extended_text_iflow__ &Flow,
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


static char HandleEntity_( xtf::extended_text_iflow__ &Flow )
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
	xtf::extended_text_iflow__ &Flow,
	str::string_ &Id )
{
	while ( !Flow.EOX() && ( isalnum( Flow.View() ) || Flow.View() == ':' || Flow.View() == '_' ) )
		Id.Append( Flow.Get() );
}

static inline void GetName_( 
	xtf::extended_text_iflow__ &Flow,
	str::string_ &Name )
{
	GetId_( Flow, Name );
}

static bso::bool__ GetValue_(
	xtf::extended_text_iflow__ &Flow,
	bso::char__ Delimiter,
	str::string_ &Value )
{
	char C;
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
	xtf::extended_text_iflow__ &Flow,
	str::string_ &Value )
{
	return GetValue_( Flow, '<', Value );
}

inline static void GetAttributeValue_(
	xtf::extended_text_iflow__ &Flow,
	str::string_ &Value )
{
	GetValue_( Flow, '"', Value );
}

static void GetAttribute_(
	xtf::extended_text_iflow__ &Flow,
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

	SkipSpaces_( Flow );
}

static void SkipComment_( xtf::extended_text_iflow__ &Flow )
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

		if ( Flow.Get() != '-' )
			ERRI( iBeam );

		if ( Flow.EOX() )
			ERRI( iBeam );

		if ( Flow.Get() == '>' )
			Continue = false;
	}
}

static void SkipHeader_( xtf::extended_text_iflow__ &Flow )
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
	Attribute
};

using xtf::location__;

E_ROW( srow__ );

bso::bool__ xml::Parse(
	xtf::extended_text_iflow__ &Flow,
	callback__ &Callback,
	xtf::location__ &ErrorLine,
	xtf::location__ &ErrorColumn )
{
	bso::bool__ Success = true;
ERRProlog
	state__ State = HeaderExpected;
	str::string Name, Value, Tag;
	stk::E_XMCSTACKt( str::string_, srow__ ) Tags;
ERRBegin
	Tags.Init();

	SkipSpaces_( Flow );

	while ( !Flow.EOX() ) {
		switch ( State ) {
		case HeaderExpected:
			if ( Flow.Get() != '<' )
				ERRI( iBeam );

			if ( Flow.View() == '?' ) {
				SkipHeader_( Flow );
				SkipSpaces_( Flow );
			} else
				Flow.Unget( '<' );
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

			if ( !Callback.XMLTag( Name ) )
				ERRI( iBeam );

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

				if ( !Callback.XMLTagClosed( Name ) )
					ERRI( iBeam );

				Value.Init();
				if ( GetTagValue_( Flow, Value ) ) {
					if ( Tags.IsEmpty() )
						ERRI( iBeam );

					Tag.Init();

					Tags.Top( Tag );

					if ( !Callback.XMLValue( Tag, Value ) )
						ERRI( iBeam );
				}

				State = TagExpected;
				SkipSpaces_( Flow, false );
				break;
			case '>':
				Flow.Get();
				if ( Flow.View() != '<' ) {
					Value.Init();
					if ( GetTagValue_( Flow, Value ) ) {
						if ( Tags.IsEmpty() )
							ERRI( iBeam );

						Tag.Init();

						Tags.Top( Tag );

						if ( !Callback.XMLValue( Tag, Value ) )
							ERRI( iBeam );
					}
				}
				State = TagExpected;
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

			if ( !Callback.XMLAttribute( Tag, Name, Value ) )
					ERRI( iBeam );

			SkipSpaces_( Flow );

			if ( Flow.View() == '/' ) {

				Flow.Get();

				SkipSpaces_( Flow );

				if ( Flow.View() == '>' ) {

					if ( Tags.IsEmpty() )
						ERRI( iBeam );

					Tag.Init();

					Tags.Pop( Tag );

					if ( !Callback.XMLTagClosed( Tag ) )
						ERRI( iBeam );

					State = TagExpected;
					Flow.Get();
					SkipSpaces_( Flow );
				} else
					ERRI( iBeam );
			} else if ( Flow.View() == '>' ) {
				Flow.Get();
				if ( Flow.View() != '<' ) {
					Value.Init();
					if ( GetTagValue_( Flow, Value ) ) {

						if ( Tags.IsEmpty() )
							ERRI( iBeam );

						Tag.Init();

						Tags.Top( Tag );

						if ( !Callback.XMLValue( Tag, Value ) )
							ERRI( iBeam );
					}
				}
				State = TagExpected;
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

			if ( !Callback.XMLTagClosed( Tag ) )
				ERRI( iBeam );

			if ( Flow.View() != '<' ) {
				Value.Init();
				if ( GetTagValue_( Flow, Value ) ) {

					if ( Tags.IsEmpty() )
						ERRI( iBeam );

					Tag.Init();

					Tags.Top( Tag );

					if ( !Callback.XMLValue( Tag, Value ) )
						ERRI( iBeam );
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
		ErrorLine = Flow.Line();
		ErrorColumn = Flow.Column();
		ERRRst();
	}
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
