/*
  'broker' library by Claude L. Simon (epeios@epeios.org)
  Requires the 'broker' header file ('broker.h').
  Copyright (C) 2000 Claude L. SIMON (epeios@epeios.org).

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
  along with this program; if not, go to http://www.fsf.org or write to the:
  
                        Free Software Foundation, Inc.,
           59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

//	$Id$

#define BROKER__COMPILATION

#include "broker.h"

class brokertutor
: public ttr_tutor
{
public:
	brokertutor( void )
	: ttr_tutor( BROKER_NAME )
	{
#ifdef BROKER_DBG
		Version = BROKER_VERSION " (DBG)";
#else
		Version = BROKER_VERSION;
#endif
		Owner = BROKER_OWNER;
		Date = "$Date$";
	}
	virtual ~brokertutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace broker;

static const char *PrimaryCommandName = brkcmd::CommandsNames[brkcmd::cGetCommand];

namespace {
	cast GetCastID_( const str::string_ &Name )
	{
		cast C = cInvalid;
	ERRProlog
		char *N = NULL;
	ERRBegin
		N = Name.Convert();

		C = GetID( N );
	ERRErr
	ERREnd
		if ( N != NULL )
			free( N );
	ERREpilog
		return C;
	}
}

struct master_data__ {
	bso__bool Deconnexion;
	void *UP;
};

void master_module::Handle_(
	index__,
	request_manager___ &Requete,
	void *PU )
{
	command__ C;
	master_data__ &MasterData = *(master_data__ *)PU;

	flw::Get( Requete.Input(), C );

	if (  C < Descriptions.Amount() )
	{
		Requete.SetDescription( Descriptions( C ) );
		(*(function__)UPs( C ))( *Broker(), *this, C, Requete, MasterData.Deconnexion, MasterData.UP );
	}
	else if ( C == BROKER_MASTER_COMMAND )
	{
		char Car;

		if ( ( Car = Requete.Input().Get() ) != 0 )
		{
ERRProlog
			cast__ Cast;
			str::string S;
ERRBegin
			Requete.Output().Put( 0 );	// No explanation message;

			do {
				S.Init();

				S.Add( Car );

				while( ( Car = Requete.Input().Get() ) != 0 )
					S.Add( Car );

				Cast = GetCastID_( S );

				flw::Put( Cast, Requete.Output() );

			} while ( ( Car = Requete.Input().Get() ) != 0 );
ERRErr
ERREnd
ERREpilog
		}
		else
		{
			tym::row__ P = Descriptions.First();
			command__ C = BROKER_INVALID_COMMAND;

			while ( ( P != NONE ) && ( Descriptions( P ).Name != str::string( PrimaryCommandName ) ) )
				P = Descriptions.Next( P );

			if ( P != NONE )
				C = (command__)P.V;

			Requete.Output().Put( 0 );	// No explanation message;

			flw::Put( C, Requete.Output() );

		}
	}
	else
		ERRb();
}


// Donne la liste des identificateurs et des libellés des langues.
static void GetLanguagesIDAndName_(
	broker_ &Broker,
	untyped_module &Module,
	command__ Command,
	request_manager___ &Requete,
	bso__bool &,
	void * )
{
ERRProlog
	items16 Items;
	item16 Item;
ERRBegin
	Items.Init();
	Item.Init();

	Item.ID( lgg::lDefault );
	Item.Value = LGGLanguageName[lgg::lDefault];
	Items.Add( Item );

	for ( bso__ushort I = 0; I < Broker.Langues_.Nombre; I++ )
	{
		Item.Init();
		Item.ID( Broker.Langues_.Identificateurs[I] );
		Item.Value = LGGLanguageName[Broker.Langues_.Identificateurs[I]];
		Items.Add( Item );
	}

	Requete.PushItems16( Items );
	Requete.Complete();
ERRErr
ERREnd
ERREpilog
}

// Donne la liste des identificateurs et des libellés de types
static void GetTypesIDAndName_(
	broker_ &Broker,
	untyped_module &Module,
	command__ Command,
	request_manager___ &Requete,
	bso__bool &,
	void * )
{
ERRProlog
	items16 Items;
	item16 Item;
	tym::row__ P;
	type__ Type;
ERRBegin
	Items.Init();

	P = Broker.Modules.First();

	while( P != NONE )
	{
		Item.Init();

#ifdef BROKER_DEBUG
		if ( P > BROKER_TYPE_MAX )
			ERRc();
#endif
		Type = (type_t__)P.V;

		Item.ID( Type.V );
		Item.Value = Broker.Module( Type ).Name();
		Items.Add( Item );
		P = Broker.Modules.Next( P );
	}

	Requete.PushItems16( Items );
	Requete.Complete();
ERRErr
ERREnd
ERREpilog
}

static void WriteCommandsIDAndName_(
	const descriptions_ &Descriptions,
	request_manager___ &Requete )
{
ERRProlog
	items16 Items;
	item16 Item;
	ctn::E_CITEM( description_ ) Description;
	tym::row__ P;
	command__ Command;
ERRBegin
	Items.Init();

	P = Descriptions.First();

	Description.Init( Descriptions );

	while( P != NONE )
	{
		Item.Init();

#ifdef BROKER_DEBUG
		if ( P > BROKER_COMMAND_MAX )
			ERRC();
#endif
		Command = (command__)P.V;

		Item.ID( Command );
		Item.Value = Description( P ).Name;
		
		if ( Item.Value( 0 ) != '_' )
			Items.Add( Item );
			
		P = Descriptions.Next( P );
	}

	Requete.PushItems16( Items );
ERRErr
ERREnd
ERREpilog
}


static void GetCommandsIDAndName_(
	broker_ &Broker,
	untyped_module &Module,
	command__ Command,
	request_manager___ &Requete,
	bso__bool &,
	void * )
{
	id16__ Type;

	Requete.PopId16( Type );

	WriteCommandsIDAndName_( Broker.Module( (type__)Type.V ).Descriptions, Requete );

	Requete.Complete();
}

static inline void WriteParameters_(
	const description_ &Description,
	request_manager___ &Requete )
{
	Requete.PushIds8( Description.Casts );
}


static void WriteParameters_(
	const descriptions_ &Descriptions,
	command__ Command,
	request_manager___ &Requete )
{
	ctn::E_CITEM( description_ ) Description;

	Description.Init( Descriptions );

	WriteParameters_( Description( Command ), Requete );
}


static void GetParameters_(
	broker_ &Broker,
	untyped_module &Module,
	command__,
	request_manager___ &Requete,
	bso__bool &,
	void * )
{
	id16__ Type;
	id16__ Command;

	Requete.PopId16( Type );
	Requete.PopId16( Command );

	WriteParameters_( Broker.Module( (type__)Type.V ).Descriptions, Command.V, Requete );

	Requete.Complete();
}


// Donne le libellés de la version de l'interface.
static void GetVersion_(
	broker_ &Broker,
	untyped_module &Module,
	command__ Command,
	request_manager___ &Requete,
	bso__bool &,
	void * )
{
	Requete.PushString( string( Broker.Version_ ) );
	Requete.Complete();
}

// Définit le comportement provoqué par une erreur et retourne son ancien comportement.
static void SetErrorBreakingStatus_(
	broker_ &Broker,
	untyped_module &Module,
	command__ Command,
	request_manager___ &Requete,
	bso__bool &,
	void * )
{
	bso__bool Retour;
	boolean__ O;

	Retour = (bso__raw)Broker.ErrorBreaking();

	Requete.PopBoolean( O );

	Broker.ErrorBreaking( O != 0  );

	Requete.PushBoolean( Retour );

	Requete.Complete();
}

// Retourne l'error breaking.
static void GetErrorBreakingStatus_(
	broker_ &Broker,
	untyped_module &Module,
	command__ Command,
	request_manager___ &Requete,
	bso__bool &,
	void * )
{
	bso__bool Retour;

	Retour = (bso__raw)Broker.ErrorBreaking();

	Requete.PushBoolean( Retour );

	Requete.Complete();
}

// Retourne un nouvel objet.
static void GetNewObject_(
	broker_ &Broker,
	untyped_module &Module,
	command__ Command,
	request_manager___ &Requete,
	bso__bool &,
	void * )
{
	object__ O;
	id16__ T;

	Requete.PopId16( T );

	if ( T.V >= Broker.Modules.Amount() )
		ERRb();

	if ( T == BROKER_INVALID_TYPE )
		ERRb();

	O = Broker.New( (type__)T.V );

	if ( O != BROKER_INVALID_TYPE )
		Requete.PushObject( O );
	else
		Requete.SendExplanationMessage( "No such object type." );

	Requete.Complete();
}

// Retourne l'identificateur correspondant à un type donné.
static void GetType_(
	broker_ &Broker,
	untyped_module &Module,
	command__ Command,
	request_manager___ &Requete,
	bso__bool &,
	void * )
{
ERRProlog
	str::string Type;
	type__ T;
ERRBegin
	Type.Init();

	Requete.PopString( Type );

	T = Broker.Type( Type );

	if ( ( T != BROKER_INVALID_TYPE )  )
		Requete.PushId16( T.V );
	else
		Requete.SendExplanationMessage( "No such object type name." );

	Requete.Complete();
ERRErr
ERREnd
ERREpilog
}

// Supprime un objet.
static void RemoveObject_(
	broker_ &Broker,
	untyped_module &Module,
	command__ Command,
	request_manager___ &Requete,
	bso__bool &,
	void * )
{
	object__ O;

	Requete.PopObject( O );

/*	if ( !Broker.Valide( O ) )
		ERRb();
*/
	Broker.Remove( O );

	Requete.Complete();
}

