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

    public static native void init(int width, int height, Surface surface, Bitmap defaultPaintTexture, ICallBack test);

    public static native void glDrawPaint(float[] drawPoint, int vertexCount, float textureRotate);

    public static native void glResultMatrix(float[] matrixValue);


    public static native void glResult(float r, float restR, float dx, float dy, float sc);


    public static native void glSetPaintTexture(Bitmap defaultPaintTexture, boolean isTextureRotate, float width, int outType);


    public static native void glDrawData(float[] drawPoint,
                                         int vertexCount,
                                         Bitmap defaultPaintTexture,
                                         float width,
                                         int outType,
                                         boolean isTextureRotate,
                                         float A,
                                         float R,
                                         float G,
                                         float B, boolean isClear, boolean isDisplay);

    public static native void glPaintColor(float a, float r, float g, float b);


    public static native void glTranslate(float dx, float dy);

    public static native void glScale(float s);

    public static native void glRotate(float r);

    public static native void onDestroy();

    public static native void glClearAll();

    public static native void glClearPaintColor();

    public static native void glInit(int width, int height, Bitmap bitmap);

    public static native void glTestDraw();

    public static native void glTestDrawR(float x, float y, float z);

    public static native void glSave(String path, ICallBack back);

    public interface ICallBack {
        void voidCallBack();
    }


}
