/*
	Header for the 'cpe' library by Claude SIMON (csimon@epeios.org)
	Copyright (C) 2000-2003  Claude SIMON (csimon@epeios.org).

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

#undef CPE__VC
#undef CPE__MS
#undef CPE__32
#undef CPE__MT
#undef CPE__BC
#undef CPE__MS
#undef CPE__GCC
#undef CPE__DJGPP
#undef CPE__UNIX
#undef CPE__BEOS
#undef XPE__CW

#ifdef _MSC_VER
	//d If defined, we are in the 'Microsoft Visual C++'.
#	define CPE__VC	// Visual C++.
	//d If defined, we are in an Microsoft enviroment (DOS, Windows).
#	define CPE__MS
#	ifdef _WIN32
	//d If defined, we are in a Windows 32 bits enviroment (Window 9x, 2000, NT, ...).
#	define CPE__32
#	endif	
#	ifdef _MT
	//d If defined, we are in a multitasking enviroment.
#		define CPE__MT
#	elif defined( CPE_MT )
#		error "'CPE_MT' is defined, but compiler options does not allow multitasking features."
#	endif
#endif

#ifdef __BORLANDC__
	//d If defined, we are in the 'Borland C++'.
#	define CPE__BC
#	define CPE__MS
#	ifdef __WIN32__
#		define CPE__32
#	endif
#	ifdef __MT__
#		define CPE__MT
#	elif defined( CPE_MT )
#		error "'CPE_MT' is defined, but compiler options does not allow multitasking features."
#	endif
#endif

#ifdef __GNUC__
	//d If defined, we are using 'GCC'.
#	define CPE__GCC
#	ifdef __DJGPP__
		//d If defined, we are using 'DJGPP'.
#		define CPE__DJGPP
#		ifdef __MSDOS__
#			define CPE__MS
#		elif defined( __unix__ )
#			define CPE__UNIX
#		endif
#	elif defined( __unix__ )
#		define CPE__UNIX
#	elif defined( __BEOS__ )
#		define CPE__BEOS
#	endif
#	if __GNUC__ == 3
//d Defined if 'ios::nocreate' and 'ios::noreplace'.
#		define CPE__NO_IOS_EXTENSION
#	endif
#endif

#ifdef __MWERKS__
#	define CPE__CW	// We are using MetroWerks Code Warrior.
#	define CPE__MS
#endif

#ifndef CPE__NO_IOS_EXTENSION
//d Defined if 'ios::nocreate' and 'ios::replace' exists.
#	define CPE__IOS_EXTENSION
#endif

#ifndef CPE__MT
#	ifdef CPE_MT 
#		define CPE__MT
#	endif
#endif

#ifndef CPE_USE_NO_WORKAROUND
#	ifdef CPE__GCC
#		define CPE__USE_GCC_WORKAROUND
#	endif
#	ifdef CPE__VC
#		define CPE__USE_VC_WORKAROUND
#	endif
#	ifdef CPE__CW
#		define CPE__USE_CW_WORKAROUND
#	endif
#endif	

#if defined( CPE_PROCESSES ) && !defined( CPE__MT )
#	error "'CPE_PROCESSES' is defined, but current options doesn't allow multithreading."
#endif


#if defined( CPE__MS ) && defined( CPE__PROCESSES )
#	error "Processes are not allowed with MS compiler."
#endif

#ifdef CPE_PROCESSES
#	define CPE__PROCESSES
#endif 

#ifdef CPE_THREADS_REMAINS
#	define CPE__THREADS_REMAINS
#endif

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
