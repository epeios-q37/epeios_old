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
#include "nsCOMPtr.h"

#ifdef NSXPCM_BKD
#	define NSXPCM__BKD
#endif

#ifdef NSXPCM__BKD
#	include "bkdacc.h"
#endif

namespace nsxpcm {

	using str::string_;
	using str::string;

	typedef ctn::E_XMCONTAINER_( string_ ) strings_;
	E_AUTO( strings );

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

	template <typename element> inline element *_GetElementById(
		nsIDOMDocument *Document,
		const char *Id )
	{
		element *Element = NULL;

		if ( GetElementById( Document, Id )->QueryInterface( element::GetIID(), (void **)&Element ) != NS_OK )
			ERRu();

		return Element;
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
		nsCOMPtr<element> Element;

#if 0	// Ne fonctionne pas !
		Element = do_QueryInterface( CreateElement( Document, Name ) );

		// Ne pas oulier de tester 'Result' lors de la mise en place de ma version fonctionnelle.
#else	// Fonctionne, mais présente des risques ...
		Element = (element *)CreateElement( Document, Name );
#endif
		return Element;
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
		inline name *Create##Name##( nsIDOMDocument *Document )\
		{\
			return _CreateElement< name >( Document, #name );\
		}


	NSXPCM_DEFINE( nsIDOMXULMultiSelectControlElement, listbox, Listbox )
	NSXPCM_DEFINE( nsIDOMXULSelectControlItemElement, listitem, Listitem )
	NSXPCM_DEFINE( nsIDOMXULTextBoxElement, textbox, Textbox )
	NSXPCM_DEFINE( nsIDOMXULMenuListElement, menulist, Menulist )
	NSXPCM_DEFINE( nsIDOMXULCheckboxElement, checkbox, Checkbox )

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
		Node->RemoveChild( Child, &Dummy );
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
