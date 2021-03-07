package com.lai.whiteboard.pen

import android.graphics.PointF
import android.view.MotionEvent
import android.view.View
import com.lai.whiteboard.pen.page.BrushInfoConfig

/**
 * @author: Lai
 * @createDate: 2020-04-24 15:43
 * @description:
 */
interface IPen {
    fun downEvent(pagerView: View, coordinate:PointF,event: MotionEvent)
    fun moveEvent(pagerView: View, coordinate:PointF,event: MotionEvent)
    fun upEvent(pagerView: View, coordinate:PointF,event: MotionEvent)
    fun getDrawPoints(): ArrayList<Float>
    fun drawPoints(points: FloatArray, textureRotate: Float)

    fun rest()
    fun copy(brushInfoConfig: BrushInfoConfig): IPen
    fun getConfig(): BrushInfoConfig?
}