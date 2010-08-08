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

#include <limits.h>

#include "err.h"
#include "cpe.h"

#ifndef BSO_64_BITS_TYPES_FORBIDDEN
#	if defined( CPE__64_BITS_TYPES_ALLOWED ) || defined( BSO_64_BITS_TYPES_ALLOWED )
#		define BSO__64_BITS_TYPES_ALLOWED
#	endif
#endif


namespace bso {
	//t Basic data, without any basically signification.
	typedef unsigned char raw__;

	//t Boolean.
	typedef bool bool__;

	//t A portable boolean;
	typedef bool__ p_bool__;


	//d Maximal value of a 'sbyte__'.
	#define BSO_SBYTE_MAX		SCHAR_MAX
	//d Minimal value of a 'sbyte__'.
	#define BSO_SBYTE_MIN		SCHAR_MIN
	//d Size, in bit, of a 'sbyte__'.
	#define BSO_NB_BITS_SBYTE	8

	//t Signed byte.
	typedef signed char	sbyte__;

	//t Portable signed byte.
	typedef sbyte__ p_sbyte__;


	//d Maximal value of a 'ubyte__'.
	#define BSO_UBYTE_MAX	UCHAR_MAX
	//d Minimal value of a 'ubyte__'.
	#define BSO_UBYTE_MIN	0
	//d Size, in bit, of a 'ubyte__'.
	#define BSO_NB_BITS_UBYTE	8

	//t Unsigned byte.
	typedef unsigned char ubyte__;

	//t Portable unsigned byte.
	typedef ubyte__ p_ubyte__;

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



	//d Maximal value of a 'sshort__'.
	#define BSO_SSHORT_MAX	SHRT_MAX
	//d Minimal value of a 'sshort__'.
	#define BSO_SSHORT_MIN	SHRT_MIN
	//d Size, in bit, of a 'sshort__'.
	#define BSO_NB_BITS_SSHORT	16

	// Signed short.
	typedef signed short sshort__;

	//c Portable signed short.
	class p_sshort__
	: public p_short__<sshort__>
	{
	public:
		p_sshort__( sshort__ Op = 0 )
		: p_short__<sshort__>( Op )
		{}
	};

	//d Maximal value of a 'ushort__'.
	#define BSO_USHORT_MAX	USHRT_MAX
	//d Minimal value of a 'ushort__'.
	#define BSO_USHORT_MIN	0
	//d Size, in bit, of a 'ushort__'.
	#define BSO_NB_BITS_USHORT	16

	//t Unsigned short
	typedef unsigned short ushort__;

	//c Portable unsigned short.
	class p_ushort__
	: public p_short__<ushort__>
	{
	public:
		p_ushort__( ushort__ Op = 0 )
		: p_short__<ushort__>( Op )
		{}
	};

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


#ifdef BSO__64_BITS_TYPES_ALLOWED
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
			P[5] = (raw__)( ( Op >> 32 ) & 0xff );
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


#endif

	//d Maximal value of a 'slong__'.
	#define BSO_SLONG_MAX	LONG_MAX
	//d Minimal value of a 'slong__'.
	#define BSO_SLONG_MIN   LONG_MIN
	//d Size, in bit, of a 'slong__'.
	#define BSO_NB_BITS_SLONG	32

	//t Signed long.
	typedef signed long slong__;

	//c Portable signed long.
	class p_slong__
	: public p_32_bits__<slong__>
	{
	public:
		p_slong__( slong__ Op = 0 )
		: p_32_bits__<slong__>( Op )
		{}
	};

#ifdef BSO__64_BITS_TYPES_ALLOWED
	//d Maximal value of a 'sllong__'.
	#define BSO_SLLONG_MAX	LLONG_MAX
	//d Minimal value of a 'sllong__'.
	#define BSO_SLLONG_MIN   LLONG_MIN
	//d Size, in bit, of a 'slong__'.
	#define BSO_NB_BITS_SLLONG	64

	//t Signed long.
	typedef signed long long sllong__;

	//c Portable signed long long.
	class p_sllong__
	: public p_64_bits__<sllong__>
	{
	public:
		p_sllong__( sllong__ Op = 0 )
		: p_64_bits__<sllong__>( Op )
		{}
	};

#endif

	//d Maximal value of a 'ulong__'.
	#define BSO_ULONG_MAX	ULONG_MAX
	//d Minimal value of a 'ulong__'.
	#define BSO_ULONG_MIN   0
	//d Size, in bit, of a 'ulong__'.
	#define BSO_NB_BITS_ULONG	32

	//t Unsigned long.
	typedef unsigned long ulong__;

	//c Portable unsigned long.
	class p_ulong__
	: public p_32_bits__<ulong__>
	{
	public:
		p_ulong__( ulong__ Op = 0 )
		: p_32_bits__<ulong__>( Op )
		{}
	};

#ifdef BSO__64_BITS_TYPES_ALLOWED

	//d Maximal value of a 'ullong__'.
	#define BSO_ULLONG_MAX	ULLONG_MAX
	//d Minimal value of a 'ullong__'.
	#define BSO_ULLONG_MIN   0
	//d Size, in bit, of a 'ullong__'.
	#define BSO_NB_BITS_ULLONG	64

	//t Unsigned long.
	typedef unsigned long long ullong__;

	//c Portable unsigned long long.
	class p_ullong__
	: public p_64_bits__<ullong__>
	{
	public:
		p_ullong__( ullong__ Op = 0 )
		: p_64_bits__<ullong__>( Op )
		{}
	};

#endif

	//d Maximal value of a 'size'.
	#define BSO_SIZE_MAX	BSO_ULONG_MAX
	//d Minimal value of a 'size__'.
	#define BSO_SIZE_MIN	0

