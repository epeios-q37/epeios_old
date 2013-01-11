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

# include "cpe.h"
# include "err.h"
# include "flw.h"
# include "str.h"
# include "ctn.h"
# include "lstbch.h"
# include "lcl.h"
# include "cpe.h"

# include "xpcom-config.h"

# ifndef CPE__GECKO
#  error "The NSXPCM library can only be used in a 'Gecko' copmonent :'
# endif

# ifndef CPE__LIBRARY
#  error "A 'Gecko' component must be a dynamic library !"
# endif

# if NSXPCM_GECKO_V1
#  define NSXPCM__GECKO_V1
# elif NSXPCM_GECKO_V2
#  define NSXPCM__GECKO_V2
# elif NSXPCM_GECKO_V8
#  define NSXPCM__GECKO_V8
# else
#  error "Missing adequate 'NSXPCM_GECKO_Vx' definition ! Accepted values for 'x' : 1 (Gecko 1.9) or 2 (Gecko 2) or 8 (Gecko 8)"
# endif


# pragma warning( push )
# pragma warning( disable : 4800 )	// forcing value to bool 'true' or 'false' (performance warning)

# include "nsITreeView.h"
# include "nsITreeContentView.h"
# include "nsITreeSelection.h"
# include "nsIListBoxObject.h"
# include "nsDirectoryServiceDefs.h"
# include "nsIDOMDocument.h"
# include "nsIDOMElement.h"
# include "nsIDOMAttr.h"
# include "nsIDOMXULMultSelectCntrlEl.h"
# include "nsIDOMXULSelectCntrlEl.h"
# include "nsIDOMXULSelectCntrlItemEl.h"
# include "nsIDOMXULTextboxElement.h"
# include "nsIDOMXULMenuListElement.h"
# include "nsIDOMXULCheckboxElement.h"
# include "nsIDOMXULButtonElement.h"
# include "nsIDOMXULTreeElement.h"
# include "nsIDOMXULDescriptionElement.h"
# ifdef NSXPCM__GECKO_V8
#  include "nsIDOMWindow.h"
# else
#  include "nsIDOMWindowInternal.h"
# endif
# include "nsIDOMXULLabelElement.h"
# include "nsIDOMMutationEvent.h"
# include "nsIDOMKeyEvent.h" 
# include "nsIDOMEventTarget.h"
# include "nsIAutoCompleteResult.h"

#include "nsIXULWindow.h"

// #include "content/nsIXSLTProcessor.h"

# include "nsStringAPI.h"
# include "nsCOMPtr.h"
# include "nsComponentManagerUtils.h"
# include "nsServiceManagerUtils.h"
# include "nsIInterfaceRequestor.h"
# include "nsIDOMEventListener.h"
# ifdef NSXPCM__GECKO_V1
#  inclu de "nsIGenericFactory.h"
# elif defined( NSXPCM__GECKO_V2 ) || defined ( NSXPCM__GECKO_V8 )
#  include "mozilla/ModuleUtils.h"
# else
#  error
# endif
# include "nsIDOMHTMLAnchorElement.h"
# include "nsIDOMDocumentFragment.h"
# include "nsICommandLine.h" // Situ� dans 'toolkitcomps'.

# include "nsICommandManager.h"
# include "nsICommandLineHandler.h"

# pragma warning( pop )

# undef NSXPCM__ENABLE_FORMHISTORY

# ifdef NSXPCM_FORCE_FORMHISTORY
#  define NSXPCM__ENABLE_FORMHISTORY
# endif

#if 0
#  define NSXPCM__BOOL	bool
# else
#  define NSXPCM__BOOL	PRBool
# endif


# ifdef NSXPCM_FBL
#  define NSXPCM__FBL
# endif

# ifdef NSXPCM__FBL
# include "fblfrd.h"
# endif

# if defined( __ARMEL__ ) || defined( NSXPCM_USE_ARMEL_WORKAROUND )
#  ifndef NSXPCM_NO_ARMEL_WORKAROUND
#   define NSXPCM__USE_ARMEL_WORKAROUND
#  endif
# endif

// Permet de 'logger' une erreur et d'�viter qu'elle soit ne remont�e � 'XULRunner', qui ne saurait pas quoi en faire. A placer dans 'ERRErr'.
# define NSXPCM_ERR( window )\
	if ( ERRMajor == err::itn ) {\
		if ( ERRMinor == err::iExit )\
			nsxpcm::Close( window );\
		else if ( ERRMinor != err::iAbort ) {\
			err::buffer__ Buffer;\
			nsxpcm::GetJSConsole( window );\
			nsxpcm::Log( err::Message( Buffer ) );\
		}\
	} else {\
		err::buffer__ Buffer;\
		nsxpcm::GetJSConsole( window );\
		nsxpcm::Log( err::Message( Buffer ) );\
	}\
	ERRRst()

# ifdef NSCPM_XUL_EXTENSION_DEFAULT_NAMESPACE
#  define NSCPM__XUL_EXTENSION_DEFAULT_NAMESPACE NSCPM_XUL_EXTENSION_DEFAULT_NAMESPACE
# else
#  define NSCPM__XUL_EXTENSION_DEFAULT_NAMESPACE "xex"
# endif

namespace nsxpcm {
	using str::string_;
	using str::string;

	// Rajoute une fen�tre pour pouvant �tre utilis� pour faciliter l'utilisation
	// de certaines fonctions ('GetJSConsole(...)', 'Alert(...)', ...).
	void AddMasterWindow( nsIDOMWindow *Window );

	// Retire 'Window' de la liste des 'MasterWindow's
	void RemoveMasterWindow( nsIDOMWindow *Window );

	// Retourne un 'MasterWindows'. 'RealeaseWindow' doit �tre appel� d�s que la 'Window' retourn�e n'est plus utilis�e.
	// Retourne 'NULL" si pas de fen�tre disponible.
	nsIDOMWindowInternal *RetrieveMasterWindow( void );

	void ReleaseMasterWindow( nsIDOMWindow *Window );

	// Log to the javascript console.
	void Log( const str::string_ &Text );

	// Log to the javascript console.
	inline void Log( const char *Text )
	{
		Log( str::string( Text ) );
	}


	// Si modifi�, modifier 'GetTranslation(...)' et le '.xlcl' en cons�quence.
	enum text__ {
		tXPRJFilterLabel,
		tDynamicLibraryFilterLabel,
		t_amount,
		t_Undefined
	};

	const char *GetLabel( text__ Text );

	enum event__
	{
		eCommand,
		eInput,
		eChange,
		eClick,
		eDblClick,
		eFocus,
		eBlur,
		eSelect,
		eAttributeChange,
		eKeyPress,
		eLoad,
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
		EF( Change ),
		EF( Click ),
		EF( DblClick ),
		EF( Focus ),
		EF( Blur ),
		EF( Select ),
		EF( AttributeChange ),
		EF( KeyPress ),
		EF( Load ),
		EF( Close ),
		ef_None = 0,
		ef_All = ( ( 1 << e_amount ) - 1 ),
		ef_AllButAnnoying = ef_All & ~efAttributeChange & ~efBlur & ~efFocus	// Pour faciliter le d�boguage.
	};

#undef EF

#ifdef NSXPCM__EF_BUFFER
#	define	EF	NSXPCM__EF_BUFFER
#endif


#ifdef NSXPCM__FBL
	using fblfrd::strings_;
	using fblfrd::strings;
#else
	typedef ctn::E_MCONTAINER_( string_ ) strings_;
	E_AUTO( strings );
#endif

	void Transform(
		const char *String,
		mdr::size__ Size,
		char **JString );

	void Transform(
		const char *String,
		char **JString );

	void Transform(
		const nsACString &CString,
		nsAString &WString );

	void Transform( 
		const char *String,
		nsAString &WString );

	void Transform(
		const nsAString &WString,
		nsACString &CString );

	void Transform( 
		const nsAString &WString,
		char **JString );

	void Transform(
		const str::string_ &String,
		char **JString );

	void Transform(
		const str::string_ &String,
		nsAString &WString );

	void TransformW(
		const str::string_ &String,
		nsAString &WString );

	void Transform( 
		const nsAString &WString,
		str::string_ &String );

	void TransformW( 
		const nsAString &WString,
		str::string_ &String );

	void Transform(
		const nsACString &CString,
		str::string_ &String );

	void Transform(
		const str::string_ &String,
		nsACString &CString );

	inline void Transform(
		const char *String,
		nsACString &CString );

	inline void Transform(
		const char *String,
		nsAString &WString )
	{
		nsCString CString;

		Transform( String, CString );
		NS_CStringToUTF16( CString, NS_CSTRING_ENCODING_NATIVE_FILESYSTEM , WString );
//		WString.AssignASCII( String );
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

	// 'ContractID' est une cha�ne de carat�re du genre "@mozilla.org/filepicker;1".
	template <typename t> inline t *GetService(
		const char *ContractID,
		nsCOMPtr<t> &Service )
	{	
		nsresult Error = NS_OK;

		Service = do_GetService( ContractID, &Error );

		T( Error );

		return Service;
	}

	// 'ContractID' est une cha�ne de carat�re du genre "@mozilla.org/filepicker;1".
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
		nsString EId;

		Transform( Id, EId );

		T( Document->GetElementById( EId, &Element ) );

		if ( Element == NULL )	// Chercher un �l�ment inexistant ne provoque pas d'erreur.
			ERRc();

		return Element;
	}

