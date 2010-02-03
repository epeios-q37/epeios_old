/*
	Header for the 'ndbdct' library by Claude SIMON (csimon at zeusw dot org)
	Copyright (C) 2004 Claude SIMON.

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

#ifndef NDBDCT__INC
#define NDBDCT__INC

#define NDBDCT_NAME		"NDBDCT"

#define	NDBDCT_VERSION	"$Revision$"

#define NDBDCT_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &NDBDCTTutor;

#if defined( XXX_DBG ) && !defined( NDBDCT_NODBG )
#define NDBDCT_DBG
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
//D New DataBase Dynamic ConTent (will be moved on its own project in the future) 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "mdr.h"
#include "ndbbsc.h"
#include "lstbch.h"

namespace ndbdct {

	using mdr::size__;

	using ndbbsc::rrow__;
	using ndbbsc::datum_;
	using ndbbsc::datum;
	using ndbbsc::cache_;
	using ndbbsc::cache;

	E_ROW( drow__ );	// 'Datum row'.

	typedef tym::memory_<ndbbsc::atom__, drow__> memory_;

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
#ifdef NDBDCT_DBG
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
			drow__ &Row,
			size__ Available )
		{
			size__ &Amount = Available;
			
			Amount = _AmountWritable( Data.Amount() - Offset, Available );

			dtfptb::size_buffer__ SizeBuffer;
			bso::ubyte__ SizeLength = dtfptb::GetSizeLength( Amount );

			dtfptb::PutSize( Amount, SizeBuffer );

			Memory.Store( (const ndbbsc::atom__ *)SizeBuffer, SizeLength, Row );

			*Row += SizeLength;

			Memory.Store( *(const memory_ *)&Data, Amount, Row, Offset );

			*Row += Data.Amount() - Offset;

			return Amount;
		}
		/* Ajoute 'Data' à la position 'Row', sachant que 'Unallocated' est la position du premier octet non alloué.
		L'espace necessaire est alloué. Retourne la position du nouveau premier octet non alloué. */
		drow__ _Store(
			const datum_ &Data,
			size__ Offset,
			drow__ Unallocated )
		{
			size__ DataAmount = Data.Amount() - Offset;
			size__ TotalSize = DataAmount + dtfptb::GetSizeLength( DataAmount );

			Memory.Allocate( *Unallocated + TotalSize );

			_Store( Data, Offset, Unallocated, TotalSize );	// 'Unallocated' mis à jour par cette méthode.

			return Unallocated;
		}
		size__ _GetComputedSize(
			drow__ Row,
			drow__ Unallocated ) const
		{
			dtfptb::size_buffer__ SizeBuffer;
			bso::ubyte__ PossibleSizeLength = sizeof( SizeBuffer );

			if ( ( *Unallocated - *Row ) < PossibleSizeLength )
				PossibleSizeLength = (bso::ubyte__)( *Unallocated - *Row );

			Memory.Recall( Row, PossibleSizeLength, (ndbbsc::atom__ *)SizeBuffer );

			return dtfptb::GetSize( SizeBuffer );
		}
		size__ _GetRawSize(
			drow__ Row,
			drow__ Unallocated ) const
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
		size__ Store(
			const datum_ &Data,
			size__ Offset,
			drow__ &Row,
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
		drow__ Append(
			const datum_ &Data,
			size__ Offset,
			drow__ Row )
		{
			return _Store( Data, Offset, Row );
		}
		size__ GetComputedSize(
			drow__ Row,
			drow__ Unallocated ) const
		{
			return _GetComputedSize( Row, Unallocated );
		}
		size__ GetRawSize(
			drow__ Row,
			drow__ Unallocated ) const
		{
			return _GetRawSize( Row, Unallocated );
		}
		/* Place un marqueur de taille à 'DataRow' sachant qu'il y a 'Size' octets de disponibles. Retourne le nombre
		d'octets effectivement disponibles à cette position aprés y avoir placé le marquer de taille. */
		size__ StoreSize(
			drow__ Row,
			size__ Size )
		{
#ifdef NDBDCT_DBG
			if ( Size == 0 )
				ERRu();
#endif
			dtfptb::size_buffer__ SizeBuffer;

			Size = _AmountWritable( Size, Size );

			dtfptb::PutSize( Size, SizeBuffer );

			Memory.Store( (const ndbbsc::atom__ *)SizeBuffer, dtfptb::GetSizeLength( Size ), Row );

			return Size;
		}
		void Retrieve(
			drow__ Row,
			datum_ &Data,
			drow__ Unallocated ) const
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
		drow__ Row;
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

	struct entry__
	{
	public:
		drow__ Head;
		drow__ Tail;
		entry__( void )
		{
			Head = Tail = NONE;
		}
	};

	typedef lstbch::E_LBUNCHt_( entry__, rrow__ ) entries_;

	typedef lstbch::list_bunch_file_manager___ entries_file_manager___;

	class dynamic_content_
	{
	private:
		size__ _StoreInAvailable(
			const datum_ &Data,
			size__ Offset,
			drow__ &Row )
		{
			available__ Available;
			size__ Written;

			Available = Availables.Pop();

			Row = Available.Row;

			Written = Storage.Store( Data, Offset, Available.Row, Available.RawSize );

			if ( Available.Row != NONE ) {
#ifdef NDBDCT_DBG
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
			drow__ &Row )
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
		void _Erase( drow__ DataRow )
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
			drow__ Row,
			datum_ &Data ) const
		{
			Storage.Retrieve( Row, Data, S_.Unallocated );
		}
		void _Touch( void )
		{
			S_.ModificationTimeStamp = tol::Clock( false );
		}
	public:
		storage_ Storage;
		availables_ Availables;
		entries_ Entries;
		struct s
		{
			storage_::s Storage;
			availables_::s Availables;
			entries_::s Entries;
			// Position du premier octet non alloué.
			drow__ Unallocated;
			time_t ModificationTimeStamp;
		} &S_;
		dynamic_content_( s &S )
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
		dynamic_content_ &operator =( const dynamic_content_ &DC )
		{
			if ( ( Amount() != 0 ) || ( DC.Amount() != 0 ) ) {
				Storage.Memory.Allocate( *DC.S_.Unallocated );
				Storage.Memory.Store( DC.Storage.Memory, *DC.S_.Unallocated );
				S_.Unallocated = DC.S_.Unallocated;
				// S_.ModificationTimeStamp = DC.S_.ModificationTimeStamp;	// Ecraser par le '_Touch()' ci-dessous.

				Availables = DC.Availables;

				Entries = DC.Entries;

				_Touch();
			}

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

			_Touch();

			return Row;
		}
		void Erase( rrow__ Row )
		{
			_Erase( Row );

			Entries.Delete( Row );

			_Touch();
		}
		void Store(
			const datum_ &Data,
			rrow__ Row )
		{
			_Erase( Row );

			_Store( Data, Row );

			_Touch();
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
			cache_ &Cache ) const
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

	E_AUTO( dynamic_content )

	// Content stocké dans des fichiers.
	class dynamic_content_atomized_file_manager___
	{
	private:
		dynamic_content_ *_Content;
		str::string _BaseFileName;
		tym::memory_file_manager___ _StorageFileManager;
		entries_file_manager___ _EntriesFileManager;
		fil::mode__ _Mode;
		time_t _GetUnderlyingFilesLastModificationTime( void ) const
		{
			time_t ContentTimeStamp, EntriesTimeStamp;

			ContentTimeStamp = _StorageFileManager.TimeStamp();
			EntriesTimeStamp = _EntriesFileManager.TimeStamp();

			if ( ContentTimeStamp > EntriesTimeStamp )
				return ContentTimeStamp;
			else
				return EntriesTimeStamp;
		}
		void _SaveLocationsAndAvailables( void ) const;
		void _ErasePhysically( void );
	public:
		void reset( bso::bool__ P = true )
		{
			_StorageFileManager.ReleaseFile();
			_EntriesFileManager.ReleaseFile();
			// Pour que les 'TimeStamp' des fichiers soient mis à jour.

			if ( P ) {
				if ( (_Content != NULL ) && ( _BaseFileName.Amount() != 0 ) && ( _Content->ModificationTimeStamp() != 0 ) )
					_SaveLocationsAndAvailables();
			}

			_StorageFileManager.reset( P );
			_EntriesFileManager.reset( P );
			_Mode = fil::m_Undefined;
			_BaseFileName.reset( P );
			_Content = NULL;
		}
		dynamic_content_atomized_file_manager___( void )
		{
			reset( false );
		}
		~dynamic_content_atomized_file_manager___( void )
		{
			reset();
		}
		void Init(
			dynamic_content_ &Content,
			const str::string_ &BaseFileName,
			fil::mode__ Mode,
			flm::id__ ID );
		bso::bool__ ConnectToFiles( void );
		void WriteLocationsAndAvailablesFiles( void )	// Met à jour les fichiers.
		{
			_SaveLocationsAndAvailables();
		}
		void CloseFiles( void )	// Pour libèrer les 'file handlers'.
		{
			_StorageFileManager.ReleaseFile();
			_EntriesFileManager.ReleaseFile();
		}
		void SwitchMode( fil::mode__ Mode )
		{
			if ( Mode != _Mode ) {
				_StorageFileManager.Mode( Mode );
				_EntriesFileManager.Mode( Mode );

				_Mode = Mode;
			}
		}
		const str::string_ &BaseFileName( void ) const
		{
			return _BaseFileName;
		}
	};
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
