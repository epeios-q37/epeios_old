/*
	Header for the 'str' library by Claude SIMON (csimon@epeios.org)
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

#ifndef STR__INC
#define STR__INC

#define STR_NAME		"STR"

#define	STR_VERSION	"$Revision$"

#define STR_OWNER		"Claude SIMON (csimon@epeios.org)"

#include "ttr.h"

extern class ttr_tutor &STRTutor;

#if defined( XXX_DBG ) && !defined( STR_NODBG )
#define STR_DBG
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
//D STRing 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "txf.h"
#include "bch.h"

namespace str {

	namespace {
		using bch::bunch_;
	}

	enum base__ {
		bAuto = 0,	// Détermination automatique de la base (suivant le premier caractère).
		b10 = 10,	// Base décimale.
		b16 = 16,	// Base hexadécimale.
	};


	class _string_size_handler {
	public:
		static epeios::size__ SizeOf( const char *S )
		{
#ifdef STR_DBG
			if ( strlen( S ) > EPEIOS_SIZE_MAX )
				ERRu();
#endif
			return (epeios::size__)strlen( S );
		}
	};

	//c A string.
	class string_
	: public E_BUNCHx_( bso::char__, _string_size_handler )
	{
	private:
	public:
		struct s
		: public E_BUNCHx_( bso::char__, _string_size_handler )::s {};
		string_( s &S )
		: E_BUNCHx_( bso::char__, _string_size_handler )( S )
		{}
		void reset( bool P = true )
		{
			E_BUNCHx_( bso::char__, _string_size_handler )::reset( P );
		}
		void plug( mdr::E_MEMORY_DRIVER__ &Driver )
		{
			E_BUNCHx_( bso::char__, _string_size_handler )::plug( Driver );
		}
		void plug( mmm::multimemory_ &M )
		{
			E_BUNCHx_( bso::char__, _string_size_handler )::plug( M );
		}
		string_ &operator =( const string_ &O )
		{
			E_BUNCHx_( bso::char__, _string_size_handler )::operator =( O );

			return *this;
		}
		//f Initialization.
		void Init( void )
		{
			E_BUNCHx_( bso::char__, _string_size_handler )::Init();
		}
		//f Initialization with 'Seed'.
		void Init( const bso::char__ *Seed )
		{
#ifdef STR_DBG
			if ( strlen( Seed ) > EPEIOS_SIZE_MAX )
				ERRu();
#endif
			E_BUNCHx_( bso::char__, _string_size_handler )::Init( Seed, (epeios::size__)strlen( Seed ) );
		}
		//f Initialization with 'Seed'.
		void Init( const str::string_ &Seed )
		{
			if ( this != &Seed ) {
				Init();

				this->operator =( Seed );
			}
		}
		string_ &operator =( const char *Chaine )
		{
#ifdef STR_DBG
			if ( strlen( Chaine ) > EPEIOS_SIZE_MAX )
				ERRu();
#endif
			Init();
			E_BUNCHx_( bso::char__, _string_size_handler )::StoreAndAdjust( Chaine, (epeios::size__)strlen( Chaine ) );

			return *this;
		}
		//f Convert 'Amount' characters at 'Position' from string to a 'char *'. Returned pointer MUST be freed with 'free'.
		char *Convert(
			epeios::row__ Position = 0,
			epeios::size__ Amount = TYM_MAX_SIZE ) const;
		//f Filter out the 'Model' charactere.
		void FilterOut( char Model );
		//f Remove characters 'Model' beginnig the string.
		void StripLeadingCharacter( char Model );
		//f Remove characters 'Model' at ending the string.
		void StripTailingCharacter( char Model );
		//f Remove characters 'Model' both beginning and ending the string.
		void StripCharacter( char Model )
		{
			StripLeadingCharacter( Model );
			StripTailingCharacter( Model );
		}
		//f Replace the 'Old' char with the 'New' one.
		void Replace(
			char Old,
			char New );
		//f Return the position of the first occurence of 'S', beginning at 'Start'. Return 'NONE' if no occurence.
		epeios::row__ Search(
			const string_ &S,
			epeios::row__ Start = 0 ) const;
		//f Return the position of the first occurence of 'C', beginning at 'Start'. Return 'NONE' if no occurence.
		epeios::row__ Search(
			char C,
			epeios::row__ Start = 0 ) const;
		/*f Convert to unsigned long. If 'ErrP' != NULL, put in it the position of the bad character
		if there is one. 'Limit' is the max value that the returned value can have. */
		bso::ulong__ ToUL(
			epeios::row__ Begin,
			epeios::row__ *ErrP,
			base__ Base,
			bso::ulong__ Limit = BSO_ULONG_MAX ) const;
		//f Variation in parameters.
		bso::ulong__ ToUL(
			epeios::row__ *ErrP = NULL,
			base__ Base = bAuto,
			bso::ulong__ Limit = BSO_ULONG_MAX ) const
		{
			return ToUL( 0, ErrP, Base, Limit );
		}
		/*f Convert to signed long. If 'ErrP' != NULL, put in it the position of the bad character
		if there is one. 'Limit' is the max absolute value that the returned value can have. */
		bso::slong__ ToSL(
			epeios::row__ Begin,
			epeios::row__ *ErrP,
			bso::ulong__ PositiveLimit = BSO_SLONG_MAX,
			bso::slong__ NegativeLimit = BSO_SLONG_MIN ) const
		{
			if ( Get( Begin ) == '-' )
				if ( Next( Begin ) == NONE ) {
					*ErrP = *Begin + 1;
					return 0;
				} else 
					return -(bso::slong__)ToUL( Next( Begin ), ErrP, b10, -NegativeLimit );
			else
				return (bso::slong__)ToUL( Begin, ErrP, b10, PositiveLimit );
		}
		//f Variation in parameters.
		bso::slong__ ToSL(
			epeios::row__ *ErrP = NULL,
			bso::ulong__ PositiveLimit = BSO_SLONG_MAX,
			bso::slong__ NegativeLimit = BSO_SLONG_MIN ) const
		{
			return ToSL( 0, ErrP, PositiveLimit, NegativeLimit );
		}
		/*f Convert to unsigned short. If 'ErrP' != NULL, put in it the position of the bad character
		if there is one. 'Limit' is the max value that the returned value can have. */
		bso::ushort__ ToUS(
			epeios::row__ Begin,
			epeios::row__ *ErrP,
			base__ Base = bAuto,
			bso::ushort__ Limit = BSO_USHORT_MAX ) const
		{
			return (bso::ushort__)ToUL( Begin, ErrP, Base, Limit );
		}
		bso::ushort__ ToUS(
			epeios::row__ *ErrP,
			base__ Base,
			epeios::row__ Begin,
			bso::ushort__ Limit = BSO_USHORT_MAX ) const
		{
			return (bso::ushort__)ToUS( Begin, ErrP, Base, Limit );
		}
		//f Variation in parameters.
		bso::ushort__ ToUS(
			epeios::row__ *ErrP = NULL,
			base__ Base = bAuto,
			bso::ushort__ Limit = BSO_USHORT_MAX ) const
		{
			return ToUS( ErrP, Base, 0, Limit );
		}
		/*f Convert to signed short. If 'ErrP' != NULL, put in it the position of the bad character
		if there is one. 'Limit' is the max value that the returned value can have. */
		bso::sshort__ ToSS(
			epeios::row__ Begin,
			epeios::row__ *ErrP,
			bso::ushort__ PositiveLimit = BSO_SSHORT_MAX,
			bso::sshort__ NegativeLimit = BSO_SSHORT_MIN ) const
		{
			return (bso::sshort__)ToSL( Begin, ErrP, PositiveLimit, NegativeLimit );
		}
		//f Variation in parameters.
		bso::sshort__ ToSS(
			epeios::row__ *ErrP = NULL,
			bso::ushort__ PositiveLimit = BSO_SSHORT_MAX,
			bso::sshort__ NegativeLimit = BSO_SSHORT_MIN ) const
		{
			return ToSS( 0, ErrP, PositiveLimit, NegativeLimit );
		}
		/*f Convert to unsigned byte. If 'ErrP' != NULL, put in it the position of the bad character
		if there is one. 'Limit' is the max value that the returned value can have. */
		bso::ubyte__ ToUB(
			epeios::row__ Begin,
			epeios::row__ *ErrP,
			base__ Base = bAuto,
			bso::ubyte__ Limit = BSO_UBYTE_MAX ) const
		{
			return (bso::ubyte__)ToUL( Begin, ErrP, Base, Limit );
		}
		bso::ubyte__ ToUB(
			epeios::row__ *ErrP,
			base__ Base,
			epeios::row__ Begin,
			bso::ubyte__ Limit = BSO_UBYTE_MAX ) const
		{
			return ToUB( Begin, ErrP, Base, Limit );
		}
		//f Variation in parameters.
		bso::ubyte__ ToUB(
			epeios::row__ *ErrP = NULL,
			base__ Base = bAuto,
			bso::ubyte__ Limit = BSO_UBYTE_MAX ) const
		{
			return ToUB( 0, ErrP, Base, Limit );
		}
		/*f Convert to signed byte. If 'ErrP' != NULL, put in it the position of the bad character
		if there is one. 'Limit' is the max value that the returned value can have. */
		bso::sbyte__ ToSB(
			epeios::row__ Begin,
			epeios::row__ *ErrP,
			bso::ubyte__ PositiveLimit = BSO_SBYTE_MAX,
			bso::sbyte__ NegativeLimit = BSO_SBYTE_MIN ) const
		{
			return (bso::sbyte__)ToSL( 0, ErrP, PositiveLimit, NegativeLimit );
		}
		//f Variation in parameters.
		bso::sbyte__ ToSB(
			epeios::row__ *ErrP = NULL,
			bso::ubyte__ PositiveLimit = BSO_SBYTE_MAX,
			bso::sbyte__ NegativeLimit = BSO_SBYTE_MIN ) const
		{
			return ToSB( 0, ErrP, PositiveLimit, NegativeLimit );
		}
		/*f Convert to long float. If 'ErrP' != NULL, put the character where is 
		an error or 'NONE' when no error. */
		bso::lfloat__ ToLF(
			epeios::row__ *ErrP,
			epeios::row__ Begin ) const;
		//f Variation in parameters.
		bso::lfloat__ ToLF( epeios::row__ *ErrP = NULL ) const
		{
			return ToLF( ErrP, 0 );
		}

	};

	//f Put all the caracters of 'String' on 'Ostream', and put a 'NULL' character.
	flw::oflow__ &operator <<(
		flw::oflow__ &OStream,
		const string_ &String );

	//f Append all the characters from 'IStream' to 'String' until encounter a 'NULL' character.
	flw::iflow__ &operator >>(
		flw::iflow__ &IStream,
		string_ &String );


	txf::text_oflow__ &operator <<(
		txf::text_oflow__ &OStream,
		const string_ &String );

	//f Return 'S1' - 'S2' beginning at 'BeginS1' and 'BeginS2' and a length of 'Amount'.
	inline bso::sign__ Compare(
		const string_ &S1,
		const string_ &S2,
		epeios::row__ BeginS1,
		epeios::row__ BeginS2,
		epeios::size__ Amount )
	{
		return bch::Compare( S1, S2, BeginS1 ,BeginS2 , Amount );
	}

	//f Return 'S1' - 'S2' beginning at 'BeginS1' and 'BeginS2'.
	inline bso::sign__ Compare(
		const string_ &S1,
		const string_ &S2,
		epeios::row__ BeginS1 = 0,
		epeios::row__ BeginS2 = 0 )
	{
		bso::sign__ Resultat;
		epeios::size__ T1 = S1.Amount() - *BeginS1, T2 = S2.Amount() - *BeginS2;

		if ( ( Resultat = Compare( S1, S2, BeginS1, BeginS2, T1 < T2 ? T1 : T2 ) ) != 0 )
			return Resultat;
		else
			return bso::Compare ( T1, T2 );
	}

	inline bso::bool__ operator ==(
		const string_ &S1,
		const string_ &S2 )
	{
		return Compare( S1, S2 ) == 0;
	}

	inline bso::bool__ operator !=(
		const string_ &S1,
		const string_ &S2 )
	{
		return Compare( S1, S2 ) != 0;
	}

	inline bso::bool__ operator <(
		const string_ &S1,
		const string_ &S2 )
	{
		return Compare( S1, S2 ) < 0;
	}

	inline bso::bool__ operator <=(
		const string_ &S1,
		const string_ &S2 )
	{
		return Compare( S1, S2 ) <= 0;
	}

	inline bso::bool__ operator >(
		const string_ &S1,
		const string_ &S2 )
	{
		return Compare( S1, S2 ) > 0;
	}

	inline bso::bool__ operator >=(
		const string_ &S1,
		const string_ &S2 )
	{
		return Compare( S1, S2 ) >= 0;
	}

	class string
	: public string_
	{
	public:
		string_::s static_;
		string( void )
		: string_( static_ )
		{
			reset( false );
		}
		string( const char *S )
		: string_( static_ )
		{
			reset( false );

			string_::Init();

			if ( S != NULL )
				string_::Append( S );
		}
		string(
			const char *S,
			epeios::size__ Length )
		: string_( static_ )
		{
			reset( false );

#ifdef STR_DBG
			ERRu();
#endif

			string_::Init();
			string_::Append( S, Length );
		}
		string( char C )
		: string_( static_ )
		{
			reset( false );

			string_::Init();
			string_::Append( C );
		}
		string( const string_ &String )
		: string_( static_ )
		{
			reset( false );

			string_::Init();
			string_::operator =( String );
		}
		~string( void )
		{
			reset( true );
		}
		string &operator =( const string &S )
		{
			string_::operator =( S );

			return *this;
		}
		string &operator =( const string_ &S )
		{
			string_::operator =( S );

			return *this;
		}
		string &operator =( const char *O )
		{
			string_::operator =( O );

			return *this;
		}
		string_ &operator *( void )
		{
			return *this;
		}
		const string_ &operator *( void ) const
		{
			return *this;
		}
	};

	inline bso::bool__ operator ==(
		const str::string_ &Op1,
		const char *Op2 )
	{
		return Op1 == str::string( Op2 );
	}

	inline bso::bool__ operator !=(
		const str::string_ &Op1,
		const char *Op2 )
	{
		return Op1 != str::string( Op2 );
	}

	inline bso::bool__ operator <(
		const str::string_ &Op1,
		const char *Op2 )
	{
		return Op1 < str::string( Op2 );
	}

	inline bso::bool__ operator <=(
		const str::string_ &Op1,
		const char *Op2 )
	{
		return Op1 <= str::string( Op2 );
	}

	inline bso::bool__ operator >=(
		const str::string_ &Op1,
		const char *Op2 )
	{
		return Op1 >= str::string( Op2 );
	}

	inline bso::bool__ operator >(
		const str::string_ &Op1,
		const char *Op2 )
	{
		return Op1 > str::string( Op2 );
	}

	//f Convert 'String' to upper case. Returns cnverted 'String'.
	string_ &ToUpper(
		string_ &String,
		bso::bool__ DontHandleAccent = false );

	//f Convert 'String' to lower case. Returns converted 'String'.
	string_ &ToLower(
		string_ &String,
		bso::bool__ dontHandleAccent = false );
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
