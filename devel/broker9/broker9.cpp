/*
  'broker9' library by Claude L. Simon (epeios@epeios.org)
  Requires the 'broker9' header file ('broker9.h').
  Copyright (C) 2000 Claude L. SIMON (epeios@epeios.org).

  This file is part of the Epeios (http://www.epeios.org/) project.
  

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 3
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

#define BROKER9__COMPILATION

#include "broker9.h"

class broker9tutor
: public ttr_tutor
{
public:
	broker9tutor( void )
	: ttr_tutor( BROKER9_NAME )
	{
#ifdef BROKER9_DBG
		Version = BROKER9_VERSION " (DBG)";
#else
		Version = BROKER9_VERSION;
#endif
		Owner = BROKER9_OWNER;
		Date = "$Date$";
	}
	virtual ~broker9tutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#include "broker1.h"

static const char *PrimaryCommandName = BROKER1CommandsNames[broker1::cGetCommand];

POSITION__ broker9_descriptions_::Add(
	const char *Name,
	void *UP,
	rqm::cast Cast,
	... )
{
	POSITION__ Position = rqm_descriptions_::New();
	va_list L;
	rqm_description_ &Description = rqm_descriptions_::operator()( Position );

	Description.Init();
	Description.Name = Name;

	Description.Add( Cast );

	va_start( L, Cast );

	while ( Cast != rqm::cEnd ) { 
		Cast = va_arg( L, rqm::cast );
		Description.Add( Cast );
	}

	while ( ( Cast = va_arg( L, rqm::cast ) ) != rqm::cEnd )
		Description.Add( Cast );

	va_end( L );

	rqm_descriptions_::Sync();

	if ( UPs.Add( UP ) != Position )
		ERRc();

	return Position;
}

POSITION__ broker9_descriptions_::Add(
	const char *Name,
	void *UP,
	const rqm::cast *Casts )
{
	POSITION__ Position = rqm_descriptions_::New();
	rqm_description_ &Description = rqm_descriptions_::operator()( Position );

	Description.Init( Name, Casts );

	rqm_descriptions_::Sync();

	if ( UPs.Add( UP ) != Position )
		ERRc();

	return Position;
}



POSITION__ broker9_frontend::Add(
	const char *Name,
	broker9__function FP,
	rqm::cast Cast,
	... )
{
	POSITION__ Position = Master_.Descriptions.New();
	va_list L;
	rqm_description_ &Description = Master_.Descriptions.operator()( Position );

	Description.Init();
	Description.Name = Name;

	Description.Add( Cast );

	va_start( L, Cast );

	while ( Cast != rqm::cEnd ) {
		Cast = va_arg( L, rqm::cast );
		Description.Add( Cast );
	}

	while ( ( Cast = va_arg( L, rqm::cast ) ) != rqm::cEnd )
		Description.Add( Cast );

	va_end( L );

	Master_.Descriptions.Sync();

	if ( Master_.Descriptions.UPs.Add( (void *)FP ) != Position )
		ERRc();

	return Position;
}

POSITION__ broker9_frontend::Add(
	const char *Name,
	broker9__function FP,
	const rqm::cast *Casts )
{
	POSITION__ Position = Master_.Descriptions.New();
	rqm_description_ &Description = Master_.Descriptions.operator()( Position );

	Description.Init( Name, Casts );

	Master_.Descriptions.Sync();

	if ( Master_.Descriptions.UPs.Add( (void *)FP ) != Position )
		ERRc();

	return Position;
}

struct master__data {
	bso__bool Deconnexion;
	void *UP;
};

void broker9_master_module::Handle_(
	broker9__index,
	rqm__request_manager_ &Requete,
	void *PU )
{
	broker9__command C;
	master__data &MasterData = *(master__data *)PU;

	flw::Get( Requete.Input(), C );

	if (  C() < Descriptions.Amount() )
	{
		Requete.SetDescription( Descriptions( C() ) );
		(*(broker9__function)Descriptions.UP( C() ))( *Frontend(), *this, C, Requete, MasterData.Deconnexion, MasterData.UP );
	}
	else if ( C == BROKER9_MASTER_COMMAND )
	{
		char Car;

		if ( ( Car = Requete.Input().Get() ) != 0 )
		{
ERRProlog
			broker9__cast Caste;
			str_string S;
ERRBegin
			Requete.Output().Put( 0 );	// No explanation message;

			do {
				S.Init();

				S.Add( Car );

				while( ( Car = Requete.Input().Get() ) != 0 )
					S.Add( Car );

				Caste = RQMIdCaste( S );

				flw::Put( Caste, Requete.Output() );

			} while ( ( Car = Requete.Input().Get() ) != 0 );
ERRErr
ERREnd
ERREpilog
		}
		else
		{
			POSITION__ P = Descriptions.First();
			broker9__command C = BROKER9_INVALID_COMMAND;

			while ( ( P != NONE ) && ( Descriptions( P ).Name != str_string( PrimaryCommandName ) ) )
				P = Descriptions.Next( P );

			if ( P != NONE )
				C = (broker9__tcommand)P;

			Requete.Output().Put( 0 );	// No explanation message;

			flw::Put( C, Requete.Output() );

		}
	}
	else
		ERRb();
}


// Donne la liste des identificateurs et des libellés des langues.
static void GetLanguagesIDAndName(
	broker9_frontend &Frontend,
	broker9_untyped_module &Module,
	broker9__command Command,
	rqm__request_manager_ &Requete,
	bso__bool &,
	void * )
{
ERRProlog
	bso__ushort IdLangue;
	str_string Name;
ERRBegin
	Requete.BeginMulti();

	IdLangue = lgg::lDefault;
	Requete.AddValue( rqm::cId16, &IdLangue );
	
	Name.Init();
	Name = LGGLanguageName[IdLangue];
	Requete.AddValue( rqm::cString, &Name );

	for ( bso__ushort I = 0; I < Frontend.Langues_.Nombre; I++ )
	{
		IdLangue = Frontend.Langues_.Identificateurs[I];
		Name.Init();
		Name = LGGLanguageName[IdLangue];
		Requete.AddValue( rqm::cString, &Name );
	}

	Requete.EndMulti();
ERRErr
ERREnd
ERREpilog
}

// Donne la liste des identificateurs et des libellés de types
static void GetTypesIDAndName(
	broker9_frontend &Frontend,
	broker9_untyped_module &Module,
	broker9__command Command,
	rqm__request_manager_ &Requete,
	bso__bool &,
	void * )
{
ERRProlog
	broker9__type Type;
	POSITION__ P = Frontend.Modules.First();
	str_string Name;
ERRBegin
	Requete.BeginMulti();

	while( P != NONE )
	{
		Type = (rqm__ttype)P;
		Requete.AddValue( rqm::cType, &Type );
		
		Name.Init();
		Name = Frontend.Name( Type );
		Requete.AddValue( rqm::cString, &Name );
		P = Frontend.Modules.Next( P );
	}

	Requete.EndMulti();
ERRErr
ERREnd
ERREpilog
}

static void WriteCommandsIDAndName_(
	const broker9_descriptions_ &Descriptions,
	rqm__request_manager_ &Requete )
{
	broker9__command Command;
	CITEM( broker9_description_ ) Description;
	POSITION__ P = Descriptions.First();

	Description.Init( Descriptions );

	Requete.BeginMulti();

	while( P != NONE )
	{
		Command = (broker9__tcommand)P;
		Requete.AddValue( rqm::cCommand, &Command );
		Requete.AddValue( rqm::cString, &Description( P ) );
		P = Descriptions.Next( P );
	}

	Requete.EndMulti();
}


static void GetCommandsIDAndName(
	broker9_frontend &Frontend,
	broker9_untyped_module &Module,
	broker9__command Command,
	rqm__request_manager_ &Requete,
	bso__bool &,
	void * )
{
	broker9__type Type;

	Requete.GetValue( rqm::cType, &Type );

	WriteCommandsIDAndName_( Frontend.Module( Type ).Descriptions, Requete );
}

static inline void WriteParameters_(
	const broker9_description_ &Description,
	rqm__request_manager_ &Requete )
{
	Requete.AddValue( rqm::cCasts, &Description.Casts );
}


static void WriteParameters_(
	const broker9_descriptions_ &Descriptions,
	broker9__command Command,
	rqm__request_manager_ &Requete )
{
	CITEM( broker9_description_ ) Description;

	Description.Init( Descriptions );

	WriteParameters_( Description( Command() ), Requete );
}


static void GetParameters(
	broker9_frontend &Frontend,
	broker9_untyped_module &Module,
	broker9__command,
	rqm__request_manager_ &Requete,
	bso__bool &,
	void * )
{
	broker9__type Type;
	broker9__command Command;

	Requete.GetValue( rqm::cType, &Type );
	Requete.GetValue( rqm::cCommand, &Command );

	WriteParameters_( Frontend.Module( Type ).Descriptions, Command, Requete );
}


// Donne le libellés de la version de l'interface.
static void GetVersion(
	broker9_frontend &Frontend,
	broker9_untyped_module &Module,
	broker9__command Command,
	rqm__request_manager_ &Requete,
	bso__bool &,
	void * )
{
ERRProlog
	str_string Version;
ERRBegin
	Version.Init();
	Version = Frontend.Version_;

	Requete.AddValue( rqm::cString, &Version );
ERRErr
ERREnd
ERREpilog
}

// Définit le comportement provoqué par une erreur et retourne son ancien comportement.
static void SetErrorBreakingStatus(
	broker9_frontend &Frontend,
	broker9_untyped_module &Module,
	broker9__command Command,
	rqm__request_manager_ &Requete,
	bso__bool &,
	void * )
{
	bso__bool Retour;
	bso__bool O;

	Retour = (bso__raw)Frontend.ErrorBreaking();

	Requete.GetValue( rqm::cBoolean, &O );

	Frontend.ErrorBreaking( O != 0  );

	Requete.AddValue( rqm::cBoolean, &Retour );

	Requete.Complete();
}

// Retourne l'error breaking.
static void GetErrorBreakingStatus(
	broker9_frontend &Frontend,
	broker9_untyped_module &Module,
	broker9__command Command,
	rqm__request_manager_ &Requete,
	bso__bool &,
	void * )
{
	bso__bool Retour;

	Retour = (bso__raw)Frontend.ErrorBreaking();

	Requete.AddValue( rqm::cBoolean, &Retour );

	Requete.Complete();
}

// Retourne un nouvel objet.
static void GetNewObject(
	broker9_frontend &Frontend,
	broker9_untyped_module &Module,
	broker9__command Command,
	rqm__request_manager_ &Requete,
	bso__bool &,
	void * )
{
	static broker9__object O;
	broker9__type T;

	Requete.GetValue( rqm::cType, &T );

	if ( T() >= Frontend.Modules.Amount() )
		ERRb();

	if ( T == BROKER9_INVALID_TYPE )
		ERRb();

	O = Frontend.New( T );

	if ( O() != BROKER9_INVALID_TYPE )
		Requete.AddValue( rqm::cObject, &O );
	else
		Requete.SendExplanationMessage( "No such object type." );

	Requete.Complete();
}

// Retourne l'identificateur correspondant à un type donné.
static void GetType(
	broker9_frontend &Frontend,
	broker9_untyped_module &Module,
	broker9__command Command,
	rqm__request_manager_ &Requete,
	bso__bool &,
	void * )
{
ERRProlog
	str_string Type;
	static broker9__type T;
ERRBegin
	Type.Init();

	Requete.GetValue( rqm::cString, &Type );

	T = Frontend.Type( Type );

	if ( ( T() != BROKER9_INVALID_TYPE )  )
		Requete.AddValue( rqm::cType, &T );
	else
		Requete.SendExplanationMessage( "No such object type name." );

	Requete.Complete();
ERRErr
ERREnd
ERREpilog
}

// Supprime un objet.
static void RemoveObject(
	broker9_frontend &Frontend,
	broker9_untyped_module &Module,
	broker9__command Command,
	rqm__request_manager_ &Requete,
	bso__bool &,
	void * )
{
	broker9__object O;

	Requete.GetValue( rqm::cObject, &O );

/*	if ( !Frontend.Valide( O ) )
		ERRb();
*/
	Frontend.Remove( O );

	Requete.Complete();
}

