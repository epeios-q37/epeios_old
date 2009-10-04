/*
	Header for the 'mtk' library by Claude SIMON (csimon@epeios.org)
	Copyright (C) $COPYRIGHT_DATES$Claude SIMON (csimon@epeios.org).
$_RAW_$
	This file is part of the Epeios (http://epeios.org/) project.

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

#ifndef MTK__INC
#define MTK__INC

#define MTK_NAME		"MTK"

#define	MTK_VERSION	"$Revision$"

#define MTK_OWNER		"Claude SIMON (csimon@epeios.org)"

#include "ttr.h"

extern class ttr_tutor &MTKTutor;

#if defined( XXX_DBG ) && !defined( MTK_NODBG )
#define MTK_DBG
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
//D MultiTasKing 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "cpe.h"
#include "errno.h"
#include "tht.h"

#if defined( CPE__T_LINUX ) || defined( CPE__T_CYGWIN ) || defined( CPE__T_BEOS ) || defined( CPE__T_MAC )
#	define MTK__POSIX
#elif defined( CPE__T_MS )
#	define MTK__MS
#else
#	error "Unknown target !"
#endif

#ifdef MTK_THREADS_REMAIN
#	define MTK__THREADS_REMAIN
#endif


#ifdef MTK__MS
#	include <process.h>
#	include <windows.h>
#elif defined( MTK__POSIX )
#	ifdef CPE__T_BEOS
#		include <be/kernel/OS.h>
#	else
#		include <pthread.h>
#	endif
#	include <unistd.h>
#	include <stdlib.h>
#	include <signal.h>
#else
#	error
#endif

#ifndef CPE__T_MT
#	error "Multitasking required, but compilation options don't allow this."
#endif

typedef void (* mtk__routine)(void *);

#ifdef MTK_KILL
#	ifdef MTK_KEEP
#		error "Both 'MTK_KILL' and 'MTK_KEEP' cannot be defined together."
#	else
#		define MTK__KILL
#	endif
#elif defined( MTK_KEEP )
#	define MTK__KEEP
#else
#	ifdef MTK__T_LINUX
#		define MTK__KEEP
#	else
#		define MTK__KILL
#	endif
#endif

namespace mtk {
	typedef void (* routine__)(void *);
	
	/*f Launch a new thread executing 'Routine', with 'UP' as user pointer.
	Thread is killed when returning from 'Routine'. */
	void LaunchAndKill(
		routine__ Routine,
		void *UP );
		
	/*f Launch a new thread executing 'Routine', with 'UP' as user pointer.
	Thread is NOI killed when returning from 'Routine', and reused if available
	at next call of this function. */
	void LaunchAndKeep(
		routine__ Routine,
		void *UP );


	//f Launch a new thread executing 'Routine', with 'UP' as user pointer.
	inline void Launch(
		routine__ Routine,
		void *UP )
	{
#ifdef MTK__KILL
		LaunchAndKill( Routine, UP );
#elif defined( MTK__KEEP )
		LaunchAndKeep( Routine, UP );
#else
#	error "None of 'MTK_KEEP' or 'MTK_KILL' are defined."
#endif
	}

	/*f Force the program to exit after 'Seconds' second.
	Usefull to force a server to exit to obtain the profiling file. */
	void ForceExit( unsigned long Seconds );

	using tht::thread_id__;
	using tht::GetTID;

}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
