/*
  'gendoc' by Claude L. Simon (epeios@epeios.org).
  Generates library documentation by parsing header files.
  Copyright (C) 2000 Claude Simon.

  This file is part of the Epeios project (http://www.epeios.org/).

  This program is free software; you can redistribute it and/or
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

// $Id$



#include "tsrcpr.h"
#include "fil.h"
#include "fnm.h"



#define NOM_FICHIER_INDEX		"index.html"
#define TERM_FICHIER_DOC		".html"

#ifdef CPE__UNIX
#define SS	"/"
#define SC  '/'
#else
#define SS "\\"
#define SC  '\\'
#endif

using namespace tsrcpr;
using namespace txf;

void GenererEnTeteFichierHTML( txf::text_oflow___ &Stream )
{
	Stream <<  "<!doctype HTML public \"-//W3O//DTD W3 HTML 3.0//EN\"><HTML>";
}

template <class t> void Classer( t &Table )
{
ERRProlog
	bool Swap = true;
	str::string Name;
ERRBegin
	while ( Swap )
	{
		tym::row__ Courant = Table.First();
		Swap = false;

		while ( Courant != Table.Last() )
		{
			tym::row__ S = Table.Next( Courant );
			Name.Init();
			Name = Table( S ).Name;

			if ( Table( Courant ).Name > Name )
			{
				Swap = true;

				Table.Sync();

				Table.Swap( Courant, S );

			}
			else
				Courant = Table.Next( Courant );
		}
	}

	Table.Sync();
ERRErr
ERREnd
ERREpilog
}


class librairie_
{
public:
	// Library analysée de la librairie.
	library_ Library;
	// Name de la librairie.
	str::string_ Name;
	struct s
	{
		library_::s Library;
		str::string_::s Name;
	};
	librairie_( s &S )
	: Library( S.Library ),
	  Name( S.Name )
	{}
	void reset( bool P = true )
	{
		Library.reset( P );
		Name.reset( P );
	}
	void plug( mmm::multimemory_ &M )
	{
		Library.plug( M );
		Name.plug( M );
	}
	librairie_ &operator =( const librairie_ &L )
	{
		Library = L.Library;
		Name = L.Name;

		return *this;
	}
	void Init( void )
	{
		Library.Init();
		Name.Init();
	}
};

typedef ctn::E_XCONTAINER( librairie_ ) librairies;
typedef ctn::E_XCONTAINER_( librairie_ ) librairies_;

/*
class librairies
: public LIST,
  public CONTAINER( librairie_ )
{
protected:
	void LSTAllocate( SIZE__ Size )
	{
		CONTAINER( librairie_ )::Allocate( Size );
	}
public:
	void Init( void )
	{
		LIST::Init();
		CONTAINER( librairie_ )::Init();
	}
	tym::row__ Nouveau( void )
	{
		tym::row__ P =  LIST::CreateEntry();
		
		CONTAINER( librairie_ )::operator()( P ) .Init();

		CONTAINER( librairie_ )::Sync();

		return P;

	}
	FNLP( LIST:: )
};
*/
// Procède à la mise en forme de commentaire.
str::string_ &MiseEnFormeCommentaire( const str::string_ &C )
{
	static str::string S;
ERRProlog
	bool Espace = true, Special = false;
ERRBegin
	S.Init();

	for ( tym::row__ Compteur = 0; Compteur < C.Amount(); Compteur++ )
	{
		if ( C.Read( Compteur ) == ' ' )
		{
			Espace = true;
			S.Add( ' ' );
		}
		else if ( C.Read( Compteur ) == '<' )
			S.Add( "&lt;" );
		else if ( C.Read( Compteur ) == '>' )
			S.Add( "&gt;" );
		else
		{
			if ( C.Read( Compteur ) == '\'' )
			{
				if ( Special )
				{
					S.Add( "</TT></B><EM>" );
					Special = false;
				}
				else if ( Espace )
				{
					Special = true;
					S.Add( "</EM><B><TT>" );
				}
				else
					S.Add( '\'' );
			}
			else S.Add( C.Read( Compteur ) );

			Espace = false;
		}
	}

ERRErr
ERREnd
ERREpilog
	return S;
}

/* fournit une référence sur un 'str::string_' dont les caractères '<' et '>' ont
été remplacé par les symboles adéquats pour qu'un navigateur Internet ne les
prennent pas pour des délimiteurs de tags */
const str::string_ &Filtrer( const str::string_ &S )
{
	static str::string R;

	R.Init();

	for ( tym::row__ i = 0; i < S.Amount(); i++ )
		if ( S.Read( i ) == '<' )
			R.Add( "&lt;" );
		else if ( S.Read( i ) == '>' )
			R.Add( "&gt;" );
		else
			R.Add( S.Read( i ) );

	return R;
}

/********************/
/**** NIVEAU XII ****/
/********************/

inline void GenererDocumentationParametre(
	const parametre_ &Parametre,
	txf::text_oflow___ &Flot )
{
	Flot << "<LI><TT><EM>" << Filtrer( Parametre.Type ) << " <B>" << Parametre.Name << "</B></EM>";

	if ( Parametre.Valeur.Amount() )
		Flot << " = " << Parametre.Valeur;

	Flot << "</TT>" << nl;
}



/*******************/
/**** NIVEAU XI ****/
/*******************/

void GenererDocumentationParametres(
	const table_<parametre_> &Parametres,
	txf::text_oflow___ &Flot )
{
ERRProlog
	ctn::E_CITEM( parametre_ ) Parametre;
ERRBegin
	Flot << "<UL>";

	Parametre.Init( Parametres );

	if ( Parametres.Amount() )
	{
		tym::row__ Courant = Parametres.First();

		while ( Courant != NONE )
		{
			GenererDocumentationParametre( Parametre( Courant ), Flot );
			Courant = Parametres.Next( Courant );
		}
	}

	Flot << "</UL>" << nl;
ERRErr
ERREnd
ERREpilog
}



/******************/
/**** NIVEAU X ****/
/******************/

