/*
	'rpkals' module by Claude SIMON ((http://zeusw.org/epeios/contact.html))
	Part of the 'erpck' tool.
	Copyright (C) 2010 Claude SIMON.
*/

// $Id$

#include "rpkals.h"

using namespace rpkals;

trow__ rpkals::SearchTable(
	const str::string_ &Label,
	const table_aliases_ &Aliases )
{
	ctn::E_CMITEM( table_alias_ ) Alias;
	sdr::row__ Row = Aliases.First();

	Alias.Init( Aliases );

	if ( ( Row != E_NIL ) && ( Alias( Row ).Label != Label ) )
		Row = Aliases.Next( Row );

	if ( Row != E_NIL )
		return Alias( Row ).TableRow();
	else
		return E_NIL;
}
