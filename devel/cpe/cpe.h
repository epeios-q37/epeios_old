/*
	Header for the 'cpe' library by Claude SIMON (http://zeusw.org/intl/contact.html)
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

#ifndef CPE__INC
#define CPE__INC

#define CPE_NAME		"CPE"

#define	CPE_VERSION	"$Revision$"

#define CPE_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"

#include "ttr.h"

extern class ttr_tutor &CPETutor;

#if defined( XXX_DBG ) && !defined( CPE_NODBG )
#define CPE_DBG
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
//D ComPilation Enviroment 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

//#include "err.h"
//#include "flo.h"

# undef CPE__MS		// Plateforme Microsoft.
# undef CPE__BEOS	// Plateforme BeOS.
# undef CPE__MAC	// Plateforme MAC.
# undef CPE__CYGWIN	// Plateforme CYGWIN.
# undef CPE__MINGW	// Plateforme MINGW.
# undef CPE__LINUX	// Platform Linux

# undef CPE__VC		// Compilateur VC++
# undef CPE__VC6	// Compilateur VC++ V6
# undef CPE__VC8	// Compilateur VC++ V8 (2005)
# undef CPE__VC9	// Compilateur VC++ V9 (2008)
# undef CPE__GCC	// Compilateur GNU C++.
# undef CPE__GCC3	// Compilateur GNU C++ V3.x
# undef CPE__CW		// Compilateur CodeWarrior

# undef CPE__32BITS		// Cible 32 bits.
# undef CPE__MT			// Cible multitâche.
# undef CPE__CONSOLE	// Cible console.
# undef CPE__LIBRARY	// Cible bibliothèque (DLL, .so, ...)
# undef CPE__JAVA		// Cible composant java en code natif.

#undef CPE__64_BITS_TYPES_ALLOWED	// Les types 64 bits sont autorisés.

#ifndef CPE_64_BITS_FORBIDDEN
#	define CPE__64_BITS_TYPES_ALLOWED
#endif

# ifdef CPE_JAVA
#  define CPE__JAVA
#  define CPE__LIBRARY
# endif

#ifdef _MSC_VER
#	define CPE__MS
#	define CPE__VC
#	ifdef _WIN32
#		define CPE__32BITS
#	endif	
#	ifdef _MT
#		ifndef CPE_ST
#			define CPE__MT
#		endif
#	elif defined( CPE_MT )
#		error "'CPE_MT' is defined, but compiler options does not allow multitasking features."
#	endif
#endif

#ifdef __APPLE__
#	define CPE__MAC
#endif

#ifdef CPE__MAC
#	define CPE__MAC
#	define CPE__GCC
#endif


#ifdef __GNUC__
#	define CPE__GCC
#	ifdef __CYGWIN__
#		define CPE__CYGWIN
#	elif defined( __MINGW32__ )
#		define CPE__MINGW
#	elif defined( __BEOS__ )
#		define CPE__BEOS
#	elif defined( __linux__ )
#		define CPE__LINUX
#	endif
#endif

#ifdef __MWERKS__
#	define CPE__CW
#	ifdef _MT
#		ifndef CPE_ST
#			define CPE__MT
#		endif
#	elif defined( CPE_MT )
#		error "'CPE_MT' is defined, but compiler options does not allow multitasking features."
#	endif
#	ifdef macintosh
#		define CPE__MAC	// We are under Macintosh.
#	else
#		define CPE__MS	// We are under Microsoft OS
#	endif
#endif

#ifndef CPE__MT
#	ifdef CPE_MT 
#		define CPE__MT
#	endif
#endif

#ifndef CPE_USE_NO_WORKAROUNDS
#	define CPE__USE_WORKAROUNDS
#endif	

#ifdef CPE_LIBRARY
#	define CPE__LIBRARY
#elif defined( CPE__MS )
#	if defined( _USRDLL ) || defined( _WINDLL )
#		define CPE__LIBRARY
#	endif
#endif

#if defined( wxUSE_GUI ) || defined( CPE_GUI )
#	define CPE__GUI
#elif !defined( CPE__LIBRARY )
#	define CPE__CONSOLE
#endif

#ifndef CPE_DISABLE_WARNINNG_SUPPRESSION
#	define CPE__WARNING_SUPPRESSION_ENABLED
#endif



#ifdef CPE__VC
#	ifdef CPE__WARNING_SUPPRESSION_ENABLED
#		pragma warning( disable: 4786 )	// 'The identifier string exceeded the maximum allowable length and was truncated.'.
#	endif
#	if _MSC_VER == 1200
#		define CPE__VC6
#	elif _MSC_VER == 1400
#		define CPE__VC8
#	elif _MSC_VER == 1500
#		define CPE__VC9
#	endif
#endif

#ifdef CPE__C_GCC
#	if __GNUC__ == 3
#		define CPE__GCC3
#	endif
#endif

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
