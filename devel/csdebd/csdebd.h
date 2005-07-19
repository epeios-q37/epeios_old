/*
	Header for the 'csdebd' library by Claude SIMON (csimon@epeios.org)
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

#ifndef CSDEBD__INC
#define CSDEBD__INC

#define CSDEBD_NAME		"CSDEBD"

#define	CSDEBD_VERSION	"$Revision$"

#define CSDEBD_OWNER		"Claude SIMON (csimon@epeios.org)"

#include "ttr.h"

extern class ttr_tutor &CSDEBDTutor;

#if defined( XXX_DBG ) && !defined( CSDEBD_NODBG )
#define CSDEBD_DBG
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
//D Client-Server EmBeD 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "csdscm.h"
#include "bch.h"

#define CSDEBD_CACHE_SIZE	1000


namespace csdebd {

	typedef bch::E_BUNCH_( flw::datum__ ) data_;
	E_AUTO( data )

	class _generic__
	: public flw::ioflow__
	{
	private:
		flw::datum__ _Cache[CSDEBD_CACHE_SIZE];
		data_ &_Read;
		data_ &_Write;
		epeios::row__ _Row;
	protected:
		virtual flw::bsize__ FLWRead(
			flw::bsize__ Minimum,
			flw::datum__ *Buffer,
			flw::bsize__ Wanted )
		{
			if ( _Row == NONE )
				_Row = _Read.First();

			if ( _Row == NONE )
				ERRu();

			if ( ( _Read.Amount() - *_Row ) < (epeios::size__)Wanted )
				Wanted = _Read.Amount() - *_Row;

#ifdef CSDEBD_DBG
			if ( Minimum > Wanted )
				ERRu();
#endif
			_Read.Recall( _Row, Wanted, Buffer );

			_Row = _Read.Next( _Row, Wanted );

			return Wanted;
		}
		virtual void FLWDismiss( void )
		{
#ifdef CSDEBD_DBG
			if ( _Row != NONE )
				ERRu();
#endif
			_Read.Init();
		}
		virtual flw::bsize__ FLWWrite(
			const flw::datum__ *Buffer,
			flw::bsize__ Wanted,
			flw::bsize__ Minimum,
			bool Synchronization )
		{
			_Write.Append( Buffer, Wanted );

			return Wanted;
		}
	public:
		_generic__(
			data_ &Read,
			data_ &Write )
		: ioflow__( _Cache, sizeof( _Cache ), FLW_SIZE_MAX, FLW_NO_MUTEX, FLW_NO_MUTEX ) ,
		  _Read( Read ),
		  _Write( Write )
		{
			_Row = NONE;
		}
	};

	class embed_client_server
	: public _generic__
	{
	private:
		_generic__ _Backend;
		csdscm::user_functions__ *_UserFunctions;
		void *_UP;
		data _Master, _Slave;
		void _Create( void )
		{
			_UP = _UserFunctions->PreProcess( _Backend );
		}
		void _Delete( void )
		{
			if( _UP != NULL )
				_UserFunctions->PostProcess( _UP );
		}
	protected:
		virtual void FLWSynchronizing( void )
		{
			_UserFunctions->Process( _Backend, _UP );
		}
	public:
		embed_client_server( void )
		: _generic__( _Master, _Slave ),
		  _Backend( _Slave ,_Master )
		{
			reset( false );
		}
		~embed_client_server( void )
		{
			reset();
		}
		void reset( bso::bool__ P = true )
		{
			if ( P ) {
				_Delete();
			}

			_UserFunctions = NULL;
			_UP = NULL;

			_Master.reset( P );
			_Slave.reset( P );
		}
		void Init( csdscm::user_functions__ &UserFunctions)
		{
			reset();

			_UserFunctions = &UserFunctions;

			_Master.Init();
			_Slave.Init();

			_Create();
		}
	};
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
