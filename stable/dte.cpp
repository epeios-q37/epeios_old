/*
	'dte' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Requires the 'dte' header file ('dte.h').
	Copyright (C) 2001,2004 Claude SIMON (http://zeusw.org/intl/contact.html).

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

#define DTE__COMPILATION

#include "dte.h"

class dtetutor
: public ttr_tutor
{
public:
	dtetutor( void )
	: ttr_tutor( DTE_NAME )
	{
#ifdef DTE_DBG
		Version = DTE_VERSION "\b\bD $";
#else
		Version = DTE_VERSION;
#endif
		Owner = DTE_OWNER;
		Date = "$Date$";
	}
	virtual ~dtetutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#include <ctype.h>
#include <time.h>


/* Structure of a date.

  A date is stored in a 'raw_date__', which is 32 bits long.

  Here is the description of each bit from less significant, to more significant.

  0		  (1)	: Day reliability (0: no, 1: yes)
  1		  (1)	: Month reliability (0: no, 1: yes)
  2		  (1)	: Year reliabilty (0: no, 1: yes)
  3 - 4   (2)	: Meaningfullness (?) of the year.
				  ( 00 : only millenary is significant (1xxx).
				    01 : only millenary and century is significant (19xx).
					10 : only millenary, century, and decade is significant (196x).
					11 : all the given year is significant (1969). )
  5 - 9   (5)	: day (0 to 31 ; 0 : don't bother or don't know, according to bit 0.
  10 - 13 (4)	: month (0 to 12 ; 0 : don't bother, don't know, according to bit 1)
  14 - 27 (14)	: year ( 0 to 16383 ; 0 don't bother or don't know, according to bit 2).
  28 - 30 (3)	: for future use.
  31	  (1)	: sign (0: before J.C., 1: after J.C.)

  NOTE : date before J.C. are not supported yet.

*/			



using namespace dte;

raw_date__ dte::date__::_Convert(
	day__ Day,
	month__ Month,
	year__ Year ) const
{
	if ( Year > ( ( 2 << 14 ) - 1 )
//		 || ( Month == 0 )
		 || ( Month > 12 )
//		 || ( Day == 0 )
		 || ( Day > GetAmountOfDaysInMonth( Month, Year ) ) )
		return DTE_INVALID_DATE;
	else
		return DTE_SIGN_MASK | ( Year << 9 | Month << 5 | Day ) << DTE_CORE_SHIFT | ( Year ? 1 << 2 : 0 ) | ( Month ? 1 << 1 : 0 ) | ( Day ? 1 : 0 );
}

#define LIMIT	( BSO_U32_MAX / 10 )

bso::u32__ ExtractItem_( const char *&Date )
{
	bso::u32__ Item = 0;

	if ( *Date == 0 )
		return 0;

	if( !isdigit( *Date ) )
		return 0;

	while( isdigit( *Date ) && ( Item < LIMIT ) )
		Item = Item * 10 + *Date++ - '0';

	if ( *Date == 0 )
		Date = NULL;

	return Item;
}

static bso::bool__ ExtractItems_(
	const char *&Date,
	bso::u32__ &Item1,
	bso::u32__ &Item2,
	bso::u32__ &Item3 )
{
	Item1 = 0;
	Item2 = 0;
	Item3 = 0;

	if ( Date == NULL )
		ERRFwk();

	if ( *Date == 0 )
		return false;

	Item1 = ExtractItem_( Date );

	if  ( Date == NULL )
		return false;

	if ( *Date && !isdigit( *Date ) )
		Date++;

	Item2 = ExtractItem_( Date );

	if  ( Date == NULL )
		return false;

	if ( *Date && !isdigit( *Date ) )
		Date++;

	Item3 = ExtractItem_( Date );

	return true;
}

static inline bso::bool__ IsSuitableForYear_( bso::u32__ Item )
{
	return true;
}

static inline bso::bool__ IsSuitableForMonth_( bso::u32__ Item )
{
	return ( ( Item > 0 ) && ( Item <= 12 ) );
}

static inline bso::bool__ IsSuitableForDay_( bso::u32__ Item )
{
	return ( ( Item > 0 ) && ( Item <= 31 ) );
}

static inline bso::bool__ TestAndSet_(
	day__ &Day,
	month__ &Month,
	year__ &Year,
	bso::u32__ DayCandidate,
	bso::u32__ MonthCandidate,
	bso::u32__ YearCandidate )
{
	if ( IsSuitableForDay_( DayCandidate ) )
		Day = (day__)DayCandidate;
	else
		return false;

	if ( IsSuitableForMonth_( MonthCandidate ) )
		Month = (month__)MonthCandidate;
	else
		return false;

	if ( IsSuitableForYear_( YearCandidate ) )
		Year = (year__)YearCandidate;
	else
		return false;

	return true;
}

raw_date__ dte::date__::_Convert(
	const char *&Date,
	format__ Format )
{
	day__ Day = 0;
	month__ Month = 0;
	year__ Year = 0;

	bso::u32__ Item1, Item2, Item3;

	Item1 = Item2 = Item3 = 0;

	if ( !ExtractItems_( Date, Item1, Item2, Item3 ) )
		return DTE_INVALID_DATE;

	if ( !Item1 || !Item2 )
		return DTE_INVALID_DATE;

	switch ( Format ) {
		case fDDMMYYYY:
			if ( !TestAndSet_( Day, Month, Year, Item1, Item2, Item3 ) )
				return DTE_INVALID_DATE;
			break;
		case fMMDDYYYY:
			if ( !TestAndSet_( Day, Month, Year, Item2, Item1, Item3 ) )
				return DTE_INVALID_DATE;
			break;
		case fYYYYMMDD:
			if ( !TestAndSet_( Day, Month, Year, Item3, Item2, Item1 ) )
				return DTE_INVALID_DATE;
			break;
		default:
			ERRPrm();
			break;
	}


	if ( Year < 100 )
		if ( Year >= DTE_DEFAULT_DECENNIA_LIMIT )
			Year += 1900;
		else
			Year += 2000;

	return _Convert( Day, Month, Year );
}

const char *dte::date__::ASCII(
	format__ Format,
	date_buffer__ &Buffer ) const
{
	if ( RawDate_ == DTE_INVALID_DATE )
		sprintf( Buffer.Data, "invalid" );
	else
		switch( Format ) {
		case fDDMMYYYY:
			sprintf( Buffer.Data, "%02i/%02i/%i", (int)Day(), (int)Month(), (int)Year() );
			break;
		case fMMDDYYYY:
			sprintf( Buffer.Data, "%02i/%02i/%i", (int)Month(), (int)Day(), (int)Year() );
			break;
		case fYYYYMMDD:
			sprintf( Buffer.Data, "%i-02i-%02i", (int)Year(), (int)Month(), (int)Day() );
			break;
		default:
			ERRPrm();
			break;
		}

	return Buffer.Data;
}

date__ dte::Now( void )
{
   struct tm *time_now;
   time_t secs_now;
   date__ Date;

   time(&secs_now);
   time_now = localtime(&secs_now);

   Date.Init( time_now->tm_mday, time_now->tm_mon + 1, time_now->tm_year + 1900 );

   return Date;
}



/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class dtepersonnalization
: public dtetutor
{
public:
	dtepersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~dtepersonnalization( void )
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

static dtepersonnalization Tutor;

ttr_tutor &DTETutor = Tutor;