template <class t> inline void GenererIndexItemClasse(
	const str::string_ &Name,
	const t &Item,
	txf::text_oflow___ &Flot )
{
	Flot << "<A HREF = \"#" << Name << '.' << Item.Name << "\">" << Item.Name << "</A> " << nl;
}

inline void GenererCorpsItemClasse(
	const str::string_ &Name,
	const objet_ &Item,
	txf::text_oflow___ &Flot )
{
	Flot << "<LI><TT><B>" << "<A NAME = \"" << Name << '.' << Item.Name << "\">" << Item.Name << "</B> "<< Filtrer( Item.Type ) << ' ' << "</TT><BR><EM>" << MiseEnFormeCommentaire( Item.Commentaire ) << "</EM>" << nl;
}

inline void GenererCorpsItemClasse(
	const str::string_ &Name,
	const methode_ &Item,
	txf::text_oflow___ &Flot )
{
	Flot << "<LI><A NAME = \"" << Name << "." << Item.Name << "\">" << "<TT><B>" << Item.Name << "</B> : " << Item.Type << "</TT>" << nl;

	GenererDocumentationParametres( Item.Parametres, Flot );

	Flot << "<EM>" << MiseEnFormeCommentaire( Item.Commentaire ) << "</EM>" << nl;
}

inline void GenererCorpsItemClasse(
	const str::string_ &Name,
	const function_ &Item,
	txf::text_oflow___ &Flot )
{
	Flot << "<LI><A NAME = \"" << Name << "." << Item.Name << "\">" << "<TT><B>" << Item.Name << "</B> : " << Item.Type << "</TT>" << nl;

	GenererDocumentationParametres( Item.Parametres, Flot );

	Flot << "<EM>" << MiseEnFormeCommentaire( Item.Commentaire ) << "</EM>" << nl;
}


/*******************/
/**** NIVEAU IX ****/
/*******************/

inline void GenererDocumentationArgumentTemplateClasse(
	const argument_ &Argument,
	txf::text_oflow___ &Flot )
{
	Flot << Argument.Type << " <TT>" << Argument.Name << "</TT>" << nl;
}

inline void GenererDocumentationArgumentTemplateFonction(
	const argument_ &Argument,
	txf::text_oflow___ &Flot )
{
	Flot << Argument.Type << " <B>" << Argument.Name << "</B>" << nl;
}


template <class t> void GenererIndexItemsClasse(
	const str::string_ &NameClasse,
	t &Items,
	txf::text_oflow___ &Flot )
{
ERRProlog
	tym::row__ Courant = Items.First();
	str::string Name;
ERRBegin

	Flot << "<EM>";

	GenererIndexItemClasse( NameClasse, Items( Courant ), Flot );
	Name.Init();
	Name = Items( Courant ).Name;

	while ( Courant != NONE )
	{
		if ( Name != Items( Courant ).Name )
		{
			GenererIndexItemClasse( NameClasse, Items( Courant ), Flot );
			Name = Items( Courant ).Name;
		}

		Courant = Items.Next( Courant );
	}

	Items.Sync();

	Flot << "</EM><BR>" << nl;
ERRErr
ERREnd
ERREpilog
}

template <class t> void GenererCorpsItemsClasse(
	const str::string_ &Name,
	t &Items,
	txf::text_oflow___ &Flot )
{
	tym::row__ Courant = Items.First();

	Flot << "<UL>";

	while ( Courant != NONE )
	{
		GenererCorpsItemClasse( Name, Items( Courant ), Flot );
		Courant = Items.Next( Courant );
	}

	Items.Sync();

	Flot << "</UL>" << nl;

}


/*********************/
/**** NIVEAU VIII ****/
/*********************/

void GenererDocumentationArgumentsTemplateClasse(
	const table_<argument_> &Arguments,
	txf::text_oflow___ &Flot )
{
	ctn::E_CITEM( argument_ ) Argument;
	tym::row__ Courant = Arguments.First();

	Argument.Init( Arguments );

	GenererDocumentationArgumentTemplateClasse( Argument( Courant ), Flot );
	Courant = Arguments.Next( Courant );

	while ( Courant != NONE )
	{
		Flot << ", ";
		GenererDocumentationArgumentTemplateClasse( Argument( Courant ), Flot );
		Courant = Arguments.Next( Courant );
	}
}

inline void GenererDocumentationBaseClasse(
	const str::string_ &Base,
	txf::text_oflow___ &Flot )
{
	Flot << "<LI>" << Filtrer( Base ) << "</LI>" << nl;
}

void GenererDocumentationArgumentsTemplateFonction(
	const table_<argument_> &Arguments,
	txf::text_oflow___ &Flot )
{
	ctn::E_CITEM( argument_ ) Argument;
	tym::row__ Courant = Arguments.First();

	Argument.Init( Arguments );

	while ( Courant != NONE )
	{
		if ( Courant != Arguments.First() )
			Flot << ", ";

		GenererDocumentationArgumentTemplateFonction( Argument( Courant ), Flot );
		Courant = Arguments.Next( Courant );
	}
}

template <class t> void GenererDocumentationItemsClasse(
	const str::string_ &Name,
	t &Items,
	txf::text_oflow___ &Flot )
{
	if ( Items.Amount() )
	{
		Classer( Items );
		GenererIndexItemsClasse( Name, Items, Flot );
		GenererCorpsItemsClasse( Name, Items, Flot );
	}
}


/********************/
/**** NIVEAU VII ****/
/********************/

void GenererDocumentationItems(
	const table_<item_> &Items,
	txf::text_oflow___ &Flot )
{
	ctn::E_CITEM( item_ ) Item;
	tym::row__ Courant = Items.First();

	Item.Init( Items );

	while ( Courant != NONE )
	{
		Flot << "<LI><B>" << Item( Courant ).Name << "</B>: " << Item( Courant ).Commentary << "</LI>" << nl;
		Courant = Items.Next( Courant );
	}
}



