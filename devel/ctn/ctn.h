/*
  Header for the 'ctn' library by Claude L. Simon (simon@epeios.org)
  Copyright (C) 2000,2001 Claude L. SIMON (simon@epeios.org) 

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
  along with this program; if not, go to http://www.fsf.org/
  or write to the:
  
                        Free Software Foundation, Inc.,
           59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

//	$Id$

#ifndef CTN__INC
#define CTN__INC

#define CTN_NAME		"CTN"

#define	CTN_VERSION	"$Revision$"	

#define CTN_OWNER		"the Epeios project (http://www.epeios.org/)"

#include "ttr.h"

extern class ttr_tutor &CTNTutor;

#if defined( XXX_DBG ) && !defined( CTN_NODBG )
#define CTN_DBG 
#endif

/* Begin of automatic documentation generation part. */

//V $Revision$
//C Claude L. SIMON (simon@epeios.org)
//R $Date$

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/
//D ConTaiNer. Containers of dynamic objects.


#include "err.h"
#include "flw.h"
#include "mmi.h"
#include "cpe.h"

//t The size of a container.
typedef tym__size	ctn__size;

#ifdef CPE__VC
// VC++ had some difficulties with 'typename'.
#define typename
#endif

/* Remplit les parties statiques des objets 'Debut' à 'Fin' inclus
contenus dans 'Memoire', sachant qu'ils doivent contenir 'Statique'
qui est de taille 'Taille' */
void CTNRemplir_(
	uym_basic_memory_ &Memoire,
	uym__position Debut,
	uym__position Fin,
	uym__data *Statique,
	uym__bsize Taille );


//c The base of a container. Internal use.
template <class st> class ctn_basic_container_
{
public:
	//r All the static parts.
	set::SET_( st ) Statics;
	//r All the dynamic parts.
	mmi::indexed_multimemory_ Dynamics;
	struct s
	{
		set::SET_( st )::s Statics;
		mmi::indexed_multimemory_::s Dynamics;
	};
	ctn_basic_container_( s &S )
	: Dynamics( S.Dynamics ),
	  Statics( S.Statics )
	{}
	void reset( bool P = true )
	{
		Dynamics.reset( P );
		Statics.reset( P );
	}
	void plug( mmm_multimemory_ &M )
	{
		Dynamics.plug( M );
		Statics.plug( M );
	}
	ctn_basic_container_ &operator =( const ctn_basic_container_ &O )
	{
		Dynamics = O.Dynamics;
		Statics = O.Statics;

		return *this;
	}
	void write( flw::oflow___ &OFlow ) const
	{
		Dynamics.write( OFlow );
		Statics.write( OFlow );
	}
	void read( flw::iflow___ &IFlow )
	{
		Dynamics.read( IFlow );
		Statics.read( IFlow );
	}
	//f Initialization.
	void Init( void )
	{
		Dynamics.Init();
		Statics.Init();
	}
	//f Allocation room for 'Size' object of statical part 'ST'.
	void Allocate(
		uym__size Size,
		st &ST )
	{
		SIZE__ AncCap;

		Dynamics.Allocate( Size );

		AncCap = Statics.Amount();

		Statics.Allocate( Size );

		if ( AncCap < Size )
		{
			if ( ( Size - AncCap ) > 1 )
				CTNRemplir_( this->Statics, AncCap, Size - 1, (uym__data *)&ST, sizeof( ST ) );
			else
				Statics.Write( ST, AncCap );
		}
	}
#if 0
	//f Extent of the container.
	ctn__size Extent( void ) const
	{
		return Statics.Extent();
	}
#endif
	// Comme 'ecrire()', sauf pour la multimémoire, qui contient la partie dynamique.
/*	void EcrireToutDansFlotSaufPartiesDynamiques( flo_sortie_ &Flot ) const
	{
		FLOEcrire( *Multimemoire.S_, Flot );
		Dynamique.ecrire( Flot );
		Statique.ecrire( Flot );
	}
	// Comme 'lire()', sauf pour la multimémoire, qui contient la partie dynamique.
	void LireToutDeFlotSaufPartiesDynamiques( flo_entree_ &Flot )
	{
		FLOLire( Flot, *Multimemoire.S_ );
		Dynamique.lire( Flot );
		Statique.lire( Flot );
	}
*/
	FNLPA( Statics. )
};