	inline nsIDOMElement *GetElementById(
		nsIDOMDocument *Document,
		const str::string_ &Id )
	{
		nsIDOMElement *Element = NULL;
		nsString EId;

		Transform( Id, EId );

		T( Document->GetElementById( EId, &Element ) );

		if ( Element == NULL )	// Chercher un �l�ment inexistant ne provoque pas d'erreur.
			ERRc();

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
		nsString WName;

		Transform( Name, WName );

#ifdef NSXPCM_DBG
		if ( Document == NULL )
			ERRu();
#endif

		T( Document->CreateElement( WName, &Element ) );

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
#else	// Fonctionne, mais pr�sente des risques ...
		return (element *)CreateElement( Document, Name );
#endif
	}

#ifdef NSXPCM_USE_DEPRECATED
	// 'name' nom du type 'doit correspondre au nom du tag XUL associ�); 'Name' pour le fonctions associ�es.
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
#endif 

	inline const str::string_ &GetName(
		nsIDOMNode *Node,
		str::string_ &Name )
	{
		nsString RawName;

#ifdef NSXPCM_DBG
		if ( Node == NULL )
			ERRc();
#endif

		T( Node->GetNodeName( RawName ) );

		Transform( RawName, Name );

		return Name;
	}

	template <typename t> inline const str::string_ &GetValue(
		t *Element,
		str::string_ &Value )
	{
		nsString WValue;

		T( Element->GetValue( WValue ) );

		Transform( WValue, Value );

		return Value;
	}

	template <typename t> inline const str::string_ &GetValueW(
		t *Element,
		str::string_ &Value )
	{
		nsString WValue;

		T( Element->GetValue( WValue ) );

		TransformW( WValue, Value );

		return Value;
	}

	inline void GetAttributeNameAndValue(
		nsIDOMAttr *Attribute,
		str::string_ &Name,
		str::string_ &Value )
	{
		GetName( Attribute, Name );
		GetValue( Attribute, Value );
	}

	inline void GetAttributeNameAndValueW(
		nsIDOMAttr *Attribute,
		str::string_ &Name,
		str::string_ &Value )
	{
		GetName( Attribute, Name );
		GetValueW( Attribute, Value );
	}

	inline void GetAttributeNameAndValue(
		nsIDOMNode *Attribute,
		str::string_ &Name,
		str::string_ &Value )
	{
		GetAttributeNameAndValue( nsxpcm::QueryInterface<nsIDOMAttr>( Attribute ), Name, Value );
	}

	inline void GetAttributeNameAndValueW(
		nsIDOMNode *Attribute,
		str::string_ &Name,
		str::string_ &Value )
	{
		GetAttributeNameAndValueW( nsxpcm::QueryInterface<nsIDOMAttr>( Attribute ), Name, Value );
	}

	inline bso::bool__ HasAttribute(
		nsIDOMElement *Element,
		const char *Name )
	{
		nsString WName;
		NSXPCM__BOOL Result;

		Transform( Name, WName );

#ifdef NSXPCM_DBG
		if ( Element == NULL )
			ERRu();
#endif
		
		T( Element->HasAttribute( WName, &Result ) );

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
		nsString WName, WValue;

		Transform( Name, WName );
		Transform( Value, WValue );

#ifdef NSXPCM_DBG
		if ( Element == NULL )
			ERRu();
#endif
		
		T( Element->SetAttribute( WName, WValue ) );
	}

	inline void SetAttribute(
		nsIDOMNode *Node,
		const char *Name,
		const char *Value )
	{
		SetAttribute( QueryInterface<nsIDOMElement>( Node ), Name, Value );
	}

	inline void SetAttribute(
		nsIDOMElement *Element,
		const char *Name,
		const str::string_ &Value )
	{
		nsString EName, EValue;

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

	inline void RemoveAttribute(
		nsIDOMElement *Element,
		const char *Name )
	{
		nsString WName;

		Transform( Name, WName );

#ifdef NSXPCM_DBG
		if ( Element == NULL )
			ERRu();
#endif
		
		T( Element->RemoveAttribute( WName ) );
	}

	inline void RemoveAttribute(
		nsIDOMNode *Node,
		const char *Name )
	{
		RemoveAttribute( QueryInterface<nsIDOMElement>( Node ), Name );
	}

	inline void SetAttributeState( 
		nsIDOMElement *Element,
		const char *Name,
		bso::bool__ Value )
	{
		if ( Value )
			SetAttribute( Element, Name, "true" );
		else
			RemoveAttribute( Element, Name );
	}

	inline void SetAttributeState(
		nsIDOMNode *Node,
		const char *Name,
		const bso::bool__ Value )
	{
		SetAttributeState( QueryInterface<nsIDOMElement>( Node ), Name, Value );
	}

	inline const str::string_ &GetAttribute(
		nsIDOMElement *Element,
		const char *Name,
		str::string_ &Value )
	{
		nsString WName, WValue;
		Transform( Name, WName );

		T( Element->GetAttribute( WName, WValue ) );

		Transform( WValue, Value );

		return Value;
	}

	inline const str::string_ &GetAttributeNS(
		nsIDOMElement *Element,
		const char *URI,
		const char *Name,
		str::string_ &Value )
	{
		nsString WURI, WName, WValue;
		Transform( URI, WURI );
		Transform( Name, WName );

		T( Element->GetAttributeNS( WURI, WName, WValue ) );

		Transform( WValue, Value );

		return Value;
	}

	inline const str::string_ &GetAttribute(
		nsIDOMNode *Node,
		const char *Name,
		str::string_ &Value )
	{
 		return GetAttribute( QueryInterface<nsIDOMElement>( Node ), Name, Value );
	}

	inline const str::string_ &GetAttributeNS(
		nsIDOMNode *Node,
		const char *URI,
		const char *Name,
		str::string_ &Value )
	{
 		return GetAttributeNS( QueryInterface<nsIDOMElement>( Node ), URI, Name, Value );
	}

	inline bso::bool__ GetAttributeState(
		nsIDOMElement *Element,
		const char *Name )
	{
		bso::bool__ State = false;
	ERRProlog
		str::string Value;
	ERRBegin
		Value.Init();
		GetAttribute( Element, Name, Value );

		if ( Value == "true" )
			State = true;
	ERRErr
	ERREnd
	ERREpilog
		return State;
	}

	inline bso::bool__ GetAttributeState(
		nsIDOMNode *Node,
		const char *Name )
	{
 		return GetAttributeState( QueryInterface<nsIDOMElement>( Node ), Name );
	}

	template <typename t> inline void Focus( t *Element )
	{
		T( Element->Focus() );
	}

	template <typename t> inline void Blur( t *Element )
	{
		T( Element->Blur() );
	}


	template <typename t> inline void SetValue(
		t *Element,
		const str::string_ &Value )
	{
		nsString WValue;

		Transform( Value, WValue );

#ifdef NSXPCM_DBG
		if ( Element == NULL )
			ERRu();
#endif
		
		T( Element->SetValue( WValue ) );
	}

	template <typename t> inline void SetValueW(
		t *Element,
		const str::string_ &Value )
	{
		nsString WValue;

		TransformW( Value, WValue );

#ifdef NSXPCM_DBG
		if ( Element == NULL )
			ERRu();
#endif
		
		T( Element->SetValue( WValue ) );
	}

	inline bso::slong__ GetSelectedIndex( nsIDOMXULSelectControlElement *Element )
	{
		PRInt32 Index;

		T( Element->GetSelectedIndex( &Index ) );

		return Index;
	}

	inline bso::slong__ GetSelectedIndex( nsIDOMNode *Node )
	{
		return GetSelectedIndex( QueryInterface<nsIDOMXULSelectControlElement>( Node ) );
	}
	
	inline void SetSelectedIndex(
		nsIDOMXULSelectControlElement *Element,
		bso::slong__ Index )
	{
		T( Element->SetSelectedIndex( Index ) );
	}

	inline void SetSelectedIndex(
		nsIDOMNode *Node,
		bso::slong__ Index )
	{
		SetSelectedIndex( QueryInterface<nsIDOMXULSelectControlElement>( Node ), Index );
	}

	inline bso::slong__ GetSelectedIndexAttribute( nsIDOMNode *Node )
	{
		bso::slong__ Index = -1;
	ERRProlog
		str::string Value;
		mdr::row__ Error = NONE;
	ERRBegin
		Value.Init();
		nsxpcm::GetAttribute( Node, "selectedIndex", Value );

		Index = Value.ToSL( &Error );

		if ( Error != NONE )
			ERRs();
	ERRErr
	ERREnd
	ERREpilog
		return Index;
	}

	inline void SetSelectedIndexAttribute(
		nsIDOMNode *Node,
		bso::slong__ Index )
	{
		bso::integer_buffer__ Buffer;

		nsxpcm::SetAttribute( Node, "selectedIndex", bso::Convert( Index, Buffer ) );
	}

	void SetSelectedItem(	// Cherche parmis les enfants de 'Node' le premier �l�ment ayant l'attribut 'xex:selected' � 'true' et le s�lectionne.
		nsIDOMNode *Node,
		bso::bool__ SelectFirstOneIfNone = true );	// Lorsque � 'true', si aucun 'item' n'est marqu� comme s�lectionn�, s�lectionne le premier.

	inline nsIDOMXULSelectControlItemElement *GetSelectedItem( nsIDOMXULSelectControlElement *Element )
	{
		nsIDOMXULSelectControlItemElement *Item;

		T( Element->GetSelectedItem( &Item ) );

		return Item;
	}

	inline nsIDOMXULSelectControlItemElement *GetSelectedItem( nsIDOMNode *Node )
	{
		return GetSelectedItem( QueryInterface<nsIDOMXULSelectControlElement>( Node ) );
	}

	inline void SetSelectedItem(
		nsIDOMXULSelectControlElement *Element,
		nsIDOMXULSelectControlItemElement *Item )
	{
		T( Element->SetSelectedItem( Item ) );
	}

	inline void SetSelectedItem(
		nsIDOMNode *Node,
		nsIDOMNode *Item )
	{
		if ( Item != NULL )
			SetSelectedItem( QueryInterface<nsIDOMXULSelectControlElement>( Node ), QueryInterface<nsIDOMXULSelectControlItemElement>( Item ) );
		else
			SetSelectedItem( QueryInterface<nsIDOMXULSelectControlElement>( Node ), NULL );
	}

	inline const str::string_ &GetId(
		nsIDOMNode *Node,
		str::string_ &Value )
	{
		return GetAttribute( Node, "id", Value );
	}


	inline void CloneNode(
		nsIDOMNode *Node,
		bso::bool__ Deep,
		nsIDOMNode **Clone )
	{
		T( Node->CloneNode( Deep, Clone ) );
	}

	inline void ReplaceChild(
		nsIDOMNode *Node,
		nsIDOMNode *OldChild,
		nsIDOMNode *NewChild )
	{
		nsIDOMNode *Dummy = NULL;

#ifdef NSXPCM_DBG
		if ( Node == NULL )
			ERRu();

		if ( OldChild == NULL )
			ERRu();

		if ( NewChild == NULL )
			ERRu();
#endif
		T( Node->ReplaceChild( NewChild, OldChild, &Dummy ) );
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
		NSXPCM__BOOL Result;

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
		return GetName( Node, Name );
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

			_Current = GetFirstChild( Root );

			if ( _Current != NULL )
				_Root = _Current = GetParentNode( _Current );	// Bien que, dans ce cas, '_Root' soit le m�me que 'Root,
																// ils peuvent ne pas avoir la m�me valeur,
																// posant alors probl�me pour pour 'GetNext()'.
		}
		// Retourne le prochain noeud, ou 'NULL' si tous ont �t� lus.
		nsIDOMNode *GetNext( void );
	};

/*
	template <typename element> bso::slong__ GetSelectedIndex( element *Element )
	{
		PRInt32 Index;

		T( Element->GetSelectedIndex( &Index ) );

		return Index;
	}
*/
	template <typename element> inline bool IsChecked( element *Element )
	{
		NSXPCM__BOOL Checked;

		T( Element->GetChecked( &Checked ) );

		return Checked != 0;
	}

