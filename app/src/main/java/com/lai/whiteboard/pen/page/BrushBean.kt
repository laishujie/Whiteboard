package com.lai.whiteboard.pen.page

import android.content.res.Resources
import android.graphics.Bitmap
import android.graphics.BitmapFactory
import android.graphics.Color
import com.lai.whiteboard.R
import com.lai.whiteboard.pen.TexturePen

/**
 *
 * @author  Lai
 *
 * @time 2021/2/25 14:00
 * @describe describe
 *
 */
data class BrushRes(val id: String, val resBrush: Int, var isRotate: Boolean = true)

data class BrushInfoConfig(
    var res: BrushRes?,
    var brushWidth: Float,
    var outType: Int,
    var points: FloatArray? = null,
    var vertexCount: Int = 0,
    var currColorInt: Int = Color.BLACK
)

object BrushManager {

    fun getDefaultBrushInfoConfig(): BrushInfoConfig {
        return BrushInfoConfig(BrushManager.getDefaultBrushRes(), 15f, TexturePen.DRAW)
    }

    /**
     * 是否是同一个id
     */
    fun isAgainSetTexture(id: String?, info: BrushInfoConfig): BrushRes? {
        return if (id == info.res?.id) {
            null
        } else {
            info.res
        }
    }

    private fun getDefaultBrushRes(): BrushRes {
        return BrushRes("1", R.drawable.brush_paint_normal_128)
    }

    fun getBrushList(): MutableList<BrushRes> {
        return mutableListOf(
            getDefaultBrushRes(),
            BrushRes("2", R.drawable.brush_paint_crayon2_128),
            BrushRes("3", R.drawable.brush_paint_marker_128, false),
            BrushRes("4", R.drawable.brush_paint_blur_128),
            BrushRes("5", R.drawable.brush_paint_crayon_128)
        )
    }

    fun getBrushBitmap(resources: Resources, brushRes: BrushRes?): Bitmap? {
        brushRes?.apply {
            return BitmapFactory.decodeResource(resources, brushRes.resBrush)
        }
        return null
    }


}
