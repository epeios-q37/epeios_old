/*
	Header for the 'xml' library by Claude SIMON (csimon@epeios.org)
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

#ifndef XML__INC
#define XML__INC

#define XML_NAME		"XML"

#define	XML_VERSION	"$Revision$"

#define XML_OWNER		"Claude SIMON (csimon@epeios.org)"

#include "ttr.h"

extern class ttr_tutor &XMLTutor;

#if defined( XXX_DBG ) && !defined( XML_NODBG )
#define XML_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision$
//C Claude SIMON (csimon@epeios.org)
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

namespace xml {
	using xtf::location__;

	struct callback__
	{
		virtual bso::bool__ XMLProcessingInstruction( const str::string_ &Dump ) = 0;
		virtual bso::bool__ XMLStartTag(
			const str::string_ &Name,
			const str::string_ &Dump ) = 0;
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
			const str::string_ &Dump ) = 0;
		virtual bso::bool__ XMLValue(
			const str::string_ &TagName,
			const str::string_ &Value,
			const str::string_ &Dump ) = 0;
		virtual bso::bool__ XMLEndTag(
			const str::string_ &Name,
			const str::string_ &Dump ) = 0;
		void Init( void )
		{}
	};

	// Code d'erreur retourn�e par 'Parse()'.
	enum status__ {
		sOK,
		sUnexpectedEOF,
		sUnknownEntity,
		sMissingEqualSign,
		sBadAttributeValueDelimiter,
		sUnexpectedCharacter,
		sEmptyTagName,
		sMismatchedTag,
		sUserError,
		s_amount,
		s_Undefined,
	};

	const char *GetLabel( status__ Status );

	status__ Parse(
		xtf::extended_text_iflow__ &Flow,
		callback__ &Callback );
	// Si valeur retourn�e == 'false', 'Flow.Line()' et 'Flow.Column()' est positionn� l� o� il y a l'erreur.

	// Codes d'erreur retourn�e par 'ExtendedParse()'.
	enum extended_status__ {
		xsOK = sOK,
		xsNoTagsAllowedHere = s_amount,
		xsUnexpectedTag,
		xsUnknownTag,
		xsAttributeAlreadyDefined,
		xsUnexpectedAttribute,
		xsUnknownAttribute,
		xsMissingNameAttribute,
		xsMissingSelectAttribute,
		xsMissingValueAttribute,
		xsUnknownVariable,
		xsNoValueAllowedHere,
		xsTooManyTags,
		xsUnableToOpenFile,

		xs_amount,
		xs_Undefined,
	};

	const char *GetLabel( extended_status__ Status );

	// 'Parsing' avec gestin des extensions ('xxx:define', 'xxx:expand', 'xxx:set', 'xxx::ifeq', ...
	// o� 'xxx' est la valeur donn� � 'NameSpace'.
	extended_status__ ExtendedParse(
		xtf::extended_text_iflow__ &Flow,
		const str::string_ &Namespace,
		const str::string_ &Directory,
		callback__ &Callback,
		str::string_ &GuiltyFileName );
	// Si valeur retourn�e == 'false', 'Flow.Line()' et 'Flow.Column()' est positionn� l� o� il y a l'erreur.
	// Si 'GuiltyFileName' n'est pas vide, alors ce param�tre contient le nom du fichier contenant l'erreur.

	/* Traite toutes les balises 'xxx:...' (o� 'xxx' correspond � 'Namespace') du flux XML 'IFlow' et
	   g�n�re un flux XML sans balises 'xxx:...' dans 'IFlow'. */
	extended_status__ Normalize(
		xtf::extended_text_iflow__ &IFlow,
		const str::string_ &Namespace,
		const str::string_ &Directory,
		bso::bool__ Indent,
		txf::text_oflow__ &OFlow,
		str::string_ &GuiltyFileName );
	// Si valeur retourn�e == 'false', 'Flow.Line()' et 'Flow.Column()' est positionn� l� o� il y a l'erreur.
	// Si 'GuiltyFileName' n'est pas vide, alors ce param�tre contient le nom du fichier contenant l'erreur.

	void Transform( str::string_ &Target );	// Transformation des caract�res sp�ciaux, comm '<' qui devient '&lt;'.

	inline void Convert(
		const str::string_ &Source,
		str::string_ &Target )	// Conversion des caract�res sp�ciaux, comme '<' qui devient '&lt;'.
	{
		Target = Source;

		Transform( Target );
	}


	typedef str::string_	name_;
	typedef str::string		name;

	typedef str::string_	value_;
	typedef str::string		value;

	class writer_
	{
	private:
		void _CloseAllTags( void );
		void _WriteTabs( bso::ulong__ Amount ) const;
	public:
		struct s {
			stk::E_XMCSTACK_( name_ )::s Tags;
			txf::text_oflow__ *Flow;
			bso::bool__ TagNameInProgress;
			bso::bool__ TagValueInProgress;
			bso::bool__ Indent;
		} &S_;
		stk::E_XMCSTACK_( name_ ) Tags;
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
			bso::bool__ Indent = false;
		}
		void plug( mmm::E_MULTIMEMORY_ &MM )
		{
			Tags.plug( MM );
		}
		writer_ &operator =( const writer_ &W )
		{
			Tags = W.Tags;

			S_.TagNameInProgress = W.S_.TagNameInProgress;
			S_.TagValueInProgress = W.S_.TagValueInProgress;
			S_.Flow = W.S_.Flow;
			S_.Indent = W.S_.Indent;

			return *this;
		}
		void Init(
			txf::text_oflow__ &Flow,
			bso::bool__ Indent = true )
		{
			reset();

			Tags.Init();
			S_.Flow = &Flow;
			S_.Indent = Indent;
		}
		void PushTag( const name_ &Name )
		{
			if ( S_.TagNameInProgress ) {
				*S_.Flow << '>';

				if ( S_.Indent )
					*S_.Flow << txf::nl;
			}

			if ( S_.Indent )
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
		void PutValue( const value_ &Value );
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
		E_RODISCLOSE_( bso::bool__, Indent )
	};

	E_AUTO( writer )


	inline void WriteXMLHeader( txf::text_oflow__ &OFlow )
	{
		OFlow << "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>";
	}


}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