	template <typename element> inline void SetChecked(
		element *Element,
		bso::bool__ State )
	{
		T( Element->SetChecked( State ) );
	}

#if 0
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
#endif

	nsIDOMNode *_FindParent(
		nsIDOMNode *Node,
		const str::string_ &NodeName );

#if 0
	inline nsIDOMWindowInternal *GetWindowInternal( nsIDOMWindow *Window )
#else
	inline nsIDOMWindowInternal *GetWindowInternal( nsISupports *Window )
#endif
	{
# ifdef NSXPCM__USE_ARMEL_WORKAROUND
		return (nsIDOMWindowInternal *)Window;
# else
		return nsxpcm::QueryInterface<nsIDOMWindowInternal>( Window );
# endif
	}

	inline nsIDOMWindowInternal* GetParent( nsIDOMWindow *Window )
	{
		T( Window->GetParent( &Window ) );

		return GetWindowInternal( Window );
	}

	inline nsIDOMWindowInternal* GetTop( nsIDOMWindow *Window )
	{
		T( Window->GetTop( &Window ) );

		return GetWindowInternal( Window );
	}

	inline nsIDOMWindowInternal* GetOpener( nsIDOMWindowInternal *Window )
	{
# ifdef NSXPCM__GECKO_V8
		T( Window->GetOpener( (nsIDOMWindow **)&Window ) );
# else
		T( Window->GetOpener( &Window ) );
#endif

		return Window;
	}

	inline nsIDOMWindowInternal* GetOpener( nsIDOMWindow *Window )
	{
		return GetOpener( GetWindowInternal( Window ) );
	}

	nsIDOMWindowInternal* GetRoot( nsIDOMWindow *Window );	// Retourne la fen�tre racine de l'application.

	inline nsIDOMElement *GetElement( nsIDOMDocument *Document )
	{
		nsIDOMElement *Element = NULL;

		T( Document->GetDocumentElement( &Element ) );

		return Element;
	}

	inline nsIDOMDocument *GetDocument( nsIDOMWindow *Window )
	{
		nsIDOMDocument *Document = NULL;

		T( Window->GetDocument( &Document ) );

		return Document;
	}

	inline nsIDOMElement *GetElement( nsIDOMWindow *Window )
	{
		return GetElement( GetDocument( Window ) );
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
		NSXPCM__BOOL Value = false;

		T( Window->GetClosed( &Value ) );

		return Value == PR_TRUE;
	}

	inline bso::bool__ IsClosed( nsIDOMWindow *Window )
	{
		return IsClosed( GetWindowInternal( Window ) );
	}

	inline void Close( nsIDOMWindowInternal *Window )
	{
		T( Window->Close() );
	}

	inline void Close( nsIDOMWindow *Window )
	{
		Close( GetWindowInternal( Window ) );
	}

	typedef nsISupports *nsISupportsPointer;
	typedef nsIDOMElement *nsIDOMElementPointer;
	typedef nsIDOMWindow *nsIDOMWindowPointer;
	typedef nsIDOMWindowInternal *nsIDOMWindowInternalPointer;

	class ui_core__ {
	private:
		nsIDOMWindowInternal *_Window;
	public:
		void reset( bso::bool__ = true )
		{
			_Window = NULL;
		}
		E_CVDTOR( ui_core__ )
		void Init( nsIDOMWindowInternal *Window )
		{
			_Window = Window;
		}
		void Init( nsIDOMWindow *Window )
		{
			Init( GetWindowInternal( Window ) );
		}
		E_RODISCLOSE__( nsIDOMWindowInternalPointer, Window );
		nsIDOMDocument *Document( void ) const
		{
			return GetDocument( _Window );
		}
	};


	typedef bso::slong__ event_imbrication_level__;
#define NSXPCM__EVENT_IMBRICATION_LEVEL_MAX	BSO_SLONG_MAX

	class event_data__
	{
	private:
		nsCOMPtr<class event_listener__> _EventListener;
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
		nsIDOMEvent &RawEvent( void ) const
		{
			return *_RawEvent;
		}
		nsIDOMMutationEvent &MutationEvent( void )
		{
			return *_MutationEvent;
		}
		nsIDOMKeyEvent &KeyEvent( void ) const
		{
			return *_KeyEvent;
		}
		nsIDOMElement *GetTarget( void ) const
		{
			nsIDOMEventTarget *EventTarget = NULL;

			RawEvent().GetTarget( &EventTarget );

			return QueryInterface<nsIDOMElement>( EventTarget );
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
		friend class event_handler__;
	};

