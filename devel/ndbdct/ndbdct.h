/*
	Header for the 'ndbdct' library by Claude SIMON (csimon at zeusw dot org)
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

# include "err.h"
# include "sdr.h"
# include "tys.h"
# include "ndbbsc.h"
# include "lstbch.h"

namespace ndbdct {

	using sdr::size__;

	using ndbbsc::rrow__;
	using ndbbsc::datum_;
	using ndbbsc::datum;
	using ndbbsc::cache_;
	using ndbbsc::cache;

	E_ROW( drow__ );	// 'Datum row'.

	typedef tys::storage_<ndbbsc::atom__, drow__> _storage_;

	class storage_
	{
	private:
		/* Retourne le nombre d'octets que l'on peut effectivement �crire sachant que l'on veut �crire un maximum de 
		'Wanted' octets et qu'un maximum de 'Available' octets sont disponibles, et que l'on veut �galement y stocker la
		taille */
		size__ _AmountWritable(
			size__ Wanted,
			size__ Available )
		{
#ifdef NDBDCT_DBG
			if ( Available == 0 )
				ERRPrm();
#endif
			if ( Wanted >= Available )
				Wanted = Available - 1;	// Il faut au moins 1 octet pour stocker la taille.

			while ( Available < ( dtfptb::OldGetSizeLength( Wanted ) + Wanted ) )
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
			bso::u8__ SizeLength = dtfptb::OldGetSizeLength( Amount );

			dtfptb::OldPutSize( Amount, SizeBuffer );

			Memory.Store( (const ndbbsc::atom__ *)SizeBuffer, SizeLength, Row );

			*Row += SizeLength;

			Memory.Store( *(const _storage_ *)&Data, Amount, Row, Offset );

			*Row += Data.Amount() - Offset;

			return Amount;
		}
		/* Ajoute 'Data' � la position 'Row', sachant que 'Unallocated' est la position du premier octet non allou�.
		L'espace necessaire est allou�. Retourne la position du nouveau premier octet non allou�. */
		drow__ _Store(
			const datum_ &Data,
			size__ Offset,
			drow__ Unallocated )
		{
			size__ DataAmount = Data.Amount() - Offset;
			size__ TotalSize = DataAmount + dtfptb::OldGetSizeLength( DataAmount );

			Memory.Allocate( *Unallocated + TotalSize );

			_Store( Data, Offset, Unallocated, TotalSize );	// 'Unallocated' mis � jour par cette m�thode.

			return Unallocated;
		}
		size__ _GetComputedSize(
			drow__ Row,
			drow__ Unallocated ) const
		{
			dtfptb::size_buffer__ SizeBuffer;
			bso::u8__ PossibleSizeLength = sizeof( SizeBuffer );

			if ( ( *Unallocated - *Row ) < PossibleSizeLength )
				PossibleSizeLength = (bso::u8__)( *Unallocated - *Row );

			Memory.Recall( Row, PossibleSizeLength, (ndbbsc::atom__ *)SizeBuffer );

			return dtfptb::OldGetSize( SizeBuffer );
		}
		size__ _GetRawSize(
			drow__ Row,
			drow__ Unallocated ) const
		{
			size__ Size = _GetComputedSize( Row, Unallocated );

			return Size + dtfptb::OldGetSizeLength( Size );
		}
	public:
		_storage_ Memory;
		struct s
		{
			_storage_::s Memory;
		};
		storage_( s &S )
		: Memory( S.Memory )
		{}
		void reset( bso::bool__ P = true )
		{
			Memory.reset( P );
		}
		void plug( sdr::E_SDRIVER__ &SD )
		{
			Memory.plug( SD );
		}
		void plug( ags::E_ASTORAGE_ &AS )
		{
			Memory.plug( AS );
		}
		storage_ operator =( const storage_ &S )
		{
			ERRFwk();	// Impossible de dupliquer : ne conna�t pas sa taille.

			return *this;	// Pour �viter un 'warning'.
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
			size__ TotalWritten = Written + dtfptb::OldGetSizeLength( Written );

			Available -= TotalWritten;

			if ( Available == 0 )
				Row = E_NIL;

			return Written;
		}
		/* Ajoute 'Data' � 'Row', qui est la position du premier octet non allou�. La place n�cessaire est allou�e et
		la nouvelle position du premier octet non allou� et retourn�. */
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
		/* Place un marqueur de taille � 'DataRow' sachant qu'il y a 'Size' octets de disponibles. Retourne le nombre
		d'octets effectivement disponibles � cette position apr�s y avoir plac� le marquer de taille. */
		size__ StoreSize(
			drow__ Row,
			size__ Size )
		{
#ifdef NDBDCT_DBG
			if ( Size == 0 )
				ERRPrm();
#endif
			dtfptb::size_buffer__ SizeBuffer;

			Size = _AmountWritable( Size, Size );

			dtfptb::OldPutSize( Size, SizeBuffer );

			Memory.Store( (const ndbbsc::atom__ *)SizeBuffer, dtfptb::OldGetSizeLength( Size ), Row );

			return Size;
		}
		void Retrieve(
			drow__ Row,
			datum_ &Data,
			drow__ Unallocated ) const
		{
			sdr::size__ Size = _GetComputedSize( Row, Unallocated );

			Data.Append( *(const tys::storage_<bso::char__, sdr::row__>*)&Memory, Size, *Row + dtfptb::OldGetSizeLength( Size ) );
		}
	};

	E_AUTO( storage )

	// Carat�ristique d'un emplacement libre.
	struct available__
	{
		// Position.
		drow__ Row;
		// Taille brute, c'est-�-dire sans tenir compte de la place occup�e par le marqueur de taille.
		size__ RawSize;
		available__( void )
		{
			Row = E_NIL;
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
			Head = Tail = E_NIL;
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

			if ( Available.Row != E_NIL ) {
#ifdef NDBDCT_DBG
				if ( Written != ( Data.Amount() - Offset ) )
					ERRPrm();
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

			if ( Entry.Tail != E_NIL )
				_Erase( Entry.Tail );

			if ( Entry.Head != E_NIL )
				_Erase( Entry.Head );

			Entry.Head = Entry.Tail = E_NIL;

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
			S_.ModificationEpochTimeStamp = tol::EpochTime( false );
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
			// Position du premier octet non allou�.
			drow__ Unallocated;
			time_t ModificationEpochTimeStamp;
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
			S_.ModificationEpochTimeStamp = 0;
		}
		void plug( ags::E_ASTORAGE_ &AS )
		{
			Storage.plug( AS );
			Availables.plug( AS );
			Entries.plug( AS );
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
			S_.ModificationEpochTimeStamp = 0;
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

			if ( Entry.Head != E_NIL )
				_Retrieve( Entry.Head, Datum );
			else
				return false;

			if ( Entry.Tail != E_NIL )
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
			ERRVct();
		}
		// Reconstruit la liste des portions inoccup�s dans 'Storage'.
		void RebuildAvailables( void )
		{
			ERRVct();
		}
		E_NAVt( Entries., rrow__ )
		E_RODISCLOSE_( time_t, ModificationEpochTimeStamp );
	};

	E_AUTO( dynamic_content )

	// Content stock� dans des fichiers.
	class dynamic_content_atomized_file_manager___
	{
	private:
		dynamic_content_ *_Content;
		str::string _BaseFileName;
		tys::storage_file_manager___ _StorageFileManager;
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
		void _SaveAvailables( void ) const;
		bso::bool__ _LoadAvailables( void );
		void _ErasePhysically( void );
	public:
		void reset( bso::bool__ P = true )
		{
			// N�cessaire, sinon le 'Settle(...)' qui suit peut ne pas fonctionner correctement.
			_StorageFileManager.ReleaseFile();
			_EntriesFileManager.ReleaseFile();

			if ( P ) {
				Settle();	// Lanc� explicitement, pour la sauvegarde des 'availables'.
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
			const str::string_ &BaseFileName,
			fil::mode__ Mode,
			fls::id__ ID );
		void Set( dynamic_content_ &Content )
		{
			if ( _Content != NULL )
				ERRPrm();

			_Content = &Content;
		}
		uys::state__ Bind( void )
		{
			uys::state__ State = _StorageFileManager.Bind();

			if ( uys::IsError( State ) ) {
				_StorageFileManager.Bind();
				return State;
			}

			if ( _EntriesFileManager.Bind() != State ) {
				_StorageFileManager.reset();
				_EntriesFileManager.reset();
				return uys::sInconsistent;
			}

			if ( uys::Exists( State ) )
				if ( !_LoadAvailables() ) {
					_StorageFileManager.reset();
					_EntriesFileManager.reset();
					State = uys::sInconsistent;
				}

			return State;
		}
		uys::state__ Settle( void )
		{
			uys::state__ State = _StorageFileManager.Settle();

			if ( _EntriesFileManager.Settle() != State )
				State = uys::sInconsistent;

			if ( (_Content != NULL ) && ( _BaseFileName.Amount() != 0 ) && ( _Content->ModificationEpochTimeStamp() != 0 ) )
				_SaveAvailables();

			return State;
		}
		void WriteAvailables( void )	// Met � jour les fichiers.
		{
			_SaveAvailables();
		}
		void CloseFiles( void )	// Pour lib�rer les 'file handlers'.
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
		friend uys::state__ Plug(
			dynamic_content_ &Content,
			dynamic_content_atomized_file_manager___ &FileManager );
	};

	uys::state__ Plug(
		dynamic_content_ &Content,
		dynamic_content_atomized_file_manager___ &FileManager );
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
