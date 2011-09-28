/*
	Header for the 'csdleo' library by Claude SIMON (csimon at zeusw dot org)
	Copyright (C) $COPYRIGHT_DATES$Claude SIMON.
$_RAW_$
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

#ifndef CSDLEO__INC
#define CSDLEO__INC

#define CSDLEO_NAME		"CSDLEO"

#define	CSDLEO_VERSION	"$Revision$"

#define CSDLEO_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &CSDLEOTutor;

#if defined( XXX_DBG ) && !defined( CSDLEO_NODBG )
#define CSDLEO_DBG
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
//D Client-Server Devices Library Embedded Overlapping 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# include "err.h"
# include "flw.h"
# include "lcl.h"
# include "rgstry.h"

#include "csdsuf.h"

# define CSDLEO_RETRIEVE_STEERING_FUNCTION_NAME		"CSDLEORetrieveSteering"
# define CSDLEO_RELEASE_STEERING_FUNCTION_NAME		"CSDLEOReleaseSteering"

# define CSDLEO_SHARED_DATA_VERSION	"alpha 2"

namespace csdleo {
	using namespace csdsuf;

	enum mode__ {
		mEmbedded,
		mRemote,
		m_amount,
		m_Undefined
	};

#pragma pack( push, 1)
	class shared_data__
	{
	public:
		static const char *Version;	// Toujours en première position.
		static bso::ulong__ Control;	// Une valeur relative au contenu de la structure, à des fins de test primaire de compatibilité.
		mode__ Mode;
		const char *User;				// Chaîne de caratère à la discrétion de l'utilisateur.
		fdr::oflow_driver___<> *COut, *CErr;
		void reset( bso::bool__ P = true )
		{
			Mode = m_Undefined;
		}
		shared_data__( void ) 
		{
			reset( false );
		}
		~shared_data__( void ) 
		{
			reset();
		}
		void Init( mode__ Mode )
		{
			this->Mode = Mode;
		}
		static size_t ControlComputing( void )
		{
			return sizeof( fdr::oflow_driver___<> );
		}
	};
#pragma pack( pop )

	typedef csdleo::user_functions__ *(retrieve_steering)( shared_data__ * );
	typedef void (release_steering)( csdleo::user_functions__ * );
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