static void GetDescriptions_(
	rqm__request_manager_ &Manager,
	rqm_descriptions_ &Descriptions )
{
ERRProlog
	rqm_description Description;
ERRBegin
	Description.Init();

	if ( Manager.GetValue( rqm::cString, &Description.Name ) )
		ERRb();

	while( !Manager.GetValue( rqm::cCasts, &Description.Casts ) )
	{
		Descriptions.Add( Description );

		Description.Init();

		if ( Manager.GetValue( rqm::cString, &Description.Name ) )
			ERRb();
	}

	Descriptions.Add( Description );
ERRErr
ERREnd
ERREpilog
}

static void SendCommandIDs_(
	broker9__type Type,
	broker9_frontend &Frontend,
	const rqm_descriptions_ &Descriptions,
	rqm__request_manager_ &Manager )
{
ERRProlog
	broker9__command Command;
	POSITION__ Position = Descriptions.First();
	CITEM( rqm_description_ ) Description;
ERRBegin
	Description.Init( Descriptions );

	Manager.BeginMulti();

	while( Position != NONE )
	{
		if ( ( Command = Frontend.Command( Type, Description( Position ) ) ) == BROKER9_INVALID_COMMAND )
			ERRb();

		Manager.AddValue( rqm::cCommand, &Command );

		Position = Descriptions.Next( Position );
	}

	Manager.EndMulti();

	Manager.Complete();
ERRErr
ERREnd
ERREpilog
}



