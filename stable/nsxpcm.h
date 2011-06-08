/*
	Header for the 'nsxpcm' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Copyright (C) 2004 Claude SIMON (http://zeusw.org/intl/contact.html).

	This file is part of the Epeios (http://zeusw.org/epeios/) project.

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 3
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

#define NSXPCM_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"

#include "ttr.h"

extern class ttr_tutor &NSXPCMTutor;

#if defined( XXX_DBG ) && !defined( NSXPCM_NODBG )
#define NSXPCM_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision$
//C Claude SIMON (http://zeusw.org/intl/contact.html)
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

#include "cpe.h"
#include "err.h"
#include "flw.h"
#include "epeios.h"
#include "str.h"
#include "ctn.h"
#include "lstbch.h"
#include "lcl.h"

#include "xpcom-config.h"

#include "nsITreeView.h"
#include "nsITreeContentView.h"
#include "nsITreeSelection.h"
#include "nsIListBoxObject.h"

#include "nsIDOMDocument.h"
#include "nsIDOMElement.h"
#include "nsIDOMXULMultSelectCntrlEl.h"
#include "nsIDOMXULSelectCntrlItemEl.h"
#include "nsIDOMXULTextboxElement.h"
#include "nsIDOMXULMenuListElement.h"
#include "nsIDOMXULCheckboxElement.h"
#include "nsIDOMXULButtonElement.h"
#include "nsIDOMXULTreeElement.h"
#include "nsIDOMXULDescriptionElement.h"
#include "nsIDOMWindowInternal.h"
#include "nsIDOMXULLabelElement.h"
#include "nsIDOMMutationEvent.h"
#include "nsIDOMKeyEvent.h"

#include "nsIXULWindow.h"

// #include "content/nsIXSLTProcessor.h"

#include "nsEmbedString.h"
#include "nsCOMPtr.h"
#include "nsComponentManagerUtils.h"
#include "nsServiceManagerUtils.h"
#include "nsIInterfaceRequestor.h"
#include "nsIDOMEventListener.h"
#include "nsIGenericFactory.h"
#include "nsIDOMHTMLAnchorElement.h"
#include "nsIDOMDocumentFragment.h"
#include "nsICommandLine.h" // Situé dans 'toolkitcomps'.

#include "nsICommandManager.h"

#undef NSXPCM__ENABLE_FORMHISTORY

#ifdef NSXPCM_FORCE_FORMHISTORY
#	define NSXPCM__ENABLE_FORMHISTORY
#endif

/*
#ifdef NSXPCM_FBL
#	define NSXPCM__FBL
#endif

#ifdef NSXPCM__FBL
#	include "fblfrd.h"
#endif
*/
#if defined( __ARMEL__ ) || defined( NSXPCM_USE_ARMEL_WORKAROUND )
#	ifndef NSXPCM_NO_ARMEL_WORKAROUND
#		define NSXPCM__USE_ARMEL_WORKAROUND
#	endif
#endif

#define NSXPCM_EVENT_LISTENER_IID_STR "d333cd20-c453-11dd-ad8b-0800200c9a66"

#define NSXPCM_IEVENT_LISTENER_IID \
  {0xd333cd20, 0xc453, 0x11dd, \
    { 0xad, 0x8b, 0x08, 0x00, 0x20, 0x0c, 0x9a, 0x66 }}

// Permet de 'logger' une erreur et d'éviter qu'elle soit ne remontée à 'XULRunner', qui ne saurait pas quoi en faire. A placer dans 'ERRErr'.
#define NSXPCM_ERR( window )\
	if ( ERRMajor == err::itn ) {\
		if ( ERRMinor == err::iExit )\
			nsxpcm::Close( window );\
		else if ( ERRMinor != err::iReturn ) {\
			err::buffer__ Buffer;\
			nsxpcm::Log( err::Message( Buffer ) );\
			nsxpcm::GetJSConsole();\
		}\
	} else {\
		err::buffer__ Buffer;\
		nsxpcm::Log( err::Message( Buffer ) );\
		nsxpcm::GetJSConsole();\
	}\
	ERRRst()


namespace nsxpcm {
	using str::string_;
	using str::string;

	// Rajoute une fenêtre pour pouvant être utilisé pour faciliter l'utilisation
	// de certaines fonctions ('GetJSConsole(...)', 'Alert(...)', ...).
	void AddMasterWindow( nsIDOMWindow *Window );

	// Retire 'Window' de la liste des 'MasterWindow's
	void RemoveMasterWindow( nsIDOMWindow *Window );

	// Retourne un 'MasterWindows'. 'RealeaseWindow' doit être appelé dés que la 'Window' retournée n'est plus utilisée.
	// Retourne 'NULL" si pas de fenêtre disponible.
	nsIDOMWindow *RetrieveMasterWindow( void );

	void ReleaseMasterWindow( nsIDOMWindow *Window );

	// Log to the javascript console.
	void Log( const str::string_ &Text );

	// Log to the javascript console.
	inline void Log( const char *Text )
	{
		Log( str::string( Text ) );
	}


	// Si modifié, modifier 'GetTranslation(...)' et le '.xlcl' en conséquence.
	enum text__ {
		tXPRJFilterLabel,
		t_amount,
		t_Undefined
	};

	bso::bool__ GetTranslation(
		text__ Text,
		const lcl::rack__ &LocaleRack,
		str::string_ &Translation );

	const char *GetTranslation(
		text__ Text,
		const lcl::rack__ &LocaleRack,
		STR_BUFFER___ &Buffer  );

	enum event__
	{
		eCommand,
		eInput,
		eClick,
		eDblClick,
		eFocus,
		eBlur,
		eSelect,
		eAttributeChange,
		eKeyPress,
		eClose,
		e_amount,
		e_Undefined
	};

#ifdef EF
#	define NSXPCM__EF_BUFFER	EF
#	undef EF
#endif

#define EF( name )	ef##name = ( 1 << e##name )

	enum event_flag__ {
		EF( Command ),
		EF( Input ),
		EF( Click ),
		EF( DblClick ),
		EF( Focus ),
		EF( Blur ),
		EF( Select ),
		EF( AttributeChange ),
		EF( KeyPress ),
		EF( Close ),
		ef_None = 0,
		ef_All = ( ( 1 << e_amount ) - 1 ),
		ef_AllButAnnoying = ef_All & ~efAttributeChange & ~efBlur & ~efFocus	// Pour faciliter le déboguage.
	};

#undef EF

#ifdef NSXPCM__EF_BUFFER
#	define	EF	NSXPCM__EF_BUFFER
#endif


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

	inline void Transform(
		const char *String,
		nsEmbedCString &ECString )
	{
		Transform( str::string( String ), ECString );
	}

