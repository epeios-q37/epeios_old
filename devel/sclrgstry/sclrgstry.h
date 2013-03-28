/*
	Header for the 'sclrgstry' library by Claude SIMON (csimon at zeusw dot org)
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

#ifndef SCLRGSTRY__INC
#define SCLRGSTRY__INC

#define SCLRGSTRY_NAME		"SCLRGSTRY"

#define	SCLRGSTRY_VERSION	"$Revision$"

#define SCLRGSTRY_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &SCLRGSTRYTutor;

#if defined( XXX_DBG ) && !defined( SCLRGSTRY_NODBG )
#define SCLRGSTRY_DBG
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
//D SoCLe ReGiSTRY 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# include "err.h"
# include "flw.h"

# include "rgstry.h"

# define SCLREGSITRY_TAG_MARKER_STRING	"%"
# define SCLREGSITRY_TAG_MARKER_CHAR	'%'

namespace sclrgstry {

	using rgstry::tags_;
	using rgstry::tags;

//	extern const lcl::rack__ *LocaleRack;	// A définir.

	const rgstry::registry_ &GetRegistry( void );

	rgstry::row__ GetRoot( void );

	extern rgstry::entry___ Parameters;

	extern rgstry::entry___ Language;

	extern rgstry::entry___ Locale;	// Du fichier de configuration uniquement.

	using rgstry::value;
	using rgstry::value_;

	void Load(
		flw::iflow__ &Flow,
		const char *Directory,
		const char *RootPath );

	bso::bool__ IsReady( void );

	void ReportBadOrNoValueForEntryError( const rgstry::tentry__ &Entry );


	bso::bool__ GetValue(
		const rgstry::tentry__ &Entry,
		str::string_ &Value );

	bso::bool__ GetValues(
		const rgstry::tentry__ &Entry,
		str::strings_ &Values );

	const str::string_ &GetOptionalValue(
		const rgstry::tentry__ &Entry,
		str::string_ &Value,
		bso::bool__ *Missing = NULL );

	const char *GetOptionalValue(
		const rgstry::tentry__ &Entry,
		STR_BUFFER___ &Buffer,
		bso::bool__ *Missing = NULL );

	const str::string_ &GetMandatoryValue(
		const rgstry::tentry__ &Entry,
		str::string_ &Value );

	const char *GetMandatoryValue(
		const rgstry::tentry__ &Entry,
		STR_BUFFER___ &Buffer );

# define SCLRGSTRY__UN( type, name, limit )\
	type GetMandatory##name(\
		const rgstry::tentry__ &Entry,\
		type Limit = limit );\
	type Get##name(\
		const rgstry::tentry__ &Entry,\
		type DefaultValue,\
		type Limit = limit );

# ifdef BSO__64BITS_ENABLED
	SCLRGSTRY__UN( bso::u64__, U64, BSO_U64_MAX )
# endif
	SCLRGSTRY__UN( bso::u_32__, U32, BSO_U_32_MAX )
	SCLRGSTRY__UN( bso::u16__, U16, BSO_U16_MAX )
	SCLRGSTRY__UN( bso::u8__, U8, BSO_U8_MAX )

# define SCLRGSTRY__SN( type, name, min, max )\
	type GetMandatory##name(\
		const rgstry::tentry__ &Entry,\
		type Min = min,\
		type Max = max );\
	type Get##name(\
		const rgstry::tentry__ &Entry,\
		type DefaultValue,\
		type Min = min,\
		type Max = max );

# ifdef BSO__64BITS_ENABLED
	SCLRGSTRY__SN( bso::s64__, S64, BSO_S64, BSO_S64_MAX )
#endif
	SCLRGSTRY__SN( bso::s32__, S32, BSO_S32_MIN, BSO_S32_MAX )
	SCLRGSTRY__SN( bso::s16__, S16, BSO_S16_MIN, BSO_S16_MAX )
	SCLRGSTRY__SN( bso::s8__, S8, BSO_S8_MIN, BSO_S8_MAX )

// To define function retrieving mandatory registry value.
# define SCLRGSTRY_MV( name, entry )\
	inline const char *name(\
		STR_BUFFER___ &Buffer )\
	{\
		return GetMandatoryValue( entry, Buffer );\
	}\
	inline const str::string_ &name(\
		str::string_ &Value )\
	{\
		return GetMandatoryValue( entry, Value );\
	}

// To define function retrieving optional registry value.
# define SCLRGSTRY_OV( name, entry )\
	inline const char *name(\
		STR_BUFFER___ &Buffer,\
		bso::bool__ *Missing = NULL )\
	{\
		return GetOptionalValue( entry, Buffer, Missing );\
	}\
	inline const str::string_ &name(\
		str::string_ &Value,\
		bso::bool__ *Missing = NULL )\
	{\
		return GetOptionalValue( entry, Value, Missing );\
	}
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