	// ATTENTION : cet objet ne doit PAS �tre stock� dans un 'bunch' ou un 'container', car il est r�f�renc� par son adresse !
	class event_handler__ {
	private:
		event_handler__ *_Control;
		void _Test( bso::bool__ IgnoreInitializationTest );	// V�rifie que l'objet n'a pas �t� d�plac� depuis sa cr�ation (voir remarque ci-dessus)
															// V�rifie �galement si l'obeta �t� initialis� (losque 'IgnoreInitializationTest' == 'false' ).
		event_data__ _EventData;
		void _OnErr( void )
		{
			if ( ERRType != err::t_None ) {
				err::buffer__ Buffer;

				NSXPCMOnErr( err::Message( Buffer )  );
			} else
				NSXPCMOnErr( NULL );
		}
		void _ErrHandling( void )
		{
			switch ( ERRType ) {
			case err::t_Exit:
				break;
			case err::t_Return:
				ERRc();
				break;
			case err::t_Abort:
				_OnErr();
				break;
			default:
				_OnErr();
				break;
			}

			if ( ERRType != err::t_None )
				ERRRst()
		}
		void _OnEvent(
			event_data__ &Data,
			event__ Event )
		{
		ERRProlog
		ERRBegin
			NSXPCMOnEvent( Data, Event );
		ERRErr
			_ErrHandling();
		ERREnd
		ERREpilog
		}
	protected:
		event_data__ &EventData( void )
		{
			return _EventData;
		}
		virtual void NSXPCMOnEvent(
			event_data__ &EventData,
			event__ Event )
		{
			NSXPCMOnEvent( Event );
		}
		virtual void NSXPCMOnEvent(	event__ Event )
		{
			ERRu();
		}
		virtual void NSXPCMOnErr( const char *Message ) = 0;	// Appel� lors d'un 'ERR...(...)'.
																/* ATTENTION : si 'Message' == 'NULL', alors les informations relatives � l'erreur ne sont pas disponibles
																(erreur lanc�e � partir d'une biblioth�que dynamique, par exemple), et on doit se d�brouller pour les r�cup�rer
																et faire une remise � 0 de l'erreur. */
	public:
		void reset( bso::bool__ P = true )
		{
			if ( !P )
				_Control = this;

			_EventData.reset( P );
		}
		E_CVDTOR( event_handler__ );
		void Init( void );
		void Add(
			nsISupports *Supports,
			int Events );
		void Add(
			nsIDOMWindow *Window,
			const str::string_ &Id,
			int Events )
		{
			Add( nsxpcm::GetElementById( GetDocument( Window ), Id ), Events );
		}
		void Add(
			nsIDOMWindow *Window,
			const char *Id,
			int Events )
		{
			Add( Window, str::string( Id ), Events );
		}
		bso::bool__ Handle( nsIDOMEvent *RawEvent );
		int SetEventsToIgnore( int Events )
		{
			return _EventData.SetEventsToIgnore( Events );
		}
		int AddEventsToIgnore( int Events )
		{
			return _EventData.AddEventsToIgnore( Events );
		}
	};

	void Attach(
		nsIDOMNode *Node,
		const char *Id,
		event_handler__ &EventHandler,
		const char *NameSpace = NSCPM__XUL_EXTENSION_DEFAULT_NAMESPACE );

	struct supports__ 
	{
		nsIDOMWindow *Window;
		nsISupports *Supports;
		supports__(
			nsISupports *Supports,
			nsIDOMWindow *Window )
		{
			this->Window  = Window;
			this->Supports = Supports;
		}
		supports__(
			nsIDOMWindow *Window,
			const str::string_ &Id )
		{
			this->Window  = Window;
			Supports = nsxpcm::GetElementById( nsxpcm::GetDocument( Window ), Id );
		}
		supports__(
			nsIDOMWindow *Window,
			const char *Id )
		{
			this->Window  = Window;
			Supports = nsxpcm::GetElementById( nsxpcm::GetDocument( Window ), Id );
		}
	};

	class widget_core__
	{
	private:
		nsIDOMWindowInternal *_Window;
		nsISupports *_Supports;
	public:
		void reset( bso::bool__ P = true )
		{
			_Window = NULL;
			_Supports = NULL;
		}
		widget_core__( void )
		{
			reset( false );
		}
		virtual ~widget_core__( void )
		{
			reset( );
		}
		void Init( const supports__ &Supports )
		{
# ifdef NSXPCM_DBG
			if ( _Supports != NULL )
				ERRu();
# endif
			reset();

			_Window = GetWindowInternal( Supports.Window );
			_Supports = Supports.Supports;
		}
		E_RODISCLOSE__( nsISupportsPointer, Supports );
		nsIDOMWindowInternal *GetWindow( void ) const
		{
			return _Window;
		}
		nsIDOMWindowInternal *Window( void ) const
		{
			return _Window;
		}
		nsIDOMDocument *GetDocument( void )
		{
			return nsxpcm::GetDocument( GetWindow() );
		}
		nsIDOMDocument *Document( void )
		{
			return nsxpcm::GetDocument( GetWindow() );
		}
		nsIDOMWindowInternal *GetParent( void ) const
		{
			return nsxpcm::GetParent( _Window );
		}
		nsIDOMWindowInternal *GetTop( void ) const
		{
			return nsxpcm::GetTop( _Window );
		}
		nsIDOMWindowInternal *GetRoot( void ) const
		{
			return nsxpcm::GetRoot( _Window );
		}
		nsIDOMWindowInternal *GetOpener( void ) const
		{
			return nsxpcm::GetOpener( _Window );
		}
		bso::bool__ Handle( nsIDOMEvent *Event );
		nsIDOMElement *GetElement( void )
		{
			if ( _Window == _Supports )	// Si 'true', alors le 'widget' est une 'window' et 'QueryInterface...' ne fonctionne pas.
				return nsxpcm::GetElement( _Window );
			else
				return QueryInterface<nsIDOMElement>( _Supports );
		}
		nsIDOMNode *GetNode( void )
		{
			return QueryInterface<nsIDOMNode>( GetElement() );
		}
		operator nsIDOMElement*( void )
		{
			return GetElement();
		}
		operator nsIDOMNode*( void )
		{
			return GetNode();
		}
	};

	E_ROW( row__ );

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

	// Supprime (d�salloue) tous les �l�ments stock�s dans 'Cores'.
	void Delete( widget_cores_ &Widgets );
/*
	void Handle( 
		nsIDOMEvent *Event,
		const element_cores_ &Cores );
*/

	template <typename widget> class _widget__
	: public widget_core__
	{
	protected:
		// ATTENTION : selon le stade de construction de l'UI, l'interface demand�e peut ne pas �tre encore disponible.
		// De ce fait, utilser, d�s que possible, 'GetElement()' � la place.
		widget *GetWidget( void )
		{
			return QueryInterface<widget>( GetSupports() );
		}
	public:
		operator widget *( void )
		{
			return GetWidget();
		}
		bso::bool__ HasAttribute( const char *Name )
		{
			return nsxpcm::HasAttribute( GetElement(), Name );
		}
		const str::string_ &GetAttribute(
			const char *Name,
			str::string_ &Value )
		{
			return nsxpcm::GetAttribute( GetElement(), Name, Value );
		}
		void SetAttribute(
			const char *Name,
			str::string_ &Value )
		{
			nsxpcm::SetAttribute( GetElement(), Name, Value );
		}
		void SetAttribute(
			const char *Name,
			const char *Value )
		{
			nsxpcm::SetAttribute( GetElement(), Name, Value );
		}
		void SetAttributeState(
			const char *Name,
			bso::bool__ State )
		{
			nsxpcm::SetAttributeState( GetElement(), Name, State );
		}
		bso::bool__ GetAttributeState( const char *Name )
		{
			return nsxpcm::GetAttributeState( GetElement(), Name );
		}
		void RemoveAttribute( const char *Name )
		{
			nsxpcm::RemoveAttribute( GetElement(), Name );
		}
		const str::string_ &GetId( str::string_ &Value )
		{
			return nsxpcm::GetId( GetElement(), Value );
		}
		void Enable( bso::bool__ Value = true )
		{
			SetAttributeState( "disabled", !Value );
		}
		void Disable( bso::bool__ Value = true )
		{
			Enable( !Value );
		}
		bso::bool__ IsEnabled( void )
		{
			return !GetAttributeState( "disabled" );
		}
		bso::bool__ IsDisabled( void )
		{
			return !IsEnabled();
		}
		void Show( bso::bool__ Value = true )
		{
			SetAttributeState( "hidden", !Value );
		}
		void Hide( bso::bool__ Value = true )
		{
			Show( !Value );
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
			nsxpcm::RemoveChildren( GetElement() );
		}
		void AppendChild( nsIDOMNode *Child )
		{
			nsxpcm::AppendChild( GetElement(), Child );
		}
		void ReplaceChild( nsIDOMNode *NewChild )
		{
# ifdef NSXPCM_DBG
			if ( GetFirstChild( GetElement() ) != GetLastChild( GetElement() ) )
				ERRc();
# endif
			if ( GetFirstChild( GetElement() ) == NULL )
				AppendChild( NewChild );
			else
				nsxpcm::ReplaceChild( GetElement(), GetFirstChild( GetElement() ), NewChild );
		}
	};

# define NSXPCM_HANDLE_BOOLEAN_ATTRIBUTE( affix, name )\
	void Enable##affix( bso::bool__ Value = true )\
	{\
		SetAttributeState( name, Value );\
	}\
	void Disable##affix( bso::bool__ Value = true )\
	{\
		SetAttributeState( name, !Value );\
	}\
	bso::bool__ Is##affix##Enabled( void )\
	{\
		return GetAttributeState( name );\
	}\
	bso::bool__ Is##affix##Disabled( void )\
	{\
		return !GetAttributeState( name );\
	}\

# define NSXPCM__FOCUS_HANDLING\
		void Focus( void )\
		{\
			nsxpcm::Focus( GetWidget() );\
		}\
		void Blur( void )\
		{\
			nsxpcm::Blur( GetWidget() );\
		}\

# define NSXPCM__VALUE_HANDLING\
		void SetValue( const str::string_ &Value )\
		{\
			nsxpcm::SetValue( GetWidget(), Value );\
		}\
		void SetValueW( const str::string_ &Value )\
		{\
			nsxpcm::SetValueW( GetWidget(), Value );\
		}\
		const str::string_ &GetValue( str::string_ &Value )\
		{\
			nsxpcm::GetValue( GetWidget(), Value );\
		\
			return Value;\
		}\
		const str::string_ &GetValueW( str::string_ &Value )\
		{\
			nsxpcm::GetValueW( GetWidget(), Value );\
		\
			return Value;\
		}\

# define NSXPCM__INDEX_HANDLING\
		void SetSelectedIndex( bso::slong__ Index )\
		{\
			nsxpcm::SetSelectedIndex( GetWidget(), Index );\
		}\
		bso::slong__ GetSelectedIndex( void )\
		{\
			return nsxpcm::GetSelectedIndex( GetWidget() );\
		}

# define NSXPCM__INDEX_ATTRIBUTE_HANDLING\
		void SetSelectedIndex( bso::slong__ Index )\
		{\
			nsxpcm::SetSelectedIndexAttribute( GetWidget(), Index );\
		}\
		bso::slong__ GetSelectedIndex( void )\
		{\
			return nsxpcm::GetSelectedIndexAttribute( GetWidget() );\
		}

# define NSXPCM__ITEM_HANDLING\
		void SetSelectedItem( nsIDOMXULSelectControlItemElement *Item )\
		{\
			nsxpcm::SetSelectedItem( GetWidget(), Item );\
		}\
		void SetSelectedItem( nsIDOMNode *Item )\
		{\
			nsxpcm::SetSelectedItem( GetWidget(), Item );\
		}\
		void SetSelectedItem( widget_core__ &Item )\
		{\
			nsxpcm::SetSelectedItem( GetWidget(), Item.GetElement() );\
		}\
		nsIDOMXULSelectControlItemElement *GetSelectedItem( void )\
		{\
			return nsxpcm::GetSelectedItem( GetWidget() );\
		}


