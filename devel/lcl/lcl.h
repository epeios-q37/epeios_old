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

#include "err.h"
#include "flw.h"
#include "rgstry.h"

#define LCL_DEFAULT_TAG_MARKER	'%'

namespace lcl {
	using rgstry::error__;
	using rgstry::error_details_;
	using rgstry::error_details;
	using str::strings_;
	using str::strings;

	class locale_ {
	private:
		void _GetCorrespondingLabels(
			const strings_ &Labels,
			strings_ &Wordings ) const;
		// A des fins de compatibilité ascendente.
		bso::bool__ _GetTranslationFollowingLanguageThenMessage(
			const str::string_ &Text,
			const str::string_ &Language,
			str::string_ &Translation ) const;
		// A des fins de compatibilité ascendente.
		bso::bool__ _GetTranslationFollowingMessageThenLanguage(
			const str::string_ &Text,
			const str::string_ &Language,
			str::string_ &Translation ) const;
		bso::bool__ _GetTranslationFollowingLanguageThenText(
			const str::string_ &Text,
			const str::string_ &Language,
			str::string_ &Translation ) const;
		bso::bool__ _GetTranslationFollowingTextThenLanguage(
			const str::string_ &Text,
			const str::string_ &Language,
			str::string_ &Translation ) const;
	public:
		struct s {
			rgstry::registry_::s Registry;
			rgstry::row__ Root;
		} &S_;
		rgstry::registry_ Registry;
		locale_( s &S )
		: S_( S ),
		  Registry( S.Registry )
		{}
		void reset( bso::bool__ P = true )
		{
			Registry.reset( P );
			S_.Root = NONE;
		}
		locale_ &operator =( const locale_ &L )
		{
			S_.Root = L.S_.Root;

			Registry = L.Registry;

			return *this;
		}
		void Init( void )
		{
			reset();

			Registry.Init();
		}
		error__ Init(
			flw::iflow__ &IFlow,
			const char *RootPath,
			const str::string_ &BaseDirectory,
			error_details_ &ErrorDetails )
		{
			reset();

			Registry.Init();

			return rgstry::FillRegistry( IFlow, BaseDirectory, RootPath, Registry, S_.Root, ErrorDetails );
		}
		error__ Init(
			flw::iflow__ &IFlow,
			const char *RootPath,
			const str::string_ &BaseDirectory = str::string( "" ) )
		{
			reset();

			Registry.Init();

			return rgstry::FillRegistry( IFlow, RootPath, Registry, S_.Root, BaseDirectory );
		}
		error__ Init(
			const char *FileName,
			const char *RootPath,
			error_details_ &ErrorDetails )
		{
			reset();

			Registry.Init();

			return rgstry::FillRegistry( FileName, RootPath, Registry, S_.Root, ErrorDetails );
		}
		error__ Init(
			const char *FileName,
			const char *RootPath )
		{
			reset();

			Registry.Init();

			return rgstry::FillRegistry( FileName, RootPath, Registry, S_.Root );
		}
		void GetLanguages(
			strings_ &Labels,
			strings_ &Wordings ) const;
		bso::bool__ GetTranslation(
			const str::string_ &Text,
			const str::string_ &Language,
			str::string_ &Translation ) const;
		const char *GetTranslation(
			const str::string_ &Text,
			const str::string_ &Language,
			STR_BUFFER___ &Buffer ) const;	// Si la traduction n'existe pas, 'Text' est renvoyé.
	};

	E_AUTO( locale );

	class locale_rack___ {
	private:
		const locale_ *_Locale;
		str::string _Language;
	public:
		void reset( bso::bool__ P = true )
		{
			_Locale = NULL;
			_Language.reset( P );
		}
		locale_rack___( void )
		{
			reset( false );
		}
		~locale_rack___( void )
		{
			reset();
		}
		void Init(
			const locale_ &Locale,	// N'est pas dupliqué !.
			const str::string_ &Language = str::string() )
		{
			_Locale = &Locale;

			_Language.Init( Language );
		}
		const locale_ &Locale( void ) const
		{
			if ( _Locale == NULL )
				ERRu();

			return *_Locale;
		}
		E_RWDISCLOSE__( str::string_, Language );
		bso::bool__ GetTranslation(
			const str::string_ &Text,
			str::string_ &Translation ) const
		{
			return Locale().GetTranslation( Text, Language(), Translation );
		}
		const char *GetTranslation(
			const str::string_ Text,
			STR_BUFFER___ &Buffer ) const	// Si la traduction n'existe pas, 'Text' est renvoyé.
		{
			return Locale().GetTranslation( Text, Language(), Buffer );
		}
	};


	inline void ReplaceTags(
		str::string_ &Text,
		const strings_ &Values,
		char TagMarker = LCL_DEFAULT_TAG_MARKER )
	{
		str::ReplaceTags( Text, Values, TagMarker );
	}

	inline void ReplaceTag(
		str::string_ &Text,
		bso::ubyte__ Indice,
		const str::string_ &Value,
		char TagMarker = LCL_DEFAULT_TAG_MARKER )
	{
		str::ReplaceTag( Text, Indice, Value, TagMarker );
	}
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
