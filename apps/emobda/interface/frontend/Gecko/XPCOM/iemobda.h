/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM iemobda.idl
 */

#ifndef __gen_iemobda_h__
#define __gen_iemobda_h__


#ifndef __gen_nsIDOMWindow_h__
#include "nsIDOMWindow.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif

/* starting interface:    iemobdacom */
#define IEMOBDACOM_IID_STR "cfee5019-09e1-4d5c-af89-0ffadf903df5"

#define IEMOBDACOM_IID \
  {0xcfee5019, 0x09e1, 0x4d5c, \
    { 0xaf, 0x89, 0x0f, 0xfa, 0xdf, 0x90, 0x3d, 0xf5 }}

class NS_NO_VTABLE NS_SCRIPTABLE iemobdacom : public nsISupports {
 public: 

  NS_DECLARE_STATIC_IID_ACCESSOR(IEMOBDACOM_IID)

  /* void RegisteringStart (); */
  NS_SCRIPTABLE NS_IMETHOD RegisteringStart(void) = 0;

  /* void Register (in nsIDOMWindow Window); */
  NS_SCRIPTABLE NS_IMETHOD Register(nsIDOMWindow *Window) = 0;

  /* void RegisterAlone (in nsIDOMWindow Window); */
  NS_SCRIPTABLE NS_IMETHOD RegisterAlone(nsIDOMWindow *Window) = 0;

  /* void RegisteringEnd (); */
  NS_SCRIPTABLE NS_IMETHOD RegisteringEnd(void) = 0;

  /* void Retire (); */
  NS_SCRIPTABLE NS_IMETHOD Retire(void) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(iemobdacom, IEMOBDACOM_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_IEMOBDACOM \
  NS_SCRIPTABLE NS_IMETHOD RegisteringStart(void); \
  NS_SCRIPTABLE NS_IMETHOD Register(nsIDOMWindow *Window); \
  NS_SCRIPTABLE NS_IMETHOD RegisterAlone(nsIDOMWindow *Window); \
  NS_SCRIPTABLE NS_IMETHOD RegisteringEnd(void); \
  NS_SCRIPTABLE NS_IMETHOD Retire(void); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_IEMOBDACOM(_to) \
  NS_SCRIPTABLE NS_IMETHOD RegisteringStart(void) { return _to RegisteringStart(); } \
  NS_SCRIPTABLE NS_IMETHOD Register(nsIDOMWindow *Window) { return _to Register(Window); } \
  NS_SCRIPTABLE NS_IMETHOD RegisterAlone(nsIDOMWindow *Window) { return _to RegisterAlone(Window); } \
  NS_SCRIPTABLE NS_IMETHOD RegisteringEnd(void) { return _to RegisteringEnd(); } \
  NS_SCRIPTABLE NS_IMETHOD Retire(void) { return _to Retire(); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_IEMOBDACOM(_to) \
  NS_SCRIPTABLE NS_IMETHOD RegisteringStart(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->RegisteringStart(); } \
  NS_SCRIPTABLE NS_IMETHOD Register(nsIDOMWindow *Window) { return !_to ? NS_ERROR_NULL_POINTER : _to->Register(Window); } \
  NS_SCRIPTABLE NS_IMETHOD RegisterAlone(nsIDOMWindow *Window) { return !_to ? NS_ERROR_NULL_POINTER : _to->RegisterAlone(Window); } \
  NS_SCRIPTABLE NS_IMETHOD RegisteringEnd(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->RegisteringEnd(); } \
  NS_SCRIPTABLE NS_IMETHOD Retire(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->Retire(); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class _MYCLASS_ : public iemobdacom
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_IEMOBDACOM

  _MYCLASS_();

private:
  ~_MYCLASS_();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(_MYCLASS_, iemobdacom)

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

/* void Register (in nsIDOMWindow Window); */
NS_IMETHODIMP _MYCLASS_::Register(nsIDOMWindow *Window)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void RegisterAlone (in nsIDOMWindow Window); */
NS_IMETHODIMP _MYCLASS_::RegisterAlone(nsIDOMWindow *Window)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void RegisteringEnd (); */
NS_IMETHODIMP _MYCLASS_::RegisteringEnd()
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void Retire (); */
NS_IMETHODIMP _MYCLASS_::Retire()
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_iemobda_h__ */
