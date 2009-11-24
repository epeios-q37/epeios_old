/*
	Header for the 'ndbctt' library by Claude SIMON (csimon at zeusw dot org)
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

#ifndef NDBCTT__INC
#define NDBCTT__INC

#define NDBCTT_NAME		"NDBCTT"

#define	NDBCTT_VERSION	"$Revision$"

#define NDBCTT_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &NDBCTTTutor;

#if defined( XXX_DBG ) && !defined( NDBCTT_NODBG )
#define NDBCTT_DBG
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
//D New DataBase ConTenT (will be moved on its own project in the future) 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "ndbbsc.h"
#include "ndbdct.h"
#include "ndbsct.h"

namespace ndbctt {

	using ndbbsc::rrow__;
	using ndbbsc::datum_;
	using ndbbsc::cache_;

	class content__
	{
	private:
		ndbdct::dynamic_content_ *_Dynamic;
		ndbsct::static_content_ *_Static;
	public:
		void reset( bso::bool__ = true )
		{
			_Dynamic = NULL;
			_Static = NULL;
		}
		content__( void )
		{
			reset( false );
		}
		void Init( ndbdct::dynamic_content_ &Dynamic )
		{
			if ( ( _Dynamic != NULL ) || ( _Static != NULL ) )
				ERRu();

			_Dynamic = &Dynamic;
		}
		void Init( ndbsct::static_content_ &Static )
		{
			if ( ( _Dynamic != NULL ) || ( _Static != NULL ) )
				ERRu();

			_Static = &Static;
		}
		rrow__ Store( const datum_ &Data )
		{
			if ( _Static != NULL )
				return _Static->Store( Data );
			else if ( _Dynamic != NULL )
				return _Dynamic->Store( Data );
			else
				ERRu();

			return NONE;	// Pour éviter un 'warning'.
		}
		void Erase( rrow__ Row )
		{
			if ( _Static != NULL )
				_Static->Erase( Row );
			else if ( _Dynamic != NULL )
				_Dynamic->Erase( Row );
			else
				ERRu();
		}
		void Store(
			const datum_ &Data,
			rrow__ Row )
		{
			if ( _Static != NULL )
				_Static->Store( Data, Row );
			else if ( _Dynamic != NULL )
				_Dynamic->Store( Data, Row );
			else
				ERRu();
		}
		// Retourne 'true' si l'enregistrement existe, faux sinon.
		bso::bool__ Retrieve(
			rrow__ Row,
			datum_ &Datum ) const
		{
			if ( _Static != NULL ) {
				 _Static->Retrieve( Row, Datum );
				 return true;
			} else if ( _Dynamic != NULL )
				return _Dynamic->Retrieve( Row, Datum );
			else
				ERRu();

			return false;	// Pour éviter un 'warning'.
		}
		// Retourne 'true' si l'enregistrement existe, faux sinon.
		bso::bool__ Retrieve(
			rrow__ Row,
			datum_ &Datum,
			cache_ &Cache ) const
		{
			if ( _Static != NULL ) {
				_Static->Retrieve( Row, Datum, Cache );
				return true;
			} else if ( _Dynamic != NULL )
				return _Dynamic->Retrieve( Row, Datum, Cache );
			else
				ERRu();

			return false;	// Pour éviter un 'warning'.
		}
		time_t ModificationTimeStamp( void ) const
		{
			if ( _Static != NULL )
				return _Static->ModificationTimeStamp();
			else if ( _Dynamic != NULL )
				return _Dynamic->ModificationTimeStamp();
			else
				ERRu();

			return 0;	// Pour éviter un 'warning'.
		}
		rrow__ First( void ) const
		{
			if ( _Static != NULL )
				return _Static->First();
			else if ( _Dynamic != NULL )
				return _Dynamic->First();
			else
				ERRu();

			return NONE;
		}
		rrow__ Last( void ) const
		{
			if ( _Static != NULL )
				return _Static->Last();
			else if ( _Dynamic != NULL )
				return _Dynamic->Last();
			else
				ERRu();

			return NONE;
		}
		rrow__ Next( rrow__ Row ) const
		{
			if ( _Static != NULL )
				return _Static->Next( Row );
			else if ( _Dynamic != NULL )
				return _Dynamic->Next( Row );
			else
				ERRu();

			return NONE;
		}
		rrow__ Previous( rrow__ Row ) const
		{
			if ( _Static != NULL )
				return _Static->Previous( Row );
			else if ( _Dynamic != NULL )
				return _Dynamic->Previous( Row );
			else
				ERRu();

			return NONE;
		}
		epeios::size__ Extent( void ) const
		{
			if ( _Static != NULL )
				return _Static->Extent();
			else if ( _Dynamic != NULL )
				return _Dynamic->Extent();
			else
				ERRu();

			return 0;	// Pour éviter un 'warning'.
		}
		epeios::size__ Amount( void ) const
		{
			if ( _Static != NULL )
				return _Static->Amount();
			else if ( _Dynamic != NULL )
				return _Dynamic->Amount();
			else
				ERRu();

			return 0;	// Pour éviter un 'warning'.
		}
		bso::bool__ Exists( rrow__ Row ) const
		{
			if ( _Static != NULL )
				return _Static->Exists( Row );
			else if ( _Dynamic != NULL )
				return _Dynamic->Exists( Row );
			else
				ERRu();

			return false;
		}
		bso::bool__ IsEmpty( void ) const
		{
			if ( _Static != NULL )
				return _Static->IsEmpty();
			else if ( _Dynamic != NULL )
				return _Dynamic->IsEmpty();
			else
				ERRu();

			return false;
		}
	};

	class content_
	{
	private:
		content__ _Content;
	public:
		struct s {
			ndbdct::dynamic_content_::s Dynamic;
			ndbsct::static_content_::s Static;
		};
		ndbdct::dynamic_content_ Dynamic;
		ndbsct::static_content_ Static;
		content_( s &S )
		: Dynamic( S.Dynamic ),
		  Static( S.Static )
		{
		}
		void reset( bso::bool__ P = true )
		{
			_Content.reset( P );
			Dynamic.reset( P );
			Static.reset( P );
		}
		void plug( mmm::E_MULTIMEMORY_ &MM )
		{
			Dynamic.plug( MM );
			Static.plug( MM );
		}
		content_ &operator =( const content_ &C )
		{
			_Content = C._Content;

			Dynamic = C.Dynamic;
			Static = C.Static;

			return *this;
		}
		void InitDynamic( void )
		{
			reset();

			Dynamic.Init();
			_Content.Init( Dynamic );
		}
		void InitStatic( epeios::size__ Size )
		{
			reset();

			Static.Init( Size );
			_Content.Init( Static );
		}
		const content__ *operator ->( void ) const
		{
			return &_Content;
		}
		content__ *operator ->( void )
		{
			return &_Content;
		}
		const content__ *operator *( void ) const
		{
			return &_Content;
		}
		content__ *operator *( void )
		{
			return &_Content;
		}
		E_RWDISCLOSE__( content__, Content )
	};

	E_AUTO( content )


}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