static void FillCommands_(
	broker_ &Broker,
	type__ Type,
	const commands_details_ &CommandsDetails,
	ids16_ &Commands )
{
ERRProlog
	id16__ Command;
	tym::row__ Position = CommandsDetails.First();
	ctn::E_CITEM( command_detail_ ) CommandDetail;
	description Description;
ERRBegin
	CommandDetail.Init( CommandsDetails );

	while( Position != NONE )
	{
		Description.Init();

		Description.Name = CommandDetail( Position ).Name;
		Description.Casts = CommandDetail( Position ).Casts;

		if ( ( Command = Broker.Command( Type, Description ) ) == BROKER_INVALID_COMMAND )
			ERRb();

		Commands.Add( Command );

		Position = CommandsDetails.Next( Position );
	}
ERRErr
ERREnd
ERREpilog
}



// Retourne l'identificateur de type et les identificateurs de commande demandé.
static void GetTypeAndCommands_(
	broker_ &Broker,
	untyped_module &Module,
	command__ Command,
	request_manager___ &Requete,
	bso__bool &,
	void * )
{
ERRProlog
	string Name;
	commands_details CommandsDetails;
	type__ Type;
	ids16 Commands;
ERRBegin
	Name.Init();
	CommandsDetails.Init();

	Requete.PopString( Name );
	Requete.PopCommandsDetails( CommandsDetails );

	if ( ( Type = Broker.Type( Name ) ) == BROKER_INVALID_TYPE )
		ERRb();

	Commands.Init();

	FillCommands_( Broker, Type, CommandsDetails, Commands );

	Requete.PushId16( Type.V );
	Requete.PushIds16( Commands );
	Requete.Complete();
ERRErr
ERREnd
ERREpilog
}


