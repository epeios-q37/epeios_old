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

	class _string_size_handler {
	public:
		static epeios::size__ SizeOf( const char *S )
		{
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
		void plug( mdr::E_MEMORY_DRIVER_ &Driver )
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
			E_BUNCHx_( bso::char__, _string_size_handler )::Init( Seed, strlen( Seed ) );
		}
		//f Initialization with 'Seed'.
		void Init( const str::string_ &Seed )
		{
			Init();

			this->operator =( Seed );
		}
		string_ &operator =( const char *Chaine )
		{
			Init();
			E_BUNCHx_( bso::char__, _string_size_handler )::StoreAndAdjust( Chaine, strlen( Chaine ) );

			return *this;
		}
		//f Convert 'Amount' characters at 'Position' from string to a 'char *'. Returned pointer MUST be freed with 'free'.
		char *Convert(
			epeios::row__ Position = 0,
			epeios::size__ Amount = TYM_MAX_SIZE ) const;
		//f Filter out the 'Model' charactere.
		void FilterOut( char Model );
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
			bso::ulong__ Limit = BSO_ULONG_MAX ) const;
		//f Variation in parameters.
		bso::ulong__ ToUL(
			epeios::row__ *ErrP = NULL,
			bso::ulong__ Limit = BSO_ULONG_MAX ) const
		{
			return ToUL( 0, ErrP, Limit );
		}
		/*f Convert to signed long. If 'ErrP' != NULL, put in it the position of the bad character
		if there is one. 'Limit' is the max absolute value that the returned value can have. */
		bso::slong__ ToSL(
			epeios::row__ Begin,
			epeios::row__ *ErrP,
			bso::ulong__ Limit = BSO_SLONG_MAX ) const
		{
			if ( Get( Begin ) == '-' )
				if ( Next( Begin ) == NONE ) {
					*ErrP = *Begin + 1;
					return 0;
				} else 
					return -(bso::slong__)ToUL( Next( Begin ), ErrP, Limit );
			else
				return ToUL( Begin, ErrP, Limit );
		}
		//f Variation in parameters.
		bso::slong__ ToSL(
			epeios::row__ *ErrP = NULL,
			bso::ulong__ Limit = BSO_SLONG_MAX ) const
		{
			return ToSL( 0, ErrP, Limit );
		}
		/*f Convert to unsigned short. If 'ErrP' != NULL, put in it the position of the bad character
		if there is one. 'Limit' is the max value that the returned value can have. */
		bso::ushort__ ToUS(
			epeios::row__ *ErrP,
			epeios::row__ Begin,
			bso::ushort__ Limit = BSO_USHORT_MAX ) const
		{
			return (bso::ushort__)ToUL( Begin, ErrP, Limit );
		}
		//f Variation in parameters.
		bso::ushort__ ToUS(
			epeios::row__ *ErrP = NULL,
			bso::ushort__ Limit = BSO_USHORT_MAX ) const
		{
			return ToUS( ErrP, 0, Limit );
		}
		/*f Convert to signed short. If 'ErrP' != NULL, put in it the position of the bad character
		if there is one. 'Limit' is the max value that the returned value can have. */
		bso::sshort__ ToSS(
			epeios::row__ *ErrP,
			epeios::row__ Begin,
			bso::sshort__ Limit = BSO_SSHORT_MAX ) const
		{
			return (bso::sshort__)ToSL( Begin, ErrP, Limit );
		}
		//f Variation in parameters.
		bso::sshort__ ToSS(
			epeios::row__ *ErrP = NULL,
			bso::ushort__ Limit = BSO_SSHORT_MAX ) const
		{
			return ToSS( ErrP, 0, Limit );
		}
		/*f Convert to unsigned byte. If 'ErrP' != NULL, put in it the position of the bad character
		if there is one. 'Limit' is the max value that the returned value can have. */
		bso::ubyte__ ToUB(
			epeios::row__ *ErrP,
			epeios::row__ Begin,
			bso::ubyte__ Limit = BSO_UBYTE_MAX ) const
		{
			return (bso::ubyte__)ToUL( 0, ErrP, Limit );
		}
		//f Variation in parameters.
		bso::ubyte__ ToUB(
			epeios::row__ *ErrP = NULL,
			bso::ubyte__ Limit = BSO_UBYTE_MAX ) const
		{
			return ToUB( ErrP, 0, Limit );
		}
		/*f Convert to signed byte. If 'ErrP' != NULL, put in it the position of the bad character
		if there is one. 'Limit' is the max value that the returned value can have. */
		bso::sbyte__ ToSB(
			epeios::row__ *ErrP,
			epeios::row__ Begin,
			bso::ubyte__ Limit = BSO_SBYTE_MAX ) const
		{
			return (bso::sbyte__)ToSL( 0, ErrP, Limit );
		}
		//f Variation in parameters.
		bso::sbyte__ ToSB(
			epeios::row__ *ErrP = NULL,
			bso::ubyte__ Limit = BSO_SBYTE_MAX ) const
		{
			return ToSB( ErrP, 0, Limit );
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
	flw::oflow___ &operator <<(
		flw::oflow___ &OStream,
		const string_ &String );

	//f Append all the characters from 'IStream' to 'String' until encounter a 'NULL' character.
	flw::iflow___ &operator >>(
		flw::iflow___ &IStream,
		string_ &String );


	txf::text_oflow___ &operator <<(
		txf::text_oflow___ &OStream,
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
			return ( T1 == T2 ? 0 : T1 > T2 ? 1 : -1 );
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
			string_::Append( S );
		}
		string(
			const char *S,
			size_t Length )
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
	};

	//f Convert 'String' to upper case.
	void ToUpper( string_ &String );

	//f Convert 'String' to lower case.
	void ToLower( string_ &String );
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
