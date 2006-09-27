/*
	Header for the 'dbsctt' library by Claude SIMON (csimon@epeios.org)
	Copyright (C) 2004 Claude SIMON (csimon@epeios.org).

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

#ifndef DBSCTT__INC
#define DBSCTT__INC

#define DBSCTT_NAME		"DBSCTT"

#define	DBSCTT_VERSION	"$Revision$"

#define DBSCTT_OWNER		"Claude SIMON (csimon@epeios.org)"

#include "ttr.h"

extern class ttr_tutor &DBSCTTTutor;

#if defined( XXX_DBG ) && !defined( DBSCTT_NODBG )
#define DBSCTT_DBG
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
//D DataBaSe DaTA 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "str.h"
#include "dtfptb.h"
#include "stk.h"
#include "lstbch.h"
#include "lstctn.h"
#include "flm.h"
#include "que.h"

namespace dbsctt {

	using mdr::size__;

	typedef bso::char__ atom__;
	typedef str::string_ datum_;
	typedef str::string datum;

	E_ROW( datum_row__ );

	typedef tym::memory_<atom__, datum_row__> memory_;

	class storage_
	{
	private:
		/* Retourne le nombre d'octets que l'on peut effectivement écrire sachant que l'on veut écrire un maximum de 
		'Wanted' octets et qu'un maximum de 'Available' octets sont disponibles, et que l'on veut également y stocker la
		taille */
		size__ _AmountWritable(
			size__ Wanted,
			size__ Available )
		{
#ifdef DBSCTT_DBG
			if ( Available == 0 )
				ERRc();
#endif
			if ( Wanted >= Available )
				Wanted = Available - 1;	// Il faut au moins 1 octet pour stocker la taille.

			while ( Available < ( dtfptb::GetSizeLength( Wanted ) + Wanted ) )
				Wanted--;

			return Wanted;
		}
		size__ _Store(
			const datum_ &Data,
			size__ Offset,
			datum_row__ &Row,
			size__ Available )
		{
			size__ &Amount = Available;
			
			Amount = _AmountWritable( Data.Amount() - Offset, Available );

			dtfptb::size_buffer__ SizeBuffer;
			bso::ubyte__ SizeLength = dtfptb::GetSizeLength( Amount );

			dtfptb::PutSize( Amount, SizeBuffer );

			Memory.Store( (const atom__ *)SizeBuffer, SizeLength, Row );

			*Row += SizeLength;

			Memory.Store( *(const memory_ *)&Data, Amount, Offset, Row );

			*Row += Data.Amount() - Offset;

			return Amount;
		}
		/* Ajoute 'Data' à la position 'Row', sachant que 'Unallocated' est la position du premier octet non alloué.
		L'espace necessaire est alloué. Retourne la position du nouveau premier octet non alloué. */
		datum_row__ _Store(
			const datum_ &Data,
			size__ Offset,
			datum_row__ Unallocated )
		{
			size__ DataAmount = Data.Amount() - Offset;
			size__ TotalSize = DataAmount + dtfptb::GetSizeLength( DataAmount );

			Memory.Allocate( *Unallocated + TotalSize );

			_Store( Data, Offset, Unallocated, TotalSize );	// 'Unallocated' mis à jour par cette méthode.

			return Unallocated;
		}
		size__ _GetComputedSize(
			datum_row__ Row,
			datum_row__ Unallocated ) const
		{
			dtfptb::size_buffer__ SizeBuffer;
			bso::ubyte__ PossibleSizeLength = sizeof( SizeBuffer );

			if ( ( *Unallocated - *Row ) < PossibleSizeLength )
				PossibleSizeLength = (bso::ubyte__)( *Unallocated - *Row );

			Memory.Recall( Row, PossibleSizeLength, (atom__ *)SizeBuffer );

			return dtfptb::GetSize( SizeBuffer );
		}
		size__ _GetRawSize(
			datum_row__ Row,
			datum_row__ Unallocated ) const
		{
			size__ Size = _GetComputedSize( Row, Unallocated );

			return Size + dtfptb::GetSizeLength( Size );
		}
	public:
		memory_ Memory;
		struct s
		{
			memory_::s Memory;
		};
		storage_( s &S )
		: Memory( S.Memory )
		{}
		void reset( bso::bool__ P = true )
		{
			Memory.reset( P );
		}
		void plug( mdr::E_MEMORY_DRIVER__ &MD )
		{
			Memory.plug( MD );
		}
		void plug( mmm::E_MULTIMEMORY_ &MM )
		{
			Memory.plug( MM );
		}
		storage_ operator =( const storage_ &S )
		{
			ERRu();	// Impossible de dupliquer : ne connaît pas sa taille.

			return *this;	// Pour éviter un 'warning'.
		}
		void Init( void )
		{
			Memory.Init();
		}
		/* Stocke 'Data' à la position 'Row'. sachant qu'il y a 'Available' octets de disponibles à cette position.
		'Unallocated' est la position du premier octet non alloué. S'il n'y a pas assez de place à 'Row', alors
		une partie de 'Data' est placé à 'Unavailable' (en allouant l'espace nécessaire), qui est alors modifié
		pour pointer sur la nouvelle position du premier octet non alloué. Si 'Row', au retour, est != 'NONE',
		alors il reste 'Available' octets à 'Row'. */
