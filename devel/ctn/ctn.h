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
#include "aem.h"


#ifdef CPE__VC
// VC++ had some difficulties with 'typename'.
#define typename
#endif

namespace ctn {

	using tym::size__;


	/* Remplit les parties statiques des objets 'Debut' à 'Fin' inclus
	contenus dans 'Memoire', sachant qu'ils doivent contenir 'Statique'
	qui est de taille 'Taille' */
	void Remplir_(
		uym::untyped_memory_ &Memoire,
		uym::row__ Debut,
		uym::row__ Fin,
		uym::data__ *Statique,
		uym::bsize__ Taille );


	//c The base of a container. Internal use.
	template <class st> class basic_container_
	: public aem::amount_extent_manager_
	{
	public:
		//r All the static parts.
		tym::E_MEMORY_( st ) Statics;
		//r All the dynamic parts.
		mmi::indexed_multimemory_ Dynamics;
		struct s
		: public aem::amount_extent_manager_::s
		{
			tym::E_MEMORY_( st )::s Statics;
			mmi::indexed_multimemory_::s Dynamics;
		};
		basic_container_( s &S )
		: Dynamics( S.Dynamics ),
		  Statics( S.Statics ),
		  amount_extent_manager_( S )
		{}
		void reset( bool P = true )
		{
			Dynamics.reset( P );
			Statics.reset( P );
			amount_extent_manager_::reset( P );
		}
		void plug( mmm::multimemory_ &M )
		{
			Dynamics.plug( M );
			Statics.plug( M );
	//		amount_extent_manager_::plug( M );	// Not relevant
		}
		basic_container_ &operator =( const basic_container_ &O )
		{
			Dynamics.Copy( O.Dynamics, O.Amount() );
			Statics.Allocate( O.Amount() );
			Statics.Write( O.Statics, O.Amount() ); 

			amount_extent_manager_::Force( O.Amount() );
			amount_extent_manager_::operator =( O );

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
			amount_extent_manager_::Init();
			amount_extent_manager_::SetStepValue( 0 );	//Preallocation not very usefull for containers.
		}
		//f Allocation room for 'Size' object of statical part 'ST'.
		void Allocate(
			uym::size__ Size,
			st &ST,
			bso__bool Adjust )
		{
			tym::size__ AncCap;
			tym::size__ Extent = Size;

			AncCap = amount_extent_manager_::Amount();

			if ( amount_extent_manager_::AmountToAllocate( Extent, Adjust ) ) {
				Dynamics.Allocate( Extent, AncCap );
				Statics.Allocate( Extent );
			}

			if ( AncCap < Size )
			{
				if ( ( Size - AncCap ) > 1 )
					Remplir_( this->Statics, AncCap, Size - 1, (uym::data__ *)&ST, sizeof( ST ) );
				else
					Statics.Write( ST, AncCap );
			}
		}
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
		//f Adjust the extent/amount to 'Size'.
		void Adjust( void )
		{
			tym::size__ Extent = this->Extent();

			if ( amount_extent_manager_::Force( Size ) ) {
				Dynamics.Allocate( Size, Extent );
				Statics.Allocate( Size );
			}
		}
	};


