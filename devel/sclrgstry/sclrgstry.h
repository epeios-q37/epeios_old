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

	extern const lcl::rack__ *LocaleRack;	// A définir.

	const rgstry::registry_ &GetRegistry( void );

	rgstry::row__ GetRoot( void );

	extern rgstry::entry___ Parameters;

	extern rgstry::entry___ Language;

	using rgstry::value;
	using rgstry::value_;

	void Load(
		flw::iflow__ &Flow,
		const char *Directory,
		const char *RootPath );

	bso::bool__ IsRegistryReady( void );

	bso::bool__ GetValue(
		const rgstry::tentry__ &Entry,
		str::string_ &Value );

	bso::bool__ GetValues(
		const rgstry::tentry__ &Entry,
		str::strings_ &Values );

	const str::string_ &GetOptionalRegistryValue(
		const rgstry::tentry__ &Entry,
		str::string_ &Value,
		bso::bool__ *Missing = NULL );

	const char *GetOptionalRegistryValue(
		const rgstry::tentry__ &Entry,
		STR_BUFFER___ &Buffer,
		bso::bool__ *Missing = NULL );

	const str::string_ &GetMandatoryRegistryValue(
		const rgstry::tentry__ &Entry,
		str::string_ &Value );

	const char *GetMandatoryRegistryValue(
		const rgstry::tentry__ &Entry,
		STR_BUFFER___ &Buffer );

# define SCLRGSTRY__RUN( type, name, limit )\
	type GetMandatoryRegistry##name(\
		const rgstry::tentry__ &Entry,\
		type Limit = limit );\
	type GetRegistry##name(\
		const rgstry::tentry__ &Entry,\
		type DefaultValue,\
		type Limit = limit );

# ifdef CPE__64_BITS_TYPES_ALLOWED
	SCLRGSTRY__RUN( bso::ullong__, ULLong, BSO_ULLONG_MAX )
# endif
	SCLRGSTRY__RUN( bso::ulong__, ULong, BSO_ULONG_MAX )
	SCLRGSTRY__RUN( bso::ushort__, UShort, BSO_USHORT_MAX )
	SCLRGSTRY__RUN( bso::ubyte__, UByte, BSO_UBYTE_MAX )

# define SCLRGSTRY__RSN( type, name, min, max )\
	type GetMandatoryRegistry##name(\
		const rgstry::tentry__ &Entry,\
		type Min = min,\
		type Max = max );\
	type GetRegistry##name(\
		const rgstry::tentry__ &Entry,\
		type DefaultValue,\
		type Min = min,\
		type Max = max );

# ifdef CPE__64_BITS_TYPES_ALLOWED
	SCLRGSTRY__RSN( bso::sllong__, SLLong, BSO_SLLONG_MIN, BSO_SLLONG_MAX )
#endif
	SCLRGSTRY__RSN( bso::slong__, SLong, BSO_SLONG_MIN, BSO_SLONG_MAX )
	SCLRGSTRY__RSN( bso::sshort__, SShort, BSO_SSHORT_MIN, BSO_SSHORT_MAX )
	SCLRGSTRY__RSN( bso::sbyte__, SByte, BSO_SBYTE_MIN, BSO_SBYTE_MAX )

// To define function retrieving mandatory registry value.
# define SCLRGSTRY_MRV( name, entry )\
	inline const char *name(\
		STR_BUFFER___ &Buffer )\
	{\
		return GetMandatoryRegistryValue( entry, Buffer );\
	}\
	inline const str::string_ &name(\
		str::string_ &Value )\
	{\
		return GetMandatoryRegistryValue( entry, Value );\
	}

// To define function retrieving optional registry value.
# define SCLRGSTRY_ORV( name, entry )\
	inline const char *name(\
		STR_BUFFER___ &Buffer,\
		bso::bool__ *Missing = NULL )\
	{\
		return GetOptionalRegistryValue( entry, Buffer, Missing );\
	}\
	inline const str::string_ &name(\
		str::string_ &Value,\
		bso::bool__ *Missing = NULL )\
	{\
		return GetOptionalRegistryValue( entry, Value, Missing );\
	}
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
