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
#include "dbsctt.h"

namespace dbsidx {
	using dbsctt::datum_;
	using dbsctt::datum;

	using dbsctt::content_;
	using dbsctt::content;

	using dbsctt::rrow__;

	class sort_function__
	{
	protected:
		virtual bso::sign__ DBSIDXCompare(
			const datum_ &Datum1,
			const datum_ &Datum2 ) = 0;
		virtual bso::bool__ DBSIDXBegins(
			const datum_ &Datum,
			const datum_ &Pattern ) = 0;
		//NOTA : l'opérateur 'Contains' n'a pas de sens pour un index, car il ne peut y avoir un ordre de tri.
	public:
		bso::sign__ Compare(
			const datum_ &Data1,
			const datum_ &Data2 )
		{
			return DBSIDXCompare( Data1, Data2 );
		}
		bso::bool__ Begins(
			const datum_ &Datum,
			const datum_ &Pattern )
		{
			return DBSIDXBegins( Datum, Pattern );
		}
	};

	typedef idxbtq::E_INDEXt_( rrow__ ) _index_;
	E_AUTO( _index )

	struct extremities__
	{
		rrow__ Smallest, Greatest;
		bso::ulong__ Used;
		extremities__( void )
		{
			Smallest = Greatest = NONE;
			Used = 0;
		}
	};

	// Pour idiquer le comportement d'un "seek' lorsqu'il tombe sur un élément qu est égal à celui recherché.
	enum behavior__ {
		// On stoppe la recherche.
		bStop,
		// On cherche le plus grand
		bGreater,
		// On cherche le plus petit
		bLesser,
		// On stoppe s'il manque l'un des fils. (sert lors d'une indexation).
		bStopIfOneChildMissing,
		b_amount,
		b_Undefined
	};


