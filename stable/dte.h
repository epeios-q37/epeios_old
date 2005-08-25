/*
	Header for the 'dte' library by Claude SIMON (csimon@epeios.org)
	Copyright (C) 2001,2004 Claude SIMON (csimon@epeios.org).

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

#ifndef DTE__INC
#define DTE__INC

#define DTE_NAME		"DTE"

#define	DTE_VERSION	"$Revision$"

#define DTE_OWNER		"Claude SIMON (csimon@epeios.org)"

#include "ttr.h"

extern class ttr_tutor &DTETutor;

#if defined( XXX_DBG ) && !defined( DTE_NODBG )
#define DTE_DBG
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
//D DaTE 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

/* Addendum to the automatic documentation generation part. */
//D DaTE.
/* End addendum to automatic documentation generation part. */


#include "err.h"
#include "flw.h"
#include "bso.h"

//d An invalid date.
#define DTE_INVALID_DATE	0

/*d Decennia under which we consider we are in the XXI century,
and over which we consider we are in the XX century. */
#define DTE_DEFAULT_DECENNIA_LIMIT	90

#define DTE_CORE_SHIFT	5

#define DTE_SIGN_BIT_POSITION	31

#define DTE_SIGN_MASK	( 1 << DTE_SIGN_BIT_POSITION )

#define DTE_CORE_MASK	( ( ~0 << DTE_CORE_SHIFT ) & ( ~DTE_SIGN_MASK ) )


namespace dte {
	//t Type of a raw date.
	typedef bso::ulong__ raw_date__;

	//t Type for the year.
	typedef bso::ushort__	year__;

	//t Type for the month.
	typedef bso::ubyte__	month__;

	//t Type of the day.
	typedef bso::ubyte__ day__;


	struct date_buffer__
	{
		char Data[11];
		date_buffer__( void )
		{
			Data[0] = 0;
		}
	};
	
	enum format__ {
		fDDMMYYYY,	// à la française ('25/11/2003').
		fMMDDYYYY,	// à l'anglaise ('11/25/2003').
		f_amount,
		f_Undefined
	};

