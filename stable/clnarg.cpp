/* begin of 'entete.txt' template file V1.1 */
/* Best viewed with a tab size of 4 */
/*
	This file is part of the Epeios project. For all information
	concerning the Epeios project, this file and its conditions of use,
	consult the site: 

			http://www.epeios.org/
*/
/* end of 'entete.txt' template file */
/* begin of 'xxx.cpp' template file V2.8 */
/* 
	See the 'clnarg.h' file  for more details.
*/

#define CLNARG__COMPILATION

#include "clnarg.h"

#ifdef CLNARG__LINE
#line 12
#endif

class clnargtutor
: public ttr_tutor
{
public:
	clnargtutor( void )
	: ttr_tutor( CLNARG_NAME )
	{
#ifdef CLNARG_DBG
		Version = CLNARG_VERSION " (DBG)";
#else
		Version = CLNARG_VERSION;
#endif
		Owner = CLNARG_OWNER;
		Date = "24/05/2000";
	}
	virtual ~clnargtutor( void ){}
};

//  	Substitution is not applied to the rest.
/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

enum type {
	tUnknow,
	tMulti,
	tLong,
	tShort,
	tEnd,
	tNone
};

template <typename t> static POSITION__ Search_(
	clnarg__id Id,
	const SET_( t ) &Set )
{
	POSITION__ P = Set.First();

	while( ( P != NONE ) && ( Set( P ).Id != Id ) )
		P = Set.Next( P );

	return P;
}

template <typename t> static clnarg__id Search_(
	char C,
	const SET_( t ) &Set )
{
	POSITION__ P = Set.First();

	while( ( P != NONE ) && ( Set( P ).Short != C ) )
		P = Set.Next( P );

	if ( P == NONE )
		return CLNARG_NONE;
	else
		return Set( P ).Id;
}

template <typename t> static clnarg__id Search_(
	const char *String,
	const SET_( t ) &Set )
{
	POSITION__ P = Set.First();

	while( ( P != NONE ) && ( strcmp( Set( P ).Long, String ) ) )
		P = Set.Next( P );

	if ( P == NONE )
		return CLNARG_NONE;
	else
		return Set( P ).Id;
}

static inline type Analyze_(
	const char *String,
	char Flag )
{
	type Type;

	if ( ( String == NULL )
		 || ( String[0] == '\0' )
		 || ( String[0] != Flag ) 
		 || ( String[1] == '\0' ) )
		Type = tNone;
	else if ( String[1] == Flag ) {
		if( String[2] == '\0' )
			Type = tEnd;
		else
			Type = tLong;
	}
	else if ( String[2] != '\0' )
		Type = tMulti;
	else
		Type = tShort;

	return Type;
}

clnarg__id clnarg__analyzer_::GetCommand( void ) const
{
	clnarg__id Id = CLNARG_NONE;

	if ( ArgC_ > 1 )
		switch( Analyze_( ArgV_[1], Flag_ ) ) {
		case tLong:
			Id = Search_( ArgV_[1] + 2, Description_->Commands );
			break;
		case tShort:
		case tMulti:
			Id = Search_( ArgV_[1][1], Description_->Commands );
			break;
		}

	return Id;
}

static bso__bool Exists_(
	clnarg__id Id,
	const clnarg_option_list_ &Options )
{
	POSITION__ P = Options.First();

	while( ( P != NONE ) && ( Options( P ) != Id ) )
		P = Options.Next( P );

	return P != NONE;
}

static inline void Add_(
	clnarg__id Id,
	clnarg_option_list_ &Options )
{
	if ( !Exists_( Id, Options ) )
		Options.Add( Id );
}

const char *AddMulti_(
	const char *String,
	const clnarg_options_ &OptDesc,
	const clnarg_commands_ &CmdDesc,
	clnarg_option_list_ &OptList )
{
	bso__bsize i = 0;
	bso__bsize Limit = strlen( String );
	clnarg__id Id = CLNARG_NONE;
	const char *Return = NULL;
	static char Unknow[] = "-o";

	while( i < Limit ) {
		if ( ( Id = Search_( String[i], OptDesc ) ) == CLNARG_NONE ) {
			if ( ( i != 0 ) && ( Search_( String[i], CmdDesc ) == CLNARG_NONE ) ) {
				Unknow[1] = String[i];
				Return = Unknow;
				i = Limit;
			}
		} else
			Add_( Id, OptList );

		i++;
	}

	return Return;
}

