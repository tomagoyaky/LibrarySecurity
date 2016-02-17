package com.tomagoyaky.librarysecurity.utils;
import java.lang.reflect.Constructor;
import java.lang.reflect.Member;
import java.lang.reflect.Method;
import java.lang.reflect.Modifier;

public class HookHelper {

	protected static final String TAG = "tomagoyaky";
	private static boolean isArt(){
		String libName = System.getProperty("persist.sys.dalvik.vm.lib");
		return libName.toLowerCase().equals("libart.so") ? true : false;
	}
	
	public static boolean replaceJavaMethod(Member method, String methodsig){
		
		int result = -1;
		if(method instanceof Method || method instanceof Constructor<?>){
			String clsdes = method.getDeclaringClass().getName().replace('.', '/');
			
			String methodname = method.getName();
			
			if(method instanceof Constructor<?>){
				result  = hookMethodNative(clsdes, methodname, methodsig, false);
			}else{//for method
				result = hookMethodNative(clsdes, methodname, methodsig, Modifier.isStatic(method.getModifiers()));
			}
		}
		return result == -1? false : true;
	}
	
	private static native int hookMethodNative(String clsdes, String methodname, String methodsig, boolean isstatic);
}