//c The base of a volatil item. Internal use.
template <class st> class ctn__item_base_volatile
// La fonction 'Init()' est héritée de t
{
private:
	bool Vide_( void )
	{
		return Pilote_.Index() == NONE;
	}
	void Vider_( void )
	{
		if ( ( !Vide_() && ( Mode_ == mdr::mReadWrite ) ) /*&& ( Conteneur_->Mode() == plm::mModification )*/ ) {
#ifdef CTN_DBG
			if ( Conteneur_ == NULL )
				ERRu();
#endif
			Conteneur_->Statics.Write( ctn_S_, Pilote_.Index() );
		}

		Pilote_.Index( NONE );
	}
protected:
	// Conteneur auquel est rattaché l'élément.
//	ctn_conteneur_base_ < ctn_item_mono < t, st > :: s  > *Conteneur_;
	ctn_basic_container_<st> *Conteneur_;
	/* Pilote permettant l'accés à la partie dynamique des objets contenus
	dans le conteneur auquel cet élément est rattaché. */
	mmi::indexed_multimemory_driver_ Pilote_;
	// Mode d'accés
	mdr::mode Mode_;
public:
	struct s
	: public st
	{
		mmi::indexed_multimemory_driver_::s Pilote_;
	} ctn_S_; //pour ne pas risquer d'ambigüité.
	/* Retourne l'index dans le conteneur de l'élément courant. N'a de sens que
	si 'EstVide()' est à faux. */
	ctn__item_base_volatile( void )
	: Pilote_( ctn_S_.Pilote_ )
	{
		Pilote_.reset( false );

		Conteneur_ = NULL;
		Mode_ = mdr::mReadOnly;
	}
	~ctn__item_base_volatile( void )
	{
		Sync();

		Pilote_.reset( true );
	}
/*	POSITION__ ctn_Position( void )
	{
		return Pilote_.Index();
	}
*/	/* Retourne faux si l'élément est calé sur un élément du conteneur,
	vrai sinon */
	bso__bool EstVide( void )
	{
		return Vide_();
	}
	// Rattache au conteneur 'Conteneur'.
	void Init(
		ctn_basic_container_<st> &Conteneur,
		mdr::mode Mode = mdr::mReadWrite )
	{
		Init( &Conteneur, Mode );
	}
	// Rattache au conteneur 'Conteneur'.
	void Init(
		ctn_basic_container_<st> *Conteneur,
		mdr::mode Mode = mdr::mReadWrite )
	{
		Conteneur_ = Conteneur;
		Pilote_.Init( Conteneur->Dynamics );
		Mode_ = Mode;
	}
	//* Cale l'élément sur l'élément du conteneur à la position 'Position'
	void Sync( POSITION__ Position )
	{
		if ( Pilote_.Index() != Position )
		{
			Vider_();
			Conteneur_->Statics.Read( Position, ctn_S_ );
			Pilote_.Index( Position );
		}
	}
	// Synchronise avec l'élément du conteneur (leur contenu devient identique).
	void Sync( void )
	{
		Vider_();
	}
	// Bascule en mde 'Mode'.
	void ChangeMode( mdr::mode Mode )
	{
		if ( Mode_ != Mode )
		{
			Vider_();
			Mode_ = Mode;
		}
	}
};