	class widget__	// Classe g�n�rique.
	: public _widget__<nsIDOMElement>
	{};

	class button__
	: public _widget__<nsIDOMElement>	// Devrait normalement �tre '<nsIButton>', mais l'inclusion de 'nsIButton.h' pose probl�me.
	{};

	class textbox__
	: public _widget__<nsIDOMXULTextBoxElement>
	{
	public:
		NSXPCM__VALUE_HANDLING
		NSXPCM__FOCUS_HANDLING
		void Select( void )
		{
			T( GetWidget()->Select() );
		}
		void SetSize( bso::size__ Size )
		{
			T( GetWidget()->SetSize( Size ) );
		}
		bso::size__ GetSize( void )
		{
			PRInt32 Buffer;

			T( GetWidget()->GetSize( &Buffer )  );

			return Buffer;
		}
		void SetMaxLength( bso::size__ Size )
		{
			T( GetWidget()->SetMaxLength( Size ) );
		}
		bso::size__ GetMaxLength( void )
		{
			PRInt32 Buffer;

			T( GetWidget()->GetMaxLength( &Buffer ) );

			return Buffer;
		}
	};

	class description__
	: public _widget__<nsIDOMXULDescriptionElement>
	{
	public:
		NSXPCM__VALUE_HANDLING
	};

	class label__
	: public _widget__<nsIDOMXULLabelElement>
	{
	public:
		NSXPCM__VALUE_HANDLING
	};

	class checkbox__
	: public _widget__<nsIDOMXULCheckboxElement>
	{
	public:
		bso::bool__ IsChecked( void )
		{
			return nsxpcm::IsChecked( GetWidget() );
		}
		void Check( bso::bool__ Value = true )
		{
			nsxpcm::SetChecked( GetWidget(), Value );
		}
		void Uncheck( bso::bool__ Value = true )
		{
			Check( !Value );
		}
	};

	class radio__
	: public _widget__<nsIDOMXULButtonElement>
	{
	public:
		bso::bool__ IsChecked( void )
		{
			return nsxpcm::IsChecked( GetWidget() );
		}
		void Check( bso::bool__ Value = true )
		{
			nsxpcm::SetChecked( GetWidget(), Value );
		}
		void Uncheck( bso::bool__ Value = true )
		{
			Check( !Value );
		}
	};

	class radiogroup__
	: public _widget__<nsIDOMXULSelectControlElement>
	{
	public:
		NSXPCM__INDEX_HANDLING
		NSXPCM__ITEM_HANDLING
		NSXPCM__FOCUS_HANDLING
		const str::string_ &GetSelectedItemValue( str::string_ &Value )
		{
			nsIDOMXULSelectControlItemElement *Item = GetSelectedItem();

			if ( Item != NULL )
				GetValue( Item, Value );

			return Value;
		}
		const str::string_ &GetSelectedItemValueW( str::string_ &Value )
		{
			nsIDOMXULSelectControlItemElement *Item = GetSelectedItem();

			if ( Item != NULL )
				GetValueW( Item, Value );

			return Value;
		}
	};

	class listitem__
	: public _widget__<nsIDOMXULSelectControlItemElement>
	{};

	class listbox__
	: public _widget__<nsIDOMXULMultiSelectControlElement>
	{
	public:
		void SelectListboxItem( nsIDOMXULSelectControlItemElement *Item )
		{
			PRInt32 Index = -1;
			nsIBoxObject *BoxObject = NULL;
			nsIListBoxObject *ListBoxObject = NULL;

			T( GetWidget()->GetBoxObject( &BoxObject ) );

			ListBoxObject = QueryInterface<nsIListBoxObject>( BoxObject );

			T( ListBoxObject->GetIndexOfItem( Item, &Index ) );
			T( ListBoxObject->ScrollToIndex( Index ) );

			T( GetWidget()->SelectItem( Item ) );
		}
		nsIDOMXULSelectControlItemElement *GetCurrentItem( bso::bool__ ErrorIfInexistant = true )
		{
			nsIDOMXULSelectControlItemElement *Item = NULL;

			T( GetWidget()->GetCurrentItem( &Item ) );

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

			T( GetWidget()->GetSelectedCount( &Count ) );

			return Count;
		}
		void RemoveContent( void );
	};

	class deck__
	: public _widget__<nsIDOMElement>
	{
	private:
	public:
#if 1
		NSXPCM__INDEX_ATTRIBUTE_HANDLING
		void SetSelectedItem( nsIDOMNode *Node )
		{
			bso::slong__ Index = -1;

			while ( Node != NULL ) {
				Index++;
				Node = GetPreviousSibling( Node );
			}

			SetSelectedIndex( Index );
		}
		void SetSelectedItem( _widget__<nsIDOMNode> &Item )
		{
			SetSelectedItem( Item.GetElement() );
		}
#else
		NSXPCM__INDEX_HANDLING
		NSXPCM__ITEM_HANDLING
#endif
	};

	class tabs__
	: public _widget__<nsIDOMElement>
	{
	public:
		NSXPCM__INDEX_HANDLING
	};

	class tabpanels__
	: public _widget__<nsIDOMElement>
	{};

	// NOTA : pas de widget 'tabbox', car il ne sert � rien.

