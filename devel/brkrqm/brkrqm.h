/*
  Header for the 'brkrqm' library by Claude L. Simon (epeios@epeios.org)
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

#ifndef BRKRQM__INC
#define BRKRQM__INC

#define BRKRQM_NAME		"BRKRQM"

#define	BRKRQM_VERSION	"$Revision$"	

#define BRKRQM_OWNER		"the Epeios project (http://www.epeios.org/)"

#include "ttr.h"

extern class ttr_tutor &BRKRQMTutor;

#if defined( XXX_DBG ) && !defined( BRKRQM_NODBG )
#define BRKRQM_DBG 
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
//D BRoKer ReQueqt Manager.
/* End addendum to automatic documentation generation part. */


#include "err.h"
#include "flw.h"
#include "bso.h"
#include "str.h"
#include "ctn.h"
#include "brktpm.h"
#include "brkcst.h"

#ifdef CPE__VC
#	undef GetObject
#endif

#define BRKRQM_INVALID_COMMAND		BSO_USHORT_MAX
#define BRKRQM_COMMAND_MAX			( BRKRQM_INVALID_COMMAND - 1 )

#define BRKRQM_INVALID_TYPE			BSO_USHORT_MAX
#define BRKRQM_TYPE_MAX				( BRKRQM_INVALID_TYPE - 1 )

#define BRKRQM_INVALID_OBJECT		BSO_USHORT_MAX
#define BRKRQM_OBJECT_MAX			( BRKRQM_INVALID_OBJECT - 1 )

namespace brkrqm {
	namespace {
		using namespace brkcst;
	}

	using namespace brktpm;

	typedef id8__ cast__;
	using brkcst::cast;

	//c The description of a request.
	class description_
	{
	public:
		//o Name of the command.
		str_string_ Name;
		/*o Parameters and return value types. Separated by 'cEnd'. The 'cEnd'
		to signalize the end of the request must NOT be put. */
		ids8_ Casts;
		struct s
		{
			str_string_::s Name;
			ids8_::s Casts;
		} &S_;
		description_( s &S )
		: S_( S ),
		  Name( S.Name ),
		  Casts( S.Casts )
		{}
		void reset( bool P = true )
		{
			Name.reset( P );
			Casts.reset( P );
		}
		description_ &operator =( const description_ &D )
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
		/*f Initializing with name 'Name' and a the list of casts 'Casts'. The list
		must contain 2 'cEnd', the first at the end of the parameters casts,
		and 1 of the end of returned values casts. */
		void Init(
			const char *Name,
			const cast *Casts );
	};

	AUTO( description )

	//c Requests descriptions.
	class descriptions_
	: public XCONTAINER_( description_ )
	{
	public:
		struct s
		: public XCONTAINER_( description_ )::s
		{};
		descriptions_( s &S )
		: XCONTAINER_( description_ )( S )
		{}
		void reset( bool P = true )
		{
			XCONTAINER_( description_ )::reset( P );
		}
		void plug( mmm_multimemory_ &M )
		{
			XCONTAINER_( description_ )::plug( M );
		}
		descriptions_ &operator =( const descriptions_ &D )
		{
			XCONTAINER_( description_ )::operator =( D );

			return *this;
		}
		//f Initialization.
		void Init( void )
		{
			XCONTAINER_( description_ )::Init();
		}
		//f Adding 'Description'. Return the position where added.
		POSITION__ Add( const description_ &Description )
		{
			return XCONTAINER_( description_ )::Add( Description );
		}
		/*f See 'descriptions_::Add()' for the descriptions of the parameters.
		return the position where request description added. */
		POSITION__ Add(
			const char *Name,
			const cast *Casts )
		{
			description Description;

			Description.Init( Name, Casts );
			
			return Add( Description );
		}
		/*f Return the position of the description 'Description'. */
		POSITION__ Position( const description_ &Description ) const;
	};

	AUTO( descriptions )

