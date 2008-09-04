/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM ietest.idl
 */

#ifndef __gen_ietest_h__
#define __gen_ietest_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif

/* starting interface:    ietest */
#define IETEST_IID_STR "ca2c7cf2-d7b2-4419-9410-f8aac6832552"

#define IETEST_IID \
  {0xca2c7cf2, 0xd7b2, 0x4419, \
    { 0x94, 0x10, 0xf8, 0xaa, 0xc6, 0x83, 0x25, 0x52 }}

class NS_NO_VTABLE NS_SCRIPTABLE ietest : public nsISupports {
 public: 

  NS_DECLARE_STATIC_IID_ACCESSOR(IETEST_IID)

  /* string Test (); */
  NS_SCRIPTABLE NS_IMETHOD Test(char **_retval) = 0;

  /* string XPCOM (); */
  NS_SCRIPTABLE NS_IMETHOD XPCOM(char **_retval) = 0;

  /* string GetErrorMessage (); */
  NS_SCRIPTABLE NS_IMETHOD GetErrorMessage(char **_retval) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(ietest, IETEST_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_IETEST \
  NS_SCRIPTABLE NS_IMETHOD Test(char **_retval); \
  NS_SCRIPTABLE NS_IMETHOD XPCOM(char **_retval); \
  NS_SCRIPTABLE NS_IMETHOD GetErrorMessage(char **_retval); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_IETEST(_to) \
  NS_SCRIPTABLE NS_IMETHOD Test(char **_retval) { return _to Test(_retval); } \
  NS_SCRIPTABLE NS_IMETHOD XPCOM(char **_retval) { return _to XPCOM(_retval); } \
  NS_SCRIPTABLE NS_IMETHOD GetErrorMessage(char **_retval) { return _to GetErrorMessage(_retval); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_IETEST(_to) \
  NS_SCRIPTABLE NS_IMETHOD Test(char **_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->Test(_retval); } \
  NS_SCRIPTABLE NS_IMETHOD XPCOM(char **_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->XPCOM(_retval); } \
  NS_SCRIPTABLE NS_IMETHOD GetErrorMessage(char **_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetErrorMessage(_retval); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class _MYCLASS_ : public ietest
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_IETEST

  _MYCLASS_();

private:
  ~_MYCLASS_();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(_MYCLASS_, ietest)

_MYCLASS_::_MYCLASS_()
{
  /* member initializers and constructor code */
}

_MYCLASS_::~_MYCLASS_()
{
  /* destructor code */
}

/* string Test (); */
NS_IMETHODIMP _MYCLASS_::Test(char **_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* string XPCOM (); */
NS_IMETHODIMP _MYCLASS_::XPCOM(char **_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* string GetErrorMessage (); */
NS_IMETHODIMP _MYCLASS_::GetErrorMessage(char **_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_ietest_h__ */