void GenererDocumentationArguments(
	const table_<str::string_> &Arguments,
	txf::text_oflow___ &Flot )
{
	ctn::E_CITEM( str::string_ ) Argument;
	tym::row__ Courant = Arguments.First();

	Argument.Init( Arguments );

	while ( Courant != NONE )
	{
		if ( Courant != Arguments.First() )
			Flot << ", ";

		Flot << "<B>" << Argument( Courant ) << "</B>" << nl;
		Courant = Arguments.Next( Courant );
	}
}


inline void GenererIndexClasse(
	const classe_ &Classe,
	txf::text_oflow___ &Flot )
{
	Flot << "<A HREF = \"#" << Classe.Name << "\">" << Classe.Name << "</A> " << nl;
}

void GenererDocumentationTemplateClasse(
	const template_ &Template,
	txf::text_oflow___ &Flot )
{
	if ( Template.Arguments.Amount() )
	{
		Flot << "<EM>( ";

		GenererDocumentationArgumentsTemplateClasse( Template.Arguments, Flot );

		Flot << " )</EM>";
	}
}

void GenererDocumentationBasesClasse(
	const table_<str::string_> &Bases,
	txf::text_oflow___ &Flot )
{
	ctn::E_CITEM( str::string_ ) Base;
	tym::row__ Courant = Bases.First();

	Base.Init( Bases );

	Flot << "<UL>";

	GenererDocumentationBaseClasse( Base( Courant ), Flot );
	Courant = Bases.Next( Courant );

	while ( Courant != NONE ) {
		GenererDocumentationBaseClasse( Base( Courant ), Flot );
		Courant = Bases.Next( Courant );
	}

	Flot << "</UL>";
}

void GenererDocumentationTemplateFonction(
	const template_ &Template,
	txf::text_oflow___ &Flot )
{
	if ( Template.Arguments.Amount() )
	{
		Flot << "<TT>" << Filtrer( str::string( "<" ) );

		GenererDocumentationArgumentsTemplateFonction( Template.Arguments, Flot );

		Flot << Filtrer( str::string( "> " ) ) << "</TT> " << nl;
	}
}

void GenererIndexDocumentationClasse(
	const classe_ &Classe,
	txf::text_oflow___ &Flot )
{

	if ( Classe.Objets.Amount() )
		Flot << "<A HREF = \"#" << Classe.Name << ".Objects\">Objects</A> ";

	if ( Classe.Restreints.Amount() )
		Flot << "<A HREF = \"#" << Classe.Name << ".Restricts\">Restricts</A> ";

	if ( Classe.Methodes.Amount() )
		Flot << "<A HREF = \"#" << Classe.Name << ".Functions\">Functions</A> ";

	if ( Classe.Virtuels.Amount() )
		Flot << "<A HREF = \"#" << Classe.Name << ".Virtuals\">Virtuals</A> ";
}

void GenererDocumentationObjetsClasse(
	classe_ &Classe,
	txf::text_oflow___ &Flot )
{

	if ( Classe.Objets.Amount() )
	{
		Flot << "<A NAME = \"" << Classe.Name << ".Objects\"><H4>Objects</H4>";
		GenererDocumentationItemsClasse( Classe.Name, Classe.Objets, Flot );
	}

	if ( Classe.Restreints.Amount() )
	{
		Flot << "<A NAME = \"" << Classe.Name << ".Restricts\"><H4>Restricts</H4>";
		GenererDocumentationItemsClasse( Classe.Name, Classe.Restreints, Flot );
	}
}

void GenererDocumentationMethodesClasse(
	classe_ &Classe,
	txf::text_oflow___ &Flot )
{
	if ( Classe.Methodes.Amount() )
	{
		Flot << "<A NAME = \"" << Classe.Name << ".Functions\"><H4>Functions</H4>";
		GenererDocumentationItemsClasse( Classe.Name, Classe.Methodes, Flot );
	}

	if ( Classe.Virtuels.Amount() )
	{
		Flot << "<A NAME = \"" << Classe.Name << ".Virtuals\"><H4>Virtuals</H4>";
		GenererDocumentationItemsClasse( Classe.Name, Classe.Virtuels, Flot );
	}
}


/*******************/
/**** NIVEAU VI ****/
/*******************/


void GenererDocumentationDefine(
	const define_ &Define,
	txf::text_oflow___ &Flot )
{
	Flot << "<LI><TT>" << Define.Name ;

	if ( Define.Arguments.Amount() ) {
		Flot << "( ";
		GenererDocumentationArguments( Define.Arguments, Flot );
		Flot << " )";
	}

	Flot << ": </TT><EM>" << MiseEnFormeCommentaire( Define.Commentaire ) << "</EM>" << nl;
}

void GenererDocumentationEnum(
	const enum_ &Enum,
	txf::text_oflow___ &Flot )
{
	Flot << "<LI><TT>" << Enum.Name ;

	Flot << ": </TT><EM>" << MiseEnFormeCommentaire( Enum.Commentary ) << "</EM>" << nl;

	if ( Enum.Items.Amount() ) {
		Flot << "<UL>";
		GenererDocumentationItems( Enum.Items, Flot );
		Flot << "</UL>";
	}

}

void GenererDocumentationShortcut(
	const shortcut_ &Shortcut,
	txf::text_oflow___ &Flot )
{
	Flot << "<LI><TT>" << Shortcut.Name << "( ";

	if ( Shortcut.Arguments.Amount() )
		GenererDocumentationArguments( Shortcut.Arguments, Flot );

	Flot << " ): </TT><EM>" << MiseEnFormeCommentaire( Shortcut.Commentaire ) << "</EM>" << nl;
}

void GenererDocumentationTypedef(
	const typedef_ &Typedef,
	txf::text_oflow___ &Flot )
{
	Flot << "<LI><TT>" << Typedef.Name << ": </TT><EM>" << MiseEnFormeCommentaire( Typedef.Commentaire ) << "</EM>";
}

void GenererIndexClasses(
	const table_<classe_> &Classes,
	txf::text_oflow___ &Flot )
{
	ctn::E_CITEM( classe_ ) Classe;
	tym::row__ Courant = Classes.First();

	Classe.Init( Classes );

	Flot << "<TT>";

	while ( Courant != NONE )
	{
		GenererIndexClasse( Classe( Courant ), Flot );
		Courant = Classes.Next( Courant );
	}

	Flot << "</TT><BR>" << nl;
}