template <class st> class ctn__item_base_const
// La fonction 'Init()' est héritée de t
{
private:
	bool Vide_( void )
	{
		return Pilote_.Index() == NONE;
	}
	void Vider_( void )
	{
		Pilote_.Index( NONE );
	}
protected:
	// Conteneur auquel est rattaché l'élément.
//	ctn_conteneur_base_ < ctn_item_mono < t, st > :: s  > *Conteneur_;
	const ctn_basic_container_<st> *Conteneur_;
	/* Pilote permettant l'accés à la partie dynamique des objets contenus
	dans le conteneur auquel cet élément est rattaché. */
	mmi::const_indexed_multimemory_driver_ Pilote_;
public:
	struct s
	: public st
	{
		mmi::const_indexed_multimemory_driver_::s Pilote_;
	} ctn_S_; //pour ne pas risquer d'ambigüité.
	/* Retourne l'index dans le conteneur de l'élément courant. N'a de sens que
	si 'EstVide()' est à faux. */
	ctn__item_base_const( void )
	: Pilote_( ctn_S_.Pilote_ )
	{
		Pilote_.reset( false );

		Conteneur_ = NULL;
	}
	~ctn__item_base_const( void )
	{
		Vider();

		Pilote_.reset( true );
	}
/*	mbs__position Position( void )
	{
		return Pilote_.Index();
	}
*/	/* Retourne faux si l'élément est calé sur un élément du conteneur,
	vrai sinon */
	bso__bool EstVide( void )
	{
		return Vide_();
	}
	// Rattache au conteneur 'Conteneur'.
	void Init( const ctn_basic_container_<st> &Conteneur )
	{
		Conteneur_ = &Conteneur;
		Pilote_.Init( Conteneur.Dynamics );
	}
	//* Cale l'élément sur l'élément du conteneur à la position 'Position'
	void Caler(	POSITION__ Position )
	{
		if ( Pilote_.Index() != Position )
		{
			Vider_();
			Pilote_.Index( Position );
#ifdef CTN_DBG
			if ( Conteneur_ == NULL )
				ERRu();
#endif
			Conteneur_->Statics.Read( Position, ctn_S_ );
		}
	}
	// Synchronise avec l'élément du conteneur (leur contenu devient identique).
	void Vider( void )
	{
		Vider_();
	}
};


template <class st> struct ctn__item_mono_statique
: public st {};

/*c To reach an object from a 'MCONTAINER_( t )'. Use 'MITEM( t )'
rather then directly this class. */
template <class t> class ctn_volatile_mono_item
: public ctn__item_base_volatile< ctn__item_mono_statique<typename t::s> >
{
private:
	t Objet_;
	void BaseConstructeur_( void )
	{
		Objet_.reset( false );

		Objet_.plug( ctn__item_base_volatile< ctn__item_mono_statique< typename t::s > >::Pilote_ );
	}
public:
	ctn_volatile_mono_item( void )
	: Objet_( ctn__item_base_volatile< ctn__item_mono_statique< typename t::s > >::ctn_S_ )
	{
		BaseConstructeur_();
	}
	// Remplace la fonction d'initialisation. 
	ctn_volatile_mono_item( ctn_basic_container_< ctn__item_mono_statique< typename t::s > > &Conteneur )
	: Objet_( ctn__item_base_volatile< ctn__item_mono_statique< typename t::s > >::ctn_S_ )
	{
		BaseConstructeur_();

		ctn__item_base_volatile< ctn__item_mono_statique< typename t::s > >::Init( Conteneur );
	}
	virtual ~ctn_volatile_mono_item( void )
	{
		Sync();
		Objet_.reset( false );
	}
	ctn_volatile_mono_item &operator =( const ctn_volatile_mono_item &O )
	{
		ERRu();
	}
	//f Return the object at 'Position'.
	t &operator()( POSITION__ Position )
	{
		Sync( Position );
		return Objet_;
	}
	//f Return the object at current position.
	t &operator()( void )
	{
		return Objet_;
	}
};

