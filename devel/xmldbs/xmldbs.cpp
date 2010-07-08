/*
	'xmldbs' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'xmldbs' header file ('xmldbs.h').
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

#define XMLDBS__COMPILATION

#include "xmldbs.h"

class xmldbstutor
: public ttr_tutor
{
public:
	xmldbstutor( void )
	: ttr_tutor( XMLDBS_NAME )
	{
#ifdef XMLDBS_DBG
		Version = XMLDBS_VERSION "\b\bD $";
#else
		Version = XMLDBS_VERSION;
#endif
		Owner = XMLDBS_OWNER;
		Date = "$Date$";
	}
	virtual ~xmldbstutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace xmldbs;

value_row__ database_::Next(
	value_row__ ValueRow,
	const value_ &Value ) const
{
	value_row__ &P = ValueRow;
	ctn::E_CMITEMt( tagged_value_, value_row__ ) TaggedValue;

	TaggedValue.Init( *this );

	do
		P = Next( P );
	while( ( P != NONE ) && ( TaggedValue( P ).Value != Value ) );

	return P;
}

value_row__ database_::Next(
	value_row__ ValueRow,
	tag_row__ TagRow,
	const value_ &Value ) const
{
	value_row__ &P = ValueRow;
	ctn::E_CMITEMt( tagged_value_, value_row__ ) TaggedValue;

	TaggedValue.Init( *this );

	do
		P = Next( P, TagRow );
	while( ( P != NONE ) && ( TaggedValue( P ).Value != Value ) );

	return P;
}



value_row__ database_::First(
	value_row__ ValueRow,
	const value_ &Value ) const
{
	value_row__ &P = ValueRow;
	ctn::E_CMITEMt( tagged_value_, value_row__ ) TaggedValue;

	TaggedValue.Init( *this );

	P = First( P );

	if ( ( P != NONE ) && ( TaggedValue( P ).Value != Value ) )
		P = Next( ValueRow, Value );

	return P;
}

value_row__ database_::First(
	value_row__ ValueRow,
	tag_row__ TagRow,
	const value_ &Value ) const
{
	value_row__ &P = ValueRow;
	ctn::E_CMITEMt( tagged_value_, value_row__ ) TaggedValue;

	TaggedValue.Init( *this );

	P = First( P, TagRow );

	if ( ( P != NONE ) && ( TaggedValue( P ).Value != Value ) )
		P = Next( P, TagRow, Value );

	return P;
}

static inline const str::string_  &Filter_(
	const str::string_ &String,
	str::string_ &Result )
{
	Result = String;

	xmlcor::Convert( Result );

	return Result;
}

static bso::bool__ WriteAttributes_(
	const database_ &XMLD,
	value_row__ ValueRow,
	txf::text_oflow__ &Flow )
{
	bso::bool__ Return = false;
ERRProlog
	ctn::E_CMITEMt( tag_, tag_row__ ) Tag;
	ctn::E_CMITEMt( tagged_value_, value_row__ ) TaggedValue;
	str::string Result;
ERRBegin
	Tag.Init( XMLD.Tags );
	TaggedValue.Init( XMLD );

	if ( ( ValueRow = XMLD.First( ValueRow ) ) != NONE )
		Return = true;

	while( ValueRow != NONE ) {
		if ( Tag( TaggedValue( ValueRow ).TagRow() ).Type() == tAttribute ) {
			Flow << ' ' << Tag( TaggedValue( ValueRow ).TagRow() ).Name << "=\"";

			Result.Init();
			Filter_( TaggedValue( ValueRow ).Value, Result );
			Flow << Result << '"';
		}

		ValueRow = XMLD.Next( ValueRow );
	}
ERRErr
ERREnd
ERREpilog
	return Return;
}

void xmldbs::WriteXML(
	const database_ &XMLD,
	txf::text_oflow__ &Flow )
{
ERRProlog
	dtr::browser__<value_row__> Browser;
	ctn::E_CMITEMt( tagged_value_, value_row__ ) TaggedValue;
	ctn::E_CMITEMt( tag_, tag_row__ ) Tag;
	tag_row__ TagRow;
	bso::bool__ HasChild;
	str::string Result;
ERRBegin
	Browser.Init( XMLD.GetRoot() );
	TaggedValue.Init( XMLD );
	Tag.Init( XMLD.Tags );

	while( Browser.Position() != NONE ) {
		switch( Browser.Status() ) {
		case dtr::bFirst:
		case dtr::bChild:
		case dtr::bBrother:
			if ( !Tag( TagRow = TaggedValue( Browser.Position() ).TagRow() ).IsAttribute() ) {
				Flow << "<" << Tag( TagRow ).Name;
				HasChild = WriteAttributes_( XMLD, Browser.Position(), Flow );
				Flow << '>';

				Result.Init();
				Filter_( TaggedValue( Browser.Position() ).Value, Result );
				Flow << Result;
			}

			if ( HasChild )
				break;
		case dtr::bParent:
		case dtr::bLast:
			if ( !Tag( TaggedValue( Browser.Position() ).TagRow() ).IsAttribute() )
				Flow << "</" << Tag( TaggedValue( Browser.Position() ).TagRow() ).Name << '>';
			break;
		default:
			ERRc();
			break;
		}

		XMLD.Browse( Browser );
	}
ERRErr
ERREnd
ERREpilog
}

static bso::bool__ WriteAttributes_(
	const database_ &XMLD,
	value_row__ ValueRow,
	database_filler__ &XMLDF )
{
	bso::bool__ Return = false;
	ctn::E_CMITEMt( tag_, tag_row__ ) Tag;
	ctn::E_CMITEMt( tagged_value_, value_row__ ) TaggedValue;

	Tag.Init( XMLD.Tags );
	TaggedValue.Init( XMLD );

	if ( ( ValueRow = XMLD.First( ValueRow ) ) != NONE )
		Return = true;

	while( ValueRow != NONE ) {
		if ( Tag( TaggedValue( ValueRow ).TagRow() ).Type() == tAttribute ) {
			XMLDF.PutAttribute( Tag( TaggedValue( ValueRow ).TagRow() ).Name, TaggedValue( ValueRow ).Value );
		}

		ValueRow = XMLD.Next( ValueRow );
	}

	return Return;
}

void xmldbs::database_filler__::Put( const database_ &XMLD )
{
ERRProlog
	dtr::browser__<value_row__> Browser;
	ctn::E_CMITEMt( tagged_value_, value_row__ ) TaggedValue;
	ctn::E_CMITEMt( tag_, tag_row__ ) Tag;
	tag_row__ TagRow;
	bso::bool__ HasChild;
ERRBegin
	Browser.Init( XMLD.GetRoot() );
	TaggedValue.Init( XMLD );
	Tag.Init( XMLD.Tags );

	while( Browser.Position() != NONE ) {
		switch( Browser.Status() ) {
		case dtr::bFirst:
		case dtr::bChild:
		case dtr::bBrother:
			if ( !Tag( TagRow = TaggedValue( Browser.Position() ).TagRow() ).IsAttribute() ) {
				PushTag( Tag( TagRow ).Name );
				HasChild = WriteAttributes_( XMLD, Browser.Position(), *this );
				PutValue( TaggedValue( Browser.Position() ).Value );
			}

			if ( HasChild )
				break;
		case dtr::bParent:
		case dtr::bLast:
			if ( !Tag( TaggedValue( Browser.Position() ).TagRow() ).IsAttribute() )
				PopTag();
			break;
		default:
			ERRc();
			break;
		}

		XMLD.Browse( Browser );
	}
ERRErr
ERREnd
ERREpilog
}


/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class xmldbspersonnalization
: public xmldbstutor
{
public:
	xmldbspersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~xmldbspersonnalization( void )
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

static xmldbspersonnalization Tutor;

ttr_tutor &XMLDBSTutor = Tutor;
