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
#include "hosdcm.h"

typedef hosdcm::document_filler__	xmldf__;
using hosdcm::tag_row__;


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

inline void Push(
	xmldf__ &XMLDF,
	tag_row__ &TagRow,
	const char *TagName )
{
	XMLDF.PushTag( TagName, TagRow );
}

typedef ctn::E_XCONTAINER( librairie_ ) librairies;
typedef ctn::E_XCONTAINER_( librairie_ ) librairies_;

// Procède à la mise en forme de commentaire.
void PutComment(
	const str::string_ &C,
	xmldf__ &XMLDF,
	tag_row__ &CommentTagRow )
{
ERRProlog
	str::string S;
	bool Espace = true, Special = false;
ERRBegin
	Push( XMLDF, CommentTagRow, "Comment" );

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
					XMLDF.PutValue( S, "raw" );
					S.Init();
					Special = false;
				}
				else if ( Espace )
				{
					XMLDF.PutValue( S );
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
	XMLDF.PutValue( S );
	
	XMLDF.PopTag();

ERRErr
ERREnd
ERREpilog
}

/********************/
/**** NIVEAU XII ****/
/********************/

inline void GenererDocumentationParametre(
	const parametre_ &Parametre,
	xmldf__ &XMLDF,
	tag_row__ &ParameterTagRow )
{
	Push( XMLDF, ParameterTagRow, "Parameter" );
	if ( Parametre.Valeur.Amount() != 0 )
		XMLDF.PutValue( Parametre.Valeur, "Value" );
	XMLDF.PutValue( Parametre.Type, "Type" );
	XMLDF.PutAttribute( "Name", Parametre.Name );
	XMLDF.PopTag();
}

/*******************/
/**** NIVEAU XI ****/
/*******************/

void GenererDocumentationParametres(
	const table_<parametre_> &Parametres,
	xmldf__ &XMLDF,
	tag_row__ &ParametersTagRow,
	tag_row__ &ParameterTagRow )
{
ERRProlog
	ctn::E_CITEM( parametre_ ) Parametre;
ERRBegin
	Push( XMLDF, ParametersTagRow, "Parameters" );

	Parametre.Init( Parametres );
	
	if ( Parametres.Amount() )
	{
		tym::row__ Courant = Parametres.First();

		while ( Courant != NONE )
		{
			GenererDocumentationParametre( Parametre( Courant ), XMLDF, ParameterTagRow );
			Courant = Parametres.Next( Courant );
		}
	}
	
	XMLDF.PopTag();
ERRErr
ERREnd
ERREpilog
}



/******************/
/**** NIVEAU X ****/
/******************/

inline void GenererCorpsItemClasse(
	const objet_ &Item,
	xmldf__ &XMLDF, 
	tag_row__ &CommentTagRow,
	tag_row__ &ObjectTagRow )
{
	Push( XMLDF, ObjectTagRow, "Object" );
	XMLDF.PutAttribute( "Name", Item.Name );
	XMLDF.PutValue( Item.Type, "Type" );
	PutComment( Item.Commentaire, XMLDF, CommentTagRow );
	XMLDF.PopTag();
}

inline void GenererCorpsItemClasse(
	const methode_ &Item,
	xmldf__ &XMLDF,
	tag_row__ &CommentTagRow,
	tag_row__ &MethodTagRow )
{
	static tag_row__ ParametersTagRow = NONE;
	static tag_row__ ParameterTagRow = NONE;

	Push( XMLDF, MethodTagRow, "Function" );
	XMLDF.PutAttribute( "Name", Item.Name );
	XMLDF.PutValue( Item.Type, "Type" );
	PutComment( Item.Commentaire, XMLDF, CommentTagRow );
	GenererDocumentationParametres( Item.Parametres, XMLDF, ParametersTagRow, ParameterTagRow );
	XMLDF.PopTag();
}

inline void GenererCorpsItemClasse(
	const function_ &Item,
	xmldf__ &XMLDF,
	tag_row__ &CommentTagRow,
	tag_row__ &FunctionTagRow )
{
	static tag_row__ ParametersTagRow = NONE;
	static tag_row__ ParameterTagRow = NONE;

	Push( XMLDF, FunctionTagRow, "Function" );
	XMLDF.PutAttribute( "Name", Item.Name );
	XMLDF.PutValue( Item.Type, "Type" );
	PutComment( Item.Commentaire, XMLDF, CommentTagRow );
	GenererDocumentationParametres( Item.Parametres, XMLDF, ParametersTagRow, ParameterTagRow );
	XMLDF.PopTag();
}


