/*
	Header for the 'daemon' library by Claude SIMON (csimon@epeios.org)
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

#ifndef DAEMON__INC
#define DAEMON__INC

#define DAEMON_NAME		"DAEMON"

#define	DAEMON_VERSION	"$Revision$"

#define DAEMON_OWNER		"Claude SIMON (csimon@epeios.org)"

#include "ttr.h"

extern class ttr_tutor &DAEMONTutor;

#if defined( XXX_DBG ) && !defined( DAEMON_NODBG )
#define DAEMON_DBG
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
//D  
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"

#include "mtx.h"
#include "lck.h"
#include "srv.h"

namespace deamon {	// 'daemon' (inversion du 'e' et du 'a') pose problème avec un ficheir d'entête de gcc.
	struct shared__	// Les ressources partagées.
	{
		bso::ulong__ Counter;	// Le compteur de client.
		bso::ulong__ Id;		// L'Identifiant de client.
		shared__( void )
		{
			Counter = Id = 0;
		}
	};

	struct mutexes___
	{
		mtx::mutex_handler__
			Even,		// Le verrou pour les valeurs paires.
			Odd;		// Le verrou pour les valeurs impaires;
		void Init( void )
		{
			Even = mtx::Create();
			Odd = mtx::Create();
		}
	};

	class user_function__
	: public srv::flow_functions__
	{
	private:
		shared__ Shared;
		mutexes___ Mutexes;
		lck::control___<shared__> Control_;	// Permet de controler l'accés aux ressources partagés.
		bso::bool__ _Start( txf::text_oflow__ &Flow );
		void _Other( txf::text_oflow__ &Flow );
	protected:
		virtual void SRVProcess( flw::ioflow__ &Flow );
	public:
		void Init( void )
		{
			Mutexes.Init();
			Control_.Init( Shared );
			mtx::Lock( Mutexes.Even );
		}
	};
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
