/*
 * NativeString.h
 *
 *  Created on: 2016年2月19日
 *      Author: peng
 */

#ifndef NATIVESTRING_H_
#define NATIVESTRING_H_

string16 jstringToString16(JNIEnv* env, jstring jstr)
{
  if (!jstr || !env)
    return string16();

  const char* s = env->GetStringUTFChars(jstr, 0);
  if (!s)
    return string16();
  string16 str = UTF8ToUTF16(s);
  env->ReleaseStringUTFChars(jstr, s);
  checkException(env);
  return str;
}

std::string jstringToStdString(JNIEnv* env, jstring jstr)
{
  if (!jstr || !env)
    return std::string();

  const char* s = env->GetStringUTFChars(jstr, 0);
  if (!s)
    return std::string();
  std::string str(s);
  env->ReleaseStringUTFChars(jstr, s);
  checkException(env);
  return str;
}




#endif /* NATIVESTRING_H_ */