const char *clnarg__analyzer_::GetOptions( clnarg_option_list_ &Options ) const
{
	clnarg__id Id = CLNARG_NONE;
	bso__bool End = false;
	bso__bool Error = false;
	int i = 1;
	const char *Return = NULL;

	while( !End && ( i < ArgC_ ) ){
		Id = CLNARG_NONE;
		switch( Analyze_( ArgV_[i], Flag_ ) ) {
		case tLong:
			if ( ( Id = Search_( ArgV_[i] + 2, Description_->Options ) ) == CLNARG_NONE ) {
				if ( ( i != 1 ) || ( Search_( ArgV_[i] + 2, Description_->Commands ) == CLNARG_NONE ) )
					Return = ArgV_[i];
			} else
				Add_( Id, Options );
			break;
		case tShort:
			if ( ( Id = Search_( ArgV_[i][1], Description_->Options ) ) == CLNARG_NONE ) {
				if ( ( i != 1 ) || ( Search_( ArgV_[i][1], Description_->Commands ) == CLNARG_NONE ) )
					Return = ArgV_[i];
			} else
				Add_( Id, Options );
			break;
		case tMulti:
			Return = AddMulti_( ArgV_[i] + 1, Description_->Options, Description_->Commands, Options );
			break;
		case tNone:
			break;
		case tEnd:
			End = true;
			break;
		default:
			ERRc();
			break;
		}

		End = ( Return != NULL );

		i++;
	}

	return Return;

}

bso__bool clnarg__analyzer_::GetArgument_( 
	int &i,
	clnarg__id Option,
	clnarg_argument_ &Argument )
{
	POSITION__ P = Search_( Option, Description_->Options );
	bso__bool Cont = true;

	switch( Analyze_( ArgV_[i], Flag_ ) ) {
	case tLong:
		if ( !strcmp( Description_->Options( P ).Long, ArgV_[i] + 2 ) )
			if ( Analyze_( ArgV_[i+1], Flag_ ) == tNone ) {
				Argument = ArgV_[i+1];
				ArgCount_++;
				i++;
			}
		break;
	case tShort:
		if ( Description_->Options( P ).Short == ArgV_[i][1] )
			if ( Analyze_( ArgV_[i+1], Flag_ ) == tNone ) {
				Argument = ArgV_[i+1];
				ArgCount_++;
				i++;
			}
		break;
	case tMulti:
		if ( Description_->Options( P ).Short == ArgV_[i][strlen( ArgV_[i] ) - 1] )
			if ( Analyze_( ArgV_[i+1], Flag_ ) == tNone ) {
				Argument = ArgV_[i+1];
				ArgCount_++;
				i++;
			}
		break;
	case tNone:
		break;
	case tEnd:
		Cont = false;
		break;
	default:
		ERRc();
		break;
	}

	i++;

	return Cont;
}



void clnarg__analyzer_::GetArgument( 
	int Option,
	clnarg_argument_ &Argument )
{
	int i = 1;

	while( ( i < ( ArgC_ - 1 ) ) && GetArgument_( i, Option, Argument ) );
}

void clnarg__analyzer_::GetArguments( 
	int Option,
	clnarg_arguments_ &Arguments )
{
ERRProlog
	int i = 1;
	str_string Argument;
ERRBegin
	while( ( i < ( ArgC_ - 1 ) ) && GetArgument_( i, Option, Argument ) ) {
		if ( Argument.Amount() )
			Arguments.Add( Argument );
		Argument.Init();
	}

ERRErr
ERREnd
ERREpilog
}

void clnarg__analyzer_::GetArguments( clnarg_arguments_ &Arguments )
{
	int i = 1;
	bso__bool End = false;

	while(  i < ArgC_ ){
		switch( Analyze_( ArgV_[i], Flag_ ) ) {
		case tNone:
			if ( ArgCount_ )
				ArgCount_--;
			else
				Arguments.Add( str_string( ArgV_[i] ) );
			break;
		case tMulti:
		case tLong:
		case tShort:
			if ( End )
				Arguments.Add( str_string( ArgV_[i] ) );
			break;
		case tEnd:
			End = true;
			break;
		default:
			ERRc();
			break;
		}

		i++;
	}
}


/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class clnargpersonnalization
: public clnargtutor
{
public:
	clnargpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~clnargpersonnalization( void )
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

static clnargpersonnalization Tutor;

ttr_tutor &CLNARGTutor = Tutor;

/* end of 'xxx.cpp' template file */