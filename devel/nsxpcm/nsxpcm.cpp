/*
	'nsxpcm' library by Claude SIMON (csimon@epeios.org)
	Requires the 'nsxpcm' header file ('nsxpcm.h').
	Copyright (C) $COPYRIGHT_DATES$Claude SIMON (csimon@epeios.org).
$_RAW_$
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

#define NSXPCM__COMPILATION

#include "nsxpcm.h"

class nsxpcmtutor
: public ttr_tutor
{
public:
	nsxpcmtutor( void )
	: ttr_tutor( NSXPCM_NAME )
	{
#ifdef NSXPCM_DBG
		Version = NSXPCM_VERSION "\b\bD $";
#else
		Version = NSXPCM_VERSION;
#endif
		Owner = NSXPCM_OWNER;
		Date = "$Date$";
	}
	virtual ~nsxpcmtutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#include "nsMemory.h"

using namespace nsxpcm;

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */


void nsxpcm::Convert(
	const char *CString,
	epeios::size__ Size,
	char **JString )
{
    if ( !JString )
        ERRu();
	else
	    *JString = (char*) nsMemory::Clone( CString, sizeof(char) * Size );
    
	if ( !JString )
		ERRa();
}

void nsxpcm::Convert(
	const char *CString,
	char **JString )
{
	Convert( CString, strlen( CString ) + 1, JString );
}

void nsxpcm::Convert(
	const nsEmbedCString &ECString,
	nsEmbedString &EString )
{
	NS_CStringToUTF16(ECString, NS_CSTRING_ENCODING_ASCII, EString );
}

void nsxpcm::Convert(
	const nsEmbedString &EString,
	nsEmbedCString &ECString )
{
	NS_UTF16ToCString( EString, NS_CSTRING_ENCODING_ASCII, ECString );
}

void nsxpcm::Convert(
	const nsEmbedString &EString,
	char **JString )
{
ERRProlog
	str::string String;
ERRBegin
	String.Init();

	Convert( EString, String );
	Convert( String, JString );
ERRErr
ERREnd
ERREpilog
}

void nsxpcm::Convert(
	const str::string_ &EString,
	char **JString )
{
ERRProlog
	tol::E_POINTER___( char ) Buffer;
ERRBegin
	Buffer = EString.Convert();

	Convert( Buffer, EString.Amount() + 1, JString );
ERRErr
ERREnd
ERREpilog
}

void nsxpcm::Convert(
	const char *String,
	nsEmbedString &EString )
{
	Convert( nsEmbedCString( String ), EString );
}

void nsxpcm::Convert(
	const str::string_ &String,
	nsEmbedString &EString )
{
ERRProlog
	tol::E_POINTER___( char ) Buffer;
ERRBegin
	Buffer = String.Convert();

	Convert( Buffer, EString );
ERRErr
ERREnd
ERREpilog
}

void nsxpcm::Convert(
	const nsEmbedString &EString,
	str::string_ &String )
{
	nsEmbedCString ECString;

	Convert( EString, ECString );

	String.Append( ECString.get() );
}

void nsxpcm::Split( 
	const string_ &Merged,
	strings_ &Splitted,
	bso::char__ Separator )
{
ERRProlog
	epeios::row__ Row = NONE;
	string Item;
ERRBegin
	Item.Init();

	Row = Merged.First();

	while ( Row != NONE ) {
		if ( Merged( Row ) != Separator )
			Item.Append( Merged( Row ) );
		else {
			Splitted.Append( Item );
			Item.Init();
		}

		Row = Merged.Next( Row );
	}

	if ( Item.Amount() != 0 )
		Splitted.Append( Item );
ERRErr
ERREnd
ERREpilog
}

void nsxpcm::Split(
	const char *Merged,
	strings_ &Splitted,
	bso::char__ Separator )
{
ERRProlog
	string SMerged;
ERRBegin
	SMerged.Init( Merged );

	Split( SMerged, Splitted, Separator );
ERRErr
ERREnd
ERREpilog
}

void nsxpcm::Merge(
	const strings_ &Splitted,
	string_ &Merged,
	bso::char__ Separator )
{
	epeios::row__ Row = Splitted.First();
	ctn::E_CMITEM( string_ ) Item;

	Item.Init( Splitted );

	if ( Row != NONE ) {
		Merged.Append( Item( Row ) );
		Row = Splitted.Next( Row );
	}

	while ( Row != NONE ) {
		Merged.Append( Separator );
		Merged.Append( Item( Row ) );
		Row = Splitted.Next( Row );
	}
}

#ifdef NSXPCM__BKD
void nsxpcm::Convert(
	const strings_ &Items,
	bkdacc::ids32_ &Ids )
{
	epeios::row__ Row = Items.First();
	epeios::row__ Error = NONE;
	ctn::E_CMITEM( string_ ) Item;

	Item.Init( Items );

	while( Row != NONE ) {
		Ids.Append( Item( Row ).ToUL( &Error ) );

		Row = Items.Next( Row );
	}

	if ( Error != NONE )
		ERRu();
}

void nsxpcm::SplitAndConvert(
	const char *Merged,
	bkdacc::ids32_ &Ids,
	bso::char__ Separator )
{
ERRProlog
	strings Splitted;
ERRBegin
	Splitted.Init();

	Split( Merged, Splitted, Separator );

	Convert( Splitted, Ids );
ERRErr
ERREnd
ERREpilog
}

inline static const char *Convert_(
	bso::ulong__ Value,
	bso::integer_buffer__ &Buffer )
{
	if ( Value == NONE )
		return "";
	else
		return bso::Convert( Value, Buffer );
}

template<typename id__> static inline void ConvertAndMerge_(
	const bch::E_BUNCH_( id__ ) &Ids,
	string_ &Merged,
	bso::char__ Separator )
{
	epeios::row__ Row = Ids.First();
	bso::integer_buffer__ Buffer;

	if ( Row != NONE ) {
		Merged.Append( Convert_( *Ids( Row ), Buffer ) );
		Row = Ids.Next( Row );
	}

	while ( Row != NONE ) {
		Merged.Append( Separator );
		Merged.Append( Convert_( *Ids( Row ), Buffer ) );
		Row = Ids.Next( Row );
	}
}


void nsxpcm::ConvertAndMerge(
	const bkdacc::ids8_ &Ids,
	string_ &Merged,
	bso::char__ Separator )
{
	ConvertAndMerge_( Ids, Merged, Separator );
}

void nsxpcm::ConvertAndMerge(
	const bkdacc::ids16_ &Ids,
	string_ &Merged,
	bso::char__ Separator )
{
	ConvertAndMerge_( Ids, Merged, Separator );
}

void nsxpcm::ConvertAndMerge(
	const bkdacc::ids32_ &Ids,
	string_ &Merged,
	bso::char__ Separator )
{
	ConvertAndMerge_( Ids, Merged, Separator );
}
#endif


class nsxpcmpersonnalization
: public nsxpcmtutor
{
public:
	nsxpcmpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~nsxpcmpersonnalization( void )
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

static nsxpcmpersonnalization Tutor;

ttr_tutor &NSXPCMTutor = Tutor;