inline void GenererDocumentationClasse(
	classe_ &Classe,
	txf::text_oflow___ &Flot )
{

	Flot << "<HR><H3><A NAME = \"" << Classe.Name << "\">" << Classe.Name;

	GenererDocumentationTemplateClasse( Classe.Template, Flot );

	Flot << "</H3>" << nl;

	if ( Classe.Bases.Amount() )
		GenererDocumentationBasesClasse( Classe.Bases, Flot );

	Flot << "<EM>" << MiseEnFormeCommentaire( Classe.Commentaire ) << "</EM><BR>" << nl;

	GenererIndexDocumentationClasse( Classe, Flot );
	GenererDocumentationObjetsClasse( Classe, Flot );
	GenererDocumentationMethodesClasse( Classe, Flot );
}

/*
void GenererDocumentationFonctions(
	ASD_TABLE_(fonction_) &Fonctions,
	txf::text_oflow___ &Flot )
{
	Flot << "<HR><H3><A NAME = \"Fonctions\">Fonctions</H3>";
	GenererIndexDocumentationFonctions( Classe.Fonctions, Flot );
	GenererCorpsDocumentationFonctions( Classe.Fonctions, Flot );
}
*/

inline void GenererDocumentationFonction(
	const function_ &Fonction,
	txf::text_oflow___ &Flot )
{
	Flot << "<LI><A NAME = \"" << Fonction.Name << "\">";

	GenererDocumentationTemplateFonction( Fonction.Template, Flot );

	Flot << "<TT><B>" << Fonction.Name << "</B> : " << Fonction.Type << "</TT>";

	GenererDocumentationParametres( Fonction.Parametres, Flot );

	Flot << "<EM>" << MiseEnFormeCommentaire( Fonction.Commentaire ) << "</EM>";
}

template <class t> void GenererItems(
	const table_<t> &Items,
	txf::text_oflow___ &Flot )
{
ERRProlog
	ctn::E_CITEM( t ) Item;
	tym::row__ Courant = Items.First();
ERRBegin
	Item.Init( Items );

	while ( Courant != NONE )
	{
		Flot << "<LI>" << Item( Courant );
		Courant = Items.Next( Courant );
	}
ERRErr
ERREnd
ERREpilog
}



/******************/
/**** NIVEAU V ****/
/******************/

void GenererDocumentationDefines(
	const table_<define_> &Defines,
	txf::text_oflow___ &Flot )
{
	ctn::E_CITEM( define_ ) Define;
	tym::row__ Courant = Defines.First();

	Define.Init( Defines );

	Flot << "<HR><H2><A NAME=\"Defines\">Defines</H2><UL>";

	while ( Courant != NONE )
	{
		GenererDocumentationDefine( Define( Courant ), Flot );
		Courant = Defines.Next( Courant );
	}

	Flot << "</UL>";
}

void GenererDocumentationEnums(
	const table_<enum_> &Enums,
	txf::text_oflow___ &Flot )
{
	ctn::E_CITEM( enum_ ) Enum;
	tym::row__ Courant = Enums.First();

	Enum.Init( Enums );

	Flot << "<HR><H2><A NAME=\"Enums\">Enums</H2><UL>";

	while ( Courant != NONE )
	{
		GenererDocumentationEnum( Enum( Courant ), Flot );
		Courant = Enums.Next( Courant );
	}

	Flot << "</UL>";
}

void GenererDocumentationShortcuts(
	const table_<shortcut_> &Shortcuts,
	txf::text_oflow___ &Flot )
{
	ctn::E_CITEM( shortcut_ ) Shortcut;
	tym::row__ Courant = Shortcuts.First();

	Shortcut.Init( Shortcuts );

	Flot << "<HR><H2><A NAME=\"Shortcuts\">Shortcuts</H2><UL>";

	while ( Courant != NONE )
	{
		GenererDocumentationShortcut( Shortcut( Courant ), Flot );
		Courant = Shortcuts.Next( Courant );
	}

	Flot << "</UL>";
}

void GenererDocumentationTypedefs(
	const table_<typedef_> &Typedefs,
	txf::text_oflow___ &Flot )
{
	ctn::E_CITEM( typedef_ ) Typedef;
	tym::row__ Courant = Typedefs.First();

	Typedef.Init( Typedefs );

	Flot << "<HR><H2><A NAME=\"Typedefs\">Typedefs</H2><UL>";

	while ( Courant != NONE )
	{
		GenererDocumentationTypedef( Typedef( Courant ), Flot );
		Courant = Typedefs.Next( Courant );
	}

	Flot << "</UL>";
}


void GenererDocumentationClasses(
	table_<classe_> &Classes,
	txf::text_oflow___ &Flot )
{
	tym::row__ Courant = Classes.First();

	Flot << "<HR><H2><A NAME=\"Classes\">Classes</H2>";

	GenererIndexClasses( Classes, Flot );

	while ( Courant != NONE )
	{
		GenererDocumentationClasse( Classes( Courant ), Flot );
		Courant = Classes.Next( Courant );
	}

	Classes.Sync();
}

void GenererDocumentationFonctions(
	table_<function_> &Fonctions,
	txf::text_oflow___ &Flot )
{
	ctn::E_CITEM( function_ ) Fonction;
	tym::row__ Courant = Fonctions.First();

	Fonction.Init( Fonctions );

	Flot << "<HR><H2><A NAME=\"Functions\">Functions</H2>";
	GenererDocumentationItemsClasse( str::string( "" ), Fonctions, Flot );

/*	Flot << "<HR><H2><A NAME=\"Functions\">Functions</H2><UL>";

	while ( Courant != NONE )
	{
		GenererDocumentationFonction( Fonction( Courant ), Flot );
		Courant = Fonctions.Next( Courant );
	}
*/

	Flot << "</UL>";
}