// Retourne l'identificateur de type et les identificateurs de commande demandé.
static void GetTypeAndCommands(
	broker9_frontend &Frontend,
	broker9_untyped_module &Module,
	broker9__command Command,
	rqm__request_manager_ &Requete,
	bso__bool &,
	void * )
{
ERRProlog
	rqm_descriptions Descriptions;
	broker9__type Type;
	str_string TypeName;
ERRBegin
	Descriptions.Init();
	TypeName.Init();

	Requete.GetValue( rqm::cString, &TypeName );

	if ( ( Type = Frontend.Type( TypeName ) ) == BROKER9_INVALID_TYPE )
		ERRb();

	GetDescriptions_( Requete, Descriptions );

	Requete.AddValue( rqm::cType, &Type );

	SendCommandIDs_( Type, Frontend, Descriptions, Requete );
ERRErr
ERREnd
ERREpilog
}


// Retourne l'identificateur d'une commande donnée pour un type donné.
static void GetCommand(
	broker9_frontend &Frontend,
	broker9_untyped_module &Module,
	broker9__command Command,
	rqm__request_manager_ &Requete,
	bso__bool &,
	void * )
{
ERRProlog
	rqm_description Description;
	broker9__type T;
	broker9__command C;
ERRBegin
	Description.Init();

	Requete.GetValue( rqm::cType, &T );
	Requete.GetValue( rqm::cString, &Description.Name );
	Requete.GetValue( rqm::cCasts, &Description.Casts );

/*	if ( !Frontend.Valide( T ) )
		if ( T() != BROKER9_TYPE_MAITRE )
			ERRb();
*/
	C = Frontend.Command( T, Description );

	if ( ( C() != BROKER9_INVALID_COMMAND ) )
		Requete.AddValue( rqm::cCommand, &C );
	else
		Requete.SendExplanationMessage( "No such command name or with such description" );

	Requete.Complete();
ERRErr
ERREnd
ERREpilog
}


