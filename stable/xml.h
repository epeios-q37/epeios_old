/*
	Header for the 'xml' library by Claude SIMON (http://zeusw.org/intl/contact.html)
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

#ifndef XML__INC
#define XML__INC

#define XML_NAME		"XML"

#define	XML_VERSION	"$Revision$"

#define XML_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"

#include "ttr.h"

extern class ttr_tutor &XMLTutor;

#if defined( XXX_DBG ) && !defined( XML_NODBG )
#define XML_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision$
//C Claude SIMON (http://zeusw.org/intl/contact.html)
//R $Date$

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D Extensible Markup Language 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "str.h"
#include "xtf.h"
#include "stk.h"
#include "ctn.h"
#include "cpe.h"

// Prédéclaration.
namespace lcl {
	class meaning_;
}

namespace xml {
	using xtf::coord__;

	// Code d'erreur 'retourné' par 'Parse()'.
	enum status__ {
		sOK,
		sUnexpectedEOF,
		sUnknownEntity,
		sMissingEqualSign,
		sBadAttributeValueDelimiter,
		sUnexpectedCharacter,
		sEmptyTagName,
		sMismatchedTag,
		s_amount,
		s_Undefined,
	};

	const char *GetLabel( status__ Status );

	void GetMeaning(
		status__ Status,
		const coord__ &Coord,	// Obtenu de 'xtf::extended_text_iflow__'.
		lcl::meaning_ &Meaning );


	class dump_ {
	public:
		struct s {
			str::string_::s Data;
			coord__ Coord;
		} &S_;
		str::string_ Data;
		dump_( s &S )
		: S_( S ),
		  Data( S.Data )
		{
		}
		void reset( bso::bool__ P = true )
		{
			Data.reset( P );

			S_.Coord.reset( P );
		}
		void plug( sdr::E_SDRIVER__ &SD )
		{
			Data.plug( SD );
		}
		void plug( ags::E_ASTORAGE_ &AS )
		{
			Data.plug( AS );
		}
		dump_ &operator =( const dump_ &D )
		{
			Data = D.Data;

			S_.Coord = D.S_.Coord;

			return *this;
		}
		void Init( void )
		{
			Data.Init();

			S_.Coord.Init();
		}
		void Set( coord__ Coord )
		{
			S_.Coord = Coord;
		}
		void Append( const dump_ &Dump )
		{
			if ( Data.Amount() == 0 )
				this->operator =( Dump );
			else
				Data.Append( Dump.Data );
		}
		void PurgeData( void )
		{
			Init();
		}
		E_RWDISCLOSE_( coord__, Coord );
	};

	E_AUTO( dump )

	class _flow___ {
	private:
		xtf::extended_text_iflow__ *_Flow;
	public:
		dump Dump;
		void reset( bso::bool__ P = true )
		{
			Dump.reset( P );
			_Flow = NULL;
		}
		_flow___( void )
		{
			reset( false );
		}
		~_flow___( void )
		{
			reset();
		}
		flw::datum__ Get( void )
		{
			if ( Dump.Data.Amount() == 0 )
				Dump.Set( _Flow->Coord() );

			flw::datum__ C = _Flow->Get();

			Dump.Data.Append( (flw::datum__)C );

			return C;
		}
		flw::datum__ View( void )
		{
			return _Flow->View();
		}
		bso::bool__ EndOfFlow( void )
		{
			return _Flow->EndOfFlow();
		}
		void Unget( unsigned char C )
		{
			_Flow->Unget( C );

			Dump.Data.Truncate();
		}
		void Purge( void )
		{
			Dump.PurgeData();
		}
		void Init( xtf::extended_text_iflow__ &Flow )
		{
			reset();

			_Flow = &Flow;

			Dump.Init();
		}
		const xtf::coord__ &GetCurrentCoord( void ) const
		{
			return _Flow->Coord();
		}
		xtf::extended_text_iflow__ &Flow( void )
		{
			return *_Flow;
		}
		const xtf::extended_text_iflow__ &Flow( void ) const
		{
			return *_Flow;
		}
	};

	enum _context__ {
		cHeaderExpected,
		cTagExpected,
		cOpeningTag,
		cClosingTag,
		cAttribute,
		cValueExpected,
		c_amount,
		c_Undefined
	};

	enum token__
	{
		tProcessingInstruction,
		tStartTag,
		tStartTagClosed,
		tAttribute,
		tSpecialAttribute,	// 'xmlns[:...]', par exemple.
		tValue,
		tEndTag,
		tComment,
		tCData,
		t_amount,
		t_Processed,	// Tout le flux XML a été traité.
		t_Error,	// Erreur dans l'analyse du flux XML; voir 'Status'.
		t_Undefined
	};

#ifdef TF
#	define XML__TF_BUFFER	TF
#	undef TF
#endif

#define TF( name )	tf##name = ( 1 << t##name )

	// Permet de n'avoir à traiter que certains 'token's.
	enum token_flag__
	{
		tfNone = 0,	// Pour n'obtenir que 'tError' et 'tProcessed', qui ne peuvent être ignorés.
		TF( ProcessingInstruction ),
		TF( StartTag ),
		TF( StartTagClosed ),
		TF( Attribute ),
		TF( SpecialAttribute ),
		TF( Value ),
		TF( EndTag ),
		TF( Comment ),
		TF( CData ),
		tfAll = ( ( 1 << t_amount ) - 1 ),
		tfAllButUseless = tfAll & ~( tfComment | tfSpecialAttribute | tfCData ),
		tfObvious = tfStartTag | tfAttribute | tfValue | tfEndTag,
		tfAllAttributes = tfAttribute | tfSpecialAttribute,
	};

#undef TF

#ifdef XML__TF_BUFFER
#	define	TF	XML__TF_BUFFER
#endif
	// Traitement des entités ('&...;').
	enum entities_handling__ {
		ehReplace,	// Les 'entity's sont remplacés par leur caractère correspondant.
		ehKeep,		// Les 'entity's sont gardés tel quel.
		eh_amount,
		eh_Undefined,
		eh_Default = ehReplace,
	};

	class parser___
	{
	private:
		_context__ _Context;
		token__ _Token;
		stk::E_MCSTACK( str::string_ ) _Tags;
		bso::bool__ _EmptyTag;	// A 'true' pour '<tag/>', sinon à 'false'.
		_flow___ _Flow;
		str::string _TagName;
		str::string _AttributeName;
		str::string _Value;
		status__ _Status;
		entities_handling__ _EntitiesHandling;
	public:
		void reset( bso::bool__ P = true )
		{
			_Context = c_Undefined;
			_Token = t_Undefined;
			_EmptyTag = false;
			_Flow.reset( P );

			_TagName.reset( P );
			_AttributeName.reset( P );
			_Value.reset( P );
			_Status = s_Undefined;
			_EntitiesHandling = eh_Undefined;

			_Tags.reset( P );
		}
		parser___( void )
		{
			reset( false );
		}
		virtual ~parser___( void )
		{
			reset();
		}
		void Init(
			xtf::extended_text_iflow__ &Flow,
			entities_handling__ EntitiesHandling )
		{
			reset();

			_Tags.Init();
			_Flow.Init( Flow );
			_Context = cHeaderExpected;

			_TagName.Init();
			_AttributeName.Init();
			_Value.Init();

			_EntitiesHandling = EntitiesHandling;
		}
		token__  Parse( int TokenToReport = tfAll );
		token__ Parse(
			str::string_ &TagName,
			str::string_ &AttributeName,
			str::string_ &Value,	// Contient la valeur d'une balise ('tag') our d'un attribut, en fonction de la valeur retournée ('tTag' ou 'tAttribute').
			xml::dump_ &Dump,
			status__ &Status,
			int TokenToReport = tfAll )	// 'Status' initialisé seulement si valeur retournée == 'tError'.
		{
			token__ Token = Parse( TokenToReport );

			TagName = _TagName;
			AttributeName = _AttributeName;
			Value = _Value;
			Status = _Status;

			return Token;
		}
		const xtf::coord__ &DumpCoord( void ) const
		{
			return _Flow.Dump.Coord();
		}
		const str::string_ &DumpData( void ) const
		{
			return _Flow.Dump.Data;
		}
		void PurgeDumpData( void )
		{
			_Flow.Dump.PurgeData();
		}
		E_RODISCLOSE__( str::string_, TagName );
		E_RODISCLOSE__( str::string_, AttributeName );
		E_RODISCLOSE__( str::string_, Value );
		E_RODISCLOSE__( status__, Status );
		E_RODISCLOSE__( token__, Token );
		E_RODISCLOSE__( entities_handling__, EntitiesHandling );
		const xtf::coord__ &GetCurrentCoord( void ) const
		{
			return _Flow.GetCurrentCoord();
		}
		xtf::extended_text_iflow__ &Flow( void )
		{
			return _Flow.Flow();
		}
		const xtf::extended_text_iflow__ &Flow( void ) const
		{
			return _Flow.Flow();
		}
	};

	struct callback__
	{
		virtual bso::bool__ XMLProcessingInstruction( const dump_ &Dump ) = 0;
		virtual bso::bool__ XMLStartTag(
			const str::string_ &Name,
			const dump_ &Dump ) = 0;
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
			const dump_ &Dump ) = 0;
		virtual bso::bool__ XMLSpecialAttribute(
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
			const dump_ &Dump ) = 0;
		virtual bso::bool__ XMLEndTag(
			const str::string_ &Name,
			const dump_ &Dump ) = 0;
		virtual bso::bool__ XMLComment(
			const str::string_ &Value,
			const dump_ &Dump )
		{
			return true;
		}
		void Init( void )
		{}
	};

	status__ Parse(
		xtf::extended_text_iflow__ &Flow,
		entities_handling__ EntitiesHandling,
		callback__ &Callback );
	// Si valeur retournée == 'false', 'Flow.Line()' et 'Flow.Column()' est positionné là où il y a l'erreur.

	// Transformation des caractères spéciaux, comm '<' qui devient '&lt;'.
	void TransformUsingEntities(
		str::string_ &Target,
		bso::bool__ DelimiterOnly );	// Si à true', seul les délimiteurs de valeur d'attributs (''' et '"') sont modifiés.

	inline void TransformUsingEntities(
		const str::string_ &Source,
		bso::bool__ DelimiterOnly,
		str::string_ &Target )	// Conversion des caractères spéciaux, comme '<' qui devient '&lt;'.
	{
		Target = Source;

		TransformUsingEntities( Target, DelimiterOnly );
	}


	typedef str::string_	name_;
	typedef str::string		name;

	typedef str::string_	value_;
	typedef str::string		value;

	// Mise en forme de la sortie.
	enum outfit__ {
		oCompact,	// Tout sur une seule ligne.
		oIndent,	// Indenté.
		o_amount,
		o_Undefined
	};	// 

	// Traitement des caractères spéciaux ('<', '&', ...).
	enum special_char_handling__ {
		schReplace,	// Sont remplacés par les 'entity's dédiés.
		schKeep,	// Ne sont pas remplacés. Cependant, les '"' sont tout de même remplacé per leur 'entity' correspondante dans les valeurs d'attributs.
		sch_amount,
		sch_Undefined,
		sch_Default = schReplace
	};

	enum _encoding__ {
		eISO_8859_1,
		e_amount,
		e_Undefined,
		e_Default = eISO_8859_1,
		e_None,
	};

	inline const char *Label( _encoding__ Encoding )
	{
		switch ( Encoding ) {
		case eISO_8859_1:
			return "ISO-8859-1";
			break;
		case e_None:
			return NULL;
			break;
		default:ERRu();
			break;
		}
		return NULL;	// Pour éviter un 'Warning'.
	}

	class encoding__
	{
	private:
		const char *_EncodingString;
	public:
		encoding__( void )
		{
			_EncodingString = Label( e_Default );
		}
		encoding__( _encoding__ Encoding )
		{
			_EncodingString = Label( Encoding );
		}
		encoding__( const char *Encoding )
		{
			_EncodingString = Encoding;
		}
		const char *EncodingString( void ) const
		{
			return _EncodingString;
		}
	};

	inline bso::bool__ WriteXMLHeader(
		txf::text_oflow__ &OFlow,
		const char *Encoding )
	{
		if ( Encoding != NULL ) {
			OFlow << "<?xml version=\"1.0\" encoding=\"" << Encoding << "\"?>";
			return true;
		} else
			return false;
	}

	inline bso::bool__ WriteXMLHeader(
		txf::text_oflow__ &OFlow,
		_encoding__ Encoding )
	{
		return WriteXMLHeader( OFlow, Label( Encoding ) );
	}

	inline bso::bool__ WriteXMLHeader(
		txf::text_oflow__ &OFlow,
		encoding__ Encoding )
	{
		return WriteXMLHeader( OFlow, Encoding.EncodingString() );
	}

	class writer_
	{
	private:
		void _CloseAllTags( void );
		void _WriteTabs( bso::ulong__ Amount ) const;
	public:
		struct s {
			stk::E_MCSTACK_( name_ )::s Tags;
			txf::text_oflow__ *Flow;
			bso::bool__ TagNameInProgress;
			bso::bool__ TagValueInProgress;
			outfit__ Outfit;
			special_char_handling__ SpecialCharHandling;
			bso::bool__ Ignore;
		} &S_;
		stk::E_MCSTACK_( name_ ) Tags;
		writer_( s &S )
		: S_( S ),
		  Tags( S.Tags )
		{}
		void reset( bso::bool__ P = true )
		{
			if ( P )
				_CloseAllTags();

			S_.TagNameInProgress = false;
			S_.TagValueInProgress = false;

			Tags.reset( P );
			S_.Flow = NULL;
			S_.Outfit = o_Undefined;
			S_.SpecialCharHandling = sch_Undefined;
			S_.Ignore = false;
		}
		void plug( ags::E_ASTORAGE_ &AS )
		{
			Tags.plug( AS );
		}
		writer_ &operator =( const writer_ &W )
		{
			Tags = W.Tags;

			S_.TagNameInProgress = W.S_.TagNameInProgress;
			S_.TagValueInProgress = W.S_.TagValueInProgress;
			S_.Flow = W.S_.Flow;
			S_.Outfit = W.S_.Outfit;
			S_.SpecialCharHandling = W.S_.SpecialCharHandling;
			S_.Ignore = W.S_.Ignore;

			return *this;
		}
		void Init(
			txf::text_oflow__ &Flow,
			outfit__ Outfit,
			encoding__ Encoding,
			special_char_handling__ SpecialCharHandling = sch_Default )
		{
			reset();

			Tags.Init();
			S_.Flow = &Flow;
			S_.Outfit = Outfit;
			S_.SpecialCharHandling = SpecialCharHandling;

			if ( WriteXMLHeader( Flow, Encoding ) )
				switch ( Outfit ) {
				case oIndent:
					Flow << txf::nl;
					break;
				case oCompact:
					break;
				default:
					ERRu();
					break;
			}
		}
		void PushTag( const name_ &Name )
		{
			if ( S_.TagNameInProgress ) {
				*S_.Flow << '>';

				if ( S_.Outfit == oIndent )
					*S_.Flow << txf::nl;
			}

			if ( S_.Outfit == oIndent )
				_WriteTabs( Tags.Amount() );

			*S_.Flow << '<' << Name;
			Tags.Push( Name );
			S_.TagNameInProgress = true;
			S_.TagValueInProgress = false;
		}
		void PushTag( const char *Name )
		{
			PushTag( name( Name ) );
		}
		void PutValue(
			const value_ &Value );
		void PutValue( const char *Value )
		{
			PutValue( value( Value ) );
		}
		void PutValue(
			const value_ &Value,
			const name_ &Name )
		{
			PushTag( Name );

			PutValue( Value );

			PopTag();
		}
		void PutValue(
			const value_ &Value,
			const char *Name )
		{
			PutValue( Value, name( Name ) );
		}
		void PutValue(
			const char *Value,
			const char *Name )
		{
			PutValue( value( Value ), name( Name ) );
		}
		void PutAttribute(
			const name_ &Name,
			const value_ &Value );
		void PutAttribute(
			const char *Name,
			const char *Value )
		{
			PutAttribute( name( Name ), value( Value ) );
		}
		void PutAttribute(
			const char *Name,
			const value_ &Value )
		{
			PutAttribute( name( Name ), Value );
		}
		void PopTag( void );
		txf::text_oflow__ &GetFlow( void )
		{
			return *S_.Flow;
		}
		E_RODISCLOSE_( outfit__, Outfit )
		E_RODISCLOSE_( special_char_handling__, SpecialCharHandling )
	};

	E_AUTO( writer )

	inline void WriteXSLDeclaration(
		const str::string_ &XSLHRef,
		txf::text_oflow__ &OFlow )
	{
		OFlow << "<?xml-stylesheet type=\"text/xsl\" href=\"" << XSLHRef << "\"?>";
	}
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