void GenererDocumentationObjets(
	table_<objet_> &Objets,
	txf::text_oflow___ &Flot )
{
	ctn::E_CITEM( objet_ ) Objet;
	tym::row__ Courant = Objets.First();

	Objet.Init( Objets );

	Flot << "<HR><H2><A NAME=\"Objects\">Objects</H2>";
	GenererDocumentationItemsClasse( str::string( "" ), Objets, Flot );

/*	Flot << "<HR><H2><A NAME=\"Functions\">Functions</H2><UL>";

	while ( Courant != NONE )
	{
		GenererDocumentationFonction( Fonction( Courant ), Flot );
		Courant = Fonctions.Next( Courant );
	}
*/

	Flot << "</UL>";
}



void GenererGeneralites(
	const str::string_ &Generalites,
	txf::text_oflow___ &Flot )
{
	Flot << "<HR><H4><A NAME = \"Généralités\">Généralités:</H4><P>" << Generalites;
}

void GenererVersions(
	const table_<str::string_> &Versions,
	txf::text_oflow___ &Flot )
{
	Flot << "<HR><H4><A NAME = \"Versions\">Versions:</H4><P><UL>";

	if ( Versions.Amount() )
		GenererItems( Versions, Flot );

	Flot << "</UL>";
}

void GenererHistorique(
	const table_<str::string_> &Historique,
	txf::text_oflow___ &Flot )
{
	Flot << "<HR><H4><A NAME = \"Historique\">Historique:</H4><P><UL>";

	if ( Historique.Amount() )
		GenererItems( Historique, Flot );

	Flot << "</UL>";
}

void GenererRemarques(
	const table_<str::string_> &Remarques,
	txf::text_oflow___ &Flot )
{
	Flot << "<HR><H4><A NAME = \"Remarques\">Remarques:</H4><P><UL>";

	if ( Remarques.Amount() )
		GenererItems( Remarques, Flot );

	Flot << "</UL>";
}


/********************/
/***** NIVEAU IV ****/
/********************/

/* Ouvre avec 'Fichier' le fichier de documentation de nom 'Name' (sans suffixe)
situé dans le répertore 'Repertoire' */
void OuvrirFichierDocumentation(
	ifstream &Fichier,
	const str::string_ &Name,
	const char *Repertoire )
{
ERRProlog
	char *RelaisNameFichier = NULL;
	str::string NameFichier;
ERRBegin
	NameFichier.Init();

	NameFichier.Write( Repertoire );

	if ( !strchr( SS ":", NameFichier( NameFichier.Amount() - 1 ) ) )
		NameFichier.Add( SC );

	NameFichier.Add( Name );

	NameFichier.Add( ".d" );

	NameFichier.Add( '\0' );

	RelaisNameFichier = NameFichier.Convert();

	Fichier.open( RelaisNameFichier );

	if ( !Fichier )
	{
		fout << "Impossible d'ouvrir le fichier '" << NameFichier << "'." << nl;
		ERRt();
	}
ERRErr
ERREnd
	if ( RelaisNameFichier )
		free( RelaisNameFichier );
ERREpilog
}


/* Ouvre avec 'Streal' le fichier library de nom 'Name' (sans suffixe)
situé dans le répertore 'Repertoire' */
void OuvrirFichierLibrary(
	fil::file_iflow___ &Stream,
	const str::string_ &Name,
	const char *Repertoire )
{
ERRProlog
	char *RelaisNameFichier = NULL;
	str::string NameFichier;
ERRBegin
	NameFichier.Init();

	NameFichier.Write( Repertoire );

	if ( !strchr( SS ":", NameFichier( NameFichier.Amount() - 1 ) ) )
		NameFichier.Add( SC );

	NameFichier.Add( Name );

/*	NameFichier.Add( ".h" );

	NameFichier.Add( '\0' );
*/
	RelaisNameFichier = NameFichier.Convert();

	if ( Stream.Init( RelaisNameFichier, err::hSkip ) != fil::sSuccess )	{
		fout << "Impossible d'ouvrir le fichier '" << NameFichier << "'." << nl;
		ERRt();
	}
ERRErr
ERREnd
	if ( RelaisNameFichier )
		free( RelaisNameFichier );
ERREpilog
}

/* Ouvre dans 'Fichier' le fichier de documentation technique de la librairie
'Librairie' dans le répertoire 'Repertoire' */
void OuvrirFichierDocumentationTechnique(
	const librairie_ &Librairie,
	fil::file_oflow___ &Fichier,
	const char *Repertoire )
{
ERRProlog
	 str::string NameFichier;
	 char *RelaisNameFichier = NULL;
ERRBegin
	NameFichier.Init();

	NameFichier.Write( Repertoire );

	if ( !strchr( SS ":", NameFichier( NameFichier.Amount() - 1 ) ) )
		NameFichier.Add( SC );

	NameFichier.Add( Librairie.Name );
	NameFichier.Add( TERM_FICHIER_DOC );

	NameFichier.Add( '\0' );

	RelaisNameFichier = NameFichier.Convert();

	if ( Fichier.Init( RelaisNameFichier, err::hSkip ) != fil::sSuccess )
	{
		fout << "Impossible d'ouvrir le fichier '" << RelaisNameFichier << "'." << nl;
		ERRt();
	}
ERRErr
ERREnd
	if ( RelaisNameFichier )
		free( RelaisNameFichier );
ERREpilog
}

void GenererTitreDocumentationTechnique(
	const librairie_ &Librairie,
	txf::text_oflow___ &Flot )
{
	Flot << "<HEAD><TITLE>" << Librairie.Name << " library documentation" << "</TITLE></HEAD><BODY>" << nl;
	Flot << "<CENTER><A HREF=\"" << NOM_FICHIER_INDEX << "\">Back to Epeios libraries index</A></CENTER>" << nl;
	Flot << "<H2><CENTER>Documentation of the <TT>" << Librairie.Name << "</TT> library.</H2></CENTER>";
	Flot << "<H3><CENTER>" << Librairie.Library.Description << "</H3></CENTER>";
	Flot << "<H4><UL>";
	Flot << "<LI><EM>Version :</EM> " << Librairie.Library.Version << "</LI>";
	Flot << "<LI><EM>Release :</EM> " << Librairie.Library.Release << "</LI>";
	Flot << "<LI><EM>Coordinator :</EM> " << Librairie.Library.Coordinators << "</LI>";
	Flot << "</UL></H4>";
}


