package com.lai.whiteboard.util

import android.graphics.PointF
import android.util.Log
import java.util.*
import kotlin.math.abs
import kotlin.math.floor
import kotlin.math.pow
import kotlin.math.sqrt


/**
 *
 * @author  Lai
 *
 * @time 2021/1/10 1:16
 * @describe describe
 *
 */
object PointUtils {
    fun isCenter(centerPoint: PointF, fromPoint: PointF, toPoint: PointF): Boolean {
        val isCenterX = abs((fromPoint.x + toPoint.x) / 2.0 - centerPoint.x) < 0.0001
        val isCenterY = abs((fromPoint.y + toPoint.y) / 2.0 - centerPoint.y) < 0.0001
        return isCenterX && isCenterY
    }


    fun createBesselCurvePoints2(
        from: PointF,
        to: PointF,
        control: PointF,
        pointSize: Int = 40
    ): ArrayList<PointF> {
        val list = ArrayList<PointF>();
        var t = 0.0


        while (t <= 1.0) {
            val x = (1f - t) * (1f - t) * from.x + 2f * (1f - t) * t * control.x + t * t * to.x;
            val y = (1f - t) * (1f - t) * from.y + 2f * (1f - t) * t * control.y + t * t * to.y;
            list.add(PointF(x.toFloat(), y.toFloat()))
            t += 0.05
        }
        return list
    }

    fun createBesselCurvePoints(
        from: PointF,
        to: PointF,
        control: PointF,
        pointSize: Float = 40f
    ): ArrayList<PointF> {
        val list = ArrayList<PointF>();
        val segmentDistance = 5f / pointSize //设置线段的距离为1px.

        val distance: Float = getDistanceTo(from, to) // 计算两个中间点的距离
        val numberOfSegments =
            48.0.coerceAtMost(floor(distance / segmentDistance.toDouble()).coerceAtLeast(24.0))
                .toInt() // 计算可以分成多少段

        var t = 0.0f
        val step = 1.0f / numberOfSegments.toFloat()
        for (j in 0 until numberOfSegments) {
            val x = (1f - t) * (1f - t) * from.x + 2f * (1f - t) * t * control.x + t * t * to.x;
            val y = (1f - t) * (1f - t) * from.y + 2f * (1f - t) * t * control.y + t * t * to.y;
            list.add(PointF(x, y))
            t += step
        }

        return list
    }


    private fun smoothPoint(
        midPoint1: PointF,
        midPoint2: PointF,
        prev1: PointF,
        t: Float
    ): PointF {
        val a1 = Math.pow(1.0f - t.toDouble(), 2.0)
        val a2 = (2.0f * (1.0f - t) * t)
        val a3 = t * t
        return PointF(
            (midPoint1.x * a1 + prev1.x * a2 + midPoint2.x * a3).toFloat(),
            (midPoint1.y * a1 + prev1.y * a2 + midPoint2.y * a3).toFloat()
        )
    }

    fun getDistanceTo(curPoint: PointF, point: PointF): Float {
        return sqrt(
            (curPoint.x * 1.0 - point.x * 1.0).pow(2.0) + (curPoint.y * 1.0 - point.y * 1.0).pow(2.0)
        ).toFloat()
    }

    fun getDistanceTo(currFloatX:Float,currFloatY:Float,toFloatX:Float,toFloatY:Float): Float {
        return sqrt(
            (currFloatX * 1.0 - toFloatX * 1.0).pow(2.0) + (currFloatY * 1.0 - toFloatY * 1.0).pow(2.0)
        ).toFloat()
    }

    // 归一化顶点坐标
    fun vertexWithPoint(x: Float, y: Float, frameWidth: Int, frameHeight: Int): FloatArray {
        val openglX = (x / frameWidth.toFloat()) * 2f - 1f
        val openglY = 1f - (y / frameHeight.toFloat()) * 2f
        return floatArrayOf(openglX, openglY)
    }

    fun multiplySum(
        point: PointF,
        point2: PointF,
        scalar: Float
    ): PointF {
        return PointF(
            (point.x + point2.x) * scalar,
            (point.y + point2.y) * scalar
        )
    }


}