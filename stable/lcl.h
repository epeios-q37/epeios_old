/*
	Header for the 'lcl' library by Claude SIMON (csimon at zeusw dot org)
	Copyright (C) 20092004 Claude SIMON.

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

#define LCL_TAG_MARKER_C	'%'
#define LCL_TAG_MARKER_S	"%"

#	define LCL_DEFAULT_FILENAME_SUFFIX	".xlcl"

// Facilite la gestion des messages.
#define LCL_CASE( label, prefix )\
	case prefix##label:\
	return #label;\
	break;

namespace lcl {
	using rgstry::status__;
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
			const char *Text,
			const char *Language,
			const char *Prefix,
			str::string_ &Translation ) const;
		// A des fins de compatibilité ascendente.
		bso::bool__ _GetTranslationFollowingMessageThenLanguage(
			const char *Text,
			const char *Language,
			const char *Prefix,
			str::string_ &Translation ) const;
		bso::bool__ _GetTranslationFollowingLanguageThenText(
			const char *Text,
			const char *Language,
			const char *Prefix,
			str::string_ &Translation ) const;
		bso::bool__ _GetTranslationFollowingTextThenLanguage(
			const char *Text,
			const char *Language,
			const char *Prefix,
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
		status__ Init(
			flw::iflow__ &IFlow,
			const char *RootPath,
			const str::string_ &BaseDirectory,
			error_details_ &ErrorDetails )
		{
			reset();

			Registry.Init();

			return rgstry::FillRegistry( IFlow, BaseDirectory, RootPath, NULL, Registry, S_.Root, ErrorDetails );
		}
		status__ Init(
			flw::iflow__ &IFlow,
			const char *RootPath,
			const str::string_ &BaseDirectory = str::string( "" ) )
		{
			reset();

			Registry.Init();

			return rgstry::FillRegistry( IFlow, RootPath, NULL, Registry, S_.Root, BaseDirectory );
		}
		status__ Init(
			const char *FileName,
			const char *RootPath,
			error_details_ &ErrorDetails )
		{
			reset();

			Registry.Init();

			return rgstry::FillRegistry( FileName, RootPath, NULL, Registry, S_.Root, ErrorDetails );
		}
		status__ Init(
			const char *FileName,
			const char *RootPath )
		{
			reset();

			Registry.Init();

			return rgstry::FillRegistry( FileName, RootPath, NULL, Registry, S_.Root );
		}
		void GetLanguages(
			strings_ &Labels,
			strings_ &Wordings ) const;
		bso::bool__ GetTranslation(
			const char *Text,
			const char *Language,
			const char *Prefix,
			str::string_ &Translation ) const;
		const char *GetTranslation(
			const char *Text,
			const char *Language,
			const char *Prefix,
			STR_BUFFER___ &Buffer ) const;	// Si la traduction n'existe pas, 'Text' est renvoyé.
	};

	E_AUTO( locale );

	struct rack__
	{
	public:
		const locale_ *Locale;
		const char *Language;
		void reset( bso::bool__ = true )
		{
			Locale = NULL;
			Language = NULL;
		}
		rack__( void )
		{
			reset();
		}
		rack__(
			const locale_ &Locale,	// N'est pas dupliqué !
			const char *Language )	// N'est pas dupliqué !
		{
			reset( false );

			this->Locale = &Locale;
			this->Language = Language;
		}
		void Init(
			const locale_ &Locale,	// N'est pas dupliqué !
			const char *Language ) // N'est pas dupliqué !
		{
			reset();

			this->Locale = &Locale;
			this->Language = Language;
		}
		bso::bool__ GetTranslation(
			const char *Text,
			const char *Prefix,
			str::string_ &Translation ) const
		{
			return Locale->GetTranslation( Text, Language, Prefix, Translation );
		}
		const char *GetTranslation(
			const char *Text,
			const char *Prefix,
			STR_BUFFER___ &Buffer ) const	// Si la traduction n'existe pas, 'Text' est renvoyé.
		{
			return Locale->GetTranslation( Text, Language, Prefix, Buffer );
		}
	};


	inline void ReplaceTags(
		str::string_ &Text,
		const strings_ &Values )
	{
		str::ReplaceTags( Text, Values, LCL_TAG_MARKER_C );
	}

	inline void ReplaceTag(
		str::string_ &Text,
		bso::ubyte__ Indice,
		const str::string_ &Value )
	{
		str::ReplaceTag( Text, Indice, Value, LCL_TAG_MARKER_C );
	}
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