	//c A date.
	class date__ {
		// The raw date. See '.cpp' for structure.
		raw_date__ RawDate_;
		/* Convert 'Date' (in 'yyyymmdd' format) into internal format.
		Return 'STXDTE_INVALID_DATE' if date not valid, or the converted date */
		raw_date__ _Convert(
			day__ Day,
			month__ Month,
			year__ Year ) const;
		/* Covert Date (in "dd.mm.yyyy" ) format into internal format.
		Return 'STXDTE_INVALID_DATE' if date not valid, or the converted date */
		raw_date__ _Convert( const char *Date );
		// Return the core date.
		bso::ulong__ _Core( raw_date__ Date ) const
		{
			return ( RawDate_ & DTE_CORE_MASK) >> DTE_CORE_SHIFT;
		}
		// Return the year of 'RawDate'.
		year__ _Year( raw_date__ RawDate ) const
		{
			return (year__)( _Core( RawDate ) >> 9 );
		}
		// Return the month of the date.
		month__ _Month( raw_date__ RawDate ) const
		{
			return (month__)( ( _Core( RawDate ) >> 5 ) & 0xf );
		}
		// Return the day of 'RawDate'.
		day__ _Day( raw_date__ RawDate ) const
		{
			return (day__)( _Core( RawDate ) & 0x1f );
		}
		// Return true if 'Date' is a simplified raw date (yyyymmdd).
		bso::bool__ _IsSimplifiedRawDate( bso::ulong__ Date ) const
		{
			return ( Date != DTE_INVALID_DATE ) && ( ( Date & DTE_SIGN_MASK ) == 0 );
		}
		// Convert a simplified raw date (yyyymmdd) to raw date.
		raw_date__ _Convert( bso::ulong__ Date ) const
		{
			if ( _IsSimplifiedRawDate( Date ) )
				return _Convert( (day__)( Date % 100 ), (month__)( ( Date % 10000 ) / 100 ), (year__)( Date / 10000 ) );
			else
				return Date;
		}
	 public:
		void reset( bso::bool__ = true )
		{
			RawDate_ = DTE_INVALID_DATE;
		}
		date__( bso::ulong__ Date = DTE_INVALID_DATE )
		{
			reset( false );

			RawDate_ = _Convert( Date );
		}
		date__( const char *Date )
		{
			reset( false );

			RawDate_ = _Convert( Date );
		}
		date__(
			day__ Day,
			month__ Month,
			year__ Year )
		{
			reset( false );

			RawDate_ = _Convert( Day, Month, Year );
		}
		//f Initialization with date 'Date'.
		void Init( bso::ulong__ Date = DTE_INVALID_DATE )
		{
			reset();

			RawDate_ = _Convert( Date );
		}
		//f Initialization with date 'Date'.
		void Init( const char *Date  )
		{
			reset();

			RawDate_ = _Convert( Date );
		}
		//f Initialization with 'Day', 'Month' and 'Year'.
		void Init( 
			day__ Day,
			month__ Month,
			year__ Year )
		{
			reset();

			RawDate_ = _Convert( Day, Month, Year );
		}
		//f Return the date in raw format.
		operator unsigned long( void ) const
		{
			return RawDate_;
		}
		//f Return simplified raw date 'yyyymmdd'.
		unsigned long GetSimplifiedRawDate( void ) const
		{
			return _Year( RawDate_ ) * 10000 + _Month( RawDate_ ) * 100 + _Day( RawDate_ );
		}
		year__ Year( raw_date__ Date = DTE_INVALID_DATE ) const
		{
			return _Year( Date == DTE_INVALID_DATE ? RawDate_ : Date );
		}
		month__ Month( raw_date__ Date = DTE_INVALID_DATE ) const
		{
			return _Month( Date == DTE_INVALID_DATE ? RawDate_ : Date );
		}
		day__ Day( raw_date__ Date = DTE_INVALID_DATE ) const
		{
			return _Day( Date == DTE_INVALID_DATE ? RawDate_ : Date );
		}
		//f Return the date in ASCII ('dd/mm/yyyy') and put in 'Result' if != 'NULL'.
		const char *ASCII(
			format__ Format,
			date_buffer__ &Buffer ) const;
#ifndef CPE__MT
		const char *ASCII( format__ Format ) const
		{
			static date_buffer__ Buffer;

			return ASCII( Format, Buffer );
		}
#endif
		void Date(
			day__ Day,
			month__ Month,
			year__ Year )
		{
			RawDate_ = _Convert( Day, Month, Year );
		}
		//f Add 'Amount' Month.
		void AddMonth( bso::ulong__ Amount = 1 )
		{
			bso::ulong__ Month = this->Month() + Amount - 1; 

			RawDate_ = _Convert( this->Day(), (month__)( Month % 12 + 1 ), (year__)( Year() + Month / 12 ) );
		}
		//f Return true if the date is valid, false otherwise.
		bso::bool__ IsValid( void ) const
		{
			return RawDate_ != DTE_INVALID_DATE;
		}
		//f Return true if date is in a leap year, false otherwise.
		bso::bool__ IsLeapYear( year__ Year = 0 ) const
		{
			if ( Year == 0 )
				Year = this->Year();
		    return ( ( ( Year % 4 ) == 0 )
					   && ( ( Year % 100 ) != 0 ) )
					 || ( ( Year % 400 ) == 0 );
		}
		day__ GetAmountOfDaysInMonth(
			month__ Month = 0,
			year__ Year = 0 ) const
		{
			if ( Month == 0 )
				Month = this->Month();

			if ( Year == 0 )
				Year = this->Year();

			switch ( Month ) {
			case 2: // Februrary
				if ( IsLeapYear( Year ) )
					return 29;
				else
					return 28;
				break;
			case 4: // April Falls Through
			case 6: // June
			case 9: // September
			case 11: // November
				return 30;
				break;
			default:
				return 31;
			}
	    } 
		void AddDay( void )
		{
			if ( Day() == GetAmountOfDaysInMonth() ) {
				Date( 1, Month(), Year() );
				AddMonth();
			} else
				Date( Day() + 1, Month(), Year() );
		}
		// Sunday == 0.
		day__ GetDayInWeek(
			day__ Day = 0,
			month__ Month = 0,
			year__ Year = 0 )
		{
			if ( Day == 0 )
				Day = this->Day();

			if ( Month == 0 )
				Month = this->Month();

			if ( Year == 0 )
				Year = this->Year();

			if ( Month < 3 ) {
				Month += 12;
				Year -= 1;
			}
			return ( ( Day + 1 + ( Month * 2 ) + ( int )( ( Month + 1 ) * 3 / 5 )
				       + Year + (int)( Year / 4 ) - (int)( Year / 100)
					   + (int) ( Year / 400 ) ) % 7 );
		}
	};

	//f Return sign of 'Date1' - 'Date2'.
	inline bso::sign__ Compare(
		const date__ &Date1,
		const date__ &Date2 )
	{
		return (bso::sign__)(bso::sbyte__)( Date1 - Date2 );
	}

	inline txf::text_oflow__ &operator <<(
		txf::text_oflow__ &Flow,
		date__ Date )
	{
		date_buffer__ Buffer;

		return Flow << Date.ASCII( fDDMMYYYY, Buffer );
	}

}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