/*c To reach an object of type from a 'MCONTAINER_( t )', but only for reading.
Use 'CMITEM( t )' rather then directly this class. */
template <class t> class ctn_const_mono_item
: public ctn__item_base_const< ctn__item_mono_statique< typename t::s > >
{
private:
	t Objet_;
public:
	ctn_const_mono_item( void )
	: Objet_( ctn__item_base_const< ctn__item_mono_statique< typename t::s > >::ctn_S_ )
	{
		Objet_.reset( false );

		Objet_.plug( ctn__item_base_const< ctn__item_mono_statique< typename t::s > >::Pilote_ );
	}
	virtual ~ctn_const_mono_item( void )
	{
		Vider();
		Objet_.reset( false );
	}
	ctn_const_mono_item &operator =( const ctn_const_mono_item &O )
	{
		ERRu();
	}
	//f Return the object at 'Position'.
	const t &operator()( POSITION__ Position )
	{
		Caler( Position );
		return Objet_;
	}
	//f Return the object at current position.
	const t &operator()( void )
	{
		return Objet_;
	}
};



//s To reach an item from 'MCONTAINER_( Type )'.
#define MITEM( Type )	ctn_volatile_mono_item< Type >

//s To reach an item from 'MCONTAINER_( Type )' , but only for reading.
#define CMITEM( Type )	ctn_const_mono_item< Type >

/*c Container for object of type 'Type', which need only one memory.
Use 'MCONTAINER_( Type )' rather then directly this class. */
template <class t> class ctn_mono_container_
: public ctn_basic_container_< ctn__item_mono_statique<typename t::s> >
{
private:
	MITEM( t ) Ponctuel_;
public:
	struct s
	: public ctn_basic_container_< ctn__item_mono_statique<typename t::s> >::s
	{};
	ctn_mono_container_( s &S )
	: ctn_basic_container_< ctn__item_mono_statique<typename t::s> >( S )
	{
		 Ponctuel_.Init( *this );
	}
	void reset( bool P = true )
	{
		Ponctuel_.Sync();
		ctn_basic_container_< ctn__item_mono_statique< typename t::s > >::reset( P );
	}
	/*f Return the object at position 'Position'. BE CAREFUL: after calling this fonction
	and if you want to call another fonction as this fonction or the next, you MUST call
	the function 'Sync()' before. */
	t &operator ()( POSITION__ Position )
	{
		return Ponctuel_( Position );
	}
	/*f Return the object at current position. This position is the position you
	gave to the previous function. BE CAREFUL: after calling this fonction
	and if you want to call another fonction as this fonction or the previous,
	you MUST call the function 'Sync()' before. */
	t &operator ()( void )
	{
		return Ponctuel_();
	}
	/*f Call this function after calling one of the two previous function
	and before calling another function. */
	void Sync( void )
	{
		Ponctuel_.Sync();
	}
	/*f Return the object at 'Position' using 'Item'.
	Valid only until next modification of 'Item'. */
	const t& operator()(
		POSITION__ Position,
		CMITEM( t ) &Item ) const
	{
		Item.Init( *this );

		return Item( Position );
	}
	//f Write 'Object' at 'Position'.
	void Write(
		const t & Objet,
		POSITION__ Position )
	{
		operator()( Position ) = Objet;

		Sync();
	}
	//f Put in 'Object' the object at position 'Position'.
	void Read(
		POSITION__ Position,
		t &Objet ) const
	{
		CMITEM( t ) Element;

		Element.Init( *this );

		Objet = Element( Position );
	}
	//f Allocate room for 'Size' objects.
	void Allocate( tym__size Size )
	{
		MITEM( t ) E;

		ctn_basic_container_< ctn__item_mono_statique< typename t::s > >::Allocate( Size, E.ctn_S_ );	// pas de E.xxx::ctn_S_ car G++ V2.90.29 n'aime pas
	}
	ctn_mono_container_ &operator =( const ctn_mono_container_ &C )
	{
		ctn_basic_container_< ctn__item_mono_statique< typename t::s > >::operator =( C );

		return *this;
	}
	//f Create a new object and return its position.
	POSITION__ New( tym__size Size = 1 )
	{
		POSITION__ P = this->Amount();

		Allocate( P + Size );

		return P;
	}
};

AUTO1( ctn_mono_container )

//s Container of object of type 'Type', which need only one memory.
#define MCONTAINER_( Type ) ctn_mono_container_< Type >
#define MCONTAINER( Type ) ctn_mono_container< Type >