/*******************/
/**** NIVEAU IX ****/
/*******************/

template <class t> void GenererCorpsItemsClasse(
	const char *Tag,
	tag_row__ &TagTagRow,
	t &Items,
	xmldf__ &XMLDF,
	const char *Type,
	tag_row__ &CommentTagRow,
	tag_row__ &ItemTagRow )
{
	tym::row__ Courant = Items.First();

	Push( XMLDF, TagTagRow, Tag );
	
	XMLDF.PutAttribute( "Type", hosdcm::value( Type ) );

	while ( Courant != NONE )
	{
		GenererCorpsItemClasse( Items( Courant ), XMLDF, CommentTagRow, ItemTagRow );
		Courant = Items.Next( Courant );
	}

	Items.Sync();
	
	XMLDF.PopTag();
}


/*********************/
/**** NIVEAU VIII ****/
/*********************/

void GenererDocumentationArgumentsTemplateClasse(
	const table_<argument_> &Arguments,
	xmldf__ &XMLDF )
{
ERRProlog
	ctn::E_CITEM( argument_ ) Argument;
	tym::row__ Courant = Arguments.First();
ERRBegin

	Argument.Init( Arguments );

	XMLDF.PushTag( "Templates" );
	
	while ( Courant != NONE )
	{
		XMLDF.PushTag( "Template" );
		XMLDF.PutAttribute( "Name", Argument( Courant ).Name );
		XMLDF.PutValue( Argument( Courant ).Type, "Type" );
	
		Courant = Arguments.Next( Courant );
		XMLDF.PopTag();
	}

	XMLDF.PopTag();
ERRErr
ERREnd
ERREpilog
}

void GenererDocumentationArgumentsTemplateFonction(
	const table_<argument_> &Arguments,
	xmldf__ &XMLDF )
{
ERRProlog
	ctn::E_CITEM( argument_ ) Argument;
	tym::row__ Courant = Arguments.First();
ERRBegin

	Argument.Init( Arguments );
	
	while ( Courant != NONE )
	{
		XMLDF.PushTag( "Template" );
		XMLDF.PutAttribute( "Name", Argument( Courant ).Name );
		XMLDF.PutValue( Argument( Courant ).Type, "Type" );
		XMLDF.PopTag();
	
		Courant = Arguments.Next( Courant );
	}
ERRErr
ERREnd
ERREpilog
}


template <class t> void GenererDocumentationItemsClasse(
	const char *Tag,
	tag_row__ &TagTagRow,
	t &Items,
	xmldf__ &XMLDF,
	const char *Type,
	tag_row__ &CommentTagRow,
	tag_row__ &ItemTagRow )
{
	if ( Items.Amount() ) {
		Classer( Items );
		GenererCorpsItemsClasse( Tag, TagTagRow, Items, XMLDF, Type, CommentTagRow, ItemTagRow );
	}
}

