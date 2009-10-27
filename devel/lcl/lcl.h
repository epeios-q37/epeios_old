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

	typedef ctn::E_XMCONTAINER_( str::string_ ) strings_;
	E_AUTO( strings );

	class locales_ {
	private:
		void _GetCorrespondingLabels(
			const strings_ &Labels,
			strings_ &Wordings ) const;
		bso::bool__ _GetTranslationFollowingLanguageThenMessage(
			const str::string_ &RawMessage,
			const str::string_ &Language,
			str::string_ &Translation ) const;
		bso::bool__ _GetTranslationFollowingMessageThenLanguage(
			const str::string_ &RawMessage,
			const str::string_ &Language,
			str::string_ &Translation ) const;
	public:
		struct s {
			rgstry::registry_::s Registry;
			rgstry::row__ Root;
		} &S_;
		rgstry::registry_ Registry;
		locales_( s &S )
		: S_( S ),
		  Registry( S.Registry )
		{}
		void reset( bso::bool__ P = true )
		{
			Registry.reset( P );
			S_.Root = NONE;
		}
		locales_ &operator =( const locales_ &L )
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
			xtf::extended_text_iflow__ &XTFlow,
			const char *RootPath,
			const str::string_ &BaseDirectory,
			error_details_ &ErrorDetails )
		{
			reset();

			Registry.Init();

			return rgstry::FillRegistry( XTFlow, BaseDirectory, RootPath, Registry, S_.Root, ErrorDetails );
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
		void GetLanguages(
			strings_ &Labels,
			strings_ &Wordings ) const;
		bso::bool__ GetTranslation(
			const str::string_ &RawMessage,
			const str::string_ &Language,
			str::string_ &Translation ) const;
		const char *GetTranslation(
			const str::string_ &RawMessage,
			const str::string_ &Language,
			STR_BUFFER___ &Buffer ) const;	// Si la traducation n'existe pas, 'RawMessage' est renvoyé.
	};

	E_AUTO( locales );

	void ReplaceTags(
		str::string_ &Message,
		const strings_ &Values,
		char TagMarker = LCL_DEFAULT_TAG_MARKER );

	void ReplaceTag(
		str::string_ &Message,
		bso::ubyte__ Indice,
		const str::string_ &Value,
		char TagMarker = LCL_DEFAULT_TAG_MARKER );

	inline void ReplaceTag(
		str::string_ &Message,
		bso::ubyte__ Indice,
		const char *Value,
		char TagMarker = LCL_DEFAULT_TAG_MARKER )
	{
		ReplaceTag( Message, Indice, str::string( Value ), TagMarker );
	}
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