// Retourne l'identificateur des commandes demandées pour un type donné.
static void GetCommands(
	broker9_frontend &Frontend,
	broker9_untyped_module &Module,
	broker9__command Command,
	rqm__request_manager_ &Requete,
	bso__bool &,
	void * )
{
ERRProlog
	rqm_descriptions Descriptions;
	broker9__type Type;
ERRBegin
	Descriptions.Init();

	Requete.GetValue( rqm::cType, &Type );

	GetDescriptions_( Requete, Descriptions );

	SendCommandIDs_( Type, Frontend, Descriptions, Requete );
ERRErr
ERREnd
ERREpilog
}


// Deconnection
static void Disconnect(
	broker9_frontend &Frontend,
	broker9_untyped_module &Module,
	broker9__command Command,
	rqm__request_manager_ &Requete,
	bso__bool &Deconnexion,
	void * )
{
	Deconnexion = true;
}

#define ADD( I )	Frontend.Add( BROKER1CommandsNames[broker1::c##I], ::##I, BROKER1CommandsParameters[broker1::c##I] )

// Initialisation avec rattachement à l'interface 'Frontend'.
void broker9_master_module::Init( broker9_frontend &Frontend )
{
	broker9_untyped_module::Init( NULL );
	broker9_untyped_module::Frontend_ = &Frontend;

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

broker9__type broker9_frontend::Type( str_string_ &Name )
{
	POSITION__ C = Modules.First();

	while ( ( C != NONE ) && ( str_string( Modules(C)->Name() ) != Name )  )
		C = Modules.Next( C );

	if ( C == NONE )
		C = BROKER9_INVALID_TYPE;
	else if ( C > BROKER9_INVALID_TYPE )
		ERRl();

	return (broker9__ttype)C;
}

bso__bool broker9_frontend::Handle(
	flw::iflow___ &Input,
	flw::oflow___ &Output,
	void *PU )
{
	master__data MasterData;
ERRProlog
	rqm__request_manager_ Requete;
	broker9__object O;
ERRBegin
	MasterData.Deconnexion = false;
	MasterData.UP = PU;

	Requete.Init( Input, Output );

	flw::Get( Input, O );

	if ( ( O() >= Links.Amount() ) && ( O != BROKER9_MASTER_OBJECT ) )
		ERRb();

	if ( O != BROKER9_MASTER_OBJECT )
		Module_( O ).Handle( Index_( O ), Requete, PU );
	else
		Master_.Handle( (broker9__index)0, Requete, &MasterData );

ERRErr
	Requete.SendExplanationMessage( ERRMessage() );

	if ( !Master_.ErrorBreaking() )
		ERRRst();
ERREnd
	Requete.Complete();
ERREpilog
	return !MasterData.Deconnexion;
}



/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class broker9personnalization
: public broker9tutor
{
public:
	broker9personnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~broker9personnalization( void )
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

static broker9personnalization Tutor;

ttr_tutor &BROKER9Tutor = Tutor;