/*
		datum_row__ Write(
			const datum_ &Data,
			datum_row__ &Row,
			size__ &Available,
			datum_row__ &Unallocated )
		{
			size__ Written = _Store( Data, Row, Available );
			size__ TotalWritten = Written + dtfptb::GetSizeLength( Written );
			datum_row__ Rest = NONE;

			Available -= TotalWritten;

			if ( Available != 0 )
				*Row += TotalWritten;
			else
				Row = NONE;

			if ( Written != Data.Amount() ) {
				Rest = Unallocated;
				Unallocated = _Store( Data, Written, Unallocated );
			}

			return Rest;
		}
*/
		size__ Store(
			const datum_ &Data,
			size__ Offset,
			datum_row__ &Row,
			size__ &Available )
		{
			size__ Written = _Store( Data, Offset, Row, Available );
			size__ TotalWritten = Written + dtfptb::GetSizeLength( Written );

			Available -= TotalWritten;

			if ( Available == 0 )
				Row = NONE;

			return Written;
		}
		/* Ajoute 'Data' à 'Row', qui est la position du premier octet non alloué. La place nécessaire est allouée et
		la nouvelle position du premier octet non alloué et retourné. */
		datum_row__ Append(
			const datum_ &Data,
			size__ Offset,
			datum_row__ Row )
		{
			return _Store( Data, Offset, Row );
		}
		size__ GetComputedSize(
			datum_row__ Row,
			datum_row__ Unallocated ) const
		{
			return _GetComputedSize( Row, Unallocated );
		}
		size__ GetRawSize(
			datum_row__ Row,
			datum_row__ Unallocated ) const
		{
			return _GetRawSize( Row, Unallocated );
		}
		/* Place un marqueur de taille à 'DataRow' sachant qu'il y a 'Size' octets de disponibles. Retourne le nombre
		d'octets effectivement disponibles à cette position aprés y avoir placé le marquer de taille. */
		size__ StoreSize(
			datum_row__ Row,
			size__ Size )
		{
#ifdef DBSCTT_DBG
			if ( Size == 0 )
				ERRu();
#endif
			dtfptb::size_buffer__ SizeBuffer;

			Size = _AmountWritable( Size, Size );

			dtfptb::PutSize( Size, SizeBuffer );

			Memory.Store( (const atom__ *)SizeBuffer, dtfptb::GetSizeLength( Size ), Row );

			return Size;
		}
		void Retrieve(
			datum_row__ Row,
			datum_ &Data,
			datum_row__ Unallocated ) const
		{
			mdr::size__ Size = _GetComputedSize( Row, Unallocated );

			Data.Append( *(const tym::memory_<bso::char__, epeios::row__>*)&Memory, Size, *Row + dtfptb::GetSizeLength( Size ) );
		}
	};

	E_AUTO( storage )

	// Caratéristique d'un emplacement libre.
	struct available__
	{
		// Position.
		datum_row__ Row;
		// Taille brute, c'est-à-dire sans tenir compte de la place occupée par le marqueur de taille.
		size__ RawSize;
		available__( void )
		{
			Row = NONE;
			RawSize = 0;
		}
		bool operator !=( const available__ &A ) const
		{
			return ( Row != A.Row ) || ( RawSize != A.RawSize );
		}
	};

	// Pile contenant tous les emplacement libres.
	typedef stk::E_BSTACK_( available__ ) availables_;
	E_AUTO( availables )


	// 'datum_row__' portable
	typedef bso::p_ulong__	p_datum_row__;

	struct entry__
	{
	public:
		datum_row__ Head;
		datum_row__ Tail;
		entry__( void )
		{
			Head = Tail = NONE;
		}
	};

	E_ROW( rrow__ );	//Record row.

	typedef lstbch::E_LBUNCHt_( entry__, rrow__ ) entries_;

	E_ROW( crow__ );	// Cache row.

	typedef lstctn::E_LXMCONTAINERt_( datum_, crow__ ) _container_;
	typedef que::E_MQUEUEt_( crow__ ) _queue_;
	typedef bch::E_BUNCHt_( crow__, rrow__ ) _list_;
	typedef bch::E_BUNCHt_( rrow__, crow__ ) _links_;

	#define DBSCTT_CACHE_DEFAULT_AMOUNT_MAX	( 2 << 15 )

	class _cache_
	{
	private:
		void _PutToHead( crow__ Row )
		{
			if ( Queue.Exists( Row ) && Queue.IsMember( Row ) )
				Queue.Delete( Row );

			if ( Queue.IsEmpty() )
				Queue.Create( Row );
			else
				Queue.BecomePrevious( Row, Queue.Head() );
		}
		bso::bool__ _IsMember( rrow__ Row ) const
		{
			if ( List.Exists( Row ) )
				return List( Row ) != NONE;
			else
				return false;
		}
	public:
		_container_ Container;
		_queue_ Queue;
		_list_ List;
		_links_ Links;
		struct s {
			_container_::s Container;
			_queue_::s Queue;
			_list_::s List;
			_links_::s Links;
			bso::ulong__ AmountMax;
		} &S_;
		_cache_( s &S )
		: S_( S ),
		  Container( S.Container ),
		  Queue( S.Queue ),
		  List( S.List ),
		  Links( S.Links )
		{}
		void reset( bso::bool__ P = true )
		{
			Container.reset( P );
			Queue.reset( P );
			List.reset( P );
			Links.reset( P );

			S_.AmountMax = 0;
		}
		void plug( mmm::E_MULTIMEMORY_ &MM )
		{
			Container.plug( MM );
			Queue.plug( MM );
			List.plug( MM );
			Links.plug( MM );
		}
		_cache_ &operator =( const _cache_ &C )
		{
			Container = C.Container;
			Queue = C.Queue;
			List = C.List;
			Links = C.Links;

			S_.AmountMax = C.S_.AmountMax;

			return *this;
		}
		void Init(
			epeios::size__ Size,
			bso::ulong__ AmountMax = DBSCTT_CACHE_DEFAULT_AMOUNT_MAX )
		{
			reset();

			Container.Init();
			Queue.Init();
			List.Init();
			Links.Init();

			Queue.Allocate( AmountMax );
			Container.Allocate( AmountMax );

			Links.Allocate( AmountMax );
			Links.Set( NONE );

			List.Allocate( Size );
			List.Set( NONE );

			S_.AmountMax = AmountMax;
		}
		bso::bool__ Retrieve(
			rrow__ Row,
			datum_ &Datum )
		{
			bso::bool__ IsMember = false;

			if ( IsMember = _IsMember( Row ) ) {
				crow__ CacheRow = List( Row );
				Container.Recall( CacheRow, Datum );
				_PutToHead( CacheRow );
			}

			return IsMember;
		}
		void Store(
			const datum_ &Datum,
			rrow__ Row )
		{
#ifdef DBSCTT_DBG
			if ( _IsMember( Row ) )
				ERRu();
#endif
			crow__ CacheRow = NONE;

			if ( Queue.Amount() >= S_.AmountMax ) {
				CacheRow = Queue.Tail();
				List.Store( NONE, Links( CacheRow ) );
			} else
				CacheRow = Container.New();

			Container.Store( Datum, CacheRow );
			Links.Store( Row, CacheRow );
			_PutToHead( CacheRow );
			List.Store( CacheRow, Row );
		}
		void Remove( rrow__ Row )
		{
			if ( _IsMember( Row ) ) {
				crow__ CacheRow = List( Row );
				Container.Delete( CacheRow );
				List.Store( NONE, Links( CacheRow ) );
				Links.Store( NONE, CacheRow );
				Queue.Delete( CacheRow );
				List.Store( NONE, Row );
			}
		}
	};

	E_AUTO( _cache )

	class content_
	{
	private:
		size__ _StoreInAvailable(
			const datum_ &Data,
			size__ Offset,
			datum_row__ &Row )
		{
			available__ Available;
			size__ Written;

			Available = Availables.Pop();

			Row = Available.Row;

			Written = Storage.Store( Data, Offset, Available.Row, Available.RawSize );

			if ( Available.Row != NONE ) {
#ifdef DBSDCT_DBG
				if ( Written != ( Data.Amount() - Offset ) )
					ERRc();
#endif
				Storage.StoreSize( Available.Row, Available.RawSize );
				Availables.Push( Available );
			}

			return Written;
		}
		void _Append(
			const datum_ &Data,
			size__ Offset,
			datum_row__ &Row )
		{
			entry__ Entry;

			Row = S_.Unallocated;

			S_.Unallocated = Storage.Append( Data, Offset, S_.Unallocated );
		}
		rrow__ _Store(
			const datum_ &Data,
			rrow__ Row )
		{
			entry__ Entry;

			if ( Availables.Amount() != 0 ) {
				size__ Written;
	
				Written = _StoreInAvailable( Data, 0, Entry.Head );

				if ( ( Written != Data.Amount() )
					 && ( Availables.Amount() != 0 )
					 && ( Storage.GetComputedSize( Availables.Top().Row, S_.Unallocated ) >= ( Data.Amount() - Written ) ) )
					_StoreInAvailable( Data, Written, Entry.Tail );
				else
					_Append( Data, Written, Entry.Tail );
			} else
				_Append( Data, 0, Entry.Head );

			Entries.Store( Entry, Row );

			return Row;
		}
		void _Erase( datum_row__ DataRow )
		{
			available__ Available;

			Available.Row = DataRow;
			Available.RawSize = Storage.GetRawSize( DataRow, S_.Unallocated );

			Availables.Push( Available );
		}
		void _Erase( rrow__ Row )
		{
			entry__ Entry = Entries.Get( Row );

			if ( Entry.Tail != NONE )
				_Erase( Entry.Tail );

			if ( Entry.Head != NONE )
				_Erase( Entry.Head );

			Entry.Head = Entry.Tail = NONE;

			Entries.Store( Entry, Row );
		}
		void _Retrieve(
			datum_row__ Row,
			datum_ &Data ) const
		{
			Storage.Retrieve( Row, Data, S_.Unallocated );
		}
	public:
		storage_ Storage;
		availables_ Availables;
		entries_ Entries;
		struct s {
			storage_::s Storage;
			availables_::s Availables;
			entries_::s Entries;
			_cache_::s Cache;
			// Position du premier octet non alloué.
			datum_row__ Unallocated;
			time_t ModificationTimeStamp;
		} &S_;
		content_( s &S )
		: S_( S ),
		  Storage( S.Storage ),
		  Availables( S.Availables ),
		  Entries( S.Entries )
		{}
		void reset( bso::bool__ P = true )
		{
			Storage.reset( P );
			Availables.reset( P );
			Entries.reset( P );

			S_.Unallocated = 0;
			S_.ModificationTimeStamp = 0;
		}
		void plug( mmm::E_MULTIMEMORY_ &MM )
		{
			Storage.plug( MM );
			Availables.plug( MM );
			Entries.plug( MM );
		}
		content_ &operator =( const content_ &C )
		{
			Storage.Memory.Allocate( *C.S_.Unallocated );
			Storage.Memory.Store( C.Storage.Memory, *C.S_.Unallocated );
			S_.Unallocated = C.S_.Unallocated;
			S_.ModificationTimeStamp = C.S_.ModificationTimeStamp;

			Availables = C.Availables;

			Entries = C.Entries;

			return *this;
		}
		void Init( void )
		{
			Storage.Init();
			Availables.Init();
			Entries.Init();

			S_.Unallocated = 0;
			S_.ModificationTimeStamp = 0;
		}
		rrow__ Store( const datum_ &Data )
		{
			rrow__ Row = Entries.New();

			_Store( Data, Row );

			S_.ModificationTimeStamp = tol::Clock();

			return Row;
		}
		void Erase( rrow__ Row )
		{
			_Erase( Row );

			Entries.Delete( Row );

			S_.ModificationTimeStamp = tol::Clock();
		}
		void Store(
			const datum_ &Data,
			rrow__ Row )
		{
			_Erase( Row );

			_Store( Data, Row );

			S_.ModificationTimeStamp = tol::Clock();
		}
		// Retourne 'true' si l'enregistrement existe, faux sinon.
		bso::bool__ Retrieve(
			rrow__ Row,
			datum_ &Datum ) const
		{
			entry__ Entry = Entries.Get( Row );

			if ( Entry.Head != NONE )
				_Retrieve( Entry.Head, Datum );
			else
				return false;

			if ( Entry.Tail != NONE )
				_Retrieve( Entry.Tail, Datum );

			return true;
		}
		// Retourne 'true' si l'enregistrement existe, faux sinon.
		bso::bool__ Retrieve(
			rrow__ Row,
			datum_ &Datum,
			_cache_ &Cache ) const
		{
			bso::bool__ Exists = true;

			if ( ( &Cache == NULL ) || !Cache.Retrieve( Row, Datum ) ) {

				Exists = Retrieve( Row, Datum );

				if ( Exists && ( &Cache != NULL ) )
					Cache.Store( Datum, Row );
			}

			return Exists;
		}
		// Reconstruction de la liste des items disponibles dans 'Entries' (sous-objet 'list_').
		void RebuildLocations( void )
		{
			ERRl();
		}
		// Reconstruit la liste des portions inoccupés dans 'Storage'.
		void RebuildAvailables( void )
		{
			ERRl();
		}
		E_NAVt( Entries., rrow__ )
		E_RODISCLOSE_( time_t, ModificationTimeStamp );
	};

	E_AUTO( content )

	// Content stocké dans des fichiers.
	class file_content_
	: public content_
	{
	private:
		void _SaveLocationsAndAvailables( void ) const;
	public:
		str::string_ RootFileName;
		struct s
		: public content_::s
		{
			struct memory_driver__ {
				flm::E_FILE_MEMORY_DRIVER___
					Storage,
					Entries;
			} MemoryDriver;
			str::string_::s RootFileName;
		} &S_;
		file_content_( s &S )
		: S_( S ), 
		  content_( S ),
		  RootFileName( S.RootFileName )
		{}
		void reset( bso::bool__ P = true )
		{
			if ( P ) {
				if ( ( RootFileName.Amount() != 0 ) && ( ModificationTimeStamp() != 0 ) )
					_SaveLocationsAndAvailables();
			}

			S_.MemoryDriver.Storage.reset( P );
			S_.MemoryDriver.Entries.reset( P );
			RootFileName.reset( P );
			content_::reset( P );
		}
		void plug( mmm::E_MULTIMEMORY_ & )
		{
			ERRu();	// Cette méthode n'a pas de sens dans ce contexte.
		}
		file_content_ &operator =( const file_content_ &C )
		{
			content_::operator =( C );

			return *this;
		}
		bso::bool__ Init(
			const str::string_ &RootFileName,
			mdr::mode Mode );
		void WriteLocationsAndAvailablesFiles( void )	// Met à jour les fichiers.
		{
			_SaveLocationsAndAvailables();
		}
		void CloseFiles( void )	// Pour libèrer les 'file handlers'.
		{
			S_.MemoryDriver.Storage.Liberer();
			S_.MemoryDriver.Entries.Liberer();
		}
		void SwitchMode( mdr::mode Mode )
		{
			S_.MemoryDriver.Storage.Mode( Mode );
			S_.MemoryDriver.Entries.Mode( Mode );
		}
	};

	E_AUTO( file_content )
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
