/*
  Header for the 'broker' library by Claude L. Simon (epeios@epeios.org)
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

#ifndef BROKER__INC
#define BROKER__INC

#define BROKER_NAME		"BROKER"

#define	BROKER_VERSION	"$Revision$"	

#define BROKER_OWNER		"the Epeios project (http://www.epeios.org/)"

#include "ttr.h"

extern class ttr_tutor &BROKERTutor;

#if defined( XXX_DBG ) && !defined( BROKER_NODBG )
#define BROKER_DBG 
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

/* Addendum to the automatic documentation generation part. */
//D BRoKer.
/* End addendum to automatic documentation generation part. */


#include "err.h"
#include "flw.h"
#include "lst.h"
#include "lgg.h"
#include "brkcst.h"
#include "brkcmd.h"
#include "brkrqm.h"

//d An invalid value for a command.
#define BROKER_INVALID_COMMAND		BRKRQM_INVALID_COMMAND
#define BROKER_COMMAND_MAX			BRKRQM_COMMAND_MAX

//d An invalid value for a type.
#define BROKER_INVALID_TYPE			BRKRQM_INVALID_TYPE
#define BROKER_TYPE_MAX				BRKRQM_TYPE_MAX

//d An invalid value for an object.
#define BROKER_INVALID_OBJECT		BRKRQM_INVALID_OBJECT
#define BROKER_OBJECT_MAX			BRKRQM_OBJECT_MAX

#define BROKER_MASTER_OBJECT		BROKER_INVALID_OBJECT

#define BROKER_MASTER_TYPE		 	BROKER_INVALID_TYPE

#define BROKER_MASTER_COMMAND		BROKER_INVALID_COMMAND


namespace broker {
	using namespace brkrqm;
	using tym::memory;

	typedef tym::row__ index__;
	typedef brktpm::id16_t__	command__;

	typedef brktpm::id16_t__	type_t__;
	TYPEDEF( type_t__, type__ );

	using brktpm::object_t__;
	using brktpm::object__;

	typedef void (* function__ )(
		class broker &Broker,
		class untyped_module &Module,
		command__ Command,
		request_manager___ &Request,
		bso::bool__ &,
		void *UP );

	//c An untyped module.
	class untyped_module
	{
	private:
		// Libelle du type de l'objet, du module.
		const char *Name_;
		// L'interface auquel le module est rattaché.
		class broker *Broker_;
	protected:
		//v To get the index of a new object.
		virtual index__ BROKERNew( void )
		{
			ERRb();
			return 0;	// Pour éviter un warning.
		}
		//v To remove the object with index 'Index'.
		virtual void BROKERRemove( index__ Index )
		{
			ERRb();
		}
		//v To get a pointer of the object of index 'Index'.
		virtual void *BROKERObject( index__ Index )
		{
			ERRb();
			return NULL;	// Pour éviter un 'warning'
		}
		// Fonction appelée pour traiter la requête 'Requete' pour l'objet d'index 'Index'.
		virtual void Handle_(
			index__ Index,
			request_manager___ &Requete,
			void *PU ) = 0;
	public:
		//r The description of the request.
		descriptions Descriptions;
		//r User pointers.
		bch::E_BUNCH( const void *) UPs;
		untyped_module( void )
		{
			Name_ = NULL;
			Broker_ = NULL;
		}
		//f Give the index of a new object.
		index__ New( void )
		{
			return BROKERNew();
		}
		//f Remove the object of index 'Index'.
		void Remove( index__ Index )
		{
			BROKERRemove( Index );
		}
		//f Give the name of the objects.
		const char *Name( void ) const
		{
			return Name_;
		}
		//f Give an pointer to the object of index 'Index'.
		void *Object( index__ Index )
		{
			return BROKERObject( Index );
		}
		//f Initialization with the object name 'Name'.
		void Init( const char *Name )
		{
			Descriptions.Init();
			UPs.Init();

			Name_ = Name;
		}
		//f Handle the request 'Request' for object of index 'Index'.
		void Handle(
			index__ Index,
			request_manager___ &Requete,
			void *UP )
		{
			Handle_( Index, Requete, UP );
		}
		//f Return the broker attached to this module.
		broker *Broker( void )
		{
			return Broker_;
		}
		//f Return the command which has 'Description' as description, or 'BROKER_INVALID_COMMAND' if non-existant.
		command__ Command( const description_ &Description ) const
		{
			tym::row__ P = Descriptions.Position( Description );

			if ( P == NONE )
				P = BROKER_INVALID_COMMAND;
			else if ( P.V > BROKER_COMMAND_MAX )
				ERRl();

			return (command__)P.V;
		}
		tym::row__ Add(
			const char *Name,
			const cast *Casts,
			const void *UP )
		{
			tym::row__ P = Descriptions.Add( Name, Casts );
			
			if ( UPs.Add( UP ) != P )
				ERRc();
				
			return P;
		}			
		tym::row__ Add(
			const char *Name,
			const void *UP,
			... )
		{
			return Add( Name, (cast *)(&UP + 1), UP );
		}
		friend class broker;
		friend class master_module;
	};

