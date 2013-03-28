/*
	Header for the 'cpe' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Copyright (C) 2000-2001, 2004 Claude SIMON (http://zeusw.org/intl/contact.html).

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

# undef CPE_MSVC	// Environnement Microsoft Visual C++.
# undef CPE_MAC		// Environnement MAC.
# undef CPE_CYGWIN	// Environnement CYGWIN.
# undef CPE_MINGW	// Environnement MINGW.
# undef CPE_LINUX	// Environnement Linux
# undef CPE_ANDROID	// Environnement Android.

# undef CPE_WIN		// Windows syst�me base API
# undef CPE_POSIX	// Posix syst�me base API.

# undef CPE_VC		// Compilateur VC++
# undef CPE_VC6		// Compilateur VC++ V6
# undef CPE_VC8		// Compilateur VC++ V8 (2005)
# undef CPE_VC9		// Compilateur VC++ V9 (2008)

# undef CPE_GCC		// Compilateur GNU C++.
# undef CPE_GCC3	// Compilateur GNU C++ V3.x

# undef CPE_MT			// Cible multit�che.
# undef CPE_CONSOLE		// Cible console.
# undef CPE_LIBRARY		// Cible biblioth�que (DLL, .so, ...)

# undef CPE_JAVA	// Cible composant java en code natif.
# undef CPE_GECKO	// Cible composant Gecko.

# undef CPE_X86		// Processeur Intel et compatible.
# undef CPE_ARM		// Processeur ARM.

# undef CPE_32BITS	// Processeur 32 bits.
# undef CPE_64BITS	// Processeur 64 bits.

# ifdef E_GECKO
#  define CPE_GECKO
#  define CPE_LIBRARY
# endif

# ifdef E_JAVA
#  define CPE_JAVA
#  define CPE_LIBRARY
# endif

# ifdef _MSC_VER
#  define CPE_MSVC
#  define CPE_WIN
#  define CPE_VC
#  ifdef _WIN64
#   define CPE_64BITS
#  elif defined( _WIN32 )
#   define CPE_32BITS
#  else
#   error "Unknown bitness".
#  endif	
#  ifdef _MT
#   ifndef E_ST
#    define CPE_MT
#   endif
#  elif defined( E_MT )
#   error "'E_MT' is defined, but compiler options does not allow multitasking features."
#  endif
# endif

# ifdef __APPLE__
#  define CPE_MAC
#  define CPE_GCC
#  define CPE_POSIX
# endif

# ifdef __GNUC__
#  define CPE_GCC
#  ifdef __CYGWIN__
#   define CPE_CYGWIN
#   define CPE_POSIX
#   ifdef __CYGWIN64__
#    define CPE_64BITS
#   elif defined( __CYGWIN32__ )
#    define CPE_32BITS
#   else
#    error "Unknown bitness".
#   endif	
#  elif defined( __MINGW32__ )
#   define CPE_MINGW
#   define CPE_WIN
#   ifdef _WIN64
#    define CPE_64BITS
#   elif defined( _WIN32 )
#    define CPE_32BITS
#   else
#    error "Unknown bitness".
#   endif	
#  elif defined( __linux__ )
#   define CPE_LINUX
#   define CPE_POSIX
#  endif
#  ifdef __ANDROID__
#   define CPE_ANDROID
#   if ( __SIZEOF_POINTER__ == 4 )
#    define CPE_32BITS
#   else
#    error "New biteness : patching required !"
#  endif
#  endif
# endif

# ifndef CPE_MT
#  ifdef E_MT 
#   define CPE_MT
#  endif
# endif

# ifndef E_USE_NO_WORKAROUNDS
#  define CPE_USE_WORKAROUNDS
# endif	

# ifdef E_LIBRARY
#  define CPE_LIBRARY
# elif defined( CPE_MSVC )
#  if defined( _USRDLL ) || defined( _WINDLL )
#   define CPE_LIBRARY
#  endif
# endif

# ifdef E_GUI
#  define CPE_GUI
# elif !defined( CPE_LIBRARY )
#  define CPE_CONSOLE
# endif

# ifndef E_DISABLE_WARNINNG_SUPPRESSION
#  define CPE_WARNING_SUPPRESSION_ENABLED
# endif

# ifdef CPE_VC
#  ifdef CPE_WARNING_SUPPRESSION_ENABLED
#   pragma warning( disable: 4786 )	// 'The identifier string exceeded the maximum allowable length and was truncated.'.
#  endif
#  if _MSC_VER == 1200
#   define CPE_VC6
#  elif _MSC_VER == 1400
#   define CPE_VC8
#  elif _MSC_VER == 1500
#   define CPE_VC9
#  endif
#  ifdef _M_IX86
#   define CPE_X86
#  endif
#  ifdef _M_X64
#   define CPE_X86
#  endif
# endif

# ifdef CPE_GCC
#  if __GNUC__ == 3
#   define CPE_GCC3
#  endif
#  ifdef __i386__
#   define CPE_X86
#  endif
#  ifdef __ARM_EABI__
#   define CPE_ARM
#  endif
#  ifdef __x86_64__
#   define CPE_X86
#   define CPE_64BITS
#  else
#   define CPE_32BITS
#  endif
# endif

# ifdef CPE_JAVA
#  ifdef CPE_CYGWIN
#   error "JNI doesn't work with 'Cygwin' genuine compiler. Use 'MinGW' compiler instead."
#  endif
# endif

# if !defined( CPE_X86 ) && !defined( CPE_ARM )
#  error "Unable to guess the processor !"
# endif

namespace cpe {

	const char *GetDescription( void );

}


/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