// Retourne l'identificateur d'une commande donnée pour un type donné.
static void GetCommand_(
	broker_ &Broker,
	untyped_module &Module,
	command__ Command,
	request_manager___ &Requete,
	bso__bool &,
	void * )
{
ERRProlog
	id16__ Type;
	description Description;
	command__ Command;
ERRBegin
	Requete.PopId16( Type );

	Description.Init();
	Requete.PopString( Description.Name );
	Requete.PopIds8( Description.Casts );

/*	if ( !Broker.Valide( T ) )
		if ( T() != BROKER9_TYPE_MAITRE )
			ERRb();
*/
	Command = Broker.Command( Type.V, Description );

	if ( ( Command != BROKER_INVALID_COMMAND ) )
		Requete.PushId16( Command );
	else
		Requete.SendExplanationMessage( "No such command name or with such description" );

	Requete.Complete();
ERRErr
ERREnd
ERREpilog
}


// Retourne l'identificateur des commandes demandées pour un type donné.
static void GetCommands_(
	broker_ &Broker,
	untyped_module &Module,
	command__ Command,
	request_manager___ &Requete,
	bso__bool &,
	void * )
{
ERRProlog
	id16__ Type;
	commands_details CommandsDetails;
	ids16 Commands;
ERRBegin
	Requete.PopId16( Type );

	CommandsDetails.Init();
	Requete.PopCommandsDetails( CommandsDetails );

	Commands.Init();

	FillCommands_( Broker, Type.V, CommandsDetails, Commands );

	Requete.PushIds16( Commands );
	Requete.Complete();
ERRErr
ERREnd
ERREpilog
}


