package com.lai.whiteboard.util;

import android.annotation.SuppressLint;
import android.content.Context;
import android.os.Build;
import android.os.Environment;
import android.os.storage.StorageManager;
import android.util.Log;
import android.os.Build.VERSION;

import java.io.File;
import java.lang.reflect.Method;

import static android.content.Context.STORAGE_SERVICE;

/**
 * @author Lai
 * @time 2021/3/4 14:29
 * @describe describe
 */
public class StorageHelper {
    private static final String TAG = StorageHelper.class.getSimpleName();
    private static String sInternalSdcardPath;

    private StorageHelper() {
    }

    @SuppressLint({"NewApi"})
    public static boolean isExternalStorageRemovable() {
        return VERSION.SDK_INT >= 9 ? Environment.isExternalStorageRemovable() : true;
    }

    public static boolean isExternalStorageMounted() {
        String state = Environment.getExternalStorageState();
        return "mounted".equals(state);
    }

    public static boolean isExternalStorageReadable() {
        String state = Environment.getExternalStorageState();
        return "mounted".equals(state) || "mounted_ro".equals(state);
    }

    public static File getExternalCacheDir(Context context) {
        File file = null;
        if (VERSION.SDK_INT >= 8) {
            file = context.getExternalCacheDir();
        }

        if (file == null) {
            file = new File(getInternalSdcardPath(context) + "/Android/data/" + context.getPackageName() + "/cache");
        }

        Log.d(TAG, "ExternalCacheDir : " + file.getAbsolutePath());
        return file;
    }

    public static File getInternalCacheDir(Context context) {
        return context.getCacheDir();
    }

    public static File getExternalFilesDir(Context context) {
        File file = null;
        if (VERSION.SDK_INT >= 8) {
            file = context.getExternalFilesDir((String)null);
        }

        if (file == null) {
            file = new File(getInternalSdcardPath(context) + "/Android/data/" + context.getPackageName() + "/files");
        }

        Log.d(TAG, "ExternalFilesDir : " + file.getAbsolutePath());
        return file;
    }

    public static File getInternalFilesDir(Context context) {
        return context.getFilesDir();
    }

    @SuppressLint({"NewApi"})
    public static StorageHelper.StorageVolumeRef[] getVolumeList(Context context) {
        try {
            StorageManager storageManager = (StorageManager)context.getSystemService(STORAGE_SERVICE);
            Class<?>[] paramClasses = new Class[0];
            Method getVolumePathsMethod = StorageManager.class.getMethod("getVolumeList", paramClasses);
            getVolumePathsMethod.setAccessible(true);
            Object[] emptyParams = new Object[0];
            Object storageVolumeArray = getVolumePathsMethod.invoke(storageManager, emptyParams);
            int length = ((Object[])((Object[])storageVolumeArray)).length;
            StorageHelper.StorageVolumeRef[] storageVolumeRefArray = new StorageHelper.StorageVolumeRef[length];
            Class<?> clazz = Class.forName("android.os.storage.StorageVolume");

            for(int i = 0; i < length; ++i) {
                Object storageVolumeObj = ((Object[])((Object[])storageVolumeArray))[i];
                Method method = clazz.getDeclaredMethod("getPath");
                if (!method.isAccessible()) {
                    method.setAccessible(true);
                }

                String path = (String)method.invoke(storageVolumeObj);
                method = clazz.getDeclaredMethod("isRemovable");
                if (!method.isAccessible()) {
                    method.setAccessible(true);
                }

                boolean removable = (Boolean)method.invoke(storageVolumeObj);
                storageVolumeRefArray[i] = new StorageHelper.StorageVolumeRef(path, removable);
            }

            return storageVolumeRefArray;
        } catch (Exception var14) {
            Log.e(TAG, TAG + " getStorageVolumeArray() failed.", var14);
        } catch (Error var15) {
            Log.e(TAG, TAG + " getStorageVolumeArray() failed.", var15);
        }

        return new StorageHelper.StorageVolumeRef[0];
    }

    public static String getInternalSdcardPath(Context context) {
        if (sInternalSdcardPath != null) {
            return sInternalSdcardPath;
        } else {
            String apiPath = Environment.getExternalStorageDirectory().getPath();
            if (VERSION.SDK_INT < 14) {
                Log.i(TAG, "External storage " + apiPath + " is " + Environment.getExternalStorageState());
                if (!isExternalStorageRemovable()) {
                    Log.w(TAG, TAG + " getInternalSdcardPath() : " + sInternalSdcardPath);
                    sInternalSdcardPath = apiPath;
                }

                return apiPath;
            } else {
                StorageHelper.StorageVolumeRef[] volumes = getVolumeList(context);
                if (volumes != null && volumes.length > 0) {
                    StorageHelper.StorageVolumeRef[] var3 = volumes;
                    int var4 = volumes.length;

                    for(int var5 = 0; var5 < var4; ++var5) {
                        StorageHelper.StorageVolumeRef svRef = var3[var5];
                        if (!svRef.removable) {
                            sInternalSdcardPath = svRef.path;
                            Log.w(TAG, TAG + " getInternalSdcardPath() : " + sInternalSdcardPath);
                            return sInternalSdcardPath;
                        }
                    }
                }

                Log.e(TAG, TAG + " getInternalSdcardPath() can NOT found a built-in volume.");
                return apiPath;
            }
        }
    }

