/*
	Header for the 'bfl' library by Claude SIMON (csimon@epeios.org)
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

#ifndef BFL__INC
#define BFL__INC

#define BFL_NAME		"BFL"

#define	BFL_VERSION	"$Revision$"

#define BFL_OWNER		"Claude SIMON (csimon@epeios.org)"

#include "ttr.h"

extern class ttr_tutor &BFLTutor;

#if defined( XXX_DBG ) && !defined( BFL_NODBG )
#define BFL_DBG
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
//D Bit FLow 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"

namespace bfl {
	using flw::bsize__;
	using flw::datum__;

	//c Input text flow.
	class bit_iflow__
	{
	private:
		// 'Flow' duquel sont lues les données.
		flw::iflow__ &_Flow;
		datum__ *_Datum;
		bso::ubyte__ _Counter;
		bso::bool__ _Get( void )
		{
			if ( _Counter == 0 ) {
				_Datum = _Flow.GetCurrentCacheDatum( true );
				_Counter = 8 * sizeof( *_Datum );
			} else
				*_Datum >>= 1;

			_Counter--;

			return *_Datum & 1;
		}
	public:
		bit_iflow__( flw::iflow__ &Flow )
		: _Flow( Flow )
		{
			_Datum = NULL;
			_Counter = 0;
		}
		bso::bool__ Get( void )
		{
			return _Get();
		}
	};


	//c Output text flow.
	class bit_oflow__
	{
	private:
		// 'Flow' dans lequel sont écrites les données.
		flw::oflow__ &_Flow;
		datum__ *_Datum;
		bso::ubyte__ _Counter;
		void _Put( bso::bool__ Value )
		{
			if ( _Counter == 0 ) {
				_Datum = _Flow.GetCurrentCacheDatum( true );
				_Counter = 8 * sizeof( *_Datum );
			}

			*_Datum  = ( *_Datum >> 1 ) | ( Value ? 0x80 : 0 );

			_Counter--;
		}
	public:
		bit_oflow__( flw::oflow__ &Flow )
		: _Flow( Flow )
		{
			_Datum = NULL;
			_Counter = 0;
		}
		void Put( bso::bool__ Value )
		{
			_Put( Value );
		}
	};
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
