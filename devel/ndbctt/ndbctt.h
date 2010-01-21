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

	enum type__ {
		tStatic,
		tDynamic,
		t_amount,
		t_Undefined
	};

	class content__
	{
	private:
		ndbdct::dynamic_content_ *_Dynamic;
		ndbsct::static_content_ *_Static;
		type__ _Test( void ) const
		{
			if ( ( _Static == NULL ) == ( _Dynamic == NULL ) )
				ERRu();

			if ( _Static != NULL )
				return tStatic;
			else if ( _Dynamic != NULL )
				return tDynamic;
			else
				ERRc();

			return t_Undefined;	// Pour éviter un 'warning'.
		}
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
		content__ &operator =( const content__ &C )
		{
			// Pour éviter la copie des pointeurs.

			return *this;
		}
		void Init( void )
		{
			reset();
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
			switch ( _Test() ) {
			case tStatic:
				return _Static->Store( Data );
				break;
			case tDynamic:
				return _Dynamic->Store( Data );
				break;
			default:
				ERRc();
				break;
			}

			return NONE;	// Pour éviter un 'warning'.
		}
		void Erase( rrow__ Row )
		{
			switch ( _Test() ) {
			case tStatic:
				_Static->Erase( Row );
				break;
			case tDynamic:
				_Dynamic->Erase( Row );
				break;
			default:
				ERRc();
				break;
			}
		}
		void Store(
			const datum_ &Data,
			rrow__ Row )
		{
			switch ( _Test() ) {
			case tStatic:
				_Static->Store( Data, Row );
				break;
			case tDynamic:
				_Dynamic->Store( Data, Row );
				break;
			default:
				ERRc();
				break;
			}
		}
		// Retourne 'true' si l'enregistrement existe, faux sinon.
		bso::bool__ Retrieve(
			rrow__ Row,
			datum_ &Datum ) const
		{
			switch ( _Test() ) {
			case tStatic:
				 _Static->Retrieve( Row, Datum );
				 return true;
				break;
			case tDynamic:
				return _Dynamic->Retrieve( Row, Datum );
				break;
			default:
				ERRc();
				break;
			}

			return false;	// Pour éviter un 'warning'.
		}
		// Retourne 'true' si l'enregistrement existe, faux sinon.
		bso::bool__ Retrieve(
			rrow__ Row,
			datum_ &Datum,
			cache_ &Cache ) const
		{
			switch ( _Test() ) {
			case tStatic:
				_Static->Retrieve( Row, Datum, Cache );
				return true;
				break;
			case tDynamic:
				return _Dynamic->Retrieve( Row, Datum, Cache );
				break;
			default:
				ERRc();
				break;
			}

			return false;	// Pour éviter un 'warning'.
		}
		time_t ModificationTimeStamp( void ) const
		{
			switch ( _Test() ) {
			case tStatic:
				return _Static->ModificationTimeStamp();
				break;
			case tDynamic:
				return _Dynamic->ModificationTimeStamp();
				break;
			default:
				ERRc();
				break;
			}

			return 0;	// Pour éviter un 'warning'.
		}
		rrow__ First( void ) const
		{
			switch ( _Test() ) {
			case tStatic:
				return _Static->First();
				break;
			case tDynamic:
				return _Dynamic->First();
				break;
			default:
				ERRc();
				break;
			}

			return NONE;
		}
		rrow__ Last( void ) const
		{
			switch ( _Test() ) {
			case tStatic:
				return _Static->Last();
				break;
			case tDynamic:
				return _Dynamic->Last();
				break;
			default:
				ERRc();
				break;
			}

			return NONE;
		}
		rrow__ Next( rrow__ Row ) const
		{
			switch ( _Test() ) {
			case tStatic:
				return _Static->Next( Row );
				break;
			case tDynamic:
				return _Dynamic->Next( Row );
				break;
			default:
				ERRc();
				break;
			}

			return NONE;
		}
		rrow__ Previous( rrow__ Row ) const
		{
			switch ( _Test() ) {
			case tStatic:
				return _Static->Previous( Row );
				break;
			case tDynamic:
				return _Dynamic->Previous( Row );
				break;
			default:
				ERRc();
				break;
			}

			return NONE;
		}
		epeios::size__ Extent( void ) const
		{
			switch ( _Test() ) {
			case tStatic:
				return _Static->Extent();
				break;
			case tDynamic:
				return _Dynamic->Extent();
				break;
			default:
				ERRc();
				break;
			}

			return 0;	// Pour éviter un 'warning'.
		}
		epeios::size__ Amount( void ) const
		{
			switch ( _Test() ) {
			case tStatic:
				return _Static->Amount();
				break;
			case tDynamic:
				return _Dynamic->Amount();
				break;
			default:
				ERRc();
				break;
			}

			return 0;	// Pour éviter un 'warning'.
		}
		bso::bool__ Exists( rrow__ Row ) const
		{
			switch ( _Test() ) {
			case tStatic:
				return _Static->Exists( Row );
				break;
			case tDynamic:
				return _Dynamic->Exists( Row );
				break;
			default:
				ERRc();
				break;
			}

			return false;
		}
		bso::bool__ IsEmpty( void ) const
		{
			switch ( _Test() ) {
			case tStatic:
				return _Static->IsEmpty();
				break;
			case tDynamic:
				return _Dynamic->IsEmpty();
				break;
			default:
				ERRc();
				break;
			}

			return false;
		}
		type__ Type( void ) const
		{
			return _Test();
		}
	};

	class content_
	{
	private:
		content__ _Content;
		ndbdct::dynamic_content_ _Dynamic;
		ndbsct::static_content_ _Static;
	public:
		struct s {
			ndbdct::dynamic_content_::s Dynamic;
			ndbsct::static_content_::s Static;
		};
		content_( s &S )
		: _Dynamic( S.Dynamic ),
		  _Static( S.Static )
		{
		}
		void reset( bso::bool__ P = true )
		{
			_Content.reset( P );
			_Dynamic.reset( P );
			_Static.reset( P );
		}
		void plug( mmm::E_MULTIMEMORY_ &MM )
		{
			_Dynamic.plug( MM );
			_Static.plug( MM );
		}
		content_ &operator =( const content_ &C )
		{
			_Content = C._Content;

			_Dynamic = C._Dynamic;
			_Static = C._Static;

			return *this;
		}
		void Init( void )
		{
			reset();

			_Content.Init();
		}
		void InitDynamic( void )
		{
			reset();

			_Static.Init( 0 );
			_Dynamic.Init();
			_Content.Init( _Dynamic );
		}
		void InitStatic( epeios::size__ Size )
		{
			reset();

			_Dynamic.Init();
			_Static.Init( Size );
			_Content.Init( _Static );
		}
		const content__ &operator ()( void ) const
		{
			return _Content;
		}
		content__ &operator ()( void )
		{
			return _Content;
		}
		type__ Type( void ) const
		{
			return _Content.Type();
		}
		ndbsct::static_content_ &Static( void )
		{
			if ( Type() != tStatic )
				ERRu();

			return _Static;
		}
		const ndbsct::static_content_ &Static( void ) const
		{
			if ( Type() != tStatic )
				ERRu();

			return _Static;
		}
		const ndbdct::dynamic_content_ &Dynamic( void ) const
		{
			if ( Type() != tDynamic )
				ERRu();

			return _Dynamic;
		}
		ndbdct::dynamic_content_ &Dynamic( void )
		{
			if ( Type() != tDynamic )
				ERRu();

			return _Dynamic;
		}
	};

	E_AUTO( content )


}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
