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

	if (  C() < Descriptions.Amount() )
	{
		Requete.SetDescription( Descriptions( C() ) );
		(*(function__)UPs( C() ))( *Broker(), *this, C, Requete, MasterData.Deconnexion, MasterData.UP );
	}
	else if ( C == BROKER_MASTER_COMMAND )
	{
		char Car;

		if ( ( Car = Requete.Input().Get() ) != 0 )
		{
ERRProlog
			cast__ Cast;
			str_string S;
ERRBegin
			Requete.Output().Put( 0 );	// No explanation message;

			do {
				S.Init();

				S.Add( Car );

				while( ( Car = Requete.Input().Get() ) != 0 )
					S.Add( Car );

				Cast = GetCast( S );

				flw::Put( Cast, Requete.Output() );

			} while ( ( Car = Requete.Input().Get() ) != 0 );
ERRErr
ERREnd
ERREpilog
		}
		else
		{
			POSITION__ P = Descriptions.First();
			command__ C = BROKER_INVALID_COMMAND;

			while ( ( P != NONE ) && ( Descriptions( P ).Name != str_string( PrimaryCommandName ) ) )
				P = Descriptions.Next( P );

			if ( P != NONE )
				C = (tcommand__)P;

			Requete.Output().Put( 0 );	// No explanation message;

			flw::Put( C, Requete.Output() );

		}
	}
	else
		ERRb();
}


// Donne la liste des identificateurs et des libellés des langues.
static void GetLanguagesIDAndName(
	broker_ &Broker,
	untyped_module &Module,
	command__ Command,
	request_manager___ &Requete,
	bso__bool &,
	void * )
{
ERRProlog
	bso__ushort IdLangue;
	str_string Name;
ERRBegin
	Requete.BeginArray();

	IdLangue = lgg::lDefault;
	Requete.AddValue( cId16, &IdLangue );
	
	Name.Init();
	Name = LGGLanguageName[IdLangue];
	Requete.AddValue( cString, &Name );

	for ( bso__ushort I = 0; I < Broker.Langues_.Nombre; I++ )
	{
		IdLangue = Broker.Langues_.Identificateurs[I];
		Name.Init();
		Name = LGGLanguageName[IdLangue];
		Requete.AddValue( cString, &Name );
	}

	Requete.EndArray();
ERRErr
ERREnd
ERREpilog
}

// Donne la liste des identificateurs et des libellés de types
static void GetTypesIDAndName(
	broker_ &Broker,
	untyped_module &Module,
	command__ Command,
	request_manager___ &Requete,
	bso__bool &,
	void * )
{
ERRProlog
	type__ Type;
	POSITION__ P = Broker.Modules.First();
	str_string Name;
ERRBegin
	Requete.BeginArray();

	while( P != NONE )
	{
		Type = (ttype__)P;
		Requete.AddValue( cType, &Type );
		
		Name.Init();
		Name = Broker.Name( Type );
		Requete.AddValue( cString, &Name );
		P = Broker.Modules.Next( P );
	}

	Requete.EndArray();
ERRErr
ERREnd
ERREpilog
}

static void WriteCommandsIDAndName_(
	const descriptions_ &Descriptions,
	request_manager___ &Requete )
{
	command__ Command;
	CITEM( description_ ) Description;
	POSITION__ P = Descriptions.First();

	Description.Init( Descriptions );

	Requete.BeginArray();

	while( P != NONE )
	{
		Command = (tcommand__)P;
		Requete.AddValue( cCommand, &Command );
		Requete.AddValue( cString, &Description( P ) );
		P = Descriptions.Next( P );
	}

	Requete.EndArray();
}


static void GetCommandsIDAndName(
	broker_ &Broker,
	untyped_module &Module,
	command__ Command,
	request_manager___ &Requete,
	bso__bool &,
	void * )
{
	type__ Type;

	Requete.GetValue( cType, &Type );

	WriteCommandsIDAndName_( Broker.Module( Type ).Descriptions, Requete );
}

static inline void WriteParameters_(
	const description_ &Description,
	request_manager___ &Requete )
{
	Requete.AddValue( cCasts, &Description.Casts );
}


