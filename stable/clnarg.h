/* begin of 'entete.txt' template file V1.1 */
/* Best viewed with a tab size of 4 */
/*
	This file is part of the Epeios project. For all information
	concerning the Epeios project, this file and its conditions of use,
	consult the site: 

			http://www.epeios.org/
*/
/* end of 'entete.txt' template file */
/* begin of 'entete.h' template file V1.3 */
/*
Version:
	1.0.6 24/05/2000 11:08:10

Instigators:
	Claude SIMON

Authors:
	Claude SIMON

Contributors:
	/

Coordinator:
	Claude SIMON

Description:
	Command LiNe ARGuments
*/

/* Begin of automatic documentation generation part. */

//V 1.0.6
//C Claude SIMON
//D Command LiNe ARGuments
//R 24/05/2000 11:08:10

/* End of automatic documentation generation part. */

/* end of 'entete.h' template file */
/* begin of 'xxx.h' template file V2.6 */

#ifndef CLNARG__INC
#define CLNARG__INC

#define CLNARG_NAME		"CLNARG"

#define	CLNARG_VERSION		"1.0.6"	
#define CLNARG_OWNER		"The Epeios Project (www.epeios.org)"	

//  	Substitution is not applied to the rest when emancipated

#ifdef CLNARG__LINE
#line 14
#endif

#include "ttr.h"

extern class ttr_tutor &CLNARGTutor;

#if defined( XXX_DBG ) && !defined( CLNARG_NODBG )
#define CLNARG_DBG 
#endif

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "bso.h"
#include "set.h"
#include "ctn.h"
#include "str.h"

#define CLNARG_DEFAULT_FLAG	'-'

//t An option/argument id.
typedef bso__ubyte clnarg__id;
// if modified, modify below.

//d Value the get an id when no value.
#define CLNARG_NONE			BSO_UBYTE_MAX
#define CLNARG_ID_MAX		( CLNARG_NONE - 1 )

//d Value of no short option/command.
#define CLNARG_NO_SHORT	0

//d Value to gice if no long option/command.
#define CLNARG_NO_LONG	NULL

class clnarg__item {
public:
	char Short;
	const char *Long;
	clnarg__id Id;
	clnarg__item( void )
	{
		Short = CLNARG_NO_SHORT;
		Long = NULL;
		Id = CLNARG_NONE;
	}
};

typedef clnarg__item clnarg__command;

typedef SET_( clnarg__command ) clnarg_commands_;
typedef SET( clnarg__command ) clnarg_commands;

typedef clnarg__item clnarg__option;

typedef SET_( clnarg__option ) clnarg_options_;
typedef SET( clnarg__option ) clnarg_options;

//c Description of the available command line options and arguments.
class clnarg_description_
{
public:
	clnarg_commands_ Commands;
	clnarg_options_ Options;
	struct s {
		clnarg_commands_::s Commands;
		clnarg_options_::s Options;
	};
	clnarg_description_( s &S )
	: Commands( S.Commands ),
	  Options( S.Options )
	{}
	void reset( bool P = true )
	{
		Commands.reset( P );
		Options.reset( P );
	}
	void plug( mmm_multimemory_ &M )
	{
		Commands.plug( M );
		Options.plug( M );
	}
	clnarg_description_ &operator =( const clnarg_description_ &D )
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
	void AddCommand( const clnarg__command &Command )
	{
		Commands.Add( Command );
	}
	//f Add command with short name, 'Short', long name 'Long' and description 'Description' and id 'Id'.
	void AddCommand(
		char Short,
		const char *Long,
		int Id )
	{
		clnarg__command Command;

		if ( Id > CLNARG_ID_MAX )
			ERRl();
		
		Command.Short = Short;
		Command.Long = Long;
		Command.Id = Id;

		AddCommand( Command );
	}
	//f Add option 'Option'.
	void AddOption( const clnarg__option &Option )
	{
		Options.Add( Option );
	}
	//f Add option with long name 'Long', short name 'Short', description 'Description', and amount 'Amount' and id 'Id'.
	void AddOption(
		char Short,
		const char *Long,
		int Id )
	{
		clnarg__option Option;

		if ( Id > CLNARG_ID_MAX )
			ERRl();

		Option.Long = Long;
		Option.Short = Short;
		Option.Id = Id;

		AddOption( Option );
	}
};

AUTO( clnarg_description )

//t An argument.
typedef str_string_ clnarg_argument_;
typedef str_string	clnarg_argument;

// Arguments.
typedef XMCONTAINER_( clnarg_argument_ ) clnarg_arguments_;
typedef XMCONTAINER( clnarg_argument_ ) clnarg_arguments;

// Option list.
typedef SET_( clnarg__id ) clnarg_option_list_;
typedef SET( clnarg__id ) clnarg_option_list;


//c Class to analyze the command line argument.
class clnarg__analyzer_
{
private:
	const char **ArgV_;
	int ArgC_;
	const clnarg_description_ *Description_;
	char Flag_;
	bso__ubyte ArgCount_;
	bso__bool clnarg__analyzer_::GetArgument_( 
		int &i,
		clnarg__id Option,
		clnarg_argument_ &Argument );
public:
	clnarg__analyzer_( void )
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
		const clnarg_description_ &Description,
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
		const clnarg_description_ &Description,
		char Flag = CLNARG_DEFAULT_FLAG )
	{
		Init( ArgV, ArgC, Description, Flag );
	}
	//f Return the eventual command.
	clnarg__id GetCommand( void ) const;
	//f Put in 'Options' options. If returned value != 'NULL', then it points of the invalid option/command.
	const char *GetOptions( clnarg_option_list_ &Options ) const;
	//f Put in 'Argument' the argument of option 'Option'.
	void GetArgument( 
		int Option,
		clnarg_argument_ &Argument );
	//f Put in 'Arguments' the arguments of option 'Option'.
	void GetArguments( 
		int Option,
		clnarg_arguments_ &Arguments );
	//f Put in 'Arguments' arguments no associated to an option. A 'GetArguments( Id, ... )' for each option MUST be called before calling this function.
	void GetArguments( clnarg_arguments_ &Arguments );
};


/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
/* end of 'xxx.h' template file */