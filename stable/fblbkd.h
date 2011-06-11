/*
	Header for the 'fblbkd' library by Claude SIMON (csimon at zeusw dot org)
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

#ifndef FBLBKD__INC
#define FBLBKD__INC

#define FBLBKD_NAME		"FBLBKD"

#define	FBLBKD_VERSION	"$Revision$"

#define FBLBKD_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &FBLBKDTutor;

#if defined( XXX_DBG ) && !defined( FBLBKD_NODBG )
#define FBLBKD_DBG
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
//D Frontend/Backend Layout BaKenD 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# include "err.h"
# include "flw.h"
# include "lcl.h"
# include "tht.h"

# include "fblbrq.h"
# include "fbltyp.h"
# include "fblbur.h"

# ifdef FBLBKD_DEFAULT_LANGUAGE
#  define FBLBKD__DEFAULT_LANGUAGE	FBLBKD_DEFAULT_LANGUAGE
# else
#  define FBLBKD__DEFAULT_LANGUAGE "fr"
# endif

//d An invalid value for a command.
# define FBLBKD_INVALID_COMMAND		FBLBRQ_INVALID_COMMAND
# define FBLBKD_COMMAND_MAX			FBLBRQ_COMMAND_MAX

//d An invalid value for a type.
# define FBLBKD_INVALID_TYPE		FBLBRQ_INVALID_TYPE
# define FBLBKD_TYPE_MAX			FBLBRQ_TYPE_MAX

//d An invalid value for an object.
# define FBLBKD_INVALID_OBJECT		FBLBRQ_INVALID_OBJECT
# define FBLBKD_OBJECT_MAX			FBLBRQ_OBJECT_MAX

# define FBLBKD_MASTER_OBJECT		FBLBKD_INVALID_OBJECT

# define FBLBKD_MASTER_TYPE		 	FBLBKD_INVALID_TYPE

# define FBLBKD_MASTER_COMMAND		FBLBKD_INVALID_COMMAND

//d This is the value of the index returned for a shared object already created.
# define FBLBKD_SHARED_OBJECT_INDEX	NONE

//d Value for an undefined Id32.
# define FBLBKD_UNDEFINED_ID32		FBLTYP_UNDEFINED_ID32

//d Value for an undefined Id16.
# define FBLBKD_UNDEFINED_ID16		FBLTYP_UNDEFINED_ID16

//d Value for an undefined Id8.
# define FBLBKD_UNDEFINED_ID8		FBLTYP_UNDEFINED_ID8

namespace fblbkd {
	using namespace fblbrq;
	using namespace fbltyp;

	typedef lcl::strings_	messages_;
	E_AUTO( messages )

	typedef epeios::row__ index__;
	typedef fbltyp::id16_t__	command__;

	typedef fbltyp::id16_t__	type_t__;
	E_TYPEDEF( type_t__, type__ );

	using fbltyp::object_t__;
	using fbltyp::object__;

	class log_functions__
	{
	protected:
		virtual void FBLBKDLog(
			const char *ObjectPrefix,
			const char *ObjectName,
			const str::string_ &MethodName,
			bso::bool__ Exit ) = 0;
	public:
		void Log(
			const char *ObjectPrefix,
			const char *ObjectName,
			const str::string_ &MethodName,
			bso::bool__ Exit )
		{
			FBLBKDLog( ObjectPrefix, ObjectName, MethodName, Exit );
		}
		void reset( bso::bool__ = true )
		{
			// Standardisation.
		}
		log_functions__( void )
		{
			reset( false );
		}
		~log_functions__( void )
		{
			reset();
		}
		void Init( void )
		{
			// Standardisation.
		}
	};

	class text_log_functions__
	: public log_functions__
	{
	private:
		txf::text_oflow__ *_TOFlow;
	protected:
		virtual void FBLBKDLog(
			const char *ObjectPrefix,
			const char *ObjectName,
			const str::string_ &MethodName,
			bso::bool__ Exit )
		{
			tol::buffer__ Buffer;
			*_TOFlow << "[" << tol::DateAndTime( Buffer ) << "] (" << tht::GetTID() << ") : " << ( Exit ? "<-- " : "--> " ) << ObjectPrefix << ':' << ObjectName << ':' << MethodName << txf::nl << txf::commit;
		}
	public:
		void reset( bso::bool__ P = true )
		{
			log_functions__::reset( P );
			_TOFlow = NULL;
		}
		text_log_functions__( void )
		{
			reset( false );
		}
		~text_log_functions__( void )
		{
			reset();
		}
		void Init( txf::text_oflow__ &TOFlow )
		{
			reset();

			log_functions__::Init();
			_TOFlow = &TOFlow;
		}
	};


	typedef void (* function__ )(
		class backend &Backend,
		class untyped_module &Module,
		index__ Index,
		command__ Command,
		request__ &Request,
		bso::bool__ &,
		void *UP );

	//c An untyped module.
	class untyped_module
	{
	private:
		// Prefix for the module/object name.
		const char *Prefix_;
		// Libelle du type de l'objet, du module.
		const char *Name_;
		// L'interface auquel le module est rattach�.
		class backend *Backend_;
		void _Clean( void );	// Permet l'effacement correct de chaque objet utilisateur.
	protected:
		//v To get the index of a new object.
		virtual index__ FBLBKDNew( void )
		{
			ERRb();
			return 0;	// Pour �viter un warning.
		}
		//v To delete the object with index 'Index'.
		virtual void FBLBKDDelete( index__ Index )
		{
			ERRb();
		}
		//v To get a pointer of the object of index 'Index'.
		virtual void *FBLBKDObject( index__ Index )
		{
			ERRb();
			return NULL;	// Pour �viter un 'warning'
		}
#if 0
		//v To get the raw messages.
		virtual void FBLBKDGetRawMessages( messages_ &Messages ) = 0;
#endif
		// Fonction appel�e pour traiter la requ�te 'Requete' pour l'objet d'index 'Index'.
		virtual void Handle_(
			index__ Index,
			request__ &Requete,
			void *PU,
			log_functions__ &LogFunctions ) = 0;
	public:
		//r The description of the request.
		fblbrd::descriptions Descriptions;
		//r User pointers.
		bch::E_BUNCH( const void *) UPs;
		bch::E_BUNCH( index__ ) Indexes;
		void reset( bso::bool__ P = true )
		{
			if ( P ) {
				_Clean();
			}

			Prefix_ = NULL;
			Name_ = NULL;
			Backend_ = NULL;

			Descriptions.reset( P );
			UPs.reset( P );
			Indexes.reset( P );
		}
		untyped_module( void )
		{
			reset( false );
		}
		~untyped_module( void )
		{
			reset();
		}
		//f Give the index of a new object.
		index__ New( void )
		{
			index__ Index = FBLBKDNew();

			Indexes.Append( Index );

			return Index;
		}
		//f Delete the object of index 'Index'.
		void Delete( index__ Index )
		{
			epeios::row__ Row = Indexes.Search( Index );

			if ( Row == NONE )
				ERRu();

			Indexes.Remove( Index );

			FBLBKDDelete( Index );
		}
		//f Give the name of the object.
		const char *Name( void ) const
		{
			return Name_;
		}
		//f Give the prefix of the object.
		const char *Prefix( void ) const
		{
			return Prefix_;
		}
		//f Give an pointer to the object of index 'Index'.
		void *Object( index__ Index )
		{
			return FBLBKDObject( Index );
		}
		//f Get the raw messages.
#if 0
		void GetRawMessages( messages_ &RawMessages )
		{
			FBLBKDGetRawMessages( RawMessages );
		}
#endif
		/*f Initialization with the object prefix 'Prefix' and name 'Name'.
		'Prefix' and 'Name' are not duplicated. They should NOT be modified. */
		void Init(
			const char *Prefix,
			const char *Name )
		{
			reset();

			Descriptions.Init();
			UPs.Init();
			Indexes.Init();

			Prefix_ = Prefix;
			Name_ = Name;
		}
		//f Handle the request 'Request' for object of index 'Index'.
		void Handle(
			index__ Index,
			request__ &Requete,
			void *UP,
			log_functions__ &LogFunctions )
		{
			Handle_( Index, Requete, UP, LogFunctions );
		}
		//f Return the backend attached to this module.
		backend *Backend( void )
		{
			return Backend_;
		}
		//f Return the command which has 'Description' as description, or 'FBLBKD_INVALID_COMMAND' if non-existant.
		command__ Command( const fblbrd::description_ &Description ) const
		{
			epeios::row__ P = Descriptions.Position( Description );

			if ( P == NONE )
				P = FBLBKD_INVALID_COMMAND;
			else if ( *P > FBLBKD_COMMAND_MAX )
				ERRl();

			return (command__)*P;
		}
		epeios::row__ Add(
			const char *Name,
			const cast *Casts,
			const void *UP )
		{
			epeios::row__ P = Descriptions.Add( Name, Casts );
			
			if ( UPs.Append( UP ) != P )
				ERRc();
				
			return P;
		}			
		epeios::row__ Add(
			const char *Name,
			const void *UP,
			... )
		{
			return Add( Name, (cast *)(&UP + 1), UP );
		}
		friend class backend;
		friend class master_module;
	};

	//c A module for an object of type 't'.
	template <class t> class module
	: public untyped_module
	{
	private:
		void Traiter_(
			t &Objet,
			index__ Index,
			fblbrq::request__ &Requete,
			void *PU,
			log_functions__ &LogFunctions )
		{
			command__ C;
			ctn::E_CITEM( fblbrd::description_ ) Description;

			flw::Get( Requete.Input(), C );

			if ( C > Descriptions.Amount() )
				ERRu();

			Description.Init( Descriptions );

			Requete.Prepare( Description( C ).Casts );

			if ( &LogFunctions != NULL )
				LogFunctions.Log( Prefix(), Name(), Description( C ).Name, false );

			Objet.HANDLE( *Backend(), *this, Index, C, Requete, PU );

			if ( &LogFunctions != NULL )
				LogFunctions.Log( Prefix(), Name(), Description( C ).Name, true );

		}
	protected:
#if 0
		virtual void FBLBKDGetRawMessages( messages_ &Messages )
		{
			t::RAW_MESSAGES( Messages );
		}
#endif
		virtual void Handle_(
			index__ Index,
			fblbrq::request__ &Requete,
			void *PU,
			log_functions__ &LogFunctions )
		{
			Traiter_( *(t *)untyped_module::Object( Index ), Index, Requete, PU, LogFunctions );
		}
	public:
		//f Initialization.
		void Init( void )
		{
			untyped_module::Init( t::PREFIX, t::NAME  );

			t::NOTIFY( *this );
		}
	};
	
	//c A module with object stored in RAM.
	template <class t, class st> class ram_module
	: public module<t>,
	  private lst::E_LIST
	{
	private:
		const lst::E_LIST_ &_List( void ) const
		{
			return *this;
		}
		lst::E_LIST_ &_List( void )
		{
			return *this;
		}
	protected:
		virtual void LSTAllocate(
			epeios::size__ Size,
			aem::mode__ Mode )
		{
#ifdef FBLBKD_DBG
			if ( Mode != aem::m_Default )
				ERRu();
#endif
			Objets.Allocate( Size, aem::mFit );
		}
		virtual index__ FBLBKDNew( void )
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

			Pointeur->reset( false );

			index__ Index = _List().New();

			Objets.Store( Pointeur, Index );

			return Index;
		}
		virtual void FBLBKDDelete( index__ Index )
		{
			t *Object = Objets( Index );

			Object->reset();

			delete Object;

			_List().Delete( Index );
		}
		virtual void *FBLBKDObject( index__ Index )
		{
			if ( *Index >= Objets.Amount() )
				ERRu();

			return (void *)( Objets( Index )->OBJECT() );
		}
	public:
		//r The pointer object.
		bch::E_BUNCH( t * ) Objets;
		//f Initialization.
		void Init( void )
		{
			_List().Init();
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
		lst::E_LIST &_List( void )
		{
			return *this;
		}
		const lst::E_LIST &_List( void ) const
		{
			return *this;
		}
	protected:
		virtual void LSTAllocate( epeios::size__ Size )
		{
			Element_.Commit();
			Objets.Allocate( Size, aem::mFit );
		}
		virtual index__ FBLBKDNew( void )
		{
			Element_.Commit();
			return _List().New();
		}
		virtual void FBLBKDDelete( index__ Index )
		{
			Element_( Index ).reset();
			_List().Delete( Index );
			Element_.Commit();
		}
		virtual void *FBLBKDObject( index__ Index )
		{
			if ( *Index >= Objets.Amount() )
				ERRu();

			return (void *)Element_( Index ).OBJECT();
		}
	public:
		//r Contient les objets.
		ctn::E_CONTAINER( t ) Objets;
		void reset( bool P = true )
		{
			Element_.Commit();
			Objets.reset( P );
			_List().reset( P );
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
			_List().Init();
			Objets.Init();
			Element_.Init( Objets );
			module<t>::Init();
		}
	};

	//c A shared module. One instance for all.
	template <class t, class st> class shared_module
	: public module<t>
	{
	private:
		t T_;
		st ST_;
		bso::bool__ Created_;
	protected:
		virtual index__ FBLBKDNew( void )
		{
			if ( Created_ )
				return FBLBKD_SHARED_OBJECT_INDEX;
			else {
				Created_ = true;
				return 0;
			}
		}
		virtual void FBLBKDDelete( index__ Index )
		{
		}
		virtual void *FBLBKDObject( index__ Index )
		{
			return T_.OBJECT();
		}
	public:
		void reset( bool P = true )
		{
			T_.reset( P );
			Created_ = false;
		}
		shared_module( void )
		: T_( ST_ )
		{
			reset( false );
		}
		~shared_module( void )
		{
			reset( true );
		}
		// Initialisation.
		void Init( void )
		{
			reset();
			module<t>::Init();
			Created_ = false;
		}
	};

	// Module ma�tre, qui fait tout le boulot.
	class master_module
	: public untyped_module
	{
	protected:
		virtual void Handle_(
			index__ Index,
			fblbrq::request__ &Requete,
			void *PU,
			log_functions__ &LogFunctions );
#if 0
		virtual void FBLBKDGetRawMessages( messages_ &RawMessages )
		{
			RawMessages = this->RawMessages;
		}
#endif
	public:
#if 0
		//o All raw messages from all modules.
		messages RawMessages;
#endif
		// Initialisation avec rattachement � l'interface 'Backend'.
		void Init( backend &Backend );
	};

	struct link__
	{
		// Le type de l'objet
		type__ Type;
		// L'index de l'objet;
		index__ Index;
	};

	//t To by-pass a visual C++ bug.
	typedef lst::E_LISTtx( object__, object_t__ ) _list;
	typedef tym::E_MEMORYt( link__, object__ ) _memory;

	// Classe de gestion des liens entre module et objets.
	class links
	: private _memory,
	  public _list
	{
	protected:
		void LSTAllocate(
			epeios::size__ Size,
			aem::mode__ Mode )
		{
			_memory::Allocate( Size );
		}
	public:
		// Initialisation.
		void Init( void )
		{
			_list::Init();
			_memory::Init();
		}
		object__ New(
			type__ IdType,
			index__ Index )
		{
			link__ Lien;
			object__ P;
			
			P = _list::New();

			if ( *P > FBLBKD_TYPE_MAX )
				ERRl();

			Lien.Type = IdType;
			Lien.Index = Index;

			_memory::Store( Lien, P );

			return P;
		}
		void Delete( object__ IdObjet )
		{
			_list::Delete( IdObjet );
		}
		type__ Type( object__ IdObjet ) const
		{
			return _memory::Get( IdObjet ).Type;
		}
		index__ Index( object__ IdObjet ) const
		{
			return _memory::Get( IdObjet ).Index;
		}
		epeios::size__ Amount( void ) const
		{
			return _list::Amount();
		}
	};


	//c A backend, which handles objects od different type and request to this object.
	class backend
	{
	private:
		bso::bool__ _CompatibilityTested;
		fblbur::mode__ _Mode;
		const char *_ClientOrigin;
		const char *_APIVersion;
		master_module Master_;
		STR_BUFFER___ _Language;
		lcl::rack__ _LocaleRack;
		const char *_TargetLabel;
		// Informations � propos du 'backend'.
		const char *_BackendInformations;
		// Information � propos du publicateur (l'ex�cutable ou la biblioth�que) du 'backend'.
		const char *_PublisherInformations;
		// Retourne le module correspondant � 'IdType'.
		untyped_module &Module_( type__ IdType ) const
		{
			if ( IdType != FBLBKD_MASTER_TYPE )
				return *Modules( *IdType );
			else
				return (untyped_module &)Master_;	// Not very happy about this conversion, 
		}
		// Retourne le module correspondant � 'IdObjet'.
		untyped_module &Module_( object__ IdObjet ) const
		{
			if ( IdObjet != FBLBKD_MASTER_OBJECT )
				return Module_( Type_( IdObjet ) );
			else
				return (untyped_module &)Master_; // Not very happy about this conversion, 
		}
		// Retourne le type correpondant � l'objet d'indetificateur 'IdObjet'.
		type__ Type_( object__ IdObjet ) const
		{
			return Links.Type( IdObjet );
		}
		// Retourne l'indexcorrespondant � l'objet d'identificateur 'IdObjet'.
		index__ Index_( object__ IdObjet ) const
		{
			return Links.Index( IdObjet );
		}
		bso::bool__ _TestCompatibility(
			flw::ioflow__ &Flow,
			const char *APIVersion,
			const char *MessageLabel,
			const char *URLLabel );
		bso::bool__ _TestCompatibility( flw::ioflow__ &Flow );
		bso::bool__ _HandleRequest(
			flw::ioflow__ &FrontendFlow,
			void *PU,
			log_functions__ &LogFunctions );
	public:
		//o The different modules.
		bch::E_BUNCH( untyped_module * ) Modules;
		//o The relation between modules an index.
		links Links;
		backend( void )
		{
			_CompatibilityTested = false;
			_Mode = fblbur::m_Undefined;
			_TargetLabel = NULL;
			_BackendInformations = NULL;
			_PublisherInformations = NULL;
			_ClientOrigin = NULL;
			_APIVersion = NULL;
		}
		// '[Backend|Publisher]Informations' ne sont PAS dupliqu�. Leur contenu de doit pas �t emodifi�.
		void Init(
			fblbur::mode__ Mode,
			const char *APIVersion,	// NON dupliqu�.
			const char *ClientOrigin,	// NON dupliqu�.
			const char *TargetLabel,
			const lcl::locale_ &Locale,	// N'est pas dupliqu� !
			const char *BackendInformations,	// NON dupliqu�.
			const char *PublisherInformations )	// NON dupliqu�.
		{
			Master_.Init( *this );

			Modules.Init();
			Links.Init();

			_CompatibilityTested = false;
			_Mode = Mode;

			_TargetLabel = TargetLabel;

			_BackendInformations = BackendInformations;
			_PublisherInformations = PublisherInformations;
			_ClientOrigin = ClientOrigin;
			_APIVersion = APIVersion;

			_LocaleRack.Init( Locale, str::string( FBLBKD__DEFAULT_LANGUAGE ).Convert( _Language ) );

//			return _TestCompatibility( Flow, APIVersion, MessageLabel, URLLabel );
		}
		//f Add 'Module' to the interface.
		void Add( untyped_module &Module )
		{
			Module.Backend_ = this;
			Modules.Append( &Module );
		}
		/*f Handle the request which come by 'Channel' and write the answer to 'Channel'.
		If 'true' is returned, than the request contains a deconnection request. */
		bso::bool__ Handle(
			flw::ioflow__ &FrontendFlow,
			void *PU,
			log_functions__ &LogFunctions )
		{
			if ( !_CompatibilityTested ) {
				_CompatibilityTested = true;
				return _TestCompatibility( FrontendFlow );
			}else
				return _HandleRequest( FrontendFlow, PU, LogFunctions );
		}
		/*f Return the command corresponding at request description 'Description' and
		object type 'Type'. 'FBLBKD_INVALID_COMMAND' is returned if command not found. */
		command__ Command(
			type__ IdType,
			const fblbrd::description_ &Description ) const
		{
			return Module_( IdType ).Command( Description );
		}
		//f Return 'treu' if 'Object' exists, false otherwise.
		bso::bool__ Exists( object__ Object ) const
		{
			return Links.Exists( Object );
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
		const void *Object( object__ Object ) const
		{
			return Module_( Object ).Object( Links.Index( Object ) );
		}
		//f Give a new object.
		object__ New( type__ Type )
		{
			if ( (unsigned long)Links.Amount() >= (unsigned long)FBLBKD_TYPE_MAX )
				ERRl();

			return Links.New( Type, Module_( Type ).New() );
		}
		//f Give the type of the object 'Object'.
		type__ Type( object__ Object) const
		{
			return Type_( Object );
		}
		//f Give the type of a nobject named 'Name'.
		type__ Type( const str::string_ &Name ) const;
		//f Delete object 'Object'.
		void Delete( object__ Object )
		{
			Module_( Object ).Delete( Links.Index( Object ) );

			Links.Delete( Object );
		}
		/*f Add a request description with name 'Name', function pointer 'FP'
		and a list of casts 'Casts'. The list must contain 2 'cEnd', the first
		at the end of the parameters casts,	and 1 of the end of returned values casts. */
		epeios::row__ Add(
			const char *Name,
			function__ FP,
			const cast *Casts )
		{
			return Master_.Add( Name, Casts, (void *)FP );
		}
		epeios::row__ Add(
			const char *Name,
			function__ FP,
			cast Cast,	// Added to avoid confusion with function above.
			... )
		{
			return Add( Name, FP, &Cast );
		}
		const char *GetTargetLabel( void ) const
		{
			return _TargetLabel;
		}
		const char *GetBackendInformations( void ) const 
		{
			return _BackendInformations;
		}
		const char *GetPublisherInformations( void ) const 
		{
			return _PublisherInformations;
		}
		const char *GetAPIVersion( void ) const 
		{
			return _APIVersion;
		}
		//f Return 'true' if 'Object' is of type given by 'Prefix' and 'Name'.
		bso::bool__ IsOfType(
			object__ Object,
			const char *Prefix,
			const char *Name ) const
		{
			return !strcmp( Module( Object ).Prefix(), Prefix ) && !strcmp( Module( Object ).Name(), Name );
		}
#if 0
		//f Return the raw messages.
		messages_ &GetMasterRawMessages( void )
		{
			return Master_.RawMessages;
		}
#endif
		const char *ClientOrigin( void ) const
		{
			return _ClientOrigin;
		}
		const lcl::rack__ &LocaleRack() const 
		{
			return _LocaleRack;
		}
		void SetLanguage( const char *Language )
		{
			str::string( Language ).Convert( _Language );
			_LocaleRack.Language = _Language;
		}
	};

	typedef backend backend_;

}

//d A ram module of an object of type 't'.
# define FBLBKD_RAM_MODULE( t )	fblbkd::ram_module<t, t::s>	

//d A standard module of an object of type 't'.
# define FBLBKD_STANDARD_MODULE( t )	fblbkd::standard_module<t, t::s>	

//d A shared module of an object od type 't'.
# define FBLBKD_SHARED_MODULE( t )	fblbkd::shared_module<t, t::s>	


/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
