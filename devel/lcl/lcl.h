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

	E_ROW( row__ );

	typedef bso::ubyte__ _level__;

	typedef stk::E_BSTACKt_( _level__, row__ ) _levels_;
	E_AUTO( _levels );


	typedef stk::E_XMCSTACKt_( str::string_, row__ ) _values_;
	E_AUTO( _values );

	class meaning_
	{
	private:
		void _Push(
			_level__ Level,
			const str::string_ &Value )
		{
			if ( Level == BSO_UBYTE_MAX )
				ERRl();

			if ( Levels.Push( Level ) != Values.Push( Value ) )
				ERRc();
		}
	public:
		struct s {
			_levels_::s Levels;
			_values_::s Values;
		};
		_levels_ Levels;
		_values_ Values;
		meaning_( s &S )
		: Levels( S.Levels ),
		  Values( S.Values )
		{}
		void reset( bso::bool__ P )
		{
			Levels.reset( P );
			Values.reset( P );
		}
		void plug( mmm::E_MULTIMEMORY_ &MM )
		{
			Levels.plug( MM );
			Values.plug( MM );
		}
		meaning_ &operator =( const meaning_ &M )
		{
			Levels = M.Levels;
			Values = M.Values;

			return *this;
		}
		void Init( void )
		{
			Levels.Init();
			Values.Init();
		}
		void SetValue( const str::string_ &Value )
		{
			if ( !Levels.IsEmpty() )
				ERRc();

			_Push( 0, Value );
		}
		void SetValue( const char *Value )
		{
			SetValue( str::string( Value ) );
		}
		void AddTag( const str::string_ &Value )
		{
			if ( Levels.IsEmpty() )
				ERRc();

			_Push( 1, Value );
		}
		void AddTag( const char *Value )
		{
			AddTag( str::string( Value ) );
		}
		void AddTag( const meaning_ &Meaning );
		bso::bool__ IsEmpty( void ) const
		{
			return Levels.Amount() == 0;
		}
	};

	E_AUTO( meaning );


	class locale_ {
	private:
		void _GetCorrespondingLabels(
			const strings_ &Labels,
			strings_ &Wordings ) const;
		// A des fins de compatibilité ascendente.
		bso::bool__ _GetTranslationFollowingLanguageThenMessage(
			const str::string_ &Text,
			const char *Language,
			str::string_ &Translation ) const;
		// A des fins de compatibilité ascendente.
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
		void _GetTranslation(
			_levels_ &Levels,
			_values_ &Values,
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
			Registry.Pop();
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
			str::string_ &Translation ) const	// Version simplifiée.
		{
			_GetTranslation( str::string( Text ), Language, Translation );

			return Translation ;
		}
	};

	E_AUTO( locale );
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
