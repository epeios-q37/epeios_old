/*
	Header for the 'lstbch' library by Claude SIMON (csimon@epeios.org)
	Copyright (C) 2000-2002  Claude SIMON (csimon@epeios.org).

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

#ifndef LSTBCH__INC
#define LSTBCH__INC

#define LSTBCH_NAME		"LSTBCH"

#define	LSTBCH_VERSION	"$Revision$"

#define LSTBCH_OWNER		"Claude SIMON (csimon@epeios.org)"

#include "ttr.h"

extern class ttr_tutor &LSTBCHTutor;

#if defined( XXX_DBG ) && !defined( LSTBCH_NODBG )
#define LSTBCH_DBG
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
//D LiST BunCH 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "bch.h"
#include "lst.h"

namespace lstbch {

	using lst::list_;
	using bch::bunch_;

	//c Bunch associated to a list.
	template <typename type, typename row> class list_bunch_
	: public list_<row>,
	  public bunch_<type, row>
	{
	protected:
		virtual void LSTAllocate( epeios::size__ Size )
		{
			bunch_<type, row>::Allocate( Size );
		}
	public:
		struct s
		: public list_<row>::s,
		  public bunch_<type, row>::s
		{};
		list_bunch_( s &S )
		: list_<row>( S ),
		  bunch_<type, row>( S )
		{}
		void reset( bso::bool__ P = true )
		{
			list_<row>::reset( P );
			bunch_<type, row>::reset( P );
		}
		void plug( mmm::E_MULTIMEMORY_ &MM )
		{
		}
		list_bunch_ &operator =( const list_bunch_ &LB )
		{
			list_<row> operator =( LB );
			bunch_<type, row> operator =( LB );

			return *this;
		}
		//f Initialization.
		void Init( void )
		{
			list_<row>::Init();
			bunch_<type, row>::Init();
		}
		NAVt( list_<row>::, row )
		//f Add 'Object' and return its row.
		row Add( const type &Object )
		{
			row Row = list_<row>::CreateEntry();

			bunch_<type, row>::Write( Object, Row );

			return Row;
		}
		//f Remove entry 'Row'.
		void Remove( row Row )
		{
			list_<row>::Remove( Row );
		}
	};

	AUTO2( list_bunch )
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