	//c The base of a volatile item. Internal use.
	template <class st> class item_base_volatile__
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
				Conteneur_->Statics.Write( ctn_S_, Pilote_.Index()() );
			}

			Pilote_.Index( NONE );
		}
	protected:
		// Conteneur auquel est rattaché l'élément.
	//	ctn_conteneur_base_ < ctn_item_mono < t, st > :: s  > *Conteneur_;
		basic_container_<st> *Conteneur_;
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
		item_base_volatile__( void )
		: Pilote_( ctn_S_.Pilote_ )
		{
			Pilote_.reset( false );

			Conteneur_ = NULL;
			Mode_ = mdr::mReadOnly;
		}
		~item_base_volatile__( void )
		{
			Sync();

			Pilote_.reset( true );
		}
	/*	tym::row__ ctn_Position( void )
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
			basic_container_<st> &Conteneur,
			mdr::mode Mode = mdr::mReadWrite )
		{
			Init( &Conteneur, Mode );
		}
		// Rattache au conteneur 'Conteneur'.
		void Init(
			basic_container_<st> *Conteneur,
			mdr::mode Mode = mdr::mReadWrite )
		{
			Conteneur_ = Conteneur;
			Pilote_.Init( Conteneur->Dynamics );
			Mode_ = Mode;
		}
		//* Cale l'élément sur l'élément du conteneur à la position 'Position'
		void Sync( tym::row__ Position )
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

	template <class st> class item_base_const__
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
		const basic_container_<st> *Conteneur_;
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
		item_base_const__( void )
		: Pilote_( ctn_S_.Pilote_ )
		{
			Pilote_.reset( false );

			Conteneur_ = NULL;
		}
		~item_base_const__( void )
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
		void Init( const basic_container_<st> &Conteneur )
		{
			Conteneur_ = &Conteneur;
			Pilote_.Init( Conteneur.Dynamics );
		}
		//* Cale l'élément sur l'élément du conteneur à la position 'Position'
		void Caler(	tym::row__ Position )
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


	template <class st> struct item_mono_statique__
	: public st {};

	/*c To reach an object from a 'MCONTAINER_( t )'. Use 'MITEM( t )'
	rather then directly this class. */
	template <class t> class volatile_mono_item
	: public item_base_volatile__< item_mono_statique__<typename t::s> >
	{
	private:
		t Objet_;
		void BaseConstructeur_( void )
		{
			Objet_.reset( false );

			Objet_.plug( item_base_volatile__< item_mono_statique__< typename t::s > >::Pilote_ );
		}
	public:
		volatile_mono_item( void )
		: Objet_( item_base_volatile__< item_mono_statique__< typename t::s > >::ctn_S_ )
		{
			BaseConstructeur_();
		}
		// Remplace la fonction d'initialisation. 
		volatile_mono_item( basic_container_< item_mono_statique__< typename t::s > > &Conteneur )
		: Objet_( item_base_volatile__< item_mono_statique__< typename t::s > >::ctn_S_ )
		{
			BaseConstructeur_();

			item_base_volatile__< item_mono_statique__< typename t::s > >::Init( Conteneur );
		}
		virtual ~volatile_mono_item( void )
		{
			Sync();
			Objet_.reset( false );
		}
		volatile_mono_item &operator =( const volatile_mono_item &O )
		{
			ERRu();
		}
		//f Return the object at 'Position'.
		t &operator()( tym::row__ Position )
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
	template <class t> class const_mono_item
	: public item_base_const__< item_mono_statique__< typename t::s > >
	{
	private:
		t Objet_;
	public:
		const_mono_item( void )
		: Objet_( item_base_const__< item_mono_statique__< typename t::s > >::ctn_S_ )
		{
			Objet_.reset( false );

			Objet_.plug( item_base_const__< item_mono_statique__< typename t::s > >::Pilote_ );
		}
		virtual ~const_mono_item( void )
		{
			Vider();
			Objet_.reset( false );
		}
		const_mono_item &operator =( const const_mono_item &O )
		{
			ERRu();
		}
		//f Return the object at 'Position'.
		const t &operator()( tym::row__ Position )
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
	#define E_MITEM( Type )	volatile_mono_item< Type >

	//s To reach an item from 'MCONTAINER_( Type )' , but only for reading.
	#define E_CMITEM( Type )	const_mono_item< Type >

	/*c Container for object of type 'Type', which need only one memory.
	Use 'MCONTAINER_( Type )' rather then directly this class. */
	template <class t> class mono_container_
	: public basic_container_< item_mono_statique__<typename t::s> >
	{
	private:
		E_MITEM( t ) Ponctuel_;
	public:
		struct s
		: public basic_container_< item_mono_statique__<typename t::s> >::s
		{};
		mono_container_( s &S )
		: basic_container_< item_mono_statique__<typename t::s> >( S )
		{
			 Ponctuel_.Init( *this );
		}
		void reset( bool P = true )
		{
			Ponctuel_.Sync();
			basic_container_< item_mono_statique__< typename t::s > >::reset( P );
		}
		/*f Return the object at position 'Position'. BE CAREFUL: after calling this fonction
		and if you want to call another fonction as this fonction or the next, you MUST call
		the function 'Sync()' before. */
		t &operator ()( tym::row__ Position )
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
			tym::row__ Position,
			E_CMITEM( t ) &Item ) const
		{
			Item.Init( *this );

			return Item( Position );
		}
		//f Write 'Object' at 'Position'.
		void Write(
			const t & Objet,
			tym::row__ Position )
		{
			operator()( Position ) = Objet;

			Sync();
		}
		//f Put in 'Object' the object at position 'Position'.
		void Read(
			tym::row__ Position,
			t &Objet ) const
		{
			E_CMITEM( t ) Element;

			Element.Init( *this );

			Objet = Element( Position );
		}
		//f Allocate room for 'Size' objects.
		void Allocate(
			tym::size__ Size,
			bso__bool Adjust = false )
		{
			E_MITEM( t ) E;

			basic_container_< item_mono_statique__< typename t::s > >::Allocate( Size, E.ctn_S_, Adjust );	// pas de E.xxx::ctn_S_ car G++ V2.90.29 n'aime pas
		}
		mono_container_ &operator =( const mono_container_ &C )
		{
			basic_container_< item_mono_statique__< typename t::s > >::operator =( C );

			return *this;
		}
		//f Create a new object and return its position.
		tym::row__ New( tym::size__ Size = 1 )
		{
			tym::row__ P = this->Amount();

			Allocate( P + Size );

			return P;
		}
	};

	AUTO1( mono_container )

	//s Container of object of type 'Type', which need only one memory.
	#define E_MCONTAINER_( Type ) mono_container_< Type >
	#define E_MCONTAINER( Type ) mono_container< Type >

	//c Same as mono_container_, but for use with object which have a 'Init()' function without parameters.
	template <class t> class mono_extended_container_
	: public mono_container_<t>
	{
	private:
		// Create an initialized object. Return position of the created object.
		tym::row__ Create_( void )
		{
			tym::row__ P = mono_container_<t>::New();

			mono_container_<t>::operator()( P ).Init();

			return P;
		}
	public:
		struct s
		: public mono_container_<t>::s
		{};
		mono_extended_container_( s &S )
		: mono_container_<t>( S )
		{}
		void reset( bool P = true )
		{
			mono_container_<t>::reset( P );
		}
		void plug( mmm::multimemory_ &M )
		{
			mono_container_<t>::plug( M );
		}
		mono_extended_container_ &operator =( const mono_extended_container_ &C )
		{
			mono_container_<t>::operator =( C );

			return *this;
		}
		//f Initialization.
		void Init( void )
		{
			mono_container_<t>::Init();
		}
		//f Create an initialized object. Return position of the created object.
		tym::row__ Create( void )
		{
			tym::row__ P = Create_();

			mono_container_<t>::Sync();

			return P;
		}
		//f Adding object 'Object'. Return position where added.
		tym::row__ Add( const t &Object )
		{
			tym::row__ P = Create_();

			mono_container_<t>::operator()() = Object;

			mono_container_<t>::Sync();

			return P;
		}
		//f Remove 'Amount' objects from the end of the container.
		void Truncate( tym::size__ Amount = 1 )
		{
	#ifdef CTN_DBG
			if ( Amount > this->Amount() )
				ERRu();
	#endif
			Allocate( this->Amount() - Amount );
		}
	};


	AUTO1( mono_extended_container )

	//s Container of object of type 'Type', which need only one memory, and had a 'Init()' function without parameters.
	#define E_XMCONTAINER_( Type ) mono_extended_container_< Type >
	#define E_XMCONTAINER( Type ) mono_extended_container< Type >

	template <class st> struct item_multi_statique__
	: public st
	{
		mmm::multimemory_::s Multimemoire;
	};

	/*c To reach an object from a 'CONTAINER_( t )'. Use 'ITEM( t )'
	rather then directly this class. */
	template <class t> class volatile_multi_item
	: public item_base_volatile__< item_multi_statique__<typename t::s> >
	{
	private:
		t Objet_;
		void BaseConstructeur_( void )
		{
			Objet_.reset( false );
			Multimemoire.reset( false );

			Multimemoire.plug( item_base_volatile__< item_multi_statique__< typename t::s > >::Pilote_ );
			Objet_.plug( Multimemoire );
		}
	public:
		mmm::multimemory_ Multimemoire;
		volatile_multi_item( void )
		: Objet_( item_base_volatile__< item_multi_statique__< typename t::s > >::ctn_S_ ),
		  Multimemoire( item_base_volatile__< item_multi_statique__< typename t::s > >::ctn_S_.Multimemoire )
		{
			BaseConstructeur_();
		}
		// Remplace la fonction d'initialisation.
		volatile_multi_item( basic_container_< item_multi_statique__< typename t::s > > &Conteneur )
		: Objet_( item_base_volatile__< item_multi_statique__< typename t::s > >::ctn_S_ ),
		  Multimemoire( item_base_volatile__< item_multi_statique__< typename t::s > >::ctn_S_.Multimemoire )
		{
			BaseConstructeur_();

			Multimemoire.Init();
			item_base_volatile__< item_multi_statique__< typename t::s > >::Init( Conteneur );
		}
		~volatile_multi_item( void )
		{
			Sync();

			Objet_.reset( false );
			Multimemoire.reset( true );
		}
		//f Initialize with container 'Container', in mode 'Mode'.
		void Init(
			basic_container_< item_multi_statique__< typename t::s > > &Container,
			mdr::mode Mode = mdr::mReadWrite )
		{
			Multimemoire.Init();
			item_base_volatile__< item_multi_statique__< typename t::s > >::Init( Container, Mode );
		}
		volatile_multi_item &operator =( const volatile_multi_item &O )
		{
			ERRu();
		}
		//f Return the object at 'Position'.
		t &operator()( tym::row__ Position )
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
	template <class t> class const_multi_item
	: public item_base_const__< item_multi_statique__<typename t::s> >
	{
	private:
		t Objet_;
	public:
		mmm::multimemory_ Multimemoire;
		const_multi_item( void )
		: Objet_( item_base_const__< item_multi_statique__< typename t::s > >::ctn_S_ ),
		  Multimemoire( item_base_const__< item_multi_statique__< typename t::s > >::ctn_S_.Multimemoire )
		{
			Objet_.reset( false );
			Multimemoire.reset( false );

			Multimemoire.plug( item_base_const__< item_multi_statique__< typename t::s > >::Pilote_ );
			Objet_.plug( Multimemoire );
		}
		virtual ~const_multi_item( void )
		{
			Vider();
			
			Objet_.reset( false );
			Multimemoire.reset( true );
		}
		//f Initializing with container 'Container'.
		void Init( const basic_container_< item_multi_statique__<typename t::s> > &Container )
		{
			Multimemoire.Init();
			item_base_const__< item_multi_statique__< typename t::s > >::Init( Container );
		}
		const_multi_item &operator =( const const_multi_item &O )
		{
			ERRu();
		}
		//f Return the object at 'Position'.
		const t &operator()( tym::row__ Position )
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
	#define E_ITEM( Type )		volatile_multi_item< Type >

	//s To reach an item from 'CONTAINER_( Type )', but only for reading.
	#define E_CITEM( Type )	const_multi_item< Type >

	/*c Container for objects 't', with static part 'st', which need more then one memory.
	Use 'CONTAINER_( t )' rather then directly this class.*/
	template <class t> class multi_container_
	: public basic_container_< item_multi_statique__< typename t::s > >
	{
	private:
		E_ITEM( t ) Ponctuel_;
	public:
		struct s
		: public basic_container_< item_multi_statique__< typename t::s > >::s {};
		multi_container_( s &S )
		: basic_container_< item_multi_statique__< typename t::s > >( S )
		{
			Ponctuel_.Init( *this );
		}
		void reset( bool P = true )
		{
			Ponctuel_.Sync();
			basic_container_< item_multi_statique__< typename t::s > >::reset( P );
		}
		/*f Return the object at position 'Position'. BE CAREFUL: after calling this fonction
		and if you want to call another fonction as this fonction or the next, you MUST call
		the function 'Sync()' before. */
		t &operator ()( tym::row__ Position )
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
			tym::row__ Position,
			E_CITEM( t ) &Item ) const
		{
			Item.Init( *this );

			return Item( Position );
		}
		//f Write 'Object' at 'Position'.
		void Write(
			const t & Object,
			tym::row__ Position )
		{
			operator()( Position ) = Object;

			Sync();
		}
		//f Put in 'Object' the object at position 'Position'.
		void Read(
			tym::row__ Position,
			t &Objet ) const
		{
			E_CITEM( t ) Element;

			Element.Init( *this );

			Objet = Element( Position );
		}
		//f Allocate room for 'Capacity' objects.
		void Allocate(
			size__ Capacity,
			bso__bool Adjust = false )
		{
			E_ITEM( t ) E;

			basic_container_< item_multi_statique__< typename t::s > >::Allocate( Capacity, E.ctn_S_, Adjust );// pas de E.xxx::ctn_S_ car G++ V2.90.29 n'aime pas
		}
		multi_container_ &operator =( const multi_container_ &C )
		{
			basic_container_< item_multi_statique__< typename t::s > >::operator =( C );

			return *this;
		}
		//f Create a new object and return its position.
		tym::row__ New( tym::size__ Size = 1 )
		{
			tym::row__ P = this->Amount();

			Allocate( P + Size );

			return P;
		}
	};

	AUTO1( multi_container )

	//s Container of object of type 'Type', which need more then one memory.
	#define E_CONTAINER_( Type ) multi_container_< Type >
	#define E_CONTAINER( Type ) multi_container< Type >

	//c Same as multi_container_, but for use with object which have a 'Init()' function without parameters.
	template <class t> class multi_extended_container_
	: public multi_container_< t >
	{
	private:
		// Create an initialized object. Return position of the created object.
		tym::row__ Create_( void )
		{
			tym::row__ P = multi_container_< t >::New();

			multi_container_< t >::operator()( P ).Init();

			return P;
		}
	public:
		struct s
		: public multi_container_< t >::s
		{};
		multi_extended_container_( s &S )
		: multi_container_< t >( S )
		{}
		void reset( bool P = true )
		{
			multi_container_< t >::reset( P );
		}
		void plug( mmm::multimemory_ &M )
		{
			multi_container_< t >::plug( M );
		}
		multi_extended_container_ &operator =( const multi_extended_container_ &C )
		{
			multi_container_< t >::operator =( C );
			
			return *this;
		}
		//f Initialization.
		void Init( void )
		{
			multi_container_< t >::Init();
		}
		//f Create an initialized object. Return position of the created object.
		tym::row__ Create( void )
		{
			tym::row__ P = Create_();

			multi_container_< t >::Sync();

			return P;
		}
		//f Adding object 'Object'. Return position where added.
		tym::row__ Add( const t &Object )
		{
			tym::row__ P = Create_();

			multi_container_< t >::operator()() = Object;

			multi_container_< t >::Sync();

			return P;
		}
		//f Remove 'Amount' objects from the end of the container.
		void Truncate( tym::size__ Amount = 1 )
		{
	#ifdef CTN_DBG
			if ( Amount > this->Amount() )
				ERRu();
	#endif
			Allocate( this->Amount() - Amount );
		}
	};


	AUTO1( multi_extended_container )

	//s Container of object of type 'Type', and which have a 'Init()' function without parameters.
	#define E_XCONTAINER_( Type ) multi_extended_container_< Type >
	#define E_XCONTAINER( Type ) multi_extended_container< Type >
}

#ifdef CPE__VC
#undef typename
#endif


/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