    public static String getDCIMPath(Context context) {
        String path = null;
        File fileDCIM;
        if (VERSION.SDK_INT >= 8) {
            fileDCIM = Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DCIM);
            if (fileDCIM != null) {
                if (!fileDCIM.exists()) {
                    fileDCIM.mkdirs();
                }

                if (fileDCIM.canWrite()) {
                    path = fileDCIM.getAbsolutePath();
                }
            }
        }

        if (path == null) {
            path = getInternalSdcardPath(context) + "/" + Environment.DIRECTORY_DCIM;
            fileDCIM = new File(path);
            if (fileDCIM.exists()) {
                fileDCIM.mkdirs();
            }
        }

        Log.d(TAG, TAG + " getDCIMPath() : " + path);
        return path;
    }

    public static String getCameraPath(Context context) {
        String dcim = getDCIMPath(context);
        String sdcard;
        if (!"HTC".equals(Build.BRAND.toUpperCase()) && !"HTC".equals(Build.MANUFACTURER.toUpperCase())) {
            if (!"SONY".equals(Build.BRAND.toUpperCase()) && !"SONY".equals(Build.MANUFACTURER.toUpperCase())) {
                if ("MEIZU".equals(Build.BRAND.toUpperCase()) || "MEIZU".equals(Build.MANUFACTURER.toUpperCase())) {
                    return dcim;
                }

                if (!"VIVO".equals(Build.BRAND.toUpperCase()) && !"VIVO".equals(Build.MANUFACTURER.toUpperCase())) {
                    if (("LENOVO".equals(Build.BRAND.toUpperCase()) || "LENOVO".equals(Build.MANUFACTURER.toUpperCase())) && Build.MODEL.toUpperCase().contains("PHAB2 Pro")) {
                        return dcim;
                    }
                } else {
                    sdcard = dcim.replace("/" + Environment.DIRECTORY_DCIM, "");
                    String cameraPath = sdcard + "/相机";
                    if ((new File(cameraPath)).exists()) {
                        return cameraPath;
                    }
                }
            } else {
                sdcard = dcim + "/100ANDRO";
                if ((new File(sdcard)).exists()) {
                    return sdcard;
                }
            }
        } else {
            sdcard = dcim + "/100MEDIA";
            if ((new File(sdcard)).exists()) {
                return sdcard;
            }
        }

        sdcard = dcim + "/Camera";
        File file = new File(sdcard);
        if (!file.exists()) {
            file.mkdirs();
        }

        return sdcard;
    }

    public static String getDownloadPath(Context context) {
        String path = null;
        File fileDCIM;
        if (Build.VERSION.SDK_INT >= 8) {
            fileDCIM = Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DOWNLOADS);
            if (fileDCIM != null) {
                if (!fileDCIM.exists()) {
                    fileDCIM.mkdirs();
                }

                if (fileDCIM.canWrite()) {
                    path = fileDCIM.getAbsolutePath();
                }
            }
        }

        if (path == null) {
            path = getInternalSdcardPath(context) + "/" + Environment.DIRECTORY_DOWNLOADS;
            fileDCIM = new File(path);
            if (fileDCIM.exists()) {
                fileDCIM.mkdirs();
            }
        }

        Log.d(TAG, TAG + " getDownloadPath() : " + path);
        return path;
    }

    public static class StorageVolumeRef {
        public final String path;
        public final boolean removable;

        private StorageVolumeRef(String path, boolean removable) {
            if (path == null) {
                throw new IllegalArgumentException("path can not be null.");
            } else {
                this.path = path;
                this.removable = removable;
            }
        }

        public boolean equals(Object o) {
            if (o == null) {
                return false;
            } else if (o instanceof StorageHelper.StorageVolumeRef) {
                StorageHelper.StorageVolumeRef that = (StorageHelper.StorageVolumeRef)o;
                return this.path.equals(that.path);
            } else {
                return false;
            }
        }

        public String toString() {
            return "StorageVolumeRef[path=" + this.path + ",removable=" + this.removable + "]";
        }
    }
}
