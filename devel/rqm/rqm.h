/*
  Header for the 'rqm' library by Claude L. Simon (epeios@epeios.org)
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

#ifndef RQM__INC
#define RQM__INC

#define RQM_NAME		"RQM"

#define	RQM_VERSION	"$Revision$"	

#define RQM_OWNER		"the Epeios project (http://www.epeios.org/)"

#include "ttr.h"

extern class ttr_tutor &RQMTutor;

#if defined( XXX_DBG ) && !defined( RQM_NODBG )
#define RQM_DBG 
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

//D ReQuest Manager.

#include <stdarg.h>

#include "err.h"
#include "bso.h"
#include "set.h"
#include "str.h"
#include "ctn.h"
#include "broker0.h"

#define RQM_INVALID_COMMAND		BSO_USHORT_MAX
#define RQM_COMMAND_MAX			( RQM_INVALID_COMMAND - 1 )

#define RQM_INVALID_TYPE		BSO_USHORT_MAX
#define RQM_TYPE_MAX			( RQM_INVALID_TYPE - 1 )

#define RQM_INVALID_OBJECT		BSO_USHORT_MAX
#define RQM_OBJECT_MAX			( RQM_INVALID_OBJECT - 1 )

typedef bso__ushort		rqm__tcommand;
//t Command
TYPEDEF( rqm__tcommand, rqm__command );

typedef bso__ushort		rqm__tobject;
//t Object
TYPEDEF( bso__ushort, rqm__object );

typedef bso__ushort		rqm__ttype;
//t Type
TYPEDEF( rqm__ttype, rqm__type );

//t Cast
typedef bso__ubyte rqm__cast;

// Différentes énumération.
namespace rqm
{
	using namespace broker0;
};

// Cast name. Internal use.
extern const char *(&RQMCastsNames)[rqm::c_amount];

//c The description of a request.
class rqm_description_
{
public:
	//o Name of the command.
	str_string_ Name;
	/*o Parameters and return value types. Separated by 'rqm::cEnd'. The 'agr::cEnd'
	to signalize the end of the request must NOT be put. */
	SET_( rqm__cast ) Casts;
	struct s
	{
		str_string_::s Name;
		SET_( rqm__cast )::s Casts;
		// User pointer.
		void *UP;
	} &S_;
	rqm_description_( s &S )
	: S_( S ),
	  Name( S.Name ),
	  Casts( S.Casts )
	{}
	void reset( bool P = true )
	{
		Name.reset( P );
		Casts.reset( P );
	}
	rqm_description_ &operator =( const rqm_description_ &D )
	{
		Name = D.Name;
		Casts = D.Casts;

		return *this;
	}
	void plug( mmm_multimemory_ &M )
	{
		Name.plug( M );
		Casts.plug( M );
	}
	//f Initialization. The name ans casts are added one per one.
	void Init( void )
	{
		Name.Init();
		Casts.Init();
	}
	/*f Initializing with name 'Name' and a list of casts. The list must
	contain 2 'rqm::cEnd', the first at the end of the parameters casts,
	and 1 of the end of returned values casts. 'Cast' is only here to avoid 
	ambiguity with the next function. */
	void Init(
		const char *Name,
		rqm::cast Cast,
		... );
	/*f Initializing with name 'Name' and a the list of casts 'Casts'. The list
	must contain 2 'rqm::cEnd', the first at the end of the parameters casts,
	and 1 of the end of returned values casts. */
	void Init(
		const char *Name,
		const rqm::cast *Casts );
	//f Adding the cast 'Cast'.
	void Add( rqm::cast Cast )
	{
		Casts.Add( Cast );
	}
	//f Return the amount of casts.
	SIZE__ Amount( void ) const
	{
		return Casts.Amount();
	}
	//f Return the cas at position 'Position'.
	rqm::cast Read( POSITION__ Position ) const
	{
		return (rqm::cast)Casts( Position );
	}
};

AUTO( rqm_description )

