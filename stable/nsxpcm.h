/*
	Header for the 'nsxpcm' library by Claude SIMON (csimon@epeios.org)
	Copyright (C) 2004 Claude SIMON (csimon@epeios.org).

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

#ifndef NSXPCM__INC
#define NSXPCM__INC

#define NSXPCM_NAME		"NSXPCM"

#define	NSXPCM_VERSION	"$Revision$"

#define NSXPCM_OWNER		"Claude SIMON (csimon@epeios.org)"

#include "ttr.h"

extern class ttr_tutor &NSXPCMTutor;

#if defined( XXX_DBG ) && !defined( NSXPCM_NODBG )
#define NSXPCM_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision$
//C Claude SIMON (csimon@epeios.org)
//R $Date$

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D from nsXPCOM 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

// Ce dichier d'entête nécessite un environnement de développement génèré à partir des sources !

#include "err.h"
#include "flw.h"
#include "epeios.h"
#include "str.h"
#include "ctn.h"

#include "nsEmbedString.h"
#include "nsiDOMDocument.h"
#include "nsiDOMElement.h"
#include "nsIDOMXULMultSelectCntrlEl.h"
#include "nsIDOMXULSelectCntrlItemEl.h"
#include "nsIDOMXULTextboxElement.h"
#include "nsIDOMXULMenuListElement.h"
#include "nsIDOMXULCheckboxElement.h"
#include "nsIDOMXULTreeElement.h"
#include "nsIDOMXULDescriptionElement.h"
#include "nsCOMPtr.h"
#include "nsITreeView.h"
#include "nsITreeContentView.h"
#include "nsITreeSelection.h"
#include "nsIListBoxObject.h"
#include "nsIDOMWindowInternal.h"
#include "nsIXULWindow.h"
#include "nsComponentManagerUtils.h"
#include "nsIInterfaceRequestor.h"
#include "nsIDOMXULSelectCntrlItemEl.h"
#ifdef NSXPCM_BKD
#	define NSXPCM__BKD
#endif

#ifdef NSXPCM__BKD
#	include "bkdacc.h"
#endif

namespace nsxpcm {

	using str::string_;
	using str::string;

#ifdef NSXPCM__BKD
	using bkdacc::strings_;
	using bkdacc::strings;
#else
	typedef ctn::E_XMCONTAINER_( string_ ) strings_;
	E_AUTO( strings );
#endif

	void Transform(
		const char *CString,
		epeios::size__ Size,
		char **JString );

	void Transform(
		const char *CString,
		char **JString );

	void Transform(
		const nsEmbedCString &ECString,
		nsEmbedString &EString );

	void Transform( 
		const char *String,
		nsEmbedString &EString );

	void Transform(
		const nsEmbedString &EString,
		nsEmbedCString &ECString );

	void Transform( 
		const nsEmbedString &EString,
		char **JString );

	void Transform(
		const str::string_ &EString,
		char **JString );

	void Transform(
		const str::string_ &String,
		nsEmbedString &EString );

	void Transform( 
		const nsEmbedString &EString,
		str::string_ &String );

	void Transform(
		const nsEmbedCString &ECString,
		str::string_ &String );

	void Transform(
		const str::string_ &String,
		nsEmbedCString &ECString );

	void Split( 
		const string_ &Joined,
		bso::char__ Seaparator,
		strings_ &Splitted );

	void Split(
		const char *Joined,
		bso::char__ Separator,
		nsxpcm::strings_ &Splitted );

	void Join(
		const strings_ &Splitted,
		bso::char__ Separator,
		str::string_ &Joined );

	void JoinAndTransform(
		const strings_ &Splitted,
		bso::char__ Separator,
		char **JString );

	// 'ContractID' est une chaîne de caratère du genre "@mozilla.org/filepicker;1".
	template <typename t> inline t *CreateInstance(
		const char *ContractID,
		nsCOMPtr<t> &Instance )
	{	
		nsresult Error = NS_OK;

		Instance = do_CreateInstance( ContractID, &Error );

		if ( Error != NS_OK )
			ERRu();

		return Instance;
	}

	inline nsIDOMElement *GetElementById(
		nsIDOMDocument *Document,
		const char *Id )
	{
		nsIDOMElement *Element = NULL;
		nsEmbedString EId;

		Transform( Id, EId );

		if ( Document->GetElementById( EId, &Element ) != NS_OK )
			ERRu();

		return Element;
	}

	inline nsIDOMElement *GetElementById(
		nsIDOMDocument *Document,
		const str::string_ &Id )
	{
		nsIDOMElement *Element = NULL;
		nsEmbedString EId;

		Transform( Id, EId );

		if ( Document->GetElementById( EId, &Element ) != NS_OK )
			ERRu();

		return Element;
	}

	template <typename element> inline element *QueryInterface(
		nsISupports *GenericElement,
		err::handle ErrHandle = err::hUsual )
	{
		element *Element = NULL;

#ifdef NSXPCM_DBG

		if ( GenericElement == NULL )
			ERRu();
#endif
		if ( ( GenericElement->QueryInterface( element::GetIID(), (void **)&Element ) != NS_OK ) && ( ErrHandle != err::hSkip ) )
			ERRu();

		return Element;
	}

	template <typename element> inline element *GetInterface( nsISupports *GenericElement )
	{
		nsCOMPtr<nsIInterfaceRequestor> Requestor;
		element *Element = NULL;

#ifdef NSXPCM_DBG

		if ( GenericElement == NULL )
			ERRu();
#endif
		Requestor = QueryInterface<nsIInterfaceRequestor>( GenericElement );


		if ( Requestor->GetInterface( element::GetIID(), (void **)&Element ) != NS_OK )
			ERRu();

		return Element;
	}

	template <typename element> inline element *_GetElementById(
		nsIDOMDocument *Document,
		const char *Id )
	{
		return QueryInterface<element>( GetElementById( Document, Id ) );
	}

	template <typename element> inline element *_GetElementById(
		nsIDOMDocument *Document,
		const string_ &Id )
	{
		return QueryInterface<element>( GetElementById( Document, Id ) );
	}

	inline nsIDOMElement *CreateElement(
		nsIDOMDocument *Document,
		const char *Name )
	{
		nsIDOMElement* Element;
		nsEmbedString EName;

		Transform( Name, EName );

#ifdef NSXPCM_DBG
		if ( Document == NULL )
			ERRu();
#endif

		Document->CreateElement( EName, &Element );

		if ( Element == NULL )
			ERRs();

		return Element;

	}

	template <typename element> inline element *_CreateElement(
		nsIDOMDocument *Document,
		const char *Name )
	{
#if 0	// Ne fonctionne pas !
		return QueryInterface<element>( CreateElement( Document, Name ) );

		// Ne pas oulier de tester 'Result' lors de la mise en place de ma version fonctionnelle.
#else	// Fonctionne, mais présente des risques ...
		return (element *)CreateElement( Document, Name );
#endif
	}

/*
	template <typename element> struct _element__
	{
	private:
		element *_Element;
	public:
		_element__( element *Element = NULL )
		{
			_Element = Element;
		}
		operator element *( void )
		{
			return _Element;
		}
		const element *operator ->( void ) const
		{
			return _Element;
		}
		element *operator ->( void )
		{
			return _Element;
		}
		element *&operator()( void )
		{
			return _Element;
		}
		const element *&operator()( void ) const
		{
			return _Element;
		}
		bool operator ==( const element *Op )
		{
			return ( Op == _Element );
		}
		bool operator !=( const element *Op )
		{
			return ( Op != _Element );
		}
	};
*/

	// 'name' nom du type 'doit correspondre au nom du tag XUL associé); 'Name' pour le fonctions associées.
	#define NSXPCM_DEFINE( element, name, Name )\
		typedef element name;\
		typedef element name##_;\
		inline name *Get##Name##ById(\
			nsIDOMDocument *Document,\
			const char *Id )\
		{\
			return _GetElementById< name >( Document, Id );\
		}\
		inline name *Get##Name##ById(\
			nsIDOMDocument *Document,\
			const string_ &Id )\
		{\
			return _GetElementById< name >( Document, Id );\
		}\
		inline name *Create##Name##( nsIDOMDocument *Document )\
		{\
			return _CreateElement< name >( Document, #name );\
		}


	NSXPCM_DEFINE( nsIDOMXULMultiSelectControlElement, listbox, Listbox )
	NSXPCM_DEFINE( nsIDOMXULSelectControlItemElement, listitem, Listitem )
	NSXPCM_DEFINE( nsIDOMXULTextBoxElement, textbox, Textbox )
	NSXPCM_DEFINE( nsIDOMXULMenuListElement, menulist, Menulist )
	NSXPCM_DEFINE( nsIDOMXULCheckboxElement, checkbox, Checkbox )
	NSXPCM_DEFINE( nsIDOMXULTreeElement, tree, Tree )

	inline void SetAttribute(
		nsIDOMElement *Element,
		const char *Name,
		const char *Value )
	{
		nsEmbedString EName, EValue;

		Transform( Name, EName );
		Transform( Value, EValue );

#ifdef NSXPCM_DBG
		if ( Element == NULL )
			ERRu();
#endif
		
		Element->SetAttribute( EName, EValue );
	}

	inline void RemoveAttribute(
		nsIDOMElement *Element,
		const char *Name )
	{
		nsEmbedString EName;

		Transform( Name, EName );

#ifdef NSXPCM_DBG
		if ( Element == NULL )
			ERRu();
#endif
		
		Element->RemoveAttribute( EName );
	}

	template <typename t> inline void SetValue(
		t *Element,
		const char *Value )
	{
		nsEmbedString EValue;

		Transform( Value, EValue );

#ifdef NSXPCM_DBG
		if ( Element == NULL )
			ERRu();
#endif
		
		Element->SetValue( EValue );
	}

	inline const str::string_ &GetAttribute(
		nsIDOMElement *Element,
		const char *Name,
		str::string_ &Value )
	{
		nsEmbedString EName, EValue;
		Transform( Name, EName );

		Element->GetAttribute( EName, EValue );

		Transform( EValue, Value );

		return Value;
	}

	template <typename t> inline const str::string_ &GetValue(
		t *Element,
		str::string_ &Value )
	{
		nsEmbedString EValue;
		Element->GetValue( EValue );

		Transform( EValue, Value );

		return Value;
	}
/*
	template <typename t> inline const str::string_ &GetValue(
		_element__<t> &Element,
		str::string_ &Value )
	{
		return GetValue( Element.operator->(), Value );
	}
*/
	inline void SetAttribute(
		nsIDOMElement *Element,
		const char *Name,
		const str::string_ &Value )
	{
		nsEmbedString EName, EValue;

		Transform( Name, EName );
		Transform( Value, EValue );

#ifdef NSXPCM_DBG
		if ( Element == NULL )
			ERRu();
#endif
		
		Element->SetAttribute( EName, EValue );
	}

	template <typename t> inline void SetValue(
		t *Element,
		const str::string_ &Value )
	{
		nsEmbedString EValue;

		Transform( Value, EValue );

#ifdef NSXPCM_DBG
		if ( Element == NULL )
			ERRu();
#endif
		
		Element->SetValue( EValue );
	}

	inline void CloneNode(
		nsIDOMNode *Node,
		bso::bool__ Deep,
		nsIDOMNode **Clone )
	{
		Node->CloneNode( Deep, Clone );
	}

	inline void AppendChild(
		nsIDOMNode *Node,
		nsIDOMNode *Child )
	{
		nsIDOMNode *Dummy = NULL;

#ifdef NSXPCM_DBG
		if ( Node == NULL )
			ERRu();

		if ( Child == NULL )
			ERRu();
#endif
		Node->AppendChild( Child, &Dummy );

	}

	inline void InsertBefore(
		nsIDOMNode *Node,
		nsIDOMNode *Sibling )
	{
		nsIDOMNode *Parent = NULL;
		nsIDOMNode *&Dummy = Parent;

		Node->GetParentNode( &Parent );

#ifdef NSXPCM_DBG
		if ( Node == NULL )
			ERRu();

		if ( Sibling == NULL )
			ERRu();

		if ( Parent == NULL )
			ERRu();
#endif
		Parent->InsertBefore( Sibling, Node, &Dummy );

	}

	inline bso::bool__ HasAttributes( nsIDOMNode *Node )
	{
		PRBool Result;

#ifdef NSXPCM_DBG
		if ( Node == NULL )
			ERRu();
#endif

		Node->HasAttributes( &Result );

		return Result != 0;
	}

	inline nsIDOMNode *GetFirstChild( nsIDOMNode *Node )
	{
		nsIDOMNode *Child = NULL;

#ifdef NSXPCM_DBG
		if ( Node == NULL )
			ERRu();
#endif

		Node->GetFirstChild( &Child );

		return Child;
	}

	inline nsIDOMNode *GetLastChild( nsIDOMNode *Node )
	{
		nsIDOMNode *Child = NULL;

#ifdef NSXPCM_DBG
		if ( Node == NULL )
			ERRu();
#endif

		Node->GetLastChild( &Child );

		return Child;
	}

	inline nsIDOMNode *GetPreviousSibling( nsIDOMNode *Node )
	{
		nsIDOMNode *Child = NULL;

#ifdef NSXPCM_DBG
		if ( Node == NULL )
			ERRu();
#endif

		Node->GetPreviousSibling( &Child );

		return Child;
	}

	inline nsIDOMNode *GetNextSibling( nsIDOMNode *Node )
	{
		nsIDOMNode *Child = NULL;

#ifdef NSXPCM_DBG
		if ( Node == NULL )
			ERRu();
#endif

		Node->GetNextSibling( &Child );

		return Child;
	}

	inline nsIDOMNode *GetParentNode( nsIDOMNode *Node )
	{
		nsIDOMNode *Parent = NULL;

#ifdef NSXPCM_DBG
		if ( Node == NULL )
			ERRu();
#endif

		Node->GetParentNode( &Parent );

		return Parent;
	}

	inline void RemoveChild(
		nsIDOMNode *Node,
		nsIDOMNode *Child )
	{
		nsIDOMNode *Dummy = NULL;

#ifdef NSXPCM_DBG
		if ( Node == NULL )
			ERRu();

		if ( Child == NULL )
			ERRu();
#endif
		if ( Node->RemoveChild( Child, &Dummy ) != NS_OK )
			ERRu();
	}

	inline void RemoveChildren( nsIDOMNode *Node )
	{
#ifdef NSXPCM_DBG
		if ( Node == NULL )
			ERRu();
#endif

		while ( GetLastChild( Node ) != NULL )
			RemoveChild( Node, GetLastChild( Node ) );
	}

	inline const str::string_ &GetNodeName(
		nsIDOMNode *Node,
		str::string_ &Name )
	{
		nsEmbedString EName;

		Node->GetNodeName( EName );

		Transform( EName, Name );

		return Name;

	}

	template <typename element> bso::slong__ GetSelectedIndex( element *Element )
	{
		PRInt32 Index;

		if ( Element->GetSelectedIndex( &Index ) != NS_OK )
			ERRx();

		return Index;
	}

	template <typename element> inline bool GetChecked( element *Element )
	{
		PRBool Checked;

		if ( Element->GetChecked( &Checked ) != NS_OK )
			ERRx();

		return Checked != 0;
	}

	template <typename element> inline void SetChecked(
		element *Element,
		bso::bool__ State )
	{
		if ( Element->SetChecked( State ) != NS_OK )
			ERRx();
	}

	void RemoveListboxContent( listbox_ *Listbox );

	inline void SelectListboxItem(
		listbox_ *Listbox,
		listitem_ *Listitem )
	{
		PRInt32 Index = -1;
		nsIBoxObject *BoxObject = NULL;
		nsIListBoxObject *ListBoxObject = NULL;

		Listbox->GetBoxObject( &BoxObject );

		ListBoxObject = QueryInterface<nsIListBoxObject>( BoxObject );

		ListBoxObject->GetIndexOfItem( Listitem, &Index );
		ListBoxObject->ScrollToIndex( Index );

		Listbox->SelectItem( Listitem );
	}

	nsIDOMNode *_FindParent(
		nsIDOMNode *Node,
		const str::string_ &NodeName );

	template <typename element> inline element *FindParent(
		nsIDOMNode *Node,
		const str::string_ &NodeName )
	{
		Node = _FindParent( Node, NodeName );

		if ( Node != NULL )
			return QueryInterface<element>( Node );
		else
			return NULL;
	}

	typedef nsIDOMElement *nsIDOMElementPointer;

	class element_core__
	{
	private:
		nsIDOMElement *_Element;
	protected:
		virtual void NSXPCMOnCommand( void ) = 0;
		virtual void NSXPCMOnClick( void ) = 0;
		virtual void NSXPCMOnInput( void ) = 0;
		virtual void NSXPCMOnFocus( void ) = 0;
	public:
		void reset( bso::bool__ = true )
		{
			_Element = NULL;
		}
		element_core__( void )
		{
			reset( false );
		}
		virtual ~element_core__( void )
		{
			reset( );
		}
		void Init(
			nsIDOMElement *Element,
			void *UP )
		{
			if ( _Element != NULL )
				ERRu();

			_Element = Element;
		}
		E_RODISCLOSE__( nsIDOMElementPointer, Element );
		void Handle( const str::string_ &EventType )
		{
			if ( EventType == "command" )
				NSXPCMOnCommand();
			else if ( EventType == "input" )
				NSXPCMOnInput();
			else if ( EventType == "click" )
				NSXPCMOnClick();
			else if ( EventType == "focus" )
				NSXPCMOnFocus();
			else
				ERRl();
		}
	};

	E_ROW( row__ );

	typedef bch::E_BUNCH_( element_core__ * ) element_cores_;
	E_AUTO( element_cores );

	// Supprime (désalloue) tous les éléments stockés dans 'Cores'.
	void Delete( element_cores_ &Cores );

	void Handle( 
		nsIDOMEvent *Event,
		const element_cores_ &Cores );

	template <typename object> class _element__
	: public element_core__
	{
	public:
		object *GetObject( void )
		{
			return QueryInterface<object>( Element() );
		}
	};

	class button__
	: public _element__<nsIDOMElement>	// Devrait normalement être '<nsIButton>', mais l'inclusion de 'nsIButton.h' pose problème.
	{
	};

	class textbox__
	: public _element__<nsIDOMXULTextBoxElement>
	{
	public:
		void SetValue( const str::string_ &Value )
		{
			nsxpcm::SetValue( GetObject(), Value );
		}
		void GetValue( str::string_ &Value )
		{
			nsxpcm::GetValue( GetObject(), Value );
		}
	};

	class description__
	: public _element__<nsIDOMXULDescriptionElement>
	{
	public:
		void SetValue( const str::string_ &Value )
		{
			nsxpcm::SetValue( GetObject(), Value );
		}
		void GetValue( str::string_ &Value )
		{
			nsxpcm::GetValue( GetObject(), Value );
		}
	};

	class listitem__
	: public _element__<nsIDOMXULSelectControlItemElement>
	{};

	class listbox__
	: public _element__<nsIDOMXULMultiSelectControlElement>
	{};

	/* Retourne 'true' si un fichier a été sélectionné ('FileName' contient alors le fichier),
	'false' si 'Cancel' a été sélectionné. */
	bso::bool__ FileOpenDialogBox(
		nsIDOMWindow *Parent,
		const char *Title,
		str::string_ &FileName );

	/* Retourne 'true' si un fichier a été sélectionné ('FileName' contient alors le fichier),
	'false' si 'Cancel' a été sélectionné. */
	bso::bool__ FileSaveDialogBox(
		nsIDOMWindow *Parent,
		const char *Title,
		str::string_ &FileName );

	/* Retourne 'true' si un répertoire a été sélectionné ('DirectoryName' contient alors le répetoire),
	'false' si 'Cancel' a été sélectionné. */
	bso::bool__ DirectorySelectDialogBox(
		nsIDOMWindow *Parent,
		const char *Title,
		str::string_ &DirectoryName );


#ifdef NSXPCM__BKD
	void Convert(
		const strings_ &Items,
		bkdacc::ids32_ &Ids );

	void Convert(
		const strings_ &Items,
		bkdacc::ids16_ &Ids );

	void Convert(
		const strings_ &Items,
		bkdacc::ids8_ &Ids );

	void SplitAndConvert(
		const char *Joined,
		bso::char__ Separator,
		bkdacc::ids32_ &Ids );

	void SplitAndConvert(
		const char *Joined,
		bso::char__ Separator,
		bkdacc::ids16_ &Ids );

	void SplitAndConvert(
		const char *Joined,
		bso::char__ Separator,
		bkdacc::ids8_ &Ids );

	void ConvertAndJoin(
		const bkdacc::ids8_ &Ids,
		bso::char__ Separator,
		string_ &Joined );

	void ConvertJoinAndTransform(
		const bkdacc::ids8_ &Ids,
		bso::char__ Separator,
		char **JString );

	void ConvertAndJoin(
		const bkdacc::ids16_ &Ids,
		bso::char__ Separator,
		string_ &Joined );

	void ConvertJoinAndTransform(
		const bkdacc::ids16_ &Ids,
		bso::char__ Separator,
		char **JString );

	void ConvertAndJoin(
		const bkdacc::ids32_ &Ids,
		bso::char__ Separator,
		string_ &Joined );

	void ConvertJoinAndTransform(
		const bkdacc::ids32_ &Ids,
		bso::char__ Separator,
		char **JString );

	void ConvertAndJoin(
		const bkdacc::slongs_ &Ids,
		bso::char__ Separator,
		string_ &Joined );

	void ConvertJoinAndTransform(
		const bkdacc::slongs_ &Ids,
		bso::char__ Separator,
		char **JString );

	void ConvertAndJoin(
		const bkdacc::ulongs_ &Ids,
		bso::char__ Separator,
		string_ &Joined );

	void ConvertJoinAndTransform(
		const bkdacc::ulongs_ &Ids,
		bso::char__ Separator,
		char **JString );

	void ConvertAndJoin(
		const bkdacc::booleans_ &Booleans,
		bso::char__ Separator,
		string_ &Joined );

	void ConvertJoinAndTransform(
		const bkdacc::booleans_ &Booleans,
		bso::char__ Separator,
		char **JString );

	void Split(
		const char *Joined,
		const char *Amounts,
		bso::char__ Separator,
		bkdacc::xstrings_ &Strings );
#endif
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