	//c A module for an object of type 't'.
	template <class t> class module
	: public untyped_module
	{
	private:
		void Traiter_(
			t &Objet,
			brkrqm::request_manager___ &Requete,
			void *PU )
		{
			command__ C;

			flw::Get( Requete.Input(), C );

			if ( C > Descriptions.Amount() )
				ERRu();

			Requete.SetDescription( Descriptions( C ) );

			Objet.HANDLE( *Broker(), *this, C, Requete, PU );
		}
	protected:
		virtual void Handle_(
			index__ Index,
			brkrqm::request_manager___ &Requete,
			void *PU )
		{
			Traiter_( *(t *)untyped_module::Object( Index ), Requete, PU );
		}
	public:
		//f Initialization.
		void Init( void )
		{
			untyped_module::Init( t::NAME );

			t::NOTIFY( *this );
		}
	};

	//c A module with object stored in RAM.
	template <class t, class st> class ram_module
	: public module<t>,
	  private lst::E_LIST
	{
	protected:
		virtual void LSTAllocate( tym::size__ Size )
		{
			Objets.Allocate( Size, aem::mFit );
		}
		virtual index__ BROKERNew( void )
		{
			st *S = NULL;
			t *Pointeur = NULL;

			if ( ( S = new st ) == NULL )
				ERRm();

			if ( ( Pointeur = new t( *S ) ) == NULL )
			{
				delete S;
				ERRm();
			}

			index__ Index = list::CreateEntry();

			Objets.Write( Pointeur, Index );

			return Index;
		}
		virtual void BROKERRemove( index__ Index )
		{
			delete Objets( Index );
			list::Remove( Index );
		}
		virtual void *BROKERObject( index__ Index )
		{
			if ( Index.V >= Objets.Amount() )
				ERRu();

			return (void *)Objets( Index );
		}
	public:
		//r The pointer object.
		bch::E_BUNCH( t * ) Objets;
		//f Initialization.
		void Init( void )
		{
			list::Init();
			Objets.Init();
			module<t>::Init();
		}
	};
	
	//c A module with object stored in standard memory.
	template <class t, class st> class standard_module
	: public module<t>,
	  private lst::E_LIST
	{
	private:
		ctn::E_ITEM( t ) Element_;
	protected:
		virtual void LSTAllocate( tym::size__ Size )
		{
			Element_.Sync();
			Objets.Allocate( Size, aem::mFit );
		}
		virtual index__ BROKERNew( void )
		{
			Element_.Sync();
			return list::CreateEntry();
		}
		virtual void BROKERRemove( index__ Index )
		{
			Element_( Index ).reset();
			list::Remove( Index );
			Element_.Sync();
		}
		virtual void *BROKERObject( index__ Index )
		{
			if ( Index.V >= Objets.Amount() )
				ERRu();

			return (void *)&Element_( Index );
		}
	public:
		//r Contient les objets.
		ctn::E_CONTAINER( t ) Objets;
		void reset( bool P = true )
		{
			Element_.Sync();
			Objets.reset( P );
			list::reset( P );
		}
		standard_module( void )
		{
			reset( false );
		}
		~standard_module( void )
		{
			reset( true );
		}
		// Initialisation.
		void Init( void )
		{
			lst::E_LIST::Init();
			Objets.Init();
			Element_.Init( Objets );
			untyped_module::Init( t::NAME );
			module<t>::Init();
		}
	};