	//t Size of a memory .
	typedef bso::ulong__ size__;

	//c The portable version of a buffer size.
	class p_size__
	: public p_32_bits__<size__>
	{
	public:
		p_size__( size__ Op = 0 )
		: p_32_bits__<size__>( Op )
		{}
	};

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

	//t A portable character.
	typedef char__ p_char__;

	//c A sign ( -(1) +(1) 0 ).
	class sign__
	{
	private:
		sbyte__ Sign_;
	public:
		sign__( signed long int Sign = 0 )
		{
			Sign_ = (sbyte__)( Sign ? ( Sign > 0 ? 1 : -1 ) : 0 );
		}
		operator sbyte__( void )
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

	//t A portable sign.
	typedef sign__ p_sign__;

	//d Maximum size of an ASCII converted signed/unsigned 64 bits integer.
	#define BSO_ASCII_CONVERTED_INTEGER_MAX_SIZE	22

	struct integer_buffer__ {
		char Datum[BSO_ASCII_CONVERTED_INTEGER_MAX_SIZE+1];	// '+1' to store the terminal 'NUL' character.
	};

#ifdef BSO__64_BITS_TYPES_ALLOWED
	typedef ullong__ _guint__;	// Generic unsigned integer; can contain the biggest unsigned integer.

	inline const char *Convert(
		ullong__ Value,
		integer_buffer__ &Buffer )
	{
		sprintf( Buffer.Datum, "%llu", Value );

		return Buffer.Datum;
	}

#	ifndef CPE__T_MT
	inline const char *Convert( ullong__ Value )
	{
		static integer_buffer__ Buffer;

		return Convert( Value, Buffer );
	}
#	endif

#else
	typedef ulong__ _guint__;	// Generic unsigned integer; can contain the biggest unsigned integer.
#endif

	inline const char *Convert(
		bso::ulong__ Value,
		integer_buffer__ &Buffer )
	{
		sprintf( Buffer.Datum, "%lu", Value );

		return Buffer.Datum;
	}

#ifndef CPE__T_MT
	//f Return 'Value' as string. Valid only until next call of a 'Convert(..)' function.
	inline const char *Convert( bso::ulong__ Value )
	{
		static integer_buffer__ Buffer;

		return Convert( Value, Buffer );
	}
#endif

	//f Return 'Value' as string in 'String'.
	inline const char *Convert(
		bso::ushort__ Value,
		integer_buffer__ &Buffer )
	{
		return Convert( (_guint__)Value, Buffer );
	}

#ifndef CPE__T_MT
	//f Return 'Value' as string. Valid only until next call of a 'Convert(..)' function.
	inline const char *Convert( bso::ushort__ Value )
	{
		return Convert( (bso::ulong__)Value );
	}
#endif

	//f Return 'Value' as string in 'String'.
	inline const char *Convert(
		bso::ubyte__ Value,
		integer_buffer__ &Buffer )
	{
		return Convert( (_guint__)Value, Buffer );
	}

#ifndef CPE__T_MT
	//f Return 'Value' as string. Valid only until next call of a 'Convert(..)' function.
	inline const char *Convert( bso::ubyte__ Value )
	{
		return Convert( (bso::ulong__)Value );
	}
#endif

#ifdef BSO__64_BITS_TYPES_ALLOWED
	typedef sllong__ _gsint__;	// Generic unsigned integer; can contain the biggest unsigned integer.

	//f Return 'Value' as string in 'String'.
	inline const char *Convert(
		sllong__ Value,
		integer_buffer__ &Buffer )
	{
		sprintf( Buffer.Datum, "%lli", Value );

		return Buffer.Datum;
	}

#	ifndef CPE__T_MT
	inline const char *Convert( sllong__ Value )
	{
		static integer_buffer__ Buffer;

		return Convert( Value, Buffer );
	}
#	endif

#else
	typedef slong__ _gsint__;	// Generic unsigned integer; can contain the biggest unsigned integer.
#endif

	//f Return 'Value' as string in 'String'.
	inline const char *Convert(
		slong__ Value,
		integer_buffer__ &Buffer )
	{
		sprintf( Buffer.Datum, "%li", Value );

		return Buffer.Datum;
	}

#ifndef CPE__T_MT
	//f Return 'Value' as string. Valid only until next call of a 'Convert(..)' function.
	inline const char *Convert( bso::slong__ Value )
	{
		static integer_buffer__ Buffer;

		return Convert( Value, Buffer );
	}
#endif

	//f Return 'Value' as string in 'String'.
	inline const char *Convert(
		bso::sshort__ Value,
		integer_buffer__ &Buffer )
	{
		return Convert( (_gsint__)Value, Buffer );
	}

#ifndef CPE__T_MT
	//f Return 'Value' as string. Valid only until next call of a 'Convert(..)' function.
	inline const char *Convert( bso::sshort__ Value )
	{
		return Convert( (bso::slong__)Value );
	}
#endif

	//f Return 'Value' as string in 'String'.
	inline const char *Convert(
		bso::sbyte__ Value,
		integer_buffer__ &Buffer )
	{
		return Convert( (_gsint__)Value, Buffer );
	}

#ifndef CPE__T_MT
	//f Return 'Value' as string. Valid only until next call of a 'Convert(..)' function.
	inline const char *Convert( bso::sbyte__ Value )
	{
		return Convert( (bso::slong__)Value );
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

#ifndef CPE__T_MT
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

#ifndef CPE__T_MT
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

#ifndef CPE__T_MT
	//f Return 'Value' as string. Valid only until next call of a 'Convert(..)' function.
	inline const char *Convert( sfloat__ Value )
	{
		return Convert( (lfloat__)Value );
	}
#endif

}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
