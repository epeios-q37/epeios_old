/*
  'xmlcor' library by Claude L. Simon (csimon@webmails.com)
  Requires the 'xmlcor' header file ('xmlcor.h').
  Copyright (C) 2000,2001 Claude L. SIMON (csimon@webmails.com).

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

#define XMLCOR__COMPILATION

#include "xmlcor.h"

class xmlcortutor
: public ttr_tutor
{
public:
	xmlcortutor( void )
	: ttr_tutor( XMLCOR_NAME )
	{
#ifdef XMLCOR_DBG
		Version = XMLCOR_VERSION "\b\bD $";
#else
		Version = XMLCOR_VERSION;
#endif
		Owner = XMLCOR_OWNER;
		Date = "$Date$";
	}
	virtual ~xmlcortutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace xmlcor;

void xmlcor::Add(
	const tagged_values_ &Source,
	const tag_map_ &TagMap,
	tagged_values_ &Target )
{
ERRProlog
	value_row__ TargetRow;
	ctn::E_CMITEMt( tagged_value_, value_row__ ) SourceItem, TargetItem;
	dtr::browser__<value_row__> Browser;
	tagged_value Buffer;
ERRBegin
	SourceItem.Init( Source );
	TargetItem.Init( Target );
	
	TargetRow = Target.GetRoot();
	
	Browser.Init( Source.GetRoot() );
	
	Buffer.Init();
	
	while( Browser.Position() != NONE ) {
		switch( Browser.Status() ) {
		case dtr::bChild:
			if ( ( Buffer = SourceItem( Browser.Position() ) ).TagRow() != NONE )
				Buffer.TagRow( TagMap( Buffer.TagRow() ) );
				
			TargetRow = Target.BecomeLast( Buffer, TargetRow );
			break;									
		case dtr::bBrother:
			if ( ( Buffer = SourceItem( Browser.Position() ) ).TagRow() != NONE )
				Buffer.TagRow( TagMap( Buffer.TagRow() ) );
				
			TargetRow = Target.BecomeNext( Buffer, TargetRow );
			break;									
		case dtr::bParent:
		case dtr::bLast:
			TargetRow = Target.Parent( TargetRow );
			break;
		case dtr::bFirst:
			break;
		default:
			ERRc();
			break;
		}
		
		Source.Browse( Browser );
	}
ERRErr
ERREnd
ERREpilog
}

value_row__ xml_core_::Next(
	value_row__ ValueRow,
	tag_row__ TagRow ) const
{
	value_row__ &P = ValueRow;
	ctn::E_CMITEMt( tagged_value_, value_row__ ) TaggedValue;
	
	TaggedValue.Init( *this );
		
	do	
		P = Next( P );
	while( ( P != NONE ) && ( TaggedValue( P ).TagRow() != TagRow ) );
		
	return P;
}

value_row__ xml_core_::First(
	value_row__ ValueRow,
	tag_row__ TagRow ) const
{
	value_row__ &P = ValueRow;
	ctn::E_CMITEMt( tagged_value_, value_row__ ) TaggedValue;
	
	TaggedValue.Init( *this );
	
	P = First( P );

	if ( ( P != NONE ) && ( TaggedValue( P ).TagRow() != TagRow ) )
		P = Next( P, TagRow );
		
	return P;
}



/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class xmlcorpersonnalization
: public xmlcortutor
{
public:
	xmlcorpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~xmlcorpersonnalization( void )
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

static xmlcorpersonnalization Tutor;

ttr_tutor &XMLCORTutor = Tutor;
