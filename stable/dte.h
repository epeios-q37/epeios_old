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

//d An undefined date.
#define DTE_UNDEFINED_DATE	0

//d An invalid date.
#define DTE_INVALID_DATE	(raw_date__)-1

/*d Decennia under which we consider we are in the XXI century,
and over which we consider we are in the XX century. */
#define DTE_LIMIT_DECENNIA	90

#define DTE_SHIFT	5

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
		// The raw date, in 'yyyymmdd' format.
		raw_date__ RawDate_;
		/* Convert 'Date' (in 'yyyymmdd' format) into internal format.
		Return 'STXDTE_INVALID_DATE' if date not valid, or the converted date */
		raw_date__ Convert_( bso::ulong__ Date );
		/* Covert Date (in "dd.mm.yyyy" ) format into internal format.
		Return 'STXDTE_INVALID_DATE' if date not valid, or the converted date */
		raw_date__ Convert_( const char *Date );
	 public:
		void reset( bso::bool__ = true )
		{
			RawDate_ = DTE_INVALID_DATE;
		}
		date__( bso::ulong__ Date = DTE_UNDEFINED_DATE )
		{
			reset( false );
			RawDate_ = Convert_( Date );
		}
		date__( const char *Date )
		{
			reset( false );
			RawDate_ = Convert_( Date );
		}
		date__(
			day__ Day,
			month__ Month,
			year__ Year )
		{
			reset( false );

			if ( Year < 100 )
				if ( Year >= DTE_LIMIT_DECENNIA )
					Year += 1900;
				else
					Year += 2000;

				RawDate_ = date__( Day + (bso::ulong__)Month * 100 + (bso::ulong__)Year * 10000UL ).RawDate_;
		}
		//f Initialization with date 'Date'.
		void Init( bso::ulong__ Date = DTE_UNDEFINED_DATE )
		{
			reset();
			RawDate_ = Convert_( Date );
		}
		//f Initialization with date 'Date'.
		void Init( const char *Date  )
		{
			reset();
			RawDate_ = Convert_( Date );
		}
		//f Return the date in raw format ('yyyymmdd').
		operator unsigned long( void ) const
		{
			return RawDate_;
		}
		year__ Year( bso::ulong__ Date = DTE_INVALID_DATE ) const
		{
			if ( Date == DTE_INVALID_DATE )
				Date = RawDate_ >> DTE_SHIFT;

			return (year__)( Date / 10000L );
		}
		month__ Month( bso::ulong__ Date = DTE_INVALID_DATE ) const
		{
			if ( Date == DTE_INVALID_DATE )
				Date = RawDate_ >> DTE_SHIFT;

			return (month__)( ( Date % 10000L ) / 100L );
		}
		day__ Day( bso::ulong__ Date = DTE_INVALID_DATE ) const
		{
			if ( Date == DTE_INVALID_DATE )
				Date = RawDate_ >> DTE_SHIFT;

			return (day__)( Date % 100L );
		}
		//f Return the date in ASCII ('dd/mm/yyyy') and put in 'Result' if != 'NULL'.
		const char *ASCII( char *Result = NULL ) const;
		void Date(
			day__ Day,
			month__ Month,
			year__ Year )
		{
			RawDate_ = date__( Day, Month, Year ).RawDate_ << DTE_SHIFT;
		}
		//f Add 'Amount' Month.
		void AddMonth( bso::ulong__ Amount )
		{
			bso::ulong__ Month = this->Month() + Amount - 1; 

			Date( this->Day(), (month__)( Month % 12 + 1 ), (year__)( Year() + Month / 12 ) );
		}
		//f Return true if the date is valid, false potherwise.
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

	inline txf::text_oflow___ &operator <<(
		txf::text_oflow___ &Flow,
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
