/*
	Header for the 'dte' library by Claude SIMON (csimon@epeios.org)
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

//d An undefined date.
#define DTE_UNDEFINED_DATE	0

//d An invalid date.
#define DTE_INVALID_DATE	(raw_date__)-1

/*d Decennia under which we consider we are in the XXI century,
and over which we consider we are in the XX century. */
#define DTE_LIMIT_DECENNIA	90

#define DTE_CORE_SHIFT	5

#define DTE_CORE_MASK	0xFFFFFE0

namespace dte {
	//t Type of a raw date.
	typedef bso::ulong__ raw_date__;

	//t Type for the year.
	typedef bso::ushort__	year__;

	//t Type for the month.
	typedef bso::ubyte__	month__;

	//t Type of the day.
	typedef bso::ubyte__ day__;
	

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
	 public:
		void reset( bso::bool__ = true )
		{
			RawDate_ = DTE_INVALID_DATE;
		}
		date__( raw_date__ Date = DTE_UNDEFINED_DATE )
		{
			reset( false );
			RawDate_ = Date;
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
		void Init( raw_date__ RawDate = DTE_UNDEFINED_DATE )
		{
			reset();
			RawDate_ = RawDate;
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
			RawDate_ = _Convert( Day, Month, Year );
		}
		//f Return the date in raw format ('yyyymmdd').
		operator unsigned long( void ) const
		{
			return RawDate_;
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
		const char *ASCII( char *Result = NULL ) const;
		void Date(
			day__ Day,
			month__ Month,
			year__ Year )
		{
			RawDate_ = _Convert( Day, Month, Year );
		}
		//f Add 'Amount' Month.
		void AddMonth( bso::ulong__ Amount )
		{
			bso::ulong__ Month = this->Month() + Amount - 1; 

			RawDate_ = _Convert( this->Day(), (month__)( Month % 12 + 1 ), (year__)( Year() + Month / 12 ) );
		}
		//f Return true if the date is valid, false otherwise.
		bso::bool__ IsValid( void ) const
		{
			return RawDate_ != DTE_INVALID_DATE;
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
		return Flow << Date.ASCII();
	}

}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