//c Same as ctn_mono_container_, but for use with object which have a 'Init()' function without parameters.
template <class t> class ctn_mono_extended_container_
: public ctn_mono_container_<t>
{
private:
	// Create an initialized object. Return position of the created object.
	POSITION__ Create_( void )
	{
		POSITION__ P = ctn_mono_container_<t>::New();

		ctn_mono_container_<t>::operator()( P ).Init();

		return P;
	}
public:
	struct s
	: public ctn_mono_container_<t>::s
	{};
	ctn_mono_extended_container_( s &S )
	: ctn_mono_container_<t>( S )
	{}
	void reset( bool P = true )
	{
		ctn_mono_container_<t>::reset( P );
	}
	void plug( mmm_multimemory_ &M )
	{
		ctn_mono_container_<t>::plug( M );
	}
	ctn_mono_extended_container_ &operator =( const ctn_mono_extended_container_ &C )
	{
		ctn_mono_container_<t>::operator =( C );

		return *this;
	}
	//f Initialization.
	void Init( void )
	{
		ctn_mono_container_<t>::Init();
	}
	//f Create an initialized object. Return position of the created object.
	POSITION__ Create( void )
	{
		POSITION__ P = Create_();

		ctn_mono_container_<t>::Sync();

		return P;
	}
	//f Adding object 'Object'. Return position where added.
	POSITION__ Add( const t &Object )
	{
		POSITION__ P = Create_();

		ctn_mono_container_<t>::operator()() = Object;

		ctn_mono_container_<t>::Sync();

		return P;
	}
	//f Remove 'Amount' objects from the end of the container.
	void Truncate( SIZE__ Amount = 1 )
	{
#ifdef CTN_DBG
		if ( Amount > this->Amount() )
			ERRu();
#endif
		Allocate( this->Amount() - Amount );
	}
};


AUTO1( ctn_mono_extended_container )

//s Container of object of type 'Type', which need only one memory, and had a 'Init()' function without parameters.
#define XMCONTAINER_( Type ) ctn_mono_extended_container_< Type >
#define XMCONTAINER( Type ) ctn_mono_extended_container< Type >

template <class st> struct ctn__item_multi_statique
: public st
{
	mmm_multimemory_::s Multimemoire;
};

/*c To reach an object from a 'CONTAINER_( t )'. Use 'ITEM( t )'
rather then directly this class. */
template <class t> class ctn_volatile_multi_item
: public ctn__item_base_volatile< ctn__item_multi_statique<typename t::s> >
{
private:
	t Objet_;
	void BaseConstructeur_( void )
	{
		Objet_.reset( false );
		Multimemoire.reset( false );

		Multimemoire.plug( ctn__item_base_volatile< ctn__item_multi_statique< typename t::s > >::Pilote_ );
		Objet_.plug( Multimemoire );
	}
public:
	mmm_multimemory_ Multimemoire;
	ctn_volatile_multi_item( void )
	: Objet_( ctn__item_base_volatile< ctn__item_multi_statique< typename t::s > >::ctn_S_ ),
	  Multimemoire( ctn__item_base_volatile< ctn__item_multi_statique< typename t::s > >::ctn_S_.Multimemoire )
	{
		BaseConstructeur_();
	}
	// Remplace la fonction d'initialisation.
	ctn_volatile_multi_item( ctn_basic_container_< ctn__item_multi_statique< typename t::s > > &Conteneur )
	: Objet_( ctn__item_base_volatile< ctn__item_multi_statique< typename t::s > >::ctn_S_ ),
	  Multimemoire( ctn__item_base_volatile< ctn__item_multi_statique< typename t::s > >::ctn_S_.Multimemoire )
	{
		BaseConstructeur_();

		Multimemoire.Init();
		ctn__item_base_volatile< ctn__item_multi_statique< typename t::s > >::Init( Conteneur );
	}
	~ctn_volatile_multi_item( void )
	{
		Sync();

		Objet_.reset( false );
		Multimemoire.reset( true );
	}
	//f Initialize with container 'Container', in mode 'Mode'.
	void Init(
		ctn_basic_container_< ctn__item_multi_statique< typename t::s > > &Container,
		mdr::mode Mode = mdr::mReadWrite )
	{
		Multimemoire.Init();
		ctn__item_base_volatile< ctn__item_multi_statique< typename t::s > >::Init( Container, Mode );
	}
	ctn_volatile_multi_item &operator =( const ctn_volatile_multi_item &O )
	{
		ERRu();
	}
	//f Return the object at 'Position'.
	t &operator()( POSITION__ Position )
	{
		Sync( Position );
		return Objet_;
	}
	//f Return the object at current position.
	t &operator()( void )
	{
		return Objet_;
	}
};