// Génère l'index de la documentation technique dcorrespondat au library 'Library'.
void GenererIndexDocumentationTechnique(
	const library_ &Library,
	txf::text_oflow___ &Flot )
{
	if ( Library.Defines.Amount() )
		Flot << "<A HREF = \"#Defines\">Defines</A> ";

	if ( Library.Enums.Amount() )
		Flot << "<A HREF = \"#Enums\">Enums</A> ";

	if ( Library.Shortcuts.Amount() )
		Flot << "<A HREF = \"#Shortcuts\">Shortcuts</A> ";

	if ( Library.Typedefs.Amount() )
		Flot << "<A HREF = \"#Typedefs\">Typedefs</A> ";

	if ( Library.Classes.Amount() )
		Flot << "<A HREF = \"#Classes\">Classes</A> ";

	if ( Library.Functions.Amount() )
		Flot << "<A HREF = \"#Functions\">Functions</A> ";

	if ( Library.Objets.Amount() )
		Flot << "<A HREF = \"#Objects\">Objects</A> ";
}

void GenererCorpsDocumentationTechnique(
	library_ &Library,
	txf::text_oflow___ &Flot )
{
	if ( Library.Defines.Amount() )
	{
		Classer( Library.Defines );
		GenererDocumentationDefines( Library.Defines, Flot );
	}

	if ( Library.Enums.Amount() )
	{
		Classer( Library.Enums );
		GenererDocumentationEnums( Library.Enums, Flot );
	}

	if ( Library.Shortcuts.Amount() )
	{
		Classer( Library.Shortcuts );
		GenererDocumentationShortcuts( Library.Shortcuts, Flot );
	}

	if ( Library.Typedefs.Amount() )
	{
		Classer( Library.Typedefs );
		GenererDocumentationTypedefs( Library.Typedefs, Flot );
	}

	if ( Library.Classes.Amount() )
	{
		Classer( Library.Classes );
		GenererDocumentationClasses( Library.Classes, Flot );
	}

	if ( Library.Functions.Amount() )
	{
		Classer( Library.Functions );
		GenererDocumentationFonctions( Library.Functions, Flot );
	}

	if ( Library.Objets.Amount() )
	{
		Classer( Library.Objets );
		GenererDocumentationObjets( Library.Objets, Flot );
	}
}

/* Ouvre dans 'Fichier' le fichier de documentation technique de la librairie
'Librairie' dans le répertoire 'Repertoire' */
/*
void OuvrirFichierDocumentationDescriptive(
	const librairie_ &Librairie,
	fil__file_oflow_ &Fichier,
	const char *Repertoire )
{
ERRProlog
	 str::string NameFichier;
	 char *RelaisNameFichier = NULL;
ERRBegin
	NameFichier.Init();

	NameFichier.Write( Repertoire );

	if ( !strchr( SS ":", NameFichier( NameFichier.Amunt() - 1 ) ) )
		NameFichier.Add( SC );

	NameFichier.Add( Librairie.Name );
	NameFichier.Add( TERM_FICHIER_DOC_DESC );

	NameFichier.Add( '\0' );

	RelaisNameFichier = NameFichier.Convert();

	Fichier.Init( RelaisNameFichier );

	if ( !Fichier )
	{
		fout << "Impossible d'ouvrir le fichier '" << RelaisNameFichier << "'." << nl;
		ERRt();
	}
ERRErr
ERREnd
	if ( RelaisNameFichier )
		free( RelaisNameFichier );
ERREpilog
}
*/

/*
void GenererTitreDocumentationDescriptive(
	const librairie_ &Librairie,
	txf::text_oflow___ &Flot )
{
	Flot << "<H1><CENTER>Documentation descriptive de la librairie " << Librairie.Name << " (" << Librairie.Library.Description << ").</H1></CENTER>";
	Flot << "<H2><CENTER>" << Librairie.Library.Version << ' ' << Librairie.Library.Coordinators << ' ' << Librairie.Library.Release <<" </H2></CENTER>";
//	Flot << "<H2><CENTER>" << Librairie.Documentation.Description << "</CENTER></H2>";
}
*/
// Génère l'index de la documentation descriptive dcorrespondant à la documentation 'Documentation'
/*void GenererIndexDocumentationDescriptive(
	add_documentation_ &Documentation,
	txf::text_oflow___ &Flot )
{
	Flot << "<A HREF = \"#Versions\">Versions</A> ";

	Flot << "<A HREF = \"#Historique\">Historique</A> ";

	Flot << "<A HREF = \"#Généralités\">Généralités</A> ";

	if ( !Documentation.Remarques.IsEmpty() )
		Flot << "<A HREF = \"#Remarques\">Remarques</A> ";
}
*/
/*
void GenererCorpsDocumentationDescriptive(
	const add_documentation_ &Documentation,
	txf::text_oflow___ &Flot )
{
	GenererGeneralites( Documentation.Generalites, Flot );
	GenererVersions( Documentation.Versions, Flot );
	GenererHistorique( Documentation.Historique, Flot );
	GenererRemarques( Documentation.Remarques, Flot );
}
*/

/*********************/
/***** NIVEAU III ****/
/*********************/

/* Ouvre 'Fichier' avec le fichier prédéfini contenant la liste des librairies
et placé dans le répertoire de nom 'Repertoire. */
void OuvrirFichierListe(
	const char *NameFichier,
	ifstream &Fichier )
{
	Fichier.open( NameFichier );

	if ( !Fichier )
	{
		fout << "Impossible d'ouvrir le fichier '" << NameFichier << "'." << nl;
		ERRt();
	}
}


