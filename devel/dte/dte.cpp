/*
  'dte' library by Claude L. Simon (csimon@webmails.com)
  Requires the 'dte' header file ('dte.h').
  Copyright (C) 2000,2001 Claude L. SIMON (csimon@webmails.com).

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

using namespace dte;

raw_date__ dte::date__::Convert_( bso::ulong__ Date )
{
	year__ Year;
	month__ Month;
	day__ Day;

	Year = this->Year( Date );
	Month = this->Month( Date );
	Day = this->Day( Date );

	if ( Year > 9999
		 || ( Day < 1 ) || ( Day > 31 )
		 || ( Month < 1 ) ||  ( Month > 12 ) )
		return 0;
	else
		return Date;
}

raw_date__ date__::Convert_( const char *Date )
{
	long Jour = 0, Mois = 0, Annee = 0;

	while( *Date && !isdigit( *Date ) )
		Date++;

	if ( !*Date )
		return 0;

	while( isdigit( *Date ) )
		Jour = Jour * 10 + *Date++ - '0';

	while( *Date && !isdigit( *Date ) )
		Date++;

	if ( !*Date )
		return 0;

	while( isdigit( *Date ) )
		Mois = Mois * 10 + *Date++ - '0';

	while( *Date && !isdigit( *Date ) )
		Date++;

	if ( !*Date )
		return 0;

	while( isdigit( *Date ) )
		Annee = Annee * 10 + *Date++ - '0';

	if ( Annee < 100 )
		if ( Annee >= DTE_LIMIT_DECENNIA )
			Annee += 1900;
		else
			Annee += 2000;

	return Convert_( Annee * 10000 + Mois * 100 + Jour );
}

const char *date__::ASCII( char *Result ) const
{
	static char Retour[11];

	if ( !Result )
		Result = Retour;

	if ( !RawDate_ )
		sprintf( Result, "invalid" );
	else
		sprintf( Result, "%02i/%02i/%i", (int)Day(), (int)Month(), (int)Year() );

	return Result;
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
