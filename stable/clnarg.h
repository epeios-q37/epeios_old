/*
	Header for the 'clnarg' library by Claude SIMON (csimon@epeios.org)
	Copyright (C) 2000-2001, 2003 Claude SIMON (csimon@epeios.org).

	This file is part of the Epeios (http://epeios.org/) project.

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

#ifndef CLNARG__INC
#define CLNARG__INC

#define CLNARG_NAME		"CLNARG"

#define	CLNARG_VERSION	"$Revision$"

#define CLNARG_OWNER		"Claude SIMON (csimon@epeios.org)"

#include "ttr.h"

extern class ttr_tutor &CLNARGTutor;

#if defined( XXX_DBG ) && !defined( CLNARG_NODBG )
#define CLNARG_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision$
//C Claude SIMON (csimon@epeios.org)
//R $Date$

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D Command LiNe ARGuments 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "bso.h"
#include "bch.h"
#include "ctn.h"
#include "str.h"


#define CLNARG_DEFAULT_FLAG	'-'

//d The command seperator for the general usage printing ('GetCommandLabels' method).
#define CLNARG_GENERAL_SEPARATOR	"|"

//d The command seperator for the detail usage printing ('GetCommandLabels' method).
#define CLNARG_DETAIL_SEPARATOR	", "

namespace clnarg {
	//t An option/argument id.
	typedef bso::ubyte__ id__;
	// if modified, modify below.

	//d Value the get an id when no value.
	#define CLNARG_NONE			BSO_UBYTE_MAX
	#define CLNARG_ID_MAX		( CLNARG_NONE - 1 )

	//d Value of no short option/command.
	#define CLNARG_NO_SHORT	0

	//d Value to gice if no long option/command.
	#define CLNARG_NO_LONG	NULL

	//e View mode
	enum view {
		//i All print on one line.
		vOneLine = 10,	// Begins at 10 to detect old boolean usage.
		//i Split in 2 lines.
		vSplit,
		//i Amount of view.
		v_amount
	};

	class item__ {
	public:
		char Short;
		const char *Long;
		id__ Id;
		item__( void )
		{
			Short = CLNARG_NO_SHORT;
			Long = NULL;
			Id = CLNARG_NONE;
		}
	};

	typedef item__ command__;

	typedef bch::E_BUNCH_( command__ ) commands_;
	typedef bch::E_BUNCH( command__ ) commands;

	typedef item__ option__;

	typedef bch::E_BUNCH_( option__ ) options_;
	typedef bch::E_BUNCH( option__ ) options;

	//c Description of the available command line options and arguments.
	class description_
	{
	public:
		commands_ Commands;
		options_ Options;
		struct s {
			commands_::s Commands;
			options_::s Options;
		};
		description_( s &S )
		: Commands( S.Commands ),
		  Options( S.Options )
		{}
		void reset( bool P = true )
		{
			Commands.reset( P );
			Options.reset( P );
		}
		void plug( mmm::multimemory_ &M )
		{
			Commands.plug( M );
			Options.plug( M );
		}
		description_ &operator =( const description_ &D )
		{
			Commands = D.Commands;
			Options = D.Options;

			return *this;
		}
		//f Initialization
		void Init( void )
		{
			Commands.Init();
			Options.Init();
		}
		//f Add command 'Command'.
		void AddCommand( const command__ &Command )
		{
			Commands.Add( Command );
		}
		//f Add command with short name, 'Short', long name 'Long' and description 'Description' and id 'Id'.
		void AddCommand(
			char Short,
			const char *Long,
			int Id )
		{
			command__ Command;

			if ( Id > CLNARG_ID_MAX )
				ERRl();
			
			Command.Short = Short;
			Command.Long = Long;
			Command.Id = (id__)Id;

			AddCommand( Command );
		}
		//f Add option 'Option'.
		void AddOption( const option__ &Option )
		{
			Options.Add( Option );
		}
		//f Add option with long name 'Long', short name 'Short', description 'Description', and amount 'Amount' and id 'Id'.
		void AddOption(
			char Short,
			const char *Long,
			int Id )
		{
			option__ Option;

			if ( Id > CLNARG_ID_MAX )
				ERRl();

			Option.Long = Long;
			Option.Short = Short;
			Option.Id = (id__)Id;

			AddOption( Option );
		}
		//f Return the long and short command label corresponding to 'Id' using 'Separator' to seperate them.
		const char *GetCommandLabels(
			int Id,
			const char *Separator = CLNARG_GENERAL_SEPARATOR ) const;
		//f Return the option label corresponding to 'Id'.
		const char *GetOptionLabels( int Id ) const;
	};

	E_AUTO( description )

	//t An argument.
	typedef str::string_ argument_;
	typedef str::string	argument;

	// Arguments.
	typedef ctn::E_XMCONTAINER_( argument_ ) arguments_;
	typedef ctn::E_XMCONTAINER( argument_ ) arguments;

	// Option list.
	typedef bch::E_BUNCH_( id__ ) option_list_;
	typedef bch::E_BUNCH( id__ ) option_list;


	//c Class to analyze the command line argument.
	class analyzer___
	{
	private:
		const char **ArgV_;
		int ArgC_;
		const description_ *Description_;
		char Flag_;
		bso::ubyte__ ArgCount_;
		bso::bool__ analyzer___::GetArgument_( 
			int &i,
			id__ Option,
			argument_ &Argument );
	public:
		analyzer___( void )
		{
			ArgV_ = NULL;
			ArgC_ = 0;
			Description_ = NULL;
			Flag_ = 0;
			ArgCount_ = 0;
		}
		//f Initialization.
		void Init(
			const char *ArgV[],
			int ArgC,
			const description_ &Description,
			char Flag = CLNARG_DEFAULT_FLAG )
		{
			ArgV_ = ArgV;
			ArgC_ = ArgC;
			Description_ = &Description;
			Flag_ = Flag;
			ArgCount_ = 0;
		}
		//f Initialization.
		void Init(
			int ArgC,
			const char *ArgV[],
			const description_ &Description,
			char Flag = CLNARG_DEFAULT_FLAG )
		{
			Init( ArgV, ArgC, Description, Flag );
		}
		//f Return the eventual command.
		id__ GetCommand( void ) const;
		//f Put in 'Options' options. If returned value != 'NULL', then it points of the invalid option/command.
		const char *GetOptions( option_list_ &Options ) const;
		//f Put in 'Argument' the argument of option 'Option'.
		void GetArgument( 
			int Option,
			argument_ &Argument );
		//f Put in 'Arguments' the arguments of option 'Option'.
		void GetArguments( 
			int Option,
			arguments_ &Arguments );
		//f Put in 'Arguments' arguments no associated to an option. A 'GetArguments( Id, ... )' for each option MUST be called before calling this function.
		void GetArguments( arguments_ &Arguments );
		//f Return descriptions.
		const description_ &Description( void )
		{
			return *Description_;
		}
	};
	
	/*f Print the usage text for command in 'Description' identified by 'CommandId'
	using 'Text' and 'View'. If 'Default' at true, the command is the default one. */
	void PrintCommandUsage(
		const description_ &Description,
		int CommandId,
		const char *Text,
		clnarg::view View,
		bso::bool__ Default = false );
		
	/*f Print the usage text for option, with parameter, in 'Description' identified
	by 'OptionId' using 'Text' and 'View'. 'Parameter' is the parameter of the option.	*/
	void PrintOptionUsage(
		const description_ &Description,
		int OptionId,
		const char *Parameter,
		const char *Text,
		clnarg::view View );

	/*f Print the usage text for option, without parameter, in 'Description'
	identified by 'OptionId' using 'Text' and 'View'. */
	inline void PrintOptionUsage(
		const description_ &Description,
		int OptionId,
		const char *Text,
		clnarg::view View )
	{
		PrintOptionUsage( Description, OptionId, NULL, Text, View );
	}
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
