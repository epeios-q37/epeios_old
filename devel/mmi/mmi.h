/*
	Header for the 'mmi' library by Claude SIMON (csimon@epeios.org)
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

#ifndef MMI__INC
#define MMI__INC

#define MMI_NAME		"MMI"

#define	MMI_VERSION	"$Revision$"

#define MMI_OWNER		"Claude SIMON (csimon@epeios.org)"

#include "ttr.h"

extern class ttr_tutor &MMITutor;

#if defined( XXX_DBG ) && !defined( MMI_NODBG )
#define MMI_DBG
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
//D MultiMemory Index 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "mmm.h"
#include "tym.h"
#include "mmisub.h"
#include "bch.h"

namespace mmi {

	struct descripteur__
	{
		mmm::descriptor__ Descripteur;
		epeios::size__ Capacite;
	};

	typedef bch::E_BUNCHt_( descripteur__, index__ ) descriptors_;

	//c An indexed multimemory.
	class indexed_multimemory_
	{
	private:
		void Initialize_(
			epeios::size__ CapaciteCourante,
			epeios::size__ NouvelleCapacite )
		{
			descripteur__ D;

			D.Descripteur = 0;
			D.Capacite = 0;

			Descripteurs.Store( D, CapaciteCourante, NouvelleCapacite - CapaciteCourante );
		}
	// fonctions
		void AllouerPlus_(
			epeios::size__ CapaciteCourante,
			epeios::size__ NouvelleCapacite )
		{
			Descripteurs.Allocate( NouvelleCapacite );

			Initialize_( CapaciteCourante, NouvelleCapacite );
		}
		// alloue plus de la place pour pouvoir contenir 'NouvelleCapacite' objets,
		// sachant que 'Capacite courante' est la capacite actuelle
		void AllouerMoins_(
			epeios::size__ CapaciteCourante,
			epeios::size__ NouvelleCapacite );
		// alloue plus de la place pour pouvoir contenir 'NouvelleCapacite' objets,
		// sachant que 'Capacite courante' est la capacite actuelle
		void Lire_(
			index__ Index,
			epeios::row_t__ Position,
			epeios::size__ Taille,
			epeios::datum__ *Tampon ) const
		{
			Multimemoire.Read( Descripteurs(*Index).Descripteur, Position, Taille, Tampon );
		}
		// place dans 'Tampon' 'Taille' octets, à partir de 'Position', de l'objet 'Index'
		void Ecrire_(
			const epeios::datum__ *Tampon,
			epeios::size__ Taille,
			index__ Index,
			epeios::row_t__ Position )
		{
			Multimemoire.Write( Tampon, Taille, Descripteurs(*Index).Descripteur, Position );
		}
		// écrit à 'Position' de l'objet 'Index' 'Taille' octets de 'Tampon'
		void Allouer_(
			index__ Index,
			epeios::size__ Nombre )
		{
			descripteur__ D = Descripteurs.Get( *Index );

			D.Descripteur = Multimemoire.Reallocate( D.Descripteur, Nombre );

			D.Capacite = Nombre;

			Descripteurs.Store( D, *Index );
		}
		// allocation pour 'Capacite' objets
		void Liberer_( index__ Index )
		{
			descripteur__ D = Descripteurs.Get( *Index );

			Multimemoire.Free( D.Descripteur );

			D.Descripteur = 0;
			D.Capacite = 0;

			Descripteurs.Store( D, *Index );
		}
		// libère la mémoire d'index 'Index'
		void Push_(
			epeios::row__ Position,
			epeios::size__ Amount,
			epeios::size__ Size )	// Assuming allocation already done !
		{
			Descripteurs.Store( Descripteurs, Size - *Position - Amount, *Position, *Position + Amount );
		}
	public:
		descriptors_ Descripteurs;
		// les différents descripteurs
		mmm::multimemory_ Multimemoire;
		// la memoire générale
		struct s
		{
			descriptors_::s Descripteurs;
			mmm::multimemory_::s Multimemoire;
		};
		indexed_multimemory_( s &S )
		: Descripteurs( S.Descripteurs ),
		  Multimemoire( S.Multimemoire )
		{}
		// préparation
		void reset( bool P = true )
		{
			Descripteurs.reset( P );
			Multimemoire.reset( P );
		}
		void plug( mmm::multimemory_ &M )
		{
			Descripteurs.plug( M );
			this->Multimemoire.plug( M );
		}
		indexed_multimemory_ &operator =( const indexed_multimemory_ &O )
		{
			ERRu();

			return *this;
		}
		void Copy(
			const indexed_multimemory_ &O,
			epeios::size__ Size )
		{
			Descripteurs.Allocate( Size );
			Descripteurs.Store( O.Descripteurs, Size );
			Multimemoire = O.Multimemoire;
		}
	/*	void ecrire( flo_sortie_ &F ) const
		{
			Descripteurs.ecrire( F );
			Multimemoire.ecrire( F );
		}
		void lire( flo_entree_ &F )
		{
			Descripteurs.lire( F );
			Multimemoire.lire( F );
		}*/
		//f Initialization.
		void Init( void )
		{
			Descripteurs.Init();
			Multimemoire.Init();
		}
		//f Allocate 'Capacity' memories in the indexed multimemory. 'ActualCapacity' is the actual capacity.
		void Allocate( epeios::size__ Amount )
		{
			epeios::size__ CurrentAmount = Descripteurs.Amount();

			if ( CurrentAmount > Amount )
				AllouerMoins_( CurrentAmount, Amount );
			else
				AllouerPlus_( CurrentAmount, Amount );
		}
		index__ New( index__ Index = NONE )
		{
			if ( Index == NONE )
				Index = Descripteurs.New();
			else if ( !Descripteurs.Exists( Index ) )
				ERRu();

			return Index;
		}
		//f Put 'Amount' bytes in 'Buffer' from the 'Index' memory at 'Position' .
		void Read(
			index__ Index,
			epeios::row_t__ Position,
			epeios::size__ Amount,
			epeios::datum__ *Buffer ) const
		{
			Lire_( Index, Position, Amount, Buffer );
		}
		//f Put 'Amount' bytes at 'Position' to the 'Index' memory from 'Buffer'.
		void Write(
			const epeios::datum__ *Buffer,
			epeios::size__ Amount,
			index__ Index,
			epeios::row_t__ Position )
		{
			Ecrire_( Buffer, Amount, Index, Position );
		}
		//f Allocate 'Size' bytes in the 'Index' memory.
		void Allocate(
			index__ Index,
			epeios::size__ Size )
		{
			Allouer_( Index, Size );
		}
		//f Discard the 'Index' memory.
		void Free( index__ Index )
		{
			Liberer_( Index );
		}
		//f Flushes the memory.
		void Flush( void )
		{
			Descripteurs.Flush();
			Multimemoire.Flush();
		}
		//f Return the size of the 'Index' memory.
		epeios::size__ Size( index__ Index ) const
		{
			return Descripteurs( *Index ).Capacite;
		}
		/*f Delete 'Amount' entries from 'Position',
		'ActualCapacity' is the actual capacity.
		No reallocationg to gain place is made. */
		void RemoveWithoutReallocating(
			epeios::row__ Position,
			epeios::size__ ActualCapacity,
			epeios::size__ Amount );
