/*
	Header for the 'fnm' library by Claude SIMON (csimon@epeios.org)
	Copyright (C) 2000-2001, 2004 Claude SIMON (csimon@epeios.org).

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

#ifndef FNM__INC
#define FNM__INC

#define FNM_NAME		"FNM"

#define	FNM_VERSION	"$Revision$"

#define FNM_OWNER		"Claude SIMON (csimon@epeios.org)"

#include "ttr.h"

extern class ttr_tutor &FNMTutor;

#if defined( XXX_DBG ) && !defined( FNM_NODBG )
#define FNM_DBG
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
//D File Name Manager 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

//D FileName. Handle file names.

#include <string.h>
#include "cpe.h"

#if defined( CPE__T_LINUX ) || defined( CPE__T_BEOS ) || defined( CPE__T_CYGWIN ) || defined( CPE__T_MAC )
#	define FNM__POSIX
#elif defined( CPE__T_MS )
#	define FNM__MS
#else
#	error "Unknown target !"
#endif

namespace fnm
{
	//e Different type of file name.
	enum type {
		//i "" or NULL
		tEmpty = 1,
		//i "x:\...\nom.suf" or "\...\nom.suf" or "x:nom.suf" etc..
		tLocalised,
		//i "nom.suf"
		tSuffixed,
		//i "nom"
		tFree,
		//i "d:" or "d:\...\directory\" etc.
		tPath,
		//i unknow or not initialized.
		tUnknow
	};

	//f Type of the file name 'FileName'.
	fnm::type Type( const char *FileName );

	/*f Correct location, i. e. remplaces '\' or '/' with correct directory separator
	depending on OS. The retuned pointer must be freed.*/
	char *CorrectLocation( const char *Location );


	//f Description of the 'Type' type.
	const char *Description( fnm::type Type );

	/*f Make file name with 'Name', 'Directory' as default
	directory, and 'Extension' as defaut extension; 
	IMPORTANT: the returned pointer MUST be freed with 'free()'.*/
	char *MakeFileName(
		const char *Rep,
		const char *Nom,
		const char *Ext );

	/*f Return the name of the file named 'Name', without its localization. */
	inline const char *GetFileName( const char *Name )
	{
		const char *Repere;

		if ( ( ( Repere = strrchr( Name, '/' ) ) == NULL )
			&& ( ( Repere = strrchr( Name, '\\' ) ) == NULL ) )
			if ( ( Repere = strrchr( Name, ':' ) ) == NULL )
				Repere = Name;
			else
				Repere++;
		else
			Repere++;

		return Repere;
	}

	// Return a string contaiinig the location only. Returned pointermust be freed.
	char *GetLocation( const char *Name );

#ifndef CPE__T_MT
	//f Return the file name of 'Name' without localization and extension.
	const char *GetFileNameRoot( const char *Nom );
#endif

	/************************************************/
	/* GESTION DE LA GENERATION D'UN NOM DE FICHIER */
	/************************************************/


	//c Manage 8 character long file name.
	class file_name_manager
	//: public utl_PU
	{
	private:
#ifndef CPE__T_MT
		/* Return a 8 characters long file name using the 'Base' string (any length)
		with 'Occurence' (>=0 <=36) the present occurence of a 'Base' based file name */
		const char *MakeFileName_(
			const char *base,
			int Occurence = 0 );
#endif
	protected:
		//v Must return 'true' if the file named 'Name' is the searched file..
		virtual bool FNMMatch(	const char *Name ) = 0;
	public:
		//f Initialization.
		void Init( void )
		{
			// for standadization reason.
		}
		/*f Return a 8 characters file name based on the 'Base' string (any lentgh)
		in 'Directory' with 'Extension' as extension. Use 'TOLFileExists' to define
		if this file already exists. If 'NULL' is returned, then no file can be
		generated, because all occurence are already used.
		IMPORTANT: the returned pointer, if != 'NULL', MUST be freed with 'free'. */
		char *SearchFileName(
			const char *Directory,
			const char *Base,
			const char *Extension );
	};
}

#ifdef FNM__MS
#	define FNM_DIRECTORY_SEPARATOR_STRING	"\\"
#	define FNM_DIRECTORY_SEPARATOR_CHARACTER	'\\'
#elif defined( FNM__POSIX )
#	define FNM_DIRECTORY_SEPARATOR_STRING	"/"
#	define FNM_DIRECTORY_SEPARATOR_CHARACTER	'/'
#else
#	error
#endif



/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