//c Request descriptions.
class rqm_descriptions_
: public CONTAINER_( rqm_description_ )
{
public:
	//o The descriptions
	struct s
	: public CONTAINER_( rqm_description_ )::s
	{};
	rqm_descriptions_( s &S )
	: CONTAINER_( rqm_description_ )( S )
	{}
	void reset( bool P = true )
	{
		CONTAINER_( rqm_description_ )::reset( P );
	}
	void plug( mmm_multimemory_ &M )
	{
		CONTAINER_( rqm_description_ )::plug( M );
	}
	rqm_descriptions_ &operator =( const rqm_descriptions_ &D )
	{
		CONTAINER_( rqm_description_ )::operator =( D );

		return *this;
	}
	//f Initialization.
	void Init( void )
	{
		CONTAINER_( rqm_description_ )::Init();
	}
	//f Adding 'Description'. Return the position where added.
	POSITION__ Add( const rqm_description_ &Description )
	{
		POSITION__ Position = CONTAINER_( rqm_description_ )::New();

		CONTAINER_( rqm_description_ )::Write( Description, Position );

		return Position;
	}
	/*f See 'rqm_descriptions_::Add()' for the descriptions of the parameters.
	return the position where request description added. 'Cast' is only here to avoid
	ambihity with the n,ext function. */
	POSITION__ Add(
		const char *Name,
		rqm::cast Cast,
		... );
	/*f See 'rqm_descriptions_::Add()' for the descriptions of the parameters.
	return the position where request description added. */
	POSITION__ Add(
		const char *Name,
		const rqm::cast *Casts )
	{
		POSITION__ Position = CONTAINER_( rqm_description_ )::New();
		rqm_description_ &Description = CONTAINER_( rqm_description_ )::operator()( Position );

		Description.Init( Name, Casts );
		CONTAINER_( rqm_description_ )::Sync();

		return Position;
	}
	/*f Return the position of the description of the request named 'Name' or 
	'NONE' if non-existant. */
	POSITION__ Position( const char *Name ) const;
	/*f Return the position of the description 'Description'. */
	POSITION__ Position( const rqm_description_ &Description ) const;
	//f Return the amount of descriptions.
	SIZE__ Amount( void ) const
	{
		return CONTAINER_( rqm_description_ )::Amount();
	}
};

AUTO( rqm_descriptions )


/*f Put the object id. 'Object' in 'Request'. You must begin with this function
to create a request. */
inline void RQMPutObject(
	rqm__object Object,
	flw::oflow___ &Request )
{
	flw::Put( Object, Request );
}

//f Put command id 'Command' in request. Succeed immediatly after 'RQMPutObject()'.
inline void RQMPutCommand(
	rqm__command Command,
	flw::oflow___ &Request )
{
	flw::Put( Command, Request );
}

//f Add to 'Request' the value 'Value' of type 'Type'.
void RQMAddValue(
	rqm::cast Cast,
	const void *Value,
	flw::oflow___ &Request );

/*f Add cast id. 'Cast' to request. Usually used to put 'agr::cMulti' and the
required 'agr::cEnd'. */
inline void RQMAddCast(
	rqm::cast Cast,
	flw::oflow___ &Request )
{
	rqm__cast C = Cast;

	flw::Put( C, Request );
}

//f Complete 'Request'.
inline void RQMComplete( flw::oflow___ &Request )
{
	RQMAddCast( rqm::cEnd, Request );
	Request.Synchronize();
}

//f Return the id of the cast 'Cast', or 'agr::cInvalid' if non-existent.
rqm::cast RQMIdCaste( const str_string_ &Caste );


