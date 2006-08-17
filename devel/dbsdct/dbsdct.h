/*
	Header for the 'dbsdct' library by Claude SIMON (csimon@epeios.org)
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

#ifndef DBSDCT__INC
#define DBSDCT__INC

#define DBSDCT_NAME		"DBSDCT"

#define	DBSDCT_VERSION	"$Revision$"

#define DBSDCT_OWNER		"Claude SIMON (csimon@epeios.org)"

#include "ttr.h"

extern class ttr_tutor &DBSDCTTutor;

#if defined( XXX_DBG ) && !defined( DBSDCT_NODBG )
#define DBSDCT_DBG
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
//D DataBaSe DataContent 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "tym.h"
#include "tol.h"
#include "dtfptb.h"
#include "str.h"
#include "stk.h"
#include "lstbch.h"
#include "flm.h"

namespace dbsdct {

	using mdr::size__;

	typedef bso::char__ datum__;
	typedef str::string_ data_;
	typedef str::string data;

	E_ROW( data_row__ );

	typedef tym::memory_<datum__, data_row__> memory_;

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
#ifdef DBSDCT_DBG
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
			const data_ &Data,
			size__ Offset,
			data_row__ &Row,
			size__ Available )
		{
			size__ &Amount = Available;
			
			Amount = _AmountWritable( Data.Amount() - Offset, Available );

			dtfptb::size_buffer__ SizeBuffer;
			bso::ubyte__ SizeLength = dtfptb::GetSizeLength( Amount );

			dtfptb::PutSize( Amount, SizeBuffer );

			Memory.Store( (const datum__ *)SizeBuffer, SizeLength, Row );

			*Row += SizeLength;

			Memory.Store( *(const memory_ *)&Data, Amount, Offset, Row );

			*Row += Data.Amount() - Offset;

			return Amount;
		}
		/* Ajoute 'Data' à la position 'Row', sachant que 'Unallocated' est la position du premier octet non alloué.
		L'espace necessaire est alloué. Retourne la position du nouveau premier octet non alloué. */
		data_row__ _Store(
			const data_ &Data,
			size__ Offset,
			data_row__ Unallocated )
		{
			size__ DataAmount = Data.Amount() - Offset;
			size__ TotalSize = DataAmount + dtfptb::GetSizeLength( DataAmount );

			Memory.Allocate( *Unallocated + TotalSize );

			_Store( Data, Offset, Unallocated, TotalSize );	// 'Unallocated' mis à jour par cette méthode.

			return Unallocated;
		}
		size__ _GetComputedSize(
			data_row__ Row,
			data_row__ Unallocated ) const
		{
			dtfptb::size_buffer__ SizeBuffer;
			bso::ubyte__ PossibleSizeLength = sizeof( SizeBuffer );

			if ( ( *Unallocated - *Row ) < PossibleSizeLength )
				PossibleSizeLength = (bso::ubyte__)( *Unallocated - *Row );

			Memory.Recall( Row, PossibleSizeLength, (datum__ *)SizeBuffer );

			return dtfptb::GetSize( SizeBuffer );
		}
		size__ _GetRawSize(
			data_row__ Row,
			data_row__ Unallocated ) const
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
		data_row__ Write(
			const data_ &Data,
			data_row__ &Row,
			size__ &Available,
			data_row__ &Unallocated )
		{
			size__ Written = _Store( Data, Row, Available );
			size__ TotalWritten = Written + dtfptb::GetSizeLength( Written );
			data_row__ Rest = NONE;

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
			const data_ &Data,
			size__ Offset,
			data_row__ &Row,
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
		data_row__ Append(
			const data_ &Data,
			size__ Offset,
			data_row__ Row )
		{
			return _Store( Data, Offset, Row );
		}
		size__ GetComputedSize(
			data_row__ Row,
			data_row__ Unallocated ) const
		{
			return _GetComputedSize( Row, Unallocated );
		}
		size__ GetRawSize(
			data_row__ Row,
			data_row__ Unallocated ) const
		{
			return _GetRawSize( Row, Unallocated );
		}
		/* Place un marqueur de taille à 'DataRow' sachant qu'il y a 'Size' octets de disponibles. Retourne le nombre
		d'octets effectivement disponibles à cette position aprés y avoir placé le marquer de taille. */
		size__ StoreSize(
			data_row__ Row,
			size__ Size )
		{
#ifdef DBSDCT_DBG
			if ( Size == 0 )
				ERRu();
#endif
			dtfptb::size_buffer__ SizeBuffer;

			Size = _AmountWritable( Size, Size );

			dtfptb::PutSize( Size, SizeBuffer );

			Memory.Store( (const datum__ *)SizeBuffer, dtfptb::GetSizeLength( Size ), Row );

			return Size;
		}
		void Retrieve(
			data_row__ Row,
			data_ &Data,
			data_row__ Unallocated ) const
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
		data_row__ Row;
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


	// 'data_row__' portable
	typedef bso::p_ulong__	p_data_row__;

	struct entry__
	{
	public:
		data_row__ Head;
		data_row__ Tail;
		entry__( void )
		{
			Head = Tail = NONE;
		}
	};

	E_ROW( row__ );

	typedef lstbch::E_LBUNCHt_( entry__, row__ ) entries_;

	class content_
	{
	private:
		size__ _StoreInAvailable(
			const data_ &Data,
			size__ Offset,
			data_row__ &Row )
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
			const data_& Data,
			size__ Offset,
			data_row__ &Row )
		{
			entry__ Entry;

			Row = _S.Unallocated;

			_S.Unallocated = Storage.Append( Data, Offset, _S.Unallocated );
		}
		row__ _Store(
			const data_ &Data,
			row__ Row )
		{
			entry__ Entry;

			if ( Availables.Amount() != 0 ) {
				size__ Written;
	
				Written = _StoreInAvailable( Data, 0, Entry.Head );

				if ( ( Written != Data.Amount() )
					 && ( Availables.Amount() != 0 )
					 && ( Storage.GetComputedSize( Availables.Top().Row, _S.Unallocated ) >= ( Data.Amount() - Written ) ) )
					_StoreInAvailable( Data, Written, Entry.Tail );
				else
					_Append( Data, Written, Entry.Tail );
			} else
				_Append( Data, 0, Entry.Head );

			Entries.Store( Entry, Row );

			return Row;
		}
		void _Erase( data_row__ DataRow )
		{
			available__ Available;

			Available.Row = DataRow;
			Available.RawSize = Storage.GetRawSize( DataRow, _S.Unallocated );

			Availables.Push( Available );
		}
		void _Erase( row__ Row )
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
			data_row__ Row,
			data_ &Data ) const
		{
			Storage.Retrieve( Row, Data, _S.Unallocated );
		}
	public:
		storage_ Storage;
		availables_ Availables;
		entries_ Entries;
		struct s {
			storage_::s Storage;
			availables_::s Availables;
			entries_::s Entries;
			// Position du premier octet non alloué.
			data_row__ Unallocated;
		} &_S;
		content_( s &S )
		: _S( S ),
		  Storage( S.Storage ),
		  Availables( S.Availables ),
		  Entries( S.Entries )
		{}
		void reset( bso::bool__ P = true )
		{
			Storage.reset( P );
			Availables.reset( P );
			Entries.reset( P );

			_S.Unallocated = 0;
		}
		void plug( mmm::E_MULTIMEMORY_ &MM )
		{
			Storage.plug( MM );
			Availables.plug( MM );
			Entries.plug( MM );
		}
		content_ &operator =( const content_ &C )
		{
			Storage.Memory.Allocate( *C._S.Unallocated );
			Storage.Memory.Store( C.Storage.Memory, *C._S.Unallocated );
			_S.Unallocated = C._S.Unallocated;

			Availables = C.Availables;

			Entries = C.Entries;

			return *this;
		}
		void Init( void )
		{
			Storage.Init();
			Availables.Init();
			Entries.Init();

			_S.Unallocated = 0;
		}
		row__ Store( const data_ &Data )
		{
			row__ Row = Entries.New();

			_Store( Data, Row );

			return Row;
		}
		void Erase( row__ Row )
		{
			_Erase( Row );

			Entries.Delete( Row );
		}
		void Store(
			const data_ &Data,
			row__ Row )
		{
			_Erase( Row );

			_Store( Data, Row );
		}
		// Retourne 'true' si l'enregistrement existe, faux sinon.
		bso::bool__ Retrieve(
			row__ Row,
			data_ &Data ) const
		{
			entry__ Entry = Entries.Get( Row );

			if ( Entry.Head != NONE )
				_Retrieve( Entry.Head, Data );
			else
				return false;

			if ( Entry.Tail != NONE )
				_Retrieve( Entry.Tail, Data );

			return true;
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
		E_NAVt( Entries., row__ )
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
		} &_S;
		file_content_( s &S )
		: _S( S ), 
		  content_( S ),
		  RootFileName( S.RootFileName )
		{}
		void reset( bso::bool__ P = true )
		{
			if ( P ) {
				if ( RootFileName.Amount() != 0 )
					_SaveLocationsAndAvailables();
			}

			_S.MemoryDriver.Storage.reset( P );
			_S.MemoryDriver.Entries.reset( P );
			RootFileName.reset( P );
			content_::reset( P );
		}
		void plug( mmm::E_MULTIMEMORY_ & )
		{
			ERRu();	// Cette méthode n'a pas de sens dans ce contexte.
		}
		file_content_ &operator =( const content_ &C )
		{
			content_::operator =( C );

			return *this;
		}
		bso::bool__ Init( const str::string_ &RootFileName );
	};

	E_AUTO( file_content )

}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
