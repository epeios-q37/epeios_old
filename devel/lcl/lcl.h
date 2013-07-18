/*
	Header for the 'lcl' library by Claude SIMON (csimon at zeusw dot org)
	Copyright (C) $COPYRIGHT_DATES$Claude SIMON.
$_RAW_$
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

#ifndef LCL__INC
#define LCL__INC

#define LCL_NAME		"LCL"

#define	LCL_VERSION	"$Revision$"

#define LCL_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &LCLTutor;

#if defined( XXX_DBG ) && !defined( LCL_NODBG )
#define LCL_DBG
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
//D LoCaLes 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# include "err.h"
# include "flw.h"
# include "stk.h"
# include "rgstry.h"

# define LCL_TAG_MARKER_C	'%'
# define LCL_TAG_MARKER_S	"%"

# define LCL_UNDEFINED_LEVEL	RGSTRY_UNDEFINED_LEVEL

namespace lcl {
	using rgstry::status__;
	using rgstry::context___;
	using rgstry::level__;
	using str::strings_;
	using str::strings;

	E_ROW( vrow__ );	// 'value row'.
	E_ROW( brow__ );	// 'basic row'.

	typedef ctn::E_MCONTAINERt_( str::string_, vrow__ ) values_;
	E_AUTO( values );

	typedef bch::E_BUNCH_( brow__ ) brows_;
	E_AUTO( brows );

	class _basic_
	{
	public:
		struct s {
			vrow__ Value;
			brows_::s Tags;
			bso::bool__ ToTranslate;
		} &S_;
		brows_ Tags;
		_basic_( s &S )
		: S_( S ),
		  Tags( S_.Tags )
		{}
		void reset( bso::bool__ P = true )
		{
			S_.Value = E_NIL;
			S_.ToTranslate = false;
			Tags.reset( P );
		}
		void plug( sdr::E_SDRIVER__ &SD )
		{
			Tags.plug( SD );
		}
		void plug( ags::E_ASTORAGE_ &AS )
		{
			Tags.plug( AS );
		}
		_basic_ &operator =( const _basic_ &B )
		{
			S_.Value = B.S_.Value;

			Tags = B.Tags;

			return *this;
		}
		void Init( bso::bool__ ToTranslate )
		{
			S_.Value = E_NIL;
			S_.ToTranslate = ToTranslate;

			Tags.Init();
		}
		E_RODISCLOSE_( bso::bool__, ToTranslate )
	};

	E_AUTO( _basic );

	typedef ctn::E_MCONTAINERt_( _basic_, brow__ ) _basics_;
	E_AUTO( _basics );

	class _core_
	{
	public:
		struct s {
			values_::s Values;
			_basics_::s Basics;
		};
		values_ Values;
		_basics_ Basics;
		_core_( s &S )
		: Values( S.Values ),
		  Basics( S.Basics )
		{}
		void reset( bso::bool__ P = true )
		{
			Values.reset( P );
			Basics.reset( P );
		}
		void plug( ags::E_ASTORAGE_ &AS )
		{
			Values.plug( AS );
			Basics.plug( AS );
		}
		_core_ &operator =( const _core_ &C )
		{
			Values = C.Values;
			Basics = C.Basics;

			return *this;
		}
		void Init( void )
		{
			Values.Init();
			Basics.Init();
		}
	};

	class meaning_ {
	private:
		_basic_ &_Basic( void )
		{
			if ( S_.Basic == E_NIL )
				ERRFwk();

			return Core.Basics( S_.Basic );
		}
		const _basic_ &_GetBasic(
			brow__ Row,
			ctn::E_CMITEMt( _basic_, brow__ ) &Basic ) const
		{
			Basic.Init( Core.Basics );

			return Basic( Row );
		}
		const _basic_ &_Basic( ctn::E_CMITEMt( _basic_, brow__ ) &Basic ) const
		{
			if ( S_.Basic == E_NIL )
				ERRFwk();

			return _GetBasic( S_.Basic, Basic );
		}
	public:
		struct s {
			brow__ Basic;
			_core_::s Core;
		} &S_;
		_core_ Core;
		meaning_( s &S )
		: S_( S ),
		  Core( S.Core )
		{}
		void reset( bso::bool__ P = true )
		{
			S_.Basic = E_NIL;
			Core.reset( P );
		}
		void plug( ags::E_ASTORAGE_ &AS )
		{
			Core.plug( AS );
		}
		meaning_ &operator =( const meaning_ &M )
		{
			S_.Basic = M.S_.Basic;
			Core = M.Core;

			return *this;
		}
		void Init( void )
		{
			Core.Init();

			S_.Basic = Core.Basics.New();

			_Basic().Init( true );

			Core.Basics.Flush();
		}
		void SetValue( const str::string_ &Value );
		void SetValue( const char *Value )
		{
			SetValue( str::string( Value ) );
		}
		void AddTag( const str::string_ &Value );
		void AddTag( const char *Value )
		{
			AddTag( str::string( Value ) );
		}
		void AddTag( const meaning_ &Meaning );
		const _basic_ &GetBasic( ctn::E_CMITEMt( _basic_, brow__ ) &Basic ) const
		{
			return _Basic( Basic );
		}
		bso::bool__ IsEmpty( void ) const
		{
			ctn::E_CMITEMt( _basic_, brow__ ) Basic;

			return _Basic( Basic ).S_.Value == E_NIL;
		}
	};

	E_AUTO( meaning )

	class locale_ {
	private:
		void _GetCorrespondingLabels(
			const strings_ &Labels,
			strings_ &Wordings ) const;
		// A des fins de compatibilit� ascendente.
		bso::bool__ _GetTranslationFollowingLanguageThenMessage(
			const str::string_ &Text,
			const char *Language,
			str::string_ &Translation ) const;
		// A des fins de compatibilit� ascendente.
		bso::bool__ _GetTranslationFollowingMessageThenLanguage(
			const str::string_ &Text,
			const char *Language,
			str::string_ &Translation ) const;
		bso::bool__ _GetTranslationFollowingLanguageThenText(
			const str::string_ &Text,
			const char *Language,
			str::string_ &Translation ) const;
		bso::bool__ _GetTranslationFollowingTextThenLanguage(
			const str::string_ &Text,
			const char *Language,
			str::string_ &Translation ) const;
		void _GetTranslation(
			const str::string_ &Text,
			const char *Language,
			str::string_ &Translation ) const;
	public:
		struct s {
			rgstry::multi_level_registry_::s Registry;
		} &S_;
		rgstry::multi_level_registry_ Registry;
		locale_( s &S )
		: S_( S ),
		  Registry( S.Registry )
		{}
		void reset( bso::bool__ P = true )
		{
			Registry.reset( P );
		}
		locale_ &operator =( const locale_ &L )
		{
			Registry = L.Registry;

			return *this;
		}
		void Init( void )
		{
			reset();

			Registry.Init();
		}
		level__ Push(
			xtf::extended_text_iflow__ &XFlow,
			const xpp::criterions___ &Criterions,
			const char *RootPath,
			context___ &Context )
		{
			level__ Level = Registry.PushEmbeddedLevel();

			if ( Registry.Fill( Level, XFlow, Criterions, RootPath, Context ) != rgstry::sOK )
				Level = LCL_UNDEFINED_LEVEL;

			return Level;
		}
		level__ Push(
			xtf::extended_text_iflow__ &XFlow,
			const xpp::criterions___ &Criterions,
			const char *RootPath )
		{
			level__ Level = Registry.PushEmbeddedLevel();

			if ( Registry.Fill( Level, XFlow, Criterions, RootPath ) != rgstry::sOK )
				Level = LCL_UNDEFINED_LEVEL;

			return Level;
		}
		level__ Push(
			const char *FileName,
			const xpp::criterions___ &Criterions,
			const char *RootPath,
			context___ &Context )
		{
			level__ Level = Registry.PushEmbeddedLevel();

			if ( Registry.Fill( Level, FileName, Criterions, RootPath, Context ) != rgstry::sOK )
				Level = LCL_UNDEFINED_LEVEL;

			return Level;
		}
		level__ Push(
			const char *FileName,
			const char *RootPath,
			context___ &Context )
		{
			level__ Level = Registry.PushEmbeddedLevel();

			if ( Registry.Fill( Level, FileName, xpp::criterions___(), RootPath, Context ) != rgstry::sOK )
				Level = LCL_UNDEFINED_LEVEL;

			return Level;
		}
		level__ Push(
			const char *FileName,
			const xpp::criterions___ &Criterions,
			const char *RootPath )
		{
			level__ Level = Registry.PushEmbeddedLevel();

			if ( Registry.Fill( Level, FileName, Criterions, RootPath ) != rgstry::sOK )
				Level = LCL_UNDEFINED_LEVEL;

			return Level;
		}
		level__ Push(
			const char *FileName,
			const char *RootPath )
		{
			level__ Level = Registry.PushEmbeddedLevel();

			if ( Registry.Fill( Level, FileName, xpp::criterions___(), RootPath ) != rgstry::sOK )
				Level = LCL_UNDEFINED_LEVEL;

			return Level;
		}
		void Push( const locale_ &Locale )
		{
			Registry.Push( Locale.Registry );
		}
		level__ Pop( void )
		{
			return Registry.Pop();
		}
		void GetLanguages(
			strings_ &Labels,
			strings_ &Wordings ) const;
		const str::string_  &GetTranslation(
			const meaning_ &Meaning,
			const char *Language,
			str::string_ &Translation ) const;
		const str::string_ &GetTranslation(
			const char *Text,
			const char *Language,
			str::string_ &Translation ) const	// Version simplifi�e.
		{
			_GetTranslation( str::string( Text ), Language, Translation );

			return Translation ;
		}
	};

	E_AUTO( locale );

	// Fonction utilis�e dans le cadre de l'internationalisation, qui sert juste � contr�ler l'existence du premier param�tre, qui correspond g�n�ralement � une entr�e d'un 'enume'.
	inline const char *Control_(
		int,	// Sert juste � v�rifier l'existence d'une entr�e d'un 'enum'.
		const char *Text )
	{
		return Text;
	}

// A utiliser dans une macro '_( name )', qui pr�d�fini le param�tre 'prefix'.
# define LCL_M( prefix, name ) lcl::Control_( prefix##name, #name )

}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