/* Place dans 'Documentation' l'analyse du fichier de documentation de nom 'Name'
(non suffixé) situé dans 'Repertoire'. */
/*
void AnalyserDocumentation(
	add_documentation_ &Documentation,
	const str::string_ &Name,
	const char *Repertoire )
{
ERRProlog
	ifstream Stream;
	flo_entree_texte_fichier Fichier;
	add_documentation D;
ERRBegin
	D.Init();

	OuvrirFichierDocumentation( Stream, Name, Repertoire );
	Fichier.Init( Stream );
	D.Analyser( Fichier );

	Documentation = D;
ERRErr
	fout << nl << ">>>>> ERREUR <<<<< " << Fichier.Ligne() << tab << Fichier.Colonne() << nl;
ERREnd
ERREpilog
}
*/

/* Place dans 'Library' l'analyse du fichier library de nom 'Name'
(non suffixé) situé dans 'Repertoire'. */
void AnalyserLibrary(
	library_ &Library,
	const str::string_ &Name,
	const char *Repertoire )
{
ERRProlog
	fil::file_iflow___ Fichier;
	xtf::extended_text_iflow___ Entree;
	library S;
ERRBegin
	S.Init();

	OuvrirFichierLibrary( Fichier, Name, Repertoire );

	Fichier.EOFT( XTF_EOXT );

	Entree.Init( Fichier );
	S.Analyze( Entree );

	Library = S;
ERRErr
	fout << nl << ">>>>> ERREUR <<<<< " << Entree.Line() << tab << Entree.Column() << nl;
ERREnd
ERREpilog
}

void GenererDocumentationTechnique(
	librairie_ &Librairie,
	const char *Repertoire )
{
ERRProlog
	fil::file_oflow___ Fichier;
	txf::text_oflow___ Sortie;
ERRBegin
	OuvrirFichierDocumentationTechnique( Librairie, Fichier, Repertoire );
	Sortie.Init( Fichier );
	GenererEnTeteFichierHTML( Sortie );
	GenererTitreDocumentationTechnique( Librairie, Sortie );
	GenererIndexDocumentationTechnique( Librairie.Library, Sortie );
	GenererCorpsDocumentationTechnique( Librairie.Library, Sortie );
	Sortie << "<hr><p align=\"right\"><small>Generated " << TOLDateAndTime() << " by GenDoc (c) Claude SIMON</small></p></BODY>";
ERRErr
ERREnd
ERREpilog
}
/*
void GenererDocumentationDescriptive(
	const librairie_ &Librairie,
	const char *Repertoire )
{
ERRProlog
	fil__file_oflow Flot;
ERRBegin
	OuvrirFichierDocumentationDescriptive( Librairie, Flot, Repertoire );
	GenererEnTeteFichierHTML( Flot );
	GenererTitreDocumentationDescriptive( Librairie, Flot );
	GenererIndexDocumentationDescriptive( Librairie.Documentation, Flot );
	GenererCorpsDocumentationDescriptive( Librairie.Documentation, Flot );
ERRErr
ERREnd
ERREpilog
}
*/

/********************/
/***** NIVEAU II ****/
/********************/

/* Place dans 'Liste' la liste des noms de librairie contenus
dans le fichier prédéfinis placés dans le répertoire de nom 'Repertoire'. */
void LireListe(
	const char *NameListe,
	ctn::E_CONTAINER_( str::string_ ) &Liste )
{
ERRProlog
	ifstream Fichier;
	str::string Ligne;
	int C;
	int Compteur = 100;
ERRBegin

	OuvrirFichierListe( NameListe, Fichier );

	Ligne.Init();

	while ( Fichier.good() && Compteur )
	{
		C = Fichier.get();

		if ( C == '\n' )
		{
			if ( Ligne.Amount() )
				Liste.Write( Ligne, Liste.New() );

			Ligne.Init();

			Compteur--;
		}
		else
			Ligne.Add( C );
	}
ERRErr
ERREnd
ERREpilog
}

// Classe la liste des librairies contenus dans 'Liste'.
void ClasserListe( ctn::E_CONTAINER_( str::string_ ) &Liste )
{
ERRProlog
	bso__bool Swap = true;
	str::string S1, S2;
ERRBegin
	S1.Init();
	S2.Init();

	while ( Swap )
	{
		Swap = false;

		if ( Liste.Amount() )
			S2 = Liste( 0 );

		for ( tym::row__ i = 1; i < Liste.Amount(); i++ )
		{
			S1 = S2;
			S2 = Liste( i );

			if ( S1 > S2 )
			{
				Swap = true;

				Liste( i-1 ) = S2;
				Liste( i ) = S1;

				S2 = S1;

			}
		}
	}

	Liste.Sync();
ERRErr
ERREnd
ERREpilog
}

/* Place dans 'Librairie' l'analyse de la librairie de nom 'Name'
située dans le répertoire 'Repertoire'. */
void Analyser(
	librairie_ &Librairie,
	const str::string_ &Name,
	const char *Repertoire )
{
	Librairie.Name = Name;
//	AnalyserDocumentation( Librairie.Documentation, Name, Repertoire );
	AnalyserLibrary( Librairie.Library, Name, Repertoire );
}

/* Ouvre dans 'Fichier' le fichier d'index de librairie sachant qu'il doit
être placé dans le répertoire 'Repertoire' */
void OuvrirFichierIndex(
	fil::file_oflow___ &Fichier,
	const char *Repertoire )
{
ERRProlog
	 str::string NameFichier;
	 char *RelaisNameFichier = NULL;
ERRBegin
	NameFichier.Init();

	NameFichier.Write( Repertoire );

	if ( !strchr( SS ":", NameFichier( NameFichier.Amount() - 1 ) ) )
	   NameFichier.Add( SC );

	NameFichier.Add( NOM_FICHIER_INDEX );

	NameFichier.Add( '\0' );

	RelaisNameFichier = NameFichier.Convert();

	if ( Fichier.Init( RelaisNameFichier, err::hSkip ) != fil::sSuccess )
	{
		fout << "Impossible d'ouvrir le fichier '" << RelaisNameFichier << "'." << nl;
		ERRt();
	}
ERRErr
ERREnd
	if ( RelaisNameFichier )
		free( RelaisNameFichier );
ERREpilog
}

