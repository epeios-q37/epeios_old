/*
	Header for the 'lst' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Copyright (C) 2000-2004 Claude SIMON (http://zeusw.org/intl/contact.html).

	This file is part of the Epeios (http://zeusw.org/epeios/) project.

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 3
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

#ifndef LST__INC
#define LST__INC

#define LST_NAME		"LST"

#define	LST_VERSION	"$Revision$"

#define LST_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"

#include "ttr.h"

extern class ttr_tutor &LSTTutor;

#if defined( XXX_DBG ) && !defined( LST_NODBG )
#define LST_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision$
//C Claude SIMON (http://zeusw.org/intl/contact.html)
//R $Date$

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D LiST 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "ids.h"
#include "bitbch.h"

namespace lst {

	typedef ids::E_IDS_STORE_( mdr::row__ ) store_;

	// Retourne l'id de la première entrée disponible (hors pile des 'released').
	uym::state__ WriteToFile_(
		const store_ &Store,
		const char *FileName );

	uym::state__ ReadFromFile_(
		const char *FileName,
		store_ &Store );

	inline uym::state__ Test_(
		const char *FileName,
		time_t ReferenceTimeStamp )
	{
		if ( FileName == NULL )
			return uym::sInconsistent;

		if ( !fil::FileExists( FileName ) )
			return uym::sAbsent;

		if ( fil::GetFileLastModificationTime( FileName ) <= ReferenceTimeStamp )
			return uym::sInconsistent;

		return uym::sExists;
	}

	mdr::row_t__ Successeur_(
		mdr::row_t__ Element,
		mdr::size__ Amount,
		const store_ &Libres );

	mdr::row_t__ Predecesseur_(
		mdr::row_t__ Element,
		const store_ &Libres );

	void Insert_(
		mdr::row_t__ First,
		mdr::row_t__ Last,
		store_ &Store );

	void MarkAsReleased_(
		mdr::row_t__ First,
		mdr::row_t__ Last,
		store_ &Store );


	//c Handle a list of objects. Use 'LIST_' rather than directly this class.
	template <typename r, typename r_t> class list_
	{
	protected:
		/*v Cette fonction est appelée lors d'allocations dans la liste;
		permet de synchroniser la taille de la liste avec d'autres ensembles;
		'Size' est la capacité allouée. */
		virtual void LSTAllocate(
			mdr::size__ Size,
			aem::mode__ Mode ) = 0;
	private:
		// Return the extent, based on 'Locations'.
		mdr::row_t__ Extent_( void ) const
		{
			return *Locations.GetFirstAvailable();
		}
		mdr::row_t__ Nouveau_( aem::mode__ Mode )
		{
			bso::bool__ Released = false;

			mdr::row_t__ New = *Locations.New( Released );

			if ( !Released )
				LSTAllocate( Extent_(), Mode );

			return New;
		}
		// Retourne l'élément succédant à 'Element', ou LST_INEXISTANT si inexistant.
		mdr::row_t__ Successeur_( mdr::row_t__ Element ) const
		{
			return lst::Successeur_( Element, Extent_(), Locations );
		}
		// Retourne l'élément précédent 'Element', ou LST_INEXISTANT si inexistant.
		mdr::row_t__ Predecesseur_( mdr::row_t__ Element ) const
		{
			return lst::Predecesseur_( Element, Locations );
		}
	public:
		//o Store of locations.
		store_ Locations;
		struct s
		{
			store_::s Locations;
		};
	// fonctions
		list_( s &S )
		: Locations( S.Locations )
		{}
		void reset( bool P = true )
		{
			Locations.reset( P );
		}
		void plug( mdr::E_MEMORY_DRIVER__ &M )
		{
			Locations.plug( M );
		}
		void plug( mmm::multimemory_ &M )
		{
			Locations.plug( M );
		}
		list_ &operator =( const list_ &L )
		{
			Locations = L.Locations;

			return *this;
		}
	/*	void ecrire( flo_sortie_ &Flot )
		{
			FLOEcrire( S_.Nombre, Flot );

			Libres.ecrire( Flot );
		}
		void lire( flo_entree_ &Flot )
		{
			FLOLire( Flot, S_.Nombre );

			Libres.lire( Flot );
		}
	*/	//f Initialiration.
		void Init( void )
		{
			Locations.Init();
		}
		//f Delete 'Entry'.
		void Delete( r Entry )
		{
			Locations.Release( *Entry );
		}
		//f Return the position of a new entry.
		r New( aem::mode__ Mode = aem::m_Default )
		{
			return (r_t)Nouveau_( Mode );
		}
		//f Return the row of a new entry. Use 'Row' if != 'NONE' (restoration purpose).
		r New(
			r Row,
			aem::mode__ Mode = aem::m_Default )
		{
			if ( Row != NONE ) {
				r FirstAvailable = *Locations.GetFirstAvailable();

				if ( Locations.New( *Row ) ) {

					LSTAllocate( *Row + 1, Mode );

					if ( Row != FirstAvailable )
						MarkAsReleased_( *FirstAvailable, *Row - 1, Locations );
				}

				return Row;
			} else
				return New();
		}
		//f Return the first entry if exists, 'NONE' if list empty.
		r First( void ) const
		{
			if ( Extent_() )
				if ( !Locations.IsAvailable( 0 ) )
					return 0;
				else
					return Successeur_( 0 );
			else
				return NONE;
		}
		r First( mdr::size__ Offset ) const
		{
			r Row = First();

			if ( Row != NONE )
				Row = Next( Row, Offset  );

			return Row;
		}
		//f Return the last entry, 'NONE' if list empty.
		r Last( void ) const
		{
			if ( Extent_() )
			{
				mdr::row_t__ P = Extent_() - 1;

				if ( !Locations.IsAvailable( P ) )
					return P;
				else
					return Predecesseur_( P );
			}
			else
				return NONE;
		}
		r Last( mdr::size__ Offset ) const
		{
			r Row = Last();

			if ( Row != NONE )
				Row = Previous( Row, Offset  );

			return Row;
		}
		//f Return true if empty, false otherway.
		bso::bool__ IsEmpty( void ) const
		{
			return Amount() == 0;
		}
		//f Return the entry next to 'Entry', 'NONE' if 'Entry' is the last one.
		r Next(
			r Entry,
			mdr::size__ Offset = 1 ) const
		{
			if ( ( *Entry += Offset ) < Extent_() )
				if ( !Locations.IsAvailable( *Entry ) )
					return Entry;
				else
					return Successeur_( *Entry );
			else
				return NONE;
		}
		//f Return the previous entry of 'Entry', 'NONE' if 'Entry' the first one.
		r Previous(
			r Entry,
			mdr::size__ Offset = 1 ) const
		{
			if ( ( *Entry -= Offset ) > 0 )
				if ( !Locations.IsAvailable( *Entry ) )
					return Entry;
				else
					return Predecesseur_( *Entry );
			else
				return NONE;
		}
		//f Amount of entries, NOT the extent of the list.
		mdr::size__ Amount( void ) const
		{
			return Extent_() - Locations.Amount();
		}
		//f Extent of list.
		mdr::size__ Extent( void ) const
		{
			return Extent_();
		}
		//f Return true if 'Entry' exists, false otherwise.
		bso::bool__ Exists( r Entry ) const
		{
			if ( *Entry >= Extent_() )
				return false;
			else
				return !Locations.IsAvailable( *Entry );
		}
		// Ne peut être appelé que lorsqu'il y a aucune entrée libre.
		void Allocate(
			mdr::size__ Size,
			aem::mode__ Mode = aem::m_Default )
		{
			if ( Locations.Amount() != 0 )
				ERRu();

//			Locations.Init( Size );

			LSTAllocate( Size, Mode );
		}
	};

	E_AUTO2( list )


	#define E_LISTtx( r, r_t )	list<r, r_t>
	#define E_LISTtx_( r, r_t )	list_<r, r_t>

	//d Handle a list of objects.
	#define E_LISTt( r )	E_LISTtx( r, mdr::row_t__ )
	#define E_LISTt_( r )	E_LISTtx_( r, mdr::row_t__ )

	#define E_LIST	E_LISTt( mdr::row__ )
	#define E_LIST_	E_LISTt_( mdr::row__ )

