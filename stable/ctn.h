/*
	Header for the 'ctn' library by Claude SIMON (csimon@epeios.org)
	Copyright (C) 2000-2002  Claude SIMON (csimon@epeios.org).

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

#ifndef CTN__INC
#define CTN__INC

#define CTN_NAME		"CTN"

#define	CTN_VERSION	"$Revision$"

#define CTN_OWNER		"Claude SIMON (csimon@epeios.org)"

#include "ttr.h"

extern class ttr_tutor &CTNTutor;

#if defined( XXX_DBG ) && !defined( CTN_NODBG )
#define CTN_DBG
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
//D ConTaiNer (of dynamic objects). 
/* End addendum to automatic documentation generation part. */

#include "err.h"
#include "flw.h"
#include "mmi.h"
#include "cpe.h"
#include "aem.h"


#ifdef CPE__USE_VC_WORKAROUND
#	define typename_
#else
#	define	typename_	typename
#endif

namespace ctn {

	using epeios::size__;
	using aem::amount_extent_manager_;


	/* Remplit les parties statiques des objets 'Debut' à 'Fin' inclus
	contenus dans 'Memoire', sachant qu'ils doivent contenir 'Statique'
	qui est de taille 'Taille' */
	void Remplir_(
		uym::untyped_memory_ &Memoire,
		epeios::row_t__ Debut,
		epeios::row_t__ Fin,
		epeios::data__ *Statique,
		epeios::bsize__ Taille );


	//c The base of a container. Internal use.
	template <class st, typename r> class basic_container_
	: public aem::amount_extent_manager_<r>
	{
	public:
		//r All the static parts.
		tym::E_MEMORYt_( st, r ) Statics;
		//r All the dynamic parts.
		mmi::indexed_multimemory_ Dynamics;
		struct s
		: public aem::amount_extent_manager_<r>::s
		{
			tym::E_MEMORYt_( st, r )::s Statics;
			mmi::indexed_multimemory_::s Dynamics;
		};
		basic_container_( s &S )
		: Dynamics( S.Dynamics ),
		  Statics( S.Statics ),
		  amount_extent_manager_<r>( S )
		{}
		void reset( bool P = true )
		{
			Dynamics.reset( P );
			Statics.reset( P );
			amount_extent_manager_<r>::reset( P );
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

			amount_extent_manager_<r>::Force( O.Amount() );
			amount_extent_manager_<r>::operator =( O );

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
			this->amount_extent_manager_<r>::Init();
			amount_extent_manager_<r>::SetStepValue( 0 );	//Preallocation not very usefull for containers.
		}
		//f Allocation room for 'Size' object of statical part 'ST'.
		void Allocate(
			epeios::size__ Size,
			st &ST,
			aem::mode Mode )
		{
			epeios::size__ AncCap;
			epeios::size__ Extent = Size;

			AncCap = amount_extent_manager_<r>::Amount();

			if ( amount_extent_manager_<r>::AmountToAllocate( Extent, Mode ) ) {
				Dynamics.Allocate( Extent, AncCap );
				Statics.Allocate( Extent );
			}

			if ( AncCap < Size )
			{
				if ( ( Size - AncCap ) > 1 )
					Remplir_( this->Statics, AncCap, Size - 1, (epeios::data__ *)&ST, sizeof( ST ) );
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
			epeios::size__ Extent = this->Extent();

			if ( amount_extent_manager_<r>::Force( Size ) ) {
				Dynamics.Allocate( Size, Extent );
				Statics.Allocate( Size );
			}
		}
		//f Delete 'Amount' entries from 'Position'.
		void Delete(
			r Position,
			epeios::size__ Amount = 1,
			aem::mode Mode = aem::mDefault )
		{
			epeios::size__ CurrentExtent = amount_extent_manager_<r>::Extent();
			epeios::size__ NewExtent = CurrentExtent - Amount;

			Dynamics.DeleteWithoutReallocating( Position, CurrentExtent, Amount );
			Statics.Write( Statics, NewExtent - *Position, *Position + Amount, Position );

			if ( amount_extent_manager_<r>::AmountToAllocate( NewExtent, Mode ) ) {
				Dynamics.Allocate( NewExtent, CurrentExtent );
				Statics.Allocate( NewExtent );
			}
		}

	};


	//c The base of a volatile item. Internal use.
	template <class st, typename r> class item_base_volatile__
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
				Conteneur_->Statics.Write( ctn_S_, *Pilote_.Index() );
			}

