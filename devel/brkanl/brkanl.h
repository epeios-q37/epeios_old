/*
  Header for the 'brkanl' library by Claude L. Simon (epeios@epeios.org)
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

#ifndef BRKANL__INC
#define BRKANL__INC

#define BRKANL_NAME		"BRKANL"

#define	BRKANL_VERSION	"$Revision$"	

#define BRKANL_OWNER		"the Epeios project (http://www.epeios.org/)"

#include "ttr.h"

extern class ttr_tutor &BRKANLTutor;

#if defined( XXX_DBG ) && !defined( BRKANL_NODBG )
#define BRKANL_DBG 
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

//D BRoKer ANaLyzer. Analyze the contents of a broker.

#include "err.h"
#include "flw.h"
#include "broker8.h"

namespace brkanl {
	//t Broker parameters.
	typedef broker8_casts_ 	broker_parameters_;
	typedef broker8_casts	broker_parameters;

	//c A broker Command.
	class broker_command_
	{
	public:
		//o The command identification.
		broker8_command_item_ Identification;
		//o The parameters.
		broker_parameters_ Parameters;
		struct s {
			broker8_command_item_::s Identification;
			broker_parameters_::s Parameters;
		};
		broker_command_( s &S )
		: Identification ( S.Identification ),
		  Parameters( S.Parameters )
		 {}
		void reset( bool P = true )
		{
	 		Identification.reset( P );
			Parameters.reset( P );
		}
		void plug( mmm_multimemory_ &M )
		{
			Identification .plug( M );
			Parameters.plug( M );
		}
		broker_command_ &operator =( const broker_command_ &BC )
		{
			Identification = BC.Identification;
			Parameters = BC.Parameters;

			return *this;
		}
		//f Initialization.
		void Init( void )
		{
			Identification.Init();
			Parameters.Init();
		}
	};

	AUTO( broker_command )

	//t Broker commands.
	typedef XCONTAINER_( broker_command_ ) broker_commands_;
	typedef XCONTAINER( broker_command_ ) broker_commands;

	//c A broker object type.
	class broker_type_
	{
	public:
		//o The type identification.
		broker8_type_item_ Identification;
		//o The commands of the type.
		broker_commands_ Commands;
		struct s {
			broker8_type_item_::s Identification;
			broker_commands_::s Commands;
		};
		broker_type_( s &S )
		: Identification( S.Identification ),
		  Commands( S.Commands )
		{}
		void reset( bool P = true )
		{
			Identification.reset( P );
			Commands.reset( P );
		}
		void plug( mmm_multimemory_ &M )
		{
			Identification.plug( M );
			Commands.plug( M );
		}
		broker_type_ &operator =( const broker_type_ &BT )
		{
			Identification = BT.Identification;
			Commands = BT.Commands;

			return *this;
		}
		//f Initialization.
		void Init( void )
		{
			Identification.Init();
			Commands.Init();
		}
	};

	AUTO( broker_type )

	//t Broker types.
	typedef XCONTAINER_( broker_type_ ) broker_types_;
	typedef XCONTAINER( broker_type_ ) broker_types;

	//f Fill 'Types' with contents of 'Broker'.
	void Analyze(
		broker8__core_ &Broker,
		broker_types_ &Types );	
}
	
/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
