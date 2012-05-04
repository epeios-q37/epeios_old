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

# define GECKOO_OVERLAPPING_VERSION	"4"

# define GECKOO_CREATE_STEERING_FUNCTION_NAME			GECKOOCreateSteering
# define GECKOO_RETRIEVE_STEERING_FUNCTION_NAME			GECKOORetrieveSteering
# define GECKOO_DELETE_STEERING_FUNCTION_NAME			GECKOODeleteSteering

namespace geckoo {
	typedef mdr::row__ id__;

	class pseudo_event_callback__
	{
	protected:
		virtual void GECKOOHandle( nsIDOMElement *Element ) = 0;
	public:
		void reset( bso::bool__ = true )
		{
			// Standardisation.
		}
		E_CVDTOR( pseudo_event_callback__);
		void Init( void )
		{
			reset();
		}
		void Handle( nsIDOMElement *Element )
		{
			GECKOOHandle( Element );
		}
	};

	void AddPseudoEventHandler(
		nsIDOMNode *Node,
		const char *PseudoEventName,
		pseudo_event_callback__ &Handler );


	class steering_callback__ {
	protected:
		virtual bso::bool__ GECKOORegister(
			nsIDOMWindow *Window,
			const str::string_ &Id ) = 0;
	public:
		void reset( bso::bool__ = true )
		{
			// Standardisation.
		}
		E_CDTOR( steering_callback__ )
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
		static const char *Version;	// Toujours en premi�re position.
		static bso::ulong__ Control;	// Une valeur relative au contenu de la structure, � des fins de test primaire de compatibilit�.
		const char *LauncherIdentification;
		const char *Language;
		const char *Path;	// Chemin d la biblioth�que.
		void *UP;				// A la discr�tion de l'utilisateur.
		fdr::oflow_driver___<> *COut, *CErr;
		void reset( bso::bool__ P = true )
		{
			LauncherIdentification = NULL;
			Path = NULL;
			UP = NULL;
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
			const char *LauncherIdentification,
			const char *Language,
			const char *Path,
			void *UP = NULL )
		{
			this->LauncherIdentification = LauncherIdentification,
			this->Language = Language;
			this->Path = Path,
			this->UP = UP;

			COut = &cio::COutDriver;
			CErr = &cio::CErrDriver;
		}
		static size_t ControlComputing( void )
		{
			return sizeof( fdr::oflow_driver___<> );
		}
	};
#pragma pack( pop )

	typedef steering_callback__ *(create_steering)( shared_data__ * );
	typedef steering_callback__ *(retrieve_steering)( void );
	typedef void (delete_steering)( steering_callback__ * );

}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
