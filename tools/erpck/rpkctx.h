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

	typedef epeios::size__ amount__;
#	define RPKCTX_AMOUNT_MAX	EPEIOS_SIZE_MAX

	typedef bch::E_BUNCH_( rrow__ ) pool_;
	E_AUTO( pool );

	void Dump(
		const pool_ &Pool,
		xml::writer_ &Writer );

	void Retrieve(
		xml::browser___ &Browser,
		pool_ &Pool );

	class context_ {
	public:
		struct s {
			pool_::s Previous, Current;
		};
		pool_ Previous, Current;
		context_( s &S )
		: Previous( S.Previous ),
		  Current( S.Current )
		{
		}
		void reset( bso::bool__ P = true )
		{
			Previous.reset( P );
			Current.reset( P );
		}
		void plug( mmm::E_MULTIMEMORY_ &MM )
		{
			Previous.plug( MM );
			Current.plug( MM );
		}
		void Init( void )
		{
			Previous.Init();
			Current.Init();
		}
		rrow__ Pick( amount__ Amount );
	};

	E_AUTO( context );

	void Dump(
		const context_ &Context,
		xml::writer_ &Writer );

	void Retrieve(
		xml::browser___ &Browser,
		context_ &Context );

};

#endif