//c A request manager.
class rqm__request_manager_
{
private:
	const rqm_description_ *Description_;
	// Concerne le canal de sortie.
	POSITION__ Position_;
	// A vrai si paramètre 'Multi', faux sinon.
	bso__bool Multi_;
	// La réponse est close (la marque de fin de réponse est placée).
	bso__bool Clot_;
	// Canal en entrée.
	rqm::cast Caste_;
	// A vrai si toute la requête a été lue.
	bso__bool Lu_;
	flw::iflow___ *Entree_;
	// Canal en sortie.
	flw::oflow___ *Sortie_;
	rqm::cast GetCast_( void )
	{
		rqm__cast Caste = Caste_;

		if ( Caste != rqm::cInvalid )
			Caste_ = rqm::cInvalid;
		else
		{
			flw::Get( *Entree_, Caste );

			if ( !Multi_ && ( Caste != Description_->Read( Position_++ ) ) )
				ERRu();
		}

		return (rqm::cast)Caste;
	}
	void GetValue_(
		rqm::cast Caste,
		void *Valeur );
	void TestFinLecture_( bso__bool ExplanationMessage = false )
	{
		if ( !Lu_ )
			if ( GetCast_() != rqm::cEnd )
				ERRu();
			else if ( !ExplanationMessage )
				Sortie_->Put( 0 );	// Empty explaination message.

		Lu_ = true;
	}

public:
	void reset( bool P = true )
	{
		Description_ = NULL;

		Position_ = UYM_UNREACHABLE_POSITION;
		Multi_ = false;
		Clot_ = true;
		Caste_ = rqm::cInvalid;
		Lu_ = false;

		Entree_ = NULL;
		Sortie_ = NULL;
	}
	rqm__request_manager_( void )
	{
		reset( false );
	}
	~rqm__request_manager_( void )
	{
		reset( true );
	}
	/*f Initialization with 'IFlow' to read the request,
	and 'OFlow' to write the answer. */
	void Init(
		flw::iflow___ &IFlow,
		flw::oflow___ &OFlow )
	{
		reset();

		Clot_ = false;

		Entree_ = &IFlow;
		Sortie_ = &OFlow;
	}
	/*f Initialization with flow 'IOFlow' to write the answer and read
	the request. */
	void Init( flw::ioflow___ &IOFlow )
	{
		Init( IOFlow, IOFlow );
	}
	//f 'Description' is the description of the request being handled.
	void SetDescription( const rqm_description_ &Description )
	{
		Description_ = &Description;
		Position_ = 0;
	}
	/*f Put to 'Value' the next return value which is of cast 'Cast'.
	Return if the value red is the last of a multi-parameters session. */
	bso__bool GetValue(
		rqm::cast Cast,
		void *Value );
	//f The request is complet.
	void Complete( void )
	{
		if ( Clot_ )
			return;

		if ( Description_ ) /* If == 'NULL', it means that the request was handled
							   by handling DIRECTLY the underlying flows. */
		{
			TestFinLecture_();

			if ( Position_ != Description_->Amount() )
				ERRu();

			if ( Multi_ )
				ERRu();

		}

		RQMAddCast( rqm::cEnd, *Sortie_ );

		Clot_ = true;

		Lu_ = false;

		Sortie_->Synchronize();
	}
	//f Add 'Value' of type 'Type'.
	void AddValue(
		rqm::cast Cast,
		const void *Value )
	{
		TestFinLecture_();

		if ( !Multi_ )
			if ( ( Position_ >= Description_->Amount() )
				|| ( ( Position_ == 0 ) && ( Description_->Read( Position_++ ) != rqm::cEnd ) )
				 || ( Description_->Read( Position_ ) != Cast ) )
				ERRu();
			else
				Position_++;

		RQMAddValue( Cast, Value, *Sortie_ );
	}
	//f Signalize that a multi-parameters session begins.
	void BeginMulti( void )
	{
		TestFinLecture_();

		if ( Multi_ )
			ERRu();

		if ( ( Position_ >= Description_->Amount() )
			  || ( Description_->Read( Position_ ) != rqm::cMulti ) )
			ERRu();
		else
			Position_++;


		RQMAddCast( rqm::cMulti, *Sortie_) ;

		Multi_ = true;
	}
	//f Signalize the end of a multi-parameters session.
	void EndMulti( void )
	{
		if ( !Multi_ )
			ERRu();

		RQMAddCast( rqm::cEnd, *Sortie_ ) ;

		Multi_ = false;
	}
	//f Send a message that explain the reason of no treatment.
	void SendExplanationMessage( const char *Message )
	{
		TestFinLecture_( true );

		if ( !Message[0] )
			ERRu();

		flw::PutString( Message, *Sortie_ );

		Clot_ = true;

		Sortie_->Synchronize();
	}
	//f Return the flow used to read the request.
	flw::iflow___ &Input( void )
	{
		return *Entree_;
	}
	//f Return the flow used to wrtie the answer.
	flw::oflow___ &Output( void )
	{
		return *Sortie_;
	}
};


/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
