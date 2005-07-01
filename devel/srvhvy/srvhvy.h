/*
	Header for the 'srvhvy' library by Claude SIMON (csimon@epeios.org)
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

#ifndef SRVHVY__INC
#define SRVHVY__INC

#define SRVHVY_NAME		"SRVHVY"

#define	SRVHVY_VERSION	"$Revision$"

#define SRVHVY_OWNER		"Claude SIMON (csimon@epeios.org)"

#include "ttr.h"

extern class ttr_tutor &SRVHVYTutor;

#if defined( XXX_DBG ) && !defined( SRVHVY_NODBG )
#define SRVHVY_DBG
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
//D SeRCer fo HeaVY operations. 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "srv.h"
#include "lstbch.h"

namespace srvhvy {

	using srv::action__;
	using srv::aContinue;
	using srv::aStop;
	using srv::service__;

	typedef bso::ushort__ id__;

#define SRVHVY_UNDEFINED	BSO_USHORT_MAX

	typedef void *_user_pointer__;

	typedef lstbch::E_LBUNCH_( _user_pointer__ ) user_pointers_;

	class core_
	{
	public:
		struct s
		{
			user_pointers_::s UPs;
		};
		user_pointers_ UPs;
		core_ ( s &S )
		: UPs( S.UPs )
		{}
		void reset( bso::bool__ P = true )
		{
			UPs.reset( P );
		}
		void plug( mmm::E_MULTIMEMORY_ &MM )
		{
			UPs.plug( MM );
		}
		core_ &operator =( const core_ &C )
		{
			ERRu();

			return *this;
		}
		void Init( void )
		{
			UPs.Init();
		}
		id__ New( void )
		{
			epeios::row__ Row = UPs.New();

			if ( *Row >= BSO_USHORT_MAX )
				ERRl();

			return (id__)*Row;
		}
		void Store(
			_user_pointer__ UP,
			id__ Id )
		{
#ifdef SRVHVY_DBG
			if ( Id == SRVHVY_UNDEFINED )
				ERRu();
#endif
			UPs.Store( UP, Id );
		}
		bso::bool__ Exists( id__ Id ) const
		{
			return UPs.Exists( Id );
		}
		_user_pointer__ Get( id__ Id ) const
		{
#ifdef SRVHVY_DBG
			if ( !Exists( Id ) )
				ERRu();
#endif
			return UPs( Id );
		}
	};

	E_AUTO( core)


	class user_functions__
	{
	protected:
		virtual void *SRVHVYPreProcess( flw::ioflow__ &Flow ) = 0;
		virtual action__ SRVHVYProcess(
			flw::ioflow__ &Flow,
			void *UP ) = 0;
		virtual void SRVHVYPostProcess( void *UP ) = 0;
	public:
		void *PreProcess( flw::ioflow__ &Flow )
		{
			return SRVHVYPreProcess( Flow );
		}
		action__ Process(
			flw::ioflow__ &Flow,
			void *UP )
		{
			return SRVHVYProcess( Flow, UP );
		}
		void PostProcess( void *UP )
		{
			SRVHVYPostProcess( UP );
		}
	};

	class server___
	{
	private:
		srv::server___ _Server;
	public:
		void Init( void )
		{}
		void Process(
			service__ Service,
			user_functions__ &Functiosn,
			core_ &Core );
	};


}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
