/*
	Header for the 'xpp' library by Claude SIMON (csimon at zeusw dot org)
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

#ifndef XPP__INC
#define XPP__INC

#define XPP_NAME		"XPP"

#define	XPP_VERSION	"$Revision$"

#define XPP_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &XPPTutor;

#if defined( XXX_DBG ) && !defined( XPP_NODBG )
#define XPP_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision$
//C Claude SIMON (csimon at zeusw dot org)
//R $Date$

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D XML PreProcessor 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# include "err.h"
# include "flw.h"
# include "xml.h"
# include "fdr.h"
# include "xtf.h"
# include "flx.h"
# include "flf.h"
# include "crptgr.h"
# include "cdgb64.h"


# ifdef XPP_PREPROCESSOR_DEFAULT_NAMESPACE
#  define XPP__PREPROCESSOR_DEFAULT_NAMESPACE XPP_PREPROCESSOR_DEFAULT_NAMESPACE
#else
# define XPP__PREPROCESSOR_DEFAULT_NAMESPACE	"xpp"
#endif

namespace xpp {

	// NOTA : Si modifi�, modifier 'GetTranslation()' en cons�quent, ainsi que le contenu du ficher 'exml.xlc'.
	enum status__ {
		sOK = xml::sOK,
		sNoTagsAllowedHere = xml::s_amount,
		sUnexpectedValue,
		sUnknownDirective,
		sUnexpectedAttribute,
		sUnknownAttribute,
		sMissingNameAttribute,
		sMissingSelectOrHRefAttribute,
		sMissingSelectAttribute,
		sMissingValueAttribute,
		sMissingNameAndValueAttributes,
		sMissingSelectAndValueAttributes,
		sMustBeEmpty,
		sTooManyTags,
		sUnableToOpenFile,
		sUnknownMacro,
		sBadAttributeDefinitionSyntax,
		sBadCypherKey,
		sMissingCypherKey,
		sMissingKeyOrFormatAttribute,
		sEmptyResult,

		s_amount,
		s_Undefined,
		s_Pending,
	};

	inline status__ _Convert( xml::status__ Status )
	{
		return (status__)Status;
	}

	const char *Label( status__ Status );

	struct context___ {
		status__ Status;
		xtf::coord__ Coord;
		str::string FileName;
		void reset( bso::bool__ P = true )
		{
			Status = s_Undefined;

			Coord.reset( P  );
			FileName.reset( P );
		}
		context___( void )
		{
			reset( false );

			Init();
		}
		context___(
			status__ Status,
			const xtf::coord__ &Coord,
			const str::string_ &FileName = str::string() )
		{
			reset( false );

			this->Status = Status;
			this->Coord = Coord;
			this->FileName.Init( FileName );
		}
		~context___( void )
		{
			reset();
		}
		void Init( void )
		{
			Status = s_Undefined;

			Coord.Init();
			FileName.Init();
		}
	};

	const str::string_ &GetTranslation(
		const context___ &Context,
		const lcl::rack__ &LocaleRack,
		str::string_ &Translation );

	struct _qualified_preprocessor_directives___ {
		str::string NamespaceWithSeparator;
		str::string DefineTag;
		str::string ExpandTag;
		str::string SetTag;
		str::string IfeqTag;
		str::string BlocTag;
		str::string CypherTag;
		str::string AttributeAttribute;	//'<tag xpp:attribute="..." ...>'//
		str::string XMLNS;	// <... xmlns:xpp="..." ...> ('xpp' ou ce qui a �t� choidi par l'utilisateur ...).
		void reset( bso::bool__ P = true )
		{
			NamespaceWithSeparator.reset( P );

			DefineTag.reset( P );
			ExpandTag.reset( P );
			SetTag.reset( P );
			IfeqTag.reset( P );
			BlocTag.reset( P );
			AttributeAttribute.reset( P );
			XMLNS.reset( P );
		}
		_qualified_preprocessor_directives___( void )
		{
			reset( false );
		}
		~_qualified_preprocessor_directives___( void )
		{
			reset();
		}
		void Init( const str::string_ &Namespace );
	};

	E_ROW( _rrow__ );	// Repository row.

	class _repository_
	{
	private:
		_rrow__ _Locate( const str::string_ &Name ) const
		{
			ctn::E_CMITEMt( str::string_, _rrow__ ) NamesItem;
			_rrow__ Row = Names.First();

			NamesItem.Init( Names );

			while ( ( Row!= NONE ) && ( NamesItem( Row ) != Name ) )
				Row = Names.Next( Row );

			return Row;
		}
	public:
		struct s {
			ctn::E_XMCONTAINERt_( str::string_, _rrow__ )::s Names;
			bch::E_BUNCHt_( xtf::coord__, _rrow__ )::s Coords;
			ctn::E_XMCONTAINERt_( str::string_, _rrow__ )::s FileNames;
			ctn::E_XMCONTAINERt_( str::string_, _rrow__ )::s Contents;
		};
		ctn::E_XMCONTAINERt_( str::string_, _rrow__ ) Names;
		bch::E_BUNCHt_( xtf::coord__, _rrow__ ) Coords;
			ctn::E_XMCONTAINERt_( str::string_, _rrow__ ) FileNames;
		ctn::E_XMCONTAINERt_( str::string_, _rrow__ ) Contents;
		_repository_( s &S )
		: Names( S.Names ),
		  Coords( S.Coords ),
		  FileNames( S.FileNames ),
		  Contents( S.Contents )
		{}
		void reset( bso::bool__ P = true )
		{
			Names.reset( P );
			Coords.reset( P );
			FileNames.reset( P );
			Contents.reset( P );
		}
		void plug( mmm::E_MULTIMEMORY_ &MM )
		{
			Names.plug( MM );
			Coords.plug( MM );
			FileNames.plug( MM );
			Contents.plug( MM );
		}
		_repository_ &operator =( const _repository_ &R )
		{
			Names = R.Names;
			Coords = R.Coords;
			FileNames = R.FileNames;
			Contents = R.Contents;

			return *this;
		}
		void Init( void )
		{
			Names.Init();
			Coords.Init();
			FileNames.Init();
			Contents.Init();
		}
		bso::bool__ Delete( const str::string_ &Name )
		{
			_rrow__ Row = _Locate( Name );

			if ( Row != NONE ) {
				Names.Remove( Row );
				Coords.Remove( Row );
				FileNames.Remove( Row );
				Contents.Remove( Row );

				return true;
			} else
				return false;
		}
		bso::bool__ Store(
			const str::string_ &Name,
			xtf::coord__ Coord,
			const str::string_ &FileName,
			const str::string_ &Content )
		{
			bso::bool__ AlreadyExists = Delete( Name );
			_rrow__ Row = Names.Append( Name );

			if ( Row != Coords.Append( Coord ) )
				ERRc();

			if ( Row != FileNames.Append( FileName ) )
				ERRc();

			if ( Row != Contents.Append( Content ) )
				ERRc();

			return AlreadyExists;
		}
		bso::bool__ Get(
			const str::string_ &Name,
			xtf::coord__ &Coord,
			str::string_ &FileName,
			str::string_ &Content ) const
		{
			_rrow__ Row = _Locate( Name );

			if ( Row == NONE )
				return false;

			Coord = Coords.Get( Row );

			FileNames.Recall( Row, FileName );

			Contents.Recall( Row, Content );

			return true;
		}
	};

	E_AUTO( _repository )

	E_ROW( _vrow__ );	// Variable row.

	class _variables
	{
	private:
		_vrow__ _Locate( const str::string_ &Name ) const
		{
			ctn::E_CMITEMt( str::string_, _vrow__ ) NamesItem;
			_vrow__ Row = Names.First();

			NamesItem.Init( Names );

			while ( ( Row != NONE ) && ( NamesItem( Row ) != Name ) )
				Row = Names.Next( Row );

			return Row;
		}
	public:
		void reset( bso::bool__ P = true )
		{
			Names.reset( P );
			Values.reset( P );
		}
		_variables( void )
		{
			reset( false );
		}
		~_variables( void )
		{
			reset();
		}
		ctn::E_XMCONTAINERt( str::string_, _vrow__ ) Names, Values;
		void Init( void )
		{
			Names.Init();
			Values.Init();
		}
		void Set(
			const str::string_ &Name,
			const str::string_ &Value )
		{
			_vrow__ Row = _Locate( Name );

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
			_vrow__ Row = _Locate( Name );

			if ( Row != NONE ) {
				ctn::E_CMITEMt( str::string_, _vrow__ ) ValuesItem;
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
			ctn::E_CMITEMt( str::string_, _vrow__ ) ValuesItem;
			_vrow__ Row = _Locate( Name );

			if ( Row == NONE )
				ERRu();

			ValuesItem.Init( Values );

			return ValuesItem( Row ) == Value;
		}
	};

	typedef _variables	_variables_;

	class _extended_parser___
	{
	private:
		flf::file_iflow___ _FFlow;
		str::string _MacroContent;
		flx::E_STRING_IFLOW__ _SFlow;
		cdgb64::decoding_iflow___ _Decoder;
		crptgr::decrypt_iflow___ _Decrypter;
		xtf::extended_text_iflow__ _XFlow;
		xml::parser___ _Parser;
		_repository_ &_Repository;
		_variables_ &_Variables;
		_qualified_preprocessor_directives___ &_Directives;
		str::string _LocalizedFileName;	// Si le 'parser' sert � l'inclusion d'un fichier ('<xpp:expand href="...">), contient le nom du fichier inclut.
		str::string _Directory;
		str::string _CypherKey;
		bso::bool__ _IgnorePreprocessingInstruction;
		bso::bool__ _AttributeDefinitionInProgress;
		status__ _HandleDefineDirective( _extended_parser___ *&Parser );
		status__ _InitWithFile(
			const str::string_ &FileName,
			const str::string_ &Directory,
			const str::string_ &CypherKey );
		status__ _InitWithContent(
			const str::string_ &Content,
			const str::string_ &NameOfTheCurrentFile,
			const xtf::coord__ &Coord,
			const str::string_ &Directory,
			const str::string_ &CypherKey );
		status__ _InitCypher(
			flw::iflow__ &Flow,
			const str::string_ &FileName,
			const xtf::coord__ &Coord,
			const str::string_ &Directory,
			const str::string_ &CypherKey );
		status__ _HandleMacroExpand(
			const str::string_ &MacroName,
			_extended_parser___ *&Parser );
		status__ _HandleFileExpand(
			const str::string_ &FileName,
			_extended_parser___ *&Parser );
		status__ _HandleExpandDirective( _extended_parser___ *&Parser );
		status__ _HandleSetDirective( _extended_parser___ *&Parser );
		status__ _HandleIfeqDirective( _extended_parser___ *&Parser );
		status__ _HandleCypherDecryption(
			const str::string_ &MacroName,
			_extended_parser___ *&Parser );
		status__ _HandleCypherOverride(
			const str::string_ &CypherKey,
			_extended_parser___ *&Parser );
		status__ _HandleCypherDirective( _extended_parser___ *&Parser );
		status__ _HandleAttributeDirective(
			const str::string_ &Parameters,
			_extended_parser___ *&Parser,
			str::string_ &Data );
		status__ _HandlePreprocessorDirective(
			const str::string_ &DirectiveName,
			_extended_parser___ *&Parser );
	public:
		void reset( bso::bool__ P = true )
		{
			//_Repository.reset( P );
			//_Tags.reset( P );
			_MacroContent.reset( P );
			_FFlow.reset( P );
			_SFlow.reset( P );
			_Decoder.reset( P );
			_Decrypter.reset( P );
			_XFlow.reset( P );
			_LocalizedFileName.reset( P );
			_Directory.reset( P );
			_CypherKey.reset( P );
			_Parser.reset( P );
			_IgnorePreprocessingInstruction = false;
			_AttributeDefinitionInProgress = false;
		}
		_extended_parser___(
			_repository_ &Repository,
			_variables_ &Variables,
			_qualified_preprocessor_directives___ &Directives )
		: _Repository( Repository ),
		  _Variables( Variables ),
		  _Directives( Directives )
		{
			reset( false );
		}
		~_extended_parser___( void )
		{
			reset();
		}
		status__ Init(
			xtf::extended_text_iflow__ &XFlow,
			const str::string_ &LocalizedFileName,	// Si 'XFlow' est rattach� � un fichier, le nom de ce fichier (utile pour la gestion d'erreurs).
			const str::string_ &Directory,
			const str::string_ &CypherKey )
		{
			// _Repository.Init();
			// _Tags.Init();

			_Parser.Init( XFlow, xml::ehKeep );
			_LocalizedFileName.Init( LocalizedFileName );
			_Directory.Init( Directory );
			_CypherKey.Init( CypherKey );
			_IgnorePreprocessingInstruction = false;
			_AttributeDefinitionInProgress = false;

			return sOK;
		}
		status__ Handle(
			_extended_parser___ *&Parser,
			str::string_ &Data );
		const str::string_ &DumpData_( void ) const
		{
			return _Parser.DumpData();
		}
		const xtf::coord__ &DumpCoord( void ) const
		{
			return _Parser.DumpCoord();
		}
		const xtf::coord__ &Coord( void ) const
		{
			return _Parser.Flow().Coord();
		}
		E_RODISCLOSE__( str::string_, LocalizedFileName );
	};

	typedef stk::E_BSTACK_(_extended_parser___ *) _xparser_stack_;
	E_AUTO( _xparser_stack );

	typedef fdr::iflow_driver___<> _iflow_driver___;

	inline _extended_parser___ *NewParser(
		_repository_ &Repository,
		_variables_ &Variables,
		_qualified_preprocessor_directives___ &Directives )
	{
		_extended_parser___ *Parser = new _extended_parser___( Repository, Variables, Directives );

		if ( Parser == NULL )
			ERRa();

		return Parser;
	}

	struct criterions___
	{
		str::string
			Directory,
			CypherKey,
			Namespace;
		void reset( bso::bool__ P = true )
		{
			Directory.reset( P);
			CypherKey.reset( P );
			Namespace.reset( P );
		}
		criterions___( void )
		{
			reset( false );
		}
		~criterions___( void )
		{
			reset();
		}
		criterions___( 
			const str::string_ &Directory,
			const str::string_ &CypherKey = str::string() ,
			const str::string_ &Namespace = str::string() )
		{
			reset( false );

			Init( Directory, CypherKey, Namespace );
		}
		void Init( 
			const str::string_ &Directory,
			const str::string_ &CypherKey = str::string() ,
			const str::string_ &Namespace = str::string() )
		{
			this->Directory.Init( Directory );
			this->CypherKey.Init( CypherKey );
			this->Namespace.Init( Namespace );
		}
		bso::bool__ IsNamespaceDefined( void ) const
		{
			return Namespace.Amount() != 0;
		}
	};

	class _preprocessing_iflow_driver___
	: public _iflow_driver___
	{
	private:
		status__ _Status;
		_qualified_preprocessor_directives___ _Directives;
		_repository _Repository;
		_variables _Variables;
		str::string _Data;
		mdr::size__ _Position;	// Position du premier caract�re non lu dans le '_Data'.
		_xparser_stack _Parsers;
		_extended_parser___ *_CurrentParser;
		void _DeleteParsers( void );
		_extended_parser___ &_Parser( void )
		{
#ifdef XPP_DBG
			if ( _CurrentParser == NULL )
				ERRu();
#endif
			return *_CurrentParser;
		}
		const _extended_parser___ &_Parser( void ) const
		{
#ifdef XPP_DBG
			if ( _CurrentParser == NULL )
				ERRu();
#endif
			return *_CurrentParser;
		}
	protected:
		virtual mdr::size__ FDRRead(
			mdr::size__ Maximum,
			mdr::datum__ *Buffer );
		virtual void FDRDismiss( void )
		{}
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P ) {
				_DeleteParsers();
			}

			_Repository.reset( P );
			_Variables.reset( P );
			_Directives.reset();
			_Data.reset( P );
			_Position = 0;
			_iflow_driver___::reset( P );
			_Parsers.reset( P );
			_CurrentParser = NULL;
		}
		_preprocessing_iflow_driver___( void )
		{
			reset( false );
		}
		virtual ~_preprocessing_iflow_driver___( void )
		{
			reset();
		}
		void Init(
			xtf::extended_text_iflow__ &XFlow,
			fdr::thread_safety__ ThreadSafety,
			const criterions___ &Criterions )
		{
			_DeleteParsers();
			_Repository.Init();
			_Variables.Init();
# if 0	// A priori �quivalent � ce qu'il y a dans le '#else', mais VC++ 10 d�truit 'Criterions.Namespace' quand 'Criterions.IsNamespaceDefined()' est vrai. Fonctionner avec 'g++4.
			_Directives.Init( Criterions.IsNamespaceDefined() ? Criterions.Namespace : str::string( XPP__PREPROCESSOR_DEFAULT_NAMESPACE ) );
# else
			if ( Criterions.IsNamespaceDefined() )
				_Directives.Init( Criterions.Namespace );
			else
				_Directives.Init( str::string( XPP__PREPROCESSOR_DEFAULT_NAMESPACE ) );
# endif
			_Data.Init();
			_Position = 0;
			_iflow_driver___::Init( ThreadSafety );
			_CurrentParser = NewParser( _Repository, _Variables, _Directives );
			_Parsers.Init();
			if ( _Parser().Init( XFlow, str::string(), Criterions.Directory, Criterions.CypherKey ) != sOK )
				ERRc();
		}
		const context___ &GetContext( context___ &Context ) const
		{
			Context.Coord = _Parser().Coord();
			Context.FileName = _Parser().LocalizedFileName();
			Context.Status = _Status;

			return Context;
		}
	};

	typedef flw::standalone_iflow__<> _iflow__;

	class preprocessing_iflow___
	: public _iflow__
	{
	private:
		_preprocessing_iflow_driver___ _FlowDriver;
	public:
		void reset( bso::bool__ P = true )
		{
			_FlowDriver.reset( P );
			_iflow__::reset( P );
		}
		preprocessing_iflow___( void )
		{
			reset( false );
		}
		~preprocessing_iflow___( void )
		{
			reset( true );
		}
		void Init(
			xtf::extended_text_iflow__ &XFlow,
			const criterions___ &Criterions )
		{
			_FlowDriver.Init( XFlow, fdr::tsDisabled, Criterions );
			_iflow__::Init( _FlowDriver, FLW_SIZE_MAX );
			_iflow__::EOFD( XTF_EOXT );
		}
		const context___ &GetContext( context___ &Context ) const
		{
			return _FlowDriver.GetContext( Context );
		}
	};

	// Lorsqu'une erreur s'est produite; information stock�es dans 'PFlow'.
	inline const str::string_ &GetTranslation(
		const preprocessing_iflow___ &PFlow,
		const lcl::rack__ &LocaleRack,
		str::string_ &Translation )
	{
	ERRProlog
		context___ Context;
	ERRBegin
		Context.Init();

		GetTranslation( PFlow.GetContext( Context ), LocaleRack, Translation );
	ERRErr
	ERREnd
	ERREpilog
		return Translation;
	}

	status__ Encrypt(
		const str::string_ &Namespace,
		flw::iflow__ &IFlow,
		xml::writer_ &Writer,
		context___ &Context );

	status__ Encrypt(
		const str::string_ &Namespace,
		flw::iflow__ &IFlow,
		xml::outfit__ Outfit,
		txf::text_oflow__ &OFlow,
		context___ &Context );

	status__ Process(
		xtf::extended_text_iflow__ &XFlow,
		const criterions___ &Criterions,
		xml::writer_ &Writer,
		context___ &Context );

	status__ Process(
		xtf::extended_text_iflow__ &XFlow,
		const criterions___ &Criterions,
		xml::outfit__ Outfit,
		txf::text_oflow__ &OFlow,
		context___ &Context );

	inline status__ Process(
		xtf::extended_text_iflow__ &XFlow,
		const criterions___ &Criterions,
		xml::writer_ &Writer )
	{
		status__ Status = s_Undefined;
	ERRProlog
		context___ Context;
	ERRBegin
		Context.Init();

		Status = Process( XFlow, Criterions, Writer, Context );
	ERRErr
	ERREnd
	ERREpilog
		return Status;
	}

	inline status__ Process(
		xtf::extended_text_iflow__ &XFlow,
		const criterions___ &Criterions,
		xml::outfit__ Outfit,
		txf::text_oflow__ &OFlow )
	{
		status__ Status = s_Undefined;
	ERRProlog
		context___ Context;
	ERRBegin
		Context.Init();

		Status =  Process( XFlow, Criterions, Outfit, OFlow, Context );
	ERRErr
	ERREnd
	ERREpilog
		return Status;
	}


	inline status__ Process(
		xtf::extended_text_iflow__ &XFlow,
		xml::writer_ &Writer )
	{
		return Process( XFlow, str::string( "" ), Writer );
	}

	inline status__ Process(
		xtf::extended_text_iflow__ &XFlow,
		xml::outfit__ Outfit,
		txf::text_oflow__ &OFlow )
	{
		return Process( XFlow, str::string( "" ), Outfit, OFlow );
	}

}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
