/*
	'cpe' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Requires the 'cpe' header file ('cpe.h').
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

#define CPE__COMPILATION

#include "cpe.h"

class cpetutor
: public ttr_tutor
{
public:
	cpetutor( void )
	: ttr_tutor( CPE_NAME )
	{
#ifdef CPE_DBG
		Version = CPE_VERSION "\b\bD $";
#else
		Version = CPE_VERSION;
#endif
		Owner = CPE_OWNER;
		Date = "$Date$";
	}
	virtual ~cpetutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#include "strng.h"

using namespace cpe;

static const char *Processor_( void )
{
#ifdef CPE__X86
	return "x86";
#elif defined( ARM )
	return "ARM";
#else
# error
#endif
}

static const char *Size_( void )
{
#ifdef CPE__32BITS
	return "32";
#elif defined( CPE__64BITS )
	return "64";
#else
# error
#endif
}

static const char *Enviroment_( void )
{
#if defined( CPE__MSVC )
	return "MSVC";
#elif defined( CPE__MAC )
	return "MacOS";
#elif defined( CPE__CYGWIN )
	return "Cygwin";
#elif defined( CPE__MINGW )
	return "MinGW";
#elif defined( CPE__LINUX )
	return "GNU/Linux";
#elif defined( CPE__ANDROID )
	return "Android";
#else
	return "Unknown";
#endif

}

const char *cpe::GetDescription( void )
{
	static char Buffer[100];
	Buffer[0] = 0;

	strcat( Buffer, Enviroment_() );
	strcat( Buffer, "_" );
	strcat( Buffer, Processor_() );
	strcat( Buffer, "-" );
	strcat( Buffer, Size_() );

	return Buffer;
}


/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */
class cpepersonnalization
: public cpetutor
{
public:
	cpepersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~cpepersonnalization( void )
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

static cpepersonnalization Tutor;

ttr_tutor &CPETutor = Tutor;
