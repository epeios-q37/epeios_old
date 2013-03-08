/*
	Header for the 'str' library by Claude SIMON (http://zeusw.org/intl/contact.html)
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

#ifndef STR__INC
#define STR__INC

#define STR_NAME		"STR"

#define	STR_VERSION	"$Revision$"

#define STR_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"

#include "ttr.h"

extern class ttr_tutor &STRTutor;

#if defined( XXX_DBG ) && !defined( STR_NODBG )
#define STR_DBG
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
//D STRing 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "txf.h"
#include "bch.h"
#include "cpe.h"
#include "ctn.h"

#define STR_HEXADECIMAL_MARKER	'#'	// LEs nombres préfixés par ce caractère sont considèrés comme exprimés en hexadécimal.

namespace str {

	namespace {
		using bch::bunch_;
		using bso::_guint__;
		using bso::_gsint__;
	}

	enum base__ {
		bAuto = 0,	// Détermination automatique de la base (suivant le premier caractère).
		b10 = 10,	// Base décimale.
		b16 = 16,	// Base hexadécimale.
	};

	class string_;	// Prédéclaration.

#define STR_BUFFER___	tol::E_FPOINTER___( bso::char__ )


	_guint__ _GenericUnsignedConversion(
		const class string_ &String,
		mdr::row__ Begin,
		mdr::row__ *ErrP,
		base__ Base,
		_guint__ Limit );

	_gsint__ _GenericSignedConversion(
		const class string_ &String,
		mdr::row__ Begin,
		mdr::row__ *ErrP,
		base__ Base,
		_gsint__ PositiveLimit,
		_gsint__ NegativeLimit );

	class _string_size_handler {
	public:
		static mdr::size__ SizeOf( const char *S )
		{
			if ( NULL == S )
				S = "";

			return strlen( S );
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
			if ( NULL == Seed )
				Seed = "";

			E_BUNCHx_( bso::char__, _string_size_handler )::Init( Seed, strlen( Seed ) );
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
			if ( NULL == Chaine )
				Chaine = "";

			Init();
			E_BUNCHx_( bso::char__, _string_size_handler )::StoreAndAdjust( Chaine, strlen( Chaine ) );

			return *this;
		}
		//f Convert 'Amount' characters at 'Position' from string to a 'char *'.
		const char *Convert(
			mdr::row__ Position,
			mdr::size__ Amount,
			STR_BUFFER___ &Buffer ) const;
		const char *Convert(
			mdr::row__ Position,
			STR_BUFFER___ &Buffer ) const
		{
			return Convert( Position, TYM_MAX_SIZE, Buffer );
		}
		const char *Convert( STR_BUFFER___ &Buffer ) const
		{
			return Convert( 0, TYM_MAX_SIZE, Buffer );
		}
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
		string_ &Truncate( mdr::row__ Row )
		{
			E_BUNCHx_( bso::char__, _string_size_handler )::Truncate( Row );

			return *this;
		}
		string_ &Truncate( bso::size__ Amount = 1 )
		{
			E_BUNCHx_( bso::char__, _string_size_handler )::Truncate( Amount );

			return *this;
		}
		//f Replace the 'Old' char with the 'New' one.
		void Replace(
			char Old,
			char New );
		// Remplace la 'Position'ième occurence de 'Tag' par 'Value'. Retourne 'true' si succés, 'false' sinon.
		bso::bool__ Replace(
			char Tag,
			bso::ulong__ Position,
			const string_ &Value );
		//f Return the position of the first occurence of 'S', beginning at 'Start'. Return 'NONE' if no occurence.
		mdr::row__ Search(
			const string_ &S,
			mdr::row__ Start = 0 ) const;
		//f Return the position of the first occurence of 'C', beginning at 'Start'. Return 'NONE' if no occurence.
		mdr::row__ Search(
			char C,
			mdr::row__ Start = 0 ) const;
		// NOTA : Les méthodes 'ToNumber'(...)' facilitent la mise en oeuvre de 'template's.
#define STR_UN( name, type, limit )\
	type To##name(\
			mdr::row__ Begin,\
			mdr::row__ *ErrP,\
			base__ Base,\
			type Limit = limit ) const\
		{\
			return (type)_GenericUnsignedConversion( *this, Begin, ErrP, Base, Limit );\
		}\
		type To##name(\
			mdr::row__ *ErrP = NULL,\
			base__ Base = bAuto,\
			type Limit = limit ) const\
		{\
			return To##name( 0, ErrP, Base, Limit );\
		}\
		void ToNumber(\
			type &Number,\
			mdr::row__ *Error = NULL ) const\
		{\
			Number = To##name( Error );\
		}\
		void ToNumber(\
			type Limit,\
			type &Number,\
			mdr::row__ *Error = NULL ) const\
		{\
			Number = To##name( Error, bAuto, Limit );\
		}
#define STR_SN( name, type, positive_limit, negative_limit )\
	type To##name(\
			mdr::row__ Begin,\
			mdr::row__ *ErrP,\
			base__ Base,\
			type PositiveLimit = positive_limit,\
			type NegativeLimit = negative_limit ) const\
		{\
			return (type)_GenericSignedConversion( *this, Begin, ErrP, Base, PositiveLimit, NegativeLimit );\
		}\
		type To##name(\
			mdr::row__ *ErrP = NULL,\
			base__ Base = bAuto,\
			type PositiveLimit = positive_limit,\
			type NegativeLimit = negative_limit ) const\
		{\
			return To##name( 0, ErrP, Base, PositiveLimit, NegativeLimit );\
		}\
		void ToNumber(\
			type &Number,\
			mdr::row__ *Error = NULL ) const\
		{\
			Number = To##name( Error );\
		}\
		void ToNumber(\
			type PositiveLimit,\
			type NegativeLimit,\
			type &Number,\
			mdr::row__ *Error = NULL ) const\
		{\
			Number = To##name( Error, bAuto, PositiveLimit, NegativeLimit );\
		}
		STR_UN( ULL, bso::ullong__, BSO_ULLONG_MAX )
		STR_SN( SLL, bso::sllong__, BSO_SLLONG_MAX, BSO_SLLONG_MIN )
		STR_UN( UL, bso::ulong__, BSO_ULONG_MAX )
		STR_SN( SL, bso::slong__, BSO_SLONG_MAX, BSO_SLONG_MIN )
		STR_UN( US, bso::ushort__, BSO_USHORT_MAX )
		STR_SN( SS, bso::sshort__, BSO_SSHORT_MAX, BSO_SSHORT_MIN )
		STR_UN( UB, bso::ubyte__, BSO_UBYTE_MAX )
		STR_SN( SB, bso::sbyte__, BSO_SBYTE_MAX, BSO_SBYTE_MIN )
		bso::lfloat__ ToLF(
			mdr::row__ *ErrP,
			mdr::row__ Begin ) const;
		bso::lfloat__ ToLF( mdr::row__ *ErrP = NULL ) const
		{
			return ToLF( ErrP, 0 );
		}
		void *ToPointer( mdr::row__ *ErrP = NULL )
		{
# ifdef CPE__64BITS
			return (void *)ToULL( ErrP, str::b16 );
# elif defined ( CPE__32BITS )
			return (void *)ToUL( ErrP, str::b16 );
# else
#  error "Unnknown bitness !"
# endif
		}
		void ToNumber(
			bso::lfloat__ &Number,
			mdr::row__ *Error = NULL ) const
		{
			Number = ToLF( Error );
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
		mdr::row__ BeginS1,
		mdr::row__ BeginS2,
		mdr::size__ Amount )
	{
		return bch::Compare( S1, S2, BeginS1 ,BeginS2 , Amount );
	}

	//f Return 'S1' - 'S2' beginning at 'BeginS1' and 'BeginS2'.
	inline bso::sign__ Compare(
		const string_ &S1,
		const string_ &S2,
		mdr::row__ BeginS1 = 0,
		mdr::row__ BeginS2 = 0 )
	{
		bso::sign__ Resultat;
		mdr::size__ T1 = S1.Amount() - *BeginS1, T2 = S2.Amount() - *BeginS2;

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
			mdr::size__ Length )
		: string_( static_ )
		{
			reset( false );

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

	typedef ctn::E_MCONTAINER_( str::string_ ) strings_;
	E_AUTO( strings );

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

	void ReplaceTag(
		str::string_ &String,
		bso::ubyte__ Indice,
		const str::string_ &Value,
		char TagMarker );

	void ReplaceTags(
		str::string_ &String,
		const strings_ &Values,
		char TagMarker );

}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
