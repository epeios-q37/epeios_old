/*
	Header for the 'dir' library by Claude SIMON (csimon at zeusw dot org)
	Copyright (C) $COPYRIGHT_DATES$Claude SIMON.
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

#ifndef DIR__INC
#define DIR__INC

#define DIR_NAME		"DIR"

#define	DIR_VERSION	"$Revision$"

#define DIR_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &DIRTutor;

#if defined( XXX_DBG ) && !defined( DIR_NODBG )
#define DIR_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision$
//C Claude SIMON (csimon at zeusw dot org)
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
#include "fnm.h"

#include <limits.h>

#if defined( CPE__LINUX ) || defined( CPE__CYGWIN ) || defined( CPE__BEOS ) || defined( CPE__MAC )
#	define DIR__POSIX
#elif defined( CPE__MS )
#	define DIR__MS
#else
#	error "Unknown target !"
#endif


#ifdef DIR__MS
#	include <direct.h>
#	include <windows.h>
# define DIR_PATH_MAX_SIZE	MAX_PATH
#elif defined( DIR__POSIX )
# ifdef CPE__MAC
#  include <mach-o/dyld.h>
#  include <sys/param.h>
# endif
#	include <unistd.h>
#	include <sys/stat.h>
#	include <dirent.h>
# define DIR_PATH_MAX_SIZE	PATH_MAX
#else
#	error
#endif

#include <errno.h>

namespace dir {
	enum state__ {
		sOK,
		sExists,		// Le répertoire existe déjà.
		sBadPath,		// Le chemin fournit n'est pas correct.
		sIncorrectPath,	// Un élment du chemin n'existe pas ou n'est pas un répertoire adapté (fichier, mauvais droits, ...).
		s_amount,
		s_Undefined
	};

	typedef tol::E_FPOINTER___(bso::char__) buffer___;
#	define DIR_BUFFER___ dir::buffer___



	inline const char *GetSelfPath( DIR_BUFFER___ &Buffer )
	{
#ifdef DIR__MS
		char Path[MAX_PATH];
		DWORD Size = GetModuleFileNameA( NULL, Path, sizeof( Path ) );
#endif
#ifdef DIR__POSIX
# ifdef CPE__MAC
		char Path[MAXPATHLEN];
		uint32_t Size = sizeof( Path );
		switch ( _NSGetExecutablePath( Path, &Size ) ) {
		case -1 :	// La taille de 'Path' est insuffisante.
			ERRl();
			break;
		case 0:	// Succés.
			break;
		default:
			ERRs();
			break;
		}

# else	// Ne fonctionne peur-être pas sur tous les sytèmes POSIX, mais du moins avec 'GNU/Linux' et 'Cygwin'.
		char Path[PATH_MAX];
		int Size = readlink( "/proc/self/exe", Path, sizeof( Path ) );

		// Valeur d"erreur retournée par 'GetModuleFileName(..)'.
		// Valeur d'erreur retrounée par 'readlink(...)', mais '0' est normalement une impossibilité.
		if ( Size <= 0 )
			ERRs();

		if ( Size == sizeof( Path ) )
			ERRl();

		Path[Size] = 0;	//'readlink(...) ne rajoute pas le '\0' final.
# endif
#endif
		return fnm::GetLocation( Path, Buffer );
	}

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

	inline state__ ChangeDir( const char *Path )
	{
#pragma message ( __LOC__ "Gestion des valeurs de retours à revoir !" )
#ifdef DIR__MS
		switch ( _chdir( Path ) ) {
#elif defined( DIR__POSIX )
		switch ( chdir( Path ) ) {
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

#ifdef DIR__MS
	typedef HANDLE	handle___;
#	define DIR_INVALID_HANDLE	INVALID_HANDLE_VALUE
#elif defined( DIR__POSIX )
	typedef DIR	*handle___;
#	define DIR_INVALID_HANDLE	NULL
#else
#	error
#endif

	// Si retourne chaîne vide, plus de fichier; si retourne NULL, erreur.
	inline const char *GetFirstFile(
		const char *Directory,
		handle___ &Handle )
	{
#ifdef DIR__MS
		static WIN32_FIND_DATAA File;
		HANDLE &hSearch = Handle;
		char SearchString[MAX_PATH+1] = "";

		if ( ( strlen( Directory ) + 4 ) > MAX_PATH )
			ERRl();

		strcpy( SearchString, Directory );

		if ( *SearchString )
			strcat( SearchString, "\\" );

		strcat( SearchString, "*.*" );

	    hSearch = FindFirstFileA( SearchString, &File );

		if ( hSearch == INVALID_HANDLE_VALUE )
			if ( GetLastError() == ERROR_NO_MORE_FILES )
				return "";
			else
				return NULL;
		else
			return File.cFileName;
#elif defined( DIR__POSIX )
	struct dirent * ent;
    DIR *&rep = Handle;
    
    rep = opendir( Directory );

	if( rep == NULL )
		return NULL;

	errno = 0;
    
    if ( ( ent = readdir(rep) ) == NULL )
		if ( errno != 0 )
			return NULL;
		else
			return "";
	else
		return ent->d_name;
    
    return 0;
#endif
	}

	// Si retourne chaîne vide, plus de fichier; si retourne NULL, erreur.
	inline const char *GetNextFile( handle___ &Handle )
	{
#ifdef DIR_DBG
		if ( Handle == DIR_INVALID_HANDLE )
			ERRu();
#endif
#ifdef DIR__MS
		static WIN32_FIND_DATAA File;
		HANDLE &hSearch = Handle;

		if ( !FindNextFileA( hSearch, &File ) )
			if ( GetLastError() == ERROR_NO_MORE_FILES )
				return "";
			else
				return NULL;

		return File.cFileName;
#endif
#ifdef DIR__POSIX
	struct dirent * ent;
    DIR *&rep = Handle;
    
	errno = 0;
    
    if ( ( ent = readdir(rep) ) == NULL )
		if ( errno != 0 )
			return NULL;
		else
			return "";
	else
		return ent->d_name;
    
    return 0;
#endif
	}

	inline void Close( handle___ &Handle )
	{
#ifdef DIR_DBG
		if ( Handle == DIR_INVALID_HANDLE )
			ERRu();
#endif
#ifdef DIR__MS
		if ( !FindClose( Handle ) )
			ERRs();

		Handle = INVALID_HANDLE_VALUE;
#endif
		
#ifdef DIR__POSIX
    DIR *&rep = Handle;
    
    if ( closedir(rep) )
		ERRs();

	Handle = NULL;
#endif
	}
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
