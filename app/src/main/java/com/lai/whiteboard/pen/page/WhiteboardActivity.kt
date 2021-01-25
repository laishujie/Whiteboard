package com.lai.whiteboard.pen.page

import android.graphics.BitmapFactory
import android.graphics.SurfaceTexture
import android.os.Bundle
import android.util.Log
import android.view.Surface
import android.view.TextureView
import androidx.appcompat.app.AppCompatActivity
import com.lai.whiteboard.R
import com.lai.whiteboard.ShaderNative
import kotlinx.android.synthetic.main.activity_whiteboard.*

class WhiteboardActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_whiteboard)

        white_view.surfaceTextureListener = object : TextureView.SurfaceTextureListener {
            // SurfaceTexture 初始化好的时候调用
            override fun onSurfaceTextureAvailable(
                surface: SurfaceTexture?,
                width: Int,
                height: Int
            ) {
                Log.e("11111", "onSurfaceTextureAvailable")

                if (surface == null) return
                ShaderNative.init(
                    width, height, Surface(surface),
                    BitmapFactory.decodeResource(resources, R.mipmap.brush_paint_marker_64)
                )
            }

            override fun onSurfaceTextureSizeChanged(
                surface: SurfaceTexture?,
                width: Int,
                height: Int
            ) {
                Log.e("11111", "onSurfaceTextureSizeChanged")
            }

            override fun onSurfaceTextureUpdated(surface: SurfaceTexture?) {
                Log.e("11111", "onSurfaceTextureUpdated")
            }

            override fun onSurfaceTextureDestroyed(surface: SurfaceTexture?): Boolean {
                Log.e("11111", "onSurfaceTextureDestroyed")
                ShaderNative.onDestroy()
                return true
            }
        }
        reset.setOnClickListener {
            white_view.glClearPaintColor()
        }
        btn_redo.setOnClickListener {
            white_view.redo()
        }
        btn_undo.setOnClickListener {
            white_view.undo()
        }
    }

    override fun onDestroy() {
        super.onDestroy()
        ShaderNative.onDestroy()
    }
}