static void WriteParameters_(
	const descriptions_ &Descriptions,
	command__ Command,
	request_manager___ &Requete )
{
	CITEM( description_ ) Description;

	Description.Init( Descriptions );

	WriteParameters_( Description( Command() ), Requete );
}


static void GetParameters(
	broker_ &Broker,
	untyped_module &Module,
	command__,
	request_manager___ &Requete,
	bso__bool &,
	void * )
{
	type__ Type;
	command__ Command;

	Requete.GetValue( cType, &Type );
	Requete.GetValue( cCommand, &Command );

	WriteParameters_( Broker.Module( Type ).Descriptions, Command, Requete );
}


// Donne le libellés de la version de l'interface.
static void GetVersion(
	broker_ &Broker,
	untyped_module &Module,
	command__ Command,
	request_manager___ &Requete,
	bso__bool &,
	void * )
{
ERRProlog
	str_string Version;
ERRBegin
	Version.Init();
	Version = Broker.Version_;

	Requete.AddValue( cString, &Version );
ERRErr
ERREnd
ERREpilog
}

// Définit le comportement provoqué par une erreur et retourne son ancien comportement.
static void SetErrorBreakingStatus(
	broker_ &Broker,
	untyped_module &Module,
	command__ Command,
	request_manager___ &Requete,
	bso__bool &,
	void * )
{
	bso__bool Retour;
	bso__bool O;

	Retour = (bso__raw)Broker.ErrorBreaking();

	Requete.GetValue( cBoolean, &O );

	Broker.ErrorBreaking( O != 0  );

	Requete.AddValue( cBoolean, &Retour );

	Requete.Complete();
}

// Retourne l'error breaking.
static void GetErrorBreakingStatus(
	broker_ &Broker,
	untyped_module &Module,
	command__ Command,
	request_manager___ &Requete,
	bso__bool &,
	void * )
{
	bso__bool Retour;

	Retour = (bso__raw)Broker.ErrorBreaking();

	Requete.AddValue( cBoolean, &Retour );

	Requete.Complete();
}

// Retourne un nouvel objet.
static void GetNewObject(
	broker_ &Broker,
	untyped_module &Module,
	command__ Command,
	request_manager___ &Requete,
	bso__bool &,
	void * )
{
	static object__ O;
	type__ T;

	Requete.GetValue( cType, &T );

	if ( T() >= Broker.Modules.Amount() )
		ERRb();

	if ( T == BROKER_INVALID_TYPE )
		ERRb();

	O = Broker.New( T );

	if ( O() != BROKER_INVALID_TYPE )
		Requete.AddValue( cObject, &O );
	else
		Requete.SendExplanationMessage( "No such object type." );

	Requete.Complete();
}

// Retourne l'identificateur correspondant à un type donné.
static void GetType(
	broker_ &Broker,
	untyped_module &Module,
	command__ Command,
	request_manager___ &Requete,
	bso__bool &,
	void * )
{
ERRProlog
	str_string Type;
	static type__ T;
ERRBegin
	Type.Init();

	Requete.GetValue( cString, &Type );

	T = Broker.Type( Type );

	if ( ( T() != BROKER_INVALID_TYPE )  )
		Requete.AddValue( cType, &T );
	else
		Requete.SendExplanationMessage( "No such object type name." );

	Requete.Complete();
ERRErr
ERREnd
ERREpilog
}

// Supprime un objet.
static void RemoveObject(
	broker_ &Broker,
	untyped_module &Module,
	command__ Command,
	request_manager___ &Requete,
	bso__bool &,
	void * )
{
	object__ O;

	Requete.GetValue( cObject, &O );

/*	if ( !Broker.Valide( O ) )
		ERRb();
*/
	Broker.Remove( O );

	Requete.Complete();
}

static void GetDescriptions_(
	request_manager___ &Manager,
	descriptions_ &Descriptions )
{
ERRProlog
	description Description;
ERRBegin
	Description.Init();

	if ( !Manager.GetArrayValue( cString, &Description.Name ) )
		ERRb();

	while( Manager.GetArrayValue( cCasts, &Description.Casts ) )
	{
		Descriptions.Add( Description );

		Description.Init();

		if ( !Manager.GetArrayValue( cString, &Description.Name ) )
			ERRb();
	}

	Descriptions.Add( Description );
ERRErr
ERREnd
ERREpilog
}