#if 0
		void write(
			flw::oflow__ &OFlow,
			epeios::size__ Size) const
		{
			Descripteurs.write( 0, Size, OFlow );
			Multimemoire.write( OFlow );
		}
		void read(
			flw::iflow__ &IFlow,
			epeios::size__ Size )
		{
			Descripteurs.read( IFlow, 0, Size );
			Multimemoire.read( IFlow );
		}
#endif
	};

	E_AUTO( indexed_multimemory )

	class indexed_multimemory_file_manager___ {
	private:
		bch::bunch_file_manager___ _Descriptors;
		mmm::multimemory_file_manager___ _Multimemory;
	public:
		void reset( bso::bool__ P = true )
		{
			_Descriptors.reset( P );
			_Multimemory.reset( P );
		}
		indexed_multimemory_file_manager___( void )
		{
			reset( false );
		}
		~indexed_multimemory_file_manager___( void )
		{
			reset();
		}
		void Init( 
			const char *DescriptorsFileName,
			const char *MultimemoryFileName,
			mdr::mode__ Mode,
			bso::bool__ Persistent )
		{
			_Descriptors.Init( DescriptorsFileName, Mode, Persistent );
			_Multimemory.Init( MultimemoryFileName, Mode, Persistent );
		}
		void ReleaseFile( void )
		{
			_Descriptors.ReleaseFile();
			_Multimemory.ReleaseFile();
		}
		void Mode( mdr::mode__ Mode )
		{
			_Descriptors.Mode( Mode );
			_Multimemory.Mode( Mode );
		}
		bso::bool__ IsPersistent( void ) const
		{
#ifdef MMI_DBG
			if ( _Descriptors.IsPersistent() != _Multimemory.IsPersistent() )
				ERRc();
#endif
			return _Descriptors.IsPersistent();
		}
		void Drop( void )
		{
			_Descriptors.Drop();
			_Multimemory.Drop();
		}
		tym::memory_file_manager___ &DescriptorsFileManager( void )
		{
			return _Descriptors;
		}
		mmm::multimemory_file_manager___ &MultimemoryFileManager( void )
		{
			return _Multimemory;
		}
		bso::bool__ Exists( void ) const
		{
			bso::bool__ Exists = _Descriptors.Exists();

			if ( Exists != _Multimemory.Exists() )
				ERRc();

			return Exists;
		}
		time_t TimeStamp( void ) const
		{
			time_t DescriptorsTimeStamp, MultimemoryTimeStamp;

			DescriptorsTimeStamp = _Descriptors.TimeStamp();
			MultimemoryTimeStamp = _Multimemory.TimeStamp();

			return ( DescriptorsTimeStamp > MultimemoryTimeStamp ? DescriptorsTimeStamp : MultimemoryTimeStamp );
		}
	};

	inline bso::bool__ Connect(
		indexed_multimemory_ &Memory,
		indexed_multimemory_file_manager___ &FileManager )
	{
		bso::bool__ Exists = bch::Connect( Memory.Descripteurs, FileManager.DescriptorsFileManager() );

		if ( mmm::Connect( Memory.Multimemoire, FileManager.MultimemoryFileManager() ) != Exists )
			ERRc();

		return Exists;
	}



//	using mdr::E_MEMORY_DRIVER__;

	inline void _base_indexed_multimemory_driver__::MDRRecall(
		mdr::row_t__ Position,
		mdr::size__ Amount,
		mdr::datum__ *Buffer )
	{
		Multimemoire_->Read( _Index, Position, Amount, Buffer );
	}

	inline epeios::size__ _base_indexed_multimemory_driver__::Size( void ) const
	{
		return Multimemoire_->Size( _Index );
	}

	inline void indexed_multimemory_driver__::MDRStore(
		const epeios::datum__ *Buffer,
		mdr::size__ Amount,
		mdr::row_t__ Position )
	{
		Multimemoire_->Write( Buffer, Amount, _Index, Position );
	}

	inline void indexed_multimemory_driver__::MDRAllocate( mdr::size__ Capacity )
	{
		Multimemoire_->Allocate( _Index, Capacity );
	}

	inline void indexed_multimemory_driver__::MDRFlush( void )
	{
		if ( Multimemoire_ )
			Multimemoire_->Flush();
	}
}



/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