#ifndef FLM__COMPILATION

	class list_file_manager___
	{
	private:
		lst::store_ *_Store;
		tol::E_FPOINTER___( bso::char__ ) _FileName;
		fil::mode__ _Mode;	// Ne sert à rien, juste présent à des fins de standardisation.
		bso::bool__ _Persistent;	// Ne sert à rien, juste présent à des fins de standardisation.
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P ) {
			//	Settle();	// Fait en amoont, car il manque le 'TimeStamp()'.
			}

			_FileName.reset( P );
			_Store = NULL;
			_Mode = fil::m_Undefined;
			_Persistent = false;
		}
		list_file_manager___( void )
		{
			reset( false );
		}
		~list_file_manager___( void )
		{
			reset();
		}
		void Init(
			const char *FileName,
			fil::mode__ Mode,
			bso::bool__ Persistent )
		{
			reset();

			if ( ( _FileName = malloc( strlen( FileName ) + 1 ) ) == NULL )
				ERRa();

			strcpy( _FileName, FileName );

			_Mode = Mode;
			_Persistent = Persistent;
		}
		fil::mode__ Mode( fil::mode__ Mode )
		{
			tol::Swap( Mode, _Mode );

			return Mode;
		}
		fil::mode__ Mode( void ) const
		{
			return _Mode;
		}
		bso::bool__ IsPersistent( void ) const
		{
			return _Persistent;
		}
		uym::state__ State( void ) const
		{
			return Test_( _FileName, 0 );
		}
		uym::state__ Bind( time_t ReferenceTimeStamp )
		{
			uym::state__ State = Test_( FileName(), ReferenceTimeStamp );

			if ( uym::Exists( State ) )
				State = ReadFromFile_( FileName(), *_Store );

			return State;
		}
		uym::state__ Settle( time_t ReferenceTimeStamp )
		{
			uym::state__ State = uym::s_Undefined;

			if ( _Store == NULL )
				return uym::sInconsistent;

			if ( ( ReferenceTimeStamp == 0 )
				|| ( !fil::FileExists( _FileName ) )
				|| ( fil::GetFileLastModificationTime( _FileName ) <= ReferenceTimeStamp ) )
					State = WriteToFile_( *_Store, _FileName );
			else
				State = uym::sExists;

			if ( uym::IsError( State ) )
				return State;
			
			while ( fil::GetFileLastModificationTime( _FileName ) <= ReferenceTimeStamp ) {
				tol::EpochTime( true );	// Permet d'attendre une unité de temps.
				fil::TouchFile( _FileName );
			}

			return State;
		}
		void Drop( void )
		{
			if ( ( _Store == NULL ) || ( _FileName == NULL ) )
				ERRu();

			if ( fil::FileExists( _FileName ) )
				if ( remove( _FileName ) != 0 )
					ERRu();
		}
		const char *FileName( void ) const
		{
			return _FileName;
		}
		void Set( lst::store_ &Store )
		{
			if ( _Store != NULL )
				ERRu();

			_Store = &Store;
		}
		bso::bool__ Exists( void ) const
		{
			return uym::Exists( State() );
		}
