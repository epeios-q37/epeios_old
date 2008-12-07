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

/* starting interface:    ieshared */
#define IESHARED_IID_STR "82aff662-258f-4644-a1bb-c0e22f55595d"

#define IESHARED_IID \
  {0x82aff662, 0x258f, 0x4644, \
    { 0xa1, 0xbb, 0xc0, 0xe2, 0x2f, 0x55, 0x59, 0x5d }}

class NS_NO_VTABLE NS_SCRIPTABLE ieshared : public nsISupports {
 public: 

  NS_DECLARE_STATIC_IID_ACCESSOR(IESHARED_IID)

  /* void Test (); */
  NS_SCRIPTABLE NS_IMETHOD Test(void) = 0;

  /* void Set (in string Value); */
  NS_SCRIPTABLE NS_IMETHOD Set(const char *Value) = 0;

  /* string Get (); */
  NS_SCRIPTABLE NS_IMETHOD Get(char **_retval) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(ieshared, IESHARED_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_IESHARED \
  NS_SCRIPTABLE NS_IMETHOD Test(void); \
  NS_SCRIPTABLE NS_IMETHOD Set(const char *Value); \
  NS_SCRIPTABLE NS_IMETHOD Get(char **_retval); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_IESHARED(_to) \
  NS_SCRIPTABLE NS_IMETHOD Test(void) { return _to Test(); } \
  NS_SCRIPTABLE NS_IMETHOD Set(const char *Value) { return _to Set(Value); } \
  NS_SCRIPTABLE NS_IMETHOD Get(char **_retval) { return _to Get(_retval); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_IESHARED(_to) \
  NS_SCRIPTABLE NS_IMETHOD Test(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->Test(); } \
  NS_SCRIPTABLE NS_IMETHOD Set(const char *Value) { return !_to ? NS_ERROR_NULL_POINTER : _to->Set(Value); } \
  NS_SCRIPTABLE NS_IMETHOD Get(char **_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->Get(_retval); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class _MYCLASS_ : public ieshared
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_IESHARED

  _MYCLASS_();

private:
  ~_MYCLASS_();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(_MYCLASS_, ieshared)

_MYCLASS_::_MYCLASS_()
{
  /* member initializers and constructor code */
}

_MYCLASS_::~_MYCLASS_()
{
  /* destructor code */
}

/* void Test (); */
NS_IMETHODIMP _MYCLASS_::Test()
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void Set (in string Value); */
NS_IMETHODIMP _MYCLASS_::Set(const char *Value)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* string Get (); */
NS_IMETHODIMP _MYCLASS_::Get(char **_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


/* starting interface:    ieprivate */
#define IEPRIVATE_IID_STR "eb11441c-aedc-48a2-ba79-936372a30382"

#define IEPRIVATE_IID \
  {0xeb11441c, 0xaedc, 0x48a2, \
    { 0xba, 0x79, 0x93, 0x63, 0x72, 0xa3, 0x03, 0x82 }}

class NS_NO_VTABLE NS_SCRIPTABLE ieprivate : public nsISupports {
 public: 

  NS_DECLARE_STATIC_IID_ACCESSOR(IEPRIVATE_IID)

  /* void SetUI (in nsIDOMDocument Document); */
  NS_SCRIPTABLE NS_IMETHOD SetUI(nsIDOMDocument *Document) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(ieprivate, IEPRIVATE_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_IEPRIVATE \
  NS_SCRIPTABLE NS_IMETHOD SetUI(nsIDOMDocument *Document); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_IEPRIVATE(_to) \
  NS_SCRIPTABLE NS_IMETHOD SetUI(nsIDOMDocument *Document) { return _to SetUI(Document); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_IEPRIVATE(_to) \
  NS_SCRIPTABLE NS_IMETHOD SetUI(nsIDOMDocument *Document) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetUI(Document); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class _MYCLASS_ : public ieprivate
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_IEPRIVATE

  _MYCLASS_();

private:
  ~_MYCLASS_();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(_MYCLASS_, ieprivate)

_MYCLASS_::_MYCLASS_()
{
  /* member initializers and constructor code */
}

_MYCLASS_::~_MYCLASS_()
{
  /* destructor code */
}

/* void SetUI (in nsIDOMDocument Document); */
NS_IMETHODIMP _MYCLASS_::SetUI(nsIDOMDocument *Document)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


/* starting interface:    ieevent_listener */
#define IEEVENT_LISTENER_IID_STR "d333cd20-c453-11dd-ad8b-0800200c9a66"

#define IEEVENT_LISTENER_IID \
  {0xd333cd20, 0xc453, 0x11dd, \
    { 0xad, 0x8b, 0x08, 0x00, 0x20, 0x0c, 0x9a, 0x66 }}

class NS_NO_VTABLE NS_SCRIPTABLE ieevent_listener : public nsIDOMEventListener {
 public: 

  NS_DECLARE_STATIC_IID_ACCESSOR(IEEVENT_LISTENER_IID)

};

  NS_DEFINE_STATIC_IID_ACCESSOR(ieevent_listener, IEEVENT_LISTENER_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_IEEVENT_LISTENER \
  /* no methods! */

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_IEEVENT_LISTENER(_to) \
  /* no methods! */

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_IEEVENT_LISTENER(_to) \
  /* no methods! */

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class _MYCLASS_ : public ieevent_listener
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_IEEVENT_LISTENER

  _MYCLASS_();

private:
  ~_MYCLASS_();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(_MYCLASS_, ieevent_listener)

_MYCLASS_::_MYCLASS_()
{
  /* member initializers and constructor code */
}

_MYCLASS_::~_MYCLASS_()
{
  /* destructor code */
}

/* End of implementation class template. */
#endif


#endif /* __gen_iegecko_h__ */
