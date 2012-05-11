/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class org_zeusw_XPPInputStream */

#ifndef _Included_org_zeusw_XPPInputStream
#define _Included_org_zeusw_XPPInputStream
#ifdef __cplusplus
extern "C" {
#endif
#undef org_zeusw_XPPInputStream_MAX_SKIP_BUFFER_SIZE
#define org_zeusw_XPPInputStream_MAX_SKIP_BUFFER_SIZE 2048L
/*
 * Class:     org_zeusw_XPPInputStream
 * Method:    contructor
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_zeusw_XPPInputStream_contructor
  (JNIEnv *, jobject);

/*
 * Class:     org_zeusw_XPPInputStream
 * Method:    destructor
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_zeusw_XPPInputStream_destructor
  (JNIEnv *, jobject);

/*
 * Class:     org_zeusw_XPPInputStream
 * Method:    available
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_org_zeusw_XPPInputStream_available
  (JNIEnv *, jobject);

/*
 * Class:     org_zeusw_XPPInputStream
 * Method:    read
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_org_zeusw_XPPInputStream_read__
  (JNIEnv *, jobject);

/*
 * Class:     org_zeusw_XPPInputStream
 * Method:    read
 * Signature: ([B)I
 */
JNIEXPORT jint JNICALL Java_org_zeusw_XPPInputStream_read___3B
  (JNIEnv *, jobject, jbyteArray);

/*
 * Class:     org_zeusw_XPPInputStream
 * Method:    read
 * Signature: ([BII)I
 */
JNIEXPORT jint JNICALL Java_org_zeusw_XPPInputStream_read___3BII
  (JNIEnv *, jobject, jbyteArray, jint, jint);

#ifdef __cplusplus
}
#endif
#endif