	void Split( 
		const string_ &Joined,
		bso::char__ Separator,
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

#ifdef T
#	define NSXPCM__T_BUFFER T
#	undef T
#endif

	inline bso::bool__ _T( nsresult NSResult )
	{
		if ( NSResult != NS_OK )
			return false;
		else
			return true;
	}


#define T( f )\
		if ( !_T( f ) )\
			ERRx()

	// 'ContractID' est une chaîne de caratère du genre "@mozilla.org/filepicker;1".
	template <typename t> inline t *GetService(
		const char *ContractID,
		nsCOMPtr<t> &Service )
	{	
		nsresult Error = NS_OK;

		Service = do_GetService( ContractID, &Error );

		T( Error );

		return Service;
	}

	// 'ContractID' est une chaîne de caratère du genre "@mozilla.org/filepicker;1".
	template <typename t> inline t *CreateInstance(
		const char *ContractID,
		nsCOMPtr<t> &Instance )
	{	
		nsresult Error = NS_OK;

		Instance = do_CreateInstance( ContractID, &Error );

		T( Error  );

		return Instance;
	}

	inline nsIDOMElement *GetElementById(
		nsIDOMDocument *Document,
		const char *Id )
	{
		nsIDOMElement *Element = NULL;
		nsEmbedString EId;

		Transform( Id, EId );

		T( Document->GetElementById( EId, &Element ) );

		return Element;
	}

	inline nsIDOMElement *GetElementById(
		nsIDOMDocument *Document,
		const str::string_ &Id )
	{
		nsIDOMElement *Element = NULL;
		nsEmbedString EId;

		Transform( Id, EId );

		T( Document->GetElementById( EId, &Element ) );

		return Element;
	}

	template <typename element> inline element *QueryInterface( nsISupports *GenericElement )
	{
		element *Element = NULL;

#ifdef NSXPCM_DBG
		if ( GenericElement == NULL )
			ERRu();
#endif

		T( GenericElement->QueryInterface( element::GetIID(), (void **)&Element ) );

		return Element;
	}

	template <typename element> inline element *_GetInterface( nsISupports *GenericElement )
	{
		nsCOMPtr<nsIInterfaceRequestor> Requestor;
		element *Element = NULL;

#ifdef NSXPCM_DBG
		if ( GenericElement == NULL )
			ERRu();
#endif

		Requestor = QueryInterface<nsIInterfaceRequestor>( GenericElement );


		T( Requestor->GetInterface( element::GetIID(), (void **)&Element ) );

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

		T( Document->CreateElement( EName, &Element ) );

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
		inline name *Create##Name( nsIDOMDocument *Document )\
		{\
			return _CreateElement< name >( Document, #name );\
		}


	NSXPCM_DEFINE( nsIDOMXULMultiSelectControlElement, listbox, Listbox )
	NSXPCM_DEFINE( nsIDOMXULSelectControlItemElement, listitem, Listitem )
	NSXPCM_DEFINE( nsIDOMXULTextBoxElement, textbox, Textbox )
	NSXPCM_DEFINE( nsIDOMXULMenuListElement, menulist, Menulist )
	NSXPCM_DEFINE( nsIDOMXULCheckboxElement, checkbox, Checkbox )
	NSXPCM_DEFINE( nsIDOMXULTreeElement, tree, Tree )
	NSXPCM_DEFINE( nsIDOMXULLabelElement, label, Label );
	NSXPCM_DEFINE( nsIDOMHTMLAnchorElement, html_anchor, HTMLAnchor );


	inline bso::bool__ HasAttribute(
		nsIDOMElement *Element,
		const char *Name )
	{
		nsEmbedString EName;
		PRBool Result;

		Transform( Name, EName );

#ifdef NSXPCM_DBG
		if ( Element == NULL )
			ERRu();
#endif
		
		T( Element->HasAttribute( EName, &Result ) );

		return Result != 0;
	}

	inline bso::bool__ HasAttribute(
		nsIDOMNode *Node,
		const char *Name )
	{
		return HasAttribute( QueryInterface<nsIDOMElement>( Node ), Name );
	}

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
		
		T( Element->SetAttribute( EName, EValue ) );
	}

	inline void SetAttribute(
		nsIDOMNode *Node,
		const char *Name,
		const char *Value )
	{
		SetAttribute( QueryInterface<nsIDOMElement>( Node ), Name, Value );
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
		
		T( Element->RemoveAttribute( EName ) );
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
		
		T( Element->SetValue( EValue ) );
	}

	inline const str::string_ &GetAttribute(
		nsIDOMElement *Element,
		const char *Name,
		str::string_ &Value )
	{
		nsEmbedString EName, EValue;
		Transform( Name, EName );

		T( Element->GetAttribute( EName, EValue ) );

		Transform( EValue, Value );

		return Value;
	}

	inline const str::string_ &GetAttribute(
		nsIDOMNode *Node,
		const char *Name,
		str::string_ &Value )
	{
 		nsIDOMElement *Element = QueryInterface<nsIDOMElement>( Node );

		if ( Element != NULL )
			return GetAttribute( Element, Name, Value );
		else
			return Value;
	}

	template <typename t> inline const str::string_ &GetValue(
		t *Element,
		str::string_ &Value )
	{
		nsEmbedString EValue;

		T( Element->GetValue( EValue ) );

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
		
		T( Element->SetAttribute( EName, EValue ) );
	}

	inline void SetAttribute(
		nsIDOMNode *Node,
		const char *Name,
		const str::string_ &Value )
	{
		SetAttribute( QueryInterface<nsIDOMElement>( Node ), Name, Value );
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
		
		T( Element->SetValue( EValue ) );
	}

	inline void CloneNode(
		nsIDOMNode *Node,
		bso::bool__ Deep,
		nsIDOMNode **Clone )
	{
		T( Node->CloneNode( Deep, Clone ) );
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
		T( Node->AppendChild( Child, &Dummy ) );
	}

	inline void InsertBefore(
		nsIDOMNode *Node,
		nsIDOMNode *Sibling )
	{
		nsIDOMNode *Parent = NULL;
		nsIDOMNode *&Dummy = Parent;

		T( Node->GetParentNode( &Parent ) );

#ifdef NSXPCM_DBG
		if ( Node == NULL )
			ERRu();

		if ( Sibling == NULL )
			ERRu();

		if ( Parent == NULL )
			ERRu();
#endif
		T( Parent->InsertBefore( Sibling, Node, &Dummy ) );
	}

	inline bso::bool__ HasAttributes( nsIDOMNode *Node )
	{
		PRBool Result;

#ifdef NSXPCM_DBG
		if ( Node == NULL )
			ERRu();
#endif

		T( Node->HasAttributes( &Result ) );

		return Result != 0;
	}

	inline nsIDOMNode *GetFirstChild( nsIDOMNode *Node )
	{
		nsIDOMNode *Child = NULL;

#ifdef NSXPCM_DBG
		if ( Node == NULL )
			ERRu();
#endif

		T( Node->GetFirstChild( &Child ) );

		return Child;
	}

	inline nsIDOMNode *GetLastChild( nsIDOMNode *Node )
	{
		nsIDOMNode *Child = NULL;

#ifdef NSXPCM_DBG
		if ( Node == NULL )
			ERRu();
#endif

		T( Node->GetLastChild( &Child ) );

		return Child;
	}

	inline nsIDOMNode *GetPreviousSibling( nsIDOMNode *Node )
	{
		nsIDOMNode *Child = NULL;

#ifdef NSXPCM_DBG
		if ( Node == NULL )
			ERRu();
#endif

		T( Node->GetPreviousSibling( &Child ) );

		return Child;
	}

	inline nsIDOMNode *GetNextSibling( nsIDOMNode *Node )
	{
		nsIDOMNode *Child = NULL;

#ifdef NSXPCM_DBG
		if ( Node == NULL )
			ERRu();
#endif

		T( Node->GetNextSibling( &Child ) );

		return Child;
	}

	inline nsIDOMNode *GetParentNode( nsIDOMNode *Node )
	{
		nsIDOMNode *Parent = NULL;

#ifdef NSXPCM_DBG
		if ( Node == NULL )
			ERRu();
#endif

		T( Node->GetParentNode( &Parent ) );

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
		T( Node->RemoveChild( Child, &Dummy ) );
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

		T( Node->GetNodeName( EName ) );

		Transform( EName, Name );

		return Name;

	}

	// Permet de parcourir une arborescence de noeuds.
	class browser__
	{
	private:
		nsIDOMNode *_Root, *_Current;
	public:
		void reset( bso::bool__ = true )
		{
			_Root = _Current = NULL;
		}
		browser__( void )
		{
			reset( false );
		}
		~browser__( void )
		{
			reset();
		}
		void Init( nsIDOMNode *Root )
		{
			reset();

			_Root = Root;
			_Current = _Root;
		}
		// Retourne le prochain noeud, ou 'NULL' si tous ont été lus.
		nsIDOMNode *GetNext( void );
	};

	template <typename element> bso::slong__ GetSelectedIndex( element *Element )
	{
		PRInt32 Index;

		T( Element->GetSelectedIndex( &Index ) );

		return Index;
	}

	template <typename element> inline bool IsChecked( element *Element )
	{
		PRBool Checked;

		T( Element->GetChecked( &Checked ) );

		return Checked != 0;
	}

	template <typename element> inline void SetChecked(
		element *Element,
		bso::bool__ State )
	{
		T( Element->SetChecked( State ) );
	}

	void RemoveListboxContent( listbox_ *Listbox );

	inline void SelectListboxItem(
		listbox_ *Listbox,
		listitem_ *Listitem )
	{
		PRInt32 Index = -1;
		nsIBoxObject *BoxObject = NULL;
		nsIListBoxObject *ListBoxObject = NULL;

		T( Listbox->GetBoxObject( &BoxObject ) );

		ListBoxObject = QueryInterface<nsIListBoxObject>( BoxObject );

		T( ListBoxObject->GetIndexOfItem( Listitem, &Index ) );
		T( ListBoxObject->ScrollToIndex( Index ) );

		T( Listbox->SelectItem( Listitem ) );
	}

	nsIDOMNode *_FindParent(
		nsIDOMNode *Node,
		const str::string_ &NodeName );

	inline nsIDOMElement *GetDocumentElement( nsIDOMDocument *Document )
	{
		nsIDOMElement *Element = NULL;

		T( Document->GetDocumentElement( &Element ) );

		return Element;
	}

	inline nsIDOMDocument *GetWindowDocument( nsIDOMWindow *Window )
	{
		nsIDOMDocument *Document = NULL;

		T( Window->GetDocument( &Document ) );

		return Document;
	}

	inline nsIDOMElement *GetWindowElement( nsIDOMWindow *Window )
	{
		return GetDocumentElement( GetWindowDocument( Window ) );
	}

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

		inline bso::bool__ IsClosed( nsIDOMWindowInternal *Window )
	{
		PRBool Value = false;

		T( Window->GetClosed( &Value ) );

		return Value == PR_TRUE;
	}

	inline void Close( nsIDOMWindowInternal *Window )
	{
		T( Window->Close() );
	}

	inline nsIDOMWindowInternal *GetWindowInternal( nsIDOMWindow *Window )
	{
#ifdef NSXPCM__USE_ARMEL_WORKAROUND
		return (nsIDOMWindowInternal *)Window;
#else
		return nsxpcm::QueryInterface<nsIDOMWindowInternal>( Window );
#endif
	}

	inline void Close( nsIDOMWindow *Window )
	{
		Close( GetWindowInternal( Window ) );
	}
/*
	inline void Close( void )
	{
		if ( GetMasterWindow() == NULL )
			ERRu();

		Close( GetMasterWindow() );
	}
*/

	inline bso::bool__ IsClosed( nsIDOMWindow *Window )
	{
		return IsClosed( GetWindowInternal( Window ) );
	}

	typedef nsISupports *nsISupportsPointer;
	typedef nsIDOMElement *nsIDOMElementPointer;
	typedef nsIDOMWindow *nsIDOMWindowPointer;

	struct ui_core__ {
		nsIDOMDocument *Document;
		nsIDOMWindow *Window;
		ui_core__( void )
		{
			Document = NULL;
			Window = NULL;
		}
		void Init( nsIDOMWindow *Window )
		{
			this->Window = Window;
			this->Document = GetWindowDocument( Window );
		}
	};


	typedef bso::slong__ event_imbrication_level__;
#define NSXPCM__EVENT_IMBRICATION_LEVEL_MAX	BSO_SLONG_MAX

	class event_data__
	{
	private:
		nsCOMPtr<struct event_listener> _EventListener;
		nsIDOMEvent *_RawEvent;
		nsIDOMMutationEvent *_MutationEvent;
		nsIDOMKeyEvent *_KeyEvent;
		event_imbrication_level__ _EventImbricationLevel;
		int _EventsToIgnore;
	public:
		void reset( bso::bool__ P = true )
		{
			_RawEvent = NULL;
			_MutationEvent = NULL;
			_KeyEvent = NULL;
			_EventImbricationLevel = -1;
			_EventsToIgnore = ef_None;
		}
		event_data__( void )
		{
			reset( false );
		}
		~event_data__( void )
		{
			reset();
		}
		void Init( void )
		{
			reset();
		}
		nsIDOMEvent &RawEvent( void )
		{
			return *_RawEvent;
		}
		nsIDOMMutationEvent &MutationEvent( void )
		{
			return *_MutationEvent;
		}
		nsIDOMKeyEvent &KeyEvent( void )
		{
			return *_KeyEvent;
		}
		E_RODISCLOSE__( event_imbrication_level__, EventImbricationLevel );
		bso::bool__ IsEventImbricated( void ) const
		{
			return _EventImbricationLevel != 0;
		}
		void EventStopPropagation( void )
		{
			if ( _RawEvent == NULL )
				ERRu();

			_RawEvent->StopPropagation();
		}
		void EventPreventDefault( void )
		{
			if ( _RawEvent == NULL )
				ERRu();

			_RawEvent->PreventDefault();
		}
		int SetEventsToIgnore( int Events )
		{
			int Buffer = _EventsToIgnore;

			_EventsToIgnore = Events;

			return Buffer;
		}
		int AddEventsToIgnore( int Events )
		{
			int Buffer = _EventsToIgnore;

			_EventsToIgnore |= Events;

			return Buffer;
		}
		friend class widget_core__;
	};

	class _event_handler__ {
	protected:
		virtual void NSXPCMOnEvent(
			event_data__ &,
			event__ Event )
		{
			NSXPCMOnEvent( Event );
		}
		virtual void NSXPCMOnEvent(
			event__ Event )
		{
			ERRu();
		}
	public:
		void reset( bso::bool__ = true )
		{
			// A des fins de standardisation.
		}
		_event_handler__( void )
		{
			reset( false );
		}
		~_event_handler__( void )
		{
			reset();
		}
		void Init( void )
		{
			// A des fins de standardisation.
		}
		void OnEvent(
			event_data__ &Data,
			event__ Event )
		{
			NSXPCMOnEvent( Data, Event );
		}
	};

	typedef _event_handler__ event_handler__;

	class widget_core__
	{
	private:
		nsIDOMWindow *_Window;
		nsISupports *_Supports;
		event_data__ _EventData;
		_event_handler__ *_EventHandler;	// Si défini, est prioritaire sur le traitement des évènements défini par surcharge.
	protected:
		virtual void NSXPCMOnEvent( event__ Event )
		{
			ERRu();
		}
		event_data__ &EventData( void )
		{
			return _EventData;
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_Window = NULL;
			_Supports = NULL;
			_EventData.reset( P );
			_EventHandler = NULL;
		}
		widget_core__( void )
		{
			reset( false );
		}
		virtual ~widget_core__( void )
		{
			reset( );
		}
		void Init( void )
		{
			reset();
			_EventData.Init();
		}
		void Init(
			nsISupports *Supports,
			nsIDOMWindow *Window,
			int Events );
		void SupersedEventHandling( _event_handler__ &EventHandler )
		{
			_EventHandler = &EventHandler;
		}
		E_RODISCLOSE__( nsISupportsPointer, Supports );
		E_RODISCLOSE__( nsIDOMWindowPointer, Window );
		bso::bool__ Handle( nsIDOMEvent *Event );
		nsIDOMElement *GetElement( void )
		{
			return QueryInterface<nsIDOMElement>( _Supports );
		}
		int SetEventsToIgnore( int Events )
		{
			return _EventData.SetEventsToIgnore( Events );
		}
		int AddEventsToIgnore( int Events )
		{
			return _EventData.AddEventsToIgnore( Events );
		}
	};

	E_ROW( row__ );

	inline void Register(
		nsxpcm::widget_core__ &Widget,
		nsISupports *Supports,
		nsIDOMWindow *Window,
		int Events )
	{
		Widget.Init( Supports, Window, Events );
	}

	template <class id_type> inline void Register(
		nsxpcm::widget_core__ &Widget,
		nsIDOMWindow *Window,
		const id_type &Id,
		int Events )
	{
		Register( Widget, nsxpcm::GetElementById( GetWindowDocument( Window ), Id ), Window, Events );
	}

	void Alert(
		nsIDOMWindow *Window,
		const char *Text );

	inline void Alert( const char *Text )
	{
	ERRProlog
		nsIDOMWindow *MasterWindow = NULL;
	ERRBegin
		if ( ( MasterWindow = RetrieveMasterWindow() ) != NULL )
			Alert( MasterWindow, Text );
	ERRErr
	ERREnd
		if ( MasterWindow != NULL )
			ReleaseMasterWindow( MasterWindow );
	ERREpilog
	}

	void Alert(
		nsIDOMWindow *Window,
		const str::string_ &Text );

	inline void Alert( const str::string_ &Text )
	{
	ERRProlog
		nsIDOMWindow *MasterWindow = NULL;
	ERRBegin
		if ( ( MasterWindow = RetrieveMasterWindow() ) != NULL )
			Alert( MasterWindow, Text );
	ERRErr
	ERREnd
		if ( MasterWindow != NULL )
			ReleaseMasterWindow( MasterWindow );
	ERREpilog
	}

	bso::bool__ Confirm(
		nsIDOMWindow *Window,
		const char *Text );

	bso::bool__ Confirm(
		nsIDOMWindow *Window,
		const str::string_ &Text );

	typedef bch::E_BUNCH_( widget_core__ * ) widget_cores_;
	E_AUTO( widget_cores );

	// Supprime (désalloue) tous les éléments stockés dans 'Cores'.
	void Delete( widget_cores_ &Widgets );
/*
	void Handle( 
		nsIDOMEvent *Event,
		const element_cores_ &Cores );
*/
	template <typename object> class _widget__
	: public widget_core__
	{
	public:
		object *GetObject( void )
		{
			return QueryInterface<object>( GetSupports() );
		}
		const str::string_ &GetAttribute(
			const str::string_ &Name,
			str::string_ &Value )
		{
			return nsxpcm::GetAttribute( GetObject(), Name, Value );
		}
		const str::string_ &GetAttribute(
			const char *Name,
			str::string_ &Value )
		{
			return nsxpcm::GetAttribute( GetObject(), Name, Value );
		}
		void SetAttribute(
			const str::string_ &Name,
			str::string_ &Value )
		{
			nsxpcm::SetAttribute( GetObject(), Name, Value );
		}
		void SetAttribute(
			const char *Name,
			str::string_ &Value )
		{
			nsxpcm::SetAttribute( GetObject(), Name, Value );
		}
		void SetAttribute(
			const char *Name,
			const char *Value )
		{
			nsxpcm::SetAttribute( GetObject(), Name, Value );
		}
		void RemoveAttribute( const str::string_ &Name )
		{
			nsxpcm::RemoveAttribute( GetObject(), Name );
		}
		void RemoveAttribute( const char *Name )
		{
			nsxpcm::RemoveAttribute( GetObject(), Name );
		}
		const str::string_ &GetId( str::string_ &Value )
		{
			return GetAttribute( "id", Value );
		}
		void Enable( bso::bool__ Value = true )
		{
			if ( Value )
				RemoveAttribute( "disabled" );
			else
				SetAttribute( "disabled", "true" );
		}
		void Disable( bso::bool__ Value = true )
		{
			Enable( !Value );
		}
		bso::bool__ IsEnabled( void )
		{
			bso::bool__ Enabled = false;
		ERRProlog
			str::string Value;
		ERRBegin
			Value.Init();

			GetAttribute( "disabled", Value );

			Enabled = Value != "true";
		ERRErr
		ERREnd
		ERREpilog
			return Enabled;
		}
		bso::bool__ IsDisabled( void )
		{
			return !IsEnabled();
		}
		void Show( bso::bool__ Value = true )
		{
			nsxpcm::SetAttribute( GetObject(), "hidden", Value ? "false" : "true" );
		}
		void Hide( void )
		{
			Show( false );
		}
		void SetLabel( str::string_ &Label )
		{
			SetAttribute( "label", Label );
		}
		void SetLabel( const char *Label )
		{
			SetAttribute( "label", Label );
		}
		void RemoveChildren( void )
		{
			nsxpcm::RemoveChildren( GetObject() );
		}
		void AppendChild( nsIDOMNode *Node )
		{
			nsxpcm::AppendChild( GetObject(), Node );
		}
	};

	class widget__	// Classe générique.
	: public _widget__<nsIDOMElement>
	{};

	class button__
	: public _widget__<nsIDOMElement>	// Devrait normalement être '<nsIButton>', mais l'inclusion de 'nsIButton.h' pose problème.
	{};

	class textbox__
	: public _widget__<nsIDOMXULTextBoxElement>
	{
	public:
		void SetValue( const str::string_ &Value )
		{
			nsxpcm::SetValue( GetObject(), Value );
		}
		const str::string_ &GetValue( str::string_ &Value )
		{
			nsxpcm::GetValue( GetObject(), Value );

			return Value;
		}
		void Select( void )
		{
			T( GetObject()->Select() );
		}
		void SetSize( bso::size__ Size )
		{
			T( GetObject()->SetSize( Size ) );
		}
		bso::size__ GetSize( void )
		{
			PRInt32 Buffer;

			T( GetObject()->GetSize( &Buffer )  );

			return Buffer;
		}
		void SetMaxLength( bso::size__ Size )
		{
			T( GetObject()->SetMaxLength( Size ) );
		}
		bso::size__ GetMaxLength( void )
		{
			PRInt32 Buffer;

			T( GetObject()->GetMaxLength( &Buffer ) );

			return Buffer;
		}
	};

	class description__
	: public _widget__<nsIDOMXULDescriptionElement>
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

	class label__
	: public _widget__<nsIDOMXULLabelElement>
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

	class checkbox__
	: public _widget__<nsIDOMXULCheckboxElement>
	{
	public:
		bso::bool__ IsChecked( void )
		{
			return nsxpcm::IsChecked( GetObject() );
		}
	};

	class radio__
	: public _widget__<nsIDOMXULButtonElement>
	{
	public:
		bso::bool__ IsChecked( void )
		{
			return nsxpcm::IsChecked( GetObject() );
		}
	};

	class listitem__
	: public _widget__<nsIDOMXULSelectControlItemElement>
	{};

	class listbox__
	: public _widget__<nsIDOMXULMultiSelectControlElement>
	{
	private:
	public:
		nsIDOMXULSelectControlItemElement *GetCurrentItem( bso::bool__ ErrorIfInexistant = true )
		{
			nsIDOMXULSelectControlItemElement *Item = NULL;

			T( GetObject()->GetCurrentItem( &Item ) );

			if ( ( Item == NULL ) && ( ErrorIfInexistant ) )
				ERRu();

			return Item;
		}
		const str::string_ &GetCurrentItemAttribute(
			const char *Name,
			str::string_ &Value )
		{
			return nsxpcm::GetAttribute( GetCurrentItem( true ), Name, Value );
		}
		const str::string_ &GetCurrentItemId( str::string_ &Value )
		{
			return GetCurrentItemAttribute( "id", Value );
		}
		bso::slong__ GetSelectedCount( void )
		{
			PRInt32 Count = 0;

			T( GetObject()->GetSelectedCount( &Count ) );

			return Count;
		}
	};

	class panel__
	: public _widget__<nsIDOMElement>
	{};

	class deck__
	: public _widget__<nsIDOMElement>
	{
	private:
	public:
		bso::slong__ GetSelectedIndex( void )
		{
			bso::slong__ Index = -1;
		ERRProlog
			str::string Value;
			epeios::row__ Error = NONE;
		ERRBegin
			Value.Init();
			nsxpcm::GetAttribute( GetObject(), "selectedIndex", Value );

			Index = Value.ToSL( &Error );

			if ( Error != NONE )
				ERRs();
		ERRErr
		ERREnd
		ERREpilog
			return Index;
		}
		void SetSelectedIndex( bso::slong__ Index )
		{
			bso::integer_buffer__ Buffer;

			nsxpcm::SetAttribute( GetObject(), "selectedIndex", bso::Convert( Index, Buffer ) );
		}
		void SetSelectedPanel( nsIDOMNode *Node )
		{
			bso::slong__ Index = -1;

			while ( Node != NULL ) {
				Index++;
				Node = GetPreviousSibling( Node );
			}

			SetSelectedIndex( Index );
		}
		void SetSelectedPanel( panel__ &Panel )
		{
			SetSelectedPanel( Panel.GetElement() );
		}
	};


	class tree__
	: public _widget__<nsIDOMXULTreeElement>
	{
	private:
		nsITreeView *_GetView( void )
		{
			nsITreeView *View = NULL;

			T( GetObject()->GetView( &View ) );

			if ( View == NULL )
				ERRu();

			return View;
		}
		nsITreeSelection *_GetSelection( void )
		{
			nsITreeSelection *Selection = NULL;

			T( _GetView()->GetSelection( &Selection ) );

			if ( Selection == NULL )
				ERRu();

			return Selection;
		}
		nsITreeContentView *_GetContentView( void )
		{
			return nsxpcm::QueryInterface<nsITreeContentView>( _GetView() );
		}
	public:
		bso::slong__ GetCurrentIndex( void )
		{
			PRInt32 Count = 0;

			T( _GetSelection()->GetCurrentIndex( &Count ) );

			return Count;
		}
		void Select( bso::slong__ Index )
		{
//			_GetSelection()->SetCurrentIndex( Index );
			T( _GetSelection()->Select( Index ) );
		}
		nsIDOMElement *GetCurrentItem( bso::bool__ ErrorIfInexistent = true )
		{
			nsIDOMElement *Element = NULL;

			T( _GetContentView()->GetItemAtIndex( GetCurrentIndex(), &Element ) );

			if ( ( Element == NULL ) && ErrorIfInexistent )
				ERRu();

			return Element;
		}
		void Select( nsIDOMElement *Element )
		{
			PRInt32 Index = -1;

			T( _GetContentView()->GetIndexOfItem( Element, &Index ) );

			Select( Index );
		}
		void Select( nsIDOMNode *Node )
		{
			Select( QueryInterface<nsIDOMElement>( Node ) );
		}
		const str::string_ &GetCurrentItemAttribute(
			const char *Name,
			str::string_ &Value )
		{
			return nsxpcm::GetAttribute( GetCurrentItem( true ), Name, Value );
		}
		const str::string_ &GetCurrentItemId( str::string_ &Value )
		{
			return GetCurrentItemAttribute( "id", Value );
		}
		bso::bool__ IsThereSelected( void )
		{
			return GetCurrentIndex() != -1;
		}
		/* Par défaut (avec 'SkipSelectEvent' à false, un 'ClearSelection' lance un évènement 'select'.
		Cela peut être évité en mettant 'SkipSelectEvent' à 'true'. */
		void ClearSelection( bso::bool__ SkipSelectEvent )
		{
			int Buffer = AddEventsToIgnore( SkipSelectEvent ? efSelect : ef_None );

			T( _GetSelection()->ClearSelection() );

			SetEventsToIgnore( Buffer );
		}
	};

	class broadcast__
	: public _widget__<nsIDOMElement>
	{
	public:
		void Enable( bso::bool__ = true )
		{
			ERRu();
			// L'attribut 'disable' d'un 'broadcaster' lorsque mis à 'true' n'est pas repercuté sur les éléments qui 'observe'.
			// Lancer 'Disable()/Enable()' sue le 'command' correspondant.
		}
		void Disable( bso::bool__ Value = true )
		{
			Enable( !Value );
		}
	};

	class command__
	: public _widget__<nsIDOMElement>
	{};


	class html_anchor__
	: public _widget__<nsIDOMHTMLAnchorElement>
	{};

	class menu__
	: public _widget__<nsIDOMElement>	// Pas cherché le 'nsI...' correspondant ...
	{};

	class menu_item__
	: public _widget__<nsIDOMElement>	// Pas trouvé le 'nsI...' correspondant ...
	{};
/*
	class document__
	: public _widget__<nsIDOMDocument>
	{};
*/
	class window__
	: public _widget__<nsIDOMWindow>
	{
	public:
		void Close( void )
		{
			nsxpcm::Close( GetObject() );
		}
		bso::bool__ IsClosed( void )
		{
			return nsxpcm::IsClosed( GetObject() );
		}
		nsIDOMWindowInternal *GetInternal( void )
		{
			return nsxpcm::GetWindowInternal( GetObject() );
		}
	};

	// Un 'GetElementById(...)' d'une balise 'window' ne retourne pas un 'nsIDOMWindow'
	// Cette fonction a pour donc but d'empêcher toute tentative dans ce sens.
	template <class id_type> inline void Register(
		window__ &Widget,
		nsIDOMWindow *Window,
		const id_type &Id,
		int Events )
	{
		ERRl();	
		Register( (widget_core__)Widget, Window, Id, Events );	// Pour détecter tout changement de la signature de la fonction dont on est censé interdire l'accés.
	}

	class file_picker_filter_ 
	{
	public:
		struct s {
			str::string_::s
				Title,
				Mask;
		};
		str::string_
			Title,
			Mask;
		file_picker_filter_( s &S )
			: Title( S.Title ),
			  Mask( S.Mask )
		{}
		void reset( bso::bool__ P = true  )
		{
			Title.reset( P );
			Mask.reset( P );
		}
		void plug( mmm::E_MULTIMEMORY_ &MM )
		{
			Title.plug( MM );
			Mask.plug( MM );
		}
		file_picker_filter_ &operator =( const file_picker_filter_ &FPF )
		{
			Title = FPF.Title;
			Mask = FPF.Mask;

			return *this;
		}
		void Init( void )
		{
			Title.Init();
			Mask.Init();
		}
	};

	E_AUTO( file_picker_filter );

	typedef ctn::E_XCONTAINER_( file_picker_filter_ ) file_picker_filters_;
	E_AUTO( file_picker_filters );

	enum file_picker_type__
	{
		fptOpen,
		fptSave,
		fptFolder,
		fpt_amount,
		fpt_Undefined
	};

	enum file_picker_mask__ {
		fpmAll,	// '*.*'.
		fpmHTML,
		fpmText,
		fpmImages,
		fpmXML,
		fpmXUL,
		fpmApps,
		fpmXPRJ,
		fpm_amount,
		fpm_Undefined
	};

#ifdef MF
#	define NSXPCM__MF_BUFFER	MF
#	undef MF
#endif

#define MF( name )	fpmf##name = ( 1 << fpm##name )

	enum file_picker_mask_flag__ {
		MF( All ),
		MF( HTML ),
		MF( Text ),
		MF( Images ),
		MF( XML ),
		MF( XUL ),
		MF( Apps ),
		MF( XPRJ ),
		fpmf_None = 0,
		fpmf_All = ( ( 1 << fpm_amount ) - 1 )	// Ne pas confondre avec 'fpmfAll', désignat que l'on désire tous les fichiers (filtre '*.*').
	};
	
#undef MF

#ifdef NSXPCM__MF_BUFFER
#	define	MF	NSXPCM__MF_BUFFER
#endif

	class file_picker_
	{
	public:
		struct s {
			str::string_::s Title;
			file_picker_filters_::s Filters;
			int PredefinedFilters;
		} &S_;
		str::string_ Title;
		file_picker_filters_ Filters;
		file_picker_( s &S )
		: S_( S ),
		  Title( S.Title ),
		  Filters( S.Filters )
		{}
		void reset( bso::bool__ P = true )
		{
			S_.PredefinedFilters = fpmf_None;

			Title.reset( P );
			Filters.reset( P );
		}
		void plug( mmm::E_MULTIMEMORY_ &MM )
		{
			Title.plug( MM );
			Filters.plug( MM );
		}
		file_picker_ &operator =( const file_picker_ &FP )
		{
			S_.PredefinedFilters = FP.S_.PredefinedFilters;

			Title = FP.Title;
			Filters = FP.Filters;

			return *this;
		};
		void Init( const str::string_ &Title )
		{
			this->Title.Init( Title );
			Filters.Init();
		}
		int SetPredefinedFilter( int Filters )	// 'Filters' : combinaison de 'file_picker_mask_flag__'. Retourne les anciens filtres.
		{
			tol::Swap( S_.PredefinedFilters, Filters );

			return Filters;
		}
		void AddFilter(
			const str::string_ &Title,
			const str::string_ &Mask );	// Pour ajouter un filtre ne faisant pas partie de ceux prédéfinis.
		/* Retourne 'true' si un fichier a été sélectionné ('FileName' contient alors le fichier),
		'false' si 'Cancel' a été sélectionné. */
		bso::bool__ Show(
			nsIDOMWindow *ParentWindow,	// Cette fonction bloque toute action sur 'ParentWindow', donc pas possibilité d'utiliser 'MasterWindow'.
			file_picker_type__ Type,
			const char *DefaultExtension,
		const lcl::rack__ &LocaleRack,
			str::string_ &FileName );
		bso::bool__ Show(
			nsIDOMWindow *ParentWindow,	// Cette fonction bloque toute action sur 'ParentWindow', donc pas possibilité d'utiliser 'MasterWindow'.
			file_picker_type__ Type,
		const lcl::rack__ &LocaleRack,
			str::string_ &FileName )
		{
			return Show( ParentWindow, Type, NULL, LocaleRack, FileName );
		}
		E_RODISCLOSE_( int, PredefinedFilters );
	};

	E_AUTO( file_picker );

	/* Retourne 'true' si un fichier a été sélectionné ('FileName' contient alors le fichier),
	'false' si 'Cancel' a été sélectionné. */
	bso::bool__ FileOpenDialogBox(
		nsIDOMWindow *ParentWindow,	// Fonction bloquante, dont 'ParentWindow' ne peut être déduit de 'MasterWindow'.
		const str::string_ &Title,
		const char *DefaultExtension,
		int PredefinedFilters,
		const lcl::rack__ &LocaleRack,
		str::string_ &FileName );

	/* Retourne 'true' si un fichier a été sélectionné ('FileName' contient alors le fichier),
	'false' si 'Cancel' a été sélectionné. */
	inline bso::bool__ HTMLFileOpenDialogBox(
		nsIDOMWindow *ParentWindow,	// Fonction bloquante, dont 'ParentWindow' ne peut être déduit de 'MasterWindow'.
		const str::string_ &Title,
		const lcl::rack__ &LocaleRack,
		str::string_ &FileName )
	{
		return FileOpenDialogBox( ParentWindow, Title, "html", fpmfHTML, LocaleRack, FileName );
	}

	/* Retourne 'true' si un fichier a été sélectionné ('FileName' contient alors le fichier),
	'false' si 'Cancel' a été sélectionné. */
	inline bso::bool__ XPRJFileOpenDialogBox(
		nsIDOMWindow *ParentWindow,	// Fonction bloquante, dont 'ParentWindow' ne peut être déduit de 'MasterWindow'.
		const str::string_ &Title,
		const lcl::rack__ &LocaleRack,
		str::string_ &FileName )
	{
		return FileOpenDialogBox( ParentWindow, Title, "xprj", fpmfXPRJ, LocaleRack, FileName );
	}

	/* Retourne 'true' si un fichier a été sélectionné ('FileName' contient alors le fichier),
	'false' si 'Cancel' a été sélectionné. */
	bso::bool__ FileSaveDialogBox(
		nsIDOMWindow *ParentWindow,	// Fonction bloquante, dont 'ParentWindow' ne peut être déduit de 'MasterWindow'.
		const str::string_ &Title,
		const char *DefaultExtension,
		int PredefinedFilters,
		const lcl::rack__ &LocaleRack,
		str::string_ &FileName );

	/* Retourne 'true' si un fichier a été sélectionné ('FileName' contient alors le fichier),
	'false' si 'Cancel' a été sélectionné. */
	inline bso::bool__ HTMLFileSaveDialogBox(
		nsIDOMWindow *ParentWindow,	// Fonction bloquante, dont 'ParentWindow' ne peut être déduit de 'MasterWindow'.
		const str::string_ &Title,
		const lcl::rack__ &LocaleRack,
		str::string_ &FileName )
	{
		return FileSaveDialogBox( ParentWindow, Title, "html", fpmfHTML, LocaleRack, FileName );
	}

	/* Retourne 'true' si un fichier a été sélectionné ('FileName' contient alors le fichier),
	'false' si 'Cancel' a été sélectionné. */
	inline bso::bool__ XPRJFileSaveDialogBox(
		nsIDOMWindow *ParentWindow,	// Fonction bloquante, dont 'ParentWindow' ne peut être déduit de 'MasterWindow'.
		const str::string_ &Title,
		const lcl::rack__ &LocaleRack,
		str::string_ &FileName )
	{
		return FileSaveDialogBox( ParentWindow, Title, "xprj", fpmfXPRJ, LocaleRack, FileName );
	}

	/* Retourne 'true' si un répertoire a été sélectionné ('DirectoryName' contient alors le répetoire),
	'false' si 'Cancel' a été sélectionné. */
	bso::bool__ DirectorySelectDialogBox(
		nsIDOMWindow *ParentWindow,	// Fonction bloquante, dont 'ParentWindow' ne peut être déduit de 'MasterWindow'.
		const str::string_ &Title,
		const lcl::rack__ &LocaleRack,
		str::string_ &DirectoryName );

	inline void OpenWindow(
		nsIDOMWindow *ParentWindow,
		const char *URL,
		const char *Name,
		nsIDOMWindow **Window = NULL )
	{
	ERRProlog
		nsIDOMWindow **WindowBuffer = NULL, *DummyWindow = NULL, *MasterWindow = NULL;
		nsEmbedString TransformedURL, TransformedName;
	ERRBegin
		if ( Window != NULL )
			WindowBuffer = Window;
		else
			WindowBuffer = &DummyWindow;


		Transform( URL, TransformedURL );
		Transform( Name, TransformedName );

		if ( ParentWindow == NULL )
			if ( ( MasterWindow = RetrieveMasterWindow() ) != NULL )
				ParentWindow = MasterWindow;
			else
				ERRu();

		T( GetWindowInternal( ParentWindow )->Open( TransformedURL, TransformedName, NS_LITERAL_STRING( "chrome,extrachrome,menubar,resizable,scrollbars,status,toolbar" ), WindowBuffer ) );;
	ERRErr
	ERREnd
		if ( MasterWindow != NULL )
			ReleaseMasterWindow( MasterWindow );
	ERREpilog
	}

	inline void OpenWindow(
		const char *URL,
		const char *Name,
		nsIDOMWindow **Window = NULL )
	{
		OpenWindow( NULL, URL, Name, Window );
	}

	inline void OpenDialog(
		nsIDOMWindow *ParentWindow,
		const char *URL,
		const char *Name,
		nsIDOMWindow **Window = NULL )
	{
	ERRProlog
		nsIDOMWindow **WindowBuffer = NULL, *DummyWindow = NULL, *MasterWindow = NULL;
		nsEmbedString TransformedURL, TransformedName;
	ERRBegin
		if ( Window != NULL )
			WindowBuffer = Window;
		else
			WindowBuffer = &DummyWindow;


		Transform( URL, TransformedURL );
		Transform( Name, TransformedName );

		if ( ParentWindow == NULL )
			ERRu();	// Fonction bloquante, donc on ne peut pas prendre 'MAsterWindow' (risque de bloquer la fenêtre non initiatrice de l'ouverture de la boîte de dialogue).

		T( GetWindowInternal( ParentWindow )->OpenDialog( TransformedURL, TransformedName, NS_LITERAL_STRING( "chrome,extrachrome,menubar,resizable,scrollbars,status,toolbar,modal" ), NULL, WindowBuffer ) );
	ERRErr
	ERREnd
		if ( MasterWindow != NULL )
			ReleaseMasterWindow( MasterWindow );
	ERREpilog
	}

	/*inline void OpenDialog(
		const char *URL,
		const char *Name,
		nsIDOMWindow **Window = NULL )
	{
		OpenDialog( NULL, URL, Name, Window );
	}
	*/

	inline nsIDOMWindowInternal *GetJSConsole(
		nsIDOMWindow *ParentWindow,
		nsIDOMWindowInternal **JSConsoleWindow )
	{
		nsIDOMWindow *Window = NULL;

		if ( ( *JSConsoleWindow == NULL ) || ( IsClosed( *JSConsoleWindow ) ) ) {
//			GetWindowInternal( ParentWindow )->Open( NS_LITERAL_STRING( "chrome://global/content/console.xul" ), NS_LITERAL_STRING( "_blank" ), NS_LITERAL_STRING( "chrome,extrachrome,menubar,resizable,scrollbars,status,toolbar" ), &Window );
			OpenWindow( ParentWindow, "chrome://global/content/console.xul", "_blank", &Window );
			*JSConsoleWindow = GetWindowInternal( Window );
		} else
			T( (*JSConsoleWindow)->Focus() );

		return *JSConsoleWindow;
	}

	void GetJSConsole( nsIDOMWindow *ParentWindow );

	inline void GetJSConsole( void )
	{
		GetJSConsole( NULL );
	}

	// Voir https://developer.mozilla.org/en/XULRunner_tips#DOM_Inspector
	inline void GetDOMInspector( nsIDOMWindow *ParentWindow )
	{
//		nsIDOMWindow *Window = NULL;

//		GetWindowInternal( ParentWindow )->Open( NS_LITERAL_STRING( "chrome://inspector/content/inspector.xul" ), NS_LITERAL_STRING( "_blank" ), NS_LITERAL_STRING( "chrome,extrachrome,menubar,resizable,scrollbars,status,toolbar" ), &Window );
		OpenWindow( ParentWindow, "chrome://inspector/content/inspector.xul", "_blank" );
	}

	// Voir https://developer.mozilla.org/en/XULRunner_tips#DOM_Inspector
	inline void GetDOMInspector( void )
	{
		GetDOMInspector( NULL );
	}

	template <typename type, typename row> E_TTYPEDEF( lstbch::E_LBUNCHt_( type *, row ), _lpbunch_ );	// 'List Pointer Bunch'.

	template <typename type, typename row> class repository_
	: public _lpbunch_<type, row>
	{
	public:
		struct s
		: _lpbunch_<type, row>::s
		{
			row Row;
		} &S_;
		void reset( bso::bool__ P = true )
		{
			if ( P ) {
				if ( _lpbunch_<type, row>::Amount() != 0 )
					ERRu();	// Car des objets existent qui n'ont pas encore été supprimé ('delete'r).
			}

			_lpbunch_<type, row>::reset( P );
			S_.Row = NONE;
		}
		repository_( s &S )
		: S_( S ),
		  _lpbunch_<type, row>( S )
		{}
		void plug( mmm::E_MULTIMEMORY_ &MMM )
		{
			_lpbunch_<type, row>::plug();
		}
		repository_ &operator =( const repository_ &R )
		{
			_lpbunch_<type, row>::operator =( *this );
			S_.Row = R.S_.Row;

			return *this;
		}
		void Init( void )
		{
			reset();
			_lpbunch_<type, row>::Init();
		}
		row CreateNewObject( void )
		{
			type *UserObject = NULL;

			if ( S_.Row != NONE )
				ERRu();

			if ( ( UserObject = new type ) == NULL )
				ERRa();

			return S_.Row = Add( UserObject );
		}
		void DismissCurrentObject( void )
		{
			if ( S_.Row == NONE )
				ERRu();

			S_.Row = NONE;
		}
		void SetCurrentRow( row Row )
		{
			if ( S_.Row != NONE )
				ERRu();

			S_.Row = Row;
		}
		row GetCurrentRow( void ) const
		{
			if ( S_.Row == NONE )
				ERRu();

			return S_.Row;
		}
		type &GetCurrentObject( void ) const
		{
			return *Get( GetCurrentRow() );
		}
		void Delete( row Row )
		{
	#ifdef XXX_DBG
			if ( !Exists( Row ) )
				ERRu();
	#endif
			delete Get( Row );

			Store( NULL, Row );

			_lpbunch_<type, row>::Delete( Row );

		}
	};

	E_AUTO2( repository );


	class xslt_parameter_
	{
	public:
		struct s {
			str::string_::s Name, Value;
		};
		str::string_ Name, Value;
		xslt_parameter_( s &S )
		: Name( S.Name ),
		  Value( S.Value )
		{}
		void reset( bso::bool__ P = true )
		{
			Name.reset( P );
			Value.reset( P );
		}
		void plug( mmm::E_MULTIMEMORY_ &MM )
		{
			Name.plug( MM );
			Value.plug( MM );
		}
		xslt_parameter_ &operator =( const xslt_parameter_ &XP )
		{
			Name = XP.Name;
			Value = XP.Value;

			return *this;
		}
		void Init( void )
		{
			reset();

			Name.Init();
			Value.Init();
		}
	};

	E_AUTO( xslt_parameter );

	typedef ctn::E_XCONTAINER_( xslt_parameter_ ) _xslt_parameters_;

	class xslt_parameters_
	: public _xslt_parameters_
	{
	public:
		struct s
		: public _xslt_parameters_::s
		{};
		xslt_parameters_( s &S )
		: _xslt_parameters_( S )
		{}
		void reset( bso::bool__ P = true )
		{
			_xslt_parameters_::reset( P );
		}
		void plug( mmm::E_MULTIMEMORY_ &MM )
		{
			_xslt_parameters_::plug( MM );
		}
		xslt_parameters_ &operator =( const xslt_parameters_ &XP )
		{
			_xslt_parameters_::operator =( XP );

			return *this;
		}
		void Init( void )
		{
			_xslt_parameters_::Init();
		}
		void Append(
			const str::string_ &Name,
			const str::string_ &Value );
		void Append(
			const char *Name,
			const char *Value )
		{
			Append( str::string( Name ), str::string( Value ) );
		}
		void Append(
			const str::string_ &Name,
			const char *Value )
		{
			Append( Name, str::string( Value ) );
		}
		void Append(
			const char *Name,
			const str::string_ &Value )
		{
			Append( str::string( Name ), str::string( Value ) );
		}
	};


	E_AUTO( xslt_parameters );

	nsIDOMDocumentFragment *XSLTransform(
		nsIDOMDocument *XMLDocument,
		nsIDOMDocument *XSLTStylesheet,
		nsIDOMDocument *Owner,
		const xslt_parameters_ &Parameters );

	nsIDOMDocumentFragment *XSLTransformByContent(
		const str::string_ &XMLString,	// Doit être du XML valide.
		const str::string_ &XSLString,	// Doit être du XSL valide.
		nsIDOMDocument *Owner,
		const xslt_parameters_ &Parameters );

	nsIDOMDocumentFragment *XSLTransformByFileName(
		const str::string_ &XMLString,	// Doit être du XML valide.
		const str::string_ &XSLFileName,	// Doit être du XSL valide.
		nsIDOMDocument *Owner,
		const xslt_parameters_ &Parameters );

	void LaunchURI( const str::string_ &URI );

	inline void LaunchURI( const char *URI )
	{
		LaunchURI( str::string( URI ) );
	}

	void LaunchEmbedFile( const char *File );	// 'File' doit contenir un fichier avec un chemin relatif par rapport à où est situé 'xulrunner.exe'.

	// Pour les 'textbox's de type 'autocomplete'.
	// NOTA : La ligne 'pref("browser.formfill.enable", true);' doit être ajoutée dans le fihcier 'prefs.js'.
	void AddFormHistoryEntry(
		const str::string_ &Entry,	// Valuer de l'attribut 'autocompletesearchparam' dudit 'textbox'.
		const str::string_ &Value );

	inline void AddFormHistoryEntry(
		const char *Name,
		const str::string_ &Entry )
	{
		AddFormHistoryEntry( str::string( Name ), Entry );
	}

	void RemoveFormHistoryEntry(
		const str::string_ &RawName,
		const str::string_ &RawEntry );

	inline void RemoveFormHistoryEntry(
		const char *Name,
		const str::string_ &Entry )
	{
		RemoveFormHistoryEntry( str::string( Name ), Entry );
	}

	void RemoveEntriesForName( const str::string_ &Name );

	inline void RemoveEntriesForName( const char *Name )
	{
		RemoveEntriesForName( str::string( Name ) );
	}

	typedef str::string_ argument_;
	typedef str::string argument;

	typedef ctn::E_XMCONTAINER_( argument_ ) arguments_;
	E_AUTO( arguments )

	bso::ulong__ GetArguments(
		nsICommandLine *CommandLine,
		arguments_ &Arguments );

	void PatchBadCommandBehaviorForListeningMenuItems( nsIDOMDocument *Document );

	void PatchCommandBadCommandBehaviorforKeysetListener( nsIDOMDocument *Document );

	inline void PatchOverallBadCommandBehavior( nsIDOMDocument *Document )
	{
		PatchBadCommandBehaviorForListeningMenuItems( Document );
		PatchCommandBadCommandBehaviorforKeysetListener( Document );
	}

	nsIDOMWindow *GetDocumentWindow( nsIDOMDocument *Document );


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

// Début de la partie concernant l''event_listener'.
// Copié à partir du '.h' d'un '.idl' contenant cd qui suit :

/*
[scriptable, uuid(d333cd20-c453-11dd-ad8b-0800200c9a66)]
interface eevent_listener
: nsIDOMEventListener
{};
*/

namespace nsxpcm {
	class NS_NO_VTABLE NS_SCRIPTABLE ievent_listener
	: public nsIDOMEventListener
	{
	public: 
		NS_DECLARE_STATIC_IID_ACCESSOR(NSXPCOM_IEVENT_LISTENER_IID)
	};

	/*
	ATTENTION, IMPORTANT : pour des raison de simplification de mise en oeuvre et de par le système
	d'enregistrement de 'XULRunner', cet object est commun à tous les composants utilisant la gestion d'évènement
	des bibliothèques Epeios. Une modification de cet objet	peut ne donc par être répercuté dans le composant,
	parce que l'ancienne version de ce composant est encore	actif car existant dans un autre composant.
	Il faut donc effacer touts les composants et les donnée utilisateurs associées pour que lea modifications
	de cet objet soient prises en compte.
	*/
	struct event_listener
	: ievent_listener
	{
	public:
		NS_DECL_ISUPPORTS
	//  NS_DECL_IEEVENT_LISTENER
		  event_listener()
		  {
			  reset( false );
		  }
		  ~event_listener()
		  {
			  reset();
		  }
		  void reset( bso::bool__ = true )
		  {
			  _Widget = NULL;
		  }
	protected:
		NS_IMETHOD HandleEvent(nsIDOMEvent *event);
	private:
		widget_core__ *_Widget;
	public:
		void Init( widget_core__ &Widget )
		{
#ifdef NSXPCM_DBG
			if ( _Widget != NULL )
				ERRu();
#endif
			reset();

			_Widget = &Widget;
		}
	};

	NS_GENERIC_FACTORY_CONSTRUCTOR(event_listener)
}


	NS_DEFINE_STATIC_IID_ACCESSOR(nsxpcm::ievent_listener, NSXPCM_IEVENT_LISTENER_IID)

#define NSXPCM_EVENT_LISTENER_CONTRACTID "@zeusw.org/nsxpcm_event_listener;1"
#define NSXPCM_EVENT_LISTENER_CLASSNAME "NSXPCMEventListener"
// {d333cd20-c453-11dd-ad8b-0800200c9a66}
#define NSXPCM_EVENT_LISTENER_CID  NSXPCM_IEVENT_LISTENER_IID

// Fin de la partie concernant l''event_listener'.

// Début de la partie concernant l'xslt_processor'.
// Copié à partir du '.h' d'un '.idl' contenant ce qui suit :

/*
[scriptable, uuid(90d1d640-019d-4e60-bed1-3a5f3b208a2a)]
interface iexslt_processor
: nsIXSLTProcessor
{};
*/
/*
namespace nsxpcm {
	class NS_NO_VTABLE NS_SCRIPTABLE iexslt_processor
	: public nsIXSLTProcessor
	{
	public: 
		NS_DECLARE_STATIC_IID_ACCESSOR(IEXSLT_PROCESSOR_IID)
	};



	class exslt_processor
	: public iexslt_processor
	{
	public:
		NS_DECL_ISUPPORTS
		// NS_DECL_IEXSLT_PROCESSOR

		exslt_processor()
		{}
		~exslt_processor()
		{}
	};

	NS_GENERIC_FACTORY_CONSTRUCTOR(exslt_processor)

	NS_DEFINE_STATIC_IID_ACCESSOR(iexslt_processor, IEXSLT_PROCESSOR_IID)

#define NSXPCM_XSLT_PROCESSOR_CONTRACTID "@zeusw.org/nsxpcm_xslt_processor;1"
#define NSXPCM_XSLT_PROCESSOR_CLASSNAME "NSXPCMXSLTProcessor"
// {90d1d640-019d-4e60-bed1-3a5f3b208a2a}
#define NSXPCM_XSLT_PROCESSOR_CID  NSXPCM_XSLT_PROCESSOR_IID
*/
// fin de la partie concernant l'xslt_processor'

#define NSXPCM_COMPONENTS\
/*    {\
       NSXPCM_XSLT_PROCESSOR_CLASSNAME,\
       NSXPCM_XSLT_PROCESSOR_CID,\
       NSXPCM_XSLT_PROCESSOR_CONTRACTID,\
	   nsxpcm::xslt_processorConstructor,\
    },\
*/    {\
       NSXPCM_EVENT_LISTENER_CLASSNAME,\
       NSXPCM_EVENT_LISTENER_CID,\
       NSXPCM_EVENT_LISTENER_CONTRACTID,\
	   nsxpcm::event_listenerConstructor,\
    }

#ifdef NSXPCM__T_BUFFER
#	define T	NSXPCM__T_BUFFER
#else
#	undef T
#endif

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