	//c A request manager.
	class request_manager___
	{
	private:
		// The corresponding request description.
		const description_ *Description_;
		// Position in the Description_;
		POSITION__ Position_;
		// At true if all the answer be sent.
		bso__bool Closed_;
		// Cast buffer.
		cast Cast_;
		// At true if the request parsed.
		bso__bool Parsed_;
		// The input/output channel for the request.
		flw::ioflow___ *Channel_;
		void Test_( cast Cast )
		{
			if ( Description_->Casts.Read( Position_ ) != Cast )
				ERRu();

			Position_ = Description_->Casts.Next( Position_ );
		}
		void TestInput_( cast Cast )
		{
			Test_( Cast );

			if ( Channel_->Get() != Cast )
				ERRu();
		}
		void TestOutput_(
			cast Cast,
			bso__bool ExplanationMessage = false )
		{
			if ( !Parsed_ ) {
				Test_( cEnd );

				if ( !ExplanationMessage )
					Channel_->Put( 0 );	// Empty explanation message.
				else
					ERRb();

				Parsed_ = true;
			}

			Test_( Cast );
			Channel_->Put( Cast );
		}
	public:
		void reset( bool = true )
		{
			Description_ = NULL;

			Position_ = NONE;
			Closed_ = true;
			Cast_ = cInvalid;
			Parsed_ = false;

			Channel_ = NULL;
		}
		request_manager___( void )
		{
			reset( false );
		}
		~request_manager___( void )
		{
			reset( true );
		}
		//f Initialization with 'Channel' to parse/answer the request.
		void Init( flw::ioflow___ &Channel )
		{
			reset();

			Closed_ = false;

			Channel_ = &Channel;
		}
		//f 'Description' is the description of the request being handled.
		void SetDescription( const description_ &Description )
		{
			Description_ = &Description;
			Position_ = Description_->Casts.First();
		}
		//f Get 'Object'.
		void PopObject( object__ &Object )
		{
			TestInput_( cObject );
			brktpm::GetObject( *Channel_, Object );
		}
		//f Get 'Boolean'.
		void PopBoolean( boolean__ &Boolean )
		{
			TestInput_( cBoolean );
			brktpm::GetBoolean( *Channel_, Boolean );
		}
		//f Get 'Id8'.
		void PopId8( id8__ &Id8 )
		{
			TestInput_( cId8 );
			brktpm::GetId8( *Channel_, Id8 );
		}
		//f Get 'IDs8'.
		void PopIds8( ids8_ &Ids8 )
		{
			TestInput_( cIds8 );
			brktpm::GetIds8( *Channel_, Ids8 );
		}
		//f Get 'Id16'.
		void PopId16( id16__ &Id16 )
		{
			TestInput_( cId16 );
			brktpm::GetId16( *Channel_, Id16 );
		}
		//f Get 'IDs16'.
		void PopIds16( ids16_ &Ids16 )
		{
			TestInput_( cIds16 );
			brktpm::GetIds16( *Channel_, Ids16 );
		}
		//f Get 'Id32'.
		void PopId32( id32__ &Id32 )
		{
			TestInput_( cId32 );
			brktpm::GetId32( *Channel_, Id32 );
		}
		//f Get 'IDs32'.
		void PopIds32( ids32_ &Ids32 )
		{
			TestInput_( cIds32 );
			brktpm::GetIds32( *Channel_, Ids32 );
		}
		//f Get 'Char'.
		void PopChar( char__ &Char )
		{
			TestInput_( cChar );
			brktpm::GetChar( *Channel_, Char );
		}
		//f Get 'String'.
		void PopString( string_ &String )
		{
			TestInput_( cString );
			brktpm::GetString( *Channel_, String );
		}
		//f Get 'Strings'.
		void PopStrings( strings_ &Strings )
		{
			TestInput_( cStrings );
			brktpm::GetStrings( *Channel_, Strings );
		}
		//f Get 'Byte'.
		void PopByte( byte__ &Byte )
		{
			TestInput_( cByte );
			brktpm::GetByte( *Channel_, Byte );
		}
		//f Get 'Binary'.
		void PopBinary( binary_ &Binary )
		{
			TestInput_( cBinary );
			brktpm::GetBinary( *Channel_, Binary );
		}
		//f Get 'Binaries'.
		void PopBinaries( binaries_ &Binaries )
		{
			TestInput_( cBinaries );
			brktpm::GetBinaries( *Channel_, Binaries );
		}
		//f Get 'Items8'.
		void PopItems8( items8_ &Items8 )
		{
			TestInput_( cItems8 );
			brktpm::GetItems8( *Channel_, Items8 );
		}
		//f Get 'Items16'.
		void PopItems16( items16_ &Items16 )
		{
			TestInput_( cItems16 );
			brktpm::GetItems16( *Channel_, Items16 );
		}
		//f Get 'Items32'.
		void PopItems32( items32_ &Items32 )
		{
			TestInput_( cItems32 );
			brktpm::GetItems32( *Channel_, Items32 );
		}
		//f Get 'CommandsDetails'.
		void PopCommandsDetails( commands_details_ &CommandsDetails )
		{
			TestInput_( cCommandsDetails );
			brktpm::GetCommandsDetails( *Channel_, CommandsDetails );
		}
		//f Get 'ObjectsReferences'.
		void PopObjectsreferences( objects_references_ &ObjectsReferences )
		{
			TestInput_( cObjectsReferences );
			brktpm::GetObjectsReferences( *Channel_, ObjectsReferences );
		}
		//f Put 'Object'.
		void PushObject( object__ Object )
		{
			TestOutput_( cObject );
			brktpm::PutObject( Object, *Channel_ );
		}
		//f Put 'Boolean'.
		void PushBoolean( boolean__ Boolean )
		{
			TestOutput_( cBoolean );
			brktpm::PutBoolean( Boolean, *Channel_ );
		}
		//f Put 'Id8'.
		void PushId8( id8__ Id8 )
		{
			TestOutput_( cId8 );
			brktpm::PutId8( Id8, *Channel_ );
		}
		//f Put 'Ids8'.
		void PushIds8( const ids8_ &Ids8 )
		{
			TestOutput_( cIds8 );
			brktpm::PutIds8( Ids8, *Channel_ );
		}
		//f Put 'Id16'.
		void PushId16( id16__ Id16 )
		{
			TestOutput_( cId16 );
			brktpm::PutId16( Id16, *Channel_ );
		}
		//f Put 'Ids16'.
		void PushIds16( const ids16_ &Ids16 )
		{
			TestOutput_( cIds16 );
			brktpm::PutIds16( Ids16, *Channel_ );
		}
		//f Put 'Id32'.
		void PushId32( id32__ Id32 )
		{
			TestOutput_( cId32 );
			brktpm::PutId32( Id32, *Channel_ );
		}
		//f Put 'Ids32'.
		void PushIds8( const ids32_ &Ids32 )
		{
			TestOutput_( cIds32 );
			brktpm::PutIds32( Ids32, *Channel_ );
		}
		//f Put 'Char'.
		void PushChar( char__ Char )
		{
			TestOutput_( cChar );
			brktpm::PutChar( Char, *Channel_ );
		}
		//f Put 'String'.
		void PushString( const string_ &String )
		{
			TestOutput_( cString );
			brktpm::PutString( String, *Channel_ );
		}
		//f Put 'Strings'.
		void PushStrings( const strings_ &Strings )
		{
			TestOutput_( cStrings );
			brktpm::PutStrings( Strings, *Channel_ );
		}
		//f Put 'Byte'.
		void PushByte( byte__ Byte )
		{
			TestOutput_( cByte );
			brktpm::PutByte( Byte, *Channel_ );
		}
		//f Put 'Binary'.
		void PushBinary( const binary_ &Binary )
		{
			TestOutput_( cBinary );
			brktpm::PutBinary( Binary, *Channel_ );
		}
		//f Put 'Binaries'.
		void PushBinaries( const binaries_ &Binaries )
		{
			TestOutput_( cBinaries );
			brktpm::PutBinaries( Binaries, *Channel_ );
		}
		//f Put 'Items8'
		void PushItems8( const items8_ &Items8 )
		{
			TestOutput_( cItems8 );
			brktpm::PutItems8( Items8, *Channel_ );
		}
		//f Put 'Items16'
		void PushItems16( const items16_ &Items16 )
		{
			TestOutput_( cItems16 );
			brktpm::PutItems16( Items16, *Channel_ );
		}
		//f Put 'Items32'
		void PushItems32( const items32_ &Items32 )
		{
			TestOutput_( cItems32 );
			brktpm::PutItems32( Items32, *Channel_ );
		}
		//f Put 'CommandsDetails'.
		void PushCommandsDetails( const commands_details_ &CommandsDetails )
		{
			TestOutput_( cCommandsDetails );
			brktpm::PutCommandsDetails( CommandsDetails, *Channel_ );
		}
		//f Put 'ObjectsReferences'.
		void PushObjectsReferences( const objects_references_ &ObjectsReferences )
		{
			TestOutput_( cObjectsReferences );
			brktpm::PutObjectsReferences( ObjectsReferences, *Channel_ );
		}
		//f Tell that the request is complete (parsed and answered).
		void Complete( void )
		{
			if ( Closed_ )
				return;

			if ( Description_ != NULL ) /* If == 'NULL', it means that the request was handled
								   by handling DIRECTLY the underlying flows. */
			{
				if ( Channel_->Get() != brkcst::cEnd )
					ERRb();

				if ( Position_ != NONE )
					ERRu();
			}

			brktpm::PutId8( cEnd, *Channel_ );

			Closed_ = true;

			Parsed_ = false;

			Channel_->Synchronize();
		}
		//f Send a message that explain the reason of no treatment.
		void SendExplanationMessage( const char *Message )
		{
			TestOutput_( cEnd, true );

			if ( !Message[0] )
				ERRu();

			flw::PutString( Message, *Channel_ );

			Closed_ = true;

			Channel_->Synchronize();
		}
		//f Return the channel used to handle the request as input flow.
		flw::iflow___ &Input( void )
		{
			return *Channel_;
		}
		//f Return the channel used to handle the request as ouput flow.
		flw::oflow___ &Output( void )
		{
			return *Channel_;
		}
	};
};

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
