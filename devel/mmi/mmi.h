/*
  Header for the 'mmi' library by Claude L. Simon (epeios@epeios.org)
  Copyright (C) 2000 Claude L. SIMON (epeios@epeios.org) 

  This file is part of the Epeios (http://www.epeios.org/) project.
  

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.
 
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, go to http://www.fsf.org or write to the
  
                        Free Software Foundation, Inc.,
           59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

//	$Id$

#ifndef MMI__INC
#define MMI__INC

#define MMI_NAME		"MMI"

#define	MMI_VERSION	"$Revision$"	

#define MMI_OWNER		"the Epeios project (http://www.epeios.org/)"

#include "ttr.h"

extern class ttr_tutor &MMITutor;

#if defined( XXX_DBG ) && !defined( MMI_NODBG )
#define MMI_DBG 
#endif

/* Begin of automatic documentation generation part. */

//V $Revision$
//C Claude L. SIMON (epeios@epeios.org)
//R $Date$

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "mmm.h"
#include "set.h"


//t The type of an index in the indexed multimemory.
typedef POSITION__	mmi__index;

struct mmi__descripteur
{
	mmm__descriptor Descripteur;
	uym__size Capacite;
};


//c An indexed multimemory.
class mmi_indexed_multimemory_
{
private:
// fonctions
	void AllouerPlus_(
		uym__size CapaciteCourante,
		uym__size NouvelleCapacite );
	// alloue plus de la place pour pouvoir contenir 'NouvelleCapacite' objets,
	// sachant que 'Capacite courante' est la capacite actuelle
	void AllouerMoins_(
		uym__size CapaciteCourante,
		uym__size NouvelleCapacite );
	// alloue plus de la place pour pouvoir contenir 'NouvelleCapacite' objets,
	// sachant que 'Capacite courante' est la capacite actuelle
	void Lire_(
		mmi__index Index,
		uym__position Position,
		uym__bsize Taille,
		uym__data *Tampon ) const
	{
		Multimemoire.Read( Descripteurs(Index).Descripteur, Position, Taille, Tampon );
	}
	// place dans 'Tampon' 'Taille' octets, à partir de 'Position', de l'objet 'Index'
	void Ecrire_(
		const uym__data *Tampon,
		uym__bsize Taille,
		mmi__index Index,
		uym__position Position )
	{
		Multimemoire.Write( Tampon, Taille, Descripteurs(Index).Descripteur, Position );
	}
	// écrit à 'Position' de l'objet 'Index' 'Taille' octets de 'Tampon'
	void Allouer_(
		mmi__index Index,
		uym__size Nombre )
	{
		mmi__descripteur D = Descripteurs.Read( Index );

		D.Descripteur = Multimemoire.Reallocate( D.Descripteur, Nombre );

		D.Capacite = Nombre;

		Descripteurs.Write( D, Index );
	}
	// Alloue 'Nombre' octets pour l'objet d'index 'Index'
	void Allouer_( uym__size Capacite )
	{
		if ( this->Extent() > Capacite )
			AllouerMoins_( this->Extent(), Capacite );
		else
			AllouerPlus_( this->Extent(), Capacite );
	}
	// allocation pour 'Capacite' objets
	void Liberer_( mmi__index Index )
	{
		mmi__descripteur D = Descripteurs.Read( Index );

		Multimemoire.Free( D.Descripteur );

		D.Descripteur = 0;
		D.Capacite = 0;

		Descripteurs.Write( D, Index );
	}
	// libère la mémoire d'index 'Index'
public:
	SET_( mmi__descripteur ) Descripteurs;
	// les différents descripteurs
	mmm_multimemory_ Multimemoire;
	// la memoire générale
	struct s
	{
		SET_( mmi__descripteur )::s Descripteurs;
		mmm_multimemory_::s Multimemoire;
	};
	mmi_indexed_multimemory_( s &S )
	: Descripteurs( S.Descripteurs ),
	  Multimemoire( S.Multimemoire )
	{}
	// préparation
	void reset( bool P = true )
	{
		if ( P )
		{
			if ( Extent() )
				AllouerMoins_( Extent(), 0 );
		}

		Descripteurs.reset( P );
		Multimemoire.reset( P );
	}
	void plug( mmm_multimemory_ &M )
	{
		Descripteurs.plug( M );
		this->Multimemoire.plug( M );
	}
	mmi_indexed_multimemory_ &operator =( const mmi_indexed_multimemory_ &O)
	{
		Descripteurs = O.Descripteurs;
		Multimemoire = O.Multimemoire;

		return *this;
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
	//f Allocate 'Amount' memories in the indexed multimemory.
	void Allocate( tym__size Amount )
	{
		Allouer_( Amount );
	}
	//f Put 'Amount' bytes in 'Buffer' from the 'Index' memory at 'Position' .
	void Read(
		mmi__index Index,
		uym__position Position,
		uym__bsize Amount,
		uym__data *Buffer ) const
	{
		Lire_( Index, Position, Amount, Buffer );
	}
	//f Put 'Amount' bytes at 'Position' to the 'Index' memory from 'Buffer'.
	void Write(
		const uym__data *Buffer,
		uym__bsize Amount,
		mmi__index Index,
		uym__position Position )
	{
		Ecrire_( Buffer, Amount, Index, Position );
	}
	//f Allocate 'Size' bytes in the 'Index' memory.
	void Allocate(
		mmi__index Index,
		uym__size Size )
	{
		Allouer_( Index, Size );
	}
	//f Discard the 'Index' memory.
	void Free( mmi__index Index )
	{
		Liberer_( Index );
	}
	//f Flushes the memory.
	void Synchronize( void )
	{
		Descripteurs.Synchronize();
		Multimemoire.Synchronize();
	}
	//f Return the extent of the indexed memory.
	uym__size Extent( void ) const
	{
		return Descripteurs.Extent();
	}
	//f Return the extent of the 'Index' memory.
	uym__size Extent( mmi__index Index ) const
	{
		return Descripteurs( Index ).Capacite;
	}
	void write( flw::oflow___ &OFlow ) const
	{
		Descripteurs.write( OFlow );
		Multimemoire.write( OFlow );
	}
	void read( flw::iflow___ &IFlow )
	{
		Descripteurs.read( IFlow );
		Multimemoire.read( IFlow );
	}
};

AUTO( mmi_indexed_multimemory )

//c This class is the standard memory driver for the indexed multimemory.
class mmi_indexed_multimemory_driver_
: public MEMORY_DRIVER_
{
private:
	mmi_indexed_multimemory_ *Multimemoire_;
	// memoire à laquelle il a été affecté
protected:
	virtual void MDRRead(
		mdr__position Position,
		mdr__bsize Amount,
		mdr__data *Buffer )
	{
		Multimemoire_->Read( S_.Index, Position, Amount, Buffer );
	}
	// lit à partir de 'Position' et place dans 'Tampon' 'Nombre' octets;
	virtual void MDRWrite(
		const uym__data *Buffer,
		mdr__bsize Amount,
		mdr__position Position )
	{
		Multimemoire_->Write( Buffer, Amount, S_.Index, Position );
	}
	// écrit 'Nombre' octets à la position 'Position'
	virtual void MDRAllocate( mdr__size Capacity )
	{
		Multimemoire_->Allocate( S_.Index, Capacity );
	}
	// alloue 'Capacite' octets
	virtual void MDRSynchronize( void )
	{
		if ( Multimemoire_ )
			Multimemoire_->Synchronize();
	}
public:
	struct s
	: public MEMORY_DRIVER_::s
	{
		mmi__index Index;
	} &S_;
	mmi_indexed_multimemory_driver_( s &S )
	: S_( S ),
	  MEMORY_DRIVER_( S ) {}
	void reset( bool P = true )
	{
		MEMORY_DRIVER_::reset( P );

		S_.Index = NONE;
		Multimemoire_ = NULL;
	}
	//f Initialize with the 'Multimemory' multimemory.
	void Init( mmi_indexed_multimemory_ &Multimemory )
	{
		MEMORY_DRIVER_::Init();

		S_.Index = NONE;
		Multimemoire_ = &Multimemory;
	}
	//f The 'Index' memory becomes the memory handled by this driver.
	void Index( mmi__index Index )
	{
		S_.Index = Index;
	}
	//f Return the index of the current memory.
	mmi__index Index( void )
	{
		return S_.Index;
	}
	//f Return the extent of the memory.
	uym__size Extent( void )
	{
		return Multimemoire_->Extent( S_.Index );
	}
};

//c Same as 'mmmi_indexed_multimemory_driver_', but for read-only memory.
class mmi_const_indexed_multimemory_driver_
: public MEMORY_DRIVER_
{
private:
	const mmi_indexed_multimemory_ *Multimemoire_;
	// memoire à laquelle il a été affecté
protected:
	virtual void MDRRead(
		mdr__position Position,
		mdr__bsize Amount,
		mdr__data *Buffer )
	{
		Multimemoire_->Read( S_.Index, Position, Amount, Buffer );
	}
	// lit à partir de 'Position' et place dans 'Tampon' 'Nombre' octets;
	virtual void MDRWrite(
		const mdr__data *Buffer,
		mdr__bsize Amount,
		mdr__position Position )
	{
		ERRu();
	}
	// écrit 'Nombre' octets à la position 'Position'
	virtual void MDRAllocate( mdr__size Capacity )
	{
		ERRu();
	}
	// alloue 'Capacite' octets
	virtual void MDRSynchronize( void )
	{
		ERRu();
	}
public:
	struct s
	: public MEMORY_DRIVER_::s
	{
		mmi__index Index;
	} &S_;
	mmi_const_indexed_multimemory_driver_( s &S )
	: S_( S ),
	  MEMORY_DRIVER_( S ) {}
	void reset( bool P = true )
	{
		MEMORY_DRIVER_::reset( P );

		S_.Index = NONE;
		Multimemoire_ = NULL;
	}
	//f Initialize with 'Multimemory' multimemory.
	void Init( const mmi_indexed_multimemory_ &Multimemory )
	{
		MEMORY_DRIVER_::Init();

		S_.Index = NONE;
		Multimemoire_ = &Multimemory;
	}
	//f The 'Index' memory becomes the memory handled by this memory driver.
	void Index( mmi__index Index )
	{
		S_.Index = Index;
	}
	//f Return the index of the current memory.
	mmi__index Index( void )
	{
		return S_.Index;
	}
	//f Return the extent of the memory.
	uym__size Extent( void )
	{
		return Multimemoire_->Extent( S_.Index );
	}
};





/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
