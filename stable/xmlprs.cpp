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
using xmldbs::xml_database_filler__;

template <typename xml_filler__> static void HandleAttributes_(
	xml_filler__ &XMLF,
	const XML_Char **Atts )
{
	int C = 0;
	
	while( Atts[C] != NULL ) {
		XMLF.PutAttribute( str::string( Atts[C] ), str::string( Atts[C+1] ) );
		C +=2;
	}
}

template <typename xml_filler__> static inline void StartElementHandler_(
	xml_filler__ &XMLFiller,
	const XML_Char *Name,
	const XML_Char **Atts )
{
	XMLFiller.PushTag( str::string( Name ) );		
	HandleAttributes_( XMLFiller, Atts );
}

static void DCMStartElementHandler_(
	void *UserData,
	const XML_Char *Name,
	const XML_Char **Atts )
{
	StartElementHandler_( *(xml_document_filler__ *)UserData, Name, Atts );
}

static void DBSStartElementHandler_(
	void *UserData,
	const XML_Char *Name,
	const XML_Char **Atts )
{
	StartElementHandler_( *(xml_database_filler__ *)UserData, Name, Atts );
}

template <typename xml_filler__> static void EndElementHandler_(
	xml_filler__ &XMLFiller,
	const XML_Char * )
{
	XMLFiller.PopTag();		
}

static void DCMEndElementHandler_(
	void *UserData,
	const XML_Char *Name )
{
	EndElementHandler_( *(xml_document_filler__ *)UserData, Name );
}

static void DBSEndElementHandler_(
	void *UserData,
	const XML_Char *Name )
{
	EndElementHandler_( *(xml_database_filler__ *)UserData, Name );
}

template <typename xml_filler__> static void CharacterDataHandler_(
	xml_filler__ &XMLFiller,
	const XML_Char *S,
	int Len )
{
	XMLFiller.PutValue( xmldcm::value( S, Len ) );
}		

static void DCMCharacterDataHandler_(
	void *UserData,
	const XML_Char *S,
	int Len )
{
	CharacterDataHandler_( *(xml_document_filler__ *)UserData, S, Len );
}		

static void DBSCharacterDataHandler_(
	void *UserData,
	const XML_Char *S,
	int Len )
{
	CharacterDataHandler_( *(xml_database_filler__ *)UserData, S, Len );
}		

static void ExpatParse_(
	txf::text_iflow___ &Flow,
	XML_Parser &Parser,
	int Length )
{
ERRProlog
	txf::data__ *Buffer;
	txf::amount__ Amount;
ERRBegin
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


void xmlprs::ParseXML(
	txf::text_iflow___ &Flow,
	xmldcm::xml_document_ &XMLD,
	int Length )
{
ERRProlog
	XML_Parser Parser;
	xml_document_filler__ XMLDF;
ERRBegin
	XMLDF.Init( XMLD );

	Parser = XML_ParserCreate( NULL );

	XML_SetElementHandler( Parser, DCMStartElementHandler_, DCMEndElementHandler_ );
	XML_SetCharacterDataHandler( Parser, DCMCharacterDataHandler_ );

	XML_SetUserData( Parser, &XMLDF );

	ExpatParse_( Flow, Parser, Length );
ERRErr
ERREnd
ERREpilog
}

void xmlprs::ParseXML(
	txf::text_iflow___ &Flow,
	xmldbs::xml_database_ &XMLD,
	int Length )
{
ERRProlog
	XML_Parser Parser;
	xml_database_filler__ XMLDF;
ERRBegin
	XMLDF.Init( XMLD );

	Parser = XML_ParserCreate( NULL );

	XML_SetElementHandler( Parser, DBSStartElementHandler_, DBSEndElementHandler_ );
	XML_SetCharacterDataHandler( Parser, DBSCharacterDataHandler_ );

	XML_SetUserData( Parser, &XMLDF );

	ExpatParse_( Flow, Parser, Length );
ERRErr
ERREnd
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
