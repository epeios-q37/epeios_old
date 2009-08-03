/*
	'str' library by Claude SIMON (csimon@epeios.org)
	Requires the 'str' header file ('str.h').
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

#define STR__COMPILATION

#include "str.h"

class strtutor
: public ttr_tutor
{
public:
	strtutor( void )
	: ttr_tutor( STR_NAME )
	{
#ifdef STR_DBG
		Version = STR_VERSION "\b\bD $";
#else
		Version = STR_VERSION;
#endif
		Owner = STR_OWNER;
		Date = "$Date$";
	}
	virtual ~strtutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace str;

static const char Table_[] = "AAAAAAãCEEEE¦-+¤DNUUUUOÎOUUUUYÌ¯aaaaaaµceeeeiiiionooooo¸°uuuuy¦y";

static inline int GetAccentFree__( int C )
{
	if ( C >= 192 )
		C = Table_[C - 192];

	return ( C < 0 ? 256 - C : C );
}


static inline int GetAccentFree_( int C )
{
	if ( C < 0 ) {
		C = GetAccentFree__( 256 + C );

		if ( C >= 128 )
			C = C - 256;

		return C;
	} else
		return GetAccentFree__( C );
}


static inline int toxlower_( int C )
{
	return tolower( GetAccentFree_( C ) );
}

static inline int toxupper_( int C )
{
	return toupper( GetAccentFree_( C ) );
}

static bso::ubyte__ Convert_( bso::char__ C )
{
	if ( isdigit( C ) )
		return C - '0';
	else
		return tolower( C )- 'a' + 10;
}

template <class ostream> static void Put_(
	const string_ &String,
	ostream &OStream )		// Can be optimized by using a buffer.
{
	epeios::size__ Amount = String.Amount();

	if ( Amount != 0 )
	{
#ifdef STR_DBG
		flw::datum__ C;
#endif
		epeios::row_t__ P = 0;


		while ( P < Amount ) {
#ifdef STR_DBG
			OStream.Put( C = (flw::datum__)String.Get( P++ ) );

			if ( C == 0 )
				ERRu();
#else
			OStream.Put( (flw::datum__)String.Get( P++ ) );
#endif
		}
	}
}


flw::oflow__ &str::operator <<(
	flw::oflow__ &OStream,
	const string_ &String )
{
	Put_( String, OStream );

	OStream.Put( 0 );

	return OStream;
}


flw::iflow__ &str::operator >>(
	flw::iflow__ &IStream,
	string_ &S )
{
	char C;

	while( ( C = (char)IStream.Get() ) != 0 )
		S.Append( C );

	return IStream;
}
	

txf::text_oflow__ &str::operator <<(
	txf::text_oflow__ &OStream,
	const string_ &String )
{
	Put_( String, OStream );

	return OStream;
}

// Convertit le contenu de 'String' en majuscule.
string_ &str::ToUpper(
	string_ &String,
	bso::bool__ DontHandleAccent )
{
	epeios::row_t__ P = String.Amount();

	if ( DontHandleAccent )
		while ( P-- )
			String.Store( (char)toupper( String.Get( P ) ) , P );
	else
		while ( P-- )
			String.Store( (char)toxupper_( String.Get( P ) ) , P );

	return String;
}

// Convertit le contenu de 'String' en minuscule.
string_ &str::ToLower(
	string_ &String,
	bso::bool__ DontHandleAccent )
{
	epeios::row_t__ P = String.Amount();

	if ( DontHandleAccent )
		while ( P-- )
			String.Store( (char)tolower( String.Get( P )) , P );
	else
		while ( P-- )
			String.Store( (char)toxlower_( String.Get( P )) , P );

	return String;
}

// Convertit la chaine 'char *' et rajoute un 0. Le pointeur retourné doit être libèré par un 'free'.
const char *string_::Convert(
	epeios::row__ Position,
	epeios::size__ Quantity,
	STR_BUFFER___ &Buffer ) const
{
ERRProlog
ERRBegin
#ifdef STR_DBG
	if ( *Position > Amount() && ( Position != 0 ) )
		ERRu();
#endif

	if ( Quantity > ( Amount() - *Position ) )
		Quantity = Amount() - *Position;

	if ( ( Buffer = (char *)malloc( Quantity + 1 ) ) == NULL )
		ERRa();

	if ( Quantity != 0 )
		Recall( Position, Quantity, Buffer );

	Buffer[Quantity] = 0;
ERRErr
ERREnd
ERREpilog
	return Buffer;
}

// Could be easily optimized, would be when I have some time.
void string_::FilterOut( char Model )
{
	epeios::row_t__ Source, Dest = 0;
	char Char;

	for( Source = 0; Source < Amount(); Source++ ) {
		if ( ( Char = Get( Source ) ) != Model )
			Store( Char, Dest++ );
	}

	Allocate( Dest );
}

// Could be easily optimized, would be when I have some time.
void string_::StripLeadingCharacter( char Model )
{
	epeios::row__ Row = First();
	epeios::size__ Amount = 0;

	while ( ( Row != NONE ) && ( Get( Row ) == Model ) ) {
		Amount++;
		Row = Next( Row );
	}

	Remove( 0, Amount );
}

// Could be easily optimized, would be when I have some time.
void string_::StripTailingCharacter( char Model )
{
	epeios::row__ Row = Last();
	epeios::size__ Amount = 0;

	while ( ( Row != NONE ) && ( Get( Row ) == Model ) ) {
		Amount++;
		Row = Previous( Row );
	}

	Truncate( Amount );
}

// Could be easily optimized, would be when I have some time.
void string_::Replace(
	char Old,
	char New )
{
	epeios::row_t__ Source;
	char Char;

	for( Source = 0; Source < Amount(); Source++ ) {
		if ( ( Char = Get( Source ) ) == Old )
			Store( New, Source );
		else
			Store( Char, Source );
	}
}

bso::bool__ string_::Replace(
	char Tag,
	bso::ulong__ Position,
	const str::string_ &Value )
{
	epeios::row__ Row = First();

#ifdef STR_DBG
	if ( Position == 0 )
		ERRu();
#endif

	while ( ( Position-- ) && ( Row != NONE ) ) {
		Row = Search( Tag, Row );
	}

	if ( Row != NONE ) {
		Remove( Row, 1 );

		Insert( Value, Row );

		return true;
	} else
		return false;
}

epeios::row__ string_::Search(
	const string_ &S,
	epeios::row__ Start ) const
{
	if ( S.Amount() <= Amount() ) {
		epeios::size__ Amount = S.Amount();
		epeios::row_t__ Limit = this->Amount() - Amount;

		while( ( *Start <= Limit )
			&& ( Compare( S, *this, 0, Start, Amount ) != 0 ) )
			(*Start)++;

		if ( *Start > Limit )
			return NONE;
		else
			return Start;
	}
	else {
		return NONE;
	}
}

epeios::row__ string_::Search(
	char C,
	epeios::row__ Start ) const
{
	epeios::row_t__ Limit = Amount();

	while( ( *Start < Limit )
		&& ( Get( Start ) != C ) )
		(*Start)++;

	if ( *Start >= Limit )
		return NONE;
	else
		return Start;
}

_generic_integer__ str::_GenericConversion(
	const str::string_ &String,
	epeios::row__ Begin,
	epeios::row__ *ErrP,
	base__ BaseFlag,
	_generic_integer__ Limit )
{
	_generic_integer__ Result = 0;
	epeios::row__ &P = Begin;
	bso::ubyte__ C;
	bso::ubyte__ Base;
	_generic_integer__ OtherLimit = 0;

	if ( BaseFlag == bAuto )
		if ( ( ( String.Amount() != 0 ) ) && ( String.Get( P ) == '#' ) ) {
			BaseFlag = b16;
			P = String.Next( P );
		} else
			BaseFlag = b10;

	if ( BaseFlag > 255 )
		ERRu();

	Base = (int)BaseFlag;

#ifdef STR_DBG
	if ( Limit < Base )
		ERRu();
#endif

	OtherLimit = Limit / Base;

	if ( *P < String.Amount() )
		while( P != NONE ) {
			C = Convert_( String.Get( P ) );

			if ( C >= Base )
				break;

			if ( Result > OtherLimit )
				break;

			Result *= Base;

			if ( ( Limit - C ) < Result )
			   break;

			Result += C;

			P = String.Next( P );
		}

	if ( P != NONE )
		if ( ErrP )
			*ErrP = P;
		else
			ERRu();

	return Result;
}

bso::lfloat__ string_::ToLF(
	epeios::row__ *ErrP,
	epeios::row__ Begin ) const
{
	bso::bool__ Negate = false;
	bso::lfloat__ Result = 0;
	epeios::row__ &P = Begin;
	unsigned char C;

	if ( *P < Amount() ) {
		if ( P != NONE ) 
			if ( Get( P ) == '-' ) {
				Negate = true;
				P = Next( P );
			} else if ( Get( P ) == '+' ) {
				P = Next( P );
			}

		while( ( P != NONE ) && isdigit( C = Get( P ) ) && ( Result < ( BSO_ULONG_MAX / 10 ) ) ) {
			Result = Result * 10 + C - '0';
			P = Next( P );
		}

		if ( ( P != NONE )
			  && ( ( ( C = Get( P ) ) == '.' ) || ( C == ',' ) ) ) {
			bso::lfloat__ Factor = .1;
			P = Next( P );
			while( ( P != NONE ) && isdigit( C = Get( P ) ) ) {
				Result += ( C - '0' ) * Factor;
				Factor /= 10;
				P = Next( P );
			}
		}
	}

	if ( P != NONE )
		if ( ErrP )
			*ErrP = P;
		else
			ERRu();

	if ( Negate )
		return -Result;
	else
		return Result;
}


/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */
class strpersonnalization
: public strtutor
{
public:
	strpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~strpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the ending of the application  */
	}
};


/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

// 'static' by GNU C++.

static strpersonnalization Tutor;

ttr_tutor &STRTutor = Tutor;
