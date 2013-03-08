/*
	Header for the 'rpkctx' module by Claude SIMON (http://zeusw.org/epeios/contact.html)
	Copyright (C) 2010 Claude SIMON.
*/

// $Id$

#ifndef RPKCTX__INC
#define RPKCTX__INC

#include "bch.h"
#include "rpkbsc.h"
#include "xml.h"

namespace rpkctx {
	using namespace rpkbsc;

	typedef bso::ubyte__ coeff__;

	typedef mdr::size__ amount__;
#	define RPKCTX_AMOUNT_MAX	EPEIOS_SIZE_MAX

	E_TMIMIC( rrows, pool );

	class context_ {
	public:
		struct s {
			pool_::s Pool;
			amount__
				Session,	// Amount of record of the current session.
				Cycle;		// To ensure that, inside a cycle, a record is only picked once.
			time_t TimeStamp;
		} &S_;
		pool_ Pool;
		context_( s &S )
		: S_( S ),
		  Pool( S.Pool )
		{}
		void reset( bso::bool__ P = true )
		{
			Pool.reset( P );

			S_.Session = S_.Cycle = 0;
			S_.TimeStamp = 0;
		}
		void plug( mdr::E_MEMORY_DRIVER__ &MD )
		{
			Pool.plug( MD );
		}
		void plug( mmm::E_MULTIMEMORY_ &MM )
		{
			Pool.plug( MM );
		}
		context_ &operator =( const context_ &C )
		{
			Pool = C.Pool;
			S_.Cycle = C.S_.Cycle;
			S_.Session = C.S_.Session;
			S_.TimeStamp = C.S_.TimeStamp;
		}
		void Init( void )
		{
			Pool.Init();

			S_.Session = S_.Cycle = 0;
			S_.TimeStamp = 0;
		}
		rrow__ Pick(
			amount__ Amount,
			bso::ulong__ SessionDuration );	// In minute; '0' for infinite.
	};

	E_AUTO( context );

	void Dump(
		const context_ &Context,
		xml::writer_ &Writer );

	void Retrieve(
		xml::parser___ &Parser,
		context_ &Context );

};

#endif

