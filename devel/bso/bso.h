/*
	Header for the 'bso' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Copyright (C) $COPYRIGHT_DATES$Claude SIMON (http://zeusw.org/intl/contact.html).
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

#ifndef BSO__INC
#define BSO__INC

#define BSO_NAME		"BSO"

#define	BSO_VERSION	"$Revision$"

#define BSO_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"

#include "ttr.h"

extern class ttr_tutor &BSOTutor;

#if defined( XXX_DBG ) && !defined( BSO_NODBG )
#define BSO_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision$
//C Claude SIMON (http://zeusw.org/intl/contact.html)
//R $Date$

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D BaSic Objects 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# include <limits.h>
# include <stdint.h>
# include <float.h>

# include "err.h"
# include "cpe.h"
# include "strng.h"

# ifdef CPE_64BITS
#  ifndef E_DISABLE_64BITS
#   define BSO_64BITS_ENABLED
#  endif
# endif

# ifdef E_ENABLE_64BITS
#  define BSO_64BITS_ENABLED
# endif

namespace bso {
	//t Basic data, without any basically signification.
	typedef unsigned char raw__;

	//t Boolean.
	typedef bool bool__;


	//d Maximal value of a 'sbyte__'.
	#define BSO_S8_MAX		SCHAR_MAX
	//d Minimal value of a 'sbyte__'.
	#define BSO_S8_MIN		SCHAR_MIN
	//d Size, in bit, of a 'sbyte__'.
	#define BSO_S8_SIZE		1

	//t Signed byte.
	typedef signed char	s8__;


	//d Maximal value of a 'ubyte__'.
	#define BSO_U8_MAX		UCHAR_MAX
	//d Minimal value of a 'ubyte__'.
	#define BSO_U8_MIN		0
	//d Size, in bit, of a 'ubyte__'.
	#define BSO_U8_SIZE		1

	//t Unsigned byte.
	typedef unsigned char u8__;


	//d Maximal value of a 'sshort__'.
	#define BSO_S16_MAX		SHRT_MAX
	//d Minimal value of a 'sshort__'.
	#define BSO_S16_MIN		SHRT_MIN
	//d Size, in bit, of a 'sshort__'.
	#define BSO_S16_SIZE	2

	// Signed short.
	typedef signed short s16__;

	
	//d Maximal value of a 'ushort__'.
	#define BSO_U16_MAX		USHRT_MAX
	//d Minimal value of a 'ushort__'.
	#define BSO_U16_MIN		0
	//d Size, in bit, of a 'ushort__'.
	#define BSO_U16_SIZE	2

	//t Unsigned short
	typedef unsigned short u16__;


	//d Maximal value of a 'slong__'.
	#define BSO_S32_MAX		LONG_MAX
	//d Minimal value of a 'slong__'.
	#define BSO_S32_MIN		LONG_MIN
	//d Size, in bit, of a 'slong__'.
	#define BSO_S32_SIZE	4

	//t Signed long.
	typedef signed long s32__;


	//d Maximal value of a 'ulong__'.
	#define BSO_U_32_MAX	ULONG_MAX
	//d Minimal value of a 'ulong__'.
	#define BSO_U_32_MIN   0
	//d Size, in bit, of a 'ulong__'.
	#define BSO_U_32_SIZE	4

	//t Unsigned long.
	typedef unsigned long u_32__;


# ifdef BSO_64BITS_ENABLED
	//d Maximal value of a 'sllong__'.
	#define BSO_S64_MAX		LLONG_MAX
	//d Minimal value of a 'sllong__'.
	#define BSO_S64_MIN		LLONG_MIN
	//d Size, in bit, of a 'slong__'.
	#define BSO_S64_SIZE	8

	//t Signed long.
	typedef signed long long s64__;
# endif

# ifdef BSO_64BITS_ENABLED
	//d Maximal value of a 'ullong__'.
	#define BSO_U64_MAX		ULLONG_MAX
	//d Minimal value of a 'ullong__'.
	#define BSO_U64_MIN		0
	//d Size, in bit, of a 'ullong__'.
	#define BSO_U64_SIZE	8

	//t Unsigned long.
	typedef unsigned long long u64__;
# endif 


# define BSO_SIZE_MAX	((size_t)-1)

# ifdef CPE_64BITS
#  define BSO_SIZE_SIZE	8
# elif defined (CPE_32BITS )
#  define BSO_SIZE_SIZE	4
# else
#  error "Undefined bitness !"
# endif

	//t Size of a memory .
	typedef size_t size__;

	//t Short-sized float.
	typedef float sfloat__;
# define BSO_SFLOAT_MIN	FLT_MIN
# define BSO_SFLOAT_MAX	FLT_MAX

	//t Middle-sized float.
	typedef double float__;
# define BSO_FLOAT_MIN	DBL_MIN
# define BSO_FLOAT_MAX	DBL_MAX

	typedef long double lfloat__;
# define BSO_LFLOAT_MIN	LDBL_MIN
# define BSO_LFLOAT_MAX	LDBL_MAX

	//t Character
	typedef char char__;

	typedef void *pointer__;

# if BSO_SIZE_SIZE > 8
#  error "Change below !"
# endif

	//d Maximum size of an ASCII converted pointer.
	#define BSO_ASCII_CONVERTED_POINTER_MAX_SIZE	25

	struct pointer_buffer__ {
		char Datum[BSO_ASCII_CONVERTED_POINTER_MAX_SIZE+1];	// '+1' to store the terminal 'NUL' character.
	};

	inline const char *Convert(
		pointer__ Value,
		pointer_buffer__ &Buffer )
	{
		sprintf( Buffer.Datum, "%p", Value );

		return Buffer.Datum;
	}


	//d Maximum size of an ASCII converted signed/unsigned 64 bits integer.
	#define BSO_ASCII_CONVERTED_INTEGER_MAX_SIZE	22

	struct integer_buffer__ {
		char Datum[BSO_ASCII_CONVERTED_INTEGER_MAX_SIZE+1];	// '+1' to store the terminal 'NUL' character.
	};

// 'natural unsigned integer'
# ifdef CPE_64BITS
	typedef u64__ uint__;
#  define BSO_UINT_MAX		BSO_U64_MAX
#  define BSO_UINT_MIN		BSO_U64_MIN
#  define BSO_UINT_SIZE		BSO_U64_SIZE
# elif defined CPE_32BITS
	typedef u_32__ uint__;
#  define BSO_UINT_MAX		BSO_U_32_MAX
#  define BSO_UINT_MIN		BSO_U_32_MIN
#  define BSO_UINT_SIZE		BSO_U_32_SIZE
# else
#  error "Unknown bitness !"
#endif


# ifdef BSO_64BITS_ENABLED
	inline const char *Convert(
		u64__ Value,
		integer_buffer__ &Buffer )
	{
		sprintf( Buffer.Datum, "%llu", Value );

		return Buffer.Datum;
	}
#  ifndef CPE__MT
	inline const char *Convert( u64__ Value )
	{
		static integer_buffer__ Buffer;

		return Convert( Value, Buffer );
	}
#  endif
# endif


	inline const char *Convert(
		bso::u_32__ Value,
		integer_buffer__ &Buffer )
	{
		sprintf( Buffer.Datum, "%lu", Value );

		return Buffer.Datum;
	}

#ifndef CPE__MT
	//f Return 'Value' as string. Valid only until next call of a 'Convert(..)' function.
	inline const char *Convert( bso::u_32__ Value )
	{
		static integer_buffer__ Buffer;

		return Convert( Value, Buffer );
	}
#endif

	//f Return 'Value' as string in 'String'.
	inline const char *Convert(
		bso::u16__ Value,
		integer_buffer__ &Buffer )
	{
		return Convert( (uint__)Value, Buffer );
	}

#ifndef CPE__MT
	//f Return 'Value' as string. Valid only until next call of a 'Convert(..)' function.
	inline const char *Convert( bso::u16__ Value )
	{
		return Convert( (uint__)Value );
	}
#endif

	//f Return 'Value' as string in 'String'.
	inline const char *Convert(
		bso::u8__ Value,
		integer_buffer__ &Buffer )
	{
		return Convert( (uint__)Value, Buffer );
	}

#ifndef CPE__MT
	//f Return 'Value' as string. Valid only until next call of a 'Convert(..)' function.
	inline const char *Convert( bso::u8__ Value )
	{
		return Convert( (uint__)Value );
	}
#endif

// 'natural signed integer'
# ifdef CPE_64BITS
	typedef s64__ sint__;
#  define BSO_SINT_MAX		BSO_S64_MAX
#  define BSO_SINT_MIN		BSO_S64_MIN
#  define BSO_SINT_SIZE		BSO_S64_SIZE
# elif defined CPE_32BITS
	typedef s32__ sint__;
#  define BSO_SINT_MAX		BSO_S32_MAX
#  define BSO_SINT_MIN		BSO_S32_MIN
#  define BSO_SINT_SIZE		BSO_S32_SIZE
# else
#  error "Unknown bitness !"
#endif

// 'biggest signed integer'
# ifdef BSO_64BITS_ENABLED
	typedef s64__ _bsint__;
# else
	typedef s32__ _bsint__;
# endif

# if BSO_SINT_SIZE != BSO_UINT_SIZE
#  error "'sint__' and 'uint__' should be the same size !"
# endif
	// Entier générique, comme facilité.
	typedef uint__ int__;
# define BSO_INT_MAX		BSO_UINT_MAX
# define BSO_INT_MIN		BSO_UINT_MIN
# define BSO_INT_SIZE		BSO_UINT_SIZE

	//c A sign ( -(1) +(1) 0 ).
	class sign__
	{
	private:
		s8__ Sign_;
	public:
		sign__( sint__ Sign = 0 )
		{
			Sign_ = (s8__)( Sign ? ( Sign > 0 ? 1 : -1 ) : 0 );
		}
		operator s8__( void )
		{
			return Sign_;
		}
	};

	// Retourne le signe ce 'V1' - 'V2'.
	template <typename t> inline bso::sign__ Compare(
		const t &V1,
		const t &V2 )
	{
		if ( V1 < V2 )
			return -1;
		else if ( V1 == V2 )
			return 0;
		else
			return 1;
	}



# ifdef BSO_64BITS_ENABLED 
	//f Return 'Value' as string in 'String'.
	inline const char *Convert(
		s64__ Value,
		integer_buffer__ &Buffer )
	{
		sprintf( Buffer.Datum, "%lli", Value );

		return Buffer.Datum;
	}

# ifndef CPE__MT
	inline const char *Convert( s64__ Value )
	{
		static integer_buffer__ Buffer;

		return Convert( Value, Buffer );
	}
# endif
# endif


	//f Return 'Value' as string in 'String'.
	inline const char *Convert(
		s32__ Value,
		integer_buffer__ &Buffer )
	{
		sprintf( Buffer.Datum, "%li", Value );

		return Buffer.Datum;
	}

#ifndef CPE__MT
	//f Return 'Value' as string. Valid only until next call of a 'Convert(..)' function.
	inline const char *Convert( bso::s32__ Value )
	{
		static integer_buffer__ Buffer;

		return Convert( Value, Buffer );
	}
#endif

	//f Return 'Value' as string in 'String'.
	inline const char *Convert(
		bso::s16__ Value,
		integer_buffer__ &Buffer )
	{
		return Convert( (sint__)Value, Buffer );
	}

#ifndef CPE__MT
	//f Return 'Value' as string. Valid only until next call of a 'Convert(..)' function.
	inline const char *Convert( bso::s16__ Value )
	{
		return Convert( (sint__)Value );
	}
#endif

	//f Return 'Value' as string in 'String'.
	inline const char *Convert(
		bso::s8__ Value,
		integer_buffer__ &Buffer )
	{
		return Convert( (sint__)Value, Buffer );
	}

#ifndef CPE__MT
	//f Return 'Value' as string. Valid only until next call of a 'Convert(..)' function.
	inline const char *Convert( bso::s8__ Value )
	{
		return Convert( (sint__)Value );
	}
#endif

	//d Maximum size of an ASCII converted float.
	#define BSO_ASCII_CONVERTED_FLOAT_MAX_SIZE	40

	struct float_buffer__ {
		char Datum[BSO_ASCII_CONVERTED_FLOAT_MAX_SIZE+1];	// '+1' to store terminal NUL character.
	};
	//f Return 'Value' as string in 'String'.
	inline const char *Convert(
		lfloat__ Value,
		float_buffer__ &Buffer )
	{
		sprintf( Buffer.Datum, "%Lg", Value );

		return Buffer.Datum;
	}

#ifndef CPE__MT
	//f Return 'Value' as string. Valid only until next call of a 'Convert(..)' function.
	inline const char *Convert( lfloat__ Value )
	{
		static float_buffer__ Buffer;

		return Convert( Value, Buffer );
	}
#endif

	//f Return 'Value' as string in 'String'.
	inline const char *Convert(
		float__ Value,
		float_buffer__ &Buffer )
	{
		return Convert( (lfloat__)Value, Buffer );
	}

#ifndef CPE__MT
	//f Return 'Value' as string. Valid only until next call of a 'Convert(..)' function.
	inline const char *Convert( float__ Value )
	{
		return Convert( (lfloat__)Value );
	}
#endif

	//f Return 'Value' as string in 'String'.
	inline const char *Convert(
		sfloat__ Value,
		float_buffer__ &Buffer )
	{
		return Convert( (lfloat__)Value, Buffer );
	}

#ifndef CPE__MT
	//f Return 'Value' as string. Valid only until next call of a 'Convert(..)' function.
	inline const char *Convert( sfloat__ Value )
	{
		return Convert( (lfloat__)Value );
	}
#endif

# if !defined( CPE_64BITS	) && !defined( CPE_MAC ) // Sinon conflit !
	inline const char *Convert(
		size__ Value,
		integer_buffer__ &Buffer )
	{
		return Convert( (uint__)Value, Buffer );
	}

#  ifndef CPE_MT
	inline const char *Convert( size__ Value )
	{
		return Convert( (uint__)Value );
	}
#  endif
# endif


# define BSO__DINT_SIZE_MAX ( ( ( 8 * sizeof( bso::int__ ) ) / 7 ) + 1 )

	typedef raw__ dint__[BSO__DINT_SIZE_MAX];

	typedef bso::u8__ length__;
# define SDRM__LENGTH_MAX BSO_UBYTE_MAX

	struct xint__ _ConvertToDInt( int__ Int );	//Avec '_', pour éviter des problèmes d'ambiguïté ('int__' <=> 'uint__').

	struct xint__ {
	private:
		dint__ _Int;
		length__ _Length;
	public:
		const raw__ *DSizeBuffer( void ) const
		{
			if ( _Length == 0 )
				ERRc();

			return _Int + BSO__DINT_SIZE_MAX - _Length;
		}
		length__ BufferSize( void ) const
		{
			return _Length;
		}
		void reset( bso::bool__ = true )
		{
			memset( _Int, 0, sizeof( _Int ) );
			_Length = 0;
		}
		xint__( void )
		{
			reset( false );
		}
		~xint__( void )
		{
			reset();
		}
		friend xint__ _ConvertToDInt( int__ Int );
	};

	inline xint__ ConvertToDInt( uint__ UInt )
	{
		return _ConvertToDInt( UInt );
	}

	inline xint__ ConvertToDInt( sint__ SInt )
	{
		return _ConvertToDInt( ( SInt < 0 ? 1 : 0 ) | ( SInt << ( BSO_INT_SIZE * 8 - 1 ) ) );
	}

	int__ ConvertToInt(
		const raw__ *DInt,
		size__ &Length = *(size__ *)NULL );

	inline uint__ ConvertToUInt(
		const raw__ *DInt,
		size__ &Length = *(size__ *)NULL )
	{
		return ConvertToInt( DInt, Length );
	}

	inline sint__ ConvertToSInt(
		const raw__ *DInt,
		size__ &Length = *(size__ *)NULL )
	{
		int__ Int = ConvertToInt( DInt, Length );
		sign__ Sign = ( Int & 1 ? -1 : 1 );

		Int >>= BSO_INT_SIZE * 8 - 1;

		switch ( Sign ) {
		case 1:
			return Int;
			break;
		case 0:
			ERRc();
			break;
		case -1:
			return -(sint__)Int;
			break;
		default:
			ERRc();
			break;
		}

		return 0;	// Pour éviter un 'warning'.
	}
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
