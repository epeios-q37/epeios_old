/*
	Header for the 'cpe' library by Claude SIMON (csimon@epeios.org)
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

#ifndef CPE__INC
#define CPE__INC

#define CPE_NAME		"CPE"

#define	CPE_VERSION	"$Revision$"

#define CPE_OWNER		"Claude SIMON (csimon@epeios.org)"

#include "ttr.h"

extern class ttr_tutor &CPETutor;

#if defined( XXX_DBG ) && !defined( CPE_NODBG )
#define CPE_DBG
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
//D ComPilation Enviroment 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

//#include "err.h"
//#include "flo.h"

#undef CPE__P_MS		// Plateforme Microsoft.
#undef CPE__P_BEOS		// Plateforme BeOS.
#undef CPE__P_MAC		// Plateforme MAC.
#undef CPE__P_CYGWIN	// Plateforme CYGWIN.
#undef CPE__P_LINUX		// Platform Linux

#undef CPE__C_VC		// Compilateur VC++
#undef CPE__C_VC6		// Compilateur VC++ V6.0
#undef CPE__C_GCC		// Compilateur GNU C++.
#undef CPE__C_GCC3		// Compilateur GNU C++ V3.x
#undef CPE__C_CW		// Compilateur CodeWarrior
#undef CPE__C_VER		// Version du compilateur.

#undef CPE__T_MS		// Cible Microsoft (DOS, Windows, ...).
#undef CPE__T_LINUX		// Cible Linux
#undef CPE__T_CYGWIN	// Cible CYGWIN
#undef CPE__T_32		// Cible 32 bits.
#undef CPE__T_MT		// Cible multitâche.
#undef CPE__T_CONSOLE	// Cible console.
#undef CPE__T_GUI		// Cible GUI
#undef CPE__T_LIBRARY	// Cible bibliothèque (DLL, .so, ...)

#ifdef _MSC_VER
#	define CPE__P_MS
#	define CPE__C_VC
#	ifdef _WIN32
#		define CPE__T_32
#	endif	
#	ifdef _MT
#		ifndef CPE_ST
#			define CPE__T_MT
#		endif
#	elif defined( CPE_MT )
#		error "'CPE_MT' is defined, but compiler options does not allow multitasking features."
#	endif
#endif

#ifdef __GNUC__
#	define CPE__C_GCC
#	ifdef __CYGWIN__
#		define CPE__P_CYGWIN
#	elif defined( __BEOS__ )
#		define CPE__P_BEOS
#	elif defined( __linux__ )
#		define CPE__P_LINUX
#	endif
#endif

#ifdef __MWERKS__
#	define CPE__C_CW
#	ifdef _MT
#		ifndef CPE_ST
#			define CPE__T_MT
#		endif
#	elif defined( CPE_MT )
#		error "'CPE_MT' is defined, but compiler options does not allow multitasking features."
#	endif
#	ifdef macintosh
#		define CPE__P_MAC	// We are under Macintosh.
#	else
#		define CPE__P_MS	// We are under Microsoft OS
#	endif
#endif

#ifdef CPE__P_MS
#	define CPE__T_MS
#endif

#ifndef CPE__T_MT
#	ifdef CPE_MT 
#		define CPE__T_MT
#	endif
#endif

#ifndef CPE_USE_NO_WORKAROUNDS
#	define CPE__USE_WORKAROUNDS
#endif	

#ifdef CPE_LIBRARY
#	define CPE__T_LIBRARY
#elif defined( CPE__T_MS )
#	if defined(_USRDLL )
#		define CPE__T_LIBRARY
#	endif
#endif

#if defined( wxUSE_GUI ) || defined( CPE_GUI )
#	define CPE__T_GUI
#elif !defined( _USRDLL ) && !defined( CPE__T_LIBRARY )
#	define CPE__T_CONSOLE
#endif

#ifndef CPE_DISABLE_WARNINNG_SUPPRESSION
#	define CPE__WARNING_SUPPRESSION_ENABLED
#endif



#ifdef CPE__C_VC
#	ifdef CPE__WARNING_SUPPRESSION_ENABLED
#		pragma warning( disable: 4786 )	// 'The identifier string exceeded the maximum allowable length and was truncated.'.
#	endif
#	if _MSC_VER == 1200
#		define CPE__C_VC6
#	endif
#endif

#ifdef CPE__C_GCC
#	if __GNUC__ == 3
#		define CPE__C_GCC3
#	endif
#endif

#ifdef CPE__P_CYGWIN
#	define CPE__T_CYGWIN
#	define CPE__T_MS
#	ifndef CPE_ST
#		define CPE__T_MT 
#	endif
#endif

#ifdef CPE__P_LINUX
#	define CPE__T_LINUX
#endif


/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