	class tree__
	: public _widget__<nsIDOMXULTreeElement>
	{
	private:
		nsCOMPtr<class tree_view__> _TreeView;
		void _SetTreeView( class tree_view_callback__ &Callback );
		nsITreeView *_GetView( void )
		{
			nsITreeView *View = NULL;

			T( GetWidget()->GetView( &View ) );

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
		void Init(
			class tree_view_callback__ &Callback,
			const supports__ &Supports );
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
		/* Par d�faut (avec 'SkipSelectEvent' � false, un 'ClearSelection' lance un �v�nement 'select'.
		Cela peut �tre �vit� en mettant 'SkipSelectEvent' � 'true'. */
#if 0
		void ClearSelection( bso::bool__ SkipSelectEvent )
		{
			int Buffer = AddEventsToIgnore( SkipSelectEvent ? efSelect : ef_None );

			T( _GetSelection()->ClearSelection() );

			SetEventsToIgnore( Buffer );
		}
#endif
	};

	class broadcaster__
	: public _widget__<nsIDOMElement>
	{};

	class command__
	: public _widget__<nsIDOMElement>
	{};


	class anchor__
	: public _widget__<nsIDOMHTMLAnchorElement>
	{};

	class menu__
	: public _widget__<nsIDOMElement>	// Pas cherch� le 'nsI...' correspondant ...
	{};

	class menuitem__
	: public _widget__<nsIDOMXULSelectControlItemElement>
	{
	public:
		NSXPCM__VALUE_HANDLING
	};

	class menulist__
	: public _widget__<nsIDOMXULMenuListElement>
	{
	public:
		NSXPCM__VALUE_HANDLING
		NSXPCM__INDEX_HANDLING
		NSXPCM__ITEM_HANDLING
	};

/*
	class document__
	: public _widget__<nsIDOMDocument>
	{};
*/
	class window__
	: public _widget__<nsIDOMWindowInternal>
	{
	public:
		void Close( void )
		{
			nsxpcm::Close( GetWidget() );
		}
		bso::bool__ IsClosed( void )
		{
			return nsxpcm::IsClosed( GetWidget() );
		}
	};

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

	typedef ctn::E_CONTAINER_( file_picker_filter_ ) file_picker_filters_;
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
		fpmDynamicLibrary,
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
		MF( DynamicLibrary ),
		fpmf_None = 0,
		fpmf_All = ( ( 1 << fpm_amount ) - 1 )	// Ne pas confondre avec 'fpmfAll', d�signat que l'on d�sire tous les fichiers (filtre '*.*').
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
			const str::string_ &Mask );	// Pour ajouter un filtre ne faisant pas partie de ceux pr�d�finis.
		/* Retourne 'true' si un fichier a �t� s�lectionn� ('FileName' contient alors le fichier),
		'false' si 'Cancel' a �t� s�lectionn�. */
		bso::bool__ Show(
			nsIDOMWindow *ParentWindow,	// Cette fonction bloque toute action sur 'ParentWindow', donc pas possibilit� d'utiliser 'MasterWindow'.
			file_picker_type__ Type,
			const char *DefaultExtension,
			const lcl::locale_ &Locale,
			const char *Language,
			str::string_ &FileName );
		bso::bool__ Show(
			nsIDOMWindow *ParentWindow,	// Cette fonction bloque toute action sur 'ParentWindow', donc pas possibilit� d'utiliser 'MasterWindow'.
			file_picker_type__ Type,
			const lcl::locale_ &Locale,
			const char *Language,
			str::string_ &FileName )
		{
			return Show( ParentWindow, Type, NULL, Locale, Language, FileName );
		}
		E_RODISCLOSE_( int, PredefinedFilters );
	};

	E_AUTO( file_picker );

	/* Retourne 'true' si un fichier a �t� s�lectionn� ('FileName' contient alors le fichier),
	'false' si 'Cancel' a �t� s�lectionn�. */
	bso::bool__ FileOpenDialogBox(
		nsIDOMWindow *ParentWindow,	// Fonction bloquante, dont 'ParentWindow' ne peut �tre d�duit de 'MasterWindow'.
		const str::string_ &Title,
		const char *DefaultExtension,
		int PredefinedFilters,
		const lcl::locale_ &Locale,
		const char *Language,
		str::string_ &FileName );

	/* Retourne 'true' si un fichier a �t� s�lectionn� ('FileName' contient alors le fichier),
	'false' si 'Cancel' a �t� s�lectionn�. */
	inline bso::bool__ HTMLFileOpenDialogBox(
		nsIDOMWindow *ParentWindow,	// Fonction bloquante, dont 'ParentWindow' ne peut �tre d�duit de 'MasterWindow'.
		const str::string_ &Title,
		const lcl::locale_ &Locale,
		const char *Language,
		str::string_ &FileName )
	{
		return FileOpenDialogBox( ParentWindow, Title, "html", fpmfHTML, Locale, Language, FileName );
	}

	/* Retourne 'true' si un fichier a �t� s�lectionn� ('FileName' contient alors le fichier),
	'false' si 'Cancel' a �t� s�lectionn�. */
	inline bso::bool__ XPRJFileOpenDialogBox(
		nsIDOMWindow *ParentWindow,	// Fonction bloquante, dont 'ParentWindow' ne peut �tre d�duit de 'MasterWindow'.
		const str::string_ &Title,
		const lcl::locale_ &Locale,
		const char *Language,
		str::string_ &FileName )
	{
		return FileOpenDialogBox( ParentWindow, Title, "xprj", fpmfXPRJ, Locale, Language, FileName );
	}

# ifdef CPE__MS
#  define NSXPCM__DYNAMIC_LIBRARY_EXTENSION "dll"
# elif defined( CPE__LINUX )
#  define NSXPCM__DYNAMIC_LIBRARY_EXTENSION "so"
# elif defined( CPE__MAC )
#  define NSXPCM__DYNAMIC_LIBRARY_EXTENSION "dylib"
# else
#  error "Unknown platform !"
# endif

	/* Retourne 'true' si un fichier a �t� s�lectionn� ('FileName' contient alors le fichier),
	'false' si 'Cancel' a �t� s�lectionn�. */
	inline bso::bool__ DynamicLibraryFileOpenDialogBox(
		nsIDOMWindow *ParentWindow,	// Fonction bloquante, dont 'ParentWindow' ne peut �tre d�duit de 'MasterWindow'.
		const str::string_ &Title,
		const lcl::locale_ &Locale,
		const char *Language,
		str::string_ &FileName )
	{
		return FileOpenDialogBox( ParentWindow, Title, NSXPCM__DYNAMIC_LIBRARY_EXTENSION, fpmfDynamicLibrary, Locale, Language, FileName );
	}

# undef NSXPCM__EXT

	/* Retourne 'true' si un fichier a �t� s�lectionn� ('FileName' contient alors le fichier),
	'false' si 'Cancel' a �t� s�lectionn�. */
	bso::bool__ FileSaveDialogBox(
		nsIDOMWindow *ParentWindow,	// Fonction bloquante, dont 'ParentWindow' ne peut �tre d�duit de 'MasterWindow'.
		const str::string_ &Title,
		const char *DefaultExtension,
		int PredefinedFilters,
		const lcl::locale_ &Locale,
		const char *Language,
		str::string_ &FileName );

	/* Retourne 'true' si un fichier a �t� s�lectionn� ('FileName' contient alors le fichier),
	'false' si 'Cancel' a �t� s�lectionn�. */
	inline bso::bool__ HTMLFileSaveDialogBox(
		nsIDOMWindow *ParentWindow,	// Fonction bloquante, dont 'ParentWindow' ne peut �tre d�duit de 'MasterWindow'.
		const str::string_ &Title,
		const lcl::locale_ &Locale,
		const char *Language,
		str::string_ &FileName )
	{
		return FileSaveDialogBox( ParentWindow, Title, "html", fpmfHTML, Locale, Language, FileName );
	}

	/* Retourne 'true' si un fichier a �t� s�lectionn� ('FileName' contient alors le fichier),
	'false' si 'Cancel' a �t� s�lectionn�. */
	inline bso::bool__ XPRJFileSaveDialogBox(
		nsIDOMWindow *ParentWindow,	// Fonction bloquante, dont 'ParentWindow' ne peut �tre d�duit de 'MasterWindow'.
		const str::string_ &Title,
		const lcl::locale_ &Locale,
		const char *Language,
		str::string_ &FileName )
	{
		return FileSaveDialogBox( ParentWindow, Title, "xprj", fpmfXPRJ, Locale, Language, FileName );
	}

	/* Retourne 'true' si un r�pertoire a �t� s�lectionn� ('DirectoryName' contient alors le r�petoire),
	'false' si 'Cancel' a �t� s�lectionn�. */
	bso::bool__ DirectorySelectDialogBox(
		nsIDOMWindow *ParentWindow,	// Fonction bloquante, dont 'ParentWindow' ne peut �tre d�duit de 'MasterWindow'.
		const str::string_ &Title,
		const lcl::locale_ &Locale,
		const char *Language,
		str::string_ &DirectoryName );

	inline void OpenWindow(
		nsIDOMWindow *ParentWindow,
		const char *URL,
		const char *Name,
		nsIDOMWindow **Window = NULL )
	{
	ERRProlog
		nsIDOMWindow **WindowBuffer = NULL, *DummyWindow = NULL, *MasterWindow = NULL;
		nsString TransformedURL, TransformedName;
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
		nsString TransformedURL, TransformedName;
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

		if ( ParentWindow == NULL )
			ERRu();	// Fonction bloquante, donc on ne peut pas prendre 'MAsterWindow' (risque de bloquer la fen�tre non initiatrice de l'ouverture de la bo�te de dialogue).

		T( GetWindowInternal( ParentWindow )->OpenDialog( TransformedURL, TransformedName, NS_LITERAL_STRING( "chrome,extrachrome,menubar,resizable,scrollbars,status,toolbar,modal" ), NULL, WindowBuffer ) );
	ERRErr
	ERREnd
		if ( MasterWindow != NULL )
			ReleaseMasterWindow( MasterWindow );
	ERREpilog
	}

	inline void OpenDialog(
		const char *URL,
		const char *Name,
		nsIDOMWindow **Window = NULL )
	{
		OpenDialog( NULL, URL, Name, Window );
	}
	

