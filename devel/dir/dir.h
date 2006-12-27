/*
	Header for the 'dir' library by Claude SIMON (csimon@epeios.org)
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

#ifndef DIR__INC
#define DIR__INC

#define DIR_NAME		"DIR"

#define	DIR_VERSION	"$Revision$"

#define DIR_OWNER		"Claude SIMON (csimon@epeios.org)"

#include "ttr.h"

extern class ttr_tutor &DIRTutor;

#if defined( XXX_DBG ) && !defined( DIR_NODBG )
#define DIR_DBG
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
//D DIRectory. 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "cpe.h"
#include "tol.h"

#if defined( CPE__T_LINUX ) || defined( CPE__T_CYGWIN ) || defined( CPE__T_BEOS ) || defined( CPE__T_MAC )
#	define DIR__POSIX
#elif defined( CPE__T_MS )
#	define DIR__MS
#else
#	error "Unknown target !"
#endif


#ifdef DIR__MS
#	include <direct.h>
#elif defined( DIR__POSIX )
#	include <sys/stat.h>
#else
#	error
#endif

#include <errno.h>

namespace dir {
	enum state__ {
		sOK,
		sExists,		// Le répertoire existe déjà.
		sBadPath,		// Le chemin fourinit n'est pas correct.
		sIncorrectPath,	// Un élment du chemin n'existe pas ou n'est pas un répertoire adapté (fichier, mauvais droits, ...).
		s_amount,
		s_Undefined
	};

	inline state__ CreateDir( const char *Path )
	{
#ifdef DIR__MS
		switch ( _mkdir( Path ) ) {
#elif defined( DIR__POSIX )
		switch ( mkdir( Path, 0777 ) ) {
#else
#	error
#endif
		case 0:
			return sOK;
			break;
		case -1:
			switch ( errno ) {
			case EEXIST:
				return sExists;
				break;
			case ENOENT:
#ifdef DIR__POSIX
			case EPERM:
			case EACCES:
			case ENOTDIR:
			case EROFS:
			case ELOOP:
#endif
				return sIncorrectPath;
				break;
#ifdef DIR__POSIX
			case ENAMETOOLONG:
				return sBadPath;
				break;
			case EFAULT:
				ERRu();
				break;
#endif
			default:
				ERRs();
				break;
			}
		default:
			ERRs();
			break;
		}

		return s_Undefined;	// Pour éviter un 'warning'.
	}

	inline state__ DropDir( const char *Path )
	{
#pragma message ( __LOC__ "Gestion des valeurs de retours à revoir !" )
#ifdef DIR__MS
		switch ( _rmdir( Path ) ) {
#elif defined( DIR__POSIX )
		switch ( rmdir( Path ) ) {
#else
#	error
#endif
		case 0:
			return sOK;
			break;
		case -1:
			switch ( errno ) {
			case EEXIST:
				return sExists;
				break;
			case ENOENT:
#ifdef DIR__POSIX
			case EPERM:
			case EACCES:
			case ENOTDIR:
			case EROFS:
			case ELOOP:
#endif
				return sIncorrectPath;
				break;
#ifdef DIR__POSIX
			case ENAMETOOLONG:
				return sBadPath;
				break;
			case EFAULT:
				ERRu();
				break;
#endif
			default:
				ERRs();
				break;
			}
		default:
			ERRs();
			break;
		}

		return s_Undefined;	// Pour éviter un 'warning'.
	}
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