#ifdef CPE__VC
#	undef CreateFile
#endif
		bso::bool__ CreateFiles( err::handling__ ErrorHandling = err::h_Default )
		{
			if ( fil::FileExists( _FileName ) )
				if ( ErrorHandling == err::hThrowException )
					ERRf();
				else
					return false;

			return fil::CreateFile( _FileName, ErrorHandling );
		}
		void ReleaseFiles( void )
		{
			// A des fins de standardisation.
		}
		time_t TimeStamp( void ) const
		{
			if ( Exists() )
				return fil::GetFileLastModificationTime( _FileName );
			else
				return 0;
		}
	};


	template <typename list> uym::state__ Plug(
		list &List,
		list_file_manager___ &FileManager,
		mdr::row__ FirstUnused,
		time_t ReferenceTimeStamp )
	{
		uym::state__ State = Test_( FileManager.FileName(), ReferenceTimeStamp );

		if ( uym::IsError( State ) )
			FileManager.reset();
		else {
			FileManager.Set( List.Locations );
			List.Locations.SetFirstUnused( FirstUnused );
		}

		return State;
	}
#endif

	//c Handle a list with a maximum of 't' entries. Use 'LIST__' rather than directly this class.
	template <int t, typename r> class list__
	{
	private:
		// Table de l'occupation de la liste.
		bitbch::bit_bunch__<t, r> Occupation_;
		// Nombre d'éléments dans la liste.
		mdr::size__ Nombre_;
	public:
		list__( void )
		{
			Occupation_.Reset( false );
			Nombre_ = 0;
		}
		//f First entry, 'NONE' if no entries.
		r First( void ) const
		{
			return Occupation_.First( true );
		}
		//f LAsttry, 'NONE' if no entries.
		r Last( void ) const
		{
			return Occupation_.Last( true );
		}
		//f Entry next to 'Entry', none if 'Entry' the last one.
		r Next( r Entry ) const
		{
			return Occupation_.Next( Entry, true );
		}
		//f Previous entry from 'Entry', none if 'Entry' the first one.
		r Previous( r Entry ) const
		{
			return Occupation_.Previous( Entry, true );
		}
		//f Return the position of a new entry.
		r CreateEntry( err::handling__ ErrorHandling = err::h_Default  )
		{
			mdr::row_t__ Position = NONE;

			if ( Nombre_ == t ) 
			{
				if ( ErrorHandling == err::hThrowException )
					ERRu();
			}
			else
			{
				Position = Occupation_.First( false );

				Occupation_.Write( true, Position );

				Nombre_++;
			}

			return Position;
		}
		//f Delete 'Entry', which MUST exists.
		void Delete( r Entry )
		{
			Occupation_.Write( false, Entry );

			Nombre_--;
		}
		//f Return true if 'Entry' exists, fals otherwise.
		bso::bool__ Exists( r Entry ) const
		{
			return Occupation_.Read( Entry );
		}
		//f Return true if no entry, false otherwise.
		bso::bool__ IsEmpty( void ) const
		{
			return !Nombre_;
		}
		//f Mark the list as full.
		void Fill( void )
		{
			Occupation_.Reset( true );
			Nombre_ = t;
		}
		//f Mark the list as empty.
		void Empty( void )
		{
			Occupation_.Reset( false );
			Nombre_ = 0;
		}
		//f Return true if totally full, false otherwise.
		bso::bool__ IsFull( void )
		{
			return Nombre_ >= t;
		}

		//m Handle a list with a maximum of 'n' entries.
		#define E_LIST__( n )		list__< n >
	};

}



/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
