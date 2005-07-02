/*
	'clnarg' library by Claude SIMON (csimon@epeios.org)
	Requires the 'clnarg' header file ('clnarg.h').
	Copyright (C) 2000-2001, 2004 Claude SIMON (csimon@epeios.org).

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

#define CLNARG__COMPILATION

#include "clnarg.h"

class clnargtutor
: public ttr_tutor
{
public:
	clnargtutor( void )
	: ttr_tutor( CLNARG_NAME )
	{
#ifdef CLNARG_DBG
		Version = CLNARG_VERSION "\b\bD $";
#else
		Version = CLNARG_VERSION;
#endif
		Owner = CLNARG_OWNER;
		Date = "$Date$";
	}
	virtual ~clnargtutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#include "cio.h"

using namespace clnarg;

enum type {
	tUnknow,
	tMulti,
	tLong,
	tShort,
	tEnd,
	tNone
};

template <typename t> static tym::row__ Search_(
	id__ Id,
	const bch::E_BUNCH_( t ) &Bunch )
{
	tym::row__ P = Bunch.First();

	while( ( P != NONE ) && ( Bunch( P ).Id != Id ) )
		P = Bunch.Next( P );

	return P;
}

template <typename t> static id__ Search_(
	char C,
	const bch::E_BUNCH_( t ) &Bunch )
{
	tym::row__ P = Bunch.First();

	while( ( P != NONE ) && ( Bunch( P ).Short != C ) )
		P = Bunch.Next( P );

	if ( P == NONE )
		return CLNARG_NONE;
	else
		return Bunch( P ).Id;
}

template <typename t> static id__ Search_(
	const char *String,
	const bch::E_BUNCH_( t ) &Bunch )
{
	tym::row__ P = Bunch.First();

	while( ( P != NONE ) && ( strcmp( Bunch( P ).Long, String ) ) )
		P = Bunch.Next( P );

	if ( P == NONE )
		return CLNARG_NONE;
	else
		return Bunch( P ).Id;
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

id__ clnarg::analyzer___::GetCommand( void ) const
{
	id__ Id = CLNARG_NONE;

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

static bso::bool__ Exists_(
	id__ Id,
	const option_list_ &Options )
{
	tym::row__ P = Options.First();

	while( ( P != NONE ) && ( Options( P ) != Id ) )
		P = Options.Next( P );

	return P != NONE;
}

static inline void Add_(
	id__ Id,
	option_list_ &Options )
{
	if ( !Exists_( Id, Options ) )
		Options.Append( Id );
}

static const char *AddMulti_(
	const char *String,
	const options_ &OptDesc,
	const commands_ &CmdDesc,
	option_list_ &OptList )
{
	bso::bsize__ i = 0;
	bso::bsize__ Limit = strlen( String );
	id__ Id = CLNARG_NONE;
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

const char *clnarg::analyzer___::GetOptions( option_list_ &Options ) const
{
	id__ Id = CLNARG_NONE;
	bso::bool__ End = false;
	bso::bool__ Error = false;
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

bso::bool__ clnarg::analyzer___::GetArgument_( 
	int &i,
	id__ Option,
	argument_ &Argument )
{
	tym::row__ P = Search_( Option, Description_->Options );
	bso::bool__ Cont = true;

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



void clnarg::analyzer___::GetArgument( 
	int Option,
	argument_ &Argument )
{
	int i = 1;

	while( ( i < ( ArgC_ - 1 ) ) && GetArgument_( i, (id__)Option, Argument ) ) {};
}

void clnarg::analyzer___::GetArguments( 
	int Option,
	arguments_ &Arguments )
{
ERRProlog
	int i = 1;
	str::string Argument;
ERRBegin
	while( ( i < ( ArgC_ - 1 ) ) && GetArgument_( i, (id__)Option, Argument ) ) {
		if ( Argument.Amount() )
			Arguments.Append( Argument );
		Argument.Init();
	}

ERRErr
ERREnd
ERREpilog
}

void clnarg::analyzer___::GetArguments( arguments_ &Arguments )
{
	int i = 1;
	bso::bool__ End = false;

	while(  i < ArgC_ ){
		switch( Analyze_( ArgV_[i], Flag_ ) ) {
		case tNone:
			if ( ArgCount_ )
				ArgCount_--;
			else
				Arguments.Append( str::string( ArgV_[i] ) );
			break;
		case tMulti:
		case tLong:
		case tShort:
			if ( End )
				Arguments.Append( str::string( ArgV_[i] ) );
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

static const char *GetLabel_(
	const item__ &Item,
	int Id,
	const char *Separator )
{
	static char Label[50];
	
	if ( Item.Short != CLNARG_NO_SHORT ) {
		Label[0] = '-';
		Label[1] = Item.Short;
		Label[2] = 0;
	} else
		Label[0] = 0;
		
	if ( Item.Long != NULL ) {
		if ( Label[0] != 0 )
			strcat( Label, Separator );
		
		strcat( Label, "--" );
		
		if (  strlen( Label ) + strlen( Item.Long ) >= sizeof( Label ) )
			ERRl();
		
		strcat( Label, Item.Long );
	}
	
	return Label;
}	
		
static const char *GetLabel_(
	const bch::E_BUNCH_( item__ ) &Items,
	int Id,
	const char *Separator )
{
	tym::row__ P = Items.First();
	
	while( ( P != NONE ) && ( Items( P ).Id != Id ) )
		P = Items.Next( P );
		
	if ( P == NONE )
		ERRu();
		
	return  GetLabel_( Items( P ), Id, Separator );
}
	
const char *description_::GetCommandLabels(
	int Id,
	const char *Separator ) const
{
	return GetLabel_( Commands, Id, Separator );
}
	
const char *description_::GetOptionLabels( int Id ) const
{
	return GetLabel_( Options, Id, CLNARG_DETAIL_SEPARATOR );
}

static void HandleView_(
	clnarg::view View,
	txf::text_oflow__ &Flow )
{
	switch( View ) {
	case clnarg::vOneLine:
		Flow << txf::tab;
		break;
	case clnarg::vSplit:
		Flow << txf::nl << txf::tab << txf::tab;
		break;
	default:
		ERRu();
		break;
	}
}

void clnarg::PrintCommandUsage(
	const description_ &Description,
	int CommandId,
	const char *Text,
	clnarg::view View,
	txf::text_oflow__ &Flow,
	bso::bool__ Default )
{
	Flow << txf::tab;
		
	if ( Default )
		Flow << "<none>, ";
			
	Flow << Description.GetCommandLabels( CommandId, CLNARG_DETAIL_SEPARATOR ) << ':';
	
	HandleView_( View, Flow );
		
	Flow << Text << txf::nl;
}

void clnarg::PrintOptionUsage(
	const description_ &Description,
	int OptionId,
	const char *Parameter,
	const char *Text,
	clnarg::view View,
	txf::text_oflow__ &Flow )
{
	Flow << txf::tab;
		
	Flow << Description.GetOptionLabels( OptionId );
	
	if ( ( Parameter != NULL )
		 && ( Parameter[0] != 0 ) ) 
		 Flow << ' ' << Parameter;

	Flow << ':';
		
	HandleView_( View, Flow );
		
	Flow << Text << txf::nl;
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
