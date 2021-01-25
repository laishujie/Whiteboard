package com.lai.whiteboard.pen

import android.view.MotionEvent
import android.view.View

/**
 * @author: Lai
 * @createDate: 2020-04-24 15:43
 * @description:
 */
interface IPen {
    fun downEvent(pagerView: View, event: MotionEvent)
    fun moveEvent(pagerView: View, event: MotionEvent)
    fun upEvent(pagerView: View,event: MotionEvent)
    fun getDrawPoints(): ArrayList<Float>
    fun drawPoints(points: FloatArray)
    fun rest()
    fun copy(): IPen
}