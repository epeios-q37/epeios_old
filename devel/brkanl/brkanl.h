/*
  Header for the 'brkanl' library by Claude L. Simon (epeios@epeios.org)
  Copyright (C) 2000 Claude L. SIMON (epeios@epeios.org) 

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

//D Obsolete. Use 'BKDDCB' instead.

#error Obsolete. Use 'BKDDCB' instead.

#include "err.h"
#include "flw.h"
#include "frtend.h"
#include "bso.h"
#include "brktpm.h"

namespace brkanl {
	//t Broker parameter.
	typedef frtend::id8__	parameter__;

	//t Broker parameters.
	typedef frtend::ids8_ 	parameters_;
	typedef frtend::ids8	parameters;

	BRKTPM_ITEM( bso__ushort, command_item )
	BRKTPM_ITEMS( command_item, commands_items )
	BRKTPM_ITEM( bso__ushort, type_item )
	BRKTPM_ITEMS( type_item, types_items )
	
	//c A broker Command.
	class command_
	{
	public:
		//o The command identification.
		command_item_ Identification;
		//o The parameters.
		parameters_ Parameters;
		struct s {
			command_item_::s Identification;
			parameters_::s Parameters;
		};
		command_( s &S )
		: Identification ( S.Identification ),
		  Parameters( S.Parameters )
		 {}
		void reset( bool P = true )
		{
	 		Identification.reset( P );
			Parameters.reset( P );
		}
		void plug( mmm::multimemory_ &M )
		{
			Identification .plug( M );
			Parameters.plug( M );
		}
		command_ &operator =( const command_ &BC )
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

	AUTO( command )

	//t Broker commands.
	typedef ctn::E_XCONTAINER_( command_ ) commands_;
	typedef ctn::E_XCONTAINER( command_ ) commands;

	//c A broker object type.
	class type_
	{
	public:
		//o The type identification.
		type_item_ Identification;
		//o The commands of the type.
		commands_ Commands;
		struct s {
			type_item_::s Identification;
			commands_::s Commands;
		};
		type_( s &S )
		: Identification( S.Identification ),
		  Commands( S.Commands )
		{}
		void reset( bool P = true )
		{
			Identification.reset( P );
			Commands.reset( P );
		}
		void plug( mmm::multimemory_ &M )
		{
			Identification.plug( M );
			Commands.plug( M );
		}
		type_ &operator =( const type_ &BT )
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

	AUTO( type )

	//t Broker types.
	typedef ctn::E_XCONTAINER_( type_ ) types_;
	typedef ctn::E_XCONTAINER( type_ ) types;

	//f Fill 'Types' with contents of 'Broker'.
	void Analyze(
		frtend::frontend___ &Frontend,
		types_ &Types );	
}
	
/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
