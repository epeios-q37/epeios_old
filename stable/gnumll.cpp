/*
  'gnumll' library by Claude L. Simon (epeios@epeios.org)
  Requires the 'gnumll' header file ('gnumll.h').
  Copyright (C) 2000 Claude L. SIMON (epeios@epeios.org).

  This file is part of the Epeios (http://www.epeios.org/) project.
  

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.
 
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, go to http://www.fsf.org or write to the:
  
                        Free Software Foundation, Inc.,
           59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

//	$Id$

#define GNUMLL__COMPILATION

#include "gnumll.h"

class gnumlltutor
: public ttr_tutor
{
public:
	gnumlltutor( void )
	: ttr_tutor( GNUMLL_NAME )
	{
#ifdef GNUMLL_DBG
		Version = GNUMLL_VERSION " (DBG)";
#else
		Version = GNUMLL_VERSION;
#endif
		Owner = GNUMLL_OWNER;
		Date = "$Date$";
	}
	virtual ~gnumlltutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

enum tag {
	tEmailAndName,
	tEmailOnly,
	tNewsgroup,
	tContribution,
	tAnnouncement,
	tFAQ_URL,
	tOtherEmail,
	amount,
	tUnknow = amount	// Must be last, just after 'amount'.
};

const char *Tag[] = {
	" to subscribe to ",
	" to subscribe",
	"gnUSENET newsgroup: ",
	"Send contributions to: ",
	"Send announcements to: ",
	"FAQ-URL: ",
	" to: ",
};

static tag GetType_(
	const str_string_ &L,
	POSITION__ &P,
	bso__bsize &S )
{
	int t = 0;

	while( ( t < amount )
		   && ( ( P = L.Search( str_string( Tag[t] ) ) ) == NONE ) )
		t++;

	if ( t != amount )
		S = strlen( Tag[t] );

	return (tag)t;
}
	

static const str_string_ &FilterTitlePrefix_( const str_string_ &Title )
{
	static str_string String;
	POSITION__ P = 0;

	String.Init();

	while( ( P < Title.Amount() ) && ( Title( P ) == '*' ) )
		P++;

	if ( ( P < Title.Amount() ) && ( Title( P ) == ' ' ) )
		P++;

	String.WriteAndAdjust( Title, Title.Amount() - P, P );

	return String;
}

static void GetListSectionHeader_(
	const txmpgh_paragraph_ &Paragraph,
	gnumll_section_header_ &Header )
{
	CMITEM( str_string_ ) L;
	POSITION__ P = Paragraph.First();

	L.Init( Paragraph );

	Header.Init();

	while( P != NONE ) {
		Header.Parse( FilterTitlePrefix_( L( P ) ) );
		P = Paragraph.Next( P );
	}
}

static void GetSection_(
	txmpgh_paragraph_ &Paragraph,
	gnumll_section_ &Section,
	xtf::extended_text_iflow___ &Flow )
{
	bso__bool Cont = true;
	CMITEM( str_string_ ) Item;

	do {
		Paragraph.Init();

		Paragraph.Parse( Flow, txmpgh::sSplit );

		Item.Init( Paragraph );

		if ( ( Paragraph.Amount() ) && ( Item( Paragraph.First() )( 0 ) != '*' ) )
			Section.Add( Paragraph );
		else
			Cont = false;
	} while ( Cont );
}


static void GetHead_(
	txmpgh_paragraph_ &Paragraph,
	gnumll_head_section_ &Head,
	xtf::extended_text_iflow___ &Flow )
{
	bso__bool Cont = true;

	do {
		Paragraph.Init();

		Paragraph.Parse( Flow, txmpgh::sSplit );

		if ( ( Paragraph.Amount() ) && ( Paragraph( Paragraph.First() )( 0 ) != '*' ) )
			Head.Add( Paragraph );
		else
			Cont = false;

		Paragraph.Sync();

	} while ( Cont );

}

static void GetGeneralSections_(
	txmpgh_paragraph_ &Paragraph,
	gnumll_general_sections_ &Generals,
	xtf::extended_text_iflow___ &Flow )
{
ERRProlog
	gnumll_general_section General;
ERRBegin

	do {
		General.Init();
		General.Title = FilterTitlePrefix_( Paragraph( 0 ) );

		GetSection_( Paragraph, General.Section, Flow );

		Generals.Add( General );
	} while ( Paragraph.Amount() == 1 );
ERRErr
ERREnd
ERREpilog
}

static void GetListSections_(
	txmpgh_paragraph_ &Paragraph,
	gnumll_list_sections_ &Lists,
	xtf::extended_text_iflow___ &Flow )
{
ERRProlog
	gnumll_list_section List;
ERRBegin

	do {
		List.Init();

		GetListSectionHeader_( Paragraph, List.Header );

		GetSection_( Paragraph, List.Section, Flow );

		if ( List.Section.Amount() )
			Lists.Add( List );
	} while ( !Flow.EOX() );
ERRErr
ERREnd
ERREpilog
}

static inline void Correct_( gnumll_section_ &Section )
{
	if ( Section.Amount() )
		Section.Allocate( Section.Amount() - 1 );
}

static inline void WriteEnd_(
	const str_string_ &Source,
	str_string_ &Dest,
	POSITION__ P,
	bso__bsize S )
{
	Dest.WriteAndAdjust( Source, Source.Amount() - ( P + S ), P + S );
}

static bso__bool Test_( const str_string_ &S )
{
	bso__bool R = false;
ERRProlog
	str_string T;
ERRBegin
	T.Init();

	T = S;

	STRToUpper( T );

	R = ( T == str_string( "NONE" ) ) || ( T == str_string( "NEVER" ) );
ERRErr
ERREnd
ERREpilog
	return R;
}

static void HandleNewsgroup_(
	const str_string_ &Source,
	POSITION__ P,
	bso__bsize S,
	str_string_ &NG,
	str_string_ &NGC )
{
ERRProlog
	str_string T;
	SIZE__ L;
	bso__char C;
ERRBegin
	NG.Init();
	NGC.Init();

	T.Init();

	WriteEnd_( Source, T, P, S );

	L = T.Amount();

	while( ( P < L ) && ( !isspace( C = T( P ) ) ) )
	{
		NG.Add( C );
		P++;
	}

	if ( Test_( NG ) ) {
		NG.Init();
		NGC = T;
	}
	else if ( P != L ) {
		while( ( P < L ) && ( isspace( T( P ) ) ) )
			P++;

		if ( P != L )
			while( P < L )
				{
					NGC.Add( T( P ) );
					P++;
				}
	}
ERRErr
ERREnd
ERREpilog
}


void gnumll_section_header_::Parse( const str_string_ &L )
{
	POSITION__ P;
	bso__bsize S;

	switch( GetType_( L, P, S ) ) {
	case tEmailAndName:
		WriteEnd_( L, Name, P, S );
	case tEmailOnly:
		Email.WriteAndAdjust( L, P, 0 );
		break;
	case tNewsgroup:
		HandleNewsgroup_( L, P, S, Newsgroup, NewsgroupComment );
		break;
	case tAnnouncement:
		WriteEnd_( L, Announcement, P, S );
		break;
	case tContribution:
		WriteEnd_( L, Contribution, P, S );
		break;
	case tFAQ_URL:
		WriteEnd_( L, FAQ_URL, P, S );
		break;
	case tOtherEmail:
		TextOtherEmail.WriteAndAdjust( L, P + S );
		WriteEnd_( L, OtherEmail, P, S );
		break;
	case tUnknow:
		Misc.Add( L );
		break;
	default:
		ERRc();
		break;
	}
}

void gnumll_mailing_lists_::Parse( xtf::extended_text_iflow___ &Flow )
{
ERRProlog
	txmpgh_paragraph Paragraph;
ERRBegin
	Paragraph.Init();

	GetHead_( Paragraph, Head, Flow );

	GetGeneralSections_( Paragraph, Generals, Flow );

	GetListSections_( Paragraph, Lists, Flow );

	if ( Lists.Amount() )
	{
		Correct_( Lists( Lists.Last() ).Section );
		Lists.Sync();
	}
ERRErr
ERREnd
ERREpilog
}

template <class t> void Print_(
	txf::text_oflow___ &F,
	const CONTAINER_( t ) &C )
{
	POSITION__ P = C.First();
	CITEM( t ) Element;

	Element.Init( C );

	while( P != NONE ) {
		F << "> " << Element( P );
		P = C.Next( P );
	}
}


txf::text_oflow___ &operator <<(
	txf::text_oflow___ &F,
	const gnumll_section_ &S )
{
	Print_<txmpgh_paragraph_>( F, S );
	return F;
}

txf::text_oflow___ &operator <<(
	txf::text_oflow___ &F,
	const gnumll_general_sections_ &S )
{
	Print_<gnumll_general_section_>( F, S );
	return F;
}

txf::text_oflow___ &operator <<(
	txf::text_oflow___ &F,
	const gnumll_list_sections_ &S )
{
	Print_<gnumll_list_section_>( F, S );
	return F;
}


txf::text_oflow___ &operator <<(
	txf::text_oflow___ &F,
	const gnumll_titles_ &S )
{
	POSITION__ P = S.First();
	CMITEM( str_string_ ) Element;

	Element.Init( S );

	while( P != NONE ) {
		F << txf::tab << Element( P ) << txf::nl;
		P = S.Next( P );
	}

	return F;
}

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */
class gnumllpersonnalization
: public gnumlltutor
{
public:
	gnumllpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~gnumllpersonnalization( void )
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

static gnumllpersonnalization Tutor;

ttr_tutor &GNUMLLTutor = Tutor;
