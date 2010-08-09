/*
	'rpkctx' module by Claude SIMON ((http://zeusw.org/epeios/contact.html))
	Part of the 'erpck' tool.
	Copyright (C) 2010 Claude SIMON.
*/

// $Id$

#include "rpkctx.h"

#include "bitbch.h"
#include "dte.h"

using namespace rpkctx;

typedef bitbch::E_BIT_BUNCHt_( rrow__ ) grid_;
E_AUTO( grid );

#define COEFF	3	// 

#define CONTEXT_TAG_NAME				"Context"
#define CONTEXTE_TARGET_ATTRIBUTE_NAME	"target"

#define POOLS_TAG_NAME					"Pools"


#define POOL_TAG_NAME						"Pool"

#define POOL_STATE_ATTRIBUTE_NAME			"State"
#define POOL_PREVIOUS_STATE_ATTRIBUTE_VALUE	"Previous"
#define POOL_CURRENT_STATE_ATTRIBUTE_VALUE	"Current"

#define POOL_TIMESTAMP_ATTRIBUTE_NAME		"TimeStamp"


#define RECORD_TAG_NAME					"Record"
#define RECORD_ID_ATTRIBUTE_NAME		"Id"

void Dump_(
	const rrows_ &Records,
	xml::writer_ &Writer )
{
	epeios::row__ Row = Records.First();

	while ( Row != NONE ) {
		Writer.PushTag( RECORD_TAG_NAME );
		Writer.PutAttribute( RECORD_ID_ATTRIBUTE_NAME, bso::Convert( *Records.Get( Row ) ) );
		Writer.PopTag();

		Row = Records.Next( Row );
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
			if ( Browser.AttributeName() != RECORD_ID_ATTRIBUTE_NAME )
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
static void Retrieve_(
	xml::browser___ &Browser,
	rrows_ &Records )
{
	bso::bool__ Continue = true;
	epeios::row__ Error = NONE;
	rrow__ Row = NONE;

	while ( Continue ) {
		switch ( Browser.Browse( xml::tfObvious ) ) {
		case xml::tStartTag:
			if ( Browser.TagName() != RECORD_TAG_NAME )
				ERRc();

			Records.Append( RetrieveRecordId_( Browser ) );
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

static void Dump_(
	const pool_ &Pool,
	const char *State,
	xml::writer_ &Writer )
{
	Writer.PushTag( POOL_TAG_NAME );
	Writer.PutAttribute( POOL_STATE_ATTRIBUTE_NAME, State );
	Writer.PutAttribute( POOL_TIMESTAMP_ATTRIBUTE_NAME, bso::Convert( Pool.TimeStamp() ) );
	Dump_( Pool.Records, Writer );
	Writer.PopTag();
}

void rpkctx::Dump(
	const context_ &Context,
	xml::writer_ &Writer )
{
	Writer.PushTag( POOLS_TAG_NAME );

	Dump_( Context.Previous, POOL_PREVIOUS_STATE_ATTRIBUTE_VALUE, Writer );
	Dump_( Context.Current, POOL_CURRENT_STATE_ATTRIBUTE_VALUE, Writer );

	Writer.PopTag();
}

static void RetrievePools_(
	xml::browser___ &Browser,
	context_ &Context )
{
ERRProlog
	bso::bool__ Continue = true;
	str::string State;
	time_t TimeStamp = 0;
ERRBegin
	State.Init();

	while ( Continue ) {
		switch( Browser.Browse( xml::tfObvious | xml::tfStartTagClosed ) ) {
		case xml::tStartTag:
			if ( Browser.TagName() != POOL_TAG_NAME )
				ERRc();

			break;
		case xml::tAttribute:
			if ( Browser.AttributeName() == POOL_STATE_ATTRIBUTE_NAME )
				State = Browser.Value();
			else if ( Browser.AttributeName() == POOL_TIMESTAMP_ATTRIBUTE_NAME )
				TimeStamp = Browser.Value().ToULL();
			else
				ERRc();

			break;
		case xml::tStartTagClosed:
			if ( State == POOL_PREVIOUS_STATE_ATTRIBUTE_VALUE ) {
				Context.Previous.TimeStamp() = TimeStamp;
				Retrieve_( Browser, Context.Previous.Records );
			} else if ( State == POOL_CURRENT_STATE_ATTRIBUTE_VALUE ) {
				Context.Current.TimeStamp() = TimeStamp;
				Retrieve_( Browser, Context.Current.Records );
			} else
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
			if ( Browser.TagName() != POOLS_TAG_NAME )
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
	const rrows_ &Records,
	amount__ Amount,
	amount__ TotalAmount,
	grid_ &Grid )
{
	epeios::row__ Row = Records.Last();
	amount__ Counter = 0;

	while ( ( Row != NONE ) && ( Counter < Amount ) ) {
		if ( *Records( Row ) < TotalAmount ) {
			Grid.Store( false, Records( Row ) );
			Counter++;
		}

		Row = Records.Previous( Row );
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

static void Add_(
	const rrows_ &Source,
	rrows_ &Target )
{
	epeios::row__ Row = Source.First();
	epeios::row__ Position = NONE;

	while ( Row != NONE ) {
		if ( ( Position = ( Target.Search( Source( Row ) ) ) ) != NONE )
			Target.Remove( Position );

		Target.Append( Source ( Row ) );

		Row = Source.Next( Row );
	}
}

static inline bso::bool__ IsNewSession_(
	time_t TimeStamp,
	bso::ulong__ Duration )	// in minutes.
{
	if ( Duration == 0 )
		return false;
	else
		return difftime( time( NULL ), TimeStamp ) > ( Duration * 60 );
}


rrow__ rpkctx::context_::Pick(
	amount__ Amount,
	bso::ulong__ Duration )
{
	rrow__ Row = NONE;
ERRProlog
	grid Grid;
	amount__ Remainder = 0;
ERRBegin
	Grid.Init();
	Grid.Allocate( Amount );

	Grid.Reset( true );

	if ( Current.Records.Amount() >= Amount ) {
		Previous = Current;
		Current.Init();
	}

	if ( IsNewSession_( Current.TimeStamp(), Duration ) ) {
		Add_( Current.Records, Previous.Records );
		Current.Records.Init();
	}

	Remainder = Amount - Fill_( Current.Records, Amount, Amount, Grid );

	if ( ( RPKCTX_AMOUNT_MAX / COEFF ) < Remainder )
		ERRl();

	if ( ( Remainder * COEFF ) > Amount )
		Fill_( Previous.Records, Amount / COEFF, Amount, Grid );

	Row = Pick_( Grid );

	Current.Records.Append( Row );
	Current.TimeStamp() = time( NULL );
ERRErr
ERREnd
ERREpilog
	return Row;
}

