/*
  'xmlprs' library by Claude L. Simon (csimon@webmails.com)
  Requires the 'xmlprs' header file ('xmlprs.h').
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

#define XMLPRS__COMPILATION

#include "xmlprs.h"

class xmlprstutor
: public ttr_tutor
{
public:
	xmlprstutor( void )
	: ttr_tutor( XMLPRS_NAME )
	{
#ifdef XMLPRS_DBG
		Version = XMLPRS_VERSION "\b\bD $";
#else
		Version = XMLPRS_VERSION;
#endif
		Owner = XMLPRS_OWNER;
		Date = "$Date$";
	}
	virtual ~xmlprstutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#include "expat.h"

using xmldcm::xml_document_filler__;

static void HandleAttributes_(
	xml_document_filler__ &XMLDF,
	const XML_Char **Atts )
{
	int C = 0;
	
	while( Atts[C] != NULL ) {
		XMLDF.PutAttribute( str::string( Atts[C] ), str::string( Atts[C+1] ) );
		C +=2;
	}
}
		
static void StartElementHandler_(
	void *UserData,
	const XML_Char *Name,
	const XML_Char **Atts )
{
	xml_document_filler__ &XMLDF = *(xml_document_filler__ *)UserData;

	XMLDF.PushTag( str::string( Name ) );		
	HandleAttributes_( XMLDF, Atts );
}

static void EndElementHandler_(
	void *UserData,
	const XML_Char * )
{
	xml_document_filler__ &XMLDF = *(xml_document_filler__ *)UserData;

	XMLDF.PopTag();		

}

static void CharacterDataHandler_(
	void *UserData,
	const XML_Char *S,
	int Len )
{
	xml_document_filler__ &XMLDF = *(xml_document_filler__ *)UserData;

	XMLDF.PutValue( xmldcm::value( S, Len ) );
}		

void xmlprs::ParseXML(
	txf::text_iflow___ &Flow,
	xmldcm::xml_document_ &XMLD,
	int Length )
{
ERRProlog
	XML_Parser Parser;
	txf::data__ *Buffer;
	txf::amount__ Amount;
	xml_document_filler__ XMLDF;
ERRBegin
	XMLDF.Init( XMLD );

	Parser = XML_ParserCreate( NULL );

	XML_SetElementHandler( Parser, StartElementHandler_, EndElementHandler_ );
	XML_SetCharacterDataHandler( Parser, CharacterDataHandler_ );

	XML_SetUserData( Parser, &XMLDF );

	if ( ( Buffer = (txf::data__ *)XML_GetBuffer( Parser, Length ) ) == NULL )
		ERRm();

	Amount = Flow.Get( Length, Buffer );

	while( Buffer[Amount - 1] != 0x1a ) {

		if ( !XML_ParseBuffer( Parser, Amount, false ) )
			ERRf();

		if ( ( Buffer = (txf::data__ *)XML_GetBuffer( Parser, Length ) ) == NULL )
			ERRm();

		Amount = Flow.Get( Length, Buffer );
	}

	if ( !XML_ParseBuffer( Parser, Amount - 1, true ) )
		ERRf();


ERRErr
ERREnd
	XML_ParserFree( Parser );
ERREpilog
}

void xmlprs::License( txf::text_oflow___ &Flow )
{
	Flow << "expat copyright (c) 1998, 1999, 2000 Thai Open Source Software Center Ltd" << txf::nl;
	Flow << "and Clark Cooper" << txf::nl;
	Flow << txf::nl;
	Flow << "Permission is hereby granted, free of charge, to any person obtaining" << txf::nl;
	Flow << "a copy of this software and associated documentation files (the" << txf::nl;
	Flow << "\"Software\"), to deal in the Software without restriction, including" << txf::nl;
	Flow << "without limitation the rights to use, copy, modify, merge, publish," << txf::nl;
	Flow << "distribute, sublicense, and/or sell copies of the Software, and to" << txf::nl;
	Flow << "permit persons to whom the Software is furnished to do so, subject to" << txf::nl;
	Flow << "the following conditions:" << txf::nl;
	Flow << txf::nl;
	Flow << "The above copyright notice and this permission notice shall be included" << txf::nl;
	Flow << "in all copies or substantial portions of the Software." << txf::nl;
}


/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class xmlprspersonnalization
: public xmlprstutor
{
public:
	xmlprspersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~xmlprspersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the ending of the application  */
	}
	void TTRMore( txf::text_oflow___ &Flow )
	{
		Flow << txf::tab << "'XMLPRS' uses " << XML_ExpatVersion() << txf::nl << txf::tab << "(c) Thai Open Source Software Center Ltd and Clark Cooper." << txf::nl;
	}
};


/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

// 'static' by GNU C++.

static xmlprspersonnalization Tutor;

ttr_tutor &XMLPRSTutor = Tutor;
