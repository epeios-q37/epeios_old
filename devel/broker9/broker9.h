/*
  Header for the 'broker9' library by Claude L. Simon (epeios@epeios.org)
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

#ifndef BROKER9__INC
#define BROKER9__INC

#define BROKER9_NAME		"BROKER9"

#define	BROKER9_VERSION	"$Revision$"	

#define BROKER9_OWNER		"the Epeios project (http://www.epeios.org/)"

#include "ttr.h"

extern class ttr_tutor &BROKER9Tutor;

#if defined( XXX_DBG ) && !defined( BROKER9_NODBG )
#define BROKER9_DBG 
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
//D BROKER part 9.

#error Obsolete. Use library 'BROKER' instead.

#include "err.h"
#include "flw.h"
#include "rqm.h"
#include "lst.h"
#include "lgg.h"
#include "cpe.h"

//d An invalid value for a command.
#define BROKER9_INVALID_COMMAND		RQM_INVALID_COMMAND
#define BROKER9_COMMAND_MAX			RQM_COMMAND_MAX

//d An invalid value for a type.
#define BROKER9_INVALID_TYPE		RQM_INVALID_TYPE
#define BROKER9_TYPE_MAX			RQM_TYPE_MAX

//d An invalid value for an object.
#define BROKER9_INVALID_OBJECT		RQM_INVALID_OBJECT
#define BROKER9_OBJECT_MAX			RQM_OBJECT_MAX

#define BROKER9_MASTER_OBJECT		BROKER9_INVALID_OBJECT

#define BROKER9_MASTER_TYPE		 	BROKER9_INVALID_TYPE

#define BROKER9_MASTER_COMMAND		BROKER9_INVALID_COMMAND

//t To handle an object within a module.
TYPEDEF( POSITION__, broker9__index );

//t An object type.
typedef rqm__object		broker9__object;
typedef rqm__tobject	broker9__tobject;

//t A type type (sorry!).
typedef rqm__type		broker9__type;
typedef rqm__ttype		broker9__ttype;

//t A command type.
typedef rqm__command	broker9__command;
typedef rqm__tcommand	broker9__tcommand;

//t A cast type.
typedef rqm__cast		broker9__cast;

typedef void (* broker9__function )(
	class broker9_frontend &Frontend,
	class broker9_untyped_module &Module,
	broker9__command Commande,
	rqm__request_manager_ &Requete,
	bso__bool &,
	void *UP );


typedef rqm_description_	broker9_description_;
typedef rqm_description		broker9_description;

class broker9_descriptions_
: public rqm_descriptions_
{
public:
	SET_( void *) UPs;
	struct s
	: public rqm_descriptions_::s
	{
		SET_( void *)::s UPs;
	};
	broker9_descriptions_( s &S )
	: rqm_descriptions_( S ),
	  UPs( S.UPs )
	{}
	void reset( bool P = true )
	{
		rqm_descriptions_::reset( P );
		UPs.reset( P );
	}
	void plug( mmm_multimemory &M )
	{
		rqm_descriptions_::plug( M );
		UPs.plug( M );
	}
	broker9_descriptions_ &operator =( const broker9_descriptions_ &D )
	{
		rqm_descriptions_::operator =( D );
		UPs = D.UPs;

		return *this;
	}
	void Init( void )
	{
		rqm_descriptions_::Init();
		UPs.Init();
	}
	// Cast is only here to avoid ambiguity with the nest function.
	POSITION__ Add(
		const char *Name,
		void *UP,
		rqm::cast Cast,
		... );
	POSITION__ Add(
		const char *Name,
		void *UP,
		const rqm::cast *Casts );
	void *UP( POSITION__ Position )
	{
		return UPs( Position );
	}
};

AUTO( broker9_descriptions )


//c An untyped module.
class broker9_untyped_module
{
private:
	// Libelle du type de l'objet, du module.
	const char *Name_;
	// L'interface auquel le module est rattaché.
	class broker9_frontend *Frontend_;
protected:
	//v To get the index of a new object.
	virtual broker9__index BROKER9New( void )
	{
		ERRb();
		return (broker9__index)0;	// Pour éviter un warning.
	}
	//v To remove the object with index 'Index'.
	virtual void BROKER9Remove( broker9__index Index )
	{
		ERRb();
	}
	//v To get a pointer of the object of index 'Index'.
	virtual void *BROKER9Object( broker9__index Index )
	{
		ERRb();
		return NULL;	// Pour éviter un 'warning'
	}
	// Fonction appelée pour traiter la requête 'Requete' pour l'objet d'index 'Index'.
	virtual void Handle_(
		broker9__index Index,
		rqm__request_manager_ &Requete,
		void *PU ) = 0;
public:
	//r The description of the request.
	broker9_descriptions Descriptions;
	broker9_untyped_module( void )
	{
		Name_ = NULL;
		Frontend_ = NULL;
	}
	//f Give the index of a new object.
	broker9__index New( void )
	{
		return BROKER9New();
	}
	//f Remove the object of index 'Index'.
	void Remove( broker9__index Index )
	{
		BROKER9Remove( Index );
	}
	//f Give the name of the objects.
	const char *Name( void ) const
	{
		return Name_;
	}
	//f Give an pointer to the object of index 'Index'.
	void *Object( broker9__index Index )
	{
		return BROKER9Object( Index );
	}
	//f Initialization with the object name 'Name'.
	void Init( const char *Name )
	{
		Descriptions.Init();

		Name_ = Name;
	}
	//f Handle the request 'Request' for object of index 'Index'.
	void Handle(
		broker9__index Index,
		rqm__request_manager_ &Requete,
		void *UP )
	{
		Handle_( Index, Requete, UP );
	}
	//f Return the interface attache to this module.
	broker9_frontend *Frontend( void )
	{
		return Frontend_;
	}
	//f Return the command which has 'Description' as description, or 'BROKER9_INVALID_COMMAND' if non-existant.
	broker9__command Command( const rqm_description_ &Description ) const
	{
		POSITION__ P = Descriptions.Position( Description );

		if ( P == NONE )
			P = BROKER9_INVALID_COMMAND;
		else if ( P > BROKER9_COMMAND_MAX )
			ERRl();

		return (broker9__tcommand)P;
	}
	friend class broker9_frontend;
	friend class broker9_master_module;
};

//c A module for an object of type 't'.
template <class t> class broker9_module
: public broker9_untyped_module
{
private:
	void Traiter_(
		t &Objet,
		rqm__request_manager_ &Requete,
		void *PU )
	{
		broker9__command C;

		FLWGet( Requete.Input(), C );

		if ( C() > Descriptions.Amount() )
			ERRu();

		Requete.SetDescription( Descriptions( C() ) );

		Objet.HANDLE( *Frontend(), *this, C, Requete, PU );
	}
protected:
	virtual void Handle_(
		broker9__index Index,
		rqm__request_manager_ &Requete,
		void *PU )
	{
		Traiter_( *(t *)broker9_untyped_module::Object( Index ), Requete, PU );
	}
	//f Initialization.
	void Init( void )
	{
		broker9_untyped_module::Init( t::NAME );

		t::NOTIFY( broker9_untyped_module::Descriptions );
	}
};

//c A module with object stored in RAM.
template <class t, class st> class broker9_ram_module
: public broker9_module<t>,
  private LIST
{
protected:
	virtual void LSTAllocate( SIZE__ Size )
	{
		Objets.Allocate( Size );
	}
	virtual broker9__index BROKER9New( void )
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

		broker9__index Index = LIST::CreateEntry();

		Objets.Write( Pointeur, Index() );

		return Index;
	}
	virtual void BROKER9Remove( broker9__index Index )
	{
		delete Objets( Index() );
		LIST::Remove( Index() );
	}
	virtual void *BROKER9Object( broker9__index Index )
	{
		if ( Index() >= Objets.Amount() )
			ERRu();

		return (void *)Objets( Index() );
	}
public:
	//r The pointer object.
	SET( t * ) Objets;
	//f Initialization.
	void Init( void )
	{
		LIST::Init();
		Objets.Init();
		broker9_module<t>::Init();
	}
};

//c A module with object stored in standard memory.
template <class t, class st> class broker9_standard_module
: public broker9_module<t>,
  private LIST
{
private:
	ITEM( t ) Element_;
protected:
	virtual void LSTAllocate( SIZE__ Size )
	{
		Element_.Sync();
		Objets.Allocate( Size );
	}
	virtual broker9__index BROKER9New( void )
	{
		Element_.Sync();
		return LIST::CreateEntry();
	}
	virtual void BROKER9Remove( broker9__index Index )
	{
		Element_( Index() ).reset();
		LIST::Remove( Index() );
		Element_.Sync();
	}
	virtual void *BROKER9Object( broker9__index Index )
	{
		if ( Index() >= Objets.Amount() )
			ERRu();

		return (void *)&Element_( Index() );
	}
public:
	//r Contient les objets.
	CONTAINER( t ) Objets;
	void reset( bool P = true )
	{
		Element_.Sync();
		Objets.reset( P );
		LIST::reset( P );
	}
	broker9_standard_module( void )
	{
		reset( false );
	}
	~broker9_standard_module( void )
	{
		reset( true );
	}
	// Initialisation.
	void Init( void )
	{
		LIST::Init();
		Objets.Init();
		Element_.Init( Objets );
		broker9_untyped_module::Init( t::NAME );
		broker9_module<t>::Init();
	}
};

// Module maître, qui fait tout le boulot.
class broker9_master_module
: public broker9_untyped_module
{
private:
	// Si vrai, une erreur arrête le thread, sinon celui-ci se poursuit.
	bso__bool ErrorBreaking_;
protected:
	virtual void Handle_(
		broker9__index Index,
		rqm__request_manager_ &Requete,
		void *PU );
public:
	broker9_master_module( void )
	{
		ErrorBreaking_ = true;
	}
	// Initialisation avec rattachement à l'interface 'Frontend'.
	void Init( broker9_frontend &Frontend );
	bso__bool ErrorBreaking( void  )
	{
		return ErrorBreaking_;
	}
	void ErrorBreaking( bso__bool Value )
	{
		ErrorBreaking_ = Value;
	}
};

struct broker9__link
{
	// Le type de l'objet
	broker9__type Type;
	// L'index de l'objet;
	broker9__index Index;
};

// Classe de gestion des liens entre module et objets.
class broker9_links
: private SET( broker9__link ),
  private LIST
{
protected:
	void LSTAllocate( SIZE__ Size )
	{
		SET( broker9__link )::Allocate( Size );
	}
public:
	// Initialisation.
	void Init( void )
	{
		LIST::Init();
		SET( broker9__link )::Init();
	}
	broker9__object New(
		broker9__type IdType,
		broker9__index Index )
	{
		broker9__link Lien;
		broker9__object IdObjet;
		POSITION__ P;
		
		P = LIST::CreateEntry();

		if ( P > BROKER9_TYPE_MAX )
			ERRl();

		IdObjet = (broker9__ttype)P;

		Lien.Type = IdType;
		Lien.Index = Index;

		SET( broker9__link )::Write( Lien, IdObjet() );

		return IdObjet;
	}
	void Remove( broker9__object IdObjet )
	{
		LIST::Remove( IdObjet() );
	}
	broker9__type Type( broker9__object IdObjet ) const
	{
		return SET( broker9__link )::Read( IdObjet() ).Type;
	}
	broker9__index Index( broker9__object IdObjet ) const
	{
		return SET( broker9__link )::Read( IdObjet() ).Index;
	}
	SIZE__ Amount( void ) const
	{
		return LIST::Amount();
	}
};


//c A frontend, which handles objects od different type and request to this object.
class broker9_frontend
{
private:
	broker9_master_module Master_;
	// Retourne le module correspondant à 'IdType'.
	broker9_untyped_module &Module_( broker9__type IdType ) const
	{
		if ( IdType() != BROKER9_MASTER_TYPE )
			return *Modules( IdType() );
		else
			return (broker9_untyped_module &)Master_;	// Not very happy about this conversion, 
	}
	// Retourne le module correspondant à 'IdObjet'.
	broker9_untyped_module &Module_( broker9__object IdObjet ) const
	{
		if ( IdObjet() != BROKER9_MASTER_OBJECT )
			return Module_( Type_( IdObjet ) );
		else
			return (broker9_untyped_module &)Master_; // Not very happy about this conversion, 
	}
	// Retourne le type correpondant à l'objet d'indetificateur 'IdObjet'.
	broker9__type Type_( broker9__object IdObjet ) const
	{
		return Links.Type( IdObjet );
	}
	// Retourne l'indexcorrespondant à l'objet d'identificateur 'IdObjet'.
	broker9__index Index_( broker9__object IdObjet ) const
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
		bso__ushort Nombre;
		// Les identificateurs des langues supportées.
		const lgg::language *Identificateurs;
		// La langue courante.
		lgg::language Courante;
	} Langues_;
	//o The different modules.
	SET( broker9_untyped_module * ) Modules;
	//o The relation between modules an index.
	broker9_links Links;
	broker9_frontend( void )
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
	void Add( broker9_untyped_module &Module )
	{
		Module.Frontend_ = this;
		Modules.Add( &Module );
	}
	/*f Handle the request which come by 'Input' and write the answer to 'Output'.
	If 'true' is returned, than the request contains a deconnection request. */
	bso__bool Handle(
		flw::iflow___ &Input,
		flw::oflow___ &Output,
		void *PU = NULL );
	/*f Return the command corresponding at request description 'Description' and
	object type 'Type'. 'BROKER9_INVALID_COMMAND' is returned if command not found. */
	broker9__command Command(
		broker9__type IdType,
		const rqm_description_ &Description )
	{
		return Module_( IdType ).Command( Description );
	}
	//f Return the error breaking status.
	bso__bool ErrorBreaking( void  )
	{
		return Master_.ErrorBreaking();
	}
	//f Set to 'Value' the error breaking status.
	void ErrorBreaking( bso__bool Value )
	{
		Master_.ErrorBreaking( Value );
	}
	//f Give the module for the object of type 'Type'.
	const broker9_untyped_module &Module( broker9__type Type ) const
	{
		return Module_( Type );
	}
	//f Give the module for object 'Object'.
	const broker9_untyped_module &Module( broker9__object Object ) const
	{
		return Module_( Object );
	}
	//f Return a pointer to the 'Object' object.
	const void *Object( broker9__object Object )
	{
		return Module_( Object ).Object( Links.Index( Object ) );
	}
	//f Give a new object.
	broker9__object New( broker9__type Type )
	{
		if ( (unsigned long)Links.Amount() >= (unsigned long)BROKER9_TYPE_MAX )
			ERRl();

		return Links.New( Type, Module_( Type ).New() );
	}
	//f Give the type of the object 'Object'.
	broker9__type Type( broker9__object Object)
	{
		return Type_( Object );
	}
	//f Give the type of a nobject named 'Name'.
	broker9__type Type( str_string_ &Name );
	//f Remove object 'Object'.
	void Remove( broker9__object Object )
	{
		Module_( Object ).Remove( Links.Index( Object ) );
	}
	/*f Tell that 'Amount' language indicated in 'Languages' are supported.
	'Languages' is not copied and should NOT be modified. */
	void Languages(
		bso__ushort Amount,
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
	//f Return the name of objects of type 'Type'.
	const char *Name( broker9__type IdType ) const
	{
		return Module( IdType ).Name();
	}
	//f Return the name of the object 'Object'.
	const char *Name( broker9__object Object )
	{
		return Name( Type( Object ));
	}
	/*f Add a request descrption with name 'Name', function pointer 'FP'
	and a list of casts. The list must contain 2 'rqm::cEnd', the first
	at the end of the parameters casts,	and 1 of the end of returned values casts.
	'Cast' exists only to avoid ambiguity with next function. */
	POSITION__ Add(
		const char *Name,
		broker9__function FP,
		rqm::cast Cast,
		... );
	/*f Add a request descrption with name 'Name', function pointer 'FP'
	and a list of casts 'Casts'. The list must contain 2 'rqm::cEnd', the first
	at the end of the parameters casts,	and 1 of the end of returned values casts. */
	POSITION__ Add(
		const char *Name,
		broker9__function FP,
		const rqm::cast *Casts );
};

typedef broker9_frontend broker9_frontend_;

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
