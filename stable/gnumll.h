/*
  Header for the 'gnumll' library by Claude L. Simon (epeios@epeios.org)
  Copyright (C) 2000 Claude L. SIMON (epeios@epeios.org) 

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
  along with this program; if not, go to http://www.fsf.org or write to the
  
                        Free Software Foundation, Inc.,
           59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

//	$Id$

#ifndef GNUMLL__INC
#define GNUMLL__INC

#define GNUMLL_NAME		"GNUMLL"

#define	GNUMLL_VERSION	"$Revision$"	

#define GNUMLL_OWNER		"the Epeios project (http://www.epeios.org/)"

#include "ttr.h"

extern class ttr_tutor &GNUMLLTutor;

#if defined( XXX_DBG ) && !defined( GNUMLL_NODBG )
#define GNUMLL_DBG 
#endif

/* Begin of automatic documentation generation part. */

//V $Revision$
//C Claude L. SIMON (epeios@epeios.org)
//R $Date$

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#include "txf.h"
#include "txmpgh.h"

typedef XCONTAINER_( txmpgh_paragraph_ )	gnumll_section_;
typedef XCONTAINER( txmpgh_paragraph_ )		gnumll_section;

txf::text_oflow___ &operator <<(
	txf::text_oflow___ &F,
	const gnumll_section_ &S );


//t The before section
typedef gnumll_section_ gnumll_head_section_;
typedef gnumll_section	gnumll_head_section;

//c The general section
class gnumll_general_section_
{
public:
	//o Title of the section.
	str_string_ Title;
	//o The section itself.
	gnumll_section_ Section;
	struct s {
		str_string_::s Title;
		gnumll_section_::s Section;
	};
	gnumll_general_section_( s &S )
	: Title( S.Title ),
	  Section( S.Section )
	{}
	void reset( bool P = true )
	{
		Title.reset( P );
		Section.reset( P );
	}
	void plug( mmm_multimemory_ &M )
	{
		Title.plug( M );
		Section.plug( M );
	}
	gnumll_general_section_ &operator =( const gnumll_general_section_ &S )
	{
		Title = S.Title;
		Section = S.Section;

		return *this;
	}
	//f Initialization.
	void Init( void )
	{
		Title.Init();
		Section.Init();
	}
};

AUTO( gnumll_general_section )

inline txf::text_oflow___ &operator <<(
	txf::text_oflow___ &F,
	const gnumll_general_section_ &S )
{
	return F << "-----> Title: " << S.Title << txf::nl << S.Section << txf::nl;
}


typedef XCONTAINER( gnumll_general_section_ ) gnumll_general_sections;
typedef XCONTAINER_( gnumll_general_section_ ) gnumll_general_sections_;

txf::text_oflow___ &operator <<(
	txf::text_oflow___ &F,
	const gnumll_general_sections_ &S );


typedef XMCONTAINER_( str_string_ ) gnumll_misc_;
typedef XMCONTAINER( str_string_ ) gnumll_misc;

txf::text_oflow___ &operator <<(
	txf::text_oflow___ &F,
	const gnumll_misc_ &S );

//c Section header.
class gnumll_section_header_
{
public:
	struct s {
		str_string_::s Name;
		str_string_::s Email;
		str_string_::s OtherEmail;
		str_string_::s TextOtherEmail;
		str_string_::s Newsgroup;
		str_string_::s NewsgroupComment;
		str_string_::s Contribution;
		str_string_::s Announcement;
		str_string_::s FAQ_URL;
		gnumll_misc_::s Misc;
	};
	//o Name of the mailing-list.
	str_string_ Name;
	//o E-mail of the mailing-list.
	str_string_ Email;
	//o Other E-mail (bug reports, source code, articles, ...)
	str_string_ OtherEmail;
	//o Text associated to the other E-mail.
	str_string_ TextOtherEmail;
	//o Newsgroup name.
	str_string_ Newsgroup;
	//o Newsgroup comment.
	str_string_ NewsgroupComment;
	//o Contribution e-mail
	str_string_ Contribution;
	//o Announcement e-mail
	str_string_ Announcement;
	//o The FAQ_URL
	str_string_ FAQ_URL;
	//o Misc.
	gnumll_misc_ Misc;
	gnumll_section_header_( s &S )
	: Name( S.Name ),
	  Email( S.Email ),
	  OtherEmail( S.OtherEmail ),
	  TextOtherEmail( S.TextOtherEmail ),
	  Newsgroup( S.Newsgroup ),
	  NewsgroupComment( S.NewsgroupComment ),
	  Contribution( S.Contribution ),
	  Announcement( S.Announcement ),
	  FAQ_URL( S.FAQ_URL ),
	  Misc( S.Misc )
	{}
	void reset( bool P = true )
	{
		Name.reset( P );
		Email.reset( P );
		OtherEmail.reset( P );
		TextOtherEmail.reset( P );
		Newsgroup.reset( P );
		NewsgroupComment.reset( P );
		Contribution.reset( P );
		Announcement.reset( P );
		FAQ_URL.reset( P );
		Misc.reset( P );
	}
	void plug( mmm_multimemory_ & M )
	{
		Name.plug( M );
		Email.plug( M );
		OtherEmail.plug( M );
		TextOtherEmail.plug( M );
		Newsgroup.plug( M );
		NewsgroupComment.plug( M );
		Contribution.plug( M );
		Announcement.plug( M );
		FAQ_URL.plug( M );
		Misc.plug( M );
	}
	gnumll_section_header_ &operator =( const gnumll_section_header_ &H )
	{
		Name = H.Name;
		Email = H.Email;
		OtherEmail = H.OtherEmail;
		TextOtherEmail = H.TextOtherEmail;
		Newsgroup = H.Newsgroup;
		NewsgroupComment = H.NewsgroupComment;
		Contribution = H.Contribution;
		Announcement = H.Announcement;
		FAQ_URL = H.FAQ_URL;
		Misc = H.Misc;

		return *this;
	}
	//f Initialization
	void Init( void )
	{
		Name.Init();
		Email.Init();
		OtherEmail.Init();
		TextOtherEmail.Init();
		Newsgroup.Init();
		NewsgroupComment.Init();
		Contribution.Init();
		Announcement.Init();
		FAQ_URL.Init();
		Misc.Init();
	}
	//f Parse the line 'Line'.
	void Parse( const str_string_ &L );
};

