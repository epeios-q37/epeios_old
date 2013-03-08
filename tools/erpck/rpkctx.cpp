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

#define POOL_TAG_NAME						"Pool"

#define POOL_CYCLE_AMOUNT_ATTRIBUTE_NAME	"CycleAmount"
#define POOL_SESSION_AMOUNT_ATTRIBUTE_NAME	"SessionAmount"
#define POOL_TIMESTAMP_ATTRIBUTE_NAME		"TimeStamp"


#define RECORD_TAG_NAME					"Record"
#define RECORD_ID_ATTRIBUTE_NAME		"Id"

void Dump_(
	const rrows_ &Records,
	xml::writer_ &Writer )
{
	mdr::row__ Row = Records.First();

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
static rrow__ RetrieveRecordId_( xml::parser___ &Parser )
{
	rrow__ Id = NONE;
	bso::bool__ Continue = true;
	mdr::row__ Error = NONE;

	while ( Continue ) {
		switch ( Parser.Parse( xml::tfObvious ) ) {
		case xml::tAttribute:
			if ( Parser.AttributeName() != RECORD_ID_ATTRIBUTE_NAME )
				ERRc();

			Id = Parser.Value().ToUL( &Error );

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
	xml::parser___ &Parser,
	rrows_ &Records )
{
	bso::bool__ Continue = true;
	mdr::row__ Error = NONE;
	rrow__ Row = NONE;

	while ( Continue ) {
		switch ( Parser.Parse( xml::tfObvious ) ) {
		case xml::tStartTag:
			if ( Parser.TagName() != RECORD_TAG_NAME )
				ERRc();

			Records.Append( RetrieveRecordId_( Parser ) );
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
	amount__ SessionAmount,
	amount__ CycleAmount,
	time_t TimeStamp,
	xml::writer_ &Writer )
{
	Writer.PushTag( POOL_TAG_NAME );
	Writer.PutAttribute( POOL_SESSION_AMOUNT_ATTRIBUTE_NAME, bso::Convert( SessionAmount) );
	Writer.PutAttribute( POOL_CYCLE_AMOUNT_ATTRIBUTE_NAME, bso::Convert( CycleAmount) );
	Writer.PutAttribute( POOL_TIMESTAMP_ATTRIBUTE_NAME, bso::Convert( TimeStamp ) );
	Dump_( Pool, Writer );
	Writer.PopTag();
}

void rpkctx::Dump(
	const context_ &Context,
	xml::writer_ &Writer )
{
	Dump_( Context.Pool, Context.S_.Session, Context.S_.Cycle, Context.S_.TimeStamp, Writer );
}

static void RetrievePool_(
	xml::parser___ &Parser,
	context_ &Context )
{

	bso::bool__ Continue = true;

	while ( Continue ) {
		switch( Parser.Parse( xml::tfObvious | xml::tfStartTagClosed ) ) {
		case xml::tStartTag:
			if ( Parser.TagName() != POOL_TAG_NAME )
				ERRc();

			break;
		case xml::tAttribute:
			if ( Parser.AttributeName() == POOL_SESSION_AMOUNT_ATTRIBUTE_NAME )
				Context.S_.Session = Parser.Value().ToUL();
			else if ( Parser.AttributeName() == POOL_CYCLE_AMOUNT_ATTRIBUTE_NAME )
				Context.S_.Cycle = Parser.Value().ToUL();
			else if ( Parser.AttributeName() == POOL_TIMESTAMP_ATTRIBUTE_NAME )
				Context.S_.TimeStamp = Parser.Value().ToULL();
			else
				ERRc();

			break;
		case xml::tStartTagClosed:
			Retrieve_( Parser, Context.Pool );
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


void rpkctx::Retrieve(
	xml::parser___ &Parser,
	context_ &Context )
{
	RetrievePool_( Parser, Context );
}

static amount__ Remove_(
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
	amount__ Applicaple = 0;
ERRBegin
	Grid.Init();
	Grid.Allocate( Amount );

	Grid.Reset( true );

	if ( ( S_.Session >= Amount ) || IsNewSession_( S_.TimeStamp, Duration ) ) {
		if ( ( S_.Session < Amount ) && ( S_.Session > S_.Cycle ) )
			S_.Cycle = S_.Session;
		S_.Session = 0;
	}

	if ( S_.Cycle >= Amount )
		S_.Cycle = 0;

	Applicaple = Amount / 3;

	Applicaple = ( Applicaple > S_.Session ? Applicaple : S_.Session );

	Applicaple = ( Applicaple > S_.Cycle ? Applicaple : S_.Cycle );

	if ( Pool.Amount() != 0 )
		Pool.Remove( Pool.First(), Pool.Amount() - Remove_( Pool, Applicaple, Amount, Grid ) );

	Row = Pick_( Grid );

	Pool.Append( Row );

	S_.Session++;
	S_.Cycle++;

	S_.TimeStamp = time( NULL );
ERRErr
ERREnd
ERREpilog
	return Row;
}

