/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM egeckocom.idl
 */

#ifndef __gen_egeckocom_h__
#define __gen_egeckocom_h__


#ifndef __gen_nsIDOMWindow_h__
#include "nsIDOMWindow.h"
#endif

#ifndef __gen_nsICommandLine_h__
#include "nsICommandLine.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif

/* starting interface:    EIGeckoCOM */
#define EIGECKOCOM_IID_STR "07232fe0-fee4-11e0-be50-0800200c9a66"

#define EIGECKOCOM_IID \
  {0x07232fe0, 0xfee4, 0x11e0, \
    { 0xbe, 0x50, 0x08, 0x00, 0x20, 0x0c, 0x9a, 0x66 }}

class NS_NO_VTABLE NS_SCRIPTABLE EIGeckoCOM : public nsISupports {
 public: 

  NS_DECLARE_STATIC_IID_ACCESSOR(EIGECKOCOM_IID)

  /* void Initialize (in string ComponentId, in string Language, out string ErrorMessage); */
  NS_SCRIPTABLE NS_IMETHOD Initialize(const char *ComponentId, const char *Language, char **ErrorMessage NS_OUTPARAM) = 0;

  /* void Register (in nsIDOMWindow Window, out string ErrorMessage); */
  NS_SCRIPTABLE NS_IMETHOD Register(nsIDOMWindow *Window, char **ErrorMessage NS_OUTPARAM) = 0;

  /* void Start (in nsICommandLine CommandLine, out string ErrorMessage); */
  NS_SCRIPTABLE NS_IMETHOD Start(nsICommandLine *CommandLine, char **ErrorMessage NS_OUTPARAM) = 0;

  /* void LoneRegister (in nsIDOMWindow Window, in string LibraryName, out string ErrorMessage); */
  NS_SCRIPTABLE NS_IMETHOD LoneRegister(nsIDOMWindow *Window, const char *LibraryName, char **ErrorMessage NS_OUTPARAM) = 0;

  /* void Terminate (out string ErrorMessage); */
  NS_SCRIPTABLE NS_IMETHOD Terminate(char **ErrorMessage NS_OUTPARAM) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(EIGeckoCOM, EIGECKOCOM_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_EIGECKOCOM \
  NS_SCRIPTABLE NS_IMETHOD Initialize(const char *ComponentId, const char *Language, char **ErrorMessage NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD Register(nsIDOMWindow *Window, char **ErrorMessage NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD Start(nsICommandLine *CommandLine, char **ErrorMessage NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD LoneRegister(nsIDOMWindow *Window, const char *LibraryName, char **ErrorMessage NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD Terminate(char **ErrorMessage NS_OUTPARAM); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_EIGECKOCOM(_to) \
  NS_SCRIPTABLE NS_IMETHOD Initialize(const char *ComponentId, const char *Language, char **ErrorMessage NS_OUTPARAM) { return _to Initialize(ComponentId, Language, ErrorMessage); } \
  NS_SCRIPTABLE NS_IMETHOD Register(nsIDOMWindow *Window, char **ErrorMessage NS_OUTPARAM) { return _to Register(Window, ErrorMessage); } \
  NS_SCRIPTABLE NS_IMETHOD Start(nsICommandLine *CommandLine, char **ErrorMessage NS_OUTPARAM) { return _to Start(CommandLine, ErrorMessage); } \
  NS_SCRIPTABLE NS_IMETHOD LoneRegister(nsIDOMWindow *Window, const char *LibraryName, char **ErrorMessage NS_OUTPARAM) { return _to LoneRegister(Window, LibraryName, ErrorMessage); } \
  NS_SCRIPTABLE NS_IMETHOD Terminate(char **ErrorMessage NS_OUTPARAM) { return _to Terminate(ErrorMessage); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_EIGECKOCOM(_to) \
  NS_SCRIPTABLE NS_IMETHOD Initialize(const char *ComponentId, const char *Language, char **ErrorMessage NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->Initialize(ComponentId, Language, ErrorMessage); } \
  NS_SCRIPTABLE NS_IMETHOD Register(nsIDOMWindow *Window, char **ErrorMessage NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->Register(Window, ErrorMessage); } \
  NS_SCRIPTABLE NS_IMETHOD Start(nsICommandLine *CommandLine, char **ErrorMessage NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->Start(CommandLine, ErrorMessage); } \
  NS_SCRIPTABLE NS_IMETHOD LoneRegister(nsIDOMWindow *Window, const char *LibraryName, char **ErrorMessage NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->LoneRegister(Window, LibraryName, ErrorMessage); } \
  NS_SCRIPTABLE NS_IMETHOD Terminate(char **ErrorMessage NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->Terminate(ErrorMessage); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class _MYCLASS_ : public EIGeckoCOM
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_EIGECKOCOM

  _MYCLASS_();

private:
  ~_MYCLASS_();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(_MYCLASS_, EIGeckoCOM)

_MYCLASS_::_MYCLASS_()
{
  /* member initializers and constructor code */
}

_MYCLASS_::~_MYCLASS_()
{
  /* destructor code */
}

/* void Initialize (in string ComponentId, in string Language, out string ErrorMessage); */
NS_IMETHODIMP _MYCLASS_::Initialize(const char *ComponentId, const char *Language, char **ErrorMessage NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void Register (in nsIDOMWindow Window, out string ErrorMessage); */
NS_IMETHODIMP _MYCLASS_::Register(nsIDOMWindow *Window, char **ErrorMessage NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void Start (in nsICommandLine CommandLine, out string ErrorMessage); */
NS_IMETHODIMP _MYCLASS_::Start(nsICommandLine *CommandLine, char **ErrorMessage NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void LoneRegister (in nsIDOMWindow Window, in string LibraryName, out string ErrorMessage); */
NS_IMETHODIMP _MYCLASS_::LoneRegister(nsIDOMWindow *Window, const char *LibraryName, char **ErrorMessage NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void Terminate (out string ErrorMessage); */
NS_IMETHODIMP _MYCLASS_::Terminate(char **ErrorMessage NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_egeckocom_h__ */
