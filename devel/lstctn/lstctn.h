/*
	Header for the 'lstctn' library by Claude SIMON (csimon@epeios.org)
	Copyright (C) 2002  Claude SIMON (csimon@epeios.org).

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

#ifndef LSTCTN__INC
#define LSTCTN__INC

#define LSTCTN_NAME		"LSTCTN"

#define	LSTCTN_VERSION	"$Revision$"

#define LSTCTN_OWNER		"Claude SIMON (csimon@epeios.org)"

#include "ttr.h"

extern class ttr_tutor &LSTCTNTutor;

#if defined( XXX_DBG ) && !defined( LSTCTN_NODBG )
#define LSTCTN_DBG
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
//D LiST ConTaiNer 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "lst.h"
#include "ctn.h"

namespace lstctn {
	using lst::list_;

	template <typename container, typename row> class list_container_
	: public list_<row>,
	  public container
	{
	protected:
		virtual void LSTAllocate( epeios::size__ Size )
		{
			container::Allocate( Size );
		}
	public:
		struct s
		: public list_<row>::s,
		  public container::s
		{};
		list_container_( s &S )
		: list_<row>( S ), 
		  container( S )
		{}
		void reset( bso::bool__ P = true )
		{
			list_<row>::reset( P );
			container::reset( P );
		}
		void plug( mmm::E_MULTIMEMORY_ &MM )
		{
			list_<row>::plug( MM );
			container::plug( MM );
		}
		list_container_ &operator =( const list_container_ &LC )
		{
			list_<row>::operator =( LC );
			container::operator =( LC );

			return *this;
		}
		NAVt( list_<row>::, row )
		//f Initialization.
		void Init( void )
		{
			list_<row>::Init();
			container::Init();
		}
		//f Remove object at 'Row'.
		void Remove( row Row )
		{
			container::operator()( Row ).reset();
			list_<row>::Remove( Row );
		}
		//f Create new entry.
		row New( void )
		{
			return list_<row>::CreateEntry();
		}
	};

	AUTO2( list_container )

	template <typename container, typename object, typename row> class list_xcontainer_
	: public list_container_<container, row>
	{
	public:
		list_xcontainer_( s &S )
		: list_container_<container, row>( S )
		{}
		row Create( void )
		{
			row Row = list_container_<container, row>::New();

			list_container_<container, row>::operator()( Row ).Init();

			return Row;
		}
		row Add( const object &Object )
		{
			row Row = Create();

			list_container_<container, row>::operator( Row ).operator =( Object );

			return Row;
		}
	};

	AUTO3( list_xcontainer )

	#define E_LMCONTAINERt_( type, row )	list_container_< ctn::E_MCONTAINERt_( type, row ), row >
	#define E_LMCONTAINERt( type, row )		list_container< ctn::E_MCONTAINERt( type, row ), row >

	#define E_LMCONTAINER_( type )			E_LMCONTAINERt_( type, epeios::row__ )
	#define E_LMCONTAINER( type )			E_LMCONTAINERt( type, epeios::row__ )

	#define E_LCONTAINERt_( type, row )		list_container_< ctn::E_CONTAINERt_( type, row ), row >
	#define E_LCONTAINERt( type, row )		list_container< ctn::E_CONTAINERt( type, row ), row >

	#define E_LCONTAINER_( type )			E_LCONTAINERt_( type, epeios::row__ )
	#define E_LCONTAINER( type )			E_LCONTAINERt( type, epeios::row__ )

	#define E_LXMCONTAINERt_( type, row )	list_xcontainer_< ctn::E_XMCONTAINERt_( type, row ), type, row >
	#define E_LXMCONTAINERt( type, row )	list_xcontainer< ctn::E_XMCONTAINERt( type, row ), type, row >

	#define E_LXMCONTAINER_( type )			E_LXMCONTAINERt_( type, epeios::row__ )
	#define E_LXMCONTAINER( type )			E_LXMCONTAINERt( type, epeios::row__ )

	#define E_LXCONTAINERt_( type, row )	list_xcontainer_< ctn::E_XCONTAINERt_( type, row ), type, row >
	#define E_LXCONTAINERt( type, row )		list_xcontainer< ctn::E_XCONTAINERt( type, row ), type, row >

	#define E_LXCONTAINER_( type )			E_LXCONTAINERt_( type, epeios::row__ )
	#define E_LXCONTAINER( type )			E_LXCONTAINERt( type, epeios::row__ )

}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