/*c To reach an item from a 'CONTAINER_( t )', but only in read-only mode.
Use 'CITEM( t )' rather then directly this class. */
template <class t> class ctn_const_multi_item
: public ctn__item_base_const< ctn__item_multi_statique<typename t::s> >
{
private:
	t Objet_;
public:
	mmm_multimemory_ Multimemoire;
	ctn_const_multi_item( void )
	: Objet_( ctn__item_base_const< ctn__item_multi_statique< typename t::s > >::ctn_S_ ),
	  Multimemoire( ctn__item_base_const< ctn__item_multi_statique< typename t::s > >::ctn_S_.Multimemoire )
	{
		Objet_.reset( false );
		Multimemoire.reset( false );

		Multimemoire.plug( ctn__item_base_const< ctn__item_multi_statique< typename t::s > >::Pilote_ );
		Objet_.plug( Multimemoire );
	}
	virtual ~ctn_const_multi_item( void )
	{
		Vider();
		
		Objet_.reset( false );
		Multimemoire.reset( true );
	}
	//f Initializing with container 'Container'.
	void Init( const ctn_basic_container_< ctn__item_multi_statique<typename t::s> > &Container )
	{
		Multimemoire.Init();
		ctn__item_base_const< ctn__item_multi_statique< typename t::s > >::Init( Container );
	}
	ctn_const_multi_item &operator =( const ctn_const_multi_item &O )
	{
		ERRu();
	}
	//f Return the object at 'Position'.
	const t &operator()( POSITION__ Position )
	{
		Caler( Position );
		return Objet_;
	}
	//f Return the object at current position.
	const t &operator()( void )
	{
		return Objet_;
	}
};


//s To reach an item from 'CONTAINER_( Type )'.
#define ITEM( Type )		ctn_volatile_multi_item< Type >

//s To reach an item from 'CONTAINER_( Type )', but only for reading.
#define CITEM( Type )	ctn_const_multi_item< Type >

