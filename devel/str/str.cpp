/*
  'str' library by Claude L. Simon (simon@epeios.org)
  Requires the 'str' header file ('str.h').
  Copyright (C) 2000,2001 Claude L. SIMON (simon@epeios.org).

  This file is part of the Epeios (http://www.epeios.org/) project.
  

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

namespace str {

	template <class ostream> static void Put_(
		const string_ &String,
		ostream &OStream )		// Can be optimized by using a buffer.
	{
		epeios::size__ Amount = String.Amount();

		if ( Amount != 0 )
		{
			epeios::row_t__ P = 0;

			while ( P < Amount )
				OStream.Put( String.Read( P++ ) );
		}
	}


	flw::oflow___ &operator <<(
		flw::oflow___ &OStream,
		const string_ &String )
	{
		Put_( String, OStream );

		OStream.Put( 0 );

		return OStream;
	}


	flw::iflow___ &operator >>(
		flw::iflow___ &IStream,
		string_ &S )
	{
		char C;

		while( ( C = IStream.Get() ) != 0 )
			S.Add( C );

		return IStream;
	}
		

	txf::text_oflow___ &operator <<(
		txf::text_oflow___ &OStream,
		const string_ &String )
	{
		Put_( String, OStream );

		return OStream;
	}

	// Convertit le contenu de 'String' en majuscule.
	void ToUpper( string_ &String )
	{
		epeios::row_t__ P = String.Amount();

		while ( P-- )
			String.Write( (char)toupper( String.Read( P ) ) , P );
	}

	// Convertit le contenu de 'String' en minuscule.
	void ToLower( string_ &String )
	{
		epeios::row_t__ P = String.Amount();

		while ( P-- )
			String.Write( (char)tolower( String.Read( P )) , P );
	}

	// Convertit la chaine 'char *' et rajoute un 0. Le pointeur retourné doit être libèré par un 'free'.
	char *string_::Convert(
		epeios::row__ Position,
		epeios::size__ Quantity ) const
	{
		char *Pointeur = NULL;
	ERRProlog
	ERRBegin
		if ( ( Quantity + *Position ) > Amount() )
			Quantity = Amount() - *Position;

		if ( ( Pointeur = (char *)malloc( Quantity + 1 ) ) == NULL )
			ERRa();

		Read( Position, Quantity, Pointeur );

		Pointeur[Quantity] = 0;
	ERRErr
		if ( Pointeur )
			free( Pointeur );
	ERREnd
	ERREpilog
		return Pointeur;
	}

	// Could be easily optimized, would be when I have some time.
	void string_::FilterOut( char Model )
	{
		epeios::row_t__ Source, Dest = 0;
		char Char;

		for( Source = 0; Source < Amount(); Source++ ) {
			if ( ( Char = Read( Source ) ) != Model )
				Write( Char, Dest++ );
		}

		Allocate( Dest );
	}

	// Could be easily optimized, would be when I have some time.
	void string_::Replace(
		char Old,
		char New )
	{
		epeios::row_t__ Source;
		char Char;

		for( Source = 0; Source < Amount(); Source++ ) {
			if ( ( Char = Read( Source ) ) == Old )
				Write( New, Source );
			else
				Write( Char, Source );
		}
	}


	epeios::row__ string_::Search(
		const string_ &S,
		epeios::row__ Start ) const
	{
		if ( S.Amount() <= Amount() ) {
			epeios::size__ Amount = S.Amount();
			epeios::row_t__ Limit = this->Amount() - Amount;

			while( ( *Start <= Limit )
				&& Compare( S, *this, 0, Start, Amount ) )
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
			&& ( Read( Start ) != C ) )
			(*Start)++;

		if ( *Start >= Limit )
			return NONE;
		else
			return Start;
	}

	bso::ulong__ string_::ToUL( epeios::row__ &ErrP ) const
	{
		bso::ulong__ Result = 0;
		epeios::row__ P = First();
		char C;

		while( ( P != NONE ) && isdigit( C = Read( P ) ) && ( Result < ( BSO_ULONG_MAX / 10 ) ) ) {
			Result = Result * 10 + C - '0';
			P = Next( P );
		}

		if ( &ErrP )
			ErrP = P;

		return Result;
	}

	bso::lfloat__ string_::ToLF( epeios::row__ &ErrP ) const
	{
		bso::lfloat__ Result = 0;
		epeios::row__ P = First();
		char C;

		while( ( P != NONE ) && isdigit( C = Read( P ) ) && ( Result < ( BSO_ULONG_MAX / 10 ) ) ) {
			Result = Result * 10 + C - '0';
			P = Next( P );
		}

		if ( ( P != NONE ) && ( Read( P ) == '.' ) ) {
			bso::lfloat__ Factor = .1;
			while( ( P != NONE ) && isdigit( C = Read( P ) ) && ( Result < ( BSO_ULONG_MAX / 10 ) ) ) {
				Result += ( C - '0' ) * Factor;
				Factor /= 10;
				P = Next( P );
			}
		}

		if ( &ErrP )
			ErrP = P;

		return Result;
	}

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