			Pilote_.Index( NONE );
		}
	protected:
		// Conteneur auquel est rattaché l'élément.
	//	ctn_conteneur_base_ < ctn_item_mono < t, st > :: s  > *Conteneur_;
		basic_container_<st, r> *Conteneur_;
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
		void reset( bso::bool__ P = true )
		{
			if ( P ) {
				Sync();
			}

			Pilote_.reset( P );

			Conteneur_ = NULL;
			Mode_ = mdr::mReadOnly;
		}
		item_base_volatile__( void )
		: Pilote_( ctn_S_.Pilote_ )
		{
			reset( false );
		}
		~item_base_volatile__( void )
		{
			reset( true );
		}
	/*	tym::row__ ctn_Position( void )
		{
			return Pilote_.Index();
		}
	*/	/* Retourne faux si l'élément est calé sur un élément du conteneur,
		vrai sinon */
		bso::bool__ EstVide( void )
		{
			return Vide_();
		}
		// Rattache au conteneur 'Conteneur'.
		void Init(
			basic_container_<st, r> &Conteneur,
			mdr::mode Mode = mdr::mReadWrite )
		{
			Init( &Conteneur, Mode );
		}
		// Rattache au conteneur 'Conteneur'.
		void Init(
			basic_container_<st,r> *Conteneur,
			mdr::mode Mode = mdr::mReadWrite )
		{
			Conteneur_ = Conteneur;
			Pilote_.Init( Conteneur->Dynamics );
			Mode_ = Mode;
		}
		//* Cale l'élément sur l'élément du conteneur à la position 'Position'
		void Sync( r Position )
		{
#ifdef CTN_DBG
			if ( Position == NONE )
				ERRu();
#endif
			if ( Pilote_.Index() != *Position )
			{
				Vider_();
				Conteneur_->Statics.Read( Position, ctn_S_ );
				Pilote_.Index( *Position );
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

	template <class st, typename r> class item_base_const__
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
		const basic_container_<st,r> *Conteneur_;
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
		void reset( bso::bool__ P = true )
		{
			if ( P ) {
				Sync();
			}

			Pilote_.reset( P );

			Conteneur_ = NULL;
		}
		item_base_const__( void )
		: Pilote_( ctn_S_.Pilote_ )
		{
			reset( false );
		}
		~item_base_const__( void )
		{
			reset( true );
		}
	/*	mbs__position Position( void )
		{
			return Pilote_.Index();
		}
	*/	/* Retourne faux si l'élément est calé sur un élément du conteneur,
		vrai sinon */
		bso::bool__ EstVide( void )
		{
			return Vide_();
		}
		// Rattache au conteneur 'Conteneur'.
		void Init( const basic_container_<st,r> &Conteneur )
		{
			Conteneur_ = &Conteneur;
			Pilote_.Init( Conteneur.Dynamics );
		}
		//* Cale l'élément sur l'élément du conteneur à la position 'Position'
		void Sync( r Position )
		{
#ifdef CTN_DBG
			if ( Position == NONE )
				ERRu();
#endif
			if ( *Pilote_.Index() != *Position )
			{
				Vider_();
				Pilote_.Index( *Position );
	#ifdef CTN_DBG
				if ( Conteneur_ == NULL )
					ERRu();
	#endif
				Conteneur_->Statics.Read( Position, ctn_S_ );
			}
		}
		// Synchronise avec l'élément du conteneur (leur contenu devient identique).
		void Sync( void )
		{
			Vider_();
		}
	};


	template <class st> struct item_mono_statique__
	: public st {};

	/*c To reach an object from a 'MCONTAINER_( t )'. Use 'MITEM( t )'
	rather then directly this class. */
	template <class t, typename r> class volatile_mono_item
	: public item_base_volatile__< item_mono_statique__<typename t::s>, r >
	{
	private:
		t Objet_;
	public:
		void reset( bso::bool__ P = true )
		{
			item_base_volatile__< item_mono_statique__<typename_ t::s>, r >::reset( P );
				
			Objet_.reset( false );

			Objet_.plug( item_base_volatile__< item_mono_statique__< typename_ t::s >, r >::Pilote_ );
		}
		volatile_mono_item( void )
		: Objet_( item_base_volatile__< item_mono_statique__< typename_ t::s >, r >::ctn_S_ )
		{
			reset( false );
		}
		// Remplace la fonction d'initialisation. 
		volatile_mono_item( basic_container_< item_mono_statique__< typename t::s >, r > &Conteneur )
		: Objet_( item_base_volatile__< item_mono_statique__< typename t::s >, r >::ctn_S_ )
		{
			reset( false );

			item_base_volatile__< item_mono_statique__<typename_ t::s >, r >::Init( Conteneur );
		}
		virtual ~volatile_mono_item( void )
		{
			reset( true );
		}
		volatile_mono_item &operator =( const volatile_mono_item &O )
		{
			ERRu();
		}
		//f Return the object at 'Position'.
		t &operator()( r Position )
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
	template <class t, typename r> class const_mono_item
	: public item_base_const__< item_mono_statique__< typename t::s >, r >
	{
	private:
		t Objet_;
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P ) {
				Sync();
			}

			item_base_const__< item_mono_statique__<typename_ t::s >, r >::reset( P );
			Objet_.reset( false );

			Objet_.plug( item_base_const__< item_mono_statique__< typename_ t::s >, r >::Pilote_ );
		}
		const_mono_item( void )
		: Objet_( item_base_const__< item_mono_statique__< typename_ t::s >, r >::ctn_S_ )
		{
			reset( false );
		}
		virtual ~const_mono_item( void )
		{
			reset( true );
		}
		const_mono_item &operator =( const const_mono_item &O )
		{
			ERRu();
		}
		//f Return the object at 'Position'.
		const t &operator()( r Position )
		{
			Sync( Position );
			return Objet_;
		}
		//f Return the object at current position.
		const t &operator()( void )
		{
			return Objet_;
		}
	};



	//s To reach an item from 'MCONTAINER_( Type )'.
	#define E_MITEMt( Type, r )		volatile_mono_item< Type, r >
	#define E_CMITEMt( Type, r )	const_mono_item< Type, r >

