/*
	Header for the 'bso' library by Claude SIMON (csimon@epeios.org)
	Copyright (C) $COPYRIGHT_DATES$Claude SIMON (csimon@epeios.org).
$_RAW_$
	This file is part of the Epeios (http://epeios.org/) project.

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

#ifndef BSO__INC
#define BSO__INC

#define BSO_NAME		"BSO"

#define	BSO_VERSION	"$Revision$"

#define BSO_OWNER		"Claude SIMON (csimon@epeios.org)"

#include "ttr.h"

extern class ttr_tutor &BSOTutor;

#if defined( XXX_DBG ) && !defined( BSO_NODBG )
#define BSO_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision$
//C Claude SIMON (csimon@epeios.org)
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

#include "err.h"
#include "txf.h"


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

	// Portable long. Internal use only.
	template <typename t> class p_long__
	{
	private:
		t Op_;
	public:
		p_long__( t Op )
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
	};



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
	: public p_long__<slong__>
	{
	public:
		p_slong__( slong__ Op = 0 )
		: p_long__<slong__>( Op )
		{}
	};

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
	: public p_long__<ulong__>
	{
	public:
		p_ulong__( ulong__ Op = 0 )
		: p_long__<ulong__>( Op )
		{}
	};

	//d Maximal value of a 'size__'.
	#define BSO_BSIZE_MAX	UINT_MAX
	//d Minimal value of a 'size__'.
	#define BSO_BSIZE_MIN	0

	//t Size of a buffer.
	typedef size_t bsize__;

	//c The portable version of a buffer size.
	class p_bsize__
	: p_long__<bsize__>
	{
	public:
		p_bsize__( bsize__ Op = 0 )
			: p_long__<bsize__>( Op )
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
		sign__( sbyte__ Sign )
		{
			Sign_ = (sbyte__)( Sign ? ( Sign > 0 ? 1 : -1 ) : 0 );
		}
		sign__( int Sign = 0 )
		{
			Sign_ = (sbyte__)( Sign ? ( Sign > 0 ? 1 : -1 ) : 0 );
		}
		operator sbyte__( void )
		{
			return Sign_;
		}
	};

	//t A portable sign.
	typedef sign__ p_sign__;

	//f Return 'Value' as string. Valid only until next call of a 'Convert(..)' function.
	inline const char *Convert( bso::ulong__ Value )
	{
		static char Buffer[20];

		sprintf( Buffer, "%lu", Value );

		return Buffer;
	}

	//f Return 'Value' as string. Valid only until next call of a 'Convert(..)' function.
	inline const char *Convert( bso::ushort__ Value )
	{
		return Convert( (bso::ulong__)Value );
	}

	//f Return 'Value' as string. Valid only until next call of a 'Convert(..)' function.
	inline const char *Convert( bso::ubyte__ Value )
	{
		return Convert( (bso::ulong__)Value );
	}

	//f Return 'Value' as string. Valid only until next call of a 'Convert(..)' function.
	inline const char *Convert( bso::slong__ Value )
	{
		static char Buffer[20];

		sprintf( Buffer, "%li", Value );

		return Buffer;
	}

	//f Return 'Value' as string. Valid only until next call of a 'Convert(..)' function.
	inline const char *Convert( bso::sshort__ Value )
	{
		return Convert( (bso::slong__)Value );
	}

	//f Return 'Value' as string. Valid only until next call of a 'Convert(..)' function.
	inline const char *Convert( bso::sbyte__ Value )
	{
		return Convert( (bso::slong__)Value );
	}

	//f Return 'Value' as string. Valid only until next call of a 'Convert(..)' function.
	inline const char *Convert( lfloat__ Value )
	{
		static char Buffer[40];

		sprintf( Buffer, "%Lf", Value );

		return Buffer;
	}

	//f Return 'Value' as string. Valid only until next call of a 'Convert(..)' function.
	inline const char *Convert( float__ Value )
	{
		return Convert( (lfloat__)Value );
	}

	//f Return 'Value' as string. Valid only until next call of a 'Convert(..)' function.
	inline const char *Convert( sfloat__ Value )
	{
		return Convert( (lfloat__)Value );
	}

}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
