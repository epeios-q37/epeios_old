/*
	Header for the 'dbsidx' library by Claude SIMON (csimon@epeios.org)
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

#ifndef DBSIDX__INC
#define DBSIDX__INC

#define DBSIDX_NAME		"DBSIDX"

#define	DBSIDX_VERSION	"$Revision$"

#define DBSIDX_OWNER		"Claude SIMON (csimon@epeios.org)"

#include "ttr.h"

extern class ttr_tutor &DBSIDXTutor;

#if defined( XXX_DBG ) && !defined( DBSIDX_NODBG )
#define DBSIDX_DBG
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
//D DataBaSe Index 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "idxbtq.h"
#include "dbsdct.h"

namespace dbsidx {
	using dbsdct::data_;
	using dbsdct::data;

	using dbsdct::content_;
	using dbsdct::content;

	using dbsdct::row__;

	class sort_function__
	{
	protected:
		virtual bso::sign__ DBSIDXCompare(
			const data_ &Data1,
			const data_ &Data2 ) = 0;
	public:
		bso::sign__ Compare(
			const data_ &Data1,
			const data_ &Data2 )
		{
			return DBSIDXCompare( Data1, Data2 );
		}
	};

	typedef idxbtq::E_INDEXt_( row__ ) _index_;
	E_AUTO( _index )

	class index_
	{
	private:
		bso::sign__ _Search(
			const data_ &Data,
			row__ &Row ) const;
		const content_ &_Content( void ) const
		{
			return *_S.Content;
		}
		void _Retrieve(
			row__ Row,
			data_ &Data ) const
		{
			_Content().Retrieve( Row, Data );
		}
	public:
		_index_ BaseIndex;
		struct s {
			_index_::s BaseIndex;
			row__ Root;
			sort_function__ *Sort;
			const content_ *Content;
		} &_S;
		index_( s &S )
		: _S( S ),
		  BaseIndex( S.BaseIndex )
		{}
		void reset( bso::bool__ P = true )
		{
			BaseIndex.reset( P );
			_S.Root = NONE;

			_S.Sort = NULL;
			_S.Content = NULL;

		}
		void plug( mmm::E_MULTIMEMORY_ &MM )
		{
			BaseIndex.plug( MM );
		}
		index_ &operator =( const index_ &I )
		{
			BaseIndex = I.BaseIndex;
			_S.Root = I._S.Root;

			_S.Sort = I._S.Sort;
			_S.Content = I._S.Content;

			return *this;
		}
		void Init(
			const content_ &Content = *(content_ *)NULL,
			sort_function__ &Sort = *(sort_function__ *)NULL)
		{
			BaseIndex.Init();
			_S.Root = NONE;

			_S.Content = &Content;
			_S.Sort = &Sort;
		}
		// Vide l'index.
		void Reset( void )
		{
			_S.Root = NONE;
			BaseIndex.Init();
		}
		void Allocate( mdr::size__ Size )
		{
			BaseIndex.Allocate( Size );
		}
		void Index( row__ Row );
		void Delete( row__ Row )
		{
#ifdef DBSIDX_DBG
			if ( _S.Root == NONE )
				ERRu();
#endif
			_S.Root = BaseIndex.Delete( Row, _S.Root );
		}
		row__ Search(
			const data_ &Data,
			bso::sign__ &Sign ) const;
		row__ Search( const data &Data ) const
		{
			bso::sign__ Sign;

			return Search( Data, Sign );
		}
		row__ SearchRoot( void )
		{
			ERRl();

			return _S.Root;
		}
		row__ First( void ) const
		{
			if ( _S.Root != NONE )
				return BaseIndex.First( _S.Root );
			else
				return NONE;
		}
		row__ Last( void ) const
		{
			if ( _S.Root != NONE )
				return BaseIndex.Last( _S.Root );
			else
				return NONE;
		}
		row__ Next( row__ Row ) const
		{
			return BaseIndex.Next( Row );
		}
		row__ Previous( row__ Row ) const
		{
			return BaseIndex.Previous( Row );
		}
		mdr::size__ Amount( void ) const
		{
			return BaseIndex.Amount();
		}
	};

	E_AUTO( index )

	// 'index' stockés dans des fichiers.

	class file_index_
	: public index_
	{
	private:
		void _SaveRoot( void ) const;
	public:
		str::string_ RootFileName;
		struct s
		: public index_::s
		{
			struct memory_driver__ {
				struct tree__ {
					flm::E_FILE_MEMORY_DRIVER___
						Nodes,
						Colors;
				} Tree;
				flm::E_FILE_MEMORY_DRIVER___ Queue;
			} MemoryDriver;
			str::string_::s RootFileName;
		} &_S;
		file_index_( s &S )
		: _S( S ), 
		  index_( S ),
		  RootFileName( S.RootFileName )
		{}
		void reset( bso::bool__ P = true )
		{
			if ( P ) {
				if ( RootFileName.Amount() != 0 )
					_SaveRoot();
			}

			_S.MemoryDriver.Tree.Nodes.reset( P );
			_S.MemoryDriver.Tree.Colors.reset( P );
			_S.MemoryDriver.Queue.reset( P );
			RootFileName.reset( P );
			index_::reset( P );
		}
		void plug( mmm::E_MULTIMEMORY_ & )
		{
			ERRu();	// Cette méthode n'a pas de sens dans ce contexte.
		}
		file_index_ &operator =( const index_ &FI )
		{
			index_::operator =( FI );

			return *this;	// Pour éviter un warning
		}
		bso::bool__ Init(
			const str::string_ &RootFileName,
			const content_ &Content = *(content_ *)NULL,
			sort_function__ &Sort = *(sort_function__ *)NULL );
	};

	E_AUTO( file_index )




}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