void GenererDocumentationBasesClasseItems(
	const table_<str::string_> &Bases,
	xmldf__ &XMLDF )
{
ERRProlog
	ctn::E_CITEM( str::string_ ) Base;
	tym::row__ Courant = Bases.First();
ERRBegin
	Base.Init( Bases );

	while ( Courant != NONE ) {
		XMLDF.PushTag( "Base_class" );

		XMLDF.PutAttribute( "Name", Base( Courant ) );
		Courant = Bases.Next( Courant );

		XMLDF.PopTag();
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
	xmldf__ &XMLDF )
{
ERRProlog
	ctn::E_CITEM( item_ ) Item;
	tym::row__ Courant = Items.First();
	static tag_row__ CommentTagRow = NONE;
	static tag_row__ ItemsTagRow = NONE;
	static tag_row__ ItemTagRow = NONE;
ERRBegin
	Item.Init( Items );
	
	Push( XMLDF, ItemsTagRow, "Items" );

	while ( Courant != NONE )
	{
		Push( XMLDF, ItemTagRow, "Item" );
		XMLDF.PutAttribute( "Name", Item( Courant ).Name );
		PutComment( Item( Courant ).Commentary, XMLDF, CommentTagRow );
		
		Courant = Items.Next( Courant );
		XMLDF.PopTag();
	}
	
	XMLDF.PopTag();
ERRErr
ERREnd
ERREpilog
}


void GenererDocumentationArguments(
	const table_<str::string_> &Arguments,
	xmldf__ &XMLDF )
{
ERRProlog
	ctn::E_CITEM( str::string_ ) Argument;
	tym::row__ Courant = Arguments.First();
ERRBegin
	Argument.Init( Arguments );
	
	XMLDF.PushTag( "Arguments" );
	
	while ( Courant != NONE )
	{
		XMLDF.PushTag( "Argument" );
		XMLDF.PutAttribute( "Name", Argument( Courant ) );
		XMLDF.PopTag();
		Courant = Arguments.Next( Courant );
	}
	
	XMLDF.PopTag();
ERRErr
ERREnd
ERREpilog
}


void GenererDocumentationTemplateClasse(
	const template_ &Template,
	xmldf__ &XMLDF )
{
	if ( Template.Arguments.Amount() )
		GenererDocumentationArgumentsTemplateClasse( Template.Arguments,XMLDF );
}

void GenererDocumentationBasesClasse(
	const table_<str::string_> &Bases,
	xmldf__ &XMLDF )
{
	XMLDF.PushTag( "Base_classes" );
	GenererDocumentationBasesClasseItems( Bases, XMLDF );
	XMLDF.PopTag();

}

void GenererDocumentationTemplateFonction(
	const template_ &Template,
	xmldf__ &XMLDF )
{
	if ( Template.Arguments.Amount() )
	{
		XMLDF.PushTag( "Templates" );
		GenererDocumentationArgumentsTemplateFonction( Template.Arguments, XMLDF );
		XMLDF.PopTag();
	}
}

void GenererDocumentationObjetsClasse(
	classe_ &Classe,
	xmldf__ &XMLDF )
{
	static tag_row__ CommentTagRow = NONE;
	static tag_row__ ObjectsTagRow = NONE;
	static tag_row__ ObjectTagRow = NONE;

	if ( Classe.Objets.Amount() )
		GenererDocumentationItemsClasse( "Objects", ObjectsTagRow, Classe.Objets, XMLDF, "Public", CommentTagRow, ObjectTagRow );

	if ( Classe.Restreints.Amount() )
		GenererDocumentationItemsClasse( "Objects", ObjectsTagRow, Classe.Restreints, XMLDF, "Restricted", CommentTagRow, ObjectTagRow );
}

void GenererDocumentationMethodesClasse(
	classe_ &Classe,
	xmldf__ &XMLDF )
{
	static tag_row__ CommentTagRow = NONE;
	static tag_row__ MethodsTagRow = NONE;
	static tag_row__ MethodTagRow = NONE;

	if ( Classe.Methodes.Amount() )
		GenererDocumentationItemsClasse( "Functions", MethodsTagRow, Classe.Methodes, XMLDF, "Methods", CommentTagRow, MethodTagRow );

	if ( Classe.Virtuels.Amount() )
		GenererDocumentationItemsClasse( "Functions", MethodsTagRow, Classe.Virtuels, XMLDF, "Handlers", CommentTagRow, MethodTagRow );
}


/*******************/
/**** NIVEAU VI ****/
/*******************/


void GenererDocumentationDefine(
	const define_ &Define,
	xmldf__ &XMLDF )
{
	static tag_row__ CommentTagRow = NONE;
	
	XMLDF.PushTag( "Define" );
	XMLDF.PutAttribute( "Name", Define.Name );
	PutComment( Define.Commentaire, XMLDF, CommentTagRow );
	if ( Define.Arguments.Amount() )
		GenererDocumentationArguments( Define.Arguments, XMLDF );
	XMLDF.PopTag();
}

void GenererDocumentationEnum(
	const enum_ &Enum,
	xmldf__ &XMLDF )
{
	static tag_row__ CommentTagRow = NONE;
	
	XMLDF.PushTag( "Enum" );

	XMLDF.PutAttribute( "Name", Enum.Name );
	PutComment( Enum.Commentary, XMLDF, CommentTagRow );

	if ( Enum.Items.Amount() )
		GenererDocumentationItemsEnum( Enum.Items, XMLDF );
		
	XMLDF.PopTag();
}

void GenererDocumentationShortcut(
	const shortcut_ &Shortcut,
	xmldf__ &XMLDF )
{
	static tag_row__ CommentTagRow = NONE;
	
	XMLDF.PushTag( "Shortcut" );
	XMLDF.PutAttribute( "Name", Shortcut.Name );
	XMLDF.PutValue( Shortcut.Alias, "Alias" );
	PutComment( Shortcut.Commentaire, XMLDF, CommentTagRow );
	if ( Shortcut.Arguments.Amount() )
		GenererDocumentationArguments( Shortcut.Arguments, XMLDF );
	XMLDF.PopTag();
}

void GenererDocumentationTypedef(
	const typedef_ &Typedef,
	xmldf__ &XMLDF )
{
	static tag_row__ CommentTagRow = NONE;
	
	XMLDF.PushTag( "Typedef" );
	XMLDF.PutAttribute( "Name", Typedef.Name );
	PutComment( Typedef.Commentaire, XMLDF, CommentTagRow );
	XMLDF.PopTag();
}

inline void GenererDocumentationClasse(
	classe_ &Classe,
	xmldf__ &XMLDF )
{
	static tag_row__ CommentTagRow = NONE;
	
	XMLDF.PushTag( "Classe" );
	XMLDF.PutAttribute( "Name", Classe.Name );
	PutComment( Classe.Commentaire, XMLDF, CommentTagRow );

	GenererDocumentationTemplateClasse( Classe.Template, XMLDF );

	if ( Classe.Bases.Amount() )
		GenererDocumentationBasesClasse( Classe.Bases, XMLDF );

	GenererDocumentationObjetsClasse( Classe, XMLDF );
	GenererDocumentationMethodesClasse( Classe, XMLDF );
	XMLDF.PopTag();
}


inline void GenererDocumentationFonction(
	const function_ &Fonction,
	xmldf__ &XMLDF )
{
	static tag_row__ CommentTagRow = NONE;
	static tag_row__ ParametersTagRow = NONE;
	static tag_row__ ParameterTagRow = NONE;

	XMLDF.PushTag( "Function" );
	
	XMLDF.PutAttribute( "Name", Fonction.Name );
	XMLDF.PutValue( Fonction.Type, "Type" );
	PutComment( Fonction.Commentaire, XMLDF, CommentTagRow );
	GenererDocumentationParametres( Fonction.Parametres, XMLDF, ParametersTagRow, ParameterTagRow );
	GenererDocumentationTemplateFonction( Fonction.Template, XMLDF );

	XMLDF.PopTag();

}

template <class t> void GenererItems(
	const table_<t> &Items,
	xmldf__ &XMLDF )
{
ERRProlog
	ctn::E_CITEM( t ) Item;
	tym::row__ Courant = Items.First();
ERRBegin
	Item.Init( Items );

	while ( Courant != NONE )
	{
		XMLDF.PutValue( Item( Courant ), "Item" );
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
	xmldf__ &XMLDF )
{
	ctn::E_CITEM( define_ ) Define;
	tym::row__ Courant = Defines.First();

	Define.Init( Defines );

	XMLDF.PushTag( "Defines" );

	while ( Courant != NONE )
	{
		GenererDocumentationDefine( Define( Courant ), XMLDF );
		Courant = Defines.Next( Courant );
	}

	XMLDF.PopTag();
}

void GenererDocumentationEnums(
	const table_<enum_> &Enums,
	xmldf__ &XMLDF )
{
	ctn::E_CITEM( enum_ ) Enum;
	tym::row__ Courant = Enums.First();

	Enum.Init( Enums );

	XMLDF.PushTag( "Enums" );

	while ( Courant != NONE )
	{
		GenererDocumentationEnum( Enum( Courant ), XMLDF );
		Courant = Enums.Next( Courant );
	}

	XMLDF.PopTag();
}

void GenererDocumentationShortcuts(
	const table_<shortcut_> &Shortcuts,
	xmldf__ &XMLDF )
{
	ctn::E_CITEM( shortcut_ ) Shortcut;
	tym::row__ Courant = Shortcuts.First();

	Shortcut.Init( Shortcuts );

	XMLDF.PushTag( "Shortcuts" );

	while ( Courant != NONE )
	{
		GenererDocumentationShortcut( Shortcut( Courant ), XMLDF );
		Courant = Shortcuts.Next( Courant );
	}

	XMLDF.PopTag();
}

void GenererDocumentationTypedefs(
	const table_<typedef_> &Typedefs,
	xmldf__ &XMLDF )
{
	ctn::E_CITEM( typedef_ ) Typedef;
	tym::row__ Courant = Typedefs.First();

	Typedef.Init( Typedefs );

	XMLDF.PushTag( "Typedefs" );

	while ( Courant != NONE )
	{
		GenererDocumentationTypedef( Typedef( Courant ), XMLDF );
		Courant = Typedefs.Next( Courant );
	}

	XMLDF.PopTag();
}


void GenererDocumentationClasses(
	table_<classe_> &Classes,
	xmldf__ &XMLDF )
{
	tym::row__ Courant = Classes.First();

	XMLDF.PushTag( "Classes" );

	while ( Courant != NONE )
	{
		GenererDocumentationClasse( Classes( Courant ), XMLDF );
		Courant = Classes.Next( Courant );
	}

	XMLDF.PopTag();
	
	Classes.Sync();
}

void GenererDocumentationFonctions(
	const table_<function_> &Fonctions,
	xmldf__ &XMLDF,
	const char *Type )
{
	static tag_row__ CommentTagRow = NONE;
	static tag_row__ FunctionsTagRow = NONE;
	static tag_row__ FunctionTagRow = NONE;
	epeios::row__ Row = NONE;
	ctn::E_CITEM( function_ ) Function;

	XMLDF.PushTag( "Functions" );

	Function.Init( Fonctions );

	Row = Fonctions.First();

	while( Row != NONE ) {
		GenererDocumentationFonction( Function( Row ), XMLDF ); 
		Row = Fonctions.Next( Row );
	}

	XMLDF.PopTag();
}

void GenererDocumentationObjets(
	table_<objet_> &Objets,
	xmldf__ &XMLDF,
	const char *Type )
{
	static tag_row__ CommentTagRow = NONE;
	static tag_row__ ObjectsTagRow = NONE;
	static tag_row__ ObjectTagRow = NONE;

	GenererDocumentationItemsClasse( "Objects", ObjectsTagRow, Objets, XMLDF, Type, CommentTagRow, ObjectTagRow );
}



void GenererGeneralites(
	const str::string_ &Generalites,
	xmldf__ &XMLDF )
{
	XMLDF.PutValue( Generalites, "Généralités" );
}

void GenererVersions(
	const table_<str::string_> &Versions,
	xmldf__ &XMLDF )
{

	XMLDF.PushTag( "Versions" );
	
	if ( Versions.Amount() )
		GenererItems( Versions, XMLDF );

	XMLDF.PopTag();
}

void GenererHistorique(
	const table_<str::string_> &Historique,
	xmldf__ &XMLDF )
{
	XMLDF.PushTag( "Historique" );

	if ( Historique.Amount() )
		GenererItems( Historique, XMLDF );

	XMLDF.PopTag();
}

void GenererRemarques(
	const table_<str::string_> &Remarques,
	xmldf__ &XMLDF )
{
	XMLDF.PushTag( "Remarques" );

	if ( Remarques.Amount() )
		GenererItems( Remarques, XMLDF );

	XMLDF.PopTag();
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
	xmldf__ &XMLDF )
{
	XMLDF.PutValue( Library.Description, "Description" );
	XMLDF.PutValue( Library.Coordinators, "Coordinators" );
	XMLDF.PutValue( Library.Version, "Version" );
	XMLDF.PutValue( Library.Release, "Release" );

	if ( Library.Defines.Amount() )
	{
		Classer( Library.Defines );
		GenererDocumentationDefines( Library.Defines, XMLDF );
	}

	if ( Library.Enums.Amount() )
	{
		Classer( Library.Enums );
		GenererDocumentationEnums( Library.Enums, XMLDF );
	}

	if ( Library.Shortcuts.Amount() )
	{
		Classer( Library.Shortcuts );
		GenererDocumentationShortcuts( Library.Shortcuts, XMLDF );
	}

	if ( Library.Typedefs.Amount() )
	{
		Classer( Library.Typedefs );
		GenererDocumentationTypedefs( Library.Typedefs, XMLDF );
	}

	if ( Library.Classes.Amount() )
	{
		Classer( Library.Classes );
		GenererDocumentationClasses( Library.Classes, XMLDF );
	}

	if ( Library.Functions.Amount() )
	{
		Classer( Library.Functions );
		GenererDocumentationFonctions( Library.Functions, XMLDF, "" );
	}

	if ( Library.Objets.Amount() )
	{
		Classer( Library.Objets );
		GenererDocumentationObjets( Library.Objets, XMLDF, "" );
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

	Fichier.EOFD( XTF_EOXT );

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
	xmldf__ &XMLDF )
{
	XMLDF.PutAttribute( "Name", Librairie.Name );
 	GenererCorpsDocumentationTechnique( Librairie.Library, XMLDF );
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
	bso::bool__ Swap = true;
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
	Librairie.Name.Allocate( Name.Amount() -  2 );
	Librairie.Name.Write( Name, Name.Amount() -  2 );
//	AnalyserDocumentation( Librairie.Documentation, Name, Repertoire );
	AnalyserLibrary( Librairie.Library, Name, Repertoire );
}


// Créer les documentation de la librairie 'Librairie' dans le repertoir 'Repertoire'.
inline void GenererDocumentations(
	librairie_ &Librairie,
	xmldf__ &XMLDF,
	unsigned long Courant,
	unsigned long Total )
{
	XMLDF.PushTag( "Library" );
	fout << "Documentation generation : in progress ... ('" << Librairie.Name << "' " << Courant << '/' << Total << ")        " << sync << rfl;
	GenererDocumentationTechnique( Librairie, XMLDF );
	XMLDF.PopTag();
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
	bso::ulong__ Compteur = 1;
	tym::row__ Courant, PListe;
ERRBegin
	Liste.Init();
	LireListe( NameListe, Liste );
	ClasserListe( Liste );

	PListe = Liste.First();

	while( PListe != NONE )
	{
		fout << "Library parsing : in progress ... ('" << Liste(PListe) << "' " << ( Compteur++ ) << '/' << Liste.Amount() << ")            " << rfl;

		Courant = Librairies.Create();
		Librairies( Courant ).Init();
		Analyser( Librairies( Courant ), Liste( PListe ), Repertoire );
		Librairies.Sync();

		PListe = Liste.Next( PListe );
	}

	Liste.Sync();

	fout << "Library parsing : terminated.                             " << nl;
ERRErr
ERREnd
ERREpilog
}


// Créer la documentation des 'Librairies' dans le répertoire 'Repertoire'
void GenererDocumentations(
	 librairies &Librairies,
	 xmldf__ &XMLDF )
{
ERRProlog
	tym::row__ Courant;
	int Compteur = 1;
ERRBegin
	XMLDF.PushTag( "Libraries" );

	if ( Librairies.Amount() )
	{
		Courant = Librairies.First();

		while( Courant != NONE )
		{
			GenererDocumentations( Librairies( Courant ), XMLDF, Compteur++, Librairies.Amount() );
			Courant = Librairies.Next( Courant );
		}
	}

	Librairies.Sync();
	
	XMLDF.PopTag();

	fout << "Documentation generation : terminated.                             " << nl;
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
	hosdcm::document XMLD;
	hosdcm::document_filler__ XMLDF;
	fil::file_oflow___ OFlow;
	txf::text_oflow___ TFlow;
ERRFBegin
	if ( argc != 4 )
		ERRt();
		
	Librairies.Init();
	Analyser( Liste, RepertoireLibrary, Librairies );
	
	XMLD.Init();
	XMLDF.Init( XMLD );
	GenererDocumentations( Librairies, XMLDF );
	
	OFlow.Init( FichierDestination );
	TFlow.Init( OFlow );
	
//	GenererEnTeteFichierXML( TFlow );

	fout << "File writing : in progress ... " << rfl << sync;

	hosdcm::WriteXML( XMLD, TFlow );

	fout << "File writing : terminated.     " << sync << nl;
ERRFErr
	ExitCode = EXIT_FAILURE;
ERRFEnd
ERRFEpilog
	return ExitCode;
}