	// Module maître, qui fait tout le boulot.
	class master_module
	: public untyped_module
	{
	private:
		// Si vrai, une erreur arrête le thread, sinon celui-ci se poursuit.
		bso::bool__ ErrorBreaking_;
	protected:
		virtual void Handle_(
			index__ Index,
			request_manager___ &Requete,
			void *PU );
	public:
		master_module( void )
		{
			ErrorBreaking_ = true;
		}
		// Initialisation avec rattachement à l'interface 'Broker'.
		void Init( broker &Broker );
		bso::bool__ ErrorBreaking( void  )
		{
			return ErrorBreaking_;
		}
		void ErrorBreaking( bso::bool__ Value )
		{
			ErrorBreaking_ = Value;
		}
	};

	struct link__
	{
		// Le type de l'objet
		type__ Type;
		// L'index de l'objet;
		index__ Index;
	};

	//t To by-pass a visual C++ bug.
	typedef lst::E_LIST	list;

	// Classe de gestion des liens entre module et objets.
	class links
	: private tym::E_MEMORY( link__ ),
	  private lst::E_LIST
	{
	protected:
		void LSTAllocate( tym::size__ Size )
		{
			E_MEMORY( link__ )::Allocate( Size );
		}
	public:
		// Initialisation.
		void Init( void )
		{
			list::Init();
			E_MEMORY( link__ )::Init();
		}
		object__ New(
			type__ IdType,
			index__ Index )
		{
			link__ Lien;
			object__ IdObjet;
			tym::row__ P;
			
			P = list::CreateEntry();

			if ( P.V > BROKER_TYPE_MAX )
				ERRl();

			IdObjet = (object_t__)P.V;

			Lien.Type = IdType;
			Lien.Index = Index;

			E_MEMORY( link__ )::Write( Lien, IdObjet.V );

			return IdObjet;
		}
		void Remove( object__ IdObjet )
		{
			list::Remove( IdObjet.V );
		}
		type__ Type( object__ IdObjet ) const
		{
			return E_MEMORY( link__ )::Read( IdObjet.V ).Type;
		}
		index__ Index( object__ IdObjet ) const
		{
			return E_MEMORY( link__ )::Read( IdObjet.V ).Index;
		}
		tym::size__ Amount( void )
		{
			return list::Amount();
		}
	};


