/*
	Header for the 'bso' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Copyright (C) 2000-2004 Claude SIMON (http://zeusw.org/intl/contact.html).

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

#include "err.h"
#include "cpe.h"

# ifdef CPE__64BITS
#  ifndef BSO_DISABLE_64BITS
#   define BSO__64BITS_ENABLED
#  endif
# endif

# ifdef BSO_ENABLE_64BITS
#  define BSO__64BITS_ENABLED
# endif

namespace bso {
	//t Basic data, without any basically signification.
	typedef unsigned char raw__;

	//t Boolean.
	typedef bool bool__;

# if 0
	//t A portable boolean;
	typedef bool__ p_bool__;
# endif

	//d Maximal value of a 'sbyte__'.
	#define BSO_S8_MAX		SCHAR_MAX
	//d Minimal value of a 'sbyte__'.
	#define BSO_S8_MIN		SCHAR_MIN
	//d Size, in bit, of a 'sbyte__'.
	#define BSO_NB_BITS_S8	8

	//t Signed byte.
	typedef signed char	s8__;

# if 0
	//t Portable signed byte.
	typedef sbyte__ p_sbyte__;
# endif


	//d Maximal value of a 'ubyte__'.
	#define BSO_U8_MAX	UCHAR_MAX
	//d Minimal value of a 'ubyte__'.
	#define BSO_U8_MIN	0
	//d Size, in bit, of a 'ubyte__'.
	#define BSO_NB_BITS_U8	8

	//t Unsigned byte.
	typedef unsigned char u8__;

# if 0
	//t Portable unsigned byte.
	typedef ubyte__ p_ubyte__;
# endif

# if 0
	// Portable short. Internal use only.
	template <typename t> class p_short__
	{
	private:
		t Op_;
	public:
		p_short__( t Op )
		{
			raw__ *P = (raw__ *)&Op_;			

			P[0] = (raw__)( Op & 0xff );
			P[1] = (raw__)( ( Op >> 8 ) & 0xff );
		}
		operator t( void )
		{
			raw__ *P = (raw__ *)&Op_;			

			return P[0] | ( P[1] << 8 );
		}
		bool operator ==( p_short__ P ) const
		{
			return Op_ == P.Op_;
		}
		bool operator !=( p_short__ P ) const
		{
			return Op_ != P.Op_;
		}
	};
# endif


	//d Maximal value of a 'sshort__'.
	#define BSO_S16_MAX	SHRT_MAX
	//d Minimal value of a 'sshort__'.
	#define BSO_S16_MIN	SHRT_MIN
	//d Size, in bit, of a 'sshort__'.
	#define BSO_NB_BITS_S16	16

	// Signed short.
	typedef signed short s16__;

#if 0
	//c Portable signed short.
	class p_sshort__
	: public p_short__<sshort__>
	{
	public:
		p_sshort__( sshort__ Op = 0 )
		: p_short__<sshort__>( Op )
		{}
	};
# endif

	//d Maximal value of a 'ushort__'.
	#define BSO_U16_MAX	USHRT_MAX
	//d Minimal value of a 'ushort__'.
	#define BSO_U16_MIN	0
	//d Size, in bit, of a 'ushort__'.
	#define BSO_NB_BITS_U16	16

	//t Unsigned short
	typedef unsigned short u16__;

# if 0
	//c Portable unsigned short.
	class p_ushort__
	: public p_short__<ushort__>
	{
	public:
		p_ushort__( ushort__ Op = 0 )
		: p_short__<ushort__>( Op )
		{}
	};
# endif

# if 0
	// Portable 32 bits integer. Internal use only.
	template <typename t> class p_32_bits__
	{
	private:
		t Op_;
	public:
		p_32_bits__( t Op )
		{
			raw__ *P = (raw__ *)&Op_;			

			P[0] = (raw__)( Op & 0xff );
			P[1] = (raw__)( ( Op >> 8 ) & 0xff );
			P[2] = (raw__)( ( Op >> 16 ) & 0xff );
			P[3] = (raw__)( ( Op >> 24 ) & 0xff );
		}
		operator t( void )
		{
			raw__ *P = (raw__ *)&Op_;			

			return P[0] | ( P[1] << 8 ) | ( P[2] << 16 ) | ( P[3] << 24 );
		}
		bool operator ==( p_32_bits__ P ) const
		{
			return Op_ == P.Op_;
		}
		bool operator != ( p_32_bits__ P ) const
		{
			return Op_ != P.Op_;
		}
	};
# endif

# if 0
	// Portable 64 bits integer. Internal use only.
	template <typename t> class p_64_bits__
	{
	private:
		t Op_;
	public:
		p_64_bits__( t Op )
		{
			raw__ *P = (raw__ *)&Op_;			

			P[0] = (raw__)( Op & 0xff );
			P[1] = (raw__)( ( Op >> 8 ) & 0xff );
			P[2] = (raw__)( ( Op >> 16 ) & 0xff );
			P[3] = (raw__)( ( Op >> 24 ) & 0xff );
			P[4] = (raw__)( ( Op >> 32 ) & 0xff );
			P[5] = (raw__)( ( Op >> 40 ) & 0xff );
			P[6] = (raw__)( ( Op >> 48 ) & 0xff );
			P[7] = (raw__)( ( Op >> 56 ) & 0xff );
		}
		operator t( void )
		{
			raw__ *P = (raw__ *)&Op_;			

			return (unsigned long long)P[0] | ( (unsigned long long)P[1] << 8 ) | ( (unsigned long long)P[2] << 16 ) | (unsigned long long)( P[3] << 24 )
				   | ( (unsigned long long)P[4] << 32 ) | ( (unsigned long long)P[5] << 40 ) | ( (unsigned long long)P[6] << 48 ) | ( (unsigned long long)P[4] << 56 );
		}
		bool operator ==( p_64_bits__ P ) const
		{
			return Op_ == P.Op_;
		}
		bool operator != ( p_64_bits__ P ) const
		{
			return Op_ != P.Op_;
		}
	};
# endif

	//d Maximal value of a 'slong__'.
	#define BSO_S32_MAX	LONG_MAX
	//d Minimal value of a 'slong__'.
	#define BSO_S32_MIN   LONG_MIN
	//d Size, in bit, of a 'slong__'.
	#define BSO_NB_BITS_S32	32

	//t Signed long.
	typedef signed long s32__;

# if 0
	//c Portable signed long.
	class p_slong__
	: public p_32_bits__<slong__>
	{
	public:
		p_slong__( slong__ Op = 0 )
		: p_32_bits__<slong__>( Op )
		{}
	};
# endif

# ifdef CPE__64
	//d Maximal value of a 'sllong__'.
	#define BSO_S64_MAX	LLONG_MAX
	//d Minimal value of a 'sllong__'.
	#define BSO_S64_MIN   LLONG_MIN
	//d Size, in bit, of a 'slong__'.
	#define BSO_NB_BITS_S64	64

	//t Signed long.
	typedef signed long long s64__;
# endif

# if 0
	//c Portable signed long long.
	class p_sllong__
	: public p_64_bits__<sllong__>
	{
	public:
		p_sllong__( sllong__ Op = 0 )
		: p_64_bits__<sllong__>( Op )
		{}
	};
# endif

	//d Maximal value of a 'ulong__'.
	#define BSO_U32_MAX	ULONG_MAX
	//d Minimal value of a 'ulong__'.
	#define BSO_U32_MIN   0
	//d Size, in bit, of a 'ulong__'.
	#define BSO_NB_BITS_U32	32

	//t Unsigned long.
	typedef unsigned long u32__;

# if 0
	//c Portable unsigned long.
	class p_ulong__
	: public p_32_bits__<ulong__>
	{
	public:
		p_ulong__( ulong__ Op = 0 )
		: p_32_bits__<ulong__>( Op )
		{}
	};
# endif

# ifdef CPE__64
	//d Maximal value of a 'ullong__'.
	#define BSO_U64_MAX	ULLONG_MAX
	//d Minimal value of a 'ullong__'.
	#define BSO_U64_MIN   0
	//d Size, in bit, of a 'ullong__'.
	#define BSO_NB_BITS_U64	64

	//t Unsigned long.
	typedef unsigned long long u64__;
# endif 

# if 0
	//c Portable unsigned long long.
	class p_ullong__
	: public p_64_bits__<ullong__>
	{
	public:
		p_ullong__( ullong__ Op = 0 )
		: p_64_bits__<ullong__>( Op )
		{}
	};
# endif

# define BSO_SIZE_MAX	((size_t)-1)

# if CPE__64BITS
#  define BSO_SIZE_SIZE	8
# elif defined (CPE__32BITS )
#  define BSO_SIZE_SIZE	4
# else
#  error "Undefined bitness !"
# endif

	//t Size of a memory .
	typedef size_t size__;

# if 0
#  ifdef CPE__32BITS
	class p_size__
	: public p_32_bits__<size__>
	{
	public:
		p_size__( size__ Op = 0 )
		: p_32_bits__<size__>( Op )
		{}
	};
#  elif defined ( CPE__64BITS )
	class p_size__
	: public p_64_bits__<size__>
	{
	public:
		p_size__( size__ Op = 0 )
		: p_64_bits__<size__>( Op )
		{}
	};
#  else
#   error "No biteness defined !"
#  endif
# endif

	//t Short-sized float.
	typedef float sfloat__;

	//t Middle-sized float.
	typedef double float__;

#if 0	// Just a workaround to a 'gendoc' bug.
	//t Long sized float.
	typedef dummy lfloat__;
#endif

	typedef long double lfloat__;

	//t Character
	typedef char char__;

# if 0
	//t A portable character.
	typedef char__ p_char__;
# endif

	//c A sign ( -(1) +(1) 0 ).
	class sign__
	{
	private:
		s8__ Sign_;
	public:
		sign__( signed long int Sign = 0 )
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

# if 0
	//t A portable sign.
	typedef sign__ p_sign__;
# endif

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
# ifdef CPE__64BITS
	typedef u64__ nuint__;
#  define BSO_NUINT_MAX	BSO_U64_MAX
#  define BSO_NUINT_MIN	BSO_U64_MIN
# elif defined CPE__32BITS
	typedef u32__ nuint__;
#  define BSO_NUINT_MAX	BSO_U32_MAX
#  define BSO_NUINT_MIN	BSO_U32_MIN
# else
#  error "Unknown bitness !"
#endif

// 'biggest unsigned integer'
# ifdef BSO__64BITS_ALLOWED
	typedef u64__ _buint__;
# else
	typedef u32__ _buint__;
# endif

# ifdef BSO__64BITS_ENABLED
	inline const char *Convert(
		u64__ Value,
		integer_buffer__ &Buffer )
	{
		sprintf( Buffer.Datum, "%llu", Value );

		return Buffer.Datum;
	}
#  ifndef CPE__MT
	inline const char *Convert( ullong__ Value )
	{
		static integer_buffer__ Buffer;

		return Convert( Value, Buffer );
	}
#  endif
# endif


	inline const char *Convert(
		bso::u32__ Value,
		integer_buffer__ &Buffer )
	{
		sprintf( Buffer.Datum, "%lu", Value );

		return Buffer.Datum;
	}

#ifndef CPE__MT
	//f Return 'Value' as string. Valid only until next call of a 'Convert(..)' function.
	inline const char *Convert( bso::u32__ Value )
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
		return Convert( (nuint__)Value, Buffer );
	}

#ifndef CPE__MT
	//f Return 'Value' as string. Valid only until next call of a 'Convert(..)' function.
	inline const char *Convert( bso::u16__ Value )
	{
		return Convert( (nuint__)Value );
	}
#endif

	//f Return 'Value' as string in 'String'.
	inline const char *Convert(
		bso::u8__ Value,
		integer_buffer__ &Buffer )
	{
		return Convert( (nuint__)Value, Buffer );
	}

#ifndef CPE__MT
	//f Return 'Value' as string. Valid only until next call of a 'Convert(..)' function.
	inline const char *Convert( bso::u8__ Value )
	{
		return Convert( (nuint__)Value );
	}
#endif

// 'natural signed integer'
# ifdef CPE__64BITS
	typedef s64__ nsint__;
#  define BSO_NSINT_MAX	BSO_S64_MAX
#  define BSO_NSINT_MIN	BSO_S64_MIN
# elif defined CPE__32BITS
	typedef s32__ nsint__;
#  define BSO_NSINT_MAX	BSO_S32_MAX
#  define BSO_NSINT_MIN	BSO_S32_MIN
# else
#  error "Unknown bitness !"
#endif

// 'biggest signed integer'
# ifdef BSO__64BITS_ALLOWED
	typedef s64__ _bsint__;
# else
	typedef s32__ _bsint__;
# endif

# ifdef BSO__64BITS_ALLOWED 
	//f Return 'Value' as string in 'String'.
	inline const char *Convert(
		sllong__ Value,
		integer_buffer__ &Buffer )
	{
		sprintf( Buffer.Datum, "%lli", Value );

		return Buffer.Datum;
	}

# ifndef CPE__MT
	inline const char *Convert( sllong__ Value )
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
		return Convert( (nsint__)Value, Buffer );
	}

#ifndef CPE__MT
	//f Return 'Value' as string. Valid only until next call of a 'Convert(..)' function.
	inline const char *Convert( bso::s16__ Value )
	{
		return Convert( (nsint__)Value );
	}
#endif

	//f Return 'Value' as string in 'String'.
	inline const char *Convert(
		bso::s8__ Value,
		integer_buffer__ &Buffer )
	{
		return Convert( (nsint__)Value, Buffer );
	}

#ifndef CPE__MT
	//f Return 'Value' as string. Valid only until next call of a 'Convert(..)' function.
	inline const char *Convert( bso::s8__ Value )
	{
		return Convert( (nsint__)Value );
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

	inline const char *Convert(
		size__ Value,
		integer_buffer__ &Buffer )
	{
		return Convert( (nuint__)Value, Buffer );
	}

#  ifndef CPE__MT
	inline const char *Convert( size__ Value )
	{
		return Convert( (nuint__)Value );
	}
#  endif


}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