#ifdef CPE__USE_GCC_WORKAROUND
	#define E_MITEM( Type )		volatile_mono_item< Type, epeios::row__ >
	#define E_CMITEM( Type )	const_mono_item< Type, epeios::row__ >
#else
	//s To reach an item from 'MCONTAINER_( Type )' , but only for reading.
	#define E_MITEM( Type )		E_MITEMt( Type, epeios::row__ )
	#define E_CMITEM( Type )	E_CMITEMt( Type, epeios::row__ )
#endif

	/*c Container for object of type 'Type', which need only one memory.
	Use 'MCONTAINER_( Type )' rather then directly this class. */
	template <class t, typename r> class mono_container_
	: public basic_container_< item_mono_statique__<typename t::s>, r >
	{
	private:
		E_MITEMt( t, r ) Ponctuel_;
	public:
		struct s
		: public basic_container_< item_mono_statique__<typename t::s>, r >::s
		{};
		mono_container_( s &S )
		: basic_container_< item_mono_statique__<typename_ t::s>, r >( S )
		{
			 Ponctuel_.Init( *this );
		}
		void reset( bool P = true )
		{
			Ponctuel_.Sync();
			basic_container_< item_mono_statique__< typename_ t::s >, r >::reset( P );
		}
		/*f Return the object at position 'Position'. BE CAREFUL: after calling this fonction
		and if you want to call another fonction as this fonction or the next, you MUST call
		the function 'Sync()' before. */
		t &Read( r Position )
		{
			return Ponctuel_( Position );
		}
		/*f Return the object at position 'Position'. BE CAREFUL: after calling this fonction
		and if you want to call another fonction as this fonction or the next, you MUST call
		the function 'Sync()' before. */
		t &operator ()( r Position )
		{
			return Read( Position );
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
		const t& Read(
			r Position,
			E_CMITEMt( t, r ) &Item ) const
		{
			Item.Init( *this );

			return Item( Position );
		}
		/*f Return the object at 'Position' using 'Item'.
		Valid only until next modification of 'Item'. */
		const t& operator()(
			r Position,
			E_CMITEMt( t, r ) &Item ) const
		{
			return Read( Position, Item );
		}
		//f Write 'Object' at 'Position'.
		void Write(
			const t & Objet,
			r Position )
		{
			operator()( Position ) = Objet;

			Sync();
		}
		//f Put in 'Object' the object at position 'Position'.
		void Read(
			r Position,
			t &Objet ) const
		{
			E_CMITEMt( t, r ) Element;

			Element.Init( *this );

			Objet = Element( Position );
		}
		//f Allocate room for 'Size' objects.
		void Allocate(
			epeios::size__ Size,
			aem::mode Mode = aem::mDefault )
		{
			E_MITEMt( t, r ) E;

			basic_container_< item_mono_statique__< typename_ t::s >, r >::Allocate( Size, E.ctn_S_, Mode );	// pas de E.xxx::ctn_S_ car G++ V2.90.29 n'aime pas
		}
		mono_container_ &operator =( const mono_container_ &C )
		{
			basic_container_< item_mono_statique__< typename_ t::s >, r >::operator =( C );

			return *this;
		}
		//f Create a new object and return its position.
		r New( epeios::size__ Size = 1 )
		{
			epeios::row_t__ P = this->Amount();

			Allocate( P + Size );

			return P;
		}
	};

	AUTO2( mono_container )

	//s Container of object of type 'Type', which need only one memory.
	#define E_MCONTAINERt_( Type, r )	mono_container_< Type, r >
	#define E_MCONTAINERt( Type, r )	mono_container< Type, r >

#ifdef CPE__USE_GCC_WORKAROUND
	#define E_MCONTAINER_( Type )	mono_container_< Type, epeios::row__ >
	#define E_MCONTAINER( Type )	mono_container_< Type, epeios::row__ >
#else
	#define E_MCONTAINER_( Type )	E_MCONTAINERt_( Type, epeios::row__ )
	#define E_MCONTAINER( Type )	E_MCONTAINERt( Type, epeios::row__ )
#endif

	//c Same as mono_container_, but for use with object which have a 'Init()' function without parameters.
	template <class t, typename r> class mono_extended_container_
	: public mono_container_<t, r>
	{
	private:
		// Create an initialized object. Return position of the created object.
		r Create_( void )
		{
			r P = mono_container_<t, r>::New();

			mono_container_<t, r>::operator()( P ).Init();

			return P;
		}
	public:
		struct s
		: public mono_container_<t, r>::s
		{};
		mono_extended_container_( s &S )
		: mono_container_<t, r>( S )
		{}
		void reset( bool P = true )
		{
			mono_container_<t, r>::reset( P );
		}
		void plug( mmm::multimemory_ &M )
		{
			mono_container_<t, r>::plug( M );
		}
		mono_extended_container_ &operator =( const mono_extended_container_ &C )
		{
			mono_container_<t, r>::operator =( C );

			return *this;
		}
		//f Initialization.
		void Init( void )
		{
			mono_container_<t, r>::Init();
		}
		//f Create an initialized object. Return position of the created object.
		r Create( void )
		{
			epeios::row__ P = Create_();

			mono_container_<t, r>::Sync();

			return P;
		}
		//f Adding object 'Object'. Return position where added.
		r Add( const t &Object )
		{
			r P = Create_();

			mono_container_<t, r>::operator()() = Object;	// Positionned by 'Create_'.

			mono_container_<t, r>::Sync();

			return P;
		}
		//f Remove 'Amount' objects from the end of the container.
		void Truncate( epeios::size__ Amount = 1 )
		{
	#ifdef CTN_DBG
			if ( Amount > this->Amount() )
				ERRu();
	#endif
			Allocate( this->Amount() - Amount );
		}
	};


	AUTO2( mono_extended_container )

	//s Container of object of type 'Type', which need only one memory, and had a 'Init()' function without parameters.
	#define E_XMCONTAINERt_( Type, r ) mono_extended_container_< Type, r >
	#define E_XMCONTAINERt( Type, r ) mono_extended_container< Type, r >

#ifdef CPE__USE_GCC_WORKAROUND
	#define E_XMCONTAINER_( Type )	mono_extended_container_< Type, epeios::row__ >
	#define E_XMCONTAINER( Type )	mono_extended_container< Type, epeios::row__ >
#else
	#define E_XMCONTAINER_( Type )	E_XMCONTAINERt_( Type, epeios::row__ )
	#define E_XMCONTAINER( Type )	E_XMCONTAINERt( Type, epeios::row__ )
#endif


	template <class st> struct item_multi_statique__
	: public st
	{
		mmm::multimemory_::s Multimemoire;
	};

	/*c To reach an object from a 'CONTAINER_( t )'. Use 'ITEM( t )'
	rather then directly this class. */
	template <class t, typename r> class volatile_multi_item
	: public item_base_volatile__< item_multi_statique__<typename t::s>, r >
	{
	private:
		t Objet_;
	public:
		mmm::multimemory_ Multimemoire;
		void reset( bso::bool__ P = true )
		{
			if ( P ) {
				Sync();
			}

			item_base_volatile__< item_multi_statique__<typename_ t::s>, r >::reset( P );

			Objet_.reset( false );
			Multimemoire.reset( P );

			Multimemoire.plug( item_base_volatile__< item_multi_statique__< typename_ t::s >, r >::Pilote_ );
			Objet_.plug( Multimemoire );
		}
		volatile_multi_item( void )
		: Objet_( item_base_volatile__< item_multi_statique__< typename_ t::s >, r >::ctn_S_ ),
		  Multimemoire( item_base_volatile__< item_multi_statique__< typename_ t::s >, r >::ctn_S_.Multimemoire )
		{
			reset( false );
		}
		// Remplace la fonction d'initialisation.
		volatile_multi_item( basic_container_< item_multi_statique__< typename t::s >, r > &Conteneur )
		: Objet_( item_base_volatile__< item_multi_statique__< typename_ t::s >, r >::ctn_S_ ),
		  Multimemoire( item_base_volatile__< item_multi_statique__< typename_ t::s >, r >::ctn_S_.Multimemoire )
		{
			reset( false );

			Multimemoire.Init();
			item_base_volatile__< item_multi_statique__< typename_ t::s >, r >::Init( Conteneur );
		}
		~volatile_multi_item( void )
		{
			reset( true );
		}
		//f Initialize with container 'Container', in mode 'Mode'.
		void Init(
			basic_container_< item_multi_statique__< typename t::s >, r > &Container,
			mdr::mode Mode = mdr::mReadWrite )
		{
			Multimemoire.Init();
			item_base_volatile__< item_multi_statique__< typename_ t::s >, r >::Init( Container, Mode );
		}
		volatile_multi_item &operator =( const volatile_multi_item &O )
		{
			ERRu();
		}
		//f Return the object at 'Position'.
		t &operator()( r Position )
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
	template <class t, typename r> class const_multi_item
	: public item_base_const__< item_multi_statique__<typename t::s>, r >
	{
	private:
		t Objet_;
	public:
		mmm::multimemory_ Multimemoire;
		void reset( bso::bool__ P = true )
		{
			if ( P ) {
				Sync();
			}

			item_base_const__< item_multi_statique__<typename_ t::s>, r >::reset(  P) ;

			Objet_.reset( false );
			Multimemoire.reset( P );

			Multimemoire.plug( item_base_const__< item_multi_statique__< typename_ t::s >, r >::Pilote_ );
			Objet_.plug( Multimemoire );
		}
		const_multi_item( void )
		: Objet_( item_base_const__< item_multi_statique__< typename_ t::s >, r >::ctn_S_ ),
		  Multimemoire( item_base_const__< item_multi_statique__< typename_ t::s >, r >::ctn_S_.Multimemoire )
		{
			reset( false );
		}
		virtual ~const_multi_item( void )
		{
			reset( true );
		}
		//f Initializing with container 'Container'.
		void Init( const basic_container_< item_multi_statique__<typename t::s>, r > &Container )
		{
			Multimemoire.Init();
			item_base_const__< item_multi_statique__< typename_ t::s >, r >::Init( Container );
		}
		const_multi_item &operator =( const const_multi_item &O )
		{
			ERRu();
		}
		//f Return the object at 'Position'.
		const t &operator()( r Position )
		{
			Sync( Position );
			return Objet_;
		}
		//f Return the object at current position.
		const t &operator()( void )
		{
			return Objet_;
		}
	};


	#define E_ITEMt( Type, r )		volatile_multi_item< Type, r >
	#define E_CITEMt( Type,r  )		const_multi_item< Type, r  >

#ifdef CPE__USE_GCC_WORKAROUND
	#define E_ITEM( Type )			volatile_multi_item< Type, epeios::row__ >
	#define E_CITEM( Type )			const_multi_item< Type, epeios::row__  >
#else
	#define E_ITEM( Type )			E_ITEMt( Type, epeios::row__ )
	#define E_CITEM( Type )			E_CITEMt( Type, epeios::row__ )
#endif

	/*c Container for objects 't', with static part 'st', which need more then one memory.
	Use 'CONTAINER_( t )' rather then directly this class.*/
	template <class t, typename r> class multi_container_
	: public basic_container_< item_multi_statique__< typename t::s >, r >
	{
	private:
		E_ITEMt( t, r ) Ponctuel_;
	public:
		struct s
		: public basic_container_< item_multi_statique__< typename t::s >, r >::s {};
		multi_container_( s &S )
		: basic_container_< item_multi_statique__< typename_ t::s >, r >( S )
		{
			Ponctuel_.Init( *this );
		}
		void reset( bool P = true )
		{
			Ponctuel_.Sync();
			basic_container_< item_multi_statique__< typename_ t::s >, r >::reset( P );
		}
		/*f Return the object at position 'Position'. BE CAREFUL: after calling this fonction
		and if you want to call another fonction as this fonction or the next, you MUST call
		the function 'Sync()' before. */
		t &Read( r Position )
		{
			return Ponctuel_( Position );
		}
		/*f Return the object at position 'Position'. BE CAREFUL: after calling this fonction
		and if you want to call another fonction as this fonction or the next, you MUST call
		the function 'Sync()' before. */
		t &operator ()( r Position )
		{
			return Read( Position );
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
		const t& Read(
			r Position,
			E_CITEMt( t, r ) &Item ) const
		{
			Item.Init( *this );

			return Item( Position );
		}
		/*f Return the object at 'Position' using 'Item'.
		Valid only until next modification of 'Item'. */
		const t& operator()(
			r Position,
			E_CITEMt( t, r ) &Item ) const
		{
			return Read( Position, Item );
		}
		//f Write 'Object' at 'Position'.
		void Write(
			const t & Object,
			r Position )
		{
			operator()( Position ) = Object;

			Sync();
		}
		//f Put in 'Object' the object at position 'Position'.
		void Read(
			r Position,
			t &Objet ) const
		{
			E_CITEMt( t, r ) Element;

			Element.Init( *this );

			Objet = Element( Position );
		}
		//f Allocate room for 'Capacity' objects.
		void Allocate(
			epeios::size__ Capacity,
			aem::mode Mode = aem::mDefault )
		{
			E_ITEMt( t, r ) E;

			basic_container_< item_multi_statique__< typename_ t::s >, r >::Allocate( Capacity, E.ctn_S_, Mode );// pas de E.xxx::ctn_S_ car G++ V2.90.29 n'aime pas
		}
		multi_container_ &operator =( const multi_container_ &C )
		{
			basic_container_< item_multi_statique__< typename_ t::s >, r >::operator =( C );

			return *this;
		}
		//f Create a new object and return its position.
		r New( epeios::size__ Size = 1 )
		{
			epeios::row_t__ P = this->Amount();

			Allocate( P + Size );

			return P;
		}
	};

	AUTO2( multi_container )

	//s Container of object of type 'Type', which need more then one memory.
	#define E_CONTAINERt_( Type, r ) multi_container_< Type, r >
	#define E_CONTAINERt( Type, r ) multi_container< Type, r >

#ifdef CPE__USE_GCC_WORKAROUND
	#define E_CONTAINER_( Type )	multi_container_< Type, epeios::row__ >
	#define E_CONTAINER( Type )		multi_container< Type, epeios::row__ >
#else
	#define E_CONTAINER_( Type )	E_CONTAINERt_( Type, epeios::row__ )
	#define E_CONTAINER( Type )		E_CONTAINERt( Type, epeios::row__ )
#endif

	//c Same as multi_container_, but for use with object which have a 'Init()' function without parameters.
	template <class t, typename r> class multi_extended_container_
	: public multi_container_< t, r >
	{
	private:
		// Create an initialized object. Return position of the created object.
		r Create_( void )
		{
			r P = multi_container_< t, r >::New();

			multi_container_< t, r >::operator()( P ).Init();

			return P;
		}
	public:
		struct s
		: public multi_container_< t, r >::s
		{};
		multi_extended_container_( s &S )
		: multi_container_< t, r >( S )
		{}
		void reset( bool P = true )
		{
			multi_container_< t, r >::reset( P );
		}
		void plug( mmm::multimemory_ &M )
		{
			multi_container_< t, r >::plug( M );
		}
		multi_extended_container_ &operator =( const multi_extended_container_ &C )
		{
			multi_container_< t, r >::operator =( C );
			
			return *this;
		}
		//f Initialization.
		void Init( void )
		{
			multi_container_< t, r >::Init();
		}
		//f Create an initialized object. Return position of the created object.
		r Create( void )
		{
			r P = Create_();

			multi_container_< t, r >::Sync();

			return P;
		}
		//f Adding object 'Object'. Return position where added.
		r Add( const t &Object )
		{
			r P = Create_();

			multi_container_< t, r >::operator()() = Object;	// Positionned by 'Create_'.

			multi_container_< t, r >::Sync();

			return P;
		}
		//f Remove 'Amount' objects from the end of the container.
		void Truncate( epeios::size__ Amount = 1 )
		{
	#ifdef CTN_DBG
			if ( Amount > this->Amount() )
				ERRu();
	#endif
			Allocate( this->Amount() - Amount );
		}
	};


	AUTO2( multi_extended_container )

	//s Container of object of type 'Type', and which have a 'Init()' function without parameters.
	#define E_XCONTAINERt_( Type, r ) multi_extended_container_< Type, r >
	#define E_XCONTAINERt( Type, r ) multi_extended_container< Type, r >

#ifdef CPE__USE_GCC_WORKAROUND
	 #define E_XCONTAINER_( Type )	multi_extended_container_< Type, epeios::row__ >
	 #define E_XCONTAINER( Type )	multi_extended_container< Type, epeios::row__ >
#else	
	 #define E_XCONTAINER_( Type )	E_XCONTAINERt_( Type, epeios::row__ )
	 #define E_XCONTAINER( Type )	E_XCONTAINERt( Type, epeios::row__ )
#endif

}

#undef typename_

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
