/*
	Header for the 'nsxpcm' library by Claude SIMON (csimon@epeios.org)
	Copyright (C) $COPYRIGHT_DATES$Claude SIMON (csimon@epeios.org).
$_RAW_$
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

#include "err.h"
#include "flw.h"
#include "epeios.h"
#include "str.h"
#include "ctn.h"
#include "lstbch.h"

#include "xpcom-config.h"

#include "layout/nsITreeView.h"
#include "layout/nsITreeContentView.h"
#include "layout/nsITreeSelection.h"
#include "layout/nsIListBoxObject.h"

#include "dom/nsIDOMDocument.h"
#include "dom/nsIDOMElement.h"
#include "dom/nsIDOMXULMultSelectCntrlEl.h"
#include "dom/nsIDOMXULSelectCntrlItemEl.h"
#include "dom/nsIDOMXULTextboxElement.h"
#include "dom/nsIDOMXULMenuListElement.h"
#include "dom/nsIDOMXULCheckboxElement.h"
#include "dom/nsIDOMXULButtonElement.h"
#include "dom/nsIDOMXULTreeElement.h"
#include "dom/nsIDOMXULDescriptionElement.h"
#include "dom/nsIDOMWindowInternal.h"
#include "dom/nsIDOMXULLabelElement.h"
#include "dom/nsIDOMMutationEvent.h"
#include "dom/nsIDOMKeyEvent.h"

#include "appshell/nsIXULWindow.h"

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

#include "commandhandler/nsICommandManager.h"

#ifdef NSXPCM_BKD
#	define NSXPCM__BKD
#endif

#ifdef NSXPCM__BKD
#	include "bkdacc.h"
#endif

#if defined( __ARMEL__ ) || defined( NSXPCM_USE_ARMEL_WORKAROUND )
#	ifndef NSXPCM_NO_ARMEL_WORKAROUND
#		define NSXPCM__USE_ARMEL_WORKAROUND
#	endif
#endif

#define NSXPCM_EVENT_LISTENER_IID_STR "d333cd20-c453-11dd-ad8b-0800200c9a66"

#define NSXPCM_IEVENT_LISTENER_IID \
  {0xd333cd20, 0xc453, 0x11dd, \
    { 0xad, 0x8b, 0x08, 0x00, 0x20, 0x0c, 0x9a, 0x66 }}

namespace nsxpcm {
	using str::string_;
	using str::string;

	// A éventuellement initialiser par l'utilisateur, pour faciliter l'utilisation
	// de certaines fonctions ('GetJSConsole(...)', 'Alert(...)', ...).
	extern nsIDOMWindow *MasterWindow;

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
		efNone = 0,
		efAll = ( ( 1 << e_amount ) - 1 ),
		efAllButAnnoying = efAll & ~efAttributeChange & ~efBlur & ~efFocus	// Pour faciliter le déboguage.
	};

#ifdef NSXPCM__F_BUFFER
#	define	EF	NSXPCM__EF_BUFFER
#	undef EF
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

	// 'ContractID' est une chaîne de caratère du genre "@mozilla.org/filepicker;1".
	template <typename t> inline t *GetService(
		const char *ContractID,
		nsCOMPtr<t> &Service )
	{	
		nsresult Error = NS_OK;

		Service = do_GetService( ContractID, &Error );

		if ( Error != NS_OK )
			ERRu();

		return Service;
	}

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
		nsresult Result = NS_OK;	// Aide au débogage.

#ifdef NSXPCM_DBG

		if ( GenericElement == NULL )
			ERRu();
#endif
		if ( ( ( Result = GenericElement->QueryInterface( element::GetIID(), (void **)&Element ) ) != NS_OK ) && ( ErrHandle != err::hSkip ) )
			ERRu();

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

	inline const str::string_ &GetAttribute(
		nsIDOMNode *Node,
		const char *Name,
		str::string_ &Value,
		err::handle ErrHandle = err::hUsual )
	{
		nsIDOMElement *Element = QueryInterface<nsIDOMElement>( Node, ErrHandle );

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

		if ( Element->GetSelectedIndex( &Index ) != NS_OK )
			ERRx();

		return Index;
	}

	template <typename element> inline bool IsChecked( element *Element )
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

	inline nsIDOMElement *GetDocumentElement( nsIDOMDocument *Document )
	{
		nsIDOMElement *Element = NULL;

		if ( Document->GetDocumentElement( &Element ) != NS_OK )
			ERRu();

		return Element;
	}

	inline nsIDOMDocument *GetWindowDocument( nsIDOMWindow *Window )
	{
		nsIDOMDocument *Document = NULL;

		if ( Window->GetDocument( &Document ) != NS_OK )
			ERRu();

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

		if ( Window->GetClosed( &Value ) != NS_OK )
			ERRu();

		return Value == PR_TRUE;
	}

	inline void Close( nsIDOMWindowInternal *Window )
	{
		if ( Window->Close() != NS_OK )
			ERRu();
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

	class element_core__
	{
	private:
		nsISupports *_Supports;
	public:
		nsCOMPtr<struct event_listener> _EventListener;
	protected:
		nsIDOMEvent *_Event;
		nsIDOMMutationEvent *_MutationEvent;
		nsIDOMKeyEvent *_KeyEvent;
		void EventStopPropagation( void )
		{
			if ( _Event == NULL )
				ERRu();

			_Event->StopPropagation();
		}
		void EventPreventDefault( void )
		{
			if ( _Event == NULL )
				ERRu();

			_Event->PreventDefault();
		}
		virtual void NSXPCMOnRawEvent( const str::string_ &Event );
		virtual void NSXPCMOnRawEvent( const char *Event );
		virtual void NSXPCMOnEvent( event__ Event );
		virtual void NSXPCMOnCommand( void )
		{
			ERRu();
		}
		virtual void NSXPCMOnClick( void )
		{
			ERRu();
		}
		virtual void NSXPCMOnDblClick( void )
		{
			ERRu();
		}
		virtual void NSXPCMOnInput( void )
		{
			ERRu();
		}
		virtual void NSXPCMOnFocus( void )
		{
			ERRu();
		}
		virtual void NSXPCMOnBlur( void )
		{
			ERRu();
		}
		virtual void NSXPCMOnSelect( void )
		{
			ERRu();
		}
		virtual void NSXPCMOnAttributeChange( void )
		{
			ERRu();
		}
		virtual void NSXPCMOnKeyPress( void )
		{
			ERRu();
		}
		virtual void NSXPCMOnClose( void )
		{
			ERRu();
		}
	public:
		void reset( bso::bool__ = true )
		{
			_Supports = NULL;
			_Event = NULL;
			_MutationEvent = NULL;
			_KeyEvent = NULL;
		}
		element_core__( void )
		{
			reset( false );
		}
		virtual ~element_core__( void )
		{
			reset( );
		}
		void Init( void )
		{
			reset();
		}
		void Init(
			nsISupports *Supports,
			int Events );
		E_RODISCLOSE__( nsISupportsPointer, Supports );
		void Handle(
			nsIDOMEvent * Event,
			const str::string_ &EventString )
		{
			// Sauvegarde pour la gestion d'évènements imbriqués.
			nsIDOMEvent *EventBuffer = _Event;
			nsIDOMMutationEvent *MutationEventBuffer = _MutationEvent;
			nsIDOMKeyEvent *KeyEventBuffer = _KeyEvent;

			_Event = Event;

			if ( EventString == "DOMAttrModified" )
				_MutationEvent = QueryInterface<nsIDOMMutationEvent>( Event );

			if ( EventString == "keypress" )
				_KeyEvent = QueryInterface<nsIDOMKeyEvent> ( Event );

			NSXPCMOnRawEvent( EventString );

			_Event = EventBuffer;
			_MutationEvent = MutationEventBuffer;
			_KeyEvent = KeyEventBuffer;
		}
		nsIDOMElement *GetElement( void )
		{
			return QueryInterface<nsIDOMElement>( _Supports );
		}
/*		void Register(
			nsIDOMElement *Element,
			nsIDOMWindow *Window );
*/	};

	E_ROW( row__ );

	inline void Register(
		nsxpcm::element_core__ &Core,
		nsISupports *Supports,
		int Events )
	{
		Core.Init( Supports, Events );
	}

	template <class id_type> inline void Register(
		nsxpcm::element_core__ &Core,
		nsIDOMDocument *Document,
		const id_type &Id,
		int Events )
	{
		Register( Core, nsxpcm::GetElementById( Document, Id ), Events );
	}

	void Alert(
		nsIDOMWindow *Window,
		const char *Text );

	inline void Alert(
		const char *Text,
		bso::bool__ IgnoreError = false )
	{
		if ( MasterWindow != NULL )
			Alert( MasterWindow, Text );
		else if ( !IgnoreError )
			ERRu();
	}

	void Alert(
		nsIDOMWindow *Window,
		const str::string_ &Text );

	inline void Alert(
		const str::string_ &Text,
		bso::bool__ IgnoreError = false )
	{
		if ( MasterWindow != NULL )
			Alert( MasterWindow, Text );
		else if ( !IgnoreError )
			ERRu();
	}

	bso::bool__ Confirm(
		nsIDOMWindow *Window,
		const char *Text );

	bso::bool__ Confirm(
		nsIDOMWindow *Window,
		const str::string_ &Text );

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
			return QueryInterface<object>( GetSupports() );
		}
		void Enable( bso::bool__ Value = true )
		{
			if ( Value )
				nsxpcm::RemoveAttribute( GetObject(), "disabled" );
			else
				nsxpcm::SetAttribute( GetObject(), "disabled", "true" );
		}
		void Disable( void )
		{
			Enable( false );
		}
		bso::bool__ IsEnabled( void )
		{
			bso::bool__ Enabled = false;
		ERRProlog
			str::string Value;
		ERRBegin
			Value.Init();

			nsxpcm::GetAttribute( GetObject(), "disabled", Value );

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
			nsxpcm::SetAttribute( GetObject(), "label", Label );
		}
		void SetLabel( const char *Label )
		{
			nsxpcm::SetAttribute( GetObject(), "label", Label );
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

	class element__	// Classe générique.
	: public _element__<nsIDOMElement>
	{};

	class button__
	: public _element__<nsIDOMElement>	// Devrait normalement être '<nsIButton>', mais l'inclusion de 'nsIButton.h' pose problème.
	{};

	class textbox__
	: public _element__<nsIDOMXULTextBoxElement>
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
			if ( GetObject()->Select() != NS_OK )
				ERRs();
		}
		void SetSize( bso::size__ Size )
		{
			if ( GetObject()->SetSize( Size ) != NS_OK )
				ERRu();
		}
		bso::size__ GetSize( void )
		{
			PRInt32 Buffer;

			if ( GetObject()->GetSize( &Buffer ) != NS_OK )
				ERRs();

			return Buffer;
		}
		void SetMaxLength( bso::size__ Size )
		{
			if ( GetObject()->SetMaxLength( Size ) != NS_OK )
				ERRu();
		}
		bso::size__ GetMaxLength( void )
		{
			PRInt32 Buffer;

			if ( GetObject()->GetMaxLength( &Buffer ) != NS_OK )
				ERRs();

			return Buffer;
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

	class label__
	: public _element__<nsIDOMXULLabelElement>
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
	: public _element__<nsIDOMXULCheckboxElement>
	{
	public:
		bso::bool__ IsChecked( void )
		{
			return nsxpcm::IsChecked( GetObject() );
		}
	};

	class radio__
	: public _element__<nsIDOMXULButtonElement>
	{
	public:
		bso::bool__ IsChecked( void )
		{
			return nsxpcm::IsChecked( GetObject() );
		}
	};

	class listitem__
	: public _element__<nsIDOMXULSelectControlItemElement>
	{};

	class listbox__
	: public _element__<nsIDOMXULMultiSelectControlElement>
	{
	private:
	public:
		nsIDOMXULSelectControlItemElement *GetCurrentItem( bso::bool__ ErrorIfInexistant = true )
		{
			nsIDOMXULSelectControlItemElement *Item = NULL;

			GetObject()->GetCurrentItem( &Item );

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

			GetObject()->GetSelectedCount( &Count );

			return Count;
		}
	};

	class panel__
	: public _element__<nsIDOMElement>
	{};

	class deck__
	: public _element__<nsIDOMElement>
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
	: public _element__<nsIDOMXULTreeElement>
	{
	private:
		nsITreeView *_GetView( void )
		{
			nsITreeView *View = NULL;

			GetObject()->GetView( &View );

			if ( View == NULL )
				ERRu();

			return View;
		}
		nsITreeSelection *_GetSelection( void )
		{
			nsITreeSelection *Selection = NULL;

			_GetView()->GetSelection( &Selection );

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

			_GetSelection()->GetCurrentIndex( &Count );

			return Count;
		}
		void Select( bso::slong__ Index )
		{
//			_GetSelection()->SetCurrentIndex( Index );
			_GetSelection()->Select( Index );
		}
		nsIDOMElement *GetCurrentItem( bso::bool__ ErrorIfInexistent = true )
		{
			nsIDOMElement *Element = NULL;

			_GetContentView()->GetItemAtIndex( GetCurrentIndex(), &Element );

			if ( ( Element == NULL ) && ErrorIfInexistent )
				ERRu();

			return Element;
		}
		void Select( nsIDOMElement *Element )
		{
			PRInt32 Index = -1;

			if ( _GetContentView()->GetIndexOfItem( Element, &Index ) != NS_OK )
				ERRu();

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
		void ClearSelection( void )
		{
			_GetSelection()->ClearSelection();
		}
	};

	class broadcaster__
	: public _element__<nsIDOMElement>
	{};

	class command__
	: public _element__<nsIDOMElement>
	{};


	class html_anchor__
	: public _element__<nsIDOMHTMLAnchorElement>
	{};

	class menu__
	: public _element__<nsIDOMElement>	// Pas cherché le 'nsI...' correspondant ...
	{};

	class menu_item__
	: public _element__<nsIDOMElement>	// Pas trouvé le 'nsI...' correspondant ...
	{};

	class window__
	: public _element__<nsIDOMWindow>
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

	/* Retourne 'true' si un fichier a été sélectionné ('FileName' contient alors le fichier),
	'false' si 'Cancel' a été sélectionné. */
	bso::bool__ FileOpenDialogBox(
		nsIDOMWindow *Parent,
		const char *Title,
		str::string_ &FileName );

	/* Retourne 'true' si un fichier a été sélectionné ('FileName' contient alors le fichier),
	'false' si 'Cancel' a été sélectionné. */
	bso::bool__ HTMLFileOpenDialogBox(
		nsIDOMWindow *Parent,
		const char *Title,
		str::string_ &FileName );

	/* Retourne 'true' si un fichier a été sélectionné ('FileName' contient alors le fichier),
	'false' si 'Cancel' a été sélectionné. */
	bso::bool__ FileSaveDialogBox(
		nsIDOMWindow *Parent,
		const char *Title,
		str::string_ &FileName );

	/* Retourne 'true' si un fichier a été sélectionné ('FileName' contient alors le fichier),
	'false' si 'Cancel' a été sélectionné. */
	bso::bool__ HTMLFileSaveDialogBox(
		nsIDOMWindow *Parent,
		const char *Title,
		str::string_ &FileName );

	/* Retourne 'true' si un répertoire a été sélectionné ('DirectoryName' contient alors le répetoire),
	'false' si 'Cancel' a été sélectionné. */
	bso::bool__ DirectorySelectDialogBox(
		nsIDOMWindow *Parent,
		const char *Title,
		str::string_ &DirectoryName );

	// Log to the javascript console.
	void Log( const char *Text );

	// Log to the javascript console.
	void Log( const str::string_ &Text );


	inline nsIDOMWindowInternal *GetJSConsole(
		nsIDOMWindow *ParentWindow,
		nsIDOMWindowInternal **JSConsoleWindow )
	{
		nsIDOMWindow *Window = NULL;

		if ( ( *JSConsoleWindow == NULL ) || ( IsClosed( *JSConsoleWindow ) ) ) {
			GetWindowInternal( ParentWindow )->Open( NS_LITERAL_STRING( "chrome://global/content/console.xul" ), NS_LITERAL_STRING( "_blank" ), NS_LITERAL_STRING( "chrome,extrachrome,menubar,resizable,scrollbars,status,toolbar" ), &Window );
			*JSConsoleWindow = GetWindowInternal( Window );
		} else
			(*JSConsoleWindow)->Focus();

		return *JSConsoleWindow;
	}

	void GetJSConsole( nsIDOMWindow *ParentWindow );

	inline void GetJSConsole( void )
	{
		if ( MasterWindow != NULL )
			GetJSConsole( MasterWindow );
	}

	// Voir https://developer.mozilla.org/en/XULRunner_tips#DOM_Inspector
	inline void GetDOMInspector( nsIDOMWindow *ParentWindow )
	{
		nsIDOMWindow *Window = NULL;

		GetWindowInternal( ParentWindow )->Open( NS_LITERAL_STRING( "chrome://inspector/content/inspector.xul" ), NS_LITERAL_STRING( "_blank" ), NS_LITERAL_STRING( "chrome,extrachrome,menubar,resizable,scrollbars,status,toolbar" ), &Window );
	}

	// Voir https://developer.mozilla.org/en/XULRunner_tips#DOM_Inspector
	inline void GetDOMInspector( void )
	{
		if ( MasterWindow != NULL )
			GetDOMInspector( MasterWindow );
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
			_lpbunch_<type *, row>::plug();
		}
		repository_ &operator =( const repository_ &R )
		{
			_lpbunch_<type *, row>::operator =( *this );
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

	nsIDOMDocumentFragment *XSLTTransform(
		nsIDOMDocument *XMLDocument,
		nsIDOMDocument *XSLTStylesheet,
		nsIDOMDocument *Owner,
		const xslt_parameters_ &Parameters );

	nsIDOMDocumentFragment *XSLTTransform(
		const str::string_ &XMLString,
		const str::string_ &XSLStylesheetFileName,
		nsIDOMDocument *Owner,
		const xslt_parameters_ &Parameters );

	void LaunchURI( const char *URI );

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
			  _Core = NULL;
		  }
	protected:
		NS_IMETHOD HandleEvent(nsIDOMEvent *event);
	private:
	public:
		element_core__ *_Core;
		void Init( element_core__ &Core )
		{
#ifdef NSXPCM_DBG
			if ( _Core != NULL )
				ERRu();
#endif
			reset();

			_Core = &Core;
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

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