	class index_
	{
	private:
		bso::sign__ _Seek(
			const datum_ &Data,
			behavior__ StopIfEqual,
			rrow__ &Row,
			bso::ubyte__ &Round,
			dbsctt::_cache_ &Cache ) const;
		const content_ &_Content( void ) const
		{
			return *S_.Content;
		}
		void _Retrieve(
			rrow__ Row,
			datum_ &Datum,
			dbsctt::_cache_ &Cache ) const
		{
			_Content().Retrieve( Row, Datum, Cache );
		}
	public:
		_index_ BaseIndex;
		struct s {
			_index_::s BaseIndex;
			rrow__ Root;
			sort_function__ *Sort;
			const content_ *Content;
			time_t ModificationTimeStamp;
		} &S_;
		index_( s &S )
		: S_( S ),
		  BaseIndex( S.BaseIndex )
		{}
		void reset( bso::bool__ P = true )
		{
			BaseIndex.reset( P );
			S_.Root = NONE;

			S_.Sort = NULL;
			S_.Content = NULL;

			S_.ModificationTimeStamp = 0;

		}
		void plug( mmm::E_MULTIMEMORY_ &MM )
		{
			BaseIndex.plug( MM );
		}
		index_ &operator =( const index_ &I )
		{
			BaseIndex = I.BaseIndex;
			S_.Root = I.S_.Root;

			S_.Sort = I.S_.Sort;
			S_.Content = I.S_.Content;

			S_.ModificationTimeStamp = I.S_.ModificationTimeStamp;

			return *this;
		}
		void Init(
			const content_ &Content = *(content_ *)NULL,
			sort_function__ &Sort = *(sort_function__ *)NULL)
		{
			BaseIndex.Init();
			S_.Root = NONE;

			S_.Content = &Content;
			S_.Sort = &Sort;

			S_.ModificationTimeStamp = 0;

		}
		// Vide l'index.
		void Reset( void )
		{
			S_.Root = NONE;
			BaseIndex.Init();

			S_.ModificationTimeStamp = 0;
		}
		void Allocate( mdr::size__ Size )
		{
			BaseIndex.Allocate( Size );
		}
		bso::ubyte__ Index(
			rrow__ Row,
			extremities__ *Extremities,	// Si à 'true', compare d'abord avec les extrémités. Utilisé pour la réindexation.
			dbsctt::_cache_ &Cache  = *(dbsctt:: _cache_ *)NULL );	// Retourne le nombre de noeuds parcourus pour cette indexation.
		void Delete( rrow__ Row )
		{
#ifdef DBSIDX_DBG
			if ( S_.Root == NONE )
				ERRu();
#endif
			S_.Root = BaseIndex.Delete( Row, S_.Root );

			S_.ModificationTimeStamp = tol::Clock();
		}
		rrow__ Seek(
			const datum_ &Datum,
			behavior__ EqualBehavior,
			bso::sign__ &Sign ) const;
		rrow__ Seek(
			behavior__ EqualBehavior,
			const datum_ &Datum ) const
		{
			bso::sign__ Sign;

			return Seek( Datum, EqualBehavior, Sign );
		}
		bso::sign__ Compare(
			rrow__ RecordId,
			const datum_ &Pattern ) const;
		bso::bool__ Begins(
			rrow__ RecordRow,
			const datum_ &Datum ) const;
		rrow__ SearchRoot( void )
		{
			ERRl();

			return S_.Root;
		}
		rrow__ First( void ) const
		{
			if ( S_.Root != NONE )
				return BaseIndex.First( S_.Root );
			else
				return NONE;
		}
		rrow__ Last( void ) const
		{
			if ( S_.Root != NONE )
				return BaseIndex.Last( S_.Root );
			else
				return NONE;
		}
		rrow__ Next( rrow__ Row ) const
		{
			return BaseIndex.Next( Row );
		}
		rrow__ Previous( rrow__ Row ) const
		{
			return BaseIndex.Previous( Row );
		}
		mdr::size__ Amount( void ) const
		{
			return BaseIndex.Amount();
		}
		bso::bool__ IsSynchronized( void ) const
		{
			return S_.ModificationTimeStamp > _Content().ModificationTimeStamp();
		}
		sort_function__ &SortFunction( void ) const
		{
			return *S_.Sort;
		}
		const content_ &Content( void ) const
		{
			return *S_.Content;
		}
		void Balance( void )
		{
			if ( S_.Root != NONE )
				S_.Root = BaseIndex.Balance( S_.Root );

			S_.ModificationTimeStamp = tol::Clock();
		}
		E_RODISCLOSE_( time_t, ModificationTimeStamp );
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
				flm::E_FILE_MEMORY_DRIVER___ Tree;
				flm::E_FILE_MEMORY_DRIVER___ Queue;
			} MemoryDriver;
			str::string_::s RootFileName;
		} &S_;
		file_index_( s &S )
		: S_( S ), 
		  index_( S ),
		  RootFileName( S.RootFileName )
		{}
		void reset( bso::bool__ P = true )
		{
			if ( P ) {
				if ( ( RootFileName.Amount() != 0 ) && ModificationTimeStamp() != 0 )
					_SaveRoot();
			}

			S_.MemoryDriver.Tree.reset( P );
			S_.MemoryDriver.Queue.reset( P );
			RootFileName.reset( P );
			index_::reset( P );
		}
		void plug( mmm::E_MULTIMEMORY_ & )
		{
			ERRu();	// Cette méthode n'a pas de sens dans ce contexte.
		}
		file_index_ &operator =( const file_index_ &FI )
		{
			index_::operator =( FI );

			return *this;
		}
		bso::bool__ Init(
			const str::string_ &RootFileName,
			const content_ &Content,
			sort_function__ &Sort,
			mdr::mode Mode,
			bso::bool__ Erase );
		void CloseFiles( void )
		{
			S_.MemoryDriver.Tree.Liberer();
			S_.MemoryDriver.Queue.Liberer();
		}
	};

	E_AUTO( file_index )




}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
