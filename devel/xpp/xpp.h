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

#	include "err.h"
#	include "flw.h"
#	include "xml.h"
#	include "fwf.h"
#	include "flw.h"
#	include "xtf.h"
#	include "flx.h"
#	include "flf.h"


#	define XPP_PREPROCESSOR_DEFAULT_NAMESPACE	"xpp"

namespace xpp {

	// NOTA : Si modifié, modifier 'GetTranslation()' en conséquent, ainsi que le contenu du ficher 'exml.xlc'.
	enum status__ {
		sOK = xml::sOK,
		sNoTagsAllowedHere = xml::s_amount,
		sUnexpectedValue,
		sUnknownTag,
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

		s_amount,
		s_Undefined,
		s_Pending,
	};

	const char *GetLabel( status__ Status );

	const str::string_ &GetTranslation(
		status__ Status,
		const str::string_ &Language,
		const lcl::locales_ &Locales,
		str::string_ &Translation );


	struct _qualified_preprocessor_tags___ {
		str::string NamespaceWithSeparator;
		str::string Define;
		str::string Expand;
		str::string Set;
		str::string Ifeq;
		str::string Bloc;
		void reset( bso::bool__ P = true )
		{
			NamespaceWithSeparator.reset( P );

			Define.reset( P );
			Expand.reset( P );
			Set.reset( P );
			Ifeq.reset( P );
			Bloc.reset( P );
		}
		_qualified_preprocessor_tags___( void )
		{
			reset( false );
		}
		~_qualified_preprocessor_tags___( void )
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
			ctn::E_XMCONTAINERt_( str::string_, _rrow__ )::s Contents;
		};
		ctn::E_XMCONTAINERt_( str::string_, _rrow__ ) Names;
		bch::E_BUNCHt_( xtf::coord__, _rrow__ ) Coords;
		ctn::E_XMCONTAINERt_( str::string_, _rrow__ ) Contents;
		_repository_( s &S )
		: Names( S.Names ),
		  Coords( S.Coords ),
		  Contents( S.Contents )
		{}
		void reset( bso::bool__ P = true )
		{
			Names.reset( P );
			Coords.reset( P );
			Contents.reset( P );
		}
		void plug( mmm::E_MULTIMEMORY_ &MM )
		{
			Names.plug( MM );
			Coords.plug( MM );
			Contents.plug( MM );
		}
		_repository_ &operator =( const _repository_ &R )
		{
			Names = R.Names;
			Coords = R.Coords;
			Contents = R.Contents;

			return *this;
		}
		void Init( void )
		{
			Names.Init();
			Coords.Init();
			Contents.Init();
		}
		bso::bool__ Delete( const str::string_ &Name )
		{
			_rrow__ Row = _Locate( Name );

			if ( Row != NONE ) {
				Names.Remove( Row );
				Coords.Remove( Row );
				Contents.Remove( Row );

				return true;
			} else
				return false;
		}
		bso::bool__ Store(
			const str::string_ &Name,
			xtf::coord__ Coord,
			const str::string_ &Content )
		{
			bso::bool__ AlreadyExists = Delete( Name );
			_rrow__ Row = Names.Append( Name );

			if ( Row != Coords.Append( Coord ) )
				ERRc();

			if ( Row != Contents.Append( Content ) )
				ERRc();

			return AlreadyExists;
		}
		bso::bool__ Get(
			const str::string_ &Name,
			xtf::coord__ &Coord,
			str::string_ &Content ) const
		{
			_rrow__ Row = _Locate( Name );

			if ( Row == NONE )
				return false;

			Coord = Coords.Get( Row );

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

	class _extended_browser___
	{
	private:
		flf::file_iflow___ _FFlow;
		str::string _MacroContent;
		flx::E_STRING_IFLOW__ _SFlow;
		xtf::extended_text_iflow__ _XFlow;
		xml::browser___ _Browser;
		_repository_ &_Repository;
		_variables_ &_Variables;
		_qualified_preprocessor_tags___ &_Tags;
		str::string _LocalizedFileName;	// Si le 'browser' sert à l'inclusion d'un fichier ('<xpp:expand href="...">), contient le nom du fichier inclut.
		str::string _Directory;
		bso::bool__ _IgnorePreprocessingInstruction;
		status__ _HandleDefineTag( _extended_browser___ *&Browser );
		status__ _HandleMacroExpand(
			const str::string_ &MacroName,
			_extended_browser___ *&Browser );
		status__ _HandleFileExpand(
			const str::string_ &FileName,
			_extended_browser___ *&Browser );
		status__ _HandleExpandTag( _extended_browser___ *&Browser );
		status__ _HandleSetTag( _extended_browser___ *&Browser );
		status__ _HandleIfeqTag( _extended_browser___ *&Browser );
		status__ _HandlePreprocessorTag(
			const str::string_ &TagName,
			_extended_browser___ *&Browser );
	public:
		void reset( bso::bool__ P = true )
		{
			//_Repository.reset( P );
			//_Tags.reset( P );
			_MacroContent.reset( P );
			_FFlow.reset( P );
			_SFlow.reset( P );
			_XFlow.reset( P );
			_LocalizedFileName.reset( P );
			_Directory.reset( P );
			_Browser.reset( P );
			_IgnorePreprocessingInstruction = false;
		}
		_extended_browser___(
			_repository_ &Repository,
			_variables_ &Variables,
			_qualified_preprocessor_tags___ &Tags )
		: _Repository( Repository ),
		  _Variables( Variables ),
		  _Tags( Tags )
		{
			reset( true );
		}
		~_extended_browser___( void )
		{
			reset();
		}
		status__ Init(
			xtf::extended_text_iflow__ &XFlow,
			const str::string_ &LocalizedFileName,	// Si 'XFlow' est rattaché à un fichier, le nom de ce fichier (utile pour la gestion d'erreurs).
			const str::string_ &Directory )
		{
			// _Repository.Init();
			// _Tags.Init();

			_Browser.Init( XFlow );
			_LocalizedFileName.Init( LocalizedFileName );
			_Directory.Init( Directory );
			_IgnorePreprocessingInstruction = false;

			return sOK;
		}
		status__ InitWithFile(
			const str::string_ &FileName,
			const str::string_ &Directory );
		status__ InitWithContent(
			const str::string_ &Content,
			const xtf::coord__ &Coord,
			const str::string_ &Directory );
		status__ Handle(
			_extended_browser___ *&Browser,
			bso::bool__ &StripHeadingSpaces );
		const str::string_ &DumpData( void ) const
		{
			return _Browser.DumpData();
		}
		const xtf::coord__ &DumpCoord( void ) const
		{
			return _Browser.DumpCoord();
		}
		const xtf::coord__ &Coord( void ) const
		{
			return _Browser.Flow().Coord();
		}
		E_RODISCLOSE__( str::string_, LocalizedFileName );
	};

	typedef stk::E_BSTACK_(_extended_browser___ *) _xbrowser_stack_;
	E_AUTO( _xbrowser_stack );

	typedef fwf::iflow_functions___ _iflow_functions___;

	inline _extended_browser___ *NewBrowser(
		_repository_ &Repository,
		_variables_ &Variables,
		_qualified_preprocessor_tags___ &Tags )
	{
		_extended_browser___ *Browser = new _extended_browser___( Repository, Variables, Tags );

		if ( Browser == NULL )
			ERRa();

		return Browser;
	}

	class _preprocessing_iflow_functions___
	: public _iflow_functions___
	{
	private:
		status__ _Status;
		_qualified_preprocessor_tags___ _Tags;
		_repository _Repository;
		_variables _Variables;
		str::string _Data;
		mdr::size__ _Position;	// Position du premier caractère non lu dans le '_Data'.
		_xbrowser_stack _Browsers;
		_extended_browser___ *_CurrentBrowser;
		void _DeleteBrowsers( void );
		_extended_browser___ &_Browser( void )
		{
#ifdef XPP_DBG
			if ( _CurrentBrowser == NULL )
				ERRu();
#endif
			return *_CurrentBrowser;
		}
		const _extended_browser___ &_Browser( void ) const
		{
#ifdef XPP_DBG
			if ( _CurrentBrowser == NULL )
				ERRu();
#endif
			return *_CurrentBrowser;
		}
	protected:
		virtual mdr::size__ FWFRead(
			mdr::size__ Minimum,
			mdr::datum__ *Buffer,
			mdr::size__ Wanted );
		virtual void FWFDismiss( void )
		{}
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P ) {
				_DeleteBrowsers();
			}

			_Repository.reset( P );
			_Variables.reset( P );
			_Tags.reset();
			_Data.reset( P );
			_Position = 0;
			_iflow_functions___::reset( P );
			_Browsers.reset( P );
			_CurrentBrowser = NULL;
		}
		_preprocessing_iflow_functions___( void )
		{
			reset( false );
		}
		virtual ~_preprocessing_iflow_functions___( void )
		{
			reset();
		}
		void Init(
			xtf::extended_text_iflow__ &XFlow,
			const str::string_ &Directory,
			const str::string_ &Namespace )
		{
			_DeleteBrowsers();
			_Repository.Init();
			_Variables.Init();
			_Tags.Init( Namespace );
			_Data.Init();
			_Position = 0;
			_iflow_functions___::Init();
			_CurrentBrowser = NewBrowser( _Repository, _Variables, _Tags );
			_Browsers.Init();
			if ( _Browser().Init( XFlow, str::string(), Directory ) != sOK )
				ERRc();
		}
		E_RODISCLOSE__( status__, Status );
		const xtf::coord__ &Coord( void ) const
		{
			return _Browser().Coord();
		}
		const str::string_ &LocalizedFileName( void ) const
		{
			return _Browser().LocalizedFileName();
		}
	};

	typedef flw::unsafe_iflow___ _iflow___;
	typedef xtf::extended_text_iflow__ _extended_text_iflow__;

	class preprocessing_extended_text_iflow___
	: public _extended_text_iflow__
	{
	private:
		_extended_text_iflow__ _XFlow;
		_preprocessing_iflow_functions___ _FlowFunctions;
		_iflow___ _IFlow;
	public:
		void reset( bso::bool__ P = true )
		{
			_XFlow.reset( P );
			_FlowFunctions.reset( P );
			_IFlow.reset( P );
			_extended_text_iflow__::reset( P );
		}
		preprocessing_extended_text_iflow___( void )
		: _IFlow(
			_FlowFunctions,
			FLW_SIZE_MAX )
		{
			reset( false );
		}
		~preprocessing_extended_text_iflow___( void )
		{
			reset( true );
		}
		void Init(
			flw::iflow__ &IFlow,
			const str::string_ &Directory,
			const str::string_ &Namespace = str::string( XPP_PREPROCESSOR_DEFAULT_NAMESPACE ) )
		{
			_XFlow.Init( IFlow );
			_FlowFunctions.Init( _XFlow, Directory, Namespace );
			_IFlow.Init();
			_IFlow.EOFD( XTF_EOXT );
			_extended_text_iflow__::Init( _IFlow );
		}
		const _preprocessing_iflow_functions___ &Preprocessor( void ) const
		{
			return _FlowFunctions;
		}
	};

	status__ Process(
		flw::iflow__ &IFlow,
		const str::string_ &Namespace,
		bso::bool__ Indent,
		const str::string_ &Directory,
		txf::text_oflow__ &OFlow,
		xtf::coord__ &Coord,
		str::string_ &GuiltyFileName );
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
