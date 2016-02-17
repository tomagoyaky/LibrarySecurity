package com.tomagoyaky.Protector;

import java.io.BufferedInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.security.cert.Certificate;
import java.security.cert.CertificateEncodingException;
import java.util.Enumeration;
import java.util.List;
import java.util.jar.JarEntry;
import java.util.jar.JarFile;

import android.content.Context;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.content.pm.Signature;
import android.util.Log;
import android.widget.Toast;

public class CheckSignature {

	protected static final String TAG = "tomagoyaky";

	public static void start(Context context, String pkgName) {
		boolean ret1 = false, ret2 = false;
		PackageManager pm = context.getPackageManager();
		List<PackageInfo> packages = pm.getInstalledPackages(0);
		for (int i = 0; i < packages.size(); i++) {
			PackageInfo packageInfo = packages.get(i);
			if (pkgName.toLowerCase().equals(packageInfo.packageName)) {

				ret1 = CheckSignature.judgeWithPackageInfo(context, packageInfo, "C128501A6F80E628");
				ret2 = CheckSignature.judgeWithJarEntry(context, packageInfo, "6798D8C431AF3328");
			}
		}
		if(ret1 && ret2){
			Log.i(TAG, "signature verification is OK");
			Toast.makeText(context, "合法签名", Toast.LENGTH_LONG).show();
		}else{
			Log.e(TAG, "signature verification is ERROR");
			Toast.makeText(context, "非法签名", Toast.LENGTH_LONG).show();
		}
	}
	
	public static boolean judgeWithPackageInfo(Context context, PackageInfo packageInfo, String hash){
		try {
			Signature[] signatures = context.getPackageManager().
					getPackageInfo(packageInfo.packageName, PackageManager.GET_SIGNATURES).signatures;
			return JudgeSignatureIsSameHash(hash, SignatureToHash(signatures));
		} catch (NameNotFoundException e) {
			e.printStackTrace();
		}
		return false;
	}
	
	// *********************************************************************
	// ref:  public boolean collectCertificates(Package pkg, int flags)
	// xref: /frameworks/base/core/java/android/content/pm/PackageParser.java 
	// *********************************************************************
	public static boolean judgeWithJarEntry(Context context, PackageInfo packageInfo, String hash){
		
		Signature[] signatures = null;
		Certificate[] certs = null;
		byte[] readBuffer = new byte[8192];
		final String archiveSourcePath = context.getPackageResourcePath();
		final String ANDROID_MANIFEST_FILENAME = "AndroidManifest.xml"; 
		try {
			JarFile jarFile = new JarFile(archiveSourcePath);
			if ((packageInfo.applicationInfo.flags & ApplicationInfo.FLAG_SYSTEM) > 0) {
				// If this package comes from the system image, then we
	            // can trust it...  we'll just use the AndroidManifest.xml
	            // to retrieve its signatures, not validating all of the
	            // files.
				Log.w(TAG, "System application");
				JarEntry jarEntry = jarFile.getJarEntry(ANDROID_MANIFEST_FILENAME);
				certs = loadCertificates(jarFile, jarEntry, readBuffer);
			    
			}else{
				Log.w(TAG, "User application");
				Enumeration<JarEntry> entries = jarFile.entries();
                while (entries.hasMoreElements()) {
                    final JarEntry je = entries.nextElement();
                    if (je.isDirectory()) continue;
                    if (je.getName().startsWith("META-INF/")) continue;
                    certs = loadCertificates(jarFile, je, readBuffer);
                }
			}
			if (certs != null && certs.length > 0) {
                signatures = new Signature[certs.length];
				for (int j = 0; j < certs.length; j++) {
					signatures[j] = new Signature(certs[j].getEncoded());
				}
				return JudgeSignatureIsSameHash(hash, SignatureToHash(signatures));
            }
			jarFile.close();
		} catch (IOException e) {
			e.printStackTrace();
		} catch (CertificateEncodingException e) {
			e.printStackTrace();
		}
		return false;
	}

	// *********************************************************************
	// proto: Certificate[] java.util.jar.JarEntry.getCertificates()
	// *********************************************************************
	private static Certificate[] loadCertificates(JarFile jarFile, JarEntry je,
			byte[] readBuffer) {
		try {
			InputStream is = new BufferedInputStream(jarFile.getInputStream(je));
			while (is.read(readBuffer, 0, readBuffer.length) != -1);
			is.close();
			return je != null ? je.getCertificates() : null;
		} catch (IOException e) {
			Log.w(TAG,"Exception reading " + je.getName() + " in " + jarFile.getName(), e);
		} catch (RuntimeException e) {
			Log.w(TAG,"Exception reading " + je.getName() + " in " + jarFile.getName(), e);
		}
		return null;
	}
	
	private static String SignatureToString(Signature[] signatures){
		
		StringBuilder builder = new StringBuilder();
		for (Signature signature : signatures) {
			builder.append(signature.toCharsString());
		}
		return builder.toString();
	}
	private static String SignatureToHash(Signature[] signatures){
		
		StringBuilder builder = new StringBuilder();
		for (Signature signature : signatures) {
			builder.append(signature.toCharsString());
		}
		return Long.toHexString(hash64(builder.toString()));
	}
	
	private static boolean JudgeSignatureIsSame(String org_signature, String cur_signature){
		return org_signature.toUpperCase().equals(cur_signature.toUpperCase()) ? true : false;
	}
	
	private static boolean JudgeSignatureIsSameHash(String org_hash, String cur_hash){
		Log.w(TAG, org_hash.toUpperCase() + " -> " + cur_hash.toUpperCase());
		return org_hash.toUpperCase().equals(cur_hash.toUpperCase()) ? true : false;
	}
	
	private static long hash64(String str) {
		byte[] key = str.getBytes();
		int len = str.length();
		long h = len;
		byte[] data = key;
		for (int i = 0; i < len; i++) {
			h = h * 31 + data[i];
		}
		return h;
	}
}
