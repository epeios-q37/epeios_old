/*
	Header for the 'lst' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Copyright (C) $COPYRIGHT_DATES$Claude SIMON (http://zeusw.org/intl/contact.html).
$_RAW_$
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

	typedef ids::E_IDS_STORE_( sdr::row__ ) store_;

	// Retourne l'id de la premi�re entr�e disponible (hors pile des 'released').
	uys::state__ WriteToFile_(
		const store_ &Store,
		const char *FileName );

	uys::state__ ReadFromFile_(
		const char *FileName,
		store_ &Store );

	inline uys::state__ Test_(
		const char *FileName,
		time_t ReferenceTimeStamp )
	{
		if ( FileName == NULL )
			return uys::sInconsistent;

		if ( !fil::FileExists( FileName ) )
			return uys::sAbsent;

		if ( fil::GetFileLastModificationTime( FileName ) <= ReferenceTimeStamp )
			return uys::sInconsistent;

		return uys::sExists;
	}

	sdr::row_t__ Successeur_(
		sdr::row_t__ Element,
		sdr::size__ Amount,
		const store_ &Libres );

	sdr::row_t__ Predecesseur_(
		sdr::row_t__ Element,
		const store_ &Libres );

	void Insert_(
		sdr::row_t__ First,
		sdr::row_t__ Last,
		store_ &Store );

	void MarkAsReleased_(
		sdr::row_t__ First,
		sdr::row_t__ Last,
		store_ &Store );


	//c Handle a list of objects. Use 'LIST_' rather than directly this class.
	template <typename r, typename r_t> class list_
	{
	protected:
		/*v Cette fonction est appel�e lors d'allocations dans la liste;
		permet de synchroniser la taille de la liste avec d'autres ensembles;
		'Size' est la capacit� allou�e. */
		virtual void LSTAllocate(
			sdr::size__ Size,
			aem::mode__ Mode ) = 0;
	private:
		// Return the extent, based on 'Locations'.
		sdr::row_t__ Extent_( void ) const
		{
			return *Locations.GetFirstAvailable();
		}
		sdr::row_t__ Nouveau_( aem::mode__ Mode )
		{
			bso::bool__ Released = false;

			sdr::row_t__ New = *Locations.New( Released );

			if ( !Released )
				LSTAllocate( Extent_(), Mode );

			return New;
		}
		// Retourne l'�l�ment succ�dant � 'Element', ou LST_INEXISTANT si inexistant.
		sdr::row_t__ Successeur_( sdr::row_t__ Element ) const
		{
			return lst::Successeur_( Element, Extent_(), Locations );
		}
		// Retourne l'�l�ment pr�c�dent 'Element', ou LST_INEXISTANT si inexistant.
		sdr::row_t__ Predecesseur_( sdr::row_t__ Element ) const
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
		void plug( sdr::E_SDRIVER__ &SD )
		{
			Locations.plug( SD );
		}
		void plug( ags::E_ASTORAGE_ &AS )
		{
			Locations.plug( AS );
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
		r First( sdr::size__ Offset ) const
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
				sdr::row_t__ P = Extent_() - 1;

				if ( !Locations.IsAvailable( P ) )
					return P;
				else
					return Predecesseur_( P );
			}
			else
				return NONE;
		}
		r Last( sdr::size__ Offset ) const
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
			sdr::size__ Offset = 1 ) const
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
			sdr::size__ Offset = 1 ) const
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
		sdr::size__ Amount( void ) const
		{
			return Extent_() - Locations.Amount();
		}
		//f Extent of list.
		sdr::size__ Extent( void ) const
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
		// Ne peut �tre appel� que lorsqu'il y a aucune entr�e libre.
		void Allocate(
			sdr::size__ Size,
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
	#define E_LISTt( r )	E_LISTtx( r, sdr::row_t__ )
	#define E_LISTt_( r )	E_LISTtx_( r, sdr::row_t__ )

	#define E_LIST	E_LISTt( sdr::row__ )
	#define E_LIST_	E_LISTt_( sdr::row__ )

#ifndef FLM__COMPILATION

	class list_file_manager___
	{
	private:
		lst::store_ *_Store;
		tol::E_FPOINTER___( bso::char__ ) _FileName;
		fil::mode__ _Mode;	// Ne sert � rien, juste pr�sent � des fins de standardisation.
		bso::bool__ _Persistent;	// Ne sert � rien, juste pr�sent � des fins de standardisation.
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
				ERRAlc();

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
		uys::state__ State( void ) const
		{
			return Test_( _FileName, 0 );
		}
		uys::state__ Bind( time_t ReferenceTimeStamp )
		{
			uys::state__ State = Test_( FileName(), ReferenceTimeStamp );

			if ( uys::Exists( State ) )
				State = ReadFromFile_( FileName(), *_Store );

			return State;
		}
		uys::state__ Settle( time_t ReferenceTimeStamp )
		{
			uys::state__ State = uys::s_Undefined;

			if ( _Store == NULL )
				return uys::sInconsistent;

			if ( ( ReferenceTimeStamp == 0 )
				|| ( !fil::FileExists( _FileName ) )
				|| ( fil::GetFileLastModificationTime( _FileName ) <= ReferenceTimeStamp ) )
					State = WriteToFile_( *_Store, _FileName );
			else
				State = uys::sExists;

			if ( uys::IsError( State ) )
				return State;
			
			while ( fil::GetFileLastModificationTime( _FileName ) <= ReferenceTimeStamp ) {
				tol::EpochTime( true );	// Permet d'attendre une unit� de temps.
				fil::TouchFile( _FileName );
			}

			return State;
		}
		void Drop( void )
		{
			if ( ( _Store == NULL ) || ( _FileName == NULL ) )
				ERRCcp();

			if ( fil::FileExists( _FileName ) )
				if ( remove( _FileName ) != 0 )
					ERRDvc();
		}
		const char *FileName( void ) const
		{
			return _FileName;
		}
		void Set( lst::store_ &Store )
		{
			if ( _Store != NULL )
				ERRCcp();

			_Store = &Store;
		}
		bso::bool__ Exists( void ) const
		{
			return uys::Exists( State() );
		}
#ifdef CPE__VC
#	undef CreateFile
#endif
		bso::bool__ CreateFiles( err::handling__ ErrorHandling = err::h_Default )
		{
			if ( fil::FileExists( _FileName ) )
				if ( ErrorHandling == err::hThrowException )
					ERRCcp();
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


	template <typename list> uys::state__ Plug(
		list &List,
		list_file_manager___ &FileManager,
		sdr::row__ FirstUnused,
		time_t ReferenceTimeStamp )
	{
		uys::state__ State = Test_( FileManager.FileName(), ReferenceTimeStamp );

		if ( uys::IsError( State ) )
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
		// Nombre d'�l�ments dans la liste.
		sdr::size__ Nombre_;
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
			sdr::row_t__ Position = NONE;

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