/*c Container for objects 't', with static part 'st', which need more then one memory.
Use 'CONTAINER_( t )' rather then directly this class.*/
template <class t> class ctn_multi_container_
: public ctn_basic_container_< ctn__item_multi_statique< typename t::s > >
{
private:
	ITEM( t ) Ponctuel_;
public:
	struct s
	: public ctn_basic_container_< ctn__item_multi_statique< typename t::s > >::s {};
	ctn_multi_container_( s &S )
	: ctn_basic_container_< ctn__item_multi_statique< typename t::s > >( S )
	{
		Ponctuel_.Init( *this );
	}
	void reset( bool P = true )
	{
		Ponctuel_.Sync();
		ctn_basic_container_< ctn__item_multi_statique< typename t::s > >::reset( P );
	}
	/*f Return the object at position 'Position'. BE CAREFUL: after calling this fonction
	and if you want to call another fonction as this fonction or the next, you MUST call
	the function 'Sync()' before. */
	t &operator ()( POSITION__ Position )
	{
		return Ponctuel_( Position );
	}
	/*f Return the object at current position. This position is the position you
	gave to the previous function. BE CAREFUL: after calling this fonction
	and if you want to call another fonction as this fonction or the previous,
	you MUST call the function 'Sync()' before. */
	t &operator ()( void )
	{
		return Ponctuel_();
	}
	/*f Always call this function after calling one of the two previous function
	and before calling another function. */
	void Sync( void )
	{
		Ponctuel_.Sync();
	}
	/*f Return the object at 'Position' using 'Item'.
	Valid only until next modification of 'Item'. */
	const t& operator()(
		POSITION__ Position,
		CITEM( t ) &Item ) const
	{
		Item.Init( *this );

		return Item( Position );
	}
	//f Write 'Object' at 'Position'.
	void Write(
		const t & Object,
		POSITION__ Position )
	{
		operator()( Position ) = Object;

		Sync();
	}
	//f Put in 'Object' the object at position 'Position'.
	void Read(
		POSITION__ Position,
		t &Objet ) const
	{
		CITEM( t ) Element;

		Element.Init( *this );

		Objet = Element( Position );
	}
	//f Allocate room for 'Capacity' objects.
	void Allocate( ctn__size Capacity )
	{
		ITEM( t ) E;

		ctn_basic_container_< ctn__item_multi_statique< typename t::s > >::Allocate( Capacity, E.ctn_S_ );// pas de E.xxx::ctn_S_ car G++ V2.90.29 n'aime pas
	}
	ctn_multi_container_ &operator =( const ctn_multi_container_ &C )
	{
		ctn_basic_container_< ctn__item_multi_statique< typename t::s > >::operator =( C );

		return *this;
	}
	//f Create a new object and return its position.
	POSITION__ New( tym__size Size = 1 )
	{
		POSITION__ P = this->Amount();

		Allocate( P + Size );

		return P;
	}
};

AUTO1( ctn_multi_container )

//s Container of object of type 'Type', which need more then one memory.
#define CONTAINER_( Type ) ctn_multi_container_< Type >
#define CONTAINER( Type ) ctn_multi_container< Type >

//c Same as ctn_multi_container_, but for use with object which have a 'Init()' function without parameters.
template <class t> class ctn_multi_extended_container_
: public ctn_multi_container_< t >
{
private:
	// Create an initialized object. Return position of the created object.
	POSITION__ Create_( void )
	{
		POSITION__ P = ctn_multi_container_< t >::New();

		ctn_multi_container_< t >::operator()( P ).Init();

		return P;
	}
public:
	struct s
	: public ctn_multi_container_< t >::s
	{};
	ctn_multi_extended_container_( s &S )
	: ctn_multi_container_< t >( S )
	{}
	void reset( bool P = true )
	{
		ctn_multi_container_< t >::reset( P );
	}
	void plug( mmm_multimemory_ &M )
	{
		ctn_multi_container_< t >::plug( M );
	}
	ctn_multi_extended_container_ &operator =( const ctn_multi_extended_container_ &C )
	{
		ctn_multi_container_< t >::operator =( C );
		
		return *this;
	}
	//f Initialization.
	void Init( void )
	{
		ctn_multi_container_< t >::Init();
	}
	//f Create an initialized object. Return position of the created object.
	POSITION__ Create( void )
	{
		POSITION__ P = Create_();

		ctn_multi_container_< t >::Sync();

		return P;
	}
	//f Adding object 'Object'. Return position where added.
	POSITION__ Add( const t &Object )
	{
		POSITION__ P = Create_();

		ctn_multi_container_< t >::operator()() = Object;

		ctn_multi_container_< t >::Sync();

		return P;
	}
	//f Remove 'Amount' objects from the end of the container.
	void Truncate( SIZE__ Amount = 1 )
	{
#ifdef CTN_DBG
		if ( Amount > this->Amount() )
			ERRu();
#endif
		Allocate( this->Amount() - Amount );
	}
};


AUTO1( ctn_multi_extended_container )

//s Container of object of type 'Type', and which have a 'Init()' function without parameters.
#define XCONTAINER_( Type ) ctn_multi_extended_container_< Type >
#define XCONTAINER( Type ) ctn_multi_extended_container< Type >

#ifdef CPE__VC
#undef typename
#endif


/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