	//c A broker, which handles objects od different type and request to this object.
	class broker
	{
	private:
		master_module Master_;
		// Retourne le module correspondant à 'IdType'.
		untyped_module &Module_( type__ IdType ) const
		{
			if ( IdType != BROKER_MASTER_TYPE )
				return *Modules( IdType.V );
			else
				return (untyped_module &)Master_;	// Not very happy about this conversion, 
		}
		// Retourne le module correspondant à 'IdObjet'.
		untyped_module &Module_( object__ IdObjet ) const
		{
			if ( IdObjet != BROKER_MASTER_OBJECT )
				return Module_( Type_( IdObjet ) );
			else
				return (untyped_module &)Master_; // Not very happy about this conversion, 
		}
		// Retourne le type correpondant à l'objet d'indetificateur 'IdObjet'.
		type__ Type_( object__ IdObjet ) const
		{
			return Links.Type( IdObjet );
		}
		// Retourne l'indexcorrespondant à l'objet d'identificateur 'IdObjet'.
		index__ Index_( object__ IdObjet ) const
		{
			return Links.Index( IdObjet );
		}
	public:
		// Contient le libellé de la version du serveur d'objet. Usage interne.
		const char *Version_;
		// Contient les informations sur les langues supportées. Usage interne.
		struct
		{
			// Nombre de langue supportée.
			bso::ushort__ Nombre;
			// Les identificateurs des langues supportées.
			const lgg::language *Identificateurs;
			// La langue courante.
			lgg::language Courante;
		} Langues_;
		//o The different modules.
		bch::E_BUNCH( untyped_module * ) Modules;
		//o The relation between modules an index.
		links Links;
		broker( void )
		{
			Langues_.Nombre = 0;
			Langues_.Identificateurs = NULL;
			Langues_.Courante = lgg::lDefault;

			Version_ = "";
		}
		//f Initialization.
		void Init( void )
		{
			Master_.Init( *this );

			Modules.Init();
			Links.Init();
		}
		//f Add 'Module' to the interface.
		void Add( untyped_module &Module )
		{
			Module.Broker_ = this;
			Modules.Add( &Module );
		}
		/*f Handle the request which come by 'Channel' and write the answer to 'Channel'.
		If 'true' is returned, than the request contains a deconnection request. */
		bso::bool__ Handle(
			flw::ioflow___ &Channel,
			void *PU = NULL );
		/*f Return the command corresponding at request description 'Description' and
		object type 'Type'. 'BROKER_INVALID_COMMAND' is returned if command not found. */
		command__ Command(
			type__ IdType,
			const description_ &Description )
		{
			return Module_( IdType ).Command( Description );
		}
		//f Return the error breaking status.
		bso::bool__ ErrorBreaking( void  )
		{
			return Master_.ErrorBreaking();
		}
		//f Set to 'Value' the error breaking status.
		void ErrorBreaking( bso::bool__ Value )
		{
			Master_.ErrorBreaking( Value );
		}
		//f Give the module for the object of type 'Type'.
		const untyped_module &Module( type__ Type ) const
		{
			return Module_( Type );
		}
		//f Give the module for object 'Object'.
		const untyped_module &Module( object__ Object ) const
		{
			return Module_( Object );
		}
		//f Return a pointer to the 'Object' object.
		const void *Object( object__ Object )
		{
			return Module_( Object ).Object( Links.Index( Object ) );
		}
		//f Give a new object.
		object__ New( type__ Type )
		{
			if ( (unsigned long)Links.Amount() >= (unsigned long)BROKER_TYPE_MAX )
				ERRl();

			return Links.New( Type, Module_( Type ).New() );
		}
		//f Give the type of the object 'Object'.
		type__ Type( object__ Object)
		{
			return Type_( Object );
		}
		//f Give the type of a nobject named 'Name'.
		type__ Type( str::string_ &Name );
		//f Remove object 'Object'.
		void Remove( object__ Object )
		{
			Module_( Object ).Remove( Links.Index( Object ) );
		}
		/*f Tell that 'Amount' language indicated in 'Languages' are supported.
		'Languages' is not copied and should NOT be modified. */
		void Languages(
			bso::ushort__ Amount,
			const lgg::language *Languages )
		{
			Langues_.Nombre = Amount;
			Langues_.Identificateurs = Languages;
		}
		/*f Tell that version is the current version. 'Version' is not copied
		and should NOT be modified. */
		void Version( const char *Version )
		{
			Version_ = Version;
		}
		//f Return the current language.
		lgg::language Language( void )
		{
			return Langues_.Courante;
		}
		/*f 'Language' brcomes the current language. 'Language' must, of course' be supported. */
		void Langue( lgg::language Langue )
		{
			Langues_.Courante = Langue;
		}
		/*f Add a request descrption with name 'Name', function pointer 'FP'
		and a list of casts 'Casts'. The list must contain 2 'cEnd', the first
		at the end of the parameters casts,	and 1 of the end of returned values casts. */
		tym::row__ Add(
			const char *Name,
			function__ FP,
			const cast *Casts )
		{
			return Master_.Add( Name, Casts, (void *)FP );
		}
		tym::row__ Add(
			const char *Name,
			function__ FP,
			cast Cast,	// Added to avoid confusion with function above.
			... )
		{
			return Add( Name, FP, &Cast );
		}

	};

	typedef broker broker_;

};

//d A ram module of an object of type 't'.
#define BROKER_RAM_MODULE( t )	::broker::ram_module<t, t::s>	

//d A standard module of an object of type 't'.
#define BROKER_STANDARD_MODULE( t )	::broker::standard_module<t, t::s>	


/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