// Deconnection
static void Disconnect_(
	broker_ &Broker,
	untyped_module &Module,
	command__ Command,
	request_manager___ &Requete,
	bso__bool &Deconnexion,
	void * )
{
	Deconnexion = true;
}

#define ADD( I )	Broker.Add( brkcmd::CommandsNames[brkcmd::c##I##], ::I##_, brkcmd::CommandsParameters[brkcmd::c##I] )

namespace broker {
	// Initialisation avec rattachement à l'interface 'Frontend'.
	void master_module::Init( ::broker::broker_ &Broker )
	{
		untyped_module::Init( NULL );
		Broker_ = &Broker;

		// Get and configure the error modus.
		ADD( SetErrorBreakingStatus );

		ADD( GetErrorBreakingStatus );

		// Create a new object.
		ADD( GetNewObject );

		// Give the type corresponding to the given name.
		ADD( GetType );

		// Give the command of the given object type and command name.
		ADD( GetCommand );

		// Give the commands of the given object type and command names.
		ADD( GetCommands );

		// Give the type and the commands of the given type name, commands names and cats.
		ADD( GetTypeAndCommands );

		// Remove the given object.
		ADD( RemoveObject );

		// The id and name of the languages supported.
		ADD( GetLanguagesIDAndName );

		// The string of the interface version.
		ADD( GetVersion );

		// Disconnection.
		ADD( Disconnect );

		// The id and name of the supported object types.
		ADD( GetTypesIDAndName );

		// The id and name of the commands of an object type.
		ADD( GetCommandsIDAndName );

		// The parameters (cast) of a command of an object type.
		ADD( GetParameters );

	}

	type__ broker::Type( str::string_ &Name )
	{
		tym::row__ C = Modules.First();

		while ( ( C != NONE ) && ( str::string( Modules(C)->Name() ) != Name )  )
			C = Modules.Next( C );

		if ( C == NONE )
			C = BROKER_INVALID_TYPE;
		else if ( C.V > BROKER_INVALID_TYPE )
			ERRl();

		return (type_t__)C.V;
	}

	bso__bool broker_::Handle(
		flw::ioflow___ &Channel,
		void *PU )
	{
		master_data__ MasterData;
	ERRProlog
		request_manager___ Requete;
		object__ O;
	ERRBegin
		MasterData.Deconnexion = false;
		MasterData.UP = PU;

		Requete.Init( Channel );

		flw::Get( Channel, O );

		if ( ( O.V >= Links.Amount() ) && ( O != BROKER_MASTER_OBJECT ) )
			ERRb();

		if ( O != BROKER_MASTER_OBJECT )
			Module_( O ).Handle( Index_( O ), Requete, PU );
		else
			Master_.Handle( (index__)0, Requete, &MasterData );

	ERRErr
		Requete.SendExplanationMessage( ERRMessage() );

		if ( !Master_.ErrorBreaking() )
			ERRRst();
	ERREnd
		Requete.Complete();
	ERREpilog
		return !MasterData.Deconnexion;
	}
#if 0	
	tym::row__ untyped_module::Add(
		const char *Name,
		const void *UP,
		... )
	{
		tym::row__ P;
	ERRProlog
		va_list L;
		description Description;
		brkcst::cast Cast;
	ERRBegin
		Description.Init( Name );
		
		va_start( L, UP );

		while ( ( Cast = va_arg( L, brkcst::cast ) ) != cEnd )
			Description.Add( Cast );
			
		Description.Add( Cast );
		
		while( ( Cast = va_arg( L, brkcst::cast ) ) != cEnd )
			Description.Add( Cast );
			
		va_end( L );
		
		P = Descriptions.Add( Description );
		
		if ( UPs.Add( UP ) != P )
			ERRc();
	ERRErr
	ERREnd
	ERREpilog
		return P;
	}
#endif
	
}

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class brokerpersonnalization
: public brokertutor
{
public:
	brokerpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~brokerpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the ending of the application  */
	}
};


/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

// 'static' by GNU C++.

static brokerpersonnalization Tutor;

ttr_tutor &BROKERTutor = Tutor;
