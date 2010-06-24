/*
	'rpkctx' module by Claude SIMON ((http://zeusw.org/epeios/contact.html))
	Part of the 'erpck' tool.
	Copyright (C) 2010 Claude SIMON.
*/

// $Id$

#include "rpkctx.h"

#include "bitbch.h"

using namespace rpkctx;

typedef bitbch::E_BIT_BUNCHt_( rrow__ ) grid_;
E_AUTO( grid );

#define COEFF	3	// 

void rpkctx::Dump(
	const pool_ &Pool,
	xml::writer_ &Writer )
{
	epeios::row__ Row = Pool.First();

	while ( Row != NONE ) {
		Writer.PushTag( "Record" );
		Writer.PutAttribute( "Id", bso::Convert( *Pool.Get( Row ) ) );
		Writer.PopTag();

		Row = Pool.Next( Row );
	}
}

// '<Record ...>/<Record>...
//          ^
//						 ^
static rrow__ RetrieveRecordId_( xml::browser___ &Browser )
{
	rrow__ Id = NONE;
	bso::bool__ Continue = true;
	epeios::row__ Error = NONE;

	while ( Continue ) {
		switch ( Browser.Browse( xml::tfObvious ) ) {
		case xml::tAttribute:
			if ( Browser.AttributeName() != "Id" )
				ERRc();

			Id = Browser.Value().ToUL( &Error );

			if ( Error != NONE )
				ERRc();
			break;
		case xml::tEndTag:
			Continue = false;
			break;
		default:
			ERRc();
			break;
		}
	}

	if ( Id == NONE )
		ERRc();

	return Id;
}


// '<... >...</...>...
//        ^
//				   ^
void rpkctx::Retrieve(
	xml::browser___ &Browser,
	pool_ &Pool )
{
	bso::bool__ Continue = true;
	epeios::row__ Error = NONE;
	rrow__ Row = NONE;

	while ( Continue ) {
		switch ( Browser.Browse( xml::tfObvious ) ) {
		case xml::tStartTag:
			if ( Browser.TagName() != "Record" )
				ERRc();

			Pool.Append( RetrieveRecordId_( Browser ) );
			break;
		case xml::tEndTag:
			Continue = false;
			break;
		default:
			ERRc();
			break;
		}
	}
}

void rpkctx::Dump(
	const context_ &Context,
	xml::writer_ &Writer )
{
	Writer.PushTag( "Pools" );

	Writer.PushTag( "Pool" );
	Writer.PutAttribute( "State", "Previous" );
	Dump( Context.Previous, Writer );
	Writer.PopTag();

	Writer.PushTag( "Pool" );
	Writer.PutAttribute( "State", "Current" );
	Dump( Context.Current, Writer );
	Writer.PopTag();

	Writer.PopTag();
}

static void RetrievePools_(
	xml::browser___ &Browser,
	context_ &Context )
{
ERRProlog
	bso::bool__ Continue = true;
	str::string State;
ERRBegin
	State.Init();

	while ( Continue ) {
		switch( Browser.Browse( xml::tfObvious | xml::tfStartTagClosed ) ) {
		case xml::tStartTag:
			if ( Browser.TagName() != "Pool" )
				ERRc();

			break;
		case xml::tAttribute:
			if ( Browser.AttributeName() != "State" )
				ERRc();

			State = Browser.Value();
			break;
		case xml::tStartTagClosed:
			if ( State == "Previous" )
				Retrieve( Browser, Context.Previous );
			else if ( State == "Current" )
				Retrieve( Browser, Context.Current );
			else
				ERRc();

			State.Init();
			break;
		case xml::tEndTag:
			Continue = false;
			break;
		default:
			ERRc();
			break;
		}
	}
ERRErr
ERREnd
ERREpilog
}


void rpkctx::Retrieve(
	xml::browser___ &Browser,
	context_ &Context )
{
	bso::bool__ Continue = true;

	while ( Continue ) {
		switch ( Browser.Browse( xml::tfObvious | xml::tfStartTagClosed ) ) {
		case xml::tStartTag:
			if ( Browser.TagName() != "Pools" )
				ERRc();
			break;
		case xml::tStartTagClosed:
			RetrievePools_( Browser, Context );
			break;
		case xml::tEndTag:
			Continue = false;
			break;
		default:
			ERRc();
			break;
		}
	}
}

static amount__ Fill_(
	const pool_ &Pool,
	amount__ Amount,
	amount__ TotalAmount,
	grid_ &Grid )
{
	epeios::row__ Row = Pool.Last();
	amount__ Counter = 0;

	while ( ( Row != NONE ) && ( Counter < Amount ) ) {
		if ( *Pool( Row ) < TotalAmount ) {
			Grid.Store( false, Pool( Row ) );
			Counter++;
		}

		Row = Pool.Previous( Row );
	}

	return Counter;
}

static rrow__ Pick_( const grid_ &Grid )
{
	rrow__ Row = NONE;

	tol::InitializeRandomGenerator();

	do {
		Row = rand() % Grid.Amount();
	} while ( Grid.Get( Row ) == false );

	return Row;
}

rrow__ rpkctx::context_::Pick( amount__ Amount )
{
	rrow__ Row = NONE;
ERRProlog
	grid Grid;
	amount__ Remainder = 0;
ERRBegin
	Grid.Init();
	Grid.Allocate( Amount );

	Grid.Reset( true );

	if ( Current.Amount() >= Amount ) {
		Previous = Current;
		Current.Init();
	}

	Remainder = Amount - Fill_( Current, Amount, Amount, Grid );

	if ( ( RPKCTX_AMOUNT_MAX / COEFF ) < Remainder )
		ERRl();

	if ( ( Remainder * COEFF ) > Amount )
		Fill_( Previous, Amount / COEFF, Amount, Grid );

	Row = Pick_( Grid );

	Current.Append( Row );
ERRErr
ERREnd
ERREpilog
	return Row;
}

