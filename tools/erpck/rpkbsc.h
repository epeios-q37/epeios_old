/*
	Header for the 'rpkbsc' module by Claude SIMON (http://zeusw.org/epeios/contact.html)
	Copyright (C) 2010 Claude SIMON.
*/

// $Id$

#ifndef RPKBSC__INC
#define RPKBSC__INC

#include "bso.h"
#include "tol.h"
#include "epeios.h"
#include "str.h"

namespace rpkbsc {
	typedef bso::ubyte__ weight__;
#	define DEFAULT_WEIGHT	1

	typedef bso::ulong__ counter__;
#define RPKBSC_COUNTER_MAX	BSO_ULONG_MAX

	E_ROW( rrow__ );	// 'record row'.
	typedef bch::E_BUNCH_( rrow__ ) rrows_;
	E_AUTO( rrows );

	E_ROW( trow__ );	// 'table row'.

	template <typename container, typename item, typename row> inline row BaseSearch(
		const str::string_ &Label,
		const container &Container )
	{
		item Item;
		row Row = Container.First();

		Item.Init( Container );

		while ( ( Row != NONE ) && ( Item( Row ).Label != Label ) )
			Row = Container.Next( Row );

		return Row;
	}

	template <typename container, typename item, typename row> inline row SearchInMulti(
		const str::string_ &Label,
		const container &Container )
	{
		return BaseSearch<container, ctn::E_CITEMt( item, row ), row >( Label, Container );
	}

	template <typename container, typename item, typename row> inline row SearchInMono(
		const str::string_ &Label,
		const container &Container )
	{
		return BaseSearch<container, ctn::E_CMITEMt( item, row ), row >( Label, Container );
	}


};

#endif