// internal use.
inline void GNUMLLPrint_(
	const str_string_ &S,
	const char *L,
	txf::text_oflow___ &F )
{
	if ( S.Amount() )
		F << txf::tab << L << ": >" << S << '<' << txf::nl;
}

inline txf::text_oflow___ &operator <<(
	txf::text_oflow___ &F,
	const gnumll_section_header_ &S )
{
	GNUMLLPrint_( S.Name, "Name", F );
	GNUMLLPrint_( S.Email, "E-mail", F );
	GNUMLLPrint_( S.OtherEmail, "Other e-mail", F );
	GNUMLLPrint_( S.TextOtherEmail, "Text other e-mail", F );
	GNUMLLPrint_( S.Newsgroup, "Newsgroup", F );
	GNUMLLPrint_( S.NewsgroupComment, "Newsgroup comment", F );
	GNUMLLPrint_( S.Contribution, "Contribution", F );
	GNUMLLPrint_( S.FAQ_URL, "FAQ URL", F );
	GNUMLLPrint_( S.Announcement, "Announcement", F );

	if ( S.Misc.Amount() )
		F << txf::nl << "Misc:" << txf::nl << S.Misc << txf::nl;

	return F;
}



typedef XMCONTAINER_( str_string_ ) gnumll_titles_;
typedef XMCONTAINER( str_string_ ) gnumll_titles;

txf::text_oflow___ &operator <<(
	txf::text_oflow___ &F,
	const gnumll_titles_ &S );


//c A list section.
class gnumll_list_section_
{
public:
	//o Section header.
	gnumll_section_header_ Header;
	//o The section itself.
	gnumll_section_ Section;
	struct s {
		gnumll_section_header_::s Header;
		gnumll_section_::s Section;
	};
	gnumll_list_section_( s &S )
	: Header( S.Header ),
	  Section( S.Section )
	{}
	void reset( bool P = true )
	{
		Header.reset( P );
		Section.reset( P );
	}
	void plug( mmm_multimemory_ &M )
	{
		Header.plug( M );
		Section.plug( M );
	}
	gnumll_list_section_ &operator =( const gnumll_list_section_ &S )
	{
		Header = S.Header;
		Section = S.Section;

		return *this;
	}
	//f Initialization.
	void Init( void )
	{
		Header.Init();
		Section.Init();
	}
};

AUTO( gnumll_list_section );

inline txf::text_oflow___ &operator <<(
	txf::text_oflow___ &F,
	const gnumll_list_section_ &S )
{
	F << "---> Header:" << txf::nl << S.Header << txf::nl;
	return F << S.Section << txf::nl;
}

typedef XCONTAINER_( gnumll_list_section_ ) gnumll_list_sections_;
typedef XCONTAINER( gnumll_list_section_ )	gnumll_list_sections;

txf::text_oflow___ &operator <<(
	txf::text_oflow___ &F,
	const gnumll_list_sections_ &S );


//c A GNU mailing lists.
class gnumll_mailing_lists_
{
public:
	//o The head section.
	gnumll_head_section_ Head;
	//o the general sections.
	gnumll_general_sections_ Generals;
	//o The list sections.
	gnumll_list_sections_ Lists;
	struct s {
		gnumll_head_section_::s Head;
		gnumll_general_sections_::s Generals;
		gnumll_list_sections_::s Lists;
	};
	gnumll_mailing_lists_( s &S )
	: Head( S.Head ),
	  Generals( S.Generals ),
	  Lists( S.Lists )
	{}
	void reset( bool P = true )
	{
		Head.reset( P );
		Generals.reset( P );
		Lists.reset( P );
	}
	void plug( mmm_multimemory_ &M )
	{
		Head.plug( M );
		Generals.plug( M );
		Lists.plug( M );
	}
	gnumll_mailing_lists_ &operator =( const gnumll_mailing_lists_ &M )
	{
		Head = M.Head;
		Generals = M.Generals;
		Lists = M.Lists;

		return *this;
	}
	//f Initialization.
	void Init( void )
	{
		Head.Init();
		Generals.Init();
		Lists.Init();
	}
	//f Parse the flow 'Flow'.
	void Parse( xtf::extended_text_iflow___ &Flow );
};

AUTO( gnumll_mailing_lists )

inline txf::text_oflow___ &operator <<(
	txf::text_oflow___ &F,
	const gnumll_mailing_lists_ &L )
{
	F << "----- HEAD -----" << txf::nl << L.Head << txf::nl;
	F << "----- GENERAL -----" << txf::nl << L.Generals << txf::nl;
	F << "----- LISTS -----" << txf::nl << L.Lists << txf::nl;
	return F;
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
