package com.lai.whiteboard;

import android.graphics.Bitmap;
import android.view.Surface;

/**
 * @author Lai
 * @time 2021/1/22 23:17
 * @describe describe
 */
public class ShaderNative {
    static {
        System.loadLibrary("native-lib");
    }

    public static native String stringFromJNI();

    public static native void init(int width, int height, Surface surface, Bitmap defaultPaintTexture);

    public static native void glDrawPaint(float[] drawPoint,int vertexCount);

    public static native void onDestroy();

    public static native void glClearPaintColor();



}
