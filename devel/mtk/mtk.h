/*
  Header for the 'mtk' library by Claude L. Simon (simon@epeios.org)
  Copyright (C) 2000,2001 Claude L. SIMON (simon@epeios.org) 

  This file is part of the Epeios (http://www.epeios.org/) project.
  

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

#ifndef MTK__INC
#define MTK__INC

#define MTK_NAME		"MTK"

#define	MTK_VERSION	"$Revision$"	

#define MTK_OWNER		"the Epeios project (http://www.epeios.org/)"

#include "ttr.h"

extern class ttr_tutor &MTKTutor;

#if defined( XXX_DBG ) && !defined( MTK_NODBG )
#define MTK_DBG 
#endif

/* Begin of automatic documentation generation part. */

//V $Revision$
//C Claude L. SIMON (simon@epeios.org)
//R $Date$

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#include "err.h"
#include "cpe.h"

#if defined( CPE__UNIX ) || defined( CPE__BEOS )
#	define MTK__UNIX_LIKE
#endif


#ifdef CPE__MS
#	include <process.h>
#	include <windows.h>
#elif defined( MTK__UNIX_LIKE )
#	ifdef CPE__BEOS
#		include <be/kernel/OS.h>
#	else
#		include <pthread.h>
#	endif
#	include <unistd.h>
#	include <stdlib.h>
#	include <signal.h>
#else
#	error "Unknow compilation enviroment"
#endif

#ifndef CPE__MT
#error "Multitasking required, but compilation options don't allow this."
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
#	define MTK__KILL
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


#ifdef CPE__MS
	//t The type of a thread ID.
	typedef DWORD	thread_id__;
#elif defined( MTK__UNIX_LIKE )
#	ifdef CPE__BEOS
	typedef thread_id	thread_id__;
#	else
	typedef pid_t	thread_id__;
#	endif
#else
#	error "Unknow compilation enviroment"
#endif

	//f Return an unique ID for the current thread.
	inline thread_id__ GetTID( void )
	{
#ifdef CPE__MS
		return GetCurrentThreadId();
#elif defined( MTK__UNIX_LIKE )
#	ifdef CPE__BEOS 
		thread_id ID = find_thread( NULL );
		
		if ( ID == B_NAME_NOT_FOUND )
			ERRs();
#	else
		thread_id__ ID = getpid();

		if ( ID == -1 )
			ERRs();
#	endif
		return ID;
#else
#error "Unknow compilation enviroment"
#endif
	}
};

//t Alias for 'mtk::routine' . Obsolete.
typedef mtk::routine__ mtk__routine;

//f Alias go 'mtk::Launch'. Obsolete.
inline void MTKLaunch(
	mtk__routine Routine,
	void *UP )
{
	mtk::Launch( Routine, UP );
}

//t Alias for mtk::thread_id__. Obsolete.
typedef mtk::thread_id__	mtk__thread_id;

//f Alias for 'mtk::GetTid'. Obsolete.
inline mtk__thread_id MTKGetTID( void )
{
	return mtk::GetTID();
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
