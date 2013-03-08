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
	mdr::row__ Row = Aliases.First();

	Alias.Init( Aliases );

	if ( ( Row != NONE ) && ( Alias( Row ).Label != Label ) )
		Row = Aliases.Next( Row );

	if ( Row != NONE )
		return Alias( Row ).TableRow();
	else
		return NONE;
}
