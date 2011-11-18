/*
	Header for the 'geckoo' library by Claude SIMON (csimon at zeusw dot org)
	Copyright (C) 20112004 Claude SIMON.

	This file is part of the Epeios (http://zeusw.org/epeios/) project.

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

#ifndef GECKOO__INC
#define GECKOO__INC

#define GECKOO_NAME		"GECKOO"

#define	GECKOO_VERSION	"$Revision$"

#define GECKOO_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &GECKOOTutor;

#if defined( XXX_DBG ) && !defined( GECKOO_NODBG )
#define GECKOO_DBG
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
//D GECKO Overlapping 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# include "err.h"
# include "flw.h"
# include "cio.h"

# include "nsxpcm.h"

# define GECKOO_RETRIEVE_STEERING_FUNCTION_NAME			GECKOORetrieveSteering
# define GECKOO_RETRIEVE_LONE_STEERING_FUNCTION_NAME	GECKOORetrieveLoneSteering
# define GECKOO_RELEASE_STEERING_FUNCTION_NAME			GECKOOReleaseSteering

# define GECKOO_SHARED_DATA_VERSION	"alpha 1"

namespace geckoo {
	typedef epeios::row__ id__;

	class user_functions__ {
	protected:
		virtual bso::bool__ GECKOORegister(
			nsIDOMWindow *Window,
			const str::string_ &Id ) = 0;
	public:
		void reset( bso::bool__ = true )
		{
			// Standardisation.
		}
		user_functions__( void )
		{
			reset( false );
		}
		~user_functions__( void )
		{
			reset();
		}
		void Init( void )
		{
			// Standadisation.
		}
		bso::bool__ Register(
			nsIDOMWindow *Window,
			const str::string_ &Id )
		{
			return GECKOORegister( Window, Id );
		}
	};

#pragma pack( push, 1)
	class shared_data__
	{
	public:
		static const char *Version;	// Toujours en première position.
		static bso::ulong__ Control;	// Une valeur relative au contenu de la structure, à des fins de test primaire de compatibilité.
		const char *Language;
		const char *User;				// Chaîne de caratère à la discrétion de l'utilisateur.
		fdr::oflow_driver___<> *COut, *CErr;
		void reset( bso::bool__ P = true )
		{
			User = NULL;
		}
		shared_data__( void ) 
		{
			reset( false );
		}
		~shared_data__( void ) 
		{
			reset();
		}
		void Init(
			const char *Language,
			const char *User = NULL )
		{
			this->Language = Language,
			this->User = User;

			COut = &cio::COutDriver;
			CErr = &cio::CErrDriver;
		}
		static size_t ControlComputing( void )
		{
			return sizeof( fdr::oflow_driver___<> );
		}
	};
#pragma pack( pop )

	typedef user_functions__ *(retrieve_steering)( shared_data__ * );
	typedef user_functions__ *(retrieve_lone_steering)( void );
	typedef void (release_steering)( user_functions__ * );

}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