// Ecrit dans 'Flot' l'entree de l'index des librairies correspondant à 'Librairie'.
void GenererEntreeIndex(
	const librairie_ &Librairie,
	txf::text_oflow___ &Flot )
{
	Flot << "<LI><TT><A HREF = \"" << Librairie.Name << TERM_FICHIER_DOC "\">" << Librairie.Name << "</A> " << Librairie.Library.Version << ' ' << Librairie.Library.Release <<  " " << Librairie.Library.Description << "</TT></LI>" << nl;
//	Flot << "<A HREF = \"" << Librairie.Name << TERM_FICHIER_DOC_DESC "\">desc.</A>):";
//	Flot << "<EM> " << Librairie.Documentation.Description << "</EM></TT>";

/*	if ( !Librairie.Documentation.Versions.IsEmpty() )
		Flot << "<BR>(" << Librairie.Documentation.Versions[ Librairie.Documentation.Versions.Occupes.Queue( Librairie.Documentation.Versions.First() ) ] << ")";
*/
}


// Créer les documentation de la librairie 'Librairie' dans le repertoir 'Repertoire'.
inline void GenererDocumentations(
	librairie_ &Librairie,
	const char *Repertoire,
	unsigned long Courant,
	unsigned long Total )
{
	fout << "Génération documentations librairie: en cours ... ('" << Librairie.Name << "' " << Courant << '/' << Total << ")        " << sync << rfl;
	GenererDocumentationTechnique( Librairie, Repertoire );
//	GenererDocumentationDescriptive( Librairie, Repertoire );
}

/*******************/
/***** NIVEAU I ****/
/*******************/

/* Place dans 'Librairies' le résultat de l'analyse des librairies
se situant dans le répértoire 'Repertoire' */
void Analyser(
	const char *NameListe,
	const char *Repertoire,
	librairies_ &Librairies )
{
ERRProlog
	// La liste des librairies.
	ctn::E_CONTAINER (str::string_ ) Liste;
	bso__ulong Compteur = 1;
	tym::row__ Courant, PListe;
ERRBegin
	Liste.Init();
	LireListe( NameListe, Liste );
	ClasserListe( Liste );

	PListe = Liste.First();

	while( PListe != NONE )
	{
		fout << "Analyse des librairies: en cours ... ('" << Liste(PListe) << "' " << ( Compteur++ ) << '/' << Liste.Amount() << ")            " << rfl;

		Courant = Librairies.Create();
		Librairies( Courant ).Init();
		Analyser( Librairies( Courant ), Liste( PListe ), Repertoire );
		Librairies.Sync();

		PListe = Liste.Next( PListe );
	}

	Liste.Sync();

	fout << "Analyse des librairies: achevée.                             " << nl;
ERRErr
ERREnd
ERREpilog
}

// Créer l'index des librairies à partir de 'Librairies' dans le répertoire 'Repertoire'
void GenererIndex(
	 const librairies &Librairies,
	 const char *Repertoire )
{
ERRProlog
	fil::file_oflow___ Fichier;
	txf::text_oflow___ Sortie;
	tym::row__ Courant;
	ctn::E_CITEM( librairie_ ) Librairie;
ERRBegin
	Librairie.Init( Librairies );

	fout << "Création de l'index des librairies: en cours ...               " << rfl;
	OuvrirFichierIndex( Fichier, Repertoire );

	Sortie.Init( Fichier );
	GenererEnTeteFichierHTML( Sortie );

	Sortie << "<HEAD><TITLE>Index of the Epeios libraries</TITLE></HEAD><BODY>";
	Sortie << "<H2><CENTER>Epeios libraries index.</CENTER></H2>" << nl;

/*
	Sortie << "<H4><CENTER>";

  for ( C = 'A'; C <= 'Z'; C++ )
		Sortie << "<A HREF = \"#" << (char)C << "\">" << (char)C << "</A> ";

	Sortie << "</CENTER></H4><UL>";
*/

	if ( Librairies.Amount() )
	{
		Courant = Librairies.First();

		while( Courant != NONE )
		{
			GenererEntreeIndex( Librairie( Courant ), Sortie );
			Courant = Librairies.Next( Courant );
		}
	}

	Sortie << "</UL><hr><p align=\"right\"><small>Generated " << TOLDateAndTime() << " by GenDoc (c) Claude SIMON</small></p></BODY>";

	fout << "Création de l'index des librairies: achevée.            " << nl;
ERRErr
ERREnd
ERREpilog
}


// Créer la documentation des 'Librairies' dans le répertoire 'Repertoire'
void GenererDocumentations(
	 librairies &Librairies,
	 const char *Repertoire )
{
ERRProlog
	tym::row__ Courant;
	int Compteur = 1;
ERRBegin
	if ( Librairies.Amount() )
	{
		Courant = Librairies.First();

		while( Courant != NONE )
		{
			GenererDocumentations( Librairies( Courant ), Repertoire, Compteur++, Librairies.Amount() );
			Courant = Librairies.Next( Courant );
		}
	}

	Librairies.Sync();
ERRErr
ERREnd
ERREpilog
}


/****************************/
/**** NIVEAU FONDAMENTAL ****/
/****************************/

int main(
	int argc,
	const char *argv[] )
{
	int ExitCode = EXIT_SUCCESS;
ERRFProlog
	// Les librairies.
	librairies Librairies;
	const char *&Liste = argv[1];
	const char *&RepertoireLibrary = argv[2];
	const char *&RepertoireDestination = argv[3];
ERRFBegin
	if ( argc != 4 )
		ERRt();

	Librairies.Init();
	Analyser( Liste, RepertoireLibrary, Librairies );
	GenererIndex( Librairies, RepertoireDestination );
	GenererDocumentations( Librairies, RepertoireDestination );
	fout << "Génération documentations librairie: achevée.                             " << nl;
ERRFErr
	ExitCode = EXIT_FAILURE;
ERRFEnd
ERRFEpilog
	return ExitCode;
}