static void SendCommandIDs_(
	type__ Type,
	broker_ &Broker,
	const descriptions_ &Descriptions,
	request_manager___ &Manager )
{
ERRProlog
	command__ Command;
	POSITION__ Position = Descriptions.First();
	CITEM( description_ ) Description;
ERRBegin
	Description.Init( Descriptions );

	Manager.BeginArray();

	while( Position != NONE )
	{
		if ( ( Command = Broker.Command( Type, Description( Position ) ) ) == BROKER_INVALID_COMMAND )
			ERRb();

		Manager.AddValue( cCommand, &Command );

		Position = Descriptions.Next( Position );
	}

	Manager.EndArray();

	Manager.Complete();
ERRErr
ERREnd
ERREpilog
}



// Retourne l'identificateur de type et les identificateurs de commande demandé.
static void GetTypeAndCommands(
	broker_ &Broker,
	untyped_module &Module,
	command__ Command,
	request_manager___ &Requete,
	bso__bool &,
	void * )
{
ERRProlog
	descriptions Descriptions;
	type__ Type;
	str_string TypeName;
ERRBegin
	Descriptions.Init();
	TypeName.Init();

	Requete.GetValue( cString, &TypeName );

	if ( ( Type = Broker.Type( TypeName ) ) == BROKER_INVALID_TYPE )
		ERRb();

	GetDescriptions_( Requete, Descriptions );

	Requete.AddValue( cType, &Type );

	SendCommandIDs_( Type, Broker, Descriptions, Requete );
ERRErr
ERREnd
ERREpilog
}


// Retourne l'identificateur d'une commande donnée pour un type donné.
static void GetCommand(
	broker_ &Broker,
	untyped_module &Module,
	command__ Command,
	request_manager___ &Requete,
	bso__bool &,
	void * )
{
ERRProlog
	description Description;
	type__ T;
	command__ C;
ERRBegin
	Description.Init();

	Requete.GetValue( cType, &T );
	Requete.GetValue( cString, &Description.Name );
	Requete.GetValue( cCasts, &Description.Casts );

/*	if ( !Broker.Valide( T ) )
		if ( T() != BROKER9_TYPE_MAITRE )
			ERRb();
*/
	C = Broker.Command( T, Description );

	if ( ( C() != BROKER_INVALID_COMMAND ) )
		Requete.AddValue( cCommand, &C );
	else
		Requete.SendExplanationMessage( "No such command name or with such description" );

	Requete.Complete();
ERRErr
ERREnd
ERREpilog
}


// Retourne l'identificateur des commandes demandées pour un type donné.
static void GetCommands(
	broker_ &Broker,
	untyped_module &Module,
	command__ Command,
	request_manager___ &Requete,
	bso__bool &,
	void * )
{
ERRProlog
	descriptions Descriptions;
	type__ Type;
ERRBegin
	Descriptions.Init();

	Requete.GetValue( cType, &Type );

	GetDescriptions_( Requete, Descriptions );

	SendCommandIDs_( Type, Broker, Descriptions, Requete );
ERRErr
ERREnd
ERREpilog
}


// Deconnection
static void Disconnect(
	broker_ &Broker,
	untyped_module &Module,
	command__ Command,
	request_manager___ &Requete,
	bso__bool &Deconnexion,
	void * )
{
	Deconnexion = true;
}

#define ADD( I )	Broker.Add( brkcmd::CommandsNames[brkcmd::c##I], ::##I, brkcmd::CommandsParameters[brkcmd::c##I] )

// Initialisation avec rattachement à l'interface 'Frontend'.
void master_module::Init( broker_ &Broker )
{
	untyped_module::Init( NULL );
	untyped_module::Broker_ = &Broker;

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

namespace broker {

	type__ broker::Type( str_string_ &Name )
	{
		POSITION__ C = Modules.First();

		while ( ( C != NONE ) && ( str_string( Modules(C)->Name() ) != Name )  )
			C = Modules.Next( C );

		if ( C == NONE )
			C = BROKER_INVALID_TYPE;
		else if ( C > BROKER_INVALID_TYPE )
			ERRl();

		return (ttype__)C;
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

		if ( ( O() >= Links.Amount() ) && ( O != BROKER_MASTER_OBJECT ) )
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
