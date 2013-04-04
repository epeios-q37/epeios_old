/*
	'fblbrr' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'fblbrr' header file ('fblbrr.h').
	Copyright (C) $COPYRIGHT_DATES$Claude SIMON.
$_RAW_$
	This file is part of the Epeios (http://zeusw.org/epeios/) project.

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

#define FBLBRR__COMPILATION

#include "fblbrr.h"

class fblbrrtutor
: public ttr_tutor
{
public:
	fblbrrtutor( void )
	: ttr_tutor( FBLBRR_NAME )
	{
#ifdef FBLBRR_DBG
		Version = FBLBRR_VERSION "\b\bD $";
#else
		Version = FBLBRR_VERSION;
#endif
		Owner = FBLBRR_OWNER;
		Date = "$Date$";
	}
	virtual ~fblbrrtutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace fblbrr;

#define CC( name, type )\
	case c##name:\
		Parameter.Init( fbltyp::New##name(), c##name );\
		break;

static parameter___ Create_(
	flw::iflow__ &Flow,
	cast Cast )
{
	parameter___ Parameter;

	switch ( Cast ) {
	CC( Object, object__)
	CC( Boolean, boolean__ )
	CC( Booleans, booleans )
	CC( SInt, sint__ )
	CC( SInts, sints )
	CC( UInt, uint__ )
	CC( UInts, uints )
	CC( Id8, id8__ )
	CC( Ids8, ids8 )
	CC( XIds8, xids8 )
	CC( Id16, id16__ )
	CC( Ids16, ids16 )
	CC( XIds16, xids16 )
	CC( Id32, id32__ )
	CC( Ids32, ids32 )
	CC( XIds32, xids32 )
	CC( Char, char__ )
	CC( String, string )
	CC( Strings, strings )
	CC( XStrings, xstrings )
	CC( Byte, byte__ )
	CC( Binary, binary )
	CC( Binaries, binaries )
	CC( Items8, items8 )
	CC( Items16, items16 )
	CC( Items32, items32 )
	CC( XItems8, xitems8 )
	CC( XItems16, xitems16 )
	CC( XItems32, xitems32 )
	CC( CommandsDetails, commands_details )
	CC( ObjectsReferences, objects_references )
	default:
		ERRCcp();
		break;
	}

	return Parameter;
}

#define CCAG( name, type )\
	case c##name:\
		Parameter = Create_( Flow, c##name );\
		fbltyp::Get##name( Flow, *(fbltyp::type *)Parameter.Content );\
		break;


static parameter___ _CreateAndGet(
	flw::iflow__ &Flow,
	cast Cast )
{
	parameter___ Parameter;

	switch ( Cast ) {
	CCAG( Object, object__)
	CCAG( Boolean, boolean__ )
	CCAG( Booleans, booleans )
	CCAG( SInt, sint__ )
	CCAG( SInts, sints )
	CCAG( UInt, uint__ )
	CCAG( UInts, uints )
	CCAG( Id8, id8__ )
	CCAG( Ids8, ids8 )
	CCAG( XIds8, xids8 )
	CCAG( Id16, id16__ )
	CCAG( Ids16, ids16 )
	CCAG( XIds16, xids16 )
	CCAG( Id32, id32__ )
	CCAG( Ids32, ids32 )
	CCAG( XIds32, xids32 )
	CCAG( Char, char__ )
	CCAG( String, string )
	CCAG( Strings, strings )
	CCAG( XStrings, xstrings )
	CCAG( Byte, byte__ )
	CCAG( Binary, binary )
	CCAG( Binaries, binaries )
	CCAG( Items8, items8 )
	CCAG( Items16, items16 )
	CCAG( Items32, items32 )
	CCAG( XItems8, xitems8 )
	CCAG( XItems16, xitems16 )
	CCAG( XItems32, xitems32 )
	CCAG( CommandsDetails, commands_details )
	CCAG( ObjectsReferences, objects_references )
	default:
		ERRCcp();
		break;
	}

	return Parameter;
}

#define CD( name, type )\
	case c##name:\
	fbltyp::Delete##name( (fbltyp::type *)Parameter.Content );\
		break;

static void Delete_( const parameter___ &Parameter )
{
	switch ( Parameter.Cast ) {
	CD( Object, object__)
	CD( Boolean, boolean__ )
	CD( Booleans, booleans )
	CD( SInt, sint__ )
	CD( SInts, sints )
	CD( UInt, uint__ )
	CD( UInts, uints )
	CD( Id8, id8__ )
	CD( Ids8, ids8 )
	CD( XIds8, xids8 )
	CD( Id16, id16__ )
	CD( Ids16, ids16 )
	CD( XIds16, xids16 )
	CD( Id32, id32__ )
	CD( Ids32, ids32 )
	CD( XIds32, xids32 )
	CD( Char, char__ )
	CD( String, string )
	CD( Strings, strings )
	CD( XStrings, xstrings )
	CD( Byte, byte__ )
	CD( Binary, binary )
	CD( Binaries, binaries )
	CD( Items8, items8 )
	CD( Items16, items16 )
	CD( Items32, items32 )
	CD( XItems8, xitems8 )
	CD( XItems16, xitems16 )
	CD( XItems32, xitems32 )
	CD( CommandsDetails, commands_details )
	CD( ObjectsReferences, objects_references )
	default:
		ERRCcp();
		break;
	}
}


void fblbrr::remote_request_functions___::_CreateAll(
	flw::iflow__ &Flow,
	const casts_ &Casts )
{
	sdr::row__ Row = NONE;

	Row = Casts.First();

	while ( ( Row != NONE )
		    && ( Casts( Row ) != cEnd ) ) {

		if ( Flow.Get() != Casts( Row ) )
			ERRCcp();

		if ( _Parameters.Append( _CreateAndGet( Flow, (cast)Casts( Row ) ) ) != Row )
			ERRCcp();

		Row = Casts.Next( Row );
	}

	if ( Row == NONE )
		ERRCcp();

	if ( _Parameters.Append( parameter___( cEnd ) ) != Row )
		ERRCcp();

	Row = Casts.Next( Row );

	while ( Row != NONE ) {
		if ( _Parameters.Append( Create_( Flow, (cast)Casts( Row ) ) ) != Row )
			ERRCcp();

		Row = Casts.Next( Row );
	}
}

void fblbrr::remote_request_functions___::_DeleteAll( void )
{
	sdr::row__ Row = _Parameters.First();

	if ( Row == NONE )
		return;

	while ( ( Row != NONE )
		    && ( _Parameters( Row ).Cast != cEnd ) ) {
		Delete_( _Parameters( Row ) );

		Row =  _Parameters.Next( Row );
	}

	if ( Row == NONE )
		ERRCcp();

	Row =  _Parameters.Next( Row );

	while ( Row != NONE ) {
		Delete_( _Parameters( Row ) );

		Row = _Parameters.Next( Row );
	}
}

#define CP( name, type )\
	case c##name:\
		Flow.Put( Parameter.Cast );\
		fbltyp::Put##name( *(fbltyp::type *)Parameter.Content, Flow );\
		break;


static void Push_(
	flw::oflow__ &Flow,
	const parameter___ &Parameter,
	cast__ Cast )
{
	if ( Parameter.Cast != Cast )
		ERRCcp();

	switch ( Parameter.Cast ) {
	CP( Object, object__)
	CP( Boolean, boolean__ )
	CP( Booleans, booleans )
	CP( SInt, sint__ )
	CP( SInts, sints )
	CP( UInt, uint__ )
	CP( UInts, uints )
	CP( Id8, id8__ )
	CP( Ids8, ids8 )
	CP( XIds8, xids8 )
	CP( Id16, id16__ )
	CP( Ids16, ids16 )
	CP( XIds16, xids16 )
	CP( Id32, id32__ )
	CP( Ids32, ids32 )
	CP( XIds32, xids32 )
	CP( Char, char__ )
	CP( String, string )
	CP( Strings, strings )
	CP( XStrings, xstrings )
	CP( Byte, byte__ )
	CP( Binary, binary )
	CP( Binaries, binaries )
	CP( Items8, items8 )
	CP( Items16, items16 )
	CP( Items32, items32 )
	CP( XItems8, xitems8 )
	CP( XItems16, xitems16 )
	CP( XItems32, xitems32 )
	CP( CommandsDetails, commands_details )
	CP( ObjectsReferences, objects_references )
	default:
		ERRCcp();
		break;
	}
}


void fblbrr::remote_request_functions___::FBLBRQPush(
	flw::oflow__ &Flow,
	const casts_ &Casts )
{
#ifdef BKDRRM_DBG
	if ( Casts.Amount() != Repository_.Amount() )
		ERRu();
#endif

	sdr::row__ Row = Casts.First();

	while ( ( Row != NONE )
		    && ( Casts( Row ) != cEnd ) ) {
		if ( _Parameters( Row ).Cast != Casts( Row ) )
			ERRCcp();
		Row = Casts.Next( Row );
	}

	if ( Row == NONE )
		ERRCcp();

	Row = Casts.Next( Row );

	while ( Row != NONE ) {
		Push_( Flow, _Parameters( Row ), (cast)Casts( Row ) );

		Row = Casts.Next( Row );
	}
}



/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class fblbrrpersonnalization
: public fblbrrtutor
{
public:
	fblbrrpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~fblbrrpersonnalization( void )
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

static fblbrrpersonnalization Tutor;

ttr_tutor &FBLBRRTutor = Tutor;
