/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM iegecko.idl
 */

#ifndef __gen_iegecko_h__
#define __gen_iegecko_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

#ifndef __gen_nsIDOMDocument_h__
#include "nsIDOMDocument.h"
#endif

#ifndef __gen_nsIDOMEventListener_h__
#include "nsIDOMEventListener.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif

/* starting interface:    iegeckocom */
#define IEGECKOCOM_IID_STR "f1e695a0-b2f1-4a30-ac62-48a9edd9558a"

#define IEGECKOCOM_IID \
  {0xf1e695a0, 0xb2f1, 0x4a30, \
    { 0xac, 0x62, 0x48, 0xa9, 0xed, 0xd9, 0x55, 0x8a }}

class NS_NO_VTABLE NS_SCRIPTABLE iegeckocom : public nsISupports {
 public: 

  NS_DECLARE_STATIC_IID_ACCESSOR(IEGECKOCOM_IID)

  /* void RegisteringStart (); */
  NS_SCRIPTABLE NS_IMETHOD RegisteringStart(void) = 0;

  /* void Register (in nsIDOMWindow Window, in string UIDesignation); */
  NS_SCRIPTABLE NS_IMETHOD Register(nsIDOMWindow *Window, const char *UIDesignation) = 0;

  /* void RegisteringEnd (); */
  NS_SCRIPTABLE NS_IMETHOD RegisteringEnd(void) = 0;

  /* void Unregister (); */
  NS_SCRIPTABLE NS_IMETHOD Unregister(void) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(iegeckocom, IEGECKOCOM_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_IEGECKOCOM \
  NS_SCRIPTABLE NS_IMETHOD RegisteringStart(void); \
  NS_SCRIPTABLE NS_IMETHOD Register(nsIDOMWindow *Window, const char *UIDesignation); \
  NS_SCRIPTABLE NS_IMETHOD RegisteringEnd(void); \
  NS_SCRIPTABLE NS_IMETHOD Unregister(void); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_IEGECKOCOM(_to) \
  NS_SCRIPTABLE NS_IMETHOD RegisteringStart(void) { return _to RegisteringStart(); } \
  NS_SCRIPTABLE NS_IMETHOD Register(nsIDOMWindow *Window, const char *UIDesignation) { return _to Register(Window, UIDesignation); } \
  NS_SCRIPTABLE NS_IMETHOD RegisteringEnd(void) { return _to RegisteringEnd(); } \
  NS_SCRIPTABLE NS_IMETHOD Unregister(void) { return _to Unregister(); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_IEGECKOCOM(_to) \
  NS_SCRIPTABLE NS_IMETHOD RegisteringStart(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->RegisteringStart(); } \
  NS_SCRIPTABLE NS_IMETHOD Register(nsIDOMWindow *Window, const char *UIDesignation) { return !_to ? NS_ERROR_NULL_POINTER : _to->Register(Window, UIDesignation); } \
  NS_SCRIPTABLE NS_IMETHOD RegisteringEnd(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->RegisteringEnd(); } \
  NS_SCRIPTABLE NS_IMETHOD Unregister(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->Unregister(); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class _MYCLASS_ : public iegeckocom
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_IEGECKOCOM

  _MYCLASS_();

private:
  ~_MYCLASS_();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(_MYCLASS_, iegeckocom)

_MYCLASS_::_MYCLASS_()
{
  /* member initializers and constructor code */
}

_MYCLASS_::~_MYCLASS_()
{
  /* destructor code */
}

/* void RegisteringStart (); */
NS_IMETHODIMP _MYCLASS_::RegisteringStart()
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void Register (in nsIDOMWindow Window, in string UIDesignation); */
NS_IMETHODIMP _MYCLASS_::Register(nsIDOMWindow *Window, const char *UIDesignation)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void RegisteringEnd (); */
NS_IMETHODIMP _MYCLASS_::RegisteringEnd()
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void Unregister (); */
NS_IMETHODIMP _MYCLASS_::Unregister()
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_iegecko_h__ */
