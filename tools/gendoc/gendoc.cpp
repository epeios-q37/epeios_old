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
#include "flx.h"
#include "txmtbl.h"
#include "hdbtol.h"
#include "hdbxml.h"

typedef hdbxml::hierarchical_database_filler__	hdbf__;


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

void GenererEnTeteFichierXML( txf::text_oflow___ &Stream )
{
	Stream <<  "<?xml version=\"1.0\"?>";
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

// Procède à la mise en forme de commentaire.
void PutComment(
	const str::string_ &C,
	hdbf__ &HDBF )
{
ERRProlog
	str::string S;
	bool Espace = true, Special = false;
ERRBegin
	HDBF.PushTag( "Comment" );

	S.Init();

	for ( epeios::row_t__ Compteur = 0; Compteur < C.Amount(); Compteur++ )
	{
		if ( C.Read( Compteur ) == ' ' )
		{
			Espace = true;
			S.Add( ' ' );
		}
		else if ( C.Read( Compteur ) == '\r' ) {}
		else if ( C.Read( Compteur ) == '\n' ) {}
		else
		{
			if ( C.Read( Compteur ) == '\'' )
			{
				if ( Special )
				{
					HDBF.PutValue( S, "Ref" );
					S.Init();
					Special = false;
				}
				else if ( Espace )
				{
					HDBF.PutValue( S );
					S.Init();
					Special = true;
				}
				else
					S.Add( '\'' );
			}
			else
				S.Add( C.Read( Compteur ) );

			Espace = false;
		}
	}
	
	S.Add( '\n' );
	HDBF.PutValue( S );
	
	HDBF.PopTag();

ERRErr
ERREnd
ERREpilog
}

/* fournit une référence sur un 'str::string_' dont les caractères '<' et '>' ont
été remplacé par les symboles adéquats pour qu'un navigateur Internet ne les
prennent pas pour des délimiteurs de tags */
const str::string_ &Filtrer( const str::string_ &S )
{
	static str::string R;

	R.Init();

	for ( epeios::row_t__ i = 0; i < S.Amount(); i++ )
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
	hdbf__ &HDBF )
{
	HDBF.PushTag( "Parameter" );
	if ( Parametre.Valeur.Amount() != 0 )
		HDBF.PutValue( Parametre.Valeur, "Value" );
	HDBF.PutValue( Parametre.Type, "Type" );
	HDBF.PutValue( Parametre.Name, "Name" );
	HDBF.PopTag();
}

/*******************/
/**** NIVEAU XI ****/
/*******************/

void GenererDocumentationParametres(
	const table_<parametre_> &Parametres,
	hdbf__ &HDBF )
{
ERRProlog
	ctn::E_CITEM( parametre_ ) Parametre;
ERRBegin
	HDBF.PushTag( "Parameters" );

	Parametre.Init( Parametres );
	
	if ( Parametres.Amount() )
	{
		tym::row__ Courant = Parametres.First();

		while ( Courant != NONE )
		{
			GenererDocumentationParametre( Parametre( Courant ), HDBF );
			Courant = Parametres.Next( Courant );
		}
	}
	
	HDBF.PopTag();
ERRErr
ERREnd
ERREpilog
}



/******************/
/**** NIVEAU X ****/
/******************/

inline void GenererCorpsItemClasse(
	const objet_ &Item,
	hdbf__ &HDBF )
{

	HDBF.PushTag( "Object" );
	HDBF.PutValue( Item.Name, "Name" );
	HDBF.PutValue( Filtrer( Item.Type ), "Type" );
	PutComment( Item.Commentaire, HDBF );
	HDBF.PopTag();
}

inline void GenererCorpsItemClasse(
	const methode_ &Item,
	hdbf__ &HDBF )
{
	HDBF.PushTag( "Method" );
	HDBF.PutValue( Item.Name, "Name" );
	HDBF.PutValue( Item.Type, "Type" );
	PutComment( Item.Commentaire, HDBF );
	GenererDocumentationParametres( Item.Parametres, HDBF );
	HDBF.PopTag();
}

inline void GenererCorpsItemClasse(
	const function_ &Item,
	hdbf__ &HDBF )
{
	HDBF.PushTag( "Function" );
	HDBF.PutValue( Filtrer( Item.Name ), "Name" );
	HDBF.PutValue( Item.Type, "Type" );
	PutComment( Item.Commentaire, HDBF );
	GenererDocumentationParametres( Item.Parametres, HDBF );
	HDBF.PopTag();
}


/*******************/
/**** NIVEAU IX ****/
/*******************/

template <class t> void GenererCorpsItemsClasse(
	const char *Tag,
	t &Items,
	hdbf__ &HDBF )
{
	tym::row__ Courant = Items.First();

	HDBF.PushTag( Tag );

	while ( Courant != NONE )
	{
		GenererCorpsItemClasse( Items( Courant ), HDBF );
		Courant = Items.Next( Courant );
	}

	Items.Sync();
	
	HDBF.PopTag();
}


/*********************/
/**** NIVEAU VIII ****/
/*********************/

void GenererDocumentationArgumentsTemplateClasse(
	const table_<argument_> &Arguments,
	hdbf__ &HDBF )
{
ERRProlog
	ctn::E_CITEM( argument_ ) Argument;
	tym::row__ Courant = Arguments.First();
ERRBegin

	Argument.Init( Arguments );
	
	while ( Courant != NONE )
	{
		HDBF.PutValue( Argument( Courant ).Name, "Name" );
		HDBF.PutValue( Argument( Courant ).Type, "Type" );
	
		Courant = Arguments.Next( Courant );
	}
ERRErr
ERREnd
ERREpilog
}

void GenererDocumentationArgumentsTemplateFonction(
	const table_<argument_> &Arguments,
	hdbf__ &HDBF )
{
ERRProlog
	ctn::E_CITEM( argument_ ) Argument;
	tym::row__ Courant = Arguments.First();
ERRBegin

	Argument.Init( Arguments );
	
	while ( Courant != NONE )
	{
		HDBF.PutValue( Argument( Courant ).Name, "Name" );
		HDBF.PutValue( Argument( Courant ).Type, "Type" );
	
		Courant = Arguments.Next( Courant );
	}
ERRErr
ERREnd
ERREpilog
}


template <class t> void GenererDocumentationItemsClasse(
	const char *Tag,
	t &Items,
	hdbf__ &HDBF )
{
	if ( Items.Amount() )
		GenererCorpsItemsClasse( Tag, Items, HDBF );
}

void GenererDocumentationBasesClasseItems(
	const table_<str::string_> &Bases,
	hdbf__ &HDBF )
{
ERRProlog
	ctn::E_CITEM( str::string_ ) Base;
	tym::row__ Courant = Bases.First();
ERRBegin
	Base.Init( Bases );

	while ( Courant != NONE ) {
		HDBF.PutValue( Filtrer( Base( Courant ) ), "Name" );
		Courant = Bases.Next( Courant );
	}
ERRErr
ERREnd
ERREpilog
}

/********************/
/**** NIVEAU VII ****/
/********************/

void GenererDocumentationItemsEnum(
	const table_<item_> &Items,
	hdbf__ &HDBF )
{
ERRProlog
	 ctn::E_CITEM( item_ ) Item;
	tym::row__ Courant = Items.First();
ERRBegin
	Item.Init( Items );
	
	HDBF.PushTag( "Items" );

	while ( Courant != NONE )
	{
		HDBF.PushTag( "Item" );
		HDBF.PutValue( Item( Courant ).Name, "Name" );
		PutComment( Item( Courant ).Commentary, HDBF );
		
		Courant = Items.Next( Courant );
		HDBF.PopTag();
	}
	
	HDBF.PopTag();
ERRErr
ERREnd
ERREpilog
}


void GenererDocumentationArguments(
	const table_<str::string_> &Arguments,
	hdbf__ &HDBF )
{
ERRProlog
	ctn::E_CITEM( str::string_ ) Argument;
	tym::row__ Courant = Arguments.First();
ERRBegin
	Argument.Init( Arguments );
	
	while ( Courant != NONE )
	{
		HDBF.PutValue( Argument( Courant ), "Name" );
		Courant = Arguments.Next( Courant );
	}
ERRErr
ERREnd
ERREpilog
}


void GenererDocumentationTemplateClasse(
	const template_ &Template,
	hdbf__ &HDBF )
{
	if ( Template.Arguments.Amount() )
		GenererDocumentationArgumentsTemplateClasse( Template.Arguments,HDBF );
}

void GenererDocumentationBasesClasse(
	const table_<str::string_> &Bases,
	hdbf__ &HDBF )
{
	HDBF.PushTag( "Base_classes" );
	GenererDocumentationBasesClasseItems( Bases, HDBF );
	HDBF.PopTag();

}

void GenererDocumentationTemplateFonction(
	const template_ &Template,
	hdbf__ &HDBF )
{
	if ( Template.Arguments.Amount() )
		GenererDocumentationArgumentsTemplateFonction( Template.Arguments, HDBF );
}

void GenererDocumentationObjetsClasse(
	classe_ &Classe,
	hdbf__ &HDBF )
{

	if ( Classe.Objets.Amount() )
		GenererDocumentationItemsClasse( "Object", Classe.Objets, HDBF );

	if ( Classe.Restreints.Amount() )
		GenererDocumentationItemsClasse( "Restrict", Classe.Restreints, HDBF );
}

void GenererDocumentationMethodesClasse(
	classe_ &Classe,
	hdbf__ &HDBF )
{
	if ( Classe.Methodes.Amount() )
		GenererDocumentationItemsClasse( "Methods", Classe.Methodes, HDBF );

	if ( Classe.Virtuels.Amount() )
		GenererDocumentationItemsClasse( "Vituals", Classe.Virtuels, HDBF );
}


/*******************/
/**** NIVEAU VI ****/
/*******************/


void GenererDocumentationDefine(
	const define_ &Define,
	hdbf__ &HDBF )
{
	if ( Define.Arguments.Amount() )
		GenererDocumentationArguments( Define.Arguments, HDBF );
}

void GenererDocumentationEnum(
	const enum_ &Enum,
	hdbf__ &HDBF )
{
	HDBF.PushTag( "Enum" );

	HDBF.PutValue( Enum.Name, "Name" );
	PutComment( Enum.Commentary, HDBF );

	if ( Enum.Items.Amount() )
		GenererDocumentationItemsEnum( Enum.Items, HDBF );
		
	HDBF.PopTag();
}

void GenererDocumentationShortcut(
	const shortcut_ &Shortcut,
	hdbf__ &HDBF )
{
	HDBF.PutValue( Shortcut.Name, "Name" );
	HDBF.PutValue( Shortcut.Alias, "Alias" );
	PutComment( Shortcut.Commentaire, HDBF );
}

void GenererDocumentationTypedef(
	const typedef_ &Typedef,
	hdbf__ &HDBF )
{
	HDBF.PushTag( "Typedef" );
	HDBF.PutValue( Typedef.Name, "Name" );
	PutComment( Typedef.Commentaire, HDBF );
	HDBF.PopTag();
}

inline void GenererDocumentationClasse(
	classe_ &Classe,
	hdbf__ &HDBF )
{

	HDBF.PushTag( "Classe" );
	HDBF.PutValue( Classe.Name, "Name" );
	PutComment( Classe.Commentaire, HDBF );

	GenererDocumentationTemplateClasse( Classe.Template, HDBF );

	if ( Classe.Bases.Amount() )
		GenererDocumentationBasesClasse( Classe.Bases, HDBF );

	GenererDocumentationObjetsClasse( Classe, HDBF );
	GenererDocumentationMethodesClasse( Classe, HDBF );
	HDBF.PopTag();
}


inline void GenererDocumentationFonction(
	const function_ &Fonction,
	hdbf__ &HDBF )
{

	HDBF.PutValue( Fonction.Name, "Name" );
	HDBF.PutValue( Fonction.Type, "Type" );
	PutComment( Fonction.Commentaire, HDBF );
	GenererDocumentationParametres( Fonction.Parametres, HDBF );
	GenererDocumentationTemplateFonction( Fonction.Template, HDBF );

}

template <class t> void GenererItems(
	const table_<t> &Items,
	hdbf__ &HDBF )
{
ERRProlog
	ctn::E_CITEM( t ) Item;
	tym::row__ Courant = Items.First();
ERRBegin
	Item.Init( Items );

	while ( Courant != NONE )
	{
		HDBF.PutValue( Item( Courant ), "Item" );
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
	hdbf__ &HDBF )
{
	ctn::E_CITEM( define_ ) Define;
	tym::row__ Courant = Defines.First();

	Define.Init( Defines );

	HDBF.PushTag( "Defines" );

	while ( Courant != NONE )
	{
		GenererDocumentationDefine( Define( Courant ), HDBF );
		Courant = Defines.Next( Courant );
	}

	HDBF.PopTag();
}

void GenererDocumentationEnums(
	const table_<enum_> &Enums,
	hdbf__ &HDBF )
{
	ctn::E_CITEM( enum_ ) Enum;
	tym::row__ Courant = Enums.First();

	Enum.Init( Enums );

	HDBF.PushTag( "Enums" );

	while ( Courant != NONE )
	{
		GenererDocumentationEnum( Enum( Courant ), HDBF );
		Courant = Enums.Next( Courant );
	}

	HDBF.PopTag();
}

void GenererDocumentationShortcuts(
	const table_<shortcut_> &Shortcuts,
	hdbf__ &HDBF )
{
	ctn::E_CITEM( shortcut_ ) Shortcut;
	tym::row__ Courant = Shortcuts.First();

	Shortcut.Init( Shortcuts );

	HDBF.PushTag( "Shortcuts" );

	while ( Courant != NONE )
	{
		GenererDocumentationShortcut( Shortcut( Courant ), HDBF );
		Courant = Shortcuts.Next( Courant );
	}

	HDBF.PopTag();
}

void GenererDocumentationTypedefs(
	const table_<typedef_> &Typedefs,
	hdbf__ &HDBF )
{
	ctn::E_CITEM( typedef_ ) Typedef;
	tym::row__ Courant = Typedefs.First();

	Typedef.Init( Typedefs );

	HDBF.PushTag( "Typedefs" );

	while ( Courant != NONE )
	{
		GenererDocumentationTypedef( Typedef( Courant ), HDBF );
		Courant = Typedefs.Next( Courant );
	}

	HDBF.PopTag();
}


void GenererDocumentationClasses(
	table_<classe_> &Classes,
	hdbf__ &HDBF )
{
	tym::row__ Courant = Classes.First();

	HDBF.PushTag( "Classes" );

	while ( Courant != NONE )
	{
		GenererDocumentationClasse( Classes( Courant ), HDBF );
		Courant = Classes.Next( Courant );
	}

	HDBF.PopTag();
	
	Classes.Sync();
}

void GenererDocumentationFonctions(
	table_<function_> &Fonctions,
	hdbf__ &HDBF )
{
	GenererDocumentationItemsClasse( "Functions", Fonctions, HDBF );
}

void GenererDocumentationObjets(
	table_<objet_> &Objets,
	hdbf__ &HDBF )
{
	GenererDocumentationItemsClasse( "Objects", Objets, HDBF );
}



void GenererGeneralites(
	const str::string_ &Generalites,
	hdbf__ &HDBF )
{
	HDBF.PutValue( Generalites, "Généralités" );
}

void GenererVersions(
	const table_<str::string_> &Versions,
	hdbf__ &HDBF )
{

	HDBF.PushTag( "Versions" );
	
	if ( Versions.Amount() )
		GenererItems( Versions, HDBF );

	HDBF.PopTag();
}

void GenererHistorique(
	const table_<str::string_> &Historique,
	hdbf__ &HDBF )
{
	HDBF.PushTag( "Historique" );

	if ( Historique.Amount() )
		GenererItems( Historique, HDBF );

	HDBF.PopTag();
}

void GenererRemarques(
	const table_<str::string_> &Remarques,
	hdbf__ &HDBF )
{
	HDBF.PushTag( "Remarques" );

	if ( Remarques.Amount() )
		GenererItems( Remarques, HDBF );

	HDBF.PopTag();
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

void GenererCorpsDocumentationTechnique(
	library_ &Library,
	hdbf__ &HDBF )
{
	if ( Library.Defines.Amount() )
	{
		Classer( Library.Defines );
		GenererDocumentationDefines( Library.Defines, HDBF );
	}

	if ( Library.Enums.Amount() )
	{
		Classer( Library.Enums );
		GenererDocumentationEnums( Library.Enums, HDBF );
	}

	if ( Library.Shortcuts.Amount() )
	{
		Classer( Library.Shortcuts );
		GenererDocumentationShortcuts( Library.Shortcuts, HDBF );
	}

	if ( Library.Typedefs.Amount() )
	{
		Classer( Library.Typedefs );
		GenererDocumentationTypedefs( Library.Typedefs, HDBF );
	}

	if ( Library.Classes.Amount() )
	{
		Classer( Library.Classes );
		GenererDocumentationClasses( Library.Classes, HDBF );
	}

	if ( Library.Functions.Amount() )
	{
		Classer( Library.Functions );
		GenererDocumentationFonctions( Library.Functions, HDBF );
	}

	if ( Library.Objets.Amount() )
	{
		Classer( Library.Objets );
		GenererDocumentationObjets( Library.Objets, HDBF );
	}
}

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
	hdbf__ &HDBF )
{
	HDBF.PutValue( Librairie.Name, "Name" );
 	GenererCorpsDocumentationTechnique( Librairie.Library, HDBF );
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

		for ( epeios::row_t__ i = 1; i < Liste.Amount(); i++ )
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


// Créer les documentation de la librairie 'Librairie' dans le repertoir 'Repertoire'.
inline void GenererDocumentations(
	librairie_ &Librairie,
	hdbf__ &HDBF,
	unsigned long Courant,
	unsigned long Total )
{
	HDBF.PushTag( "Library" );
	fout << "Génération documentations librairie: en cours ... ('" << Librairie.Name << "' " << Courant << '/' << Total << ")        " << sync << rfl;
	GenererDocumentationTechnique( Librairie, HDBF );
	HDBF.PopTag();
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


// Créer la documentation des 'Librairies' dans le répertoire 'Repertoire'
void GenererDocumentations(
	 librairies &Librairies,
	 hdbf__ &HDBF )
{
ERRProlog
	tym::row__ Courant;
	int Compteur = 1;
ERRBegin
	HDBF.PushTag( "Documentation" );

	if ( Librairies.Amount() )
	{
		Courant = Librairies.First();

		while( Courant != NONE )
		{
			GenererDocumentations( Librairies( Courant ), HDBF, Compteur++, Librairies.Amount() );
			Courant = Librairies.Next( Courant );
		}
	}

	Librairies.Sync();
	
	HDBF.PopTag();
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
	const char *&FichierDestination = argv[3];
	hdbmnb::hierarchical_database HDB;
	hdbxml::hierarchical_database_filler__ HDBF;
	fil::file_oflow___ OFlow;
	txf::text_oflow___ TFlow;

ERRFBegin
	if ( argc != 4 )
		ERRt();
		
	Librairies.Init();
	Analyser( Liste, RepertoireLibrary, Librairies );
	
	HDB.Init();
	HDBF.Init( HDB );
	GenererDocumentations( Librairies, HDBF );
	
	OFlow.Init( FichierDestination );
	TFlow.Init( OFlow );
	
	GenererEnTeteFichierXML( TFlow );
	
	hdbxml::WriteXML( HDB, TFlow );

	fout << "Génération documentations librairie: achevée.                             " << nl;
ERRFErr
	ExitCode = EXIT_FAILURE;
ERRFEnd
ERRFEpilog
	return ExitCode;
}