	inline nsIDOMWindowInternal *GetJSConsole(
		nsIDOMWindow *ParentWindow,
		nsIDOMWindow **JSConsoleWindow )
	{
		if ( ( *JSConsoleWindow == NULL ) || ( IsClosed( *JSConsoleWindow ) ) ) {
//			GetWindowInternal( ParentWindow )->Open( NS_LITERAL_STRING( "chrome://global/content/console.xul" ), NS_LITERAL_STRING( "_blank" ), NS_LITERAL_STRING( "chrome,extrachrome,menubar,resizable,scrollbars,status,toolbar" ), &Window );
			OpenWindow( ParentWindow, "chrome://global/content/console.xul", "_blank", JSConsoleWindow );
//			*JSConsoleWindow = GetWindowInternal( Window );
		} else
			T( GetWindowInternal( *JSConsoleWindow )->Focus() );

		return GetWindowInternal( *JSConsoleWindow );
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

	template <typename type, typename row> E_TTCLONE_( lstbch::E_LBUNCHt_( type *, row ), _lpbunch_ );	// 'List Pointer Bunch'.

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
					ERRu();	// Car des objets existent qui n'ont pas encore �t� supprim� ('delete'r).
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

	typedef ctn::E_CONTAINER_( xslt_parameter_ ) _xslt_parameters_;

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
		const str::string_ &XMLString,	// Doit �tre du XML valide.
		const str::string_ &XSLString,	// Doit �tre du XSL valide.
		nsIDOMDocument *Owner,
		const xslt_parameters_ &Parameters );

	nsIDOMDocumentFragment *XSLTransformByFileName(
		const str::string_ &XMLString,	// Doit �tre du XML valide.
		const str::string_ &XSLFileName,	// Doit �tre du XSL valide.
		nsIDOMDocument *Owner,
		const xslt_parameters_ &Parameters );

	bso::bool__ GetDirectory(
		const char *Name,
		nsACString &Directory,
		err::handling__ ErrHandling = err::h_Default );

	bso::bool__ GetDirectory(
		const char *Name,
		str::string_ &Directory,
		err::handling__ ErrHandling = err::h_Default );

	template <typename type> inline const type &GetWorkingDirectory( type &Directory )
	{
		if ( !GetDirectory( NS_OS_CURRENT_WORKING_DIR, Directory, err::hUserDefined ) )
			ERRx();

		return Directory;
	}

	template <typename type> inline const type &GetInstallationDirectory( type &Directory )
	{
		if ( !GetDirectory( NS_OS_CURRENT_PROCESS_DIR, Directory, err::hUserDefined ) )
			ERRx();

		return Directory;
	}

	void LaunchURI( const str::string_ &URI );

	inline void LaunchURI( const char *URI )
	{
		LaunchURI( str::string( URI ) );
	}

	void LaunchEmbedFile( const char *File );	// 'File' doit contenir un fichier avec un chemin relatif par rapport � o� est situ� 'xulrunner.exe'.

	// Pour les 'textbox's de type 'autocomplete'.
	// NOTA : La ligne 'pref("browser.formfill.enable", true);' doit �tre ajout�e dans le fihcier 'prefs.js'.
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

	typedef ctn::E_MCONTAINER_( argument_ ) arguments_;
	E_AUTO( arguments )

	bso::ulong__ GetArguments(
		nsICommandLine *CommandLine,
		arguments_ &Arguments );

	void DuplicateBroadcasterCommand( nsIDOMDocument *Document );

	void PatchCommandBadCommandBehaviorforKeysetListener( nsIDOMDocument *Document );

	inline void PatchOverallBadCommandBehavior( nsIDOMDocument *Document )
	{
//		DuplicateBroadcasterCommand( Document );
		PatchCommandBadCommandBehaviorforKeysetListener( Document );
	}

	nsIDOMWindowInternal *GetWindow( nsIDOMDocument *Document );


#ifdef NSXPCM__FBL
	void Convert(
		const strings_ &Items,
		fblfrd::ids32_ &Ids );

	void Convert(
		const strings_ &Items,
		fblfrd::ids16_ &Ids );

	void Convert(
		const strings_ &Items,
		fblfrd::ids8_ &Ids );

	void SplitAndConvert(
		const char *Joined,
		bso::char__ Separator,
		fblfrd::ids32_ &Ids );

	void SplitAndConvert(
		const char *Joined,
		bso::char__ Separator,
		fblfrd::ids16_ &Ids );

	void SplitAndConvert(
		const char *Joined,
		bso::char__ Separator,
		fblfrd::ids8_ &Ids );

	void ConvertAndJoin(
		const fblfrd::ids8_ &Ids,
		bso::char__ Separator,
		string_ &Joined );

	void ConvertJoinAndTransform(
		const fblfrd::ids8_ &Ids,
		bso::char__ Separator,
		char **JString );

	void ConvertAndJoin(
		const fblfrd::ids16_ &Ids,
		bso::char__ Separator,
		string_ &Joined );

	void ConvertJoinAndTransform(
		const fblfrd::ids16_ &Ids,
		bso::char__ Separator,
		char **JString );

	void ConvertAndJoin(
		const fblfrd::ids32_ &Ids,
		bso::char__ Separator,
		string_ &Joined );

	void ConvertJoinAndTransform(
		const fblfrd::ids32_ &Ids,
		bso::char__ Separator,
		char **JString );

	void ConvertAndJoin(
		const fblfrd::slongs_ &Ids,
		bso::char__ Separator,
		string_ &Joined );

	void ConvertJoinAndTransform(
		const fblfrd::slongs_ &Ids,
		bso::char__ Separator,
		char **JString );

	void ConvertAndJoin(
		const fblfrd::ulongs_ &Ids,
		bso::char__ Separator,
		string_ &Joined );

	void ConvertJoinAndTransform(
		const fblfrd::ulongs_ &Ids,
		bso::char__ Separator,
		char **JString );

	void ConvertAndJoin(
		const fblfrd::booleans_ &Booleans,
		bso::char__ Separator,
		string_ &Joined );

	void ConvertJoinAndTransform(
		const fblfrd::booleans_ &Booleans,
		bso::char__ Separator,
		char **JString );

	void Split(
		const char *Joined,
		const char *Amounts,
		bso::char__ Separator,
		fblfrd::xstrings_ &Strings );
#endif
}

/* Debut partie concernant 'event_listener__' */

# define NSXPCM_EVENT_LISTENER_IID_STR "d333cd20-c453-11dd-ad8b-0800200c9a66"

# define NSXPCM_EVENT_LISTENER_IID \
  {0xd333cd20, 0xc453, 0x11dd, \
    { 0xad, 0x8b, 0x08, 0x00, 0x20, 0x0c, 0x9a, 0x66 }}

namespace nsxpcm {

	class event_listener__
	: public nsIDOMEventListener
	{
	private:
		event_handler__ *_EventHandler;
	public:
		NS_DECLARE_STATIC_IID_ACCESSOR(NSXPCOM_EVENT_LISTENER_IID)
		NS_DECL_ISUPPORTS
		NS_DECL_NSIDOMEVENTLISTENER
		  void reset( bso::bool__ = true )
		  {
			  _EventHandler = NULL;
		  }
		E_CVDTOR( event_listener__ );
	public:
		void Init( event_handler__ &EventHandler )
		{
#ifdef NSXPCM_DBG
			if ( _EventHandler != NULL )
				ERRu();
#endif
			reset();

			_EventHandler = &EventHandler;
		}
		bso::bool__ IsInitialized( void ) const
		{
			return _EventHandler != NULL;
		}
	};

	NS_GENERIC_FACTORY_CONSTRUCTOR(event_listener__)
}

NS_DEFINE_STATIC_IID_ACCESSOR(nsxpcm::event_listener__, NSXPCM_EVENT_LISTENER_IID)

# define NSXPCM_EVENT_LISTENER_CONTRACTID "@zeusw.org/event_listener;4"
# define NSXPCM_EVENT_LISTENER_CLASSNAME "NSXPCMEventListener"
# define NSXPCM_EVENT_LISTENER_CID  NSXPCM_EVENT_LISTENER_IID

/* Fin partie concernant 'event_listener__' */


/* Debut partie concernant 'tree_view__' */

# define NSXPCM_TREE_VIEW_IID_STR "c9f25991-b781-49c5-bdb9-29c8fbb9bfb1"

# define NSXPCM_TREE_VIEW_IID \
  {0xc9f25991, 0xb781, 0x49c5, \
    { 0xbd, 0xb9, 0x29, 0xc8, 0xfb, 0xb9, 0xbf, 0xb1 }}

namespace nsxpcm {

	class tree_view_callback__
	{
	protected:
		virtual bso::ulong__ NSXPCMGetRowCount( void ) = 0;
		virtual void NSXPCMGetCellText(
			bso::ulong__ Row,
			bso::ulong__ Column,
			str::string_ &Text ) = 0;
	public:
		bso::ulong__ GetRowCount( void )
		{
			return NSXPCMGetRowCount();
		}
		void GetCellText(
			bso::ulong__ Row,
			bso::ulong__ Column,
			str::string_ &Text )
		{
			NSXPCMGetCellText( Row, Column, Text );
		}
		void reset( bso::bool__ = true )
		{
			// Standardisation.
		}
		E_CVDTOR( tree_view_callback__ );
		void Init( void )
		{
			// Standardisation.
		}
	};


	class tree_view__
	: public nsITreeView
	{
	private:
		tree_view_callback__ *_Callback;
		nsCOMPtr<nsITreeSelection> mSelection;
		nsCOMPtr<nsITreeBoxObject> mTree;
		PRInt32 mTotalRows;
		tree_view_callback__ &_C( void )
		{
			if ( _Callback == NULL )
				ERRc();

			return *_Callback;
		}
	public: 
		NS_DECLARE_STATIC_IID_ACCESSOR(NSXPCOM_ITREE_VIEW_IID)
		NS_DECL_ISUPPORTS
		NS_DECL_NSITREEVIEW
		void reset( bso::bool__ = true )
		{
			_Callback = NULL;
		}
		E_CVDTOR( tree_view__ );
		void Init( tree_view_callback__ &Callback )
		{
			_Callback = &Callback;
		}
	};


	NS_GENERIC_FACTORY_CONSTRUCTOR(tree_view__)

}

NS_DEFINE_STATIC_IID_ACCESSOR(nsxpcm::tree_view__, NSXPCM_TREE_VIEW_IID)

# define NSXPCM_TREE_VIEW_CONTRACTID "@zeusw.org/nsxpcm_tree_view;1"
# define NSXPCM_TREE_VIEW_CLASSNAME "NSXPCMTreeView"
# define NSXPCM_TREE_VIEW_CID  NSXPCM_TREE_VIEW_IID

/* Fin partie concernant 'tree_view____' */

/* D�but partie concernant 'textbox' 'autocomplete' */

namespace nsxpcm {
	class autocomplete_textbox_callback__
	{
	protected:
		virtual void NSXPCMGetValue(
			const str::string_ &Pattern,
			bso::ulong__ Index,
			str::string_ &Value ) = 0;
		virtual void NSXPCMGetLabel(
			const str::string_ &Pattern,
			bso::ulong__ Index,
			str::string_ &Label )
		{
			GetValue( Pattern, Index, Label );
		}
		virtual void NSXPCMGetComment(
			const str::string_ &Pattern,
			bso::ulong__ Index,
			str::string_ &Comment )
		{
			ERRu();
		}
		virtual bso::ulong__ NSXPCMGetMatchingCount( const str::string_ &Pattern ) = 0;
	public:
		void reset( bso::bool__ = true )
		{
			// Standardisation.
		}
		E_CVDTOR( autocomplete_textbox_callback__ );
		void Init( void )
		{
			// Standardisation.
		}
		void GetLabel(
			const str::string_ &Pattern,
			bso::ulong__ Index,
			str::string_ &Label )
		{
			NSXPCMGetLabel( Pattern, Index, Label );
		}
		void GetValue(
			const str::string_ &Pattern,
			bso::ulong__ Index,
			str::string_ &Value )
		{
			NSXPCMGetValue( Pattern, Index, Value );
		}
		void GetComment(
			const str::string_ &Pattern,
			bso::ulong__ Index,
			str::string_ &Comment )
		{
			NSXPCMGetComment( Pattern, Index, Comment );
		}
		bso::ulong__ GetMatchingCount( const str::string_ &Pattern )
		{
			return NSXPCMGetMatchingCount( Pattern );
		}
	};
}

# define NSXPCM_AUTOCOMPLETE_RESULT_IID_STR "5126d993-3577-4214-811a-a1f286d464f7"

# define NSXPCM_AUTOCOMPLETE_RESULT_IID \
  {0x5126d993, 0x3577, 0x4214, \
    { 0x81, 0x1a, 0xa1, 0xf2, 0x86, 0xd4, 0x64, 0xf7 }}

namespace nsxpcm {
	class autocomplete_result___
	: public nsIAutoCompleteResult
	{
	private:
		autocomplete_textbox_callback__ *_Callback;
		str::string _SearchString;
		autocomplete_textbox_callback__ &_C( void )
		{
			if ( _Callback == NULL )
				ERRc();

			return *_Callback;
		}
	public:
		NS_DECLARE_STATIC_IID_ACCESSOR( NSXPCM_AUTOCOMPLETE_RESULT_IID )
		NS_DECL_ISUPPORTS
		NS_DECL_NSIAUTOCOMPLETERESULT
		void reset( bso::bool__ P = true )
		{
			_Callback = NULL;
			_SearchString.reset( P );
		}
		E_CVDTOR( autocomplete_result___ );
	public:
		void Init( autocomplete_textbox_callback__ &Callback )	
		{
			reset();

			_Callback = &Callback;
			_SearchString.Init();
		}
		void SetSearchString( const nsAString &RawSearchWString )
		{
			_SearchString.Init();
			Transform( RawSearchWString, _SearchString );
		}
		const str::string_ &GetSearchString( void ) const
		{
			return _SearchString;
		}
	};

	NS_GENERIC_FACTORY_CONSTRUCTOR( autocomplete_result___ )
}

NS_DEFINE_STATIC_IID_ACCESSOR( nsxpcm::autocomplete_result___, NSXPCM_AUTOCOMPLETE_RESULT_IID )

# define NSXPCM_AUTOCOMPLETE_RESULT_CONTRACTID "@zeusw.org/autocomplete_result;1"
# define NXSPCM_AUTOCOMPLETE_RESULT_CLASSNAME "EAutoCompleteResult"
# define NSXPCM_AUTOCOMPLETE_RESULT_CID  NSXPCM_AUTOCOMPLETE_RESULT_IID

namespace nsxpcm {
	class autocomplete_textbox__
	: public textbox__
	{
	private:
		nsCOMPtr<nsxpcm::autocomplete_result___> _AutoCompleteResult;
		void _SubInit( nsxpcm::autocomplete_textbox_callback__ &Callback )
		{
		ERRProlog
			str::string SearchParam;
			bso::pointer_buffer__ Buffer;
		ERRBegin
			if ( HasAttribute( "type" )  )
				ERRu();

			SetAttribute( "type", "autocomplete" );

			nsxpcm::CreateInstance( NSXPCM_AUTOCOMPLETE_RESULT_CONTRACTID, _AutoCompleteResult );
			_AutoCompleteResult->Init( Callback );
			SearchParam.Init( bso::Convert( _AutoCompleteResult, Buffer ) );
			SetAttribute( "autocompletesearchparam", SearchParam );
		ERRErr
		ERREnd
		ERREpilog
		}
	public:
		void reset( bso::bool__ P = true )
		{
			textbox__::reset( P );
		}
		E_CDTOR( autocomplete_textbox__ );
		void Init(
			nsxpcm::autocomplete_textbox_callback__ &Callback,
			const supports__ &Supports )
		{
			reset();
			textbox__::Init( Supports );
			_SubInit( Callback );
		}
	};
}

/* Fin partie concernant 'textbox' 'autocomplete' */

/* Debut partie concernant 'iclh__' */

# define CLH_IID_STR "d333cd20-c453-11dd-ad8b-0800200c9a67"

# define ICLH_IID \
  {0xd333cd20, 0xc453, 0x11dd, \
    { 0xad, 0x8b, 0x08, 0x00, 0x20, 0x0c, 0x9a, 0x67 }}

namespace nsxpcm {
	class NS_NO_VTABLE NS_SCRIPTABLE iclh__
	: public nsICommandLineHandler
	{
	public: 
		NS_DECLARE_STATIC_IID_ACCESSOR(NSXPCOM_ICLH_IID)
	};

	struct clh__
	: iclh__
	{
	protected:
		NS_IMETHOD Handle(nsICommandLine *aCommandLine)
		{
			return NS_OK;
		}
		NS_IMETHOD GetHelpInfo(nsACString & aHelpInfo)
		{
			return NS_OK;
		}
	public:
		NS_DECL_ISUPPORTS
		  void reset( bso::bool__ = true )
		  {
		  }
		E_CVDTOR( clh__ );
	public:
		void Init( void )
		{
		}
	};


	NS_GENERIC_FACTORY_CONSTRUCTOR(clh__)
}
	NS_DEFINE_STATIC_IID_ACCESSOR(nsxpcm::iclh__, ICLH_IID)

#define CLH_CONTRACTID "@mozilla.org/commandlinehandler/general-startup;1?type=egeckocom"
#define CLH_CLASSNAME "CLH"
// {d333cd20-c453-11dd-ad8b-0800200c9a66}
#define CLH_CID  ICLH_IID

/* Fin partie concernant 'iclh__' */


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
	   nsxpcm::event_listener__Constructor,\
    }\
	{\
       NSXPCM_TREE_VIEW_CLASSNAME,\
       NSXPCM_TREE_VIEW_CID,\
       NSXPCM_TREE_VIEW_CONTRACTID,\
	   nsxpcm::tree_view__Constructor,\
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
